#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "instructions.h"
#include "memory.h"
#include "gamepak.h"
#include "macros.h"
#include "constants.h"

/* Temporary constants for testing */
char* nestest_path = "test_files/nestest.nes";
char* my_log_path = "./out.log";

/* Registers */
uint16_t PC = 0xC000; // Program counter
uint8_t S = 0xFD;     // Stack pointer
uint8_t P = 0x24;     // Processor status
uint8_t A = 0;        // Accumulator
uint8_t X = 0;        // Index register X
uint8_t Y = 0;        // Index register Y

/* Internal CPU state */
uint32_t CYC = 0;          // Cycle counter

/* Memory */
gamepak_t gamepak = {0};

uint8_t fetch(uint16_t idx) {
	if (idx > PRG_ROM_UPPER_LIMIT || idx < 0) {
		/* Out of valid memory range */
		ERROR("Out of valid memory range");
	} else if (idx < PRG_ROM_UPPER_LIMIT && idx > PRG_ROM_LOWER_LIMIT) {
		/* PC in PRG-ROM */

		// gamepak.prg_rom starts at 0 in code, but is indexed starting at 
		// 0x8000 in the memory map; so offset it.
		return gamepak.prg_rom[idx - 0x8000];
	} 

	ERROR("Couldn't fetch from memory");
}

void execute(uint8_t opcode, uint8_t arg1, uint8_t arg2) {
	if (opcode == 0x4C) {
		jmp(ABSOLUTE, arg1, arg2);
	} else if (opcode == 0x6C) {
		jmp(INDIRECT, arg1, arg2);
	} else {
		ERROR("Invalid opcode");
	}
}

void log_state(uint8_t opcode, uint8_t arg1, uint8_t arg2) {
	FILE *fp;

	fp = fopen(my_log_path, "w+");
	fprintf(fp, "%X  %X %X %X", PC, opcode, arg1, arg2);
	// todo: logging to out.log
	// todo: also would be nice if out.log printed upon program completion
	fclose(fp);
}

void run() {
	uint8_t opcode = fetch(PC);
	uint8_t arg1 = fetch(PC + 1);
	uint8_t arg2 = fetch(PC + 2);

	execute(opcode, arg1, arg2);
	log_state(opcode, arg1, arg2);
}

int init() {
	int res = 0;

	// todo: use ERROR for error handling in gamepak handler code
	if ((res = load(nestest_path, &gamepak)) != 0) {
		ERROR("Failed to load file");
	}

	run(gamepak);

	return 0;
}

