#pragma once
#include "nes_em/fwd.h"
#include "nes_em/ppu.h"
#include "nes_em/cpu.h"
#include "nes_em/cpu_debug.h"
#include "nes_em/bus.h"
#include "nes_em/cartridge/cartridge.h"
#include "nes_em/controller.h"

namespace nes_em {

struct NES {
    CPU cpu;
    PPU ppu;
    Bus bus;
    std::unique_ptr<Cartridge> cart;

    NES();

    void load(const NesFile&);
    void reset();
};

}