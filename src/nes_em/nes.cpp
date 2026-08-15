#include "nes_em/nes.h"
#include <iostream>
#include <cstring>

namespace nes_em {

NES::NES() {
    cpu.ppu = &ppu;
    cpu.bus = &bus;
    
    ppu.cpu = &cpu;
    ppu.bus = &bus;

    bus.build(this);
}

void NES::load(const NesFile& file) {
    cart = std::move(Cartridge::create(file));
    bus.load(cart.get());
}

void NES::reset() {
    bus.reset();
    ppu.reset();
    cpu.reset();
} 

}