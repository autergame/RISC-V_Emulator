#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "cpu.h"
#include "inst_defs.h"

#include "assembler.h"

int main(const int argc, const char* argv[])
{
	const char insts[] =
		"jal zero, 28\n" //jump to main
			"bge a2, zero, 12\n" //abs
			"sub a0, zero, a2\n"
			"jalr zero, ra, 0\n"
			"addi a0, a2, 0\n"
			"jalr zero, ra, 0\n"
		"ebreak\n"
			"addi sp, sp, -16\n" //main
			"sw sp, ra, 12\n"
			"sw sp, a0, 8\n"
			"sw sp, a2, 4\n"
			"sw sp, fp, 0\n"
			"addi fp, sp, 16\n"
		"ebreak\n"
				"addi a2, zero, -5\n"
					"jal ra, -56\n" //jump to abs
				"addi a0, a0, 5\n"
		"ebreak\n"
			"lw ra, sp, 12\n"
			"lw a0, sp, 8\n"
			"lw a2, sp, 4\n"
			"lw fp, sp, 0\n"
			"addi sp, sp, 16\n"
		"ebreak\n"
			"addi sp, sp, -16\n"
			"sw sp, ra, 12\n"
			"sw sp, a0, 8\n"
			"sw sp, a2, 4\n"
			"sw sp, fp, 0\n"
			"addi fp, sp, 16\n"
		"ebreak\n"
				"addi a2, zero, 5\n"
					"jal ra, -124\n" //jump to abs
				"addi a0, a0, -5\n"
		"ebreak\n"
			"lw ra, sp, 12\n"
			"lw a0, sp, 8\n"
			"lw a2, sp, 4\n"
			"lw fp, sp, 0\n"
			"addi sp, sp, 16\n"
		"ebreak";

	int inst_size = array_size(insts);

	uint32_t compiled_insts_size = 0;
	uint32_t* compiled_insts = assemble(insts, inst_size, &compiled_insts_size);

	riscv_cpu cpu = create_riscv_cpu();

	reset_riscv_cpu(&cpu);

	memcpy(cpu.memory, compiled_insts, compiled_insts_size * sizeof(uint32_t));
	(*memory_uint32(&cpu, compiled_insts_size * 4)) = 0xDEADC0DE;

	run_riscv_cpu(&cpu, compiled_insts_size);

	destroy_riscv_cpu(&cpu);

	free(compiled_insts);

	return 0;
}