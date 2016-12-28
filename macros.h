#pragma once

/* Print error message and quit */
#define ERROR(error_message) \
    printf("%s\n", error_message); \
    printf("Exited in function \"%s\" on line %d in %s\n", __func__, __LINE__, __FILE__); \
    exit(1)
    
/* Combine two 8-bit values to one **little-endian** 16-bit value. */
#define COMBINE(a, b) ((uint16_t) ((b << 8) | a))

/* Split 16-bit value in half and return the upper bits */
#define UPPER(a) ((uint8_t) (a >> 8))

/* Split 16-bit value in half and return the lower bits */
#define LOWER(a) ((uint8_t) a)

/* Set the nth bit of a */
#define SET_BIT(a, n) (a |= (1 << n))

/* Clear the nth bit of a */
#define CLR_BIT(a, n) (a &= ~(1 << n))

/* 1 if nth bit of a is set, 0 otherwise */
#define CHK_BIT(a, n) (!!((a) & (1 << n)))

/* For reference, the status register:
 * 7   6   5   4   3   2   1   0
 * S   V       B   D   I   Z   C
*/

/* 1 if a is 0, 0 otherwise */
#define CHECK_ZERO(a) (a == 0)

/* 1 if a is negative, 0 otherwise. */
#define CHECK_SIGN(a) (CHK_BIT(a, 7))

// todo: use XOR here instead for bonus points?
// todo: rename these perhaps..
#define SET_ZERO(a) (CHECK_ZERO(a) ? SET_BIT(P, 1) : CLR_BIT(P, 1))
#define SET_SIGN(a) (CHECK_SIGN(a) ? SET_BIT(P, 7) : CLR_BIT(P, 7))
#define SET_OVERFLOW(a) 

#define CARRY_SET() (CHK_BIT(P, 0))
#define ZERO_SET() (CHK_BIT(P, 1))
