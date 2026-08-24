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
    cpu = &nes->cpu;
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

void Bus::set_controller_input(size_t idx, uint8_t input) {
    if (idx < 2)
        controls[idx].update_input(input);
}

void Bus::set_controller_connected(size_t idx, bool connected) {
    if (idx < 2)
        controls[idx].set_connected(connected);
}

uint8_t Bus::cpu_read() {
    is_cpu_read = true;
    if (cpu_addr < 0x2000) {
        cpu_data = cpu_ram[cpu_addr & 0x7ff];
        return cpu_data;
    }
    BusRead read;

    if (cpu_addr < 0x4000) {
        BusRead read = ppu->cpu_read(cpu_addr);
        
        ppu_io_latch &= read.open_bus_mask;
        ppu_io_latch |= (read.value & ~read.open_bus_mask);

        return ppu_io_latch;
    }
        
    if (cpu_addr == 0x4016 || cpu_addr == 0x4017)
        read = controls[cpu_addr & 1].cpu_read();
    else
        read = cart->cpu_read(cpu_addr);

    cpu_data &= read.open_bus_mask;
    cpu_data |= (read.value & ~read.open_bus_mask);
    return cpu_data;
}

void Bus::cpu_write() {
    is_cpu_read = false;
    if (cpu_addr < 0x2000) {
        cpu_ram[cpu_addr & 0x7ff] = cpu_data;
    } 
    else if (cpu_addr < 0x4000) {
        ppu_io_latch = cpu_data;
        ppu->cpu_write(cpu_addr, cpu_data);
    }
    else if (cpu_addr < 0x4018) {
        switch (cpu_addr & 0x1f) {
        // case 0x14:
        //     cpu->oam_dma_addr = cpu_data << 8;
        //     cpu->oam_dma_state = 512;
        //     ppu->cpu_write(cpu_addr, cpu_data);
        //     break;
        case 0x16:
            controls[0].cpu_write(cpu_data);
            controls[1].cpu_write(cpu_data);
            break;
        }
    }
    else {
        cart->cpu_write(cpu_addr, cpu_data);
    }
}

uint16_t Bus::unmirror_ppu_addr(uint16_t addr) {
    uint16_t unmirrored_addr = addr;

    switch (cart->mirroring) {
    case Mirroring::Horizontal:
        unmirrored_addr &= 0x3ff;
        if (addr & 0x800)
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

uint8_t Bus::ppu_read() {
    // TODO: add way for cartridges to disable internal vram
    if (ppu_data < 0x2000) {
        BusRead read = cart->ppu_read(ppu_data);
        ppu_data &= read.open_bus_mask;
        ppu_data |= (read.value & ~read.open_bus_mask);
    }
    else if (ppu_data < 0x3f00) {
        ppu_data = vram[unmirror_ppu_addr(ppu_data)];
    }
    else if (ppu_data < 0x4000) {
        ppu_data &= 0x1f;
        if (ppu_data & 3)
            ppu_data = palette_idxs[ppu_data];
        else
            ppu_data = palette_idxs[ppu_data & 0xf];
    }
    return ppu_data;
}

void Bus::ppu_write(uint8_t value) {
    if (ppu_data < 0x2000) {
        cart->ppu_write(ppu_data, value);
    }
    else if (ppu_data < 0x3f00) {
        vram[unmirror_ppu_addr(ppu_data)] = value;
    }
    else if (ppu_data < 0x4000) {
        ppu_data &= 0x1f;
        if (ppu_data & 3)
            palette_idxs[ppu_data] = value;
        else
            palette_idxs[ppu_data & 0xf] = value;
    }
    ppu_data &= 0x3f00;
    ppu_data |= value;
}



}