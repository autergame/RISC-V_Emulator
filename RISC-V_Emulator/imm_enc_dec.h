#pragma once

#include "instructions.h"

int32_t sign_extend(uint32_t value, uint32_t bits)
{
	return ((int32_t)value << (32 - bits)) >> (32 - bits);
}

#pragma region uint32 int32 decoding encoding

// 1111_1111_1111_0000_0000_0000_0000_0000 -> 1 111_1111_1111_1111_1111 1111_1111_1111
int32_t i_imm(uint32_t inst)
{
	// 1111_1111_1111_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_1111_1111_1111
	uint32_t i_imm1 = (inst & 0b11111111111100000000000000000000) >> 20;

	// 1 111_1111_1111_1111_1111 1111_1111_1111
	return sign_extend(i_imm1, 12);
}

// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1111_1111_0000_0000_0000_0000_0000
uint32_t i_imm_enc(int32_t value)
{
	return (value & 0b00000000000000000000111111111111) << 20;
}

// 0000_0001_1111_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_0000_0001_1111
uint32_t i_shamt_imm(uint32_t inst)
{
	return (inst & 0b00000001111100000000000000000000) >> 20;
}

// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0001_1111_0000_0000_0000_0000_0000
uint32_t i_shamt_imm_enc(uint32_t value)
{
	return (value & 0b00000000000000000000000000011111) << 20;
}

// 1111_1110_0000_0000_0000_1111_1000_0000 -> 1 111_1111_1111_1111_1111 1111_1111_1111
int32_t s_imm(uint32_t inst)
{
	// 1111_1110_0000_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_1111_1110_0000
	uint32_t s_imm1 = (inst & 0b11111110000000000000000000000000) >> 20;

	// 0000_0000_0000_0000_0000_1111_1000_0000 -> 0000_0000_0000_0000_0000_0000_0001_1111
	uint32_t s_imm2 = (inst & 0b00000000000000000000111110000000) >> 7;

	// 0000_0000_0000_0000_0000_1111_1111_1111
	uint32_t s_imm3 = s_imm1 | s_imm2;

	// 1 111_1111_1111_1111_1111 1111_1111_1111
	return sign_extend(s_imm3, 12);
}

// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1110_0000_0000_0000_1111_1000_0000
uint32_t s_imm_enc(int32_t value)
{
	// 0000_0000_0000_0000_0000_1111_1110_0000 -> 1111_1110_0000_0000_0000_0000_0000_0000
	uint32_t s_imm1 = (value & 0b00000000000000000000111111100000) << 20;

	// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0000_0000_0000_0000_1111_1000_0000
	uint32_t s_imm2 = (value & 0b00000000000000000000000000011111) << 7;

	// 1111_1110_0000_0000_0000_1111_1000_0000
	return s_imm1 | s_imm2;
}

// 1111_1110_0000_0000_0000_1111_1000_0000 -> 1 111_1111_1111_1111_111 1_1111_1111_111 0
int32_t b_imm(uint32_t inst)
{
	// 1000_0000_0000_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0001_0000_0000_0000
	uint32_t b_imm1 = (inst & 0b10000000000000000000000000000000) >> 19;

	// 0111_1110_0000_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_0111_1110_0000
	uint32_t b_imm2 = (inst & 0b01111110000000000000000000000000) >> 20;

	// 0000_0000_0000_0000_0000_1111_0000_0000 -> 0000_0000_0000_0000_0000_0000_0001_1110
	uint32_t b_imm3 = (inst & 0b00000000000000000000111100000000) >> 7;

	// 0000_0000_0000_0000_0000_0000_1000_0000 -> 0000_0000_0000_0000_0000_1000_0000_0000
	uint32_t b_imm4 = (inst & 0b00000000000000000000000010000000) << 4;

	// 0000_0000_0000_0000_0001_1111_1111_1110
	uint32_t b_imm5 = b_imm1 | b_imm2 | b_imm3 | b_imm4;

	// 1 111_1111_1111_1111_111 1_1111_1111_111 0
	return sign_extend(b_imm5, 13);
}

// 0000_0000_0000_0000_0001_1111_1111_1110 -> 1111_1110_0000_0000_0000_1111_1000_0000
uint32_t b_imm_enc(int32_t value)
{
	// 0000_0000_0000_0000_0001_0000_0000_0000 -> 1000_0000_0000_0000_0000_0000_0000_0000
	uint32_t b_imm1 = (value & 0b00000000000000000001000000000000) << 19;

	// 0000_0000_0000_0000_0000_0111_1110_0000 -> 0111_1110_0000_0000_0000_0000_0000_0000
	uint32_t b_imm2 = (value & 0b00000000000000000000011111100000) << 20;

	// 0000_0000_0000_0000_0000_0000_0001_1110 -> 0000_0000_0000_0000_0000_1111_0000_0000
	uint32_t b_imm3 = (value & 0b00000000000000000000000000011110) << 7;

	// 0000_0000_0000_0000_0000_1000_0000_0000 -> 0000_0000_0000_0000_0000_0000_1000_0000
	uint32_t b_imm4 = (value & 0b00000000000000000000100000000000) >> 4;

	// 1111_1110_0000_0000_0000_1111_1000_0000
	return b_imm1 | b_imm2 | b_imm3 | b_imm4;
}

// 1111_1111_1111_1111_1111_0000_0000_0000 
uint32_t u_imm(uint32_t inst)
{
	return (inst & 0b11111111111111111111000000000000);
}

// 1111_1111_1111_1111_1111_0000_0000_0000
uint32_t u_imm_enc(uint32_t value)
{
	return (value & 0b11111111111111111111000000000000);
}

// 1111_1111_1111_1111_1111_0000_0000_0000 -> 1 111_1111_111 1_1111_1111_1111_1111_111 0
int32_t j_imm(uint32_t inst)
{
	// 1000_0000_0000_0000_0000_0000_0000_0000 -> 0000_0000_0001_0000_0000_0000_0000_0000
	uint32_t j_imm1 = (inst & 0b10000000000000000000000000000000) >> 11;

	// 0111_1111_1110_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_0111_1111_1110
	uint32_t j_imm2 = (inst & 0b01111111111000000000000000000000) >> 20;

	// 0000_0000_0001_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_1000_0000_0000
	uint32_t j_imm3 = (inst & 0b00000000000100000000000000000000) >> 9;

	// 0000_0000_0000_1111_1111_0000_0000_0000
	uint32_t j_imm4 = (inst & 0b00000000000011111111000000000000);

	// 0000_0000_0001_1111_1111_1111_1111_1110
	uint32_t j_imm5 = j_imm1 | j_imm2 | j_imm3 | j_imm4;

	// 1 111_1111_111 1_1111_1111_1111_1111_111 0
	return sign_extend(j_imm5, 21);
}

// 0000_0000_0001_1111_1111_1111_1111_1110 -> 1111_1111_1111_1111_1111_0000_0000_0000
uint32_t j_imm_enc(int32_t value)
{
	// 0000_0000_0001_0000_0000_0000_0000_0000 -> 1000_0000_0000_0000_0000_0000_0000_0000
	uint32_t j_imm1 = (value & 0b00000000000100000000000000000000) << 11;

	// 0000_0000_0000_0000_0000_0111_1111_1110 -> 0111_1111_1110_0000_0000_0000_0000_0000
	uint32_t j_imm2 = (value & 0b00000000000000000000011111111110) << 20;

    // 0000_0000_0000_0000_0000_1000_0000_0000 -> 0000_0000_0001_0000_0000_0000_0000_0000
	uint32_t j_imm3 = (value & 0b00000000000000000000100000000000) << 9;

	// 0000_0000_0000_1111_1111_0000_0000_0000
	uint32_t j_imm4 = (value & 0b00000000000011111111000000000000);

	// 1111_1111_1111_1111_1111_0000_0000_0000
	return j_imm1 | j_imm2 | j_imm3 | j_imm4;
}

#pragma endregion

#pragma region instruction decoding encoding

// 1111_1111_1111_0000_0000_0000_0000_0000 -> 1 111_1111_1111_1111_1111 1111_1111_1111
int32_t inst_i_imm(instruction inst)
{
	return sign_extend(inst.I.imm11_0, 12);
}

// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1111_1111_0000_0000_0000_0000_0000
instruction inst_i_imm_enc(instruction inst, int32_t value)
{
	inst.I.imm11_0 = (value & 0b00000000000000000000111111111111);

	return inst;
}

// 0000_0001_1111_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_0000_0001_1111
uint32_t inst_i_shamt_imm(instruction inst)
{
	return inst.Shift.shamt_4_0;
}

// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0001_1111_0000_0000_0000_0000_0000
instruction inst_i_shamt_imm_enc(instruction inst, uint32_t value)
{
	inst.Shift.shamt_4_0 = (value & 0b00000000000000000000000000011111);

	return inst;
}

// 1111_1110_0000_0000_0000_1111_1000_0000 -> 1 111_1111_1111_1111_1111 1111_1111_1111
int32_t inst_s_imm(instruction inst)
{
	// 1111_1110_0000_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_1111_1110_0000
	uint32_t s_imm1 = inst.S.imm11_5 << 5;

	// 0000_0000_0000_0000_0000_1111_1000_0000 -> 0000_0000_0000_0000_0000_0000_0001_1111
	uint32_t s_imm2 = inst.S.imm4_0;

	// 0000_0000_0000_0000_0000_1111_1111_1111 
	uint32_t s_imm3 = s_imm1 | s_imm2;

	// 1 111_1111_1111_1111_1111 1111_1111_1111
	return sign_extend(s_imm3, 12);
}

// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1110_0000_0000_0000_1111_1000_0000
instruction inst_s_imm_enc(instruction inst, int32_t value)
{
	// 0000_0000_0000_0000_0000_1111_1110_0000 -> 1111_1110_0000_0000_0000_0000_0000_0000
	inst.S.imm11_5 = (value & 0b00000000000000000000111111100000) >> 5;

	// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0000_0000_0000_0000_1111_1000_0000
	inst.S.imm4_0  = (value & 0b00000000000000000000000000011111);

	return inst;
}

// 1111_1110_0000_0000_0000_1111_1000_0000 -> 1 111_1111_1111_1111_111 1_1111_1111_111 0
int32_t inst_b_imm(instruction inst)
{
	// 1000_0000_0000_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0001_0000_0000_0000
	uint32_t b_imm1 = inst.B.imm12   << 12;

	// 0000_0000_0000_0000_0000_0000_1000_0000 -> 0000_0000_0000_0000_0000_1000_0000_0000
	uint32_t b_imm2 = inst.B.imm11   << 11;

	// 0111_1110_0000_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_0111_1110_0000
	uint32_t b_imm3 = inst.B.imm10_5 << 5;

	// 0000_0000_0000_0000_0000_1111_0000_0000 -> 0000_0000_0000_0000_0000_0000_0001_1110
	uint32_t b_imm4 = inst.B.imm4_1  << 1;

	// 0000_0000_0000_0000_0001_1111_1111_1110
	uint32_t b_imm5 = b_imm1 | b_imm2 | b_imm3 | b_imm4;

	// 1 111_1111_1111_1111_111 1_1111_1111_111 0
	return sign_extend(b_imm5, 13);
}

// 0000_0000_0000_0000_0001_1111_1111_1110 -> 1111_1110_0000_0000_0000_1111_1000_0000
instruction inst_b_imm_enc(instruction inst, int32_t value)
{
	// 0000_0000_0000_0000_0001_0000_0000_0000 -> 1000_0000_0000_0000_0000_0000_0000_0000
	inst.B.imm12 =   (value & 0b00000000000000000001000000000000) >> 12;

	// 0000_0000_0000_0000_0000_1000_0000_0000 -> 0000_0000_0000_0000_0000_0000_1000_0000
	inst.B.imm11 =   (value & 0b00000000000000000000100000000000) >> 11;

	// 0000_0000_0000_0000_0000_0111_1110_0000 -> 0111_1110_0000_0000_0000_0000_0000_0000
	inst.B.imm10_5 = (value & 0b00000000000000000000011111100000) >> 5;

	// 0000_0000_0000_0000_0000_0000_0001_1110 -> 0000_0000_0000_0000_0000_1111_0000_0000
	inst.B.imm4_1 =  (value & 0b00000000000000000000000000011110) >> 1;

	return inst;
}

// 1111_1111_1111_1111_1111_0000_0000_0000
uint32_t inst_u_imm(instruction inst)
{
	return inst.U.imm31_12 << 12;
}

// 1111_1111_1111_1111_1111_0000_0000_0000
instruction inst_u_imm_enc(instruction inst, uint32_t value)
{
	inst.U.imm31_12 = (value & 0b11111111111111111111000000000000) >> 12;

	return inst;
}

// 1111_1111_1111_1111_1111_0000_0000_0000 -> 1 111_1111_111 1_1111_1111_1111_1111_111 0
int32_t inst_j_imm(instruction inst)
{
	// 1000_0000_0000_0000_0000_0000_0000_0000 -> 0000_0000_0001_0000_0000_0000_0000_0000
	uint32_t j_imm1 = inst.J.imm20    << 20;

	// 0000_0000_0000_1111_1111_0000_0000_0000 -> 0000_0000_0000_1111_1111_0000_0000_0000
	uint32_t j_imm2 = inst.J.imm19_12 << 12;

	// 0000_0000_0001_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_1000_0000_0000
	uint32_t j_imm3 = inst.J.imm11    << 11;

	// 0111_1111_1110_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_0111_1111_1110
	uint32_t j_imm4 = inst.J.imm10_1  << 1;

	// 0000_0000_0001_1111_1111_1111_1111_1110
	uint32_t j_imm5 = j_imm1 | j_imm2 | j_imm3 | j_imm4;

	// 1 111_1111_111 1_1111_1111_1111_1111_111 0
	return sign_extend(j_imm5, 21);
}

// 0000_0000_0001_1111_1111_1111_1111_1110 -> 1111_1111_1111_1111_1111_0000_0000_0000
instruction inst_j_imm_enc(instruction inst, int32_t value)
{
	// 0000_0000_0001_0000_0000_0000_0000_0000 -> 1000_0000_0000_0000_0000_0000_0000_0000
	inst.J.imm20 =    (value & 0b00000000000100000000000000000000) >> 20;

	// 0000_0000_0000_1111_1111_0000_0000_0000 -> 0000_0000_0000_1111_1111_0000_0000_0000
	inst.J.imm19_12 = (value & 0b00000000000011111111000000000000) >> 12;

	// 0000_0000_0000_0000_0000_1000_0000_0000 -> 0000_0000_0001_0000_0000_0000_0000_0000
	inst.J.imm11 =    (value & 0b00000000000000000000100000000000) >> 11;

	// 0000_0000_0000_0000_0000_0111_1111_1110 -> 0111_1111_1110_0000_0000_0000_0000_0000
	inst.J.imm10_1 =  (value & 0b00000000000000000000011111111110) >> 1;

	return inst;
}

#pragma endregion