#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "constants.h"
#include "instructions.h"

/* CPU state defined in cpu.c */
extern uint8_t X;
extern uint8_t Y;
extern uint8_t P;
extern uint16_t PC;
extern uint32_t CYC;

/* Functions defined in cpu.c */
extern uint8_t read(uint16_t idx);
void write(uint16_t idx, uint8_t val);

void jmp(uint8_t mode, uint8_t arg1, uint8_t arg2) {
    switch (mode) {
        case ABSOLUTE:
            PC = COMBINE(arg1, arg2);
            break;
        case INDIRECT:
            PC = read(COMBINE(arg1, arg2));
            break;
        default:
            ERROR("Invalid mode");
    }
}

void ldx(uint8_t mode, uint8_t arg1, uint8_t arg2) {
	PC += 2;

	arg2 = 0; // temporary

	switch (mode) {
		case IMMEDIATE:
			X = arg1;
			SET_SIGN(arg1); // todo: possibly handling this wrong in some cases?
			SET_ZERO(arg1); //       i dont think so though
			break;
		// case ZERO_PAGE_ABSOLUTE:
		// 	uint8_t m = fetch(COMBINE(0, arg1));
		// 	X = m;
		// 	SET_SIGN(m);
		// 	SET_ZERO(m);
		// 	break;
		// case ABSOLUTE:
		// 	uint8_t m = fetch(COMBINE(arg1, arg2));
		// 	X = m;
		// 	SET_SIGN(m);
		// 	SET_ZERO(m);
		// 	break;
        default:
			ERROR("Invalid mode");
	}
}

void stx(uint8_t mode, uint8_t arg1, uint8_t arg2) {
	PC += 2;

	arg2 = 0;  // temp
	switch (mode) {
		case ZERO_PAGE_ABSOLUTE:
			write(COMBINE(0, arg1), X);
			break;
		default:
			ERROR("Invalid mode")
	}
}
