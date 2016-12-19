#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "constants.h"
#include "instructions.h"

/* CPU state defined in cpu.c */
extern uint16_t PC;
extern uint32_t CYC;

/* Functions defined in cpu.c */
extern uint8_t fetch(uint16_t idx);

void jmp(uint8_t mode, uint8_t arg1, uint8_t arg2) {
	if (mode == ABSOLUTE) {
		PC = COMBINE(arg1, arg2);
		CYC += 3;
	} else if (mode == INDIRECT) {
		PC = fetch(COMBINE(arg1, arg2));
		CYC += 9;
	} else {
		ERROR("Invalid mode");
	}
}