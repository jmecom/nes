#pragma once

/* Print error message and quit */
#define ERROR(error_message) \
    printf("[ERROR] %s\n", error_message); \
    printf("[ERROR] Exited in function \"%s\" on line %d in %s\n", __func__, __LINE__, __FILE__); \
    exit(1)
    
/* Combine two 8-bit values to one **little-endian** 16-bit value. */
#define COMBINE(a, b) ((uint16_t) (((b) << 8) | (a)))

/* Split 16-bit value in half and return the upper bits */
#define UPPER(a) ((uint8_t) ((a) >> 8))

/* Split 16-bit value in half and return the lower bits */
#define LOWER(a) ((uint8_t) (a))

/* Set the nth bit of a */
#define SET_BIT(a, n) ((a) |= (1 << (n)))

/* Clear the nth bit of a */
#define CLR_BIT(a, n) ((a) &= ~(1 << (n)))

/* 1 if nth bit of a is set, 0 otherwise */
#define CHK_BIT(a, n) (!!((a) & (1 << (n))))

/* Sets b[n] = a[n] */
#define CPY_BIT(a, b, n) if (CHK_BIT(a, n)) SET_BIT(b, n); else CLR_BIT(b, n);

/* For reference, the status register:
 * 7   6   5   4   3   2   1   0
 * S   V       B   D   I   Z   C
*/

#define CHECK_ZERO(a) (a == 0)
#define CHECK_SIGN(a) (CHK_BIT(a, 7))

// Set flag if result of a is zero / signed, otherwise clear
#define SET_ZERO(a) (CHECK_ZERO(a) ? SET_BIT(P, 1) : CLR_BIT(P, 1))
#define SET_SIGN(a) (CHECK_SIGN(a) ? SET_BIT(P, 7) : CLR_BIT(P, 7))
#define SET_SIGN_ZERO(a) SET_SIGN(a); SET_ZERO(a)

// Set flag if a is true, otherwise clear
#define SET_CARRY(a) (a ? SET_BIT(P, 0) : CLR_BIT(P, 0))
#define SET_INTERRUPT(a) (a ? SET_BIT(P, 2) : CLR_BIT(P, 2))
#define SET_DECIMAL(a) (a ? SET_BIT(P, 3) : CLR_BIT(P, 3))
#define SET_B(a) (a ? SET_BIT(P, 4) : CLR_BIT(P, 4))
#define SET_OVERFLOW(a) (a ? SET_BIT(P, 6) : CLR_BIT(P, 6))

/* Each macro below returns if the respective flag in P is set */
#define CARRY_SET() (CHK_BIT(P, 0))
#define ZERO_SET() (CHK_BIT(P, 1))
#define INTERRUPT_SET() (CHK_BIT(P, 2))
#define DECIMAL_SET() (CHK_BIT(P, 3))
#define B_SET() (CHK_BIT(P, 4))
#define OVERFLOW_SET() (CHK_BIT(P, 6))
#define SIGN_SET() (CHK_BIT(P, 7))

/* Increments CYC for branch instructions. Counts PPU cycles (3 per CPU cycle). */
#define BRANCH_CYCLE_INCREMENT(a) (CYC += UPPER(PC) == UPPER(a) ? 3 : 6)

/* Increments CYC for instructions where crossing a page costs an additional CPU cycle */
#define PAGE_CROSS_CYCLE_INCREMENT(a, b) (CYC += UPPER(a) == UPPER(b) ? 0 : 3)

/* Indirect, X addressing mode */
#define INDIRECT_X(lower) (COMBINE(read(COMBINE(lower, 0)), read(COMBINE((uint8_t)(lower+1), 0))))