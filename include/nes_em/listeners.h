#pragma once
#include "nes_em/fwd.h"

namespace nes_em {

struct PPUListener {
    virtual void on_ppu_cycle(const PPU&) = 0;
};

}