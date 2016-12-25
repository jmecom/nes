#pragma once

/* Print error message and quit */
#define ERROR(error_message) \
    printf("%s\n", error_message); \
    printf("Exited in function \"%s\" on line %d in %s\n", __func__, __LINE__, __FILE__); \
    exit(1);
    
/* Combine two 8-bit values to one **little-endian** 16-bit value. */
#define COMBINE(a, b) (uint16_t) ((b << 8) | a)

/* Status register
   7   6   5   4   3   2   1   0
   S   V       B   D   I   Z   C
*/

#define CHECK_ZERO(src) (src == 0)
#define CHECK_SIGN(src) !!(src << 7)

#define SET_BIT(x,n) ((x) |= (1 << (n)))
#define CLR_BIT(x,n) ((x) &= ~((1) << (n)))

#define SET_ZERO(src) CHECK_ZERO(src) ? SET_BIT(P, 1) : CLR_BIT(P, 1)
#define SET_SIGN(src) CHECK_SIGN(src) ? SET_BIT(P, 7) : CLR_BIT(P, 7)