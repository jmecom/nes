#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "gamepak.h"

/* Registers */
uint16_t PC = 0xC000; // Program counter
uint8_t S = 0xFD;     // Stack pointer
uint8_t P = 0x24;     // Processor status
uint8_t A = 0;        // Accumulator
uint8_t X = 0;        // Index register X
uint8_t Y = 0;        // Index register Y

/* Memory */
gamepak_t gamepak = {0};

static int fetch(int *val) {
	if (PC > PRG_ROM_UPPER_LIMIT || PC < 0) {
		/* Out of valid memory range */
		return 1;
	} else if (PC < PRG_ROM_UPPER_LIMIT && PC > PRG_ROM_LOWER_LIMIT) {
		/* PC in PRG-ROM */

		// gamepak.prg_rom starts at 0 in code, but is indexed starting at 
		// 0x8000 in the memory map; so offset it.
		*val = gamepak.prg_rom[PC - 0x8000];
	}

	return 0;
}

static int execute() {
	return 0;
}

int run() {
	// while (true) {

	// }	
	return 0;
}

int init() {
	int res = load("test_files/nestest.nes", &gamepak);

	printf("ret: %d\n\n", res);

	run(gamepak);
	return 0;
}
