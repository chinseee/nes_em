#pragma once
#include <cstdint>
#include <bitset>

namespace nes_em {

class PPU {
public:
    static constexpr size_t VRAM_INC_BIT = 2;

    // ppu memory
    uint8_t vram[0x4000];
    uint8_t oam[0x100];

    // memory-mapped regs
    std::bitset<8> ppuctrl, ppumask;
    uint8_t oam_addr, oam_data, x_scroll, y_scroll;
    uint16_t ppu_addr;
    uint8_t ppu_data, oam_dma;

    // internal regs
    uint16_t v, t;
    uint8_t x;
    bool w, odd_frame;

    uint16_t bg_ptrn_lo, bg_ptrn_hi;
    bool bg_attr_lo, bg_attr_hi;

    // states for emulation
    uint16_t scanline, dot;
    uint8_t tile_addr;

    // output
    uint8_t* output;

    PPU();

    void reset();
    void cycle();

    uint8_t mem_read(uint16_t);
    void mem_write(uint16_t, uint8_t);

    void coarse_x_inc();
    void y_inc();
};
}