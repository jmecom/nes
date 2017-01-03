#pragma once 

#include <stdint.h>

/* Addressing modes
 * http://www.obelisk.me.uk/6502/addressing.html
 */
#define ABSOLUTE 1
#define ABSOLUTE_X 2
#define ABSOLUTE_Y 3
#define ACCUMULATOR 4
#define IMMEDIATE 5
#define IMPLIED 6
#define INDEXED_INDIRECT 7
#define INDIRECT 8
#define INDIRECT_INDEXED 9
#define RELATIVE 10
#define ZERO_PAGE 11
#define ZERO_PAGE_X 12
#define ZERO_PAGE_Y 13

/* Gamepak */
#define HDR_MAGIC "NES\x1a"
#define HDR_MAGIC_LEN 4
#define HDR_LEN 16
#define TRAINER_LEN 512
#define PRG_ROM_BANK_LEN 16384
#define CHR_ROM_BANK_LEN 8192

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

/* Lookup tables 
 * Rather than retyping by hand, these are taken from 
 * https://github.com/fogleman/nes/blob/master/nes/cpu.go
 */
const uint8_t instr_bytes[256];
const uint8_t instr_cycles[256];
const uint8_t instr_ppu_cycles[256];
const uint8_t instr_page_cycles[256];
const uint8_t instr_modes[256];
const char* instr_names[256];

/* PPU */

// "CYC actually represents PPU dots, and the PPU outputs three dots for each 
// CPU cycle (unless you're emulating a PAL NES, in which case it outputs an extra dot 
// every fifth CPU cycle). For example, LDA absolute takes four cycles according to your 6502 
// book, but it takes 12 CYC according to a Nintendulator execution log. As sub_par pointed out,
// SL refers to scanlines, and each SL is worth 341 CYC."
// https://forums.nesdev.com/viewtopic.php?f=3&t=10266
#define SL_RESET 341