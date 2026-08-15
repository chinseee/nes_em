#include "nes_em/ppu.h"
#include "nes_em/cpu.h"
#include "nes_em/bus.h"
#include "nes_em/listeners.h"

#include <iostream>
#include <cstring>

namespace nes_em {

PPU::PPU() {

}

void PPU::reset() {
    ppuctrl = 0;
    ppumask = 0;

    w = false;
    ppu_data = 0;
    odd_frame = false;
}

void PPU::cycle() {
    uint8_t pixel = 0;

    switch (scanline) {
    case 261:
        pre_render_scanline();
        break;
    case 240:
        post_render_scanline();
        break;
    default:
        if (scanline <= 239)
            pixel = visible_scanline();
        else
            vblank_scanline();
        break;
    }

    cpu->nmi_line = ppustatus[VBLANK_BIT] && ppuctrl[NMI_ENABLE_BIT];
    
    if (output)
        output[scanline * 341 + dot] = pixel;

    for (PPUListener* listener : listeners)
        listener->on_ppu_cycle(*this);

    ++dot;
    if (dot > 340) {
        dot = 0;
        ++scanline;
        if (scanline > 261) {
            scanline = 0;
            odd_frame = !odd_frame;
        }

        // odd frames are one dot shorter: skip the idle cycle at (0,0)
        if (scanline == 0 && odd_frame && (ppumask[SHOW_BG_BIT] || ppumask[SHOW_SPRITE_BIT]))
            dot = 1;
    }
}


void PPU::pre_render_scanline() {
    if (dot == 1) {
        ppustatus[VBLANK_BIT] = false;
        ppustatus[SPRITE_ZERO_HIT_BIT] = false;
        ppustatus[SPRITE_OVERFLOW_BIT] = false;
    }

    bool rendering_enabled = ppumask[SHOW_BG_BIT] || ppumask[SHOW_SPRITE_BIT];
    if (!rendering_enabled)
        return;

    bool fetching = (dot >= 1 && dot <= 256) || (dot >= 321 && dot <= 336);
    if (fetching)
        bg_fetch_cycle();

    if (dot == 256)
        y_inc();

    if (dot == 257)
        v = (v & 0x7be0) | (t & 0x041f); // copy horizontal bits from t

    if (dot >= 280 && dot <= 304)
        v = (v & 0x041f) | (t & 0x7be0); // copy vertical bits from t
}

void PPU::post_render_scanline() {
    // idle: the PPU makes no memory accesses and renders nothing on this line
}

void PPU::vblank_scanline() {
    if (scanline == 241 && dot == 1) {
        ppustatus[VBLANK_BIT] = true;
    }
}

uint8_t PPU::visible_scanline() {
    bool rendering_enabled = ppumask[SHOW_BG_BIT] || ppumask[SHOW_SPRITE_BIT];

    if (dot >= 1 && dot <= 256) {
        // sprites for this scanline were already fetched during the previous
        // scanline's dots 257-320; evaluate now for the *next* scanline
        if (dot == 1 && rendering_enabled)
            evaluate_sprites();

        if (rendering_enabled)
            bg_fetch_cycle();

        if (dot == 256 && rendering_enabled)
            y_inc();

        return render_pixel();
    }

    if (dot == 257 && rendering_enabled) {
        v = (v & 0x7be0) | (t & 0x041f); // copy horizontal bits from t
        fetch_sprites();
    }

    if (dot >= 321 && dot <= 336 && rendering_enabled)
        bg_fetch_cycle();

    // dots 257-320, 337-340 carry no visible pixel
    return bus->ppu_read(0x3f00);
}

void PPU::evaluate_sprites() {
    for (uint8_t& b : secondary_oam)
        b = 0xff;

    uint8_t sprite_height = ppuctrl[SPRITE_HEIGHT_BIT] ? 16 : 8;
    int target_scanline = scanline + 1;

    sprite_count = 0;
    sprite_zero_on_line = false;

    for (uint8_t n = 0; n < 64; ++n) {
        uint8_t sprite_y = oam[n * 4];
        int diff = target_scanline - sprite_y;

        if (diff < 0 || diff >= sprite_height)
            continue;

        if (sprite_count < 8) {
            for (uint8_t b = 0; b < 4; ++b)
                secondary_oam[sprite_count * 4 + b] = oam[n * 4 + b];

            if (n == 0)
                sprite_zero_on_line = true;

            ++sprite_count;
        }
        else {
            // NB: doesn't reproduce hardware's buggy overflow evaluation, just the flag
            ppustatus[SPRITE_OVERFLOW_BIT] = true;
            break;
        }
    }
}

void PPU::fetch_sprites() {
    uint8_t sprite_height = ppuctrl[SPRITE_HEIGHT_BIT] ? 16 : 8;

    for (uint8_t i = 0; i < sprite_count; ++i) {
        uint8_t sprite_y = secondary_oam[i * 4];
        uint8_t tile = secondary_oam[i * 4 + 1];
        uint8_t attr = secondary_oam[i * 4 + 2];

        int row = (scanline + 1) - sprite_y;
        if (attr & 0x80) // vertical flip
            row = sprite_height - 1 - row;

        uint16_t pattern_addr;
        if (sprite_height == 16) {
            uint16_t table = (tile & 0x1) ? 0x1000 : 0x0000;
            uint8_t tile_index = tile & 0xfe;
            if (row >= 8) {
                ++tile_index;
                row -= 8;
            }
            pattern_addr = table + (tile_index << 4) + row;
        }
        else {
            uint16_t table = ppuctrl[SPRITE_PTRN_TABLE_BIT] ? 0x1000 : 0x0000;
            pattern_addr = table + (tile << 4) + row;
        }

        sprite_ptrn_lo[i] = bus->ppu_read(pattern_addr);
        sprite_ptrn_hi[i] = bus->ppu_read(pattern_addr + 8);
        sprite_attr[i] = attr;
        sprite_x[i] = secondary_oam[i * 4 + 3];
    }
}

uint8_t PPU::render_pixel() {
    bool show_bg = ppumask[SHOW_BG_BIT];
    bool show_sprites = ppumask[SHOW_SPRITE_BIT];
    uint16_t x_pos = dot - 1;

    if (!show_bg && !show_sprites)
        return bus->ppu_read(0x3f00);

    uint8_t bg_pixel = 0, bg_palette = 0;
    if (show_bg && !(x_pos < 8 && !ppumask[SHOW_BG_LEFT_BIT])) {
        uint16_t mask = 0x8000 >> x;
        bg_pixel = ((bg_ptrn_hi & mask) ? 2 : 0) | ((bg_ptrn_lo & mask) ? 1 : 0);
        bg_palette = ((bg_attr_hi & mask) ? 2 : 0) | ((bg_attr_lo & mask) ? 1 : 0);
    }

    uint8_t sprite_pixel = 0, sprite_palette = 0;
    bool sprite_priority = false; // true = behind background
    bool sprite_is_zero = false;

    if (show_sprites && !(x_pos < 8 && !ppumask[SHOW_SPRITE_LEFT_BIT])) {
        for (uint8_t i = 0; i < sprite_count; ++i) {
            if (x_pos < sprite_x[i] || x_pos >= sprite_x[i] + 8)
                continue;

            uint8_t col = x_pos - sprite_x[i];
            if (sprite_attr[i] & 0x40) // horizontal flip
                col = 7 - col;

            uint8_t bit = 7 - col;
            uint8_t pixel = (((sprite_ptrn_hi[i] >> bit) & 1) << 1) | ((sprite_ptrn_lo[i] >> bit) & 1);

            if (pixel == 0)
                continue; // transparent, fall through to the next (lower priority) sprite

            sprite_pixel = pixel;
            sprite_palette = sprite_attr[i] & 0x3;
            sprite_priority = sprite_attr[i] & 0x20;
            sprite_is_zero = (i == 0) && sprite_zero_on_line;
            break;
        }
    }

    if (sprite_is_zero && bg_pixel != 0 && sprite_pixel != 0 && x_pos != 255)
        ppustatus[SPRITE_ZERO_HIT_BIT] = true;

    bool use_sprite = sprite_pixel != 0 && (bg_pixel == 0 || !sprite_priority);
    if (use_sprite)
        return bus->ppu_read(0x3f10 + sprite_palette * 4 + sprite_pixel);
    if (bg_pixel != 0)
        return bus->ppu_read(0x3f00 + bg_palette * 4 + bg_pixel);
    return bus->ppu_read(0x3f00);
}

void PPU::bg_fetch_cycle() {
    bg_ptrn_lo <<= 1;
    bg_ptrn_hi <<= 1;
    bg_attr_lo <<= 1;
    bg_attr_hi <<= 1;

    switch (dot % 8) {
    case 1:
        nt_latch = bus->ppu_read(0x2000 | (v & 0x0fff));
        break;
    case 3:
        attr_latch = bus->ppu_read(0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07));
        break;
    case 5: {
        uint16_t pattern_base = ppuctrl[BG_PTRN_TABLE_BIT] ? 0x1000 : 0x0000;
        bg_ptrn_lo_latch = bus->ppu_read(pattern_base + (nt_latch << 4) + ((v >> 12) & 0x7));
        break;
    }
    case 7: {
        uint16_t pattern_base = ppuctrl[BG_PTRN_TABLE_BIT] ? 0x1000 : 0x0000;
        bg_ptrn_hi_latch = bus->ppu_read(pattern_base + (nt_latch << 4) + ((v >> 12) & 0x7) + 8);
        break;
    }
    case 0: {
        bg_ptrn_lo |= bg_ptrn_lo_latch;
        bg_ptrn_hi |= bg_ptrn_hi_latch;

        // select the 2-bit palette index for this tile out of the fetched attribute byte
        uint8_t attr_shift = ((v >> 4) & 0x04) | (v & 0x02);
        uint8_t attr_bits = (attr_latch >> attr_shift) & 0x3;
        bg_attr_lo |= (attr_bits & 0x1) ? 0xff : 0x00;
        bg_attr_hi |= (attr_bits & 0x2) ? 0xff : 0x00;

        coarse_x_inc();
        break;
    }
    }
}

uint8_t PPU::cpu_read(uint16_t addr) {
    switch (addr & 0x1f) {
    case 0x2: {
        // top 3 bits come from ppustatus, bottom 5 are stale bus contents
        uint8_t status_mask = (1 << VBLANK_BIT) | (1 << SPRITE_ZERO_HIT_BIT) | (1 << SPRITE_OVERFLOW_BIT);
        uint8_t value = (ppustatus.to_ulong() & status_mask) | (io_bus & 0x1f);

        ppustatus[VBLANK_BIT] = false; // reading clears the vblank flag
        w = false;

        io_bus = value;
        return value;
    }
    case 0x4: {
        uint8_t value = oam[oam_addr];
        if ((oam_addr & 0x3) == 0x2)
            value &= 0xe3; // attribute byte bits 2-4 are unimplemented, always read as 0

        io_bus = value;
        return value;
    }
    case 0x7: {
        uint16_t vram_addr = v & 0x3fff;
        uint8_t value;

        if (vram_addr >= 0x3f00) {
            // palette reads are not delayed by the internal buffer
            value = bus->ppu_read(vram_addr);
            ppu_data_buffer = bus->ppu_read(vram_addr - 0x1000);
        }
        else {
            value = ppu_data_buffer;
            ppu_data_buffer = bus->ppu_read(vram_addr);
        }

        v += ppuctrl[VRAM_INC_BIT] ? 32 : 1;

        ppu_data = value;
        io_bus = value;
        return value;
    }
    default:
        // write-only registers return whatever was last driven on the bus
        // TODO: remove ppu's io_bus member, add open_read member and move open bus behavior to Bus class
        return io_bus;
    }
}

void PPU::cpu_write(uint16_t addr, uint8_t value) {
    io_bus = value;

    switch (addr & 0x1f) {
    case 0x0:
        ppuctrl = value;

        t &= 0x73ff;
        t |= (value & 0x3) << 10;

        break;
    case 0x1:
        ppumask = value;
        break;
    case 0x3:
        oam_addr = value;
        break;
    case 0x4:
        oam[oam_addr] = value;
        ++oam_addr;
        break;
    case 0x5:
        if (w) {
            y_scroll = value;

            t &= 0x0c1f;
            t |= (value >> 3) << 5;
            t |= (value & 0x7) << 12;
        }
        else {
            x_scroll = value;

            t &= 0x7fe0;
            t |= value >> 3;
            x = value & 0x7;
        }

        w = !w;
        break;
    case 0x6:
        if (w) {
            ppu_addr &= 0xff00;
            ppu_addr |= value;

            t &= 0x7f00;
            t |= value;

            v = t; // simplified: real hardware copies t into v ~1-1.5 dots later
        }
        else {
            ppu_addr &= 0x00ff;
            ppu_addr |= value << 8;

            t &= 0x00ff;
            t |= (value & 0x3f) << 8;
        }
        w = !w;
        break;
    case 0x7:
        bus->ppu_write(v & 0x3fff, value);

        
        v += ppuctrl[VRAM_INC_BIT] ? 32 : 1;
        break;
    case 0x14:
        oam_dma = value;
        // actual 256-byte transfer from CPU memory is driven by the CPU
        break;
    }
}

void PPU::coarse_x_inc() {
    if ((v & 0x1f) == 0x1f) {
        v &= 0x7fe0;
        v ^= 0x0400;
    }
    else
        ++v;
}

void PPU::y_inc() {
    v += 0x1000;
    if (v & 0x8000) {
        if ((v & 0x03e0) == 0x03e0) {
            v &= 0x7c1f;
        }
        else if ((v & 0x03e0) == 0x03a0) {
            v &= 0x7c1f;
            v ^= 0x0800;
        }
        else {
            v &= 0x7fff;
            v += 0x0020;
        }
    }
}

void PPU::add_listener(PPUListener* listener) {
    listeners.push_back(listener);
}


}