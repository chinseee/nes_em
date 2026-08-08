#include <iostream>
#include "nes_em/cpu_debug.h"

int main(void) {
    nes_em::DebugCPU cpu;
    nes_em::NesFile file("nestest.nes");
    cpu.load(file);
    cpu.reset();

    cpu.pc = 0xc000;
    for (size_t i = 0; i < 8991; ++i)
        cpu.exec_inst();
}