#include "nes_em/cpu_debug.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace nes_em {

void DebugCPU::exec_inst() {
    uint32_t prev_a = a, prev_x = x, prev_y = y, prev_p = p.to_ulong(), prev_sp = sp;
    uint32_t prev_pc = pc;
    size_t prev_cycles = cycles;

    CPU::exec_inst();
    std::cout << std::hex << std::uppercase << std::noshowbase << std::setfill('0');
    std::cout << std::setw(4) << prev_pc;
    std::cout << " A:" << std::setw(2) << prev_a;
    std::cout << " X:" << std::setw(2) << prev_x;
    std::cout << " Y:" << std::setw(2) << prev_y;
    std::cout << " P:" << std::setw(2) << prev_p;
    std::cout << " SP:" << std::setw(2) << prev_sp;
    std::cout << std::dec;
    std::cout << " CYC:" << prev_cycles;
    std::cout << "\n";
};
}