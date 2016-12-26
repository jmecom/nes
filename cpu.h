#pragma once
#include <stdint.h>

uint8_t fetch(uint16_t idx);
void execute(uint8_t opcode);
void log_state(uint8_t opcode, uint8_t num_args, ...);
void run();
int init();
