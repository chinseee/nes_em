#include "nes_em/cpu.h"
#include <array>
#include <random>
#include <cstring>
#include <random>

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
    p(0b00100100),
    cycles(0) {

}

void CPU::reset() {
    cycle();
    cycle();
    pc = 0xfffc;
    cycle();
    --sp;
    cycle();
    --sp;
    cycle();
    --sp;
    
    uint8_t lo = inst_read();
    p[2] = true;
    pc = lo | inst_read() << 8;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist;

    for (uint16_t i = 0; i < 0x0800; ++i)
        mem[i] = dist(gen);
    for (uint16_t i = 0x6000; i < 0x8000; ++i)
        mem[i] = dist(gen);
}

void CPU::load(const NesFile &file) {
    size_t prg_rom_size = file.prg_rom.size();
    if (prg_rom_size > 0x8000)
        throw std::runtime_error("PRG-ROM too large: max size of 32768 bytes exceeded");
    std::memcpy(mem + 0x8000, file.prg_rom.data(), prg_rom_size);
    if (prg_rom_size <= 0x4000)
        std::memcpy(mem + 0xc000, file.prg_rom.data(), prg_rom_size);
}

void CPU::cycle() {
    ++cycles;
}

uint8_t CPU::inst_read() {
    return mem_read(pc++);
}

uint16_t CPU::unmirror_addr(uint16_t addr) {
    if (addr < 0x2000)
        return addr & 0x07ff;
    if (addr < 0x4000)
        return (addr & 7) | 0x2000;
    return addr;
};

uint8_t CPU::mem_read(uint16_t addr) {
    cycle();
    return mem[unmirror_addr(addr)];
}

void CPU::mem_write(uint16_t addr, uint8_t value) {
    cycle();
    mem[unmirror_addr(addr)] = value;
}

void CPU::push(uint8_t value) {
    cycle();
    mem[0x100 | sp--] = value;
}

uint8_t CPU::pull() {
    cycle();
    return mem[0x100 | ++sp];
}

void CPU::set_zn(uint8_t value) {
    p[1] = value == 0;
    p[7] = value & 0x80;
}

void CPU::branch(bool cond, uint8_t offset) {
    if (cond) {
        cycle();
        // pretty sure this is impl defined pre c++20
        uint16_t next_pc = pc + static_cast<int8_t>(offset);
        if ((next_pc & 0xff00) != (pc & 0xff00))
            cycle();
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
    else if constexpr (mode == AddrMode::Indirect) {
        // only used by JMP.
        uint8_t lo = inst_read();
        uint8_t hi = inst_read();

        uint16_t addr = mem_read(lo | hi << 8);
        // intentional bug: page crossing is broken
        ++lo;
        addr |= mem_read(lo | hi << 8) << 8;
        
        (this->*op)(addr);
    }
    else if constexpr (mode == AddrMode::IndirectX) {
        uint8_t ind = inst_read();
        
        cycle();
        ind += x;

        uint8_t lo = mem_read(ind++);
        (this->*op)(lo | mem_read(ind) << 8);
    }
    else if constexpr (mode == AddrMode::IndirectY) {
        uint8_t ind = inst_read();
        uint8_t lo = mem_read(ind++);
        uint16_t addr = lo | mem_read(ind) << 8;
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
    a = mem_read(addr);
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
    mem_write(addr, a);
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
    uint8_t lo = mem_read(ind++);
    uint16_t addr = lo | mem_read(ind) << 8;
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
    x = mem_read(addr);
    set_zn(x);
}

NES_CPU_BIN_OP_MODE(0xa2, &CPU::LDX, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa6, &CPU::LDX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb6, &CPU::LDX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0xae, &CPU::LDX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbe, &CPU::LDX, AddrMode::AbsoluteY);

void CPU::STX(uint16_t addr) {
    mem_write(addr, x);
}
NES_CPU_BIN_OP_MODE(0x86, &CPU::STX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x96, &CPU::STX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0x8e, &CPU::STX, AddrMode::Absolute);

void CPU::LDY(uint16_t addr) {
    y = mem_read(addr);
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xa0, &CPU::LDY, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xa4, &CPU::LDY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xb4, &CPU::LDY, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xac, &CPU::LDY, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0xbc, &CPU::LDY, AddrMode::AbsoluteX);

void CPU::STY(uint16_t addr) {
    mem_write(addr, y);
}
NES_CPU_BIN_OP_MODE(0x84, &CPU::STY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x94, &CPU::STY, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x8c, &CPU::STY, AddrMode::Absolute);

void CPU::TAX() {
    cycle();
    x = a;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xaa, &CPU::TAX, AddrMode::Implied);

void CPU::TXA() {
    cycle();
    a = x;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x8a, &CPU::TXA, AddrMode::Implied);

void CPU::TAY() {
    cycle();
    y = a;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xa8, &CPU::TAY, AddrMode::Implied);

void CPU::TYA() {
    cycle();
    a = y;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x98, &CPU::TYA, AddrMode::Implied);

void CPU::ADC(uint16_t addr) {
    uint8_t v = mem_read(addr);
    uint16_t r = a + v + p[0];
    p[0] = r > 0xff; // carry;
    p[6] = (r ^ a) & (r ^ v) & 0x80; // overflow
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
    uint8_t v = mem_read(addr);
    uint16_t r = a + ~v + p[0];
    p[0] = r <= 0xff; // carry
    p[6] = (r ^ a) & (r ^ ~v) & 0x80; // overflow
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
    uint8_t t = mem_read(addr);
    mem_write(addr, t++);
    mem_write(addr, t);
    set_zn(t);
}
NES_CPU_BIN_OP_MODE(0xe6, &CPU::INC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xf6, &CPU::INC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xee, &CPU::INC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xfe, &CPU::INC, AddrMode::AbsoluteX);

void CPU::DEC(uint16_t addr) {
    uint8_t t = mem_read(addr);
    mem_write(addr, t--);
    mem_write(addr, t);
    set_zn(t);
}

NES_CPU_BIN_OP_MODE(0xc6, &CPU::DEC, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xd6, &CPU::DEC, AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0xce, &CPU::DEC, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0xde, &CPU::DEC, AddrMode::AbsoluteX);

void CPU::INX() {
    cycle();
    ++x;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xe8, &CPU::INX, AddrMode::Implied);

void CPU::DEX() {
    cycle();
    --x;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xca, &CPU::DEX, AddrMode::Implied);

void CPU::INY() {
    cycle();
    ++y;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0xc8, &CPU::INY, AddrMode::Implied);

void CPU::DEY() {
    cycle();
    --y;
    set_zn(y);
}
NES_CPU_BIN_OP_MODE(0x88, &CPU::DEY, AddrMode::Implied);

void CPU::ASL() {
    cycle();
    p[0] = a & 0x80;
    a <<= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x0a, static_cast<void (CPU::*)()>(&CPU::ASL), AddrMode::Accumulator);
void CPU::ASL(uint16_t addr) {
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    p[0] = v & 0x80;
    v <<= 1;
    set_zn(v);
    mem_write(addr, v);
}
NES_CPU_BIN_OP_MODE(0x06, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x16, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x0e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x1e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ASL), AddrMode::AbsoluteX);

void CPU::LSR() {
    cycle();
    p[0] = a & 0x01;
    a >>= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x4a, static_cast<void (CPU::*)()>(&CPU::LSR), AddrMode::Accumulator);
void CPU::LSR(uint16_t addr) {
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    p[0] = v & 0x01;
    v >>= 1;
    set_zn(v);
    mem_write(addr, v);
}

NES_CPU_BIN_OP_MODE(0x46, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x56, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x4e, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x5e, static_cast<void (CPU::*)(uint16_t)>(&CPU::LSR), AddrMode::AbsoluteX);

void CPU::ROL() {
    cycle();
    uint8_t t = a << 1;
    t |= p[0];
    p[0] = a & 0x80;
    a = t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x2a, static_cast<void (CPU::*)()>(&CPU::ROL), AddrMode::Accumulator);
void CPU::ROL(uint16_t addr) {
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    uint8_t t = v << 1;
    t |= p[0];
    p[0] = v & 0x80;
    set_zn(t);
    mem_write(addr, t);
}

NES_CPU_BIN_OP_MODE(0x26, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x36, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x2e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x3e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROL), AddrMode::AbsoluteX);

void CPU::ROR() {
    cycle();
    uint8_t t = a >> 1;
    t |= (p[0] << 7);
    p[0] = a & 0x01;
    a = t;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x6a, static_cast<void (CPU::*)()>(&CPU::ROR), AddrMode::Accumulator);
void CPU::ROR(uint16_t addr) {
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    uint8_t t = v >> 1;
    t |= (p[0] << 7);
    p[0] = v & 0x01;
    set_zn(t);
    mem_write(addr, t);
}

NES_CPU_BIN_OP_MODE(0x66, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x76, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::ZeroPageX);
NES_CPU_BIN_OP_MODE(0x6e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::Absolute);
NES_CPU_BIN_OP_MODE_RMW(0x7e, static_cast<void (CPU::*)(uint16_t)>(&CPU::ROR), AddrMode::AbsoluteX);

void CPU::AND(uint16_t addr) {
    a &= mem_read(addr);
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
    a |= mem_read(addr);
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
    a ^= mem_read(addr);
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
    uint8_t v = mem_read(addr);
    p[1] = (a & v) == 0; 
    p[6] = v & 0x40;
    p[7] = v & 0x80;
}
NES_CPU_BIN_OP_MODE(0x24, &CPU::BIT, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x2c, &CPU::BIT, AddrMode::Absolute);

void CPU::CMP(uint16_t addr) {
    uint8_t v = mem_read(addr);
    p[0] = a >= v;
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
    uint8_t v = mem_read(addr);
    p[0] = x >= v;
    set_zn(x - v);
}
NES_CPU_BIN_OP_MODE(0xe0, &CPU::CPX, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xe4, &CPU::CPX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xec, &CPU::CPX, AddrMode::Absolute);

void CPU::CPY(uint16_t addr) {
    uint8_t v = mem_read(addr);
    p[0] = y >= v;
    set_zn(y - v);
}
NES_CPU_BIN_OP_MODE(0xc0, &CPU::CPY, AddrMode::Immediate);
NES_CPU_BIN_OP_MODE(0xc4, &CPU::CPY, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0xcc, &CPU::CPY, AddrMode::Absolute);

void CPU::BCC(uint16_t addr) {
    branch(!p[0], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0x90, &CPU::BCC, AddrMode::Relative);

void CPU::BCS(uint16_t addr) {
    branch(p[0], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0xb0, &CPU::BCS, AddrMode::Relative);

void CPU::BEQ(uint16_t addr) {
    branch(p[1], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0xf0, &CPU::BEQ, AddrMode::Relative);

void CPU::BNE(uint16_t addr) {
    branch(!p[1], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0xd0, &CPU::BNE, AddrMode::Relative);

void CPU::BPL(uint16_t addr) {
    branch(!p[7], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0x10, &CPU::BPL, AddrMode::Relative);

void CPU::BMI(uint16_t addr) {
    branch(p[7], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0x30, &CPU::BMI, AddrMode::Relative);

void CPU::BVC(uint16_t addr) {
    branch(!p[6], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0x50, &CPU::BVC, AddrMode::Relative);

void CPU::BVS(uint16_t addr) {
    branch(p[6], mem_read(addr));
}
NES_CPU_BIN_OP_MODE(0x70, &CPU::BVS, AddrMode::Relative);

void CPU::JMP(uint16_t addr) {
    pc = addr; //all cycles already accounted for
}
NES_CPU_BIN_OP_MODE(0x4c, &CPU::JMP, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x6c, &CPU::JMP, AddrMode::Indirect);

void CPU::JSR(uint16_t addr) {
    uint16_t ret_addr = pc - 1;
    push(ret_addr >> 8);
    push(ret_addr & 0xff);
    cycle();
    pc = addr;   
}
NES_CPU_BIN_OP_MODE(0x20, &CPU::JSR, AddrMode::Absolute);

void CPU::RTS() {
    mem_read(pc);

    uint8_t lo = pull();
    uint8_t hi = pull();

    pc &= 0xff00;
    pc |= lo;

    cycle();
    pc &= 0x00ff;
    pc |= hi << 8;

    cycle();
    ++pc;
}
NES_CPU_BIN_OP_MODE(0x60, &CPU::RTS, AddrMode::Implied);

void CPU::BRK() {
    inst_read();
    
    push(pc >> 8);
    p[2] = true;
    p[4] = true;
    
    push(pc & 0xff);

    push(p.to_ulong());

    uint8_t lo = mem_read(0xfffe);
    pc &= 0xff00;
    pc |= lo;
    
    uint8_t hi = mem_read(0xffff);
    pc &= 0x00ff;
    pc |= hi;
}
NES_CPU_BIN_OP_MODE(0x00, &CPU::BRK, AddrMode::Implied);

void CPU::RTI() {
    mem_read(pc);

    uint8_t new_p = pull();

    uint8_t lo = pull();
    p = new_p | 0x20;

    uint8_t hi = pull();
    pc &= 0xff00;
    pc |= lo;

    cycle();
    pc &= 0x00ff;
    pc |= hi << 8;
}
NES_CPU_BIN_OP_MODE(0x40, &CPU::RTI, AddrMode::Implied);

void CPU::PHA() {
    mem_read(pc);
    push(a);
}
NES_CPU_BIN_OP_MODE(0x48, &CPU::PHA, AddrMode::Implied);

void CPU::PLA() {
    mem_read(pc);
    uint8_t v = pull();
    cycle();
    a = v;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x68, &CPU::PLA, AddrMode::Implied);

void CPU::PHP() {
    mem_read(pc);
    push(p.to_ulong() | 0x10);
}
NES_CPU_BIN_OP_MODE(0x08, &CPU::PHP, AddrMode::Implied);

void CPU::PLP() {
    mem_read(pc);
    uint8_t v = pull();
    cycle();
    p = (v & 0xef) | 0x20;
}
NES_CPU_BIN_OP_MODE(0x28, &CPU::PLP, AddrMode::Implied);

void CPU::TXS() {
    cycle();
    sp = x;
}
NES_CPU_BIN_OP_MODE(0x9a, &CPU::TXS, AddrMode::Implied);

void CPU::TSX() {
    cycle();
    x = sp;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xba, &CPU::TSX, AddrMode::Implied);

void CPU::CLC() {
    cycle();
    p[0] = false;
}
NES_CPU_BIN_OP_MODE(0x18, &CPU::CLC, AddrMode::Implied);

void CPU::SEC() {
    cycle();
    p[0] = true;
}
NES_CPU_BIN_OP_MODE(0x38, &CPU::SEC, AddrMode::Implied);

void CPU::CLI() {
    cycle();
    p[2] = false;
}
NES_CPU_BIN_OP_MODE(0x58, &CPU::CLI, AddrMode::Implied);

void CPU::SEI() {
    cycle();
    p[2] = true;
}
NES_CPU_BIN_OP_MODE(0x78, &CPU::SEI, AddrMode::Implied);

void CPU::CLD() {
    cycle();
    p[3] = false;
}
NES_CPU_BIN_OP_MODE(0xd8, &CPU::CLD, AddrMode::Implied);

void CPU::SED() {
    cycle();
    p[3] = true;
}
NES_CPU_BIN_OP_MODE(0xf8, &CPU::SED, AddrMode::Implied);

void CPU::CLV() {
    cycle();
    p[6] = false;
}
NES_CPU_BIN_OP_MODE(0xb8, &CPU::CLV, AddrMode::Implied);

void CPU::NOP() {
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
    a &= mem_read(addr);
    p[0] = a & 0x01;
    a >>= 1;
    set_zn(a);
}
NES_CPU_BIN_OP_MODE(0x4b, &CPU::ALR, AddrMode::Immediate);

void CPU::ANC(uint16_t addr) {
    a &= mem_read(addr);
    set_zn(a);
    p[0] = p[7];
}
NES_CPU_BIN_OP_MODE(0x0b, &CPU::ANC, AddrMode::Immediate);

void CPU::ARR(uint16_t addr) {
    a &= mem_read(addr);
    uint8_t t = a >> 1;
    t |= (p[0] << 7);
    p[0] = a & 0x01;
    a = t;
    set_zn(a);
    p[0] = a & 0x20;
    p[6] = (a & 0x20) ^ (a & 0x10);
}
NES_CPU_BIN_OP_MODE(0x6b, &CPU::ARR, AddrMode::Immediate);

void CPU::AXS(uint16_t addr) {
    uint8_t v = mem_read(addr);
    p[0] = (a & x) >= v;
    x = (a & x) - v;
    set_zn(x);
}
NES_CPU_BIN_OP_MODE(0xcb, &CPU::AXS, AddrMode::Immediate);

void CPU::LAX(uint16_t addr) {
    a = mem_read(addr);
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
    mem_write(addr, a & x);
}
NES_CPU_BIN_OP_MODE(0x87, &CPU::SAX, AddrMode::ZeroPage);
NES_CPU_BIN_OP_MODE(0x97, &CPU::SAX, AddrMode::ZeroPageY);
NES_CPU_BIN_OP_MODE(0x8f, &CPU::SAX, AddrMode::Absolute);
NES_CPU_BIN_OP_MODE(0x83, &CPU::SAX, AddrMode::IndirectX);

void CPU::DCP(uint16_t addr) {
    uint8_t t = mem_read(addr);
    mem_write(addr, t--);
    mem_write(addr, t);
    p[0] = a >= t;
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
    uint8_t t = mem_read(addr);
    mem_write(addr, t++);
    mem_write(addr, t);
    uint16_t r = a + ~t + p[0];
    p[0] = r <= 0xff; // carry
    p[6] = (r ^ a) & (r ^ ~t) & 0x80; // overflow
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
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    uint8_t t = v << 1;
    t |= p[0];
    p[0] = v & 0x80;
    set_zn(t);
    mem_write(addr, t);
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
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    uint8_t t = v >> 1;
    t |= (p[0] << 7);
    p[0] = v & 0x01;
    set_zn(t);
    mem_write(addr, t);
    uint16_t r = a + t + p[0];
    p[0] = r > 0xff; // carry;
    p[6] = (r ^ a) & (r ^ t) & 0x80; // overflow
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
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    p[0] = v & 0x80;
    v <<= 1;
    set_zn(v);
    mem_write(addr, v);
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
    uint8_t v = mem_read(addr);
    mem_write(addr, v);
    p[0] = v & 0x01;
    v >>= 1;
    set_zn(v);
    mem_write(addr, v);
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

    uint8_t inst = inst_read();
    (this->*(jump_table[inst]))();
}

} // namespace nes_em