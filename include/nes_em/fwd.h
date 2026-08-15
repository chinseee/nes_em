#pragma once
#include <cstdint>
#include <bitset>

#include <array>
#include <string>
#include <vector>

#include <fstream>

#include <memory>
#include <utility>
#include <mutex>

namespace nes_em {

class CPU;
class PPU;
class Bus;
class Cartridge;
class NES;

struct BusRead {
    uint8_t value;
    uint8_t open_bus_mask;
};

enum class Mirroring {
    Horizontal,
    Vertical,
    FourScreen,
};


}