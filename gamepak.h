#pragma once

typedef struct {
	uint8_t prg_rom_banks;
	uint8_t chr_rom_banks;
	uint8_t flag6;
	uint8_t flag7;
	uint8_t prg_ram_size;
	uint8_t flag9;
	uint8_t flag10;
} header_data_t;

typedef struct {
	header_data_t header_data;
	uint8_t* trainer; /* 512 bytes */
	uint8_t* prg_rom;
	uint8_t* chr_rom;
} gamepak_t;

int load(char* fpath, gamepak_t* gamepak);
int parse_header(uint8_t* hdr, header_data_t* data_out);

void print_header_data_t(header_data_t header_data);
void print_gamepak_t(gamepak_t gamepak);
