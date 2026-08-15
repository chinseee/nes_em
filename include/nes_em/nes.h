#pragma once
#include "nes_em/ppu.h"
#include "nes_em/cpu.h"
#include "nes_em/cpu_debug.h"
#include "nes_em/bus.h"
#include "nes_em/cartridge/cartridge.h"
#include <memory>

namespace nes_em {

class NES {
public:
    CPU cpu;
    PPU ppu;
    Bus bus;
    std::unique_ptr<Cartridge> cart;

    NES();

    void load(const NesFile&);
    void reset();
};
}