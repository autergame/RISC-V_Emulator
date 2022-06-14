#pragma once

#include <stdio.h>
#include <inttypes.h>

#include "instructions.h"
#include "imm_enc_dec.h"

void test_imm_s(const char* name, uint32_t inst, int32_t start, int32_t end,
	uint32_t(*x_imm_enc)(int32_t), int32_t(*x_imm)(uint32_t), int32_t inc)
{
	for (int32_t x = start; x <= end; x += inc)
	{
		uint32_t inst_x_enc = x_imm_enc(x);

		uint32_t inst_x = inst_x_enc | inst;

		int32_t inst_x_dec = x_imm(inst_x);

		if (x != inst_x_dec)
		{
			printf("%s error: %" PRId32 " %" PRId32 "\n", name, x, inst_x_dec);
			break;
		}
	}
}

void test_imm_u(const char* name, uint32_t inst, uint32_t start, uint32_t end,
	uint32_t(*x_imm_enc)(uint32_t), uint32_t(*x_imm)(uint32_t), uint32_t inc)
{
	for (uint32_t x = start; x <= end; x += inc)
	{
		uint32_t inst_x_enc = x_imm_enc(x);

		uint32_t inst_x = inst_x_enc | inst;

		uint32_t inst_x_dec = x_imm(inst_x);

		if (x != inst_x_dec)
		{
			printf("%s error: %" PRId32 " %" PRId32 "\n", name, x, inst_x_dec);
			break;
		}
	}
}

void tests_imm()
{
	//uint32_t inst_i  = 0b00000000000011111111111111111111; // 000000000000_11111_111_11111_1111111
	//uint32_t inst_sb = 0b00000001111111111111000001111111; // 0000000_11111_11111_111_00000_1111111
	//uint32_t inst_uj = 0b00000000000000000000111111111111; // 00000000000000000000_11111_1111111

	instruction inst_i = {
		.type = 1,
		.I = {
			.opcode = 0b1111111,
			.rd = 31,
			.funct3 = 0b111,
			.rs1 = 31,
			.imm = 0
		}
	};

	instruction inst_sb = {
		.type = 2,
		.SB = {
			.opcode = 0b1111111,
			.imm5 = 0,
			.funct3 = 0b111,
			.rs1 = 31,
			.rs2 = 31,
			.imm7 = 0
		}
	};

	instruction inst_uj = {
		.type = 3,
		.UJ = {
			.opcode = 0b1111111,
			.rd = 31,
			.imm = 0
		}
	};

	test_imm_s("I", inst_i.bits,     -2048,       2047, i_imm_enc, i_imm, 1);
	test_imm_s("S", inst_sb.bits,    -2048,       2047, s_imm_enc, s_imm, 1);
	test_imm_s("B", inst_sb.bits,    -4096,       4095, b_imm_enc, b_imm, 2);
	test_imm_s("J", inst_uj.bits, -1048576,    1048575, j_imm_enc, j_imm, 2);
	test_imm_u("U", inst_uj.bits,        0, 0x000fffff, u_imm_enc, u_imm, 1);
}