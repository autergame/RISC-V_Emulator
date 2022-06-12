#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//=============================================//

int32_t sign_extend(uint32_t value, uint32_t bits)
{
	return ((int32_t)value << (32 - bits)) >> (32 - bits);
}

//=============================================//

int32_t i_imm(uint32_t inst)
{
	uint32_t i_imm1 = inst & 0xfff00000;       // 1111_1111_1111_0000_0000_0000_0000_0000

	uint32_t i_imm2 = i_imm1 >> 20;            // 0000_0000_0000_0000_0000_1111_1111_1111

	int32_t  i_imm3 = sign_extend(i_imm2, 12); // 1 111_1111_1111_1111_1111 1111_1111_1111

	return i_imm3;
}


uint32_t i_imm_enc(int32_t value)
{
	uint32_t i_imm1 = value & 0x00000fff; // 0000_0000_0000_0000_0000_1111_1111_1111

	uint32_t i_imm2 = i_imm1 << 20;       // 1111_1111_1111_0000_0000_0000_0000_0000

	return i_imm2;
}

//=============================================//

int32_t s_imm(uint32_t inst)
{
	uint32_t s_imm1 = inst & 0xfe000000;       // 1111_1110_0000_0000_0000_0000_0000_0000
	uint32_t s_imm2 = inst & 0x00000f80;       // 0000_0000_0000_0000_0000_1111_1000_0000

	uint32_t s_imm3 = s_imm1 >> 20;            // 0000_0000_0000_0000_0000_1111_1110_0000
	uint32_t s_imm4 = s_imm2 >> 7;             // 0000_0000_0000_0000_0000_0000_0001_1111

	uint32_t s_imm5 = s_imm3 | s_imm4;         // 0000_0000_0000_0000_0000_1111_1111_1111

	int32_t  s_imm6 = sign_extend(s_imm5, 12); // 1 111_1111_1111_1111_1111 1111_1111_1111

	return s_imm6;
}

uint32_t s_imm_enc(int32_t value)
{
	uint32_t s_imm1 = value & 0x00000fff;  // 0000_0000_0000_0000_0000_1111_1111_1111

	uint32_t s_imm2 = s_imm1 & 0x00000fe0; // 0000_0000_0000_0000_0000_1111_1110_0000
	uint32_t s_imm3 = s_imm1 & 0x0000001f; // 0000_0000_0000_0000_0000_0000_0001_1111

	uint32_t s_imm4 = s_imm2 << 20;        // 1111_1110_0000_0000_0000_0000_0000_0000
	uint32_t s_imm5 = s_imm3 << 7;         // 0000_0000_0000_0000_0000_1111_1000_0000

	uint32_t s_imm6 = s_imm4 | s_imm5;     // 1111_1110_0000_0000_0000_1111_1000_0000

	return s_imm6;
}

//=============================================//

int32_t b_imm(uint32_t inst)
{
	uint32_t b_imm1 = inst & 0x80000000;       // 1000_0000_0000_0000_0000_0000_0000_0000
	uint32_t b_imm2 = inst & 0x7e000000;       // 0111_1110_0000_0000_0000_0000_0000_0000
	uint32_t b_imm3 = inst & 0x00000f00;       // 0000_0000_0000_0000_0000_1111_0000_0000
	uint32_t b_imm4 = inst & 0x00000080;       // 0000_0000_0000_0000_0000_0000_1000_0000

	uint32_t b_imm5 = b_imm1 >> 19;            // 0000_0000_0000_0000_0001_0000_0000_0000
	uint32_t b_imm6 = b_imm2 >> 20;            // 0000_0000_0000_0000_0000_0111_1110_0000
	uint32_t b_imm7 = b_imm3 >> 7;			   // 0000_0000_0000_0000_0000_0000_0001_1110
	uint32_t b_imm8 = b_imm4 << 4;             // 0000_0000_0000_0000_0000_1000_0000_0000

	uint32_t b_imm9 = 
		b_imm5 | b_imm6 | b_imm7 | b_imm8;     // 0000_0000_0000_0000_0001_1111_1111_1110

	int32_t b_imm10 = sign_extend(b_imm9, 13); // 1 111_1111_1111_1111_111 1_1111_1111_111 0

	return b_imm10;
}

uint32_t b_imm_enc(int32_t value)
{
	uint32_t b_imm1 = value & 0x00001ffe;  // 0000_0000_0000_0000_0001_1111_1111_1110

	uint32_t b_imm2 = b_imm1 & 0x00001000; // 0000_0000_0000_0000_0001_0000_0000_0000
	uint32_t b_imm3 = b_imm1 & 0x000007e0; // 0000_0000_0000_0000_0000_0111_1110_0000
	uint32_t b_imm4 = b_imm1 & 0x0000001e; // 0000_0000_0000_0000_0000_0000_0001_1110
	uint32_t b_imm5 = b_imm1 & 0x00000800; // 0000_0000_0000_0000_0000_1000_0000_0000

	uint32_t b_imm6 = b_imm2 << 19;        // 1000_0000_0000_0000_0000_0000_0000_0000
	uint32_t b_imm7 = b_imm3 << 20;        // 0111_1110_0000_0000_0000_0000_0000_0000
	uint32_t b_imm8 = b_imm4 << 7;         // 0000_0000_0000_0000_0000_1111_0000_0000
	uint32_t b_imm9 = b_imm5 >> 4;         // 0000_0000_0000_0000_0000_0000_1000_0000

	uint32_t b_imm10 = 
		b_imm6 | b_imm7 | b_imm8 | b_imm9; // 1111_1110_0000_0000_0000_1111_1000_0000

	return b_imm10;
}

//=============================================//

uint32_t u_imm(uint32_t inst)
{
	uint32_t u_imm1 = inst & 0xfffff000; // 1111_1111_1111_1111_1111_0000_0000_0000

	uint32_t u_imm2 = u_imm1 >> 12;      // 0000_0000_0000_1111_1111_1111_1111_1111

	return u_imm2;
}

uint32_t u_imm_enc(uint32_t value)
{
	uint32_t u_imm1 = value & 0x000fffff; // 0000_0000_0000_1111_1111_1111_1111_1111

	uint32_t u_imm2 = u_imm1 << 12;       // 1111_1111_1111_1111_1111_0000_0000_0000

	return u_imm2;
}

//=============================================//

int32_t j_imm(uint32_t inst)
{
	uint32_t j_imm1 = inst & 0x80000000;      // 1000_0000_0000_0000_0000_0000_0000_0000
	uint32_t j_imm2 = inst & 0x7fe00000;      // 0111_1111_1110_0000_0000_0000_0000_0000
	uint32_t j_imm3 = inst & 0x00100000;      // 0000_0000_0001_0000_0000_0000_0000_0000
	uint32_t j_imm4 = inst & 0x000ff000;      // 0000_0000_0000_1111_1111_0000_0000_0000

	uint32_t j_imm5 = j_imm1 >> 11;           // 0000_0000_0001_0000_0000_0000_0000_0000
	uint32_t j_imm6 = j_imm2 >> 20;           // 0000_0000_0000_0000_0000_0111_1111_1110
	uint32_t j_imm7 = j_imm3 >> 9;            // 0000_0000_0000_0000_0000_1000_0000_0000 

	uint32_t j_imm8 = 
		j_imm4 | j_imm5 | j_imm6 | j_imm7;    // 0000_0000_0001_1111_1111_1111_1111_1110

	int32_t j_imm9 = sign_extend(j_imm8, 21); // 1 111_1111_111 1_1111_1111_1111_1111_111 0

	return j_imm9;
}

uint32_t j_imm_enc(int32_t value)
{
	uint32_t j_imm1 = value & 0x001fffff;  // 0000_0000_0001_1111_1111_1111_1111_1111

	uint32_t j_imm2 = j_imm1 & 0x00100000; // 0000_0000_0001_0000_0000_0000_0000_0000
	uint32_t j_imm3 = j_imm1 & 0x000ff000; // 0000_0000_0000_1111_1111_0000_0000_0000
	uint32_t j_imm4 = j_imm1 & 0x000007fe; // 0000_0000_0000_0000_0000_0111_1111_1110
	uint32_t j_imm5 = j_imm1 & 0x00000800; // 0000_0000_0000_0000_0000_1000_0000_0000

	uint32_t j_imm6 = j_imm2 << 11;        // 1000_0000_0000_0000_0000_0000_0000_0000
	uint32_t j_imm7 = j_imm4 << 20;        // 0111_1111_1110_0000_0000_0000_0000_0000
	uint32_t j_imm8 = j_imm5 << 9;         // 0000_0000_0001_0000_0000_0000_0000_0000

	uint32_t j_imm9 =
		j_imm3 | j_imm6 | j_imm7 | j_imm8; // 1111_1111_1111_1111_1111_0000_0000_0000

	return j_imm9;
}

//=============================================//

void test_imm(const char* name, uint32_t inst, int32_t start, int32_t end,
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

//=============================================//

int main(int argc, char* *argv)
{
	uint32_t inst_i  = 0b00000000000011111111111111111111; // 000000000000_11111_111_11111_1111111
	uint32_t inst_sb = 0b00000001111111111111000001111111; // 0000000_11111_11111_111_00000_1111111
	uint32_t inst_uj = 0b00000000000000000000111111111111; // 00000000000000000000_11111_1111111

	test_imm("I", inst_i, -2048, 2047, i_imm_enc, i_imm, 1);
	test_imm("S", inst_sb, -2048, 2047, s_imm_enc, s_imm, 1);
	test_imm("B", inst_sb, -4096, 4095, b_imm_enc, b_imm, 2);
	test_imm("J", inst_uj, -1048576, 1048575, j_imm_enc, j_imm, 2);

	for (uint32_t u = 0; u <= 0x000fffff; u += 1)
	{
		uint32_t inst_u_enc = u_imm_enc(u);

		uint32_t inst_u = inst_u_enc | inst_uj;

		uint32_t inst_u_dec = u_imm(inst_u);

		if (u != inst_u_dec)
		{
			printf("U error: %" PRId32 " %" PRId32 "\n", u, inst_u_dec);
			break;
		}
	}

	return 0;
}