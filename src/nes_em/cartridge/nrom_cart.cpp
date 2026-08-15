#include "nes_em/cartridge/nrom_cart.h"
#include <cstring>
#include <iostream>

namespace nes_em {

NROMCart::NROMCart(const NesFile& file) {
    mirroring = file.mirroring;
    std::memcpy(chr_rom, file.chr_rom.data(), 0x2000);
    std::memcpy(prg_rom, file.prg_rom.data(), 0x4000);
    if (file.prg_rom.size() <= 0x4000)
        std::memcpy(prg_rom + 0x4000, file.prg_rom.data(), 0x4000);
}

uint8_t NROMCart::cpu_read(uint16_t addr) {
    open_cpu_read = false;
    if (addr >= 0x6000) {
        if (addr < 0x8000)
            return prg_ram[addr - 0x6000];
        return prg_rom[addr & 0x7fff];
    }
    open_cpu_read = true;
    return 0;
}

void NROMCart::cpu_write(uint16_t addr, uint8_t value) {
    if (addr >= 0x6000 && addr < 0x8000)
        prg_ram[addr - 0x6000] = value;
}

uint8_t NROMCart::ppu_read(uint16_t addr) {
    open_ppu_read = false;
    if (addr < 0x2000)
        return chr_rom[addr];
    open_ppu_read = true;
    return 0;
}

void NROMCart::ppu_write(uint16_t, uint8_t) {
    return;
}

}