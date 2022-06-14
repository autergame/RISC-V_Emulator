#pragma once

#include <stdlib.h>
#include <inttypes.h>

typedef struct riscv_cpu
{
	uint8_t* memory;
	uint32_t registers[32];
	uint32_t program_counter;
} riscv_cpu;

riscv_cpu create_riscv_cpu()
{
	riscv_cpu cpu = {
		.registers = { 0 },
		.program_counter = 0,
		.memory = (uint8_t*)calloc(1024 * 1024 * 4, sizeof(uint8_t)),
	};
	return cpu;
}

void destroy_riscv_cpu(riscv_cpu* cpu)
{
	free(cpu->memory);
}