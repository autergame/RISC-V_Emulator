#pragma once

#include <inttypes.h>

typedef struct instruction
{
	uint8_t type;
	union
	{
		struct op
		{
			uint32_t opcode : 7;
			uint32_t rest : 25;
		};

		struct I
		{
			uint32_t opcode : 7;
			uint32_t rd : 5;
			uint32_t funct3 : 3;
			uint32_t rs1 : 5;
			uint32_t imm : 12;
		} I;

		struct SB
		{
			uint32_t opcode : 7;
			uint32_t imm5 : 5;
			uint32_t funct3 : 3;
			uint32_t rs1 : 5;
			uint32_t rs2 : 5;
			uint32_t imm7 : 7;
		} SB;

		struct UJ
		{
			uint32_t opcode : 7;
			uint32_t rd : 5;
			uint32_t imm : 20;
		} UJ;

		struct R
		{
			uint32_t opcode : 7;
			uint32_t rd : 5;
			uint32_t funct3 : 3;
			uint32_t rs1 : 5;
			uint32_t rs2 : 5;
			uint32_t funct7 : 7;
		} R;

		uint32_t bits;
	};
} instruction;