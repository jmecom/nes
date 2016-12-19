#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "memory.h"
#include "instructions.h"
#include "gamepak.h"
#include "error.h"

/* Registers */
uint16_t PC = 0xC000; // Program counter
uint8_t S = 0xFD;     // Stack pointer
uint8_t P = 0x24;     // Processor status
uint8_t A = 0;        // Accumulator
uint8_t X = 0;        // Index register X
uint8_t Y = 0;        // Index register Y

/* Memory */
gamepak_t gamepak = {0};

static uint8_t fetch(uint16_t idx) {
	if (idx > PRG_ROM_UPPER_LIMIT || idx < 0) {
		/* Out of valid memory range */
		ERROR("Out of valid memory range", 1);
	} else if (idx < PRG_ROM_UPPER_LIMIT && idx > PRG_ROM_LOWER_LIMIT) {
		/* PC in PRG-ROM */

		// gamepak.prg_rom starts at 0 in code, but is indexed starting at 
		// 0x8000 in the memory map; so offset it.
		return gamepak.prg_rom[idx - 0x8000];
	} 

	ERROR("Couldn't fetch from memory", 2);
}

static void execute(uint8_t opcode, uint8_t arg1, uint8_t arg2) {
	if (opcode == 0x4C) {
		jmp(1,1,1);
	} else if (opcode == 0x6C) {
		jmp(2,2,2);
	} else {
		ERROR("Invalid opcode", 1);
	}
}

void run() {
	uint8_t opcode = fetch(PC);
	uint8_t arg1 = fetch(PC + 1);
	uint8_t arg2 = fetch(PC + 2);

	execute(opcode, arg1, arg2);
}

int init() {
	int res = load("test_files/nestest.nes", &gamepak);

	printf("ret: %d\n\n", res);

	run(gamepak);

	return 0;
}
