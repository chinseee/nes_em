#pragma once
#include "nes_em/fwd.h"
#include "nes_em/cartridge/cartridge.h"
#include <cstdint>

namespace nes_em {

class NROMCart: public Cartridge {
    uint8_t chr_rom[0x2000];
    uint8_t prg_ram[0x2000];
    uint8_t prg_rom[0x8000];
public:
    NROMCart(const NesFile&);
    
    BusRead cpu_read(uint16_t);
    void cpu_write(uint16_t, uint8_t);
    BusRead  ppu_read(uint16_t);
    void ppu_write(uint16_t, uint8_t);
};

}