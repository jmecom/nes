#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "instructions.h"
#include "gamepak.h"
#include "macros.h"
#include "constants.h"

/* Temporary constants for testing */
const char* NESTEST_PATH = "test_files/nestest.nes";
const char* LOG_PATH = "out.log";

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

void execute(uint8_t opcode, uint8_t arg1, uint8_t arg2) {
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
            ldx(ZERO_PAGE_ABSOLUTE, arg1, arg2);
            break;
        case 0xA6:
            break;
        case 0xB6:
            break;
        case 0xAE:
            break;
        case 0xBE:
            break;
        default:
            ERROR("Invalid opcode");
    }
}

void log_state(uint8_t opcode, uint8_t arg1, uint8_t arg2) {
    FILE *fp = fopen(LOG_PATH, "w+");

    // todo: pretty print the opcodes and arguments (like nestest.log)
    //       - would be easiest with a LUT of function pointers (function names in all-caps)
    //       - if i do this, how should I cleanly pass the mode to the function?
    char* str = "%02X  %02X %02X %02X       %02X                   \
    A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:  %d SL:%d\n";

    fprintf(fp, str, PC, opcode, arg1, arg2, COMBINE(arg1, arg2), A, X, Y, P, SP, CYC, SL);
    printf(str, PC, opcode, arg1, arg2, COMBINE(arg1, arg2), A, X, Y, P, SP, CYC, SL);
    
    fclose(fp);
}

void run() {
    uint8_t opcode = fetch(PC);
    uint8_t arg1 = fetch(PC + 1);
    uint8_t arg2 = fetch(PC + 2);

    log_state(opcode, arg1, arg2);
    execute(opcode, arg1, arg2);
}

int init() {
    int res = 0;

    // todo: use ERROR for error handling in gamepak handler code?
    if ((res = load(NESTEST_PATH, &gamepak)) != 0) {
        ERROR("Failed to load file");
    }

    run();

     return res;
}

