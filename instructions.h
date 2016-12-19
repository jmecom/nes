#pragma once

typedef void(*instr_fn)(uint8_t mode, uint8_t arg1, uint8_t arg2);

void jmp(uint8_t mode, uint8_t arg1, uint8_t arg2);
