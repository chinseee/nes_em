#include "nes_em/ppu.h"

namespace nes_em {

PPU::PPU() {

}

void PPU::cycle() {



    if (dot == 0) {
        return;
    }
    else if (dot <= 256) {
        switch (dot % 8) {
        case 2:
            tile_addr = vram[0x2000 | (v & 0x0fff)];
            break;
        case 4:
            uint8_t attr_addr = 0x23c0 | (v & 0x0c00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
            uint8_t attr = vram[attr_addr];
            break;
        case 6:
            bg_ptrn_lo = vram[tile_addr];
        case 0:
            bg_attr_hi = vram[tile_addr + 8];
            break;
        }
    }
    else if (dot <= 320) {

    }
    else if (dot <= 336) {

    }
    else {

    }
}

uint8_t PPU::mem_read(uint16_t addr) {
    switch (addr & 0x1f) {
    case 0x2:
        w = false;
        return 0; // TODO: FIX THIS
    case 0x4:
        return oam_data;
    case 0x7:
        // TODO: add v inc when reading/writing 0x2007
        return ppu_data;
    }
}

void PPU::mem_write(uint16_t addr, uint8_t value) {
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
        oam_data = value;
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

            // TODO: wait "1 - 1.5 dots" after write to copy t into v
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
        ppu_data = value;
        break;
    case 0x14:
        oam_dma = value;
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
        else if ((v & 0x03e0) == 0x03a) {
            v &= 0x7c1f;
            v ^= 0x0800;
        }
        else {
            v &= 0x7fff;
            v += 0x0020;
        }
    }
}


}