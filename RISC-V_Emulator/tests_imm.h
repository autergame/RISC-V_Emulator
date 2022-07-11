#pragma once

#include <stdio.h>

#include "inst_defs.h"
#include "myassert.h"

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
			panic("%s error: %" PRId32 " %" PRId32 "\n", name, x, inst_x_dec);
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
			panic("%s error: %" PRId32 " %" PRId32 "\n", name, x, inst_x_dec);
			break;
		}
	}
}

void tests_imm()
{
	uint32_t inst_imm_i = 0b00000000000011111111111111111111; // 000000000000_11111_111_11111_1111111
	uint32_t inst_imm_s = 0b00000001111111111111000001111111; // 0000000_11111_11111_111_00000_1111111
	uint32_t inst_imm_b = 0b00000001111111111111000001111111; // 0000000_11111_11111_111_00000_1111111
	uint32_t inst_imm_u = 0b00000000000000000000111111111111; // 00000000000000000000_11111_1111111
	uint32_t inst_imm_j = 0b00000000000000000000111111111111; // 00000000000000000000_11111_1111111

	test_imm_s("I", inst_imm_i,      -2048,       2047, i_imm_enc, i_imm, 1);
	test_imm_s("S", inst_imm_s,      -2048,       2047, s_imm_enc, s_imm, 1);
	test_imm_s("B", inst_imm_b,      -4096,       4095, b_imm_enc, b_imm, 2);
	test_imm_u("U", inst_imm_u, 0x00000000, 0xffffe000, u_imm_enc, u_imm, 0x1000);
	test_imm_s("J", inst_imm_j,   -1048576,    1048575, j_imm_enc, j_imm, 2);

	uint32_t inst_imm_shift = 0b11111110000011111111111111111111; // 1111111_00000_11111_111_11111_1111111
	test_imm_u("Shift", inst_imm_shift, 0, 31, i_shamt_imm_enc, i_shamt_imm, 1);
}

void test_inst_imm_s(const char* name, instruction inst, int32_t start, int32_t end,
	instruction(*inst_x_imm_enc)(instruction, int32_t), int32_t(*inst_i_imm)(instruction), int32_t inc)
{
	for (int32_t x = start; x <= end; x += inc)
	{
		instruction inst_x_enc = inst_x_imm_enc(inst, x);
		int32_t inst_x_dec = inst_i_imm(inst_x_enc);
		if (x != inst_x_dec)
		{
			panic("%s error: %" PRId32 " %" PRId32 "\n", name, x, inst_x_dec);
			break;
		}
	}
}

void test_inst_imm_u(const char* name, instruction inst, uint32_t start, uint32_t end,
	instruction(*inst_x_imm_enc)(instruction, uint32_t), uint32_t(*inst_i_imm)(instruction), uint32_t inc)
{
	for (uint32_t x = start; x <= end; x += inc)
	{
		instruction inst_x_enc = inst_x_imm_enc(inst, x);
		uint32_t inst_x_dec = inst_i_imm(inst_x_enc);
		if (x != inst_x_dec)
		{
			panic("%s error: %" PRId32 " %" PRId32 "\n", name, x, inst_x_dec);
			break;
		}
	}
}

void tests_inst_imm()
{
	instruction inst_imm_i = inst_i(0b1111111, 0b111, 31, 31, 0b000000000000); // 000000000000_11111_111_11111_1111111
	instruction inst_imm_s = inst_s(0b1111111, 0b111, 31, 31, 0b000000000000); // 0000000_11111_11111_111_00000_1111111
	instruction inst_imm_b = inst_b(0b1111111, 0b111, 31, 31, 0b000000000000); // 0000000_11111_11111_111_00000_1111111
	instruction inst_imm_u = inst_u(0b1111111, 31, 0b00000000000000000000);    // 00000000000000000000_11111_1111111
	instruction inst_imm_j = inst_j(0b1111111, 31, 0b00000000000000000000);    // 00000000000000000000_11111_1111111

	test_inst_imm_s("I", inst_imm_i,      -2048,       2047, inst_i_imm_enc, inst_i_imm, 1);
	test_inst_imm_s("S", inst_imm_s,      -2048,       2047, inst_s_imm_enc, inst_s_imm, 1);
	test_inst_imm_s("B", inst_imm_b,      -4096,       4095, inst_b_imm_enc, inst_b_imm, 2);
	test_inst_imm_u("U", inst_imm_u, 0x00000000, 0xffffe000, inst_u_imm_enc, inst_u_imm, 0x1000);
	test_inst_imm_s("J", inst_imm_j,   -1048576,    1048575, inst_j_imm_enc, inst_j_imm, 2);

	instruction inst_imm_shift = inst_shift(0b1111111, 0b111, 0b1111111, 31, 31, 0b00000); // 1111111_00000_11111_111_11111_1111111
	test_inst_imm_u("Shift", inst_imm_shift, 0, 31, inst_i_shamt_imm_enc, inst_i_shamt_imm, 1);
}