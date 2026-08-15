#pragma once
#include "nes_em/cartridge/cartridge.h"
#include <cstdint>

namespace nes_em {

class NROMCart: public Cartridge {
public:
    NROMCart(const NesFile&);
    uint8_t chr_rom[0x2000];
    uint8_t prg_ram[0x2000];
    uint8_t prg_rom[0x8000];

    virtual uint8_t cpu_read(uint16_t);
    virtual void cpu_write(uint16_t, uint8_t);
    virtual uint8_t ppu_read(uint16_t);
    virtual void ppu_write(uint16_t, uint8_t);
};

}