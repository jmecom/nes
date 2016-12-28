#pragma once

#include <stdint.h>
#include <stdarg.h>

void begin_logging();
void end_logging();
void log_cpu_state(uint8_t opcode, uint8_t num_args, ...);