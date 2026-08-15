#include "nes_em/controller.h"

namespace nes_em {

Controller::Controller():
    connected(false),
    s(false),
    input(0xff)
{}

void Controller::update_input(uint8_t input)  {
    std::lock_guard<std::mutex> lock(mtx);
    if (s && connected)
        this->input = input;
}

BusRead Controller::cpu_read() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!connected)
        return {0, 0xfe};
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
}

void Controller::set_connected(bool connected) {
    std::lock_guard<std::mutex> lock(mtx);
    this->connected = connected;
}

}