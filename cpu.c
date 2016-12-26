#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "cpu.h"
#include "instructions.h"
#include "gamepak.h"
#include "macros.h"
#include "constants.h"

/* Temporary constants for testing */
const char *NESTEST_PATH = "test_files/nestest.nes";
const char *LOG_PATH = "out.log";
FILE *log_fp;

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

uint8_t fetch(uint16_t idx) {
    if (idx > PRG_ROM_LOWER_LIMIT) {
        /* PC in PRG-ROM */

        // gamepak.prg_rom starts at 0 in code, but is indexed starting at 
        // 0x8000 in the memory map; so offset it.
        return gamepak.prg_rom[idx - 0x8000];
    } 
    ERROR("Couldn't fetch from memory");
}

void execute(uint8_t opcode) {
    uint8_t num_args = instr_bytes[opcode] - 1;
    uint8_t arg1 = 0, arg2 = 0;

    arg1 = fetch(PC + 1);

    if (num_args == 1) {
        log_state(opcode, num_args, arg1);
    } else if (num_args == 2) {
        arg2 = fetch(PC + 2);
        log_state(opcode, num_args, arg1, arg2);
    } else {
        ERROR("Invalid number of arguments for instruction")
    }

    switch (opcode) {
        // JMP
        case 0x4C:
            jmp(ABSOLUTE, arg1, arg2);
            break;
        case 0x6C:
            jmp(IMMEDIATE, arg1, arg2);
            break;
        // LDX
        case 0xA2:
            ldx(IMMEDIATE, arg1, arg2);
            break;
        case 0xA6:
            ldx(ZERO_PAGE_ABSOLUTE, arg1, arg2);
            break;
        case 0xB6:
            ldx(ZERO_PAGE_INDEXED, arg1, arg2);
            break;
        case 0xAE:
            ldx(ABSOLUTE, arg1, arg2);
            break;
        case 0xBE:
            // todo: https://wiki.nesdev.com/w/index.php/CPU_addressing_modes
            // I think this is absolute indexed
            ldx(INDEXED, arg1, arg2);
            break;
        default:
            ERROR("Invalid opcode");
    }

    CYC += instr_cycles[opcode];
    // todo: handle page crossing
}

void log_state(uint8_t opcode, uint8_t num_args, ...) {
    va_list ap;
    va_start(ap, num_args);

    if (num_args == 1) {
        char* str = "%02X  %02X %02X     %s  %02X                 \
        A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d SL:%d\n";

        uint8_t arg1 = va_arg(ap, int); // implicit type conversion

        // Multiply by 3 because nestest.log logs PPU cycles
        fprintf(log_fp, str, PC, opcode, arg1, \
        instr_names[opcode], arg1, A, X, Y, P, SP, CYC*3, SL);

        printf(str, PC, opcode, arg1, \
        instr_names[opcode], arg1, A, X, Y, P, SP, CYC*3, SL);
    } else if (num_args == 2) {
        char* str = "%02X  %02X %02X %02X  %s  %02X               \
        A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d SL:%d\n";

        uint8_t arg1 = va_arg(ap, int);
        uint8_t arg2 = va_arg(ap, int);

        fprintf(log_fp, str, PC, opcode, arg1, arg2, \
        instr_names[opcode], COMBINE(arg1, arg2), A, X, Y, P, SP, CYC*3, SL);

        printf(str, PC, opcode, arg1, arg2, \
        instr_names[opcode], COMBINE(arg1, arg2), A, X, Y, P, SP, CYC*3, SL);
    }

    va_end(ap);
}

void run() {
    while (true) {
        execute(fetch(PC));
    }
}

int init() {
    int res = 0;

    // todo: use ERROR for error handling in gamepak handler code?
    if ((res = load(NESTEST_PATH, &gamepak)) != 0) {
        ERROR("Failed to load file");
    }

    log_fp = fopen(LOG_PATH, "w+");
    run();
    fclose(log_fp);

    free(gamepak.trainer);
    free(gamepak.prg_rom);
    free(gamepak.chr_rom);

    return res;
}

