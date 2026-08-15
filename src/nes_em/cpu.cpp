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
    cycles(0)
{}

void CPU::reset() {
    cycle();
    
    pc = 0xfffc;
    cycle();

    --sp;
    cycle();

    --sp;
    cycle();

    --sp;
    uint8_t lo = inst_read();

    p[INTERRUPT_DISABLE_BIT] = true;
    pc = lo | inst_read() << 8;

    cycle();
}


void CPU::cycle() {
    ppu->cycle();
    ppu->cycle();
    ppu->cycle();
    ++cycles;
}

void CPU::poll_interrupts() {
    if (nmi_line && !nmi_line_prev)
        nmi_pending = true;

    nmi_line_prev = nmi_line;

    irq_pending = irq_line;
}

uint8_t CPU::inst_read() {
    uint8_t temp = cycle_read(pc++);
    return temp;
}

uint8_t CPU::cycle_read(uint16_t addr) {
    cycle();
    return bus->cpu_read(addr);
}

void CPU::cycle_write(uint16_t addr, uint8_t value) {
    cycle();
    bus->cpu_write(addr, value);
}

void CPU::push(uint8_t value) {
    cycle();
    bus->cpu_write(0x100 | sp--, value);
}

uint8_t CPU::pull() {
    cycle();
    return bus->cpu_read(0x100 | ++sp);
}

void CPU::set_zn(uint8_t value) {
    p[ZERO_BIT] = value == 0;
    p[NEGATIVE_BIT] = value & (1 << NEGATIVE_BIT);
}

void CPU::branch(bool cond, uint8_t offset) {
    if (cond) {
        // intentional bug: interrupts NOT polled if page not crossed
        // this is the end of cycle 2 for all branch instructions.
        // as the 2nd to last cycle for taken branches, there should
        // be an interrupt poll just before end of cycle 2, but
        // a hardware quirk prevents this.
        cycle();

        // pretty sure this is impl defined pre c++20
        // maybe fix later
        uint16_t next_pc = pc + static_cast<int8_t>(offset);
        
        if ((next_pc & 0xff00) != (pc & 0xff00)) {
            // interrupts ARE polled on page crosses.
            poll_interrupts();
            cycle();
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

        cycle();
        addr += x;

        (this->*op)(addr);
    }
    else if constexpr (mode == AddrMode::ZeroPageY) {
        uint8_t addr = inst_read();

        cycle();
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
        uint16_t addr = lo | inst_read() << 8;
        if ((lo + x) & 0x100 || rmw)
            cycle();
        (this->*op)(addr + x);
    }
    else if constexpr (mode == AddrMode::AbsoluteY) {
        uint8_t lo = inst_read();
        uint16_t addr = lo | inst_read() << 8;
        if ((lo + y) & 0x100 || rmw)
            cycle();
        (this->*op)(addr + y);
    }
    else if constexpr (mode == AddrMode::Relative) {
        // only used by branch instructions
        (this->*op)(pc++);
    }
    else if constexpr (mode == AddrMode::IndirectX) {
        uint8_t ind = inst_read();
        
        cycle();
        ind += x;

        uint8_t lo = cycle_read(ind++);
        (this->*op)(lo | cycle_read(ind) << 8);
    }
    else if constexpr (mode == AddrMode::IndirectY) {
        uint8_t ind = inst_read();
        uint8_t lo = cycle_read(ind++);
        uint16_t addr = lo | cycle_read(ind) << 8;
        if ((lo + y) & 0x100)
            cycle();
        (this->*op)(addr + y);
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
    poll_interrupts();
    a = cycle_read(addr);
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
    poll_interrupts();
    cycle_write(addr, a);
}

template <>
void CPU::opcode_impl<&CPU::STA, AddrMode::AbsoluteX>() {
    uint8_t lo = inst_read();
    uint16_t addr = lo | inst_read() << 8;
    cycle(); // always dummy read on store inst
    STA(addr + x);
};

template <>
void CPU::opcode_impl<&CPU::STA, AddrMode::AbsoluteY>() {
    uint8_t lo = inst_read();
    uint16_t addr = lo | inst_read() << 8;
    cycle(); // always dummy read on store inst
    STA(addr + y);
};

template <>
void CPU::opcode_impl<&CPU::STA, AddrMode::IndirectY>() {
    uint8_t ind = inst_read();
    uint8_t lo = cycle_read(ind++);
    uint16_t addr = lo | cycle_read(ind) << 8;
    cycle(); // always dummy read on store inst
    STA(addr + y);
};

NES_CPU_BIN_OP_MODE(0x85, &CPU::STA, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x95, &CPU::STA, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x8d, &CPU::STA, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x9d, &CPU::STA, AddrMode::AbsoluteX);
NES_CPU_BIN_OP_MODE(0x99, &CPU::STA, AddrMode::AbsoluteY);
NES_CPU_BIN_OP_MODE(0x81, &CPU::STA, AddrMode::IndirectX);
NES_CPU_BIN_OP_MODE(0x91, &CPU::STA, AddrMode::IndirectY);

void CPU::LDX(uint16_t addr) {
    poll_interrupts();
    x = cycle_read(addr);
    set_zn(x);
}

NES_CPU_BIN_OP_MODE(0xa2, &CPU::LDX, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa6, &CPU::LDX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb6, &CPU::LDX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0xae, &CPU::LDX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbe, &CPU::LDX, AddrMode::AbsoluteY);

void CPU::STX(uint16_t addr) {
    poll_interrupts();
    cycle_write(addr, x);
}
NES_CPU_BIN_OP_MODE(0x86, &CPU::STX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x96, &CPU::STX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0x8e, &CPU::STX, AddrMode::Absolute);

void CPU::LDY(uint16_t addr) {
    poll_interrupts();
    y = cycle_read(addr);
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xa0, &CPU::LDY, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa4, &CPU::LDY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb4, &CPU::LDY, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xac, &CPU::LDY, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbc, &CPU::LDY, AddrMode::AbsoluteX);

void CPU::STY(uint16_t addr) {
    poll_interrupts();
    cycle_write(addr, y);
}
NES_CPU_BIN_OP_MODE(0x84, &CPU::STY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x94, &CPU::STY, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x8c, &CPU::STY, AddrMode::Absolute);

void CPU::TAX() {
    poll_interrupts();
    cycle();
    x = a;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xaa, &CPU::TAX, AddrMode::Implied);

void CPU::TXA() {
    poll_interrupts();
    cycle();
    a = x;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x8a, &CPU::TXA, AddrMode::Implied);

void CPU::TAY() {
    poll_interrupts();
    cycle();
    y = a;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xa8, &CPU::TAY, AddrMode::Implied);

void CPU::TYA() {
    poll_interrupts();
    cycle();
    a = y;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x98, &CPU::TYA, AddrMode::Implied);

void CPU::ADC(uint16_t addr) {
    poll_interrupts();
    uint8_t v = cycle_read(addr);
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
    poll_interrupts();
    uint8_t v = cycle_read(addr);
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
    uint8_t t = cycle_read(addr);
    cycle_write(addr, t++);
    poll_interrupts();
    cycle_write(addr, t);
    set_zn(t);
}
NES_CPU_BIN_OP_MODE(0xe6, &CPU::INC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xf6, &CPU::INC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xee, &CPU::INC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xfe, &CPU::INC, AddrMode::AbsoluteX);

void CPU::DEC(uint16_t addr) {
    uint8_t t = cycle_read(addr);
    cycle_write(addr, t--);
    poll_interrupts();
    cycle_write(addr, t);
    set_zn(t);
}

NES_CPU_BIN_OP_MODE(0xc6, &CPU::DEC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xd6, &CPU::DEC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xce, &CPU::DEC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xde, &CPU::DEC, AddrMode::AbsoluteX);

void CPU::INX() {
    poll_interrupts();
    cycle();
    ++x;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xe8, &CPU::INX, AddrMode::Implied);

void CPU::DEX() {
    poll_interrupts();
    cycle();
    --x;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xca, &CPU::DEX, AddrMode::Implied);

void CPU::INY() {
    poll_interrupts();
    cycle();
    ++y;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xc8, &CPU::INY, AddrMode::Implied);

void CPU::DEY() {
    poll_interrupts();
    cycle();
    --y;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0x88, &CPU::DEY, AddrMode::Implied);

void CPU::ASL() {
    poll_interrupts();
    cycle();
    p[CARRY_BIT] = a & (1 << NEGATIVE_BIT);
    a <<= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x0a, static_cast<void (CPU::*)()>(&CPU::ASL), AddrMode::Accumulator);
void CPU::ASL(uint16_t addr) {
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    v <<= 1;
    set_zn(v);
    poll_interrupts();
    cycle_write(addr, v);
}
NES_CPU_BIN_OP_MODE(0x06, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x16, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x0e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x1e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::AbsoluteX);

void CPU::LSR() {
    poll_interrupts();
    cycle();
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a >>= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x4a, static_cast<void (CPU::*)()>(&CPU::LSR), AddrMode::Accumulator);
void CPU::LSR(uint16_t addr) {
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    v >>= 1;
    set_zn(v);
    poll_interrupts();
    cycle_write(addr, v);
}

NES_CPU_BIN_OP_MODE(0x46, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x56, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x4e, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x5e, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::AbsoluteX);

void CPU::ROL() {
    poll_interrupts();
    cycle();
    uint8_t t = a << 1;
    t |= p[CARRY_BIT];
    p[CARRY_BIT] = a & (1 << NEGATIVE_BIT);
    a = t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x2a, static_cast<void (CPU::*)()>(&CPU::ROL), AddrMode::Accumulator);
void CPU::ROL(uint16_t addr) {
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    uint8_t t = v << 1;
    t |= p[CARRY_BIT];
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    set_zn(t);
    poll_interrupts();
    cycle_write(addr, t);
}

NES_CPU_BIN_OP_MODE(0x26, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x36, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x2e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x3e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::AbsoluteX);

void CPU::ROR() {
    poll_interrupts();
    cycle();
    uint8_t t = a >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a = t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x6a, static_cast<void (CPU::*)()>(&CPU::ROR), AddrMode::Accumulator);
void CPU::ROR(uint16_t addr) {
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    uint8_t t = v >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    set_zn(t);
    poll_interrupts();
    cycle_write(addr, t);
}

NES_CPU_BIN_OP_MODE(0x66, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x76, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x6e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x7e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::AbsoluteX);

void CPU::AND(uint16_t addr) {
    poll_interrupts();
    a &= cycle_read(addr);
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
    poll_interrupts();
    a |= cycle_read(addr);
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
    poll_interrupts();
    a ^= cycle_read(addr);
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
    poll_interrupts();
    uint8_t v = cycle_read(addr);
    p[ZERO_BIT] = (a & v) == 0; 
    p[OVERFLOW_BIT] = v & (1 << OVERFLOW_BIT);
    p[NEGATIVE_BIT] = v & (1 << NEGATIVE_BIT);
}
NES_CPU_BIN_OP_MODE(0x24, &CPU::BIT, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x2c, &CPU::BIT, AddrMode::Absolute);

void CPU::CMP(uint16_t addr) {
    poll_interrupts();
    uint8_t v = cycle_read(addr);
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
    poll_interrupts();
    uint8_t v = cycle_read(addr);
    p[CARRY_BIT] = x >= v;
    set_zn(x - v);
}
NES_CPU_BIN_OP_MODE(0xe0, &CPU::CPX, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xe4, &CPU::CPX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xec, &CPU::CPX, AddrMode::Absolute);

void CPU::CPY(uint16_t addr) {
    poll_interrupts();
    uint8_t v = cycle_read(addr);
    p[CARRY_BIT] = y >= v;
    set_zn(y - v);
}
NES_CPU_BIN_OP_MODE(0xc0, &CPU::CPY, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xc4, &CPU::CPY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xcc, &CPU::CPY, AddrMode::Absolute);

void CPU::BCC(uint16_t addr) {
    poll_interrupts();
    branch(!p[CARRY_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0x90, &CPU::BCC, AddrMode::Relative);

void CPU::BCS(uint16_t addr) {
    poll_interrupts();
    branch(p[CARRY_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0xb0, &CPU::BCS, AddrMode::Relative);

void CPU::BEQ(uint16_t addr) {
    poll_interrupts();
    branch(p[ZERO_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0xf0, &CPU::BEQ, AddrMode::Relative);

void CPU::BNE(uint16_t addr) {
    poll_interrupts();
    branch(!p[ZERO_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0xd0, &CPU::BNE, AddrMode::Relative);

void CPU::BPL(uint16_t addr) {
    poll_interrupts();
    branch(!p[NEGATIVE_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0x10, &CPU::BPL, AddrMode::Relative);

void CPU::BMI(uint16_t addr) {
    poll_interrupts();
    branch(p[NEGATIVE_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0x30, &CPU::BMI, AddrMode::Relative);

void CPU::BVC(uint16_t addr) {
    poll_interrupts();
    branch(!p[OVERFLOW_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0x50, &CPU::BVC, AddrMode::Relative);

void CPU::BVS(uint16_t addr) {
    poll_interrupts();
    branch(p[OVERFLOW_BIT], cycle_read(addr));
}
NES_CPU_BIN_OP_MODE(0x70, &CPU::BVS, AddrMode::Relative);

void CPU::JMP(uint16_t addr) {
    pc = addr; //all cycles already accounted for
}

template <>
void CPU::opcode_impl<&CPU::JMP, AddrMode::Absolute>() {
    uint16_t addr = inst_read();
    poll_interrupts();
    addr |= inst_read() << 8;
    JMP(addr);
}

template <>
void CPU::opcode_impl<&CPU::JMP, AddrMode::Indirect>() {
    // only used by JMP.
    uint8_t lo = inst_read();
    uint8_t hi = inst_read();

    uint16_t addr = cycle_read(lo | hi << 8);
    // intentional bug: page crossing is broken
    ++lo;
    poll_interrupts();
    addr |= cycle_read(lo | hi << 8) << 8;

    JMP(addr);
}

NES_CPU_BIN_OP_MODE(0x4c, &CPU::JMP, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x6c, &CPU::JMP, AddrMode::Indirect);

void CPU::JSR(uint16_t addr) {
    uint16_t ret_addr = pc - 1;
    push(ret_addr >> 8);
    push(ret_addr & 0xff);
    poll_interrupts();
    cycle();
    pc = addr;   
}
NES_CPU_BIN_OP_MODE(0x20, &CPU::JSR, AddrMode::Absolute);

void CPU::RTS() {
    cycle_read(pc);

    uint8_t lo = pull();
    uint8_t hi = pull();

    pc &= 0xff00;
    pc |= lo;

    cycle();
    pc &= 0x00ff;
    pc |= hi << 8;

    poll_interrupts();
    cycle();
    ++pc;
}
NES_CPU_BIN_OP_MODE(0x60, &CPU::RTS, AddrMode::Implied);

void CPU::BRK() {
    inst_read();

    push(pc >> 8);
    push(pc & 0xff);

    // this line is the end of cycle 4 and start of cycle 5:
    push(p.to_ulong() | (1 << BREAK_BIT)); // BRK pushes status with B set

    // same hijacking as interrupt(): the vector isn't chosen until right
    // after the status push, so a pending NMI can hijack a BRK in flight
    bool nmi = nmi_pending;
    nmi_pending = false;

    p[INTERRUPT_DISABLE_BIT] = true;

    uint16_t vector = nmi ? 0xfffa : 0xfffe;

    // end of cycle 5
    uint8_t lo = cycle_read(vector);
    pc &= 0xff00;
    pc |= lo;
    uint8_t hi = cycle_read(vector + 1);
    pc &= 0x00ff;
    pc |= hi << 8;
}
NES_CPU_BIN_OP_MODE(0x00, &CPU::BRK, AddrMode::Implied);

void CPU::RTI() {
    cycle_read(pc);

    uint8_t new_p = pull();

    uint8_t lo = pull();
    p = new_p | (1 << UNUSED_BIT);

    uint8_t hi = pull();
    pc &= 0xff00;
    pc |= lo;

    poll_interrupts();
    cycle();
    pc &= 0x00ff;
    pc |= hi << 8;
}
NES_CPU_BIN_OP_MODE(0x40, &CPU::RTI, AddrMode::Implied);

void CPU::interrupt() {
    cycle();

    push(pc >> 8);
    push(pc & 0xff);
    push(p.to_ulong() & ~(1 << BREAK_BIT));

    bool nmi = nmi_pending;
    nmi_pending = false;

    p[INTERRUPT_DISABLE_BIT] = true;

    uint16_t vector = nmi ? 0xfffa : 0xfffe;
    uint8_t lo = cycle_read(vector);
    pc &= 0xff00;
    pc |= lo;
    uint8_t hi = cycle_read(vector + 1);
    pc &= 0x00ff;
    pc |= hi << 8;

    cycle();
}

void CPU::PHA() {
    cycle_read(pc);
    poll_interrupts();
    push(a);
}
NES_CPU_BIN_OP_MODE(0x48, &CPU::PHA, AddrMode::Implied);

void CPU::PLA() {
    cycle_read(pc);
    uint8_t v = pull();
    poll_interrupts();
    cycle();
    a = v;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x68, &CPU::PLA, AddrMode::Implied);

void CPU::PHP() {
    cycle_read(pc);
    poll_interrupts();
    push(p.to_ulong() | (1 << BREAK_BIT));
}
NES_CPU_BIN_OP_MODE(0x08, &CPU::PHP, AddrMode::Implied);

void CPU::PLP() {
    cycle_read(pc);
    uint8_t v = pull();
    poll_interrupts();
    cycle();
    p = (v & ~(1 << BREAK_BIT)) | (1 << UNUSED_BIT);
}
NES_CPU_BIN_OP_MODE(0x28, &CPU::PLP, AddrMode::Implied);

void CPU::TXS() {
    poll_interrupts();
    cycle();
    sp = x;
}
NES_CPU_BIN_OP_MODE(0x9a, &CPU::TXS, AddrMode::Implied);

void CPU::TSX() {
    poll_interrupts();
    cycle();
    x = sp;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xba, &CPU::TSX, AddrMode::Implied);

void CPU::CLC() {
    poll_interrupts();
    cycle();
    p[CARRY_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0x18, &CPU::CLC, AddrMode::Implied);

void CPU::SEC() {
    poll_interrupts();
    cycle();
    p[CARRY_BIT] = true;
}
NES_CPU_BIN_OP_MODE(0x38, &CPU::SEC, AddrMode::Implied);

void CPU::CLI() {
    poll_interrupts();
    cycle();
    p[INTERRUPT_DISABLE_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0x58, &CPU::CLI, AddrMode::Implied);

void CPU::SEI() {
    poll_interrupts();
    cycle();
    p[INTERRUPT_DISABLE_BIT] = true;
}
NES_CPU_BIN_OP_MODE(0x78, &CPU::SEI, AddrMode::Implied);

void CPU::CLD() {
    poll_interrupts();
    cycle();
    p[DECIMAL_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0xd8, &CPU::CLD, AddrMode::Implied);

void CPU::SED() {
    poll_interrupts();
    cycle();
    p[DECIMAL_BIT] = true;
}
NES_CPU_BIN_OP_MODE(0xf8, &CPU::SED, AddrMode::Implied);

void CPU::CLV() {
    poll_interrupts();
    cycle();
    p[OVERFLOW_BIT] = false;
}
NES_CPU_BIN_OP_MODE(0xb8, &CPU::CLV, AddrMode::Implied);

void CPU::NOP() {
    poll_interrupts();
    cycle();
    return;
}
NES_CPU_BIN_OP_MODE(0xea, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x1a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x3a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x5a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0x7a, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0xda, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
NES_CPU_BIN_OP_MODE(0xfa, static_cast<void (CPU::*)()>(&CPU::NOP), AddrMode::Implied);
void CPU::NOP(uint16_t) {
    poll_interrupts();
    cycle();
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
    poll_interrupts();
    a &= cycle_read(addr);
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a >>= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x4b, &CPU::ALR, AddrMode::Immediate);

void CPU::ANC(uint16_t addr) {
    poll_interrupts();
    a &= cycle_read(addr);
    set_zn(a);
    p[CARRY_BIT] = p[NEGATIVE_BIT];
}
NES_CPU_BIN_OP_MODE(0x0b, &CPU::ANC, AddrMode::Immediate);

void CPU::ARR(uint16_t addr) {
    poll_interrupts();
    a &= cycle_read(addr);
    uint8_t t = a >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = a & (1 << CARRY_BIT);
    a = t;
    set_zn(a);
    p[CARRY_BIT] = a & (1 << UNUSED_BIT);
    p[OVERFLOW_BIT] = (a & (1 << UNUSED_BIT)) ^ (a & (1 << BREAK_BIT));
}
NES_CPU_BIN_OP_MODE(0x6b, &CPU::ARR, AddrMode::Immediate);

void CPU::AXS(uint16_t addr) {
    poll_interrupts();
    uint8_t v = cycle_read(addr);
    p[CARRY_BIT] = (a & x) >= v;
    x = (a & x) - v;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xcb, &CPU::AXS, AddrMode::Immediate);

void CPU::LAX(uint16_t addr) {
    poll_interrupts();
    a = cycle_read(addr);
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
    poll_interrupts();
    cycle_write(addr, a & x);
}
NES_CPU_BIN_OP_MODE(0x87, &CPU::SAX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x97, &CPU::SAX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0x8f, &CPU::SAX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x83, &CPU::SAX, AddrMode::IndirectX);

void CPU::DCP(uint16_t addr) {
    uint8_t t = cycle_read(addr);
    cycle_write(addr, t--);
    poll_interrupts();
    cycle_write(addr, t);
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
    uint8_t t = cycle_read(addr);
    cycle_write(addr, t++);
    poll_interrupts();
    cycle_write(addr, t);
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
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    uint8_t t = v << 1;
    t |= p[CARRY_BIT];
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    set_zn(t);
    poll_interrupts();
    cycle_write(addr, t);
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
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    uint8_t t = v >> 1;
    t |= (p[CARRY_BIT] << NEGATIVE_BIT);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    set_zn(t);
    poll_interrupts();
    cycle_write(addr, t);
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
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    p[CARRY_BIT] = v & (1 << NEGATIVE_BIT);
    v <<= 1;
    set_zn(v);
    poll_interrupts();
    cycle_write(addr, v);
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
    uint8_t v = cycle_read(addr);
    cycle_write(addr, v);
    p[CARRY_BIT] = v & (1 << CARRY_BIT);
    v >>= 1;
    set_zn(v);
    poll_interrupts();
    cycle_write(addr, v);
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

    if (nmi_pending || (!p[INTERRUPT_DISABLE_BIT] && irq_pending)) {
        interrupt();
        return;
    }

    uint8_t inst = bus->cpu_read(pc++);
    (this->*(jump_table[inst]))();
    cycle();
}

} // namespace nes_em