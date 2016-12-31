#pragma once 

#include <stdint.h>

/* Addressing modes */
#define IMMEDIATE 1
#define ABSOLUTE 2
#define ZERO_PAGE_ABSOLUTE 3
#define IMPLIED 4
#define ACCUMULATOR 5
#define INDEXED 6
#define ZERO_PAGE_INDEXED 7
#define INDIRECT 8
#define PRE_INDEXED_INDIRECT 9
#define POST_INDEXED_INDIRECT 10
#define RELATIVE 11

/* Memory */
#define PRG_ROM_UPPER_LIMIT 0x10000
#define PRG_ROM_LOWER_LIMIT 0x8000

#define RAM_SIZE 2048
#define RAM_MIRROR_SIZE 6144
#define RAM_MIRROR_UPPER_LIMIT 0x2000
#define RAM_UPPER_LIMIT 0x0800

#define SRAM_SIZE 8192
#define SRAM_UPPER_LIMIT 0x8000
#define SRAM_LOWER_LIMIT 0x6000

/* Lookup tables */
const uint8_t instr_bytes[256];
const uint8_t instr_cycles[256];
const uint8_t instr_ppu_cycles[256];
const uint8_t instr_page_cycles[256];
const char* instr_names[256];

/* PPU */

// "CYC actually represents PPU dots, and the PPU outputs three dots for each 
// CPU cycle (unless you're emulating a PAL NES, in which case it outputs an extra dot 
// every fifth CPU cycle). For example, LDA absolute takes four cycles according to your 6502 
// book, but it takes 12 CYC according to a Nintendulator execution log. As sub_par pointed out,
// SL refers to scanlines, and each SL is worth 341 CYC."
// https://forums.nesdev.com/viewtopic.php?f=3&t=10266
#define SL_RESET 341