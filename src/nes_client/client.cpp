#include "nes_client/client.h"
#include "nes_client/palettes/2C02_G_U_wiki.pal.h"
#include "nes_em/nes.h"
#include "nes_em/nes_file.h"
#include "nes_em/controller.h"
#include <iostream>
#include <cstdint>
#include <bitset>
#include <utility>
#include <thread>

namespace nes_client {

Client::Client():
    nes(std::make_unique<nes_em::NES>()),
    sdl_ticks(0)
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

    nes_em::NesFile rom{"test_roms/AccuracyCoin.nes"};
    nes->bus.set_controller_connected(0, true);
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
        if (sdl_ticks != 0) {
            SDL_Delay(1000 / 60.0988 - (SDL_GetTicks() - sdl_ticks));
        }   
        sdl_ticks = SDL_GetTicks();
        //std::cout << sdl_ticks << std::endl;
    }
}

void Client::run_engine() {
    while (running) {
        nes->cpu.exec_inst();
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

        SDL_PumpEvents(); 

        std::bitset<8> input;
        const bool *keystate = SDL_GetKeyboardState(NULL);
        
        input[nes_em::Controller::BTN_A]  = keystate[SDL_SCANCODE_X];
        input[nes_em::Controller::BTN_B]  = keystate[SDL_SCANCODE_Z];
        input[nes_em::Controller::SELECT] = keystate[SDL_SCANCODE_C];
        input[nes_em::Controller::START]  = keystate[SDL_SCANCODE_V];
        input[nes_em::Controller::UP]     = keystate[SDL_SCANCODE_UP];
        input[nes_em::Controller::DOWN]   = keystate[SDL_SCANCODE_DOWN];
        input[nes_em::Controller::LEFT]   = keystate[SDL_SCANCODE_LEFT];
        input[nes_em::Controller::RIGHT]  = keystate[SDL_SCANCODE_RIGHT];

        //std::cout << input.to_ullong() << std::endl;
        nes->bus.set_controller_input(0, input.to_ulong());


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
