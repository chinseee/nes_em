#pragma once
#include "fwd.h"
#include <cstdint>

namespace nes_em {

class Bus {
public:
    // devices
    uint8_t cpu_ram[0x0800];

    uint8_t vram[0x0800];
    uint8_t palette_idxs[0x20];
    
    PPU* ppu;
    Cartridge* cart;

    // io
    uint8_t cpu_data;
    uint8_t ppu_data;

    void reset();

    uint8_t cpu_read(uint16_t);
    void cpu_write(uint16_t, uint8_t);

    uint16_t unmirror_ppu_addr(uint16_t);

    uint8_t ppu_read(uint16_t);
    void ppu_write(uint16_t, uint8_t);
};
}