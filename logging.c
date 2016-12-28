#include <stdio.h>

#include "logging.h"
#include "cpu.h"
#include "constants.h"
#include "macros.h"

const char *NESTEST_PATH = "test_files/nestest.nes";
const char *LOG_PATH = "out.log";
FILE *log_fp;

void begin_logging() {
    log_fp = fopen(LOG_PATH, "w+");
}

void end_logging() {
    fclose(log_fp);
}

void log_cpu_state(uint8_t opcode, uint8_t num_args, ...) {
    va_list ap;
    va_start(ap, num_args);

    if (num_args == 0) {
        char* str = "%02X  %02X        %s                     \
        A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d SL:%d\n";

        // Multiply by 3 because nestest.log logs PPU cycles
        fprintf(log_fp, str, PC, opcode, \
        instr_names[opcode], A, X, Y, P, SP, CYC, SL);

        printf(str, PC, opcode, \
        instr_names[opcode], A, X, Y, P, SP, CYC, SL);
    }
    if (num_args == 1) {
        char* str = "%02X  %02X %02X     %s  %02X                 \
        A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d SL:%d\n";

        uint8_t arg1 = va_arg(ap, int); // implicit type conversion

        fprintf(log_fp, str, PC, opcode, arg1, \
        instr_names[opcode], arg1, A, X, Y, P, SP, CYC, SL);

        printf(str, PC, opcode, arg1, \
        instr_names[opcode], arg1, A, X, Y, P, SP, CYC, SL);
    } else if (num_args == 2) {
        char* str = "%02X  %02X %02X %02X  %s  %02X               \
        A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d SL:%d\n";

        uint8_t arg1 = va_arg(ap, int);
        uint8_t arg2 = va_arg(ap, int);

        fprintf(log_fp, str, PC, opcode, arg1, arg2, \
        instr_names[opcode], COMBINE(arg1, arg2), A, X, Y, P, SP, CYC, SL);

        printf(str, PC, opcode, arg1, arg2, \
        instr_names[opcode], COMBINE(arg1, arg2), A, X, Y, P, SP, CYC, SL);
    }

    va_end(ap);
}