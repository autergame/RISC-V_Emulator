#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//=============================================//

int32_t sign_extend(uint32_t value, uint32_t bits)
{
	return ((int32_t)value << (32 - bits)) >> (32 - bits);
}

//=============================================//

// 1111_1111_1111_0000_0000_0000_0000_0000 -> 1 111_1111_1111_1111_1111 1111_1111_1111
int32_t i_imm(uint32_t inst)
{
	// 1111_1111_1111_0000_0000_0000_0000_0000 -> 0000_0000_0000_0000_0000_1111_1111_1111
	uint32_t i_imm1 = (inst & 0b11111111111100000000000000000000) >> 20;

	// 1 111_1111_1111_1111_1111 1111_1111_1111
	int32_t i_imm2 = sign_extend(i_imm1, 12); 

	return i_imm2;
}

// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1111_1111_0000_0000_0000_0000_0000
uint32_t i_imm_enc(int32_t value)
{
	// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1111_1111_0000_0000_0000_0000_0000
	uint32_t i_imm1 = (value & 0b00000000000000000000111111111111) << 20;

	return i_imm1;
}

//=============================================//

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
	int32_t s_imm4 = sign_extend(s_imm3, 12); 

	return s_imm4;
}

// 0000_0000_0000_0000_0000_1111_1111_1111 -> 1111_1110_0000_0000_0000_1111_1000_0000
uint32_t s_imm_enc(int32_t value)
{
	// 0000_0000_0000_0000_0000_1111_1110_0000 -> 1111_1110_0000_0000_0000_0000_0000_0000
	uint32_t s_imm1 = (value & 0b00000000000000000000111111100000) << 20;

	// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0000_0000_0000_0000_1111_1000_0000
	uint32_t s_imm2 = (value & 0b00000000000000000000000000011111) << 7;

	// 1111_1110_0000_0000_0000_1111_1000_0000
	uint32_t s_imm3 = s_imm1 | s_imm2; 

	return s_imm3;
}

//=============================================//

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
	int32_t b_imm6 = sign_extend(b_imm5, 13); 

	return b_imm6;
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
	uint32_t b_imm5 = b_imm1 | b_imm2 | b_imm3 | b_imm4;

	return b_imm5;
}

//=============================================//

// 1111_1111_1111_1111_1111_0000_0000_0000 -> 0000_0000_0000_1111_1111_1111_1111_1111
uint32_t u_imm(uint32_t inst)
{
	// 1111_1111_1111_1111_1111_0000_0000_0000 -> 0000_0000_0000_1111_1111_1111_1111_1111
	uint32_t u_imm1 = (inst & 0b11111111111111111111000000000000) >> 12;

	return u_imm1;
}

// 0000_0000_0000_1111_1111_1111_1111_1111 -> 1111_1111_1111_1111_1111_0000_0000_0000
uint32_t u_imm_enc(uint32_t value)
{
	// 0000_0000_0000_1111_1111_1111_1111_1111 -> 1111_1111_1111_1111_1111_0000_0000_0000
	uint32_t u_imm1 = (value & 0b00000000000011111111111111111111) << 12;

	return u_imm1;
}

//=============================================//

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
	int32_t j_imm6 = sign_extend(j_imm5, 21); 

	return j_imm6;
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
	uint32_t j_imm5 = j_imm1 | j_imm2 | j_imm3 | j_imm4; 

	return j_imm5;
}

//=============================================//

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

//=============================================//

#define lui 0b0110111
#define auipc 0b0010111

#define jal 0b1101111
#define jalr 0b1100111
 
// beq bne blt bge bltu bgeu
#define opcode_branch 0b1100011

// lb lh lw lbu lhu
#define opcode_load 0b0000011

// sb sh sw
#define opcode_store 0b0100011

// addi slti sltiu xori ori andi slli srli srai
#define opcode_alu_and_shift_imm 0b0010011

// add sub sll slt sltu xor srl sra or and
#define opcode_alu_register 0b0110011

// ecall ebreak csrrw csrrs csrrc csrrwi csrrsi csrrci
#define opcode_e_and_system 0b1110011

// jalr beq lb sb addi add sub
#define funct3_0_000 0b000

// bne lh sh slli sll csrrw
#define funct3_1_001 0b001

// lw sw slti slt csrrs
#define funct3_2_010 0b010

// sltiu sltu csrrc
#define funct3_3_011 0b011

// blt lbu xori xor 
#define funct3_4_100 0b100

// bge lhu srli srai srl sra csrrwi
#define funct3_5_101 0b101

// bltu ori or csrrsi
#define funct3_6_110 0b110

// bgeu andi and csrrci
#define funct3_7_111 0b111

// slli srli add sll slt sltu xor srl or and
#define funct7_0_0000000 0b0000000

// srai sub sra
#define funct7_64_0100000 0b0100000

//// 0000_0000_0000_0000_0000_0000_0000_0111 -> 0000_0000_0000_0000_0111_0000_0000_0000
//#define funct3_enc(value) value << 12
//
//// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0000_0000_0000_0000_1111_1000_0000 
//#define rd_enc(value) value << 7
//
//// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0000_0000_1111_1000_0000_0000_0000 
//#define rs1_enc(value) value << 15
//
//// 0000_0000_0000_0000_0000_0000_0001_1111 -> 0000_0001_1111_0000_0000_0000_0000_0000 
//#define rs2_enc(value) value << 20

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

typedef struct riscv_cpu
{
	uint8_t* memory;
	uint32_t* registers;
	uint32_t program_counter;
} riscv_cpu;

void execute_inst(instruction inst, riscv_cpu* cpu)
{
	cpu->registers[0] = 0;

	switch (inst.opcode)
	{
		case lui:
			break;

		case auipc:
			break;

		case jal:
			break;

		case jalr:
			break;

		case opcode_branch:
			break;

		case opcode_load:
			break;

		case opcode_store:
			break;

		case opcode_alu_and_shift_imm:
		{				
			switch (inst.I.funct3)
			{
				case funct3_0_000:
				{
					cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
					break;
				}

				default:
					break;
			}
			break;
		}

		case opcode_alu_register:
		{
			switch (inst.R.funct3)
			{
				case funct3_0_000:
				{
					switch (inst.R.funct7)
					{
						case funct7_0_0000000:
						{
							cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] + cpu->registers[inst.R.rs2];
							break;
						}

						case funct7_64_0100000:
						{
							cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] - cpu->registers[inst.R.rs2];
							break;
						}

						default:
							break;
					}
					break;
				}

				default:
					break;
			}
			break;
		}

		case opcode_e_and_system:
			break;

		default:
			break;
	}
}

int main(int argc, char* *argv)
{
	//uint32_t inst_i  = 0b00000000000011111111111111111111; // 000000000000_11111_111_11111_1111111
	//uint32_t inst_sb = 0b00000001111111111111000001111111; // 0000000_11111_11111_111_00000_1111111
	//uint32_t inst_uj = 0b00000000000000000000111111111111; // 00000000000000000000_11111_1111111

	//instruction inst_i = {
	//	.type = 1,
	//	.I = {
	//		.opcode = 0b1111111,
	//		.rd = 0b11111,
	//		.funct3 = 0b111,
	//		.rs1 = 0b11111,
	//		.imm = 0
	//	}
	//};

	//instruction inst_sb = {
	//	.type = 2,
	//	.SB = {
	//		.opcode = 0b1111111,
	//		.imm5 = 0,
	//		.funct3 = 0b111,
	//		.rs1 = 0b11111,
	//		.rs2 = 0b11111,
	//		.imm7 = 0
	//	}
	//};

	//instruction inst_uj = {
	//	.type = 3,
	//	.UJ = {
	//		.opcode = 0b1111111,
	//		.rd = 0b11111,
	//		.imm = 0
	//	}
	//};

	//test_imm_s("I", inst_i.bits,     -2048,       2047, i_imm_enc, i_imm, 1);
	//test_imm_s("S", inst_sb.bits,    -2048,       2047, s_imm_enc, s_imm, 1);
	//test_imm_s("B", inst_sb.bits,    -4096,       4095, b_imm_enc, b_imm, 2);
	//test_imm_s("J", inst_uj.bits, -1048576,    1048575, j_imm_enc, j_imm, 2);
	//test_imm_u("U", inst_uj.bits,        0, 0x000fffff, u_imm_enc, u_imm, 1);

	instruction inst_list[] = {
		{
			.type = 1,
			.I = {
				.opcode = opcode_alu_and_shift_imm,
				.rd = 1,
				.funct3 = funct3_0_000,
				.rs1 = 0
			}
		},
		{
			.type = 1,
			.I = {
				.opcode = opcode_alu_and_shift_imm,
				.rd = 2,
				.funct3 = funct3_0_000,
				.rs1 = 0
			}
		},
		{
			.type = 4,
			.R = {
				.opcode = opcode_alu_register,
				.rd = 3,
				.funct3 = funct3_0_000,
				.rs1 = 1,
				.rs2 = 2,
				.funct7 = funct7_64_0100000
			}
		},
	};

	inst_list[0].bits |= i_imm_enc(788);
	inst_list[1].bits |= i_imm_enc(344);

	riscv_cpu cpu = {
		.program_counter = 0,
		.memory = (uint8_t*)calloc(1024 * 1024 * 4, sizeof(uint8_t)),
		.registers = (uint32_t*)calloc(32, sizeof(uint32_t)),
	};

	for (uint32_t i = 0; i < 3; i++)
	{
		cpu.program_counter += 4;

		execute_inst(inst_list[i], &cpu);

		if (cpu.program_counter == 0)
			break;
	}

	free(cpu.memory);
	free(cpu.registers);

	return 0;
}