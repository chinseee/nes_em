#pragma once
#include "nes_em/listeners.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>
#include <memory>
#include <atomic>


namespace nes_client {

class Client: public nes_em::PPUListener {
    static constexpr int32_t WINDOW_WIDTH = 341;
    static constexpr int32_t WINDOW_HEIGHT = 262;

public:
    std::unique_ptr<nes_em::NES> nes;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* nes_surface;
    SDL_Palette* nes_palette;
    SDL_Texture* nes_texture;
    uint8_t* nes_pixels;
    std::atomic<bool> running;
    std::atomic<bool> frame_ready;

    Client();
    ~Client();

    void on_ppu_cycle(const nes_em::PPU&);
    void run_engine();
    void run();

};
}