#include "nes_em/apu.h"
#include "nes_em/cpu.h"

namespace nes_em {

APU::APU() {

}

void APU::reset() {
    cycles = 0;
    even_cycle = false;

    frame_counter = FrameCounter{};
}

void APU::cycle_start() {
    step_frame_counter();
}

void APU::cycle_end() {
    even_cycle = !even_cycle;
    ++cycles;
}

// Steps the frame counter's internal divider by one CPU cycle, generating
// quarter/half frame clocks and the frame IRQ at the documented NTSC
// boundaries. quarter_frame()/half_frame() are intentionally no-ops for now:
// nothing downstream (envelopes, sweeps, length/linear counters) is wired up
// to them yet.
void APU::step_frame_counter() {
    if (frame_counter.reset_pending) {
        if (--frame_counter.reset_delay == 0) {
            frame_counter.reset_pending = false;
            frame_counter.cycle = 0;

            // switching into 5-step mode immediately generates one clock
            if (frame_counter.mode) {
                quarter_frame();
                half_frame();
            }
        }
        return;
    }

    ++frame_counter.cycle;

    if (!frame_counter.mode) {
        // mode 0: 4-step sequence, ~29830 cpu cycles long
        switch (frame_counter.cycle) {
        case 7457:
            quarter_frame();
            break;
        case 14913:
            quarter_frame();
            half_frame();
            break;
        case 22371:
            quarter_frame();
            break;
        case 29828:
            if (!frame_counter.irq_inhibit)
                frame_counter.interrupt_flag = true;
            break;
        case 29829:
            quarter_frame();
            half_frame();
            if (!frame_counter.irq_inhibit)
                frame_counter.interrupt_flag = true;
            break;
        case 29830:
            frame_counter.cycle = 0;
            break;
        }
    }
    else {
        // mode 1: 5-step sequence, ~37282 cpu cycles long, never generates an IRQ
        switch (frame_counter.cycle) {
        case 7457:
            quarter_frame();
            break;
        case 14913:
            quarter_frame();
            half_frame();
            break;
        case 22371:
            quarter_frame();
            break;
        case 37281:
            quarter_frame();
            half_frame();
            break;
        case 37282:
            frame_counter.cycle = 0;
            break;
        }
    }
}

void APU::quarter_frame() {
    // TODO: clock envelopes and the triangle's linear counter once the channels exist
}

void APU::half_frame() {
    // TODO: clock length counters and sweep units once the channels exist
}

BusRead APU::cpu_read(uint16_t addr) {
    if (addr == 0x4015) {
        uint8_t value = 0;

        // TODO: bits 0-4 (channel length counter > 0) and bit 7 (dmc interrupt)
        // aren't implemented yet since the channels themselves aren't wired up
        if (frame_counter.interrupt_flag)
            value |= 0x40;

        frame_counter.interrupt_flag = false; // reading $4015 clears the frame interrupt flag

        return {value, 0x00};
    }

    return {0, 0xff};
}

void APU::cpu_write(uint16_t addr, uint8_t value) {
    switch (addr) {
    case 0x4017:
        frame_counter.mode = value & 0x80;
        frame_counter.irq_inhibit = value & 0x40;

        if (frame_counter.irq_inhibit)
            frame_counter.interrupt_flag = false;

        // the sequencer reset takes effect 3 or 4 cpu cycles later, depending
        // on whether this write landed on an even or odd apu cycle
        frame_counter.reset_pending = true;
        frame_counter.reset_delay = even_cycle ? 4 : 3;
        break;
    case 0x4014:
        cpu->oam_dma_addr = value << 8;
        cpu->oam_dma_state = 512;
        cpu->dma_halt_flag = true;
        break;
    default:
        // TODO: channel registers ($4000-$4013) and $4015 (channel enables)
        break;
    }
}

}
