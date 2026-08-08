#include <algorithm>
#include <stdexcept>
#include "nes_em/nes_file.h"

namespace nes_em {

NesFile::NesFile(std::string file_name): file(file_name, std::ios::binary) {
    if (!file) {
        throw std::runtime_error("failed to open nes file: " + file_name);
    }
    read();
}

void NesFile::read() {
    std::array<uint8_t, HEADER_SIZE> header;
    file.read(reinterpret_cast<char*>(header.data()), header.size());
    if (!file || !std::equal(NES_HEADER.begin(), NES_HEADER.end(), header.begin())) {
        throw std::runtime_error("invalid nes file: missing NES header magic");
    }

    size_t prg_rom_units = header[4];
    size_t chr_rom_units = header[5];

    uint8_t flags6 = header[6];
    uint8_t flags7 = header[7];

    has_trainer = flags6 & 0x04;
    has_battery = flags6 & 0x02;
    if (flags6 & 0x08) {
        mirroring = Mirroring::FourScreen;
    } else if (flags6 & 0x01) {
        mirroring = Mirroring::Vertical;
    } else {
        mirroring = Mirroring::Horizontal;
    }
    mapper = (flags7 & 0xf0) | (flags6 >> 4);

    if (has_trainer) {
        file.seekg(TRAINER_SIZE, std::ios::cur);
    }

    prg_rom.resize(prg_rom_units * PRG_ROM_UNIT_SIZE);
    file.read(reinterpret_cast<char*>(prg_rom.data()), prg_rom.size());

    chr_rom.resize(chr_rom_units * CHR_ROM_UNIT_SIZE);
    file.read(reinterpret_cast<char*>(chr_rom.data()), chr_rom.size());

    if (file.fail()) {
        throw std::runtime_error("invalid nes file: truncated rom data");
    }
}

}
