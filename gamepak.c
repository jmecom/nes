#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gamepak.h"

const char* HDR_MAGIC = "NES\x1a";
const int HDR_MAGIC_LEN = 4;
const int HDR_LEN = 16;
const int TRAINER_LEN = 512;
const int PRG_ROM_BANK_LEN = 16384;
const int CHR_ROM_BANK_LEN = 8192;

int load(char* fpath, gamepak_t* gamepak) {
	FILE *f;
    f = fopen(fpath, "r");
    
    // Read and parse header data
    uint8_t hdr[HDR_LEN];
    fread(hdr, 1, HDR_LEN, f);

    if (parse_header(hdr, &(gamepak->header_data)) != 0) {
    	return 1;
    }

	// If it exists, read the trainer
    if (gamepak->header_data.flag6 & 4) {
    	gamepak->trainer = malloc(TRAINER_LEN);
		fread(gamepak->trainer, 1, TRAINER_LEN, f);
    }

    // Read PRG-ROM
    uint32_t prg_rom_size = gamepak->header_data.prg_rom_banks * PRG_ROM_BANK_LEN;
    if (prg_rom_size <= 0) {
    	return 2;
    }

    gamepak->prg_rom = malloc(PRG_ROM_BANK_LEN * 2);
    if (!gamepak->prg_rom) {
    	return 3;
    }

    fread(gamepak->prg_rom, 1, prg_rom_size, f);

	// Games with only one 16KB bank of PRG-ROM will load into both the upper 
    // and lower banks to ensure the vector table is located at both addresses
    if (prg_rom_size == PRG_ROM_BANK_LEN) {
    	memcpy(gamepak->prg_rom + PRG_ROM_BANK_LEN, gamepak->prg_rom, PRG_ROM_BANK_LEN);
    }

    // Read CHR-ROM
	uint32_t chr_rom_size = gamepak->header_data.chr_rom_banks * CHR_ROM_BANK_LEN;
    if (chr_rom_size <= 0) {
    	return 4;
    }

    gamepak->chr_rom = malloc(chr_rom_size);
	if (!gamepak->chr_rom) {
    	return 5;
    }

    fread(gamepak->chr_rom, 1, chr_rom_size, f);

    return 0;
}

int parse_header(uint8_t* hdr, header_data_t *data_out) {
	if (strncmp((char*)hdr, HDR_MAGIC, HDR_MAGIC_LEN) != 0) {
		return 1;
	}

	header_data_t data = {
		.prg_rom_banks = hdr[4],
		.chr_rom_banks = hdr[5],
		.flag6 = hdr[6],
		.flag7 = hdr[7],
		.prg_ram_size = hdr[8],
		.flag9 = hdr[9],
		.flag10 = hdr[10]
	};

	*data_out = data;

	return 0;
}

void print_header_data_t(header_data_t header_data) {
	printf("header_data\n");	
	printf("  prg_rom_banks: %d\n", header_data.prg_rom_banks);
	printf("  chr_rom_banks: %d\n", header_data.chr_rom_banks);
	printf("  flag6: %d\n", header_data.flag6);
	printf("  flag7: %d\n", header_data.flag7);
	printf("  prg_ram_size: %d\n", header_data.prg_ram_size);
	printf("  flag9: %d\n", header_data.flag9);
	printf("  flag10: %d\n", header_data.flag10);
}

void print_gamepak_t(gamepak_t gamepak) {
	printf("gamepak\n");
	print_header_data_t(gamepak.header_data);	
}