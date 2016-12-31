#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "cpu.h"
#include "logging.h"
#include "gamepak.h"
#include "macros.h"
#include "constants.h"

/* Registers */
uint16_t PC = 0xC000; // Program counter
uint8_t SP = 0xFD;    // Stack pointer
uint8_t P = 0x24;     // Processor status
uint8_t A = 0;        // Accumulator
uint8_t X = 0;        // Index register X
uint8_t Y = 0;        // Index register Y

/* State */
uint32_t CYC = 0;     // Cycle counter
uint32_t SL = 241;    // Scanline

/* Memory */
gamepak_t gamepak;
uint8_t sram[SRAM_SIZE];
uint8_t ram[RAM_SIZE];

uint8_t read(uint16_t idx) {
    if (idx >= PRG_ROM_LOWER_LIMIT) { // PRG-ROM
        // gamepak.prg_rom starts at 0 in code, but is indexed starting at 
        // 0x8000 in the memory map; so offset it.
        return gamepak.prg_rom[idx - PRG_ROM_LOWER_LIMIT];
    } else if (idx <= RAM_MIRROR_UPPER_LIMIT) { // RAM
        return ram[idx % RAM_SIZE]; // modulus handles mirroring
    } else if (idx <= SRAM_UPPER_LIMIT && idx >= SRAM_LOWER_LIMIT) { // SRAM
        return sram[idx];
    } else {
        ERROR("Couldn't fetch from memory");
    }
}

void write(uint16_t idx, uint8_t val) {
    if (idx <= RAM_MIRROR_UPPER_LIMIT) { // RAM
        ram[idx % RAM_SIZE] = val;
    } else if (idx <= SRAM_UPPER_LIMIT && idx >= SRAM_LOWER_LIMIT) { // SRAM
        sram[idx] = val;
    } else {
        ERROR("Couldn't write to memory");
    }
}

void stack_push(uint8_t val) {
    write(SP + 256, val);
    SP--;
}

uint8_t stack_pop() {
    SP++;
    return read(SP + 256);
}

void execute(uint8_t opcode) {
    uint8_t num_args = instr_bytes[opcode] - 1;
    uint8_t mode = instr_modes[opcode];
    uint8_t arg1 = read(PC + 1), arg2 = read(PC + 2);

    uint16_t src16 = 0;
    uint8_t src8 = 0, tmp = 0, pc_lo = 0, pc_hi = 0;

    switch (num_args) {
        case 0:
            log_cpu_state(opcode, num_args);
            break;
        case 1:
            log_cpu_state(opcode, num_args, arg1);
            break;
        case 2:
            log_cpu_state(opcode, num_args, arg1, arg2);
            break;
        default:
            ERROR("Invalid number of arguments for instruction");
    }

    PC += instr_bytes[opcode];

    switch (mode) {
        case ABSOLUTE:
            src16 = COMBINE(arg1, arg2);
            break;
        case ABSOLUTE_X:
            break;
        case ABSOLUTE_Y:
            break;
        case ACCUMULATOR:
            src8 = A;
            break;
        case IMMEDIATE:
            src8 = arg1;
            break;
        case IMPLIED:
            src8 = arg1;
            break;
        case INDEXED_INDIRECT:
            src16 = COMBINE(read(COMBINE((uint8_t)(arg1+X), 0)), read(COMBINE((uint8_t)(arg1+X+1), 0)));
            break;
        case INDIRECT:
            src16 = read(COMBINE(arg1, arg2));
            break;
        case INDIRECT_INDEXED:
            break;
        case RELATIVE:
            src16 = PC + arg1;
            break;
        case ZERO_PAGE:
            src16 = COMBINE(arg1, 0);
            break;
        case ZERO_PAGE_X:
            break;
        case ZERO_PAGE_Y:
            break;
        default:
            ERROR("Invalid mode");
    }

    switch (opcode) {
        // JMP
        case 0x4C:
        case 0x6C:
            PC = src16;
            break;
        // LDX
        case 0xA2:
            X = src8;
            SET_SIGN_ZERO(X);
            break;
        case 0xAE:
            X = read(src16);
            SET_SIGN_ZERO(X);
            break;
        // STX
        case 0x86:
        case 0x8E:
            write(src16, X);
            break;
        // JSR 
        case 0x20:
            PC--;  // JSR pushes address - 1 onto the stack
            stack_push(UPPER(PC));
            stack_push(LOWER(PC));
            PC = src16;
        // NOP 
        case 0xEA:
            break;
        // SEC
        case 0x38:
            SET_BIT(P, 0);
            break;
        // BCS
        case 0xB0:
            if (CARRY_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // BCC
        case 0x90:
            if (!CARRY_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // CLC
        case 0x18:
            CLR_BIT(P, 0);
            break;
        // LDA
        case 0xA9: // immediate 
            A = src8;
			SET_SIGN_ZERO(A);
            break;
        case 0xAD: // absolute 
        case 0xA5: // zero-page absolute
        case 0xA1: // indexed indirect
            A = read(src16);
            SET_SIGN_ZERO(A);
            break;
        // BEQ
        case 0xF0:
            if (ZERO_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // BNE
        case 0xD0:
            if (!ZERO_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // STA
        case 0x85:
        case 0x8D:
        case 0x81:
            write(src16, A);
            break;
        // BIT
        case 0x24: 
            tmp = read(src16);
            SET_SIGN(tmp);
            SET_ZERO((tmp & A));
            SET_OVERFLOW(tmp & 0x40); // copy bit 6
            break;
        // BVS
        case 0x70:
            if (OVERFLOW_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // BVC
        case 0x50:
            if (!OVERFLOW_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16;
            }
            break;
        // BPL
        case 0x10:
            if (!SIGN_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // RTS
        case 0x60:
            pc_lo = stack_pop();
            pc_hi = stack_pop();
            PC = COMBINE(pc_lo, pc_hi) + 1;
            break;
        // SEI 
        case 0x78:
            SET_INTERRUPT(1);
            break;
        // SED
        case 0xF8:
            SET_DECIMAL(1);
            break;
        // PHP
        case 0x08:
            // PHP and BRK push the flags with bit 4 true. 
            // IRQ and NMI push bit 4 false.
            SET_B(1);
            stack_push(P);
            SET_B(0);
            break;
        // PLA
        case 0x68:
            A = stack_pop();
            SET_SIGN_ZERO(A);
            break;
        // AND
        case 0x29:
            A &= src8;
            SET_SIGN_ZERO(A);
            break;
        case 0x21:
            A &= read(src16);
            SET_SIGN_ZERO(A);
            break;
        // CMP
        case 0xC9:
            tmp = A - src8;
            SET_SIGN_ZERO(tmp);
            SET_CARRY(A >= arg1);
            break;
        // CLD 
        case 0xD8:
            SET_DECIMAL(0);
            break;
        // PHA
        case 0x48:
            stack_push(A);
            break;
        // PLP
        case 0x28:
            // PLP and RTI pull P from stack, but ignore bits 4 and 5
            tmp = stack_pop();
            CPY_BIT(P, tmp, 4);
            CPY_BIT(P, tmp, 5);
            P = tmp;
            break;
        // BMI
        case 0x30:
            if (SIGN_SET()) {
                BRANCH_CYCLE_INCREMENT(src16);
                PC = src16; 
            }
            break;
        // ORA
        case 0x09:
            A |= src8;
            SET_SIGN_ZERO(A);
            break;
        case 0x01:
            A |= read(src16);
            SET_SIGN_ZERO(A);
            break;
        // CLV
        case 0xB8:
            SET_OVERFLOW(0);
            break;
        // EOR
        case 0x49:
            A ^= src8;
            SET_SIGN_ZERO(A);
            break;
        case 0x41:
            A ^= read(src16);
            SET_SIGN_ZERO(A);
            break;
        // ADC
        case 0x69: { // immediate
            uint16_t sum = A + src8 + CARRY_SET();
            SET_OVERFLOW(!((A ^ src8) & 128) && ((A ^ sum) & 128));
            SET_CARRY(sum >= 256);
            A = (uint8_t) sum;
            SET_SIGN_ZERO(A);
            break;
        }
        // SBC
        case 0xE9: { // immediate 
            uint16_t diff = A - src8 - !CARRY_SET();
            SET_OVERFLOW(((A ^ src8) & 128) && ((A ^ diff) & 128));
            SET_CARRY(diff < 256);
            A = (uint8_t) diff;
            SET_SIGN_ZERO(A);
            break;
        }
        // LDY
        case 0xA0:
            Y = src8;
			SET_SIGN_ZERO(Y);
            break;
        // CPY
        case 0xC0:
            tmp = Y - src8;
            SET_SIGN_ZERO(tmp);
            SET_CARRY(Y >= src8);     
            break;
        // CPX
        case 0xE0:
            tmp = X - src8;
            SET_SIGN_ZERO(tmp);
            SET_CARRY(X >= src8);
            break;
        // INY 
        case 0xC8:
            Y++;
            SET_SIGN_ZERO(Y);
            break;
        // INX 
        case 0xE8:
            X++;
            SET_SIGN_ZERO(X);
            break;
        // DEY
        case 0x88:
            Y--;
            SET_SIGN_ZERO(Y);
            break;
        // DEX 
        case 0xCA:
            X--;
            SET_SIGN_ZERO(X);
            break;
        // TAY
        case 0xA8:
            Y = A;
            SET_SIGN_ZERO(Y);
            break;
        // TAX
        case 0xAA:
            X = A;
            SET_SIGN_ZERO(X);
            break;
        // TYA
        case 0x98:
            A = Y;
            SET_SIGN_ZERO(A);
            break;
        // TXA
        case 0x8A:
            A = X;
            SET_SIGN_ZERO(A);
            break;
        // TSX
        case 0xBA:
            X = SP;
            SET_SIGN_ZERO(X);
            break;
        // TXS
        case 0x9A:
            SP = X;
            break;
        // RTI
        case 0x40:
            // PLP and RTI pull P from stack, but ignore bits 4 and 5
            tmp = stack_pop();
            CPY_BIT(P, tmp, 4);
            CPY_BIT(P, tmp, 5);
            P = tmp;

            pc_lo = stack_pop();
            pc_hi = stack_pop();
            PC = COMBINE(pc_lo, pc_hi);
            break;
        // LSR
        case 0x4A:
            SET_CARRY(CHK_BIT(src8, 0)); // todo, think this is right but may need to double check
            src8 >>= 1;
            SET_SIGN_ZERO(src8);
            if (mode == ACCUMULATOR) A = src8;
            break;
        // ASL
        case 0x0A:
            SET_CARRY(CHK_BIT(src8, 7));
            src8 <<= 1;
            SET_SIGN_ZERO(src8);
            if (mode == ACCUMULATOR) A = src8;
            break;
        // ROR
        case 0x6A:
            tmp = src8;
            src8 >>= 1;

            // Bit 7 = current carry flag 
            if (CARRY_SET()) SET_BIT(src8, 7); else CLR_BIT(src8, 7);
            SET_SIGN_ZERO(src8);

            // Carry flag = old bit 0
            SET_CARRY(CHK_BIT(tmp, 0));

            if (mode == ACCUMULATOR) A = src8;
            break;
        // ROL
        case 0x2A:
            tmp = src8;
            src8 <<= 1;

            // Bit 0 = current carry flag 
            if (CARRY_SET()) SET_BIT(src8, 0); else CLR_BIT(src8, 0);
            SET_SIGN_ZERO(src8);

            // Carry flag = old bit 7
            SET_CARRY(CHK_BIT(tmp, 7));

            if (mode == ACCUMULATOR) A = src8;
            break;
        default:
            ERROR("Invalid opcode");
    }

    CYC += instr_ppu_cycles[opcode];
    if (CYC >= SL_RESET) { // todo: >= or > ?
        CYC = CYC % SL_RESET;
        SL++;
        if (SL > 260) SL = -1; 
    }
}

int run() {
    int res = 0;

    // todo: use ERROR for error handling in gamepak handler code?
    if ((res = load("test_files/nestest.nes", &gamepak)) != 0) {
        ERROR("Failed to load gamepak");
    }

    memset(ram, 0, sizeof(ram));
    memset(sram, 0, sizeof(sram));

    begin_logging();

    while (true) {
        execute(read(PC));
    }

    end_logging();

    free(gamepak.trainer);
    free(gamepak.prg_rom);
    free(gamepak.chr_rom);

    return res;
}
