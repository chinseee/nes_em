#pragma once
#include "nes_em/fwd.h"
#include "nes_em/nes_file.h"
#include <bitset>
#include <cstdint>
#include <utility>


namespace nes_em {

enum class AddrMode {
    Implied,
    Accumulator,
    Immediate,
    ZeroPage,
    ZeroPageX,
    ZeroPageY,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    Relative,
    Indirect,
    IndirectX,
    IndirectY,
};

class CPU {
public:
    // p (status register) flag bit indices
    static constexpr size_t CARRY_BIT = 0;
    static constexpr size_t ZERO_BIT = 1;
    static constexpr size_t INTERRUPT_DISABLE_BIT = 2;
    static constexpr size_t DECIMAL_BIT = 3;
    static constexpr size_t BREAK_BIT = 4;
    static constexpr size_t UNUSED_BIT = 5;
    static constexpr size_t OVERFLOW_BIT = 6;
    static constexpr size_t NEGATIVE_BIT = 7;

    // connections
    PPU* ppu;
    Bus* bus;


    // registers
    uint16_t pc;
    uint8_t a, x, y, sp;
    std::bitset<8> p;

    // temporary: simulate cpu cycles
    uint64_t cycles = 0;

    // interrupts
    bool nmi_line, nmi_line_prev; // sampled /NMI input pin, current and previous cycle
    bool nmi_pending;             // latched on nmi_line's falling edge, cleared once serviced
    bool irq_line;
    bool irq_pending;

    CPU();
    void reset();

    void exec_inst();

    void cycle();
    void poll_interrupts();

    uint8_t inst_read();
    uint8_t cycle_read(uint16_t);
    void cycle_write(uint16_t, uint8_t);
    void push(uint8_t);
    uint8_t pull();

    void set_zn(uint8_t);

    void branch(bool, uint8_t);

    // access
    void LDA(uint16_t);
    void STA(uint16_t);
    void LDX(uint16_t);
    void STX(uint16_t);
    void LDY(uint16_t);
    void STY(uint16_t);

    // transfer
    void TAX();
    void TXA();
    void TAY();
    void TYA();

    // arithmetic
    void ADC(uint16_t);
    void SBC(uint16_t);
    void INC(uint16_t);
    void DEC(uint16_t);    
    void INX();
    void DEX();
    void INY();
    void DEY();

    // shift
    void ASL();
    void ASL(uint16_t);
    void LSR();
    void LSR(uint16_t);
    void ROL();
    void ROL(uint16_t);
    void ROR();
    void ROR(uint16_t);

    // bitwise
    void AND(uint16_t);
    void ORA(uint16_t);
    void EOR(uint16_t);
    void BIT(uint16_t);

    // compare
    void CMP(uint16_t);
    void CPX(uint16_t);
    void CPY(uint16_t);

    // branch
    void BCC(uint16_t);
    void BCS(uint16_t);
    void BEQ(uint16_t);
    void BNE(uint16_t);
    void BPL(uint16_t);
    void BMI(uint16_t);
    void BVC(uint16_t);
    void BVS(uint16_t);

    // jump
    void JMP(uint16_t);
    void JSR(uint16_t);
    void RTS();
    void BRK();
    void RTI();

    // stack
    void PHA();
    void PLA();
    void PHP();
    void PLP();
    void TXS();
    void TSX();

    // flags
    void CLC();
    void SEC();
    void CLI();
    void SEI();
    void CLD();
    void SED();
    void CLV();

    // nop
    void NOP();
    void NOP(uint16_t);

    // unofficial - combined
    void ALR(uint16_t);
    void ANC(uint16_t);
    void ARR(uint16_t);
    void AXS(uint16_t);
    void LAX(uint16_t);
    void SAX(uint16_t);

    // unofficial - rmw
    void DCP(uint16_t);
    void ISC(uint16_t);
    void RLA(uint16_t);
    void RRA(uint16_t);
    void SLO(uint16_t);
    void SRE(uint16_t);

    // TODO: implement last few unofficial opcodes (cringe)
    // refactoring

    // interrupts
    void interrupt();

private:
    template <void (CPU::*op)(uint16_t), AddrMode mode, bool rmw>
    void opcode_impl();
    
    template <void (CPU::*op)(uint16_t), AddrMode mode>
    void opcode_impl();

    template <void (CPU::*op)(), AddrMode mode>
    void opcode_impl();

    template <uint8_t bin>
    void opcode_impl();

    template<size_t... Is>
    static constexpr auto make_jump_table(std::index_sequence<Is...>);
};


}