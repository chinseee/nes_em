#pragma once
#include "nes_em/fwd.h"
#include "nes_em/bus.h"
#include "nes_em/nes_file.h"

namespace nes_em {

class Cartridge {
public:
    Mirroring mirroring;
    virtual BusRead cpu_read(uint16_t) = 0;
    virtual void cpu_write(uint16_t, uint8_t) = 0;

    virtual BusRead ppu_read(uint16_t) = 0;
    virtual void ppu_write(uint16_t, uint8_t) = 0;

    static std::unique_ptr<Cartridge> create(const NesFile&);

    virtual ~Cartridge() = default;
};

}