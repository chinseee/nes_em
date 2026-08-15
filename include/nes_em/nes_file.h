#pragma once
#include "nes_em/fwd.h"

namespace nes_em {

struct NesFile {
    static constexpr std::array<uint8_t, 4> NES_HEADER{
        0x4e, // N
        0x45, // E
        0x53, // S
        0x1a, // MS-DOS EOF
    };
    static constexpr size_t HEADER_SIZE = 16;
    static constexpr size_t TRAINER_SIZE = 512;
    static constexpr size_t PRG_ROM_UNIT_SIZE = 0x4000; // 16 KiB
    static constexpr size_t CHR_ROM_UNIT_SIZE = 0x2000; // 8 KiB

    std::ifstream file;
    std::vector<uint8_t> prg_rom, chr_rom;

    uint8_t mapper = 0;
    Mirroring mirroring = Mirroring::Horizontal;
    bool has_battery = false;
    bool has_trainer = false;

    NesFile(std::string);

private:
    void read();
};

}
