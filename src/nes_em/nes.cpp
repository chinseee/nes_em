#include "nes_em/nes.h"
#include <iostream>
#include <cstring>

namespace nes_em {

NES::NES() {
    cpu.ppu = &ppu;
    cpu.bus = &bus;
    
    ppu.cpu = &cpu;
    ppu.bus = &bus;

    bus.ppu = &ppu;
}

void NES::load(const NesFile& file) {
    cart = std::move(Cartridge::create(file));
    bus.cart = cart.get();
}

void NES::reset() {
    ppu.reset();
    cpu.reset();
} 

}