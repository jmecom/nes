#pragma once

// typedef struct {
// 	uint8_t todo[0x1FE0];     // 0x4020 - 0x6000   todo: what is this used for?
// 	uint8_t work_ram[0x1FFF]; // 0x6000 - 0x7FFF
// 	uint8_t rom[0x7FFF];      // 0x8000 - 0xFFFF
// } cartridge_t;

// typedef struct {
// 	// https://wiki.nesdev.com/w/index.php/CPU_memory_map
// 	uint8_t internal_ram[0x8000];
// 	uint8_t mirror1[0x800];  // todo: name the mirrors better?
// 	uint8_t mirror2[0x800]; 
// 	uint8_t mirror3[0x800];
// 	uint8_t ppu_registers[0x0008];
// 	uint8_t mirror4[0x1FF8];
// 	uint8_t apu_io_registers[0x0018];
// 	uint8_t apu_io[0x0008];
// 	cartridge_t cartridge;
// } memory_t;

// const int PRG_ROM_UPPER_BANK_UPPER_LIMIT = 0x10000;
// const int PRG_ROM_UPPER_BANK_LOWER_LIMIT = 0xC000;
// const int PRG_ROM_LOWER_BANK_UPPER_LIMIT = 

extern const int PRG_ROM_UPPER_LIMIT;
extern const int PRG_ROM_LOWER_LIMIT;