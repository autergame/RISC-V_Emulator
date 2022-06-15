#pragma once

#include <inttypes.h>

typedef struct instruction
{
	union
	{
		struct op
		{
			uint32_t opcode : 7;
			uint32_t rest : 25;
		};

		struct R
		{
			uint32_t opcode : 7;
			uint32_t rd : 5;
			uint32_t funct3 : 3;
			uint32_t rs1 : 5;
			uint32_t rs2 : 5;
			uint32_t funct7 : 7;
		} R;

		union  
		{
			struct I
			{
				uint32_t opcode : 7;
				uint32_t rd : 5;
				uint32_t funct3 : 3;
				uint32_t rs1 : 5;
				uint32_t imm12 : 12;
			} I;

			struct shift
			{
				uint32_t opcode : 7;
				uint32_t rd : 5;
				uint32_t func3 : 3;
				uint32_t rs1 : 5;
				uint32_t shamt : 5;
				uint32_t func7 : 7;
			} shift;
		};

		struct S
		{
			uint32_t opcode : 7;
			uint32_t imm5 : 5;
			uint32_t funct3 : 3;
			uint32_t rs1 : 5;
			uint32_t rs2 : 5;
			uint32_t imm7 : 7;
		} S;

		struct B
		{
			uint32_t opcode : 7;
			uint32_t imm1_1 : 1;
			uint32_t imm4 : 4;
			uint32_t funct3 : 3;
			uint32_t rs1 : 5;
			uint32_t rs2 : 5;
			uint32_t imm6 : 6;
			uint32_t imm1_2 : 1;
		} B;

		struct U
		{
			uint32_t opcode : 7;
			uint32_t rd : 5;
			uint32_t imm20 : 20;
		} U;

		struct J
		{
			uint32_t opcode : 7;
			uint32_t rd : 5;
			uint32_t imm20 : 20;
		} J;

		uint32_t bits;
	};
} instruction;