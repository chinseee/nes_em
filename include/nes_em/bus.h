#pragma once
#include "nes_em/fwd.h"
#include "nes_em/controller.h"

namespace nes_em {

class Bus {
    // devices
    uint8_t cpu_ram[0x0800];

    uint8_t vram[0x0800];
    uint8_t palette_idxs[0x20];

    Controller controls[2];
    CPU* cpu;
    PPU* ppu;
    Cartridge* cart;

public:
    // io
    uint16_t cpu_addr;
    uint8_t cpu_data;
    bool is_cpu_read;


    uint16_t ppu_data;

    // TODO: decide if implementing decay is worth it
    uint8_t ppu_io_latch;


    Bus();
    void build(NES*);
    void load(Cartridge*);
    void reset();
    void set_controller_input(size_t, uint8_t);
    void set_controller_connected(size_t, bool);

    uint8_t cpu_read();
    void cpu_write();

    uint16_t unmirror_ppu_addr(uint16_t);

    uint8_t ppu_read();
    void ppu_write(uint8_t);
};

}