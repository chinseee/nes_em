#pragma once
#include "nes_em/fwd.h"

namespace nes_em {

// $4000/$4004, $4001/$4005, $4002-$4003/$4006-$4007
struct PulseChannel {
    bool is_pulse_one; // selects one's- vs two's-complement sweep negate behavior
    bool enabled;

    uint8_t duty;                // 2 bits: selects the 8-step duty sequence
    bool length_counter_halt;    // also doubles as the envelope loop flag
    bool constant_volume;
    uint8_t volume;              // constant volume, or envelope period, depending on the flag above

    bool sweep_enabled;
    uint8_t sweep_period;
    bool sweep_negate;
    uint8_t sweep_shift;
    bool sweep_reload;
    uint8_t sweep_divider;

    uint16_t timer_period;       // 11 bits
    uint16_t timer;
    uint8_t sequence_step;       // 0-7, position in the duty sequence

    uint8_t length_counter;

    bool envelope_start;
    uint8_t envelope_divider;
    uint8_t envelope_decay;      // 0-15, the envelope's current output level
};

// $4008, $400a-$400b
struct TriangleChannel {
    bool enabled;

    bool control_flag;           // also doubles as the length counter halt flag
    uint8_t linear_counter_reload;

    uint16_t timer_period;       // 11 bits
    uint16_t timer;
    uint8_t sequence_step;       // 0-31, position in the triangle waveform sequence

    uint8_t length_counter;

    uint8_t linear_counter;
    bool linear_counter_reload_flag;
};

// $400c, $400e-$400f
struct NoiseChannel {
    bool enabled;

    bool length_counter_halt;    // also doubles as the envelope loop flag
    bool constant_volume;
    uint8_t volume;

    bool mode;                   // "loop noise" flag: taps bit 6 instead of bit 1
    uint16_t timer_period;
    uint16_t timer;
    uint16_t shift_register;     // 15-bit LFSR, reset to 1

    uint8_t length_counter;

    bool envelope_start;
    uint8_t envelope_divider;
    uint8_t envelope_decay;
};

// $4010-$4013
struct DmcChannel {
    bool enabled;

    bool irq_enable;
    bool loop;
    uint16_t timer_period;
    uint16_t timer;

    uint8_t output_level;        // 7-bit DAC value, directly audible

    uint16_t sample_addr;        // $c000 + value * 64
    uint16_t sample_length;      // value * 16 + 1

    uint16_t current_addr;       // wraps from $ffff to $8000
    uint16_t bytes_remaining;

    uint8_t sample_buffer;
    bool sample_buffer_full;

    uint8_t shift_register;
    uint8_t bits_remaining;
    bool silence;

    bool interrupt_flag;
};

// $4017
struct FrameCounter {
    bool mode;                   // false = 4-step sequence, true = 5-step sequence
    bool irq_inhibit;
    bool interrupt_flag;

    uint16_t cycle;               // divider counting CPU cycles since the last reset

    bool reset_pending;           // a write to $4017 resets the sequencer after a short delay
    uint8_t reset_delay;
};

class APU {
public:
    static constexpr std::array<uint8_t, 32> LENGTH_COUNTER_TABLE = {
        10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
        12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
    };

    // NTSC timer periods, in CPU cycles
    static constexpr std::array<uint16_t, 16> NOISE_PERIOD_TABLE = {
        4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068,
    };
    static constexpr std::array<uint16_t, 16> DMC_RATE_TABLE = {
        428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106, 84, 72, 54,
    };

    // connections
    CPU* cpu;
    Bus* bus;

    PulseChannel pulse1, pulse2;
    TriangleChannel triangle;
    NoiseChannel noise;
    DmcChannel dmc;
    FrameCounter frame_counter;

    uint64_t cycles;
    bool even_cycle; // pulse/noise/dmc timers clock once per *apu* cycle (every other cpu cycle)

    // output
    float* output;
    size_t output_pos;

    APU();

    void reset();
    void cycle_start();
    void cycle_end();

    BusRead cpu_read(uint16_t);
    void cpu_write(uint16_t, uint8_t);

    void step_frame_counter();
    void quarter_frame();
    void half_frame();

    void clock_timers();
    void clock_length_counters();
    void clock_sweeps();
    void clock_envelopes();
    void clock_linear_counter();

    void clock_dmc_reader();

    float mix();
};

}
