#pragma once
#include "nes_em/fwd.h"
#include "nes_em/cpu.h"

namespace nes_em {
class DebugCPU: public CPU {
public:
    void exec_inst() ;
};
}