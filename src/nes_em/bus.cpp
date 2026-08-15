#include "nes_em/bus.h"
#include "nes_em/ppu.h"
#include "nes_em/cartridge/cartridge.h"
#include "nes_em/nes.h"
#include <random>

#include <iostream>

namespace nes_em {

Bus::Bus():
    ppu(nullptr),
    cart(nullptr)
{}

void Bus::build(NES* nes) {
    ppu = &nes->ppu;
}

void Bus::load(Cartridge* cart) {
    this->cart = cart;
}

void Bus::reset() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist;

    for (uint16_t i = 0; i < 0x0800; ++i)
        cpu_ram[i] = dist(gen);
}

void Bus::update_controller_input(size_t idx, uint8_t input) {
    if (idx < 2)
        controls[idx].update_input(input);
}

uint8_t Bus::cpu_read(uint16_t addr) {
    if (addr < 0x2000) {
        cpu_data = cpu_ram[addr & 0x7ff];
        return cpu_data;
    }
    BusRead read;

    if (addr < 0x4000)
        read = ppu->cpu_read((addr & 7) | 0x2000);
    else if (addr == 0x4016 || addr == 0x4017)
        read = controls[addr & 1].cpu_read();
    else
        read = cart->cpu_read(addr);

    cpu_data &= read.open_bus_mask;
    cpu_data |= (read.value & ~read.open_bus_mask);
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
    else if (addr < 0x4018) {
        switch (addr & 0x1f) {
        case 0x14:
            ppu->cpu_write(addr, value);
            break;
        case 0x16:
            controls[0].cpu_write(value);
            controls[1].cpu_write(value);
            break;
        }
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
        BusRead read = cart->ppu_read(addr);
        ppu_data &= read.open_bus_mask;
        ppu_data |= (read.value & ~read.open_bus_mask);
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