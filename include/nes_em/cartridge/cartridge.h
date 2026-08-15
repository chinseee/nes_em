#pragma once
#include "nes_em/nes_file.h"
#include <cstdint>
#include <memory>


namespace nes_em {

class Cartridge {
public:
    Mirroring mirroring;
    bool open_cpu_read;
    bool open_ppu_read;
    virtual uint8_t cpu_read(uint16_t) = 0;
    virtual void cpu_write(uint16_t, uint8_t) = 0;

    virtual uint8_t ppu_read(uint16_t) = 0;
    virtual void ppu_write(uint16_t, uint8_t) = 0;

    static std::unique_ptr<Cartridge> create(const NesFile&);
};

}