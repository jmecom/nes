#pragma once

/* Print error message and quit */
#define ERROR(error_message) \
    printf("%s\n", error_message); \
    printf("Exited in function %s on line %d in %s\n", __func__, __LINE__, __FILE__); \
    exit(1);
    
/* Combine two 8-bit values to one **little-endian** 16-bit value. */
#define COMBINE(a, b) (uint16_t) ((b << 8) | a)