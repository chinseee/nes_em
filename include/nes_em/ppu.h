#pragma once
#include "nes_em/fwd.h"
#include "nes_em/nes_file.h"
#include "nes_em/listeners.h"
#include <cstdint>
#include <bitset>
#include <vector>

namespace nes_em {

class PPU {
public:
    // ppuctrl ($2000) bit indices
    static constexpr size_t BASE_NAMETABLE_BIT = 0; // 2 bits: 0-1
    static constexpr size_t VRAM_INC_BIT = 2;
    static constexpr size_t SPRITE_PTRN_TABLE_BIT = 3;
    static constexpr size_t BG_PTRN_TABLE_BIT = 4;
    static constexpr size_t SPRITE_HEIGHT_BIT = 5;
    static constexpr size_t PPU_MASTER_SLAVE_BIT = 6;
    static constexpr size_t NMI_ENABLE_BIT = 7;

    // ppumask ($2001) bit indices
    static constexpr size_t GREYSCALE_BIT = 0;
    static constexpr size_t SHOW_BG_LEFT_BIT = 1;
    static constexpr size_t SHOW_SPRITE_LEFT_BIT = 2;
    static constexpr size_t SHOW_BG_BIT = 3;
    static constexpr size_t SHOW_SPRITE_BIT = 4;
    static constexpr size_t EMPHASIZE_RED_BIT = 5;
    static constexpr size_t EMPHASIZE_GREEN_BIT = 6;
    static constexpr size_t EMPHASIZE_BLUE_BIT = 7;

    // ppustatus ($2002) bit indices
    static constexpr size_t SPRITE_OVERFLOW_BIT = 5;
    static constexpr size_t SPRITE_ZERO_HIT_BIT = 6;
    static constexpr size_t VBLANK_BIT = 7;

    // connections
    CPU* cpu;
    Bus* bus;

    uint8_t oam[0x100];
    uint8_t secondary_oam[0x20];

    // memory-mapped regs
    std::bitset<8> ppuctrl, ppumask, ppustatus;
    uint8_t oam_addr, oam_data, x_scroll, y_scroll;
    uint16_t ppu_addr;
    uint8_t ppu_data, oam_dma;
    uint8_t ppu_data_buffer; // internal read buffer behind $2007
    uint8_t io_bus; // last value driven onto the ppu data bus (open-bus reads)

    // internal regs
    uint16_t v, t;
    uint8_t x;
    bool w, odd_frame;

    // background rendering pipeline
    uint8_t nt_latch, attr_latch, bg_ptrn_lo_latch, bg_ptrn_hi_latch;
    uint16_t bg_ptrn_lo, bg_ptrn_hi;
    uint16_t bg_attr_lo, bg_attr_hi;

    // sprite rendering pipeline
    uint8_t sprite_ptrn_lo[8], sprite_ptrn_hi[8];
    uint8_t sprite_attr[8];
    uint8_t sprite_x[8];
    uint8_t sprite_count;
    bool sprite_zero_on_line, sprite_zero_hit_possible;

    // states for emulation
    uint16_t scanline, dot;

    // output
    std::vector<PPUListener*> listeners;
    uint8_t* output;

    PPU();

    void reset();
    void cycle();

    uint8_t cpu_read(uint16_t);
    void cpu_write(uint16_t, uint8_t);

    void coarse_x_inc();
    void y_inc();

    void pre_render_scanline();
    uint8_t visible_scanline();
    void post_render_scanline();
    void vblank_scanline();
    void bg_fetch_cycle();
    void evaluate_sprites();
    void fetch_sprites();
    uint8_t render_pixel();

    void add_listener(PPUListener*);
};



}