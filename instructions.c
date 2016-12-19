#include <stdint.h>
#include <stdio.h>
#include "instructions.h"

void jmp(uint8_t mode, uint8_t arg1, uint8_t arg2) {
	printf("Wow, it worked! %d %d\n", mode, arg1);
}