#include "nes_em/cartridge/cartridge.h"
#include "nes_em/cartridge/nrom_cart.h"

namespace nes_em {

std::unique_ptr<Cartridge> Cartridge::create(const NesFile& file) {
    std::unique_ptr<Cartridge> cart;
    if (file.mapper == 0)
        cart = std::make_unique<NROMCart>(file);
    return cart;
};

}