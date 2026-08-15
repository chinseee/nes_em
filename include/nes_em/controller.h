#pragma once
#include "nes_em/fwd.h"

namespace nes_em {

class Controller {
    std::mutex mtx;
    bool connected;
    bool s;
    uint8_t input;

public:
    static constexpr size_t BTN_A = 0;
    static constexpr size_t BTN_B = 1;
    static constexpr size_t SELECT = 2;
    static constexpr size_t START = 3;
    static constexpr size_t UP = 4;
    static constexpr size_t DOWN = 5;
    static constexpr size_t LEFT = 6;
    static constexpr size_t RIGHT = 7;

    Controller();
    void update_input(uint8_t);
    BusRead cpu_read();
    void cpu_write(uint8_t);
    void set_connected(bool);
};

}