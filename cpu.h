#pragma once
#include <stdint.h>

uint16_t PC;
uint8_t SP;
uint8_t P;
uint8_t A;
uint8_t X;
uint8_t Y;

uint32_t CYC;
uint32_t SL;

uint8_t read(uint16_t idx);
void write(uint16_t idx, uint8_t val);

void stack_push(uint8_t val);
uint8_t stack_pop();

void execute(uint8_t opcode);
void log_state(uint8_t opcode, uint8_t num_args, ...);

void run();
int init();
