#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "instructions.h"
#include "imm_enc_dec.h"
#include "types.h"
#include "cpu.h"

#include "execute.h"

//#include "tests.h"

int main(int argc, char* *argv)
{
	//tests_imm();

	instruction inst_list[] = {
		{
			.type = 1,
			.I = {
				.opcode = opcode_alu_and_shift_imm,
				.rd = 1,
				.funct3 = funct3_0_000,
				.rs1 = 0,
				.imm = i_imm_enc(788) >> 20
			}
		},
		{
			.type = 1,
			.I = {
				.opcode = opcode_alu_and_shift_imm,
				.rd = 2,
				.funct3 = funct3_0_000,
				.rs1 = 0,
				.imm = i_imm_enc(344) >> 20
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
				.funct7 = funct7_32_0100000
			}
		},
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	riscv_cpu cpu = create_riscv_cpu();

	for (uint32_t i = 0; i < inst_list_size; i++)
	{
		*((uint32_t*)cpu.memory + i) = inst_list[i].bits;
	}

	for (uint32_t i = 0; i < inst_list_size; i++)
	{
		cpu.registers[0] = 0;

		instruction inst = {
			.bits = *(uint32_t*)(cpu.memory + cpu.program_counter)
		};

		execute_inst(inst, &cpu);

		if (cpu.program_counter == 0)
			break;
	}

	destroy_riscv_cpu(&cpu);

	return 0;
}