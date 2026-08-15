#pragma once
#include "fwd.h"

namespace nes_em {

struct PPUListener {
    virtual void on_ppu_cycle(const PPU&) = 0;
};

}