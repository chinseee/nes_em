#include "nes_client/client.h"
#include "nes_client/palettes/2C02_G_U_wiki.pal.h"
#include "nes_em/nes.h"
#include "nes_em/nes_file.h"
#include <iostream>
#include <cstdint>
#include <utility>
#include <thread>

namespace nes_client {

Client::Client():
    nes(std::make_unique<nes_em::NES>())
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return;
    }

    if (!SDL_CreateWindowAndRenderer("NES", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        std::cerr << "Window and Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }

    nes_surface = SDL_CreateSurface(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_PIXELFORMAT_INDEX8);
    nes_palette = SDL_CreatePalette(256);
    
    SDL_SetPaletteColors(nes_palette, PALETTE_2C02_U_WIKI, 0, 256);
    SDL_SetSurfacePalette(nes_surface, nes_palette);

    nes_texture = SDL_CreateTextureFromSurface(renderer, nes_surface);
    nes_pixels = static_cast<uint8_t*>(SDL_malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint8_t)));

    nes_em::NesFile rom{"nestest.nes"};
    nes->ppu.add_listener(this);
    nes->ppu.output = nes_pixels;
    nes->load(rom);
    nes->reset();
    
}

Client::~Client() {
    SDL_free(nes_pixels);
    SDL_DestroyTexture(nes_texture);
    SDL_DestroyPalette(nes_palette);
    SDL_DestroySurface(nes_surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Client::on_ppu_cycle(const nes_em::PPU& ppu) {
    if (ppu.scanline == 241 && ppu.dot == 1) {
        frame_ready = true; // hack for now. fix later
    }
}

void Client::run_engine() {
    while (running) {
        nes->cpu.exec_inst();
        if (nes->cpu.cycles > 100000000)
            running = false;
    }
}

void Client::run() {
    running = true;
    std::thread engine_thread(&Client::run_engine, this);

    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
                break;
            }
        }

        if (frame_ready) {
            frame_ready = false;
            SDL_UpdateTexture(nes_texture, NULL, nes_pixels, WINDOW_WIDTH * sizeof(uint8_t));
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, nes_texture, NULL, NULL);

            SDL_RenderPresent(renderer);
        }
    }

    engine_thread.join();
}

}
