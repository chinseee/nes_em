#include "nes_em/cpu.h"
#include "nes_em/ppu.h"
#include "nes_em/bus.h"
#include <array>
#include <random>
#include <cstring>

#include <iostream>
#include <iomanip>

#define NES_CPU_BIN_OP_MODE(bin, op, addr_mode) \
template <> \
void CPU::opcode_impl<bin>(){ \
    CPU::opcode_impl<op, addr_mode>(); \
}

#define NES_CPU_BIN_OP_MODE_RMW(bin, op, addr_mode) \
template <> \
void CPU::opcode_impl<bin>(){ \
    CPU::opcode_impl<op, addr_mode, true>(); \
}

namespace nes_em {

CPU::CPU():
    pc(0xfffc),
    a(0),
    x(0),
    y(0),
    sp(0),
    p((1 << UNUSED_BIT) | (1 << INTERRUPT_DISABLE_BIT)),
    cycles(0),
    oam_dma_state(0)
{}

void CPU::reset() {
    // TODO: update reset behavior to be more accurate
    cycle_start();
    cycle_end();
    cycle_start();
    cycle_end();
    pc = 0xfffc;

    cycle_start();
    cycle_end();
    --sp;

    cycle_start();
    cycle_end();
    --sp;

    cycle_start();
    cycle_end();
    --sp;

    uint8_t lo = inst_read();

    p[INTERRUPT_DISABLE_BIT] = true;
    pc = lo | inst_read() << 8;
}

void CPU::cycle_start() {
    cycle_start_no_dma();
    handle_dma();
}

void CPU::cycle_start_no_dma() {
    ++cycles;
    // apu cycle start/end
    ppu->cycle();
}

void CPU::cycle_end() {
    nmi_latch |= nmi_line_cur && !nmi_line_prev;
    nmi_line_prev = nmi_line_cur;
    ppu->cycle();
    ppu->cycle();
    is_get_cycle = !is_get_cycle;
}

void CPU::handle_dma() {
    // can only halt on read cycles
    if (!is_read_cycle || !dma_halt_flag)
        return;
    
    // halt cycle
    cycle_end();
    bus->cpu_read();
    if (dmc_dma_state == 3)
        dmc_dma_state = 2;
    
    // handle oam dma
    // it is guaranteed this is the only oam dma we have to handle now,
    // since oam dma can only be triggered on write cycle
    uint8_t oam_dma_data = bus->cpu_data;
    while (oam_dma_state > 0) {
        if (dmc_dma_state == 1) {
            // if dmc dma is ready, it executes over the oam dma
            // dmc dma read cycle
            
            // TODO: add data transfer here
            cycle_start_no_dma();
            cycle_end();
            
            continue;
        }
        
        if (dmc_dma_state == 3) {
            // oam dma cycle counts as dmc dma halt cycle
            dmc_dma_state = 2;
        }
        else if (dmc_dma_state == 2) {
            // oam dma cycle counts as dmc dma dummy/align cycle
            if (!is_get_cycle)
                dmc_dma_state = 1;
        }

        cycle_start_no_dma();
        if (oam_dma_state & 1) {
            // oam dma write cycle
            bus->cpu_addr = 0x2004;
            bus->cpu_data = oam_dma_data;
            cycle_end();
            bus->cpu_write();
        }
        else if (is_get_cycle) {
            // oam dma read cycle
            bus->cpu_addr = oam_dma_addr;
            ++oam_dma_addr;
            cycle_end();
            oam_dma_data = bus->cpu_read();
        }
        else {
            // oam dma alignment cycle
            cycle_end();
            bus->cpu_read();
        }
        --oam_dma_state;
    }

    dma_halt_flag = false;
    dmc_dma_state = 0;
}

void CPU::set_nmi_line(bool nmi_line) {
    nmi_line_cur = nmi_line;
}

void CPU::poll_interrupts() {
    nmi_pending = nmi_latch;
    irq_pending = irq_line && !p[INTERRUPT_DISABLE_BIT];
}

uint8_t CPU::inst_read() {
    return cpu_read(pc++);
}

uint8_t CPU::cpu_read(uint16_t addr) {
    is_read_cycle = true;
    cycle_start();
    bus->cpu_addr = addr;
    cycle_end();
    return bus->cpu_read();
}

void CPU::cpu_write(uint16_t addr, uint8_t value) {
    is_read_cycle = false;
    cycle_start();
    bus->cpu_addr = addr;
    bus->cpu_data = value;
    cycle_end();
    bus->cpu_write();
}

void CPU::push(uint8_t value) {
    cycle_start();
    bus->cpu_addr = 0x100 | sp;
    bus->cpu_data = value;
    cycle_end();
    --sp;
    bus->cpu_write();
}


uint8_t CPU::pull() {
    ++sp;
    cycle_start();
    bus->cpu_addr = 0x100 | sp;
    cycle_end();
    return bus->cpu_read();
}

void CPU::set_zn(uint8_t value) {
    p[ZERO_BIT] = value == 0;
    p[NEGATIVE_BIT] = value & (1 << NEGATIVE_BIT);
}

void CPU::branch(bool cond, uint16_t addr) {
    uint8_t offset = cpu_read(addr);

    // interrupts always polled at end of cycle 2
    poll_interrupts();
    // disable interrupt polling so exec_inst won't call poll_interrupts again
    interrupt_polling = false;

    if (cond) {
        // intentional bug: interrupts NOT polled if page not crossed
        // this is the end of cycle 2 for all branch instructions.
        // as the 2nd to last cycle for taken branches, there should
        // be an interrupt poll just before end of cycle 2, but
        // a hardware quirk prevents this.
        cpu_read(pc);

        // pretty sure this is impl defined pre c++20
        // maybe fix later
        uint16_t next_pc = pc + static_cast<int8_t>(offset);
        pc &= 0xff00;
        pc |= (next_pc & 0xff);
        
        if (next_pc != pc) {
            // interrupts ARE polled on page crosses.
            // enable to make exec_inst call poll_interrupts again
            interrupt_polling = true;
            cpu_read(pc);
        }
        pc = next_pc;
    }
}

template <void (CPU::*op)(uint16_t), AddrMode mode, bool rmw>
void CPU::opcode_impl() {
    if constexpr (mode == AddrMode::Immediate) {
        (this->*op)(pc++);
    }
    else if constexpr (mode == AddrMode::ZeroPage) {
        (this->*op)(inst_read());
    }
    else if constexpr (mode == AddrMode::ZeroPageX) {
        uint8_t addr = inst_read();

        cpu_read(addr);
        addr += x;

        (this->*op)(addr);
    }
    else if constexpr (mode == AddrMode::ZeroPageY) {
        uint8_t addr = inst_read();

        cpu_read(addr);
        addr += y;

        (this->*op)(addr);
    }
    else if constexpr (mode == AddrMode::Absolute) {
        uint16_t addr = inst_read();
        addr |= inst_read() << 8;
        (this->*op)(addr);
    }
    else if constexpr (mode == AddrMode::AbsoluteX) {
        uint8_t lo = inst_read();
        uint8_t hi = inst_read();
        uint16_t cross = lo + x;
        if (cross & 0x100 || rmw)
            cpu_read((hi << 8) | (cross & 0xff));
        (this->*op)(((hi << 8) | lo) + x);
    }
    else if constexpr (mode == AddrMode::AbsoluteY) {
        uint8_t lo = inst_read();
        uint8_t hi = inst_read();
        uint16_t cross = lo + y;
        if (cross & 0x100 || rmw)
            cpu_read((hi << 8) | (cross & 0xff));
        (this->*op)(((hi << 8) | lo) + y);
    }
    else if constexpr (mode == AddrMode::Relative) {
        // only used by branch instructions
        (this->*op)(pc++);
    }
    else if constexpr (mode == AddrMode::IndirectX) {
        uint8_t ind = inst_read();
        
        cpu_read(ind);
        ind += x;

        uint8_t lo = cpu_read(ind++);
        (this->*op)(lo | cpu_read(ind) << 8);
    }
    else if constexpr (mode == AddrMode::IndirectY) {
        uint8_t ind = inst_read();
        uint8_t lo = cpu_read(ind++);
        uint8_t hi = cpu_read(ind);
        uint16_t cross = lo + y;
        if (cross & 0x100)
            cpu_read((hi << 8) | (cross & 0xff));
        (this->*op)(((hi << 8) | lo) + y);
    }
    else {
        static_assert(false); // addressing mode not supported for this instruction
    }
}

template <void (CPU::*op)(uint16_t), AddrMode mode>
void CPU::opcode_impl() {
    opcode_impl<op, mode, false>();
}

template <void (CPU::*op)(), AddrMode mode>
void CPU::opcode_impl() {
    if constexpr (mode == AddrMode::Implied || mode == AddrMode::Accumulator) {
        (this->*op)();
    }
    else {
        static_assert(false); // addressing mode not supported for this instruction
    }
}

template <uint8_t bin>
void CPU::opcode_impl() {
    return;
}

void CPU::LDA(uint16_t addr) {
    a = cpu_read(addr);
    set_zn(a);
}

NES_CPU_BIN_OP_MODE(0xa9, &CPU::LDA, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa5, &CPU::LDA, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb5, &CPU::LDA, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xad, &CPU::LDA, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbd, &CPU::LDA, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0xb9, &CPU::LDA, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0xa1, &CPU::LDA, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0xb1, &CPU::LDA, AddrMode::IndirectY);

void CPU::STA(uint16_t addr) {
    cpu_write(addr, a);
}

template <>
void CPU::opcode_impl<&CPU::STA, AddrMode::AbsoluteX>() {
    uint8_t lo = inst_read();
    uint8_t hi = inst_read();
    uint16_t cross = lo + x;
    cpu_read((hi << 8) | (cross & 0xff));
    STA(((hi << 8) | lo) + x);
};

template <>
void CPU::opcode_impl<&CPU::STA, AddrMode::AbsoluteY>() {
    uint8_t lo = inst_read();
    uint8_t hi = inst_read();
    uint16_t cross = lo + y;
    cpu_read((hi << 8) | (cross & 0xff));
    STA(((hi << 8) | lo) + y);
};

template <>
void CPU::opcode_impl<&CPU::STA, AddrMode::IndirectY>() {
    uint8_t ind = inst_read();
    uint8_t lo = cpu_read(ind++);
    uint8_t hi = cpu_read(ind);
    uint16_t cross = lo + y;
    cpu_read((hi << 8) | (cross & 0xff));
    STA(((hi << 8) | lo) + y);
};

NES_CPU_BIN_OP_MODE(0x85, &CPU::STA, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x95, &CPU::STA, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x8d, &CPU::STA, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x9d, &CPU::STA, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x99, &CPU::STA, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x81, &CPU::STA, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x91, &CPU::STA, AddrMode::IndirectY);

void CPU::LDX(uint16_t addr) {
    x = cpu_read(addr);
    set_zn(x);
}

NES_CPU_BIN_OP_MODE(0xa2, &CPU::LDX, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa6, &CPU::LDX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb6, &CPU::LDX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0xae, &CPU::LDX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbe, &CPU::LDX, AddrMode::AbsoluteY);

void CPU::STX(uint16_t addr) {
    cpu_write(addr, x);
}
NES_CPU_BIN_OP_MODE(0x86, &CPU::STX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x96, &CPU::STX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0x8e, &CPU::STX, AddrMode::Absolute);

void CPU::LDY(uint16_t addr) {
    y = cpu_read(addr);
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xa0, &CPU::LDY, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa4, &CPU::LDY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb4, &CPU::LDY, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xac, &CPU::LDY, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbc, &CPU::LDY, AddrMode::AbsoluteX);

void CPU::STY(uint16_t addr) {
    cpu_write(addr, y);
}
NES_CPU_BIN_OP_MODE(0x84, &CPU::STY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x94, &CPU::STY, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x8c, &CPU::STY, AddrMode::Absolute);

void CPU::TAX() {
    cpu_read(pc);
    x = a;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xaa, &CPU::TAX, AddrMode::Implied);

void CPU::TXA() {
    cpu_read(pc);
    a = x;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x8a, &CPU::TXA, AddrMode::Implied);

void CPU::TAY() {
    cpu_read(pc);
    y = a;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xa8, &CPU::TAY, AddrMode::Implied);

void CPU::TYA() {
    cpu_read(pc);
    a = y;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x98, &CPU::TYA, AddrMode::Implied);

void CPU::ADC(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    uint16_t r = a + v + p[CARRY_BIT];
    p[CARRY_BIT] = r > 0xff; // carry;
    p[OVERFLOW_BIT] = (r ^ a) & (r ^ v) & (1 << NEGATIVE_BIT); // overflow
    a = r;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x69, &CPU::ADC, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x65, &CPU::ADC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x75, &CPU::ADC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x6d, &CPU::ADC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x7d, &CPU::ADC, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x79, &CPU::ADC, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x61, &CPU::ADC, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x71, &CPU::ADC, AddrMode::IndirectY);

void CPU::SBC(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    uint16_t r = a + ~v + p[CARRY_BIT];
    p[CARRY_BIT] = r <= 0xff; // carry
    p[OVERFLOW_BIT] = (r ^ a) & (r ^ ~v) & (1 << NEGATIVE_BIT); // overflow
    a = r;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0xe9, &CPU::SBC, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xeb, &CPU::SBC, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xe5, &CPU::SBC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xf5, &CPU::SBC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xed, &CPU::SBC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xfd, &CPU::SBC, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0xf9, &CPU::SBC, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0xe1, &CPU::SBC, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0xf1, &CPU::SBC, AddrMode::IndirectY);

void CPU::INC(uint16_t addr) {
    uint8_t t = cpu_read(addr);
    cpu_write(addr, t++);
    cpu_write(addr, t);
    set_zn(t);
}
NES_CPU_BIN_OP_MODE(0xe6, &CPU::INC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xf6, &CPU::INC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xee, &CPU::INC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xfe, &CPU::INC, AddrMode::AbsoluteX);

void CPU::DEC(uint16_t addr) {
    uint8_t t = cpu_read(addr);
    cpu_write(addr, t--);
    cpu_write(addr, t);
    set_zn(t);
}

NES_CPU_BIN_OP_MODE(0xc6, &CPU::DEC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xd6, &CPU::DEC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xce, &CPU::DEC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xde, &CPU::DEC, AddrMode::AbsoluteX);

void CPU::INX() {
    cpu_read(pc);
    ++x;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xe8, &CPU::INX, AddrMode::Implied);

void CPU::DEX() {
    cpu_read(pc);
    --x;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xca, &CPU::DEX, AddrMode::Implied);

void CPU::INY() {
    cpu_read(pc);
    ++y;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xc8, &CPU::INY, AddrMode::Implied);

void CPU::DEY() {
    cpu_read(pc);
    --y;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0x88, &CPU::DEY, AddrMode::Implied);

void CPU::ASL() {
    cpu_read(pc);
    p[CARRY_BIT] = a & (1 << NEGATIVE_BIT);
    a <<= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x0a, static_cast<void (CPU::*)()>(&CPU::ASL), AddrMode::Accumulator);
void CPU::ASL(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    v <<= 1;
    set_zn(v);
    cpu_write(addr, v);
}
NES_CPU_BIN_OP_MODE(0x06, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x16, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x0e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x1e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::AbsoluteX);

void CPU::LSR() {
    cpu_read(pc);
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a >>= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x4a, static_cast<void (CPU::*)()>(&CPU::LSR), AddrMode::Accumulator);
void CPU::LSR(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    v >>= 1;
    set_zn(v);
    cpu_write(addr, v);
}

NES_CPU_BIN_OP_MODE(0x46, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x56, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x4e, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x5e, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::AbsoluteX);

void CPU::ROL() {
    cpu_read(pc);
    uint8_t t = a << 1;
    t |= p[CARRY_BIT];
    p[CARRY_BIT] = a & (1 << NEGATIVE_BIT);
    a = t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x2a, static_cast<void (CPU::*)()>(&CPU::ROL), AddrMode::Accumulator);
void CPU::ROL(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    uint8_t t = v << 1;
    t |= p[CARRY_BIT];
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    set_zn(t);
    cpu_write(addr, t);
}

NES_CPU_BIN_OP_MODE(0x26, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x36, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x2e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x3e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::AbsoluteX);

void CPU::ROR() {
    cpu_read(pc);
    uint8_t t = a >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a = t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x6a, static_cast<void (CPU::*)()>(&CPU::ROR), AddrMode::Accumulator);
void CPU::ROR(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    uint8_t t = v >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    set_zn(t);
    cpu_write(addr, t);
}

NES_CPU_BIN_OP_MODE(0x66, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x76, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x6e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x7e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::AbsoluteX);

void CPU::AND(uint16_t addr) {
    a &= cpu_read(addr);
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x29, &CPU::AND, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x25, &CPU::AND, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x35, &CPU::AND, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x2d, &CPU::AND, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x3d, &CPU::AND, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x39, &CPU::AND, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x21, &CPU::AND, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x31, &CPU::AND, AddrMode::IndirectY);

void CPU::ORA(uint16_t addr) {
    a |= cpu_read(addr);
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x09, &CPU::ORA, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x05, &CPU::ORA, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x15, &CPU::ORA, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x0d, &CPU::ORA, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x1d, &CPU::ORA, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x19, &CPU::ORA, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x01, &CPU::ORA, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x11, &CPU::ORA, AddrMode::IndirectY);

void CPU::EOR(uint16_t addr) {
    a ^= cpu_read(addr);
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x49, &CPU::EOR, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x45, &CPU::EOR, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x55, &CPU::EOR, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x4d, &CPU::EOR, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x5d, &CPU::EOR, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x59, &CPU::EOR, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x41, &CPU::EOR, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x51, &CPU::EOR, AddrMode::IndirectY);

void CPU::BIT(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    p[ZERO_BIT] = (a & v) == 0; 
    p[OVERFLOW_BIT] = v & (1 << OVERFLOW_BIT);
    p[NEGATIVE_BIT] = v & (1 << NEGATIVE_BIT);
}
NES_CPU_BIN_OP_MODE(0x24, &CPU::BIT, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x2c, &CPU::BIT, AddrMode::Absolute);

void CPU::CMP(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    p[CARRY_BIT] = a >= v;
    set_zn(a - v);
}
NES_CPU_BIN_OP_MODE(0xc9, &CPU::CMP, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xc5, &CPU::CMP, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xd5, &CPU::CMP, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xcd, &CPU::CMP, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xdd, &CPU::CMP, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0xd9, &CPU::CMP, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0xc1, &CPU::CMP, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0xd1, &CPU::CMP, AddrMode::IndirectY);

void CPU::CPX(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    p[CARRY_BIT] = x >= v;
    set_zn(x - v);
}
NES_CPU_BIN_OP_MODE(0xe0, &CPU::CPX, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xe4, &CPU::CPX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xec, &CPU::CPX, AddrMode::Absolute);

void CPU::CPY(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    p[CARRY_BIT] = y >= v;
    set_zn(y - v);
}
NES_CPU_BIN_OP_MODE(0xc0, &CPU::CPY, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xc4, &CPU::CPY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xcc, &CPU::CPY, AddrMode::Absolute);

void CPU::BCC(uint16_t addr) {
    branch(!p[CARRY_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0x90, &CPU::BCC, AddrMode::Relative);

void CPU::BCS(uint16_t addr) {
    branch(p[CARRY_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0xb0, &CPU::BCS, AddrMode::Relative);

void CPU::BEQ(uint16_t addr) {
    branch(p[ZERO_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0xf0, &CPU::BEQ, AddrMode::Relative);

void CPU::BNE(uint16_t addr) {
    branch(!p[ZERO_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0xd0, &CPU::BNE, AddrMode::Relative);

void CPU::BPL(uint16_t addr) {
    branch(!p[NEGATIVE_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0x10, &CPU::BPL, AddrMode::Relative);

void CPU::BMI(uint16_t addr) {
    branch(p[NEGATIVE_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0x30, &CPU::BMI, AddrMode::Relative);

void CPU::BVC(uint16_t addr) {
    branch(!p[OVERFLOW_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0x50, &CPU::BVC, AddrMode::Relative);

void CPU::BVS(uint16_t addr) {
    branch(p[OVERFLOW_BIT], addr);
}
NES_CPU_BIN_OP_MODE(0x70, &CPU::BVS, AddrMode::Relative);

void CPU::JMP(uint16_t addr) {
    pc = addr; //all cycles already accounted for
}

template <>
void CPU::opcode_impl<&CPU::JMP, AddrMode::Absolute>() {
    uint16_t addr = inst_read();
    addr |= inst_read() << 8;
    JMP(addr);
}

template <>
void CPU::opcode_impl<&CPU::JMP, AddrMode::Indirect>() {
    // only used by JMP.
    uint8_t lo = inst_read();
    uint8_t hi = inst_read();

    uint16_t addr = cpu_read(lo | hi << 8);
    // intentional bug: page crossing is broken
    ++lo;
    addr |= cpu_read(lo | hi << 8) << 8;

    JMP(addr);
}

NES_CPU_BIN_OP_MODE(0x4c, &CPU::JMP, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x6c, &CPU::JMP, AddrMode::Indirect);

void CPU::JSR(uint16_t) {
    // only opcode explicitly specialized below
}
template <>
void CPU::opcode_impl<&CPU::JSR, AddrMode::Absolute, false>() {

    uint16_t ret_addr = pc + 1;
    
    uint8_t lo = inst_read();
    
    pc &= 0xff00;
    pc |= lo;

    cpu_read(0x100 | sp);
    
    push(ret_addr >> 8);
    push(ret_addr & 0xff);

    uint8_t hi = cpu_read(ret_addr);
    pc &= 0x00ff;
    pc |= hi << 8;
}
NES_CPU_BIN_OP_MODE(0x20, &CPU::JSR, AddrMode::Absolute);

void CPU::RTS() {
    cpu_read(pc);

    cpu_read(0x100 | sp);
    uint8_t lo = pull();
    pc &= 0xff00;
    pc |= lo;

    uint8_t hi = pull();
    pc &= 0x00ff;
    pc |= hi << 8;

    cpu_read(pc);
    ++pc;
}
NES_CPU_BIN_OP_MODE(0x60, &CPU::RTS, AddrMode::Implied);

void CPU::BRK() {
    inst_read();
    push(pc >> 8);
    push(pc & 0xff);
    push(p.to_ulong() | (1 << BREAK_BIT)); // BRK pushes status with B set
    
    bool nmi = nmi_latch;
    nmi_latch = false;
    
    p[INTERRUPT_DISABLE_BIT] = true;

    uint16_t vector = nmi ? 0xfffa : 0xfffe;

    // end of cycle 5
    uint8_t lo = cpu_read(vector);
    pc &= 0xff00;
    pc |= lo;
    uint8_t hi = cpu_read(vector + 1);
    pc &= 0x00ff;
    pc |= hi << 8;

    // do not poll interrupts on interrupt
    interrupt_polling = false;
}
NES_CPU_BIN_OP_MODE(0x00, &CPU::BRK, AddrMode::Implied);

void CPU::RTI() {
    cpu_read(pc);

    cpu_read(0x100 | sp);
    uint8_t new_p = pull();
    p = new_p | (1 << UNUSED_BIT);

    uint8_t lo = pull();
    pc &= 0xff00;
    pc |= lo;

    uint8_t hi = pull();
    pc &= 0x00ff;
    pc |= hi << 8;
}
NES_CPU_BIN_OP_MODE(0x40, &CPU::RTI, AddrMode::Implied);

void CPU::interrupt() {
    cpu_read(pc);
    cpu_read(pc);

    push(pc >> 8);
    push(pc & 0xff);
    push(p.to_ulong() & ~(1 << BREAK_BIT));
    
    
    bool nmi = nmi_latch;
    nmi_pending = false;
    nmi_latch = false;

    p[INTERRUPT_DISABLE_BIT] = true;

    uint16_t vector = nmi ? 0xfffa : 0xfffe;
    uint8_t lo = cpu_read(vector);
    pc &= 0xff00;
    pc |= lo;
    uint8_t hi = cpu_read(vector + 1);
    pc &= 0x00ff;
    pc |= hi << 8;

    // do not poll interrupts on interrupt
    interrupt_polling = false;
}

void CPU::PHA() {
    cpu_read(pc);
    push(a);
}
NES_CPU_BIN_OP_MODE(0x48, &CPU::PHA, AddrMode::Implied);

void CPU::PLA() {
    cpu_read(pc);
    uint8_t v = pull();
    cpu_read(0x100 | sp);
    a = v;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x68, &CPU::PLA, AddrMode::Implied);

void CPU::PHP() {
    cpu_read(pc);
    push(p.to_ulong() | (1 << BREAK_BIT));
}
NES_CPU_BIN_OP_MODE(0x08, &CPU::PHP, AddrMode::Implied);

void CPU::PLP() {
    cpu_read(pc);
    uint8_t v = pull();
    cpu_read(0x100 | sp);

    // edge case: CLI, SEI, PLP change the I flag after polling for interrupts
    poll_interrupts();
    interrupt_polling = false;

    p = (v & ~(1 << BREAK_BIT)) | (1 << UNUSED_BIT);
}
NES_CPU_BIN_OP_MODE(0x28, &CPU::PLP, AddrMode::Implied);

void CPU::TXS() {
    cpu_read(pc);
    sp = x;
}
NES_CPU_BIN_OP_MODE(0x9a, &CPU::TXS, AddrMode::Implied);

void CPU::TSX() {
    cpu_read(pc);
    x = sp;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xba, &CPU::TSX, AddrMode::Implied);

void CPU::CLC() {
    cpu_read(pc);
    p[CARRY_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0x18, &CPU::CLC, AddrMode::Implied);

void CPU::SEC() {
    cpu_read(pc);
    p[CARRY_BIT] = true;
}
NES_CPU_BIN_OP_MODE(0x38, &CPU::SEC, AddrMode::Implied);

void CPU::CLI() {
    cpu_read(pc);

    // edge case: CLI, SEI, PLP change the I flag after polling for interrupts
    poll_interrupts();
    interrupt_polling = false;
    
    p[INTERRUPT_DISABLE_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0x58, &CPU::CLI, AddrMode::Implied);

void CPU::SEI() {
    cpu_read(pc);

    // edge case: CLI, SEI, PLP change the I flag after polling for interrupts
    poll_interrupts();
    interrupt_polling = false;

    p[INTERRUPT_DISABLE_BIT] = true;
}
NES_CPU_BIN_OP_MODE(0x78, &CPU::SEI, AddrMode::Implied);

void CPU::CLD() {
    cpu_read(pc);
    p[DECIMAL_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0xd8, &CPU::CLD, AddrMode::Implied);

void CPU::SED() {
    cpu_read(pc);
    p[DECIMAL_BIT] = true;
}
NES_CPU_BIN_OP_MODE(0xf8, &CPU::SED, AddrMode::Implied);

void CPU::CLV() {
    cpu_read(pc);
    p[OVERFLOW_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0xb8, &CPU::CLV, AddrMode::Implied);

void CPU::NOP() {
    cpu_read(pc);
    return;
}
NES_CPU_BIN_OP_MODE(0xea, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x1a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x3a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x5a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x7a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0xda, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0xfa, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
void CPU::NOP(uint16_t addr) {
    cpu_read(addr);
    return;
}
NES_CPU_BIN_OP_MODE(0x80, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x82, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xc2, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xe2, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x89, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0x04, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x44, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x64, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x14, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x34, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x54, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x74, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xd4, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xf4, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x0c, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x1c, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x3c, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x5c, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x7c, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0xdc, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0xfc, static_cast<void (CPU::*)(uint16_t)>(&CPU::NOP), AddrMode::AbsoluteX);

void CPU::ALR(uint16_t addr) {
    a &= cpu_read(addr);
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a >>= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x4b, &CPU::ALR, AddrMode::Immediate);

void CPU::ANC(uint16_t addr) {
    a &= cpu_read(addr);
    set_zn(a);
    p[CARRY_BIT] = p[NEGATIVE_BIT];
}
NES_CPU_BIN_OP_MODE(0x0b, &CPU::ANC, AddrMode::Immediate);

void CPU::ARR(uint16_t addr) {
    a &= cpu_read(addr);
    set_zn(a);

    uint8_t t = a >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    a = t;

    set_zn(a);
    p[CARRY_BIT] = (a & (1 << 6)) == 0;
    p[OVERFLOW_BIT] = ((a & (1 << 6)) ^ (a & (1 << 5))) == 0;
}
NES_CPU_BIN_OP_MODE(0x6b, &CPU::ARR, AddrMode::Immediate);

void CPU::AXS(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    p[CARRY_BIT] = (a & x) >= v;
    x = (a & x) - v;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xcb, &CPU::AXS, AddrMode::Immediate);

void CPU::LAX(uint16_t addr) {
    a = cpu_read(addr);
    x = a;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xa7, &CPU::LAX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb7, &CPU::LAX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0xaf, &CPU::LAX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbf, &CPU::LAX, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0xa3, &CPU::LAX, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0xb3, &CPU::LAX, AddrMode::IndirectY);

void CPU::SAX(uint16_t addr) {
    cpu_write(addr, a & x);
}
NES_CPU_BIN_OP_MODE(0x87, &CPU::SAX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x97, &CPU::SAX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0x8f, &CPU::SAX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x83, &CPU::SAX, AddrMode::IndirectX);

void CPU::DCP(uint16_t addr) {
    uint8_t t = cpu_read(addr);
    cpu_write(addr, t--);
    cpu_write(addr, t);
    p[CARRY_BIT] = a >= t;
    set_zn(a - t);
}
NES_CPU_BIN_OP_MODE(0xc7, &CPU::DCP, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xd7, &CPU::DCP, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xcf, &CPU::DCP, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xdf, &CPU::DCP, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE_RMW(0xdb, &CPU::DCP, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0xc3, &CPU::DCP, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0xd3, &CPU::DCP, AddrMode::IndirectY);

void CPU::ISC(uint16_t addr) {
    uint8_t t = cpu_read(addr);
    cpu_write(addr, t++);
    cpu_write(addr, t);
    uint16_t r = a + ~t + p[CARRY_BIT];
    p[CARRY_BIT] = r <= 0xff; // carry
    p[OVERFLOW_BIT] = (r ^ a) & (r ^ ~t) & (1 << NEGATIVE_BIT); // overflow
    a = r;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0xe7, &CPU::ISC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xf7, &CPU::ISC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xef, &CPU::ISC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xff, &CPU::ISC, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE_RMW(0xfb, &CPU::ISC, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0xe3, &CPU::ISC, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0xf3, &CPU::ISC, AddrMode::IndirectY);

void CPU::RLA(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    uint8_t t = v << 1;
    t |= p[CARRY_BIT];
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    set_zn(t);
    cpu_write(addr, t);
    a &= t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x27, &CPU::RLA, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x37, &CPU::RLA, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x2f, &CPU::RLA, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x3f, &CPU::RLA, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE_RMW(0x3b, &CPU::RLA, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x23, &CPU::RLA, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x33, &CPU::RLA, AddrMode::IndirectY);

void CPU::RRA(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    uint8_t t = v >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    set_zn(t);
    cpu_write(addr, t);
    uint16_t r = a + t + p[CARRY_BIT];
    p[CARRY_BIT] = r > 0xff; // carry;
    p[OVERFLOW_BIT] = (r ^ a) & (r ^ t) & (1 << NEGATIVE_BIT); // overflow
    a = r;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x67, &CPU::RRA, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x77, &CPU::RRA, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x6f, &CPU::RRA, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x7f, &CPU::RRA, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE_RMW(0x7b, &CPU::RRA, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x63, &CPU::RRA, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x73, &CPU::RRA, AddrMode::IndirectY);

void CPU::SLO(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    v <<= 1;
    set_zn(v);
    cpu_write(addr, v);
    a |= v;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x07, &CPU::SLO, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x17, &CPU::SLO, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x0f, &CPU::SLO, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x1f, &CPU::SLO, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE_RMW(0x1b, &CPU::SLO, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x03, &CPU::SLO, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x13, &CPU::SLO, AddrMode::IndirectY);

void CPU::SRE(uint16_t addr) {
    uint8_t v = cpu_read(addr);
    cpu_write(addr, v);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    v >>= 1;
    set_zn(v);
    cpu_write(addr, v);
    a ^= v;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x47, &CPU::SRE, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x57, &CPU::SRE, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x4f, &CPU::SRE, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x5f, &CPU::SRE, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE_RMW(0x5b, &CPU::SRE, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x43, &CPU::SRE, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x53, &CPU::SRE, AddrMode::IndirectY);

template<size_t... Is>
constexpr auto CPU::make_jump_table(std::index_sequence<Is...>) {
    return std::array<void (CPU::*)(), sizeof...(Is)>{&CPU::opcode_impl<Is>...};
}

void CPU::exec_inst() {
    static constexpr auto jump_table = CPU::make_jump_table(std::make_index_sequence<0x100>{});
    
    if (nmi_pending || irq_pending) {
        interrupt();
    }
    else {
        interrupt_polling = true;
        
        
            

        uint8_t inst = inst_read();
        (this->*(jump_table[inst]))();

        if (interrupt_polling)
            poll_interrupts();
    }
}

} // namespace nes_em