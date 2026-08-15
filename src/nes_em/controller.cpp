#include "nes_em/controller.h"
#include <iostream>

namespace nes_em {

Controller::Controller():
    connected(false),
    s(false),
    input(0xff),
    input_buf(0)
{}

void Controller::update_input(uint8_t input)  {
    std::lock_guard<std::mutex> lock(mtx);
    if (connected)
        input_buf = input;
}

BusRead Controller::cpu_read() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!connected)
        return {0, 0xfe};
    if (s)
        input = input_buf;
    uint8_t result = input & 1;
    input = input >> 1;
    input |= 0x80;
    return {result, 0xfe};
}

void Controller::cpu_write(uint8_t value) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!connected)
        return;
    s = value & 1;
    if (s)
        input = input_buf;
}

void Controller::set_connected(bool connected) {
    std::lock_guard<std::mutex> lock(mtx);
    this->connected = connected;
}

}