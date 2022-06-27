#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define memory_size 0xfffff
#define stack_pointer 0xf8000

typedef struct riscv_cpu
{
	uint64_t* csrs;
	uint8_t* memory;
	uint32_t registers[32];
	uint32_t program_counter;
} riscv_cpu;

uint32_t* memory_uint32(riscv_cpu* cpu, uint32_t address)
{
	return ((uint32_t*)(cpu->memory + address));
}

uint16_t* memory_uint16(riscv_cpu* cpu, uint32_t address)
{
	return ((uint16_t*)(cpu->memory + address));
}

#include "execute.h"

riscv_cpu create_riscv_cpu()
{
	riscv_cpu cpu = {
		.registers = { 0 },
		.program_counter = 0,
		.csrs = (uint64_t*)calloc(4096, sizeof(uint64_t)),
		.memory = (uint8_t*)calloc(memory_size, sizeof(uint8_t)),
	};
	return cpu;
}

void run_riscv_cpu(riscv_cpu* cpu, uint32_t inst_list_size)
{
	while (1)
	{
		cpu->registers[0] = 0;

		instruction inst = {
			.bits = *memory_uint32(cpu, cpu->program_counter)
		};

		if (inst.bits == 0xDEADC0DE)
			break;

		execute_inst(inst, cpu);
	}
}

void destroy_riscv_cpu(riscv_cpu* cpu)
{
	free(cpu->csrs);
	free(cpu->memory);
}

void reset_riscv_cpu(riscv_cpu* cpu)
{
	cpu->program_counter = 0;
	for (uint32_t i = 0; i < 4096; i++)
	{
		cpu->csrs[i] = 0;
	}
	for (uint32_t i = 0; i < 32; i++)
	{
		cpu->registers[i] = 0;
	}
	cpu->registers[2] = stack_pointer;
	for (uint32_t i = 0; i < memory_size; i++)
	{
		cpu->memory[i] = 0;
	}
}

void load_from_instructions(riscv_cpu* cpu, instruction inst_list[], uint32_t inst_list_size)
{
	reset_riscv_cpu(cpu);
	for (uint32_t i = 0; i < inst_list_size; i++)
	{
		*memory_uint32(cpu, i * 4) = inst_list[i].bits;
	}
	*memory_uint32(cpu, inst_list_size * 4) = 0xDEADC0DE;
}

void load_and_run(riscv_cpu* cpu, instruction inst_list[], uint32_t inst_list_size)
{
	load_from_instructions(cpu, inst_list, inst_list_size);

	run_riscv_cpu(cpu, inst_list_size);
}