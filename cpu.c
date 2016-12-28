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
uint32_t SL = 241;    // Scanline -- todo

/* Memory */
gamepak_t gamepak;
uint8_t ram[RAM_SIZE];

uint8_t read(uint16_t idx) {
    if (idx > PRG_ROM_LOWER_LIMIT) {
        /* idx in PRG-ROM */

        // gamepak.prg_rom starts at 0 in code, but is indexed starting at 
        // 0x8000 in the memory map; so offset it.
        return gamepak.prg_rom[idx - PRG_ROM_LOWER_LIMIT];
    } else if (idx < RAM_MIRROR_UPPER_LIMIT) {
        /* idx in RAM or RAM mirror */
        return ram[idx % RAM_SIZE]; // modulus handles mirroring
    } else {
        ERROR("Couldn't fetch from memory");
    }
}

void write(uint16_t idx, uint8_t val) {
    if (idx < RAM_MIRROR_UPPER_LIMIT) {
        ram[idx % RAM_SIZE] = val;
    } else {
        ERROR("Couldn't write to memory");
    }
}

void stack_push(uint8_t val) {
    write(SP, val);
    SP--;
}

uint8_t stack_pop() {
    uint8_t val = read(SP);
    SP++;
    return val;
}

void execute(uint8_t opcode) {
    uint8_t num_args = instr_bytes[opcode] - 1;
    uint8_t arg1 = read(PC + 1), arg2 = read(PC + 2);

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
    
    switch (opcode) {
        // JMP
        case 0x4C: // absolute
            PC = COMBINE(arg1, arg2);
            break;
        case 0x6C: // indirect
            PC = read(COMBINE(arg1, arg2));
            break;
        // LDX
        case 0xA2: // immediate 
            PC += instr_bytes[opcode];
            X = arg1;
			SET_SIGN(arg1); // todo: possibly handling this wrong in some cases?
			SET_ZERO(arg1);
            break;
        case 0xA6: // zero-page absolute
            ERROR("Not yet implemented");
            break;
        case 0xB6: // zero-page indexed
            ERROR("Not yet implemented");
            break;
        case 0xAE:
            ERROR("Not yet implemented");
            break;
        case 0xBE:
            // todo: https://wiki.nesdev.com/w/index.php/CPU_addressing_modes
            // I think this is `absolute indexed`
            ERROR("Not yet implemented");
            break;
        // STX
        case 0x86: // zero-page absolute
            PC += instr_bytes[opcode];
            write(COMBINE(0, arg1), X);
            break;
        case 0x96:
            ERROR("Not yet implemented");
            break;
        case 0x8E:
            ERROR("Not yet implemented");
            break;
        // JSR 
        case 0x20:
            PC--;  // JSR pushes address - 1 onto the stack
            stack_push(UPPER(PC));
            stack_push(LOWER(PC));
            PC = COMBINE(arg1, arg2);
            break;
        // NOP 
        case 0xEA:
            PC += instr_bytes[opcode];
            break;
        // SEC
        case 0x38:
            PC += instr_bytes[opcode];
            SET_BIT(P, 0);
            break;
        // BCS
        case 0xB0:
            PC += instr_bytes[opcode];
            if (CARRY_SET()) {
                uint16_t rel = PC + arg1;
                CYC += UPPER(PC) == UPPER(rel) ? 1 : 2;
                PC = rel; 
            }
            break;
        // BCC
        case 0x90:
            PC += instr_bytes[opcode];
            if (!CARRY_SET()) {
                uint16_t rel = PC + arg1;
                CYC += UPPER(PC) == UPPER(rel) ? 1 : 2;
                PC = rel; 
            }
            break;
        // CLC
        case 0x18:
            PC += instr_bytes[opcode];
            CLR_BIT(P, 0);
            break;
        // LDA
        case 0xA9: // immediate 
            PC += instr_bytes[opcode];
            A = arg1;
			SET_SIGN(arg1);
			SET_ZERO(arg1);
            break;
        // BEQ
        case 0xF0:
            PC += instr_bytes[opcode];
            if (ZERO_SET()) {
                uint16_t rel = PC + arg1;
                CYC += UPPER(PC) == UPPER(rel) ? 1 : 2;
                PC = rel; 
            }
            break;
        // BNE
        case 0xD0:
            PC += instr_bytes[opcode];
            if (!ZERO_SET()) {
                uint16_t rel = PC + arg1;
                CYC += UPPER(PC) == UPPER(rel) ? 1 : 2;
                PC = rel; 
            }
            break;
        // STA
        case 0x85: // zero-page absolute
            PC += instr_bytes[opcode];
            A = read(COMBINE(0, arg1));
            SET_SIGN(A);
            SET_ZERO(A);
            break;
        // BIT
        case 0x24: // zero-page absolute 
            PC += instr_bytes[opcode];
            uint8_t m = read(COMBINE(0, arg1));
            SET_SIGN(m);
            SET_ZERO((m & A));
            SET_OVERFLOW(m); // copy 6th bit
            break;
        default:
            ERROR("Invalid opcode");
    }

    CYC += instr_cycles[opcode];
}

void run() {
    while (true) {
        execute(read(PC));
    }
}

int init() {
    int res = 0;

    // todo: use ERROR for error handling in gamepak handler code?
    if ((res = load("test_files/nestest.nes", &gamepak)) != 0) {
        ERROR("Failed to load gamepak");
    }

    memset(ram, 0, sizeof(ram));

    begin_logging();
    run();
    end_logging();

    free(gamepak.trainer);
    free(gamepak.prg_rom);
    free(gamepak.chr_rom);

    return res;
}
