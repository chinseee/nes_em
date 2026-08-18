#include "nes_em/cartridge/nrom_cart.h"
#include <cstring>
#include <iostream>

namespace nes_em {

NROMCart::NROMCart(const NesFile& file) {
    mirroring = file.mirroring;
    std::memcpy(chr_rom, file.chr_rom.data(), file.chr_rom.size());
    if (file.prg_rom.size() > 0x8000)
        throw std::runtime_error("maximum prg-rom size of 32768 bytes for nrom exceeded");
    std::memcpy(prg_rom, file.prg_rom.data(), file.prg_rom.size());
    if (file.prg_rom.size() <= 0x4000)
        std::memcpy(prg_rom + 0x4000, file.prg_rom.data(), file.prg_rom.size());
}

BusRead NROMCart::cpu_read(uint16_t addr) {
    if (addr >= 0x6000) {
        if (addr < 0x8000)
            return {prg_ram[addr - 0x6000], 0};
        return {prg_rom[addr & 0x7fff], 0};
    }
    return {0, 0xff};
}

void NROMCart::cpu_write(uint16_t addr, uint8_t value) {
    if (addr >= 0x6000 && addr < 0x8000)
        prg_ram[addr - 0x6000] = value;
}

BusRead NROMCart::ppu_read(uint16_t addr) {
    if (addr < 0x2000)
        return {chr_rom[addr], 0};
    return {0, 0xff};
}

void NROMCart::ppu_write(uint16_t, uint8_t) {
    return;
}

}