#pragma once
#include <stdint.h>

uint8_t fetch(uint16_t idx);
void execute(uint8_t opcode, uint8_t arg1, uint8_t arg2);
void log_state(uint8_t opcode, uint8_t arg1, uint8_t arg2);
int run();
int init();
