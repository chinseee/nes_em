#include "nes_em/bus.h"
#include "nes_em/ppu.h"
#include "nes_em/cartridge/cartridge.h"
#include <random>

#include <iostream>

namespace nes_em {

void Bus::reset() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist;

    for (uint16_t i = 0; i < 0x0800; ++i)
        cpu_ram[i] = dist(gen);
}

uint8_t Bus::cpu_read(uint16_t addr) {
    if (addr < 0x2000)
        cpu_data = cpu_ram[addr & 0x7ff];
    else if (addr < 0x4000)
        cpu_data = ppu->cpu_read((addr & 7) | 0x2000);
    else {
        uint8_t cart_data = cart->cpu_read(addr);
        if (!cart->open_cpu_read)
            cpu_data = cart_data;
    }
    
    return cpu_data;
}

void Bus::cpu_write(uint16_t addr, uint8_t value) {
    cpu_data = value;
    if (addr < 0x2000) {
        cpu_ram[addr & 0x7ff] = value;
    } 
    else if (addr < 0x4000) {
        ppu->cpu_write((addr & 7) | 0x2000, value);
    }
    else if (addr == 4014) {
        ppu->cpu_write(addr, value);
    }
    else {
        cart->cpu_write(addr, value);
    }
}

uint16_t Bus::unmirror_ppu_addr(uint16_t addr) {
    uint16_t unmirrored_addr = addr;

    switch (cart->mirroring) {
    case Mirroring::Horizontal:
        unmirrored_addr &= 0x3ff;
        if (addr | 0x800)
            unmirrored_addr |= 0x400;
        break;
    case Mirroring::Vertical:
        unmirrored_addr &= 0x7ff;
        break;
    // TODO: add other mirroring cases
    case Mirroring::FourScreen:
        break;
    }
    return unmirrored_addr;
}

uint8_t Bus::ppu_read(uint16_t addr) {
    // TODO: add way for cartridges to disable internal vram
    if (addr < 0x2000) {
        uint8_t cart_data = cart->ppu_read(addr);
        if (!cart->open_ppu_read)
            ppu_data = cart_data;
    }
    else if (addr < 0x3f00) {
        ppu_data = vram[unmirror_ppu_addr(addr)];
    }
    else if (addr < 0x4000) {
        ppu_data = palette_idxs[addr & 0x1f];
    }
    return ppu_data;
}

void Bus::ppu_write(uint16_t addr, uint8_t value) {
    ppu_data = value;
    if (addr < 0x2000) {
        cart->ppu_write(addr, value);
    }
    else if (addr < 0x3f00) {
        vram[unmirror_ppu_addr(addr)] = value;
    }
    else if (addr < 0x4000) {
        palette_idxs[addr & 0x1f] = value;
    }
}



}