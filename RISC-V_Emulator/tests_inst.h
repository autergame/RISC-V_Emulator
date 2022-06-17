#pragma once

#include <assert.h>

#include "cpu.h"
#include "inst_defs.h"

void load_and_run(riscv_cpu* cpu, instruction inst_list[], uint32_t inst_list_size)
{
	load_from_instructions(cpu, inst_list, inst_list_size);

	run_riscv_cpu(cpu, inst_list_size);
}

void test_add(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291), // 0x123
		inst_addi(2, 0, 582), // 0x246
		inst_add(3, 1, 2)     // 0x369
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x369);
}

void test_addi(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291), // 0x123
		inst_addi(2, 1, 582), // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[2] == 0x369);
}

void test_and(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291), // 0x123
		inst_addi(2, 0, 582), // 0x246
		inst_and(3, 1, 2)     // 0x002
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x002);
}

void test_andi(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291), // 0x123
		inst_andi(2, 1, 582), // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[2] == 0x002);
}

void test_auipc(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291),      // 0x123
		inst_auipc(2, 0x12345000), // 0x12345004
		inst_add(3, 1, 2)          // 0x12345127
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x12345127);
}

void test_beq(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291), // 0x123
		inst_addi(2, 2, 291), // 0x123

		inst_beq(1, 2, -4),   // 1 == 4

		inst_add(3, 1, 2)     // 0x369
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x369);
}

void test_bge(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 291),   // 0x123
		inst_addi(2, 0, -1164), // 0xb74
		inst_addi(2, 2, 582),   // 0x246

		inst_bge(1, 2, -4),     // 1 >= 2

		inst_add(3, 1, 2)       // 0x369
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x369);
}

void test_bgeu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 1606), // 0x646
		inst_addi(2, 2, 1315), // 0x523

		inst_bgeu(1, 2, -4),  // 1 >= 2

		inst_add(3, 1, 2)     // 0x108c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x108c);
}

void test_blt(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 582), // 0x246
		inst_addi(2, 2, 291), // 0x123

		inst_blt(2, 1, -4),   // 2 < 1

		inst_add(3, 1, 2)     // 0x48c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x48c);
}

void test_bltu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 1606), // 0x646
		inst_addi(2, 2, 1315), // 0x523

		inst_bltu(2, 1, -4),   // 2 < 1

		inst_add(3, 1, 2)     // 0x108c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x108c);
}

void test_bne(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 582), // 0x246
		inst_addi(2, 2, 291), // 0x123

		inst_bne(1, 2, -4),   // 1 != 2

		inst_add(3, 1, 2)     // 0x48c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x48c);
}

void test_jal(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 582), // 0x246
		inst_addi(2, 1, 291), // 0x123
		inst_jal(3, 8),       // 0x00c

		inst_add(3, 1, 2),    // 0x369

		inst_add(4, 3, 2),    // 0x375
		inst_addi(4, 4, 873), // 0x6de
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[4] == 0x6de);
}

void test_jalr(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 582), // 0x246
		inst_addi(2, 1, 291), // 0x123
		inst_addi(3, 0, 12),  // 0x00c
		inst_jalr(4, 3, 8),   // 0x014

		inst_add(4, 1, 2),    // 0x369

		inst_add(5, 4, 2),    // 0x379
		inst_addi(5, 5, 873), // 0x6e2
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[5] == 0x6e2);
}









void test_sub(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(1, 0, 582), // 0x246
		inst_addi(2, 0, 291), // 0x123
		inst_sub(3, 1, 2)     // 0x123
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[3] == 0x123);
}

void tests_inst(riscv_cpu* cpu)
{
	test_add(cpu);
	test_addi(cpu);
	test_and(cpu);
	test_andi(cpu);
	test_auipc(cpu);
	test_beq(cpu);
	test_bge(cpu);
	test_bgeu(cpu);
	test_blt(cpu);
	test_bltu(cpu);
	test_bne(cpu);
	test_jal(cpu);
	test_jalr(cpu);
}