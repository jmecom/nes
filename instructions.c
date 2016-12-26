#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "constants.h"
#include "instructions.h"

/* CPU state defined in cpu.c */
extern uint8_t X;
extern uint8_t P;
extern uint16_t PC;
extern uint32_t CYC;

/* Functions defined in cpu.c */
extern uint8_t fetch(uint16_t idx);

void jmp(uint8_t mode, uint8_t arg1, uint8_t arg2) {
    switch (mode) {
        case ABSOLUTE:
            PC = COMBINE(arg1, arg2);
            break;
        case INDIRECT:
            PC = fetch(COMBINE(arg1, arg2));
            break;
        default:
            ERROR("Invalid mode");
    }
}

void ldx(uint8_t mode, uint8_t arg1, uint8_t arg2) {
	SET_SIGN(arg1); // todo, how should these work in this case? (WRT arg1, arg2...)
	SET_ZERO(arg1);
	PC += 2;

	switch (mode) {
		case IMMEDIATE:
			X = arg1;
			break;
		case ZERO_PAGE_ABSOLUTE:
			X = fetch(COMBINE(0, arg1));
			break;
		case ABSOLUTE:
			X = fetch(COMBINE(arg1, arg2));
			break;
        default:
			ERROR("Invalid mode");
	}
}
