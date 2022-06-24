#pragma once

#include <assert.h>

#include "cpu.h"
#include "inst_defs.h"

void test_lui(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_lui(r_t1, 0x12345),
		inst_add(r_t2, r_t0, r_t1)
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x12345000);
	assert(cpu->registers[r_t2] == 0x12345123);
}
void test_auipc(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_auipc(r_t1, 0x12345),    // 0x12345004
		inst_add(r_t2, r_t0, r_t1)    // 0x12345127
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x12345004);
	assert(cpu->registers[r_t2] == 0x12345127);
}

void test_jal(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_addi(r_t1, r_t0, 291),   // 0x123
		inst_jal(r_t2, 8),            // 0x00c

		inst_add(r_t2, r_t0, r_t1),   // 0x369

		inst_add(r_t3, r_t2, r_t1),   // 0x375
		inst_addi(r_t3, r_t3, 873),   // 0x6de
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x369);
	assert(cpu->registers[r_t2] == 0x00c);
	assert(cpu->registers[r_t3] == 0x6de);
}
void test_jalr(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_addi(r_t1, r_t0, 291),   // 0x123
		inst_addi(r_t2, r_zero, 12),  // 0x00c
		inst_jalr(r_t3, r_t2, 8),     // 0x010

		inst_add(r_t3, r_t0, r_t1),   // 0x369

		inst_add(r_t4, r_t3, r_t1),   // 0x379
		inst_addi(r_t4, r_t4, 873),   // 0x6e2
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x369);
	assert(cpu->registers[r_t2] == 0x00c);
	assert(cpu->registers[r_t3] == 0x010);
	assert(cpu->registers[r_t4] == 0x6e2);
}

#pragma region opcode_branch

void test_beq(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_addi(r_t1, r_t1, 291),   // 0x123

		inst_beq(r_t0, r_t1, -4),     // 1 == 2

		inst_add(r_t2, r_t0, r_t1)    // 0x369
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x369);
}

void test_bne(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_addi(r_t1, r_t1, 291),   // 0x123

		inst_bne(r_t0, r_t1, -4),     // 1 != 2

		inst_add(r_t2, r_t0, r_t1)    // 0x48c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x48c);
}

void test_blt(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_addi(r_t1, r_t1, 291),   // 0x123

		inst_blt(r_t1, r_t0, -4),     // 2 < 1

		inst_add(r_t2, r_t0, r_t1)    // 0x48c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x48c);
}

void test_bge(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291),   // 0x123
		inst_addi(r_t1, r_zero, -1164), // 0xfffffb74
		inst_addi(r_t1, r_t1, 582),     // 0x246

		inst_bge(r_t0, r_t1, -4),       // 1 >= 2
		
		inst_add(r_t2, r_t0, r_t1)      // 0x369
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x369);
}

void test_bltu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 1606), // 0x646
		inst_addi(r_t1, r_t1, 1315),   // 0x523

		inst_bltu(r_t1, r_t0, -4),     // 2 < 1

		inst_add(r_t2, r_t0, r_t1)     // 0x108c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x646);
	assert(cpu->registers[r_t1] == 0xa46);
	assert(cpu->registers[r_t2] == 0x108c);
}

void test_bgeu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 1606), // 0x646
		inst_addi(r_t1, r_t1, 1315),   // 0x523

		inst_bgeu(r_t0, r_t1, -4),     // 1 >= 2

		inst_add(r_t2, r_t0, r_t1)     // 0x108c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x646);
	assert(cpu->registers[r_t1] == 0xa46);
	assert(cpu->registers[r_t2] == 0x108c);
}

#pragma endregion

#pragma region opcode_load opcode_store

void test_lbsb(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 12),   // 0x00c
		inst_addi(r_t1, r_zero, -127), // 0xffffff81

		inst_sb(r_t0, r_t1, 8),        // 0x014
		inst_lb(r_t2, r_t0, 8),        // 0x008
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x00c);
	assert(cpu->registers[r_t1] == 0xffffff81);
	assert(cpu->registers[r_t2] == 0xffffff81);
}

void test_lhsh(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 12), // 0x00c
		//inst_"li"(r_t1, -32767),   // 0xffff8001
		inst_lui(r_t1, 0x00008),     // 0x8000
		inst_addi(r_t1, r_t1, 1),    // 0x8001

		inst_sh(r_t0, r_t1, 12),     // 0x018
		inst_lh(r_t2, r_t0, 12),     // 0x00c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x00c);
	assert(cpu->registers[r_t1] == 0x8001);
	assert(cpu->registers[r_t2] == 0xffff8001);
}

void test_lwsw(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 12),    // 0x00c
		//inst_"li"(r_t1, -2147483647), // 0x80000001
		inst_lui(r_t1, 0x80000),        // 0x80000000
		inst_addi(r_t1, r_t1, 1),       // 0x80000001

		inst_sw(r_t0, r_t1, 12),        // 0x018
		inst_lw(r_t2, r_t0, 12),        // 0x00c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x00c);
	assert(cpu->registers[r_t1] == 0x80000001);
	assert(cpu->registers[r_t2] == 0x80000001);
}

void test_lbu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 12),  // 0x00c
		inst_addi(r_t1, r_zero, 255), // 0xff

		inst_sb(r_t0, r_t1, 8),       // 0x014
		inst_lbu(r_t2, r_t0, 8),      // 0x008
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x00c);
	assert(cpu->registers[r_t1] == 0xff);
	assert(cpu->registers[r_t2] == 0xff);
}

void test_lhu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 12), // 0x00c
		//inst_"li"(r_t1, 65535),    // 0xffff
		inst_lui(r_t1, 0x00010),     // 0x10000
		inst_addi(r_t1, r_t1, -1),   // 0xfff

		inst_sh(r_t0, r_t1, 12),     // 0x018
		inst_lhu(r_t2, r_t0, 12),    // 0x00c
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x00c);
	assert(cpu->registers[r_t1] == 0xffff);
	assert(cpu->registers[r_t2] == 0xffff);
}

#pragma endregion

#pragma region opcode_alu_and_shift_imm

void test_addi(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_addi(r_t1, r_t0, 582),   // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x369);
}

void test_slti(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, -127), // 0xffffff81
		inst_slti(r_t1, r_t0, 291)     // 0x123
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0xffffff81);
	assert(cpu->registers[r_t1] == 0x1);
}

void test_sltiu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 2456), // 0x998
		inst_sltiu(r_t1, r_t0, 3456),  // 0xd80
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0xfffff998);
	assert(cpu->registers[r_t1] == 0x1);
}

void test_xori(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_xori(r_t1, r_t0, 582),   // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x365);
}

void test_ori(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_ori(r_t1, r_t0, 582),    // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x367);
}

void test_andi(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_andi(r_t1, r_t0, 582),   // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x002);
}

void test_slli(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_slli(r_t1, r_t0, 16)     // 0x2460000
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x2460000);
}

void test_srli(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_lui(r_t0, 0x2460),    // 0x2460000
		inst_srli(r_t1, r_t0, 16)  // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x2460000);
	assert(cpu->registers[r_t1] == 0x246);
}

void test_srai(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		//inst_"li"(r_t0, -2147483647), // 0x80000001
		inst_lui(r_t0, 0x80000),        // 0x80000000
		inst_addi(r_t0, r_t0, 1),       // 0x80000001
		inst_srai(r_t1, r_t0, 16)       // 0xffff8000
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x80000001);
	assert(cpu->registers[r_t1] == 0xffff8000);
}

#pragma endregion

#pragma region opcode_alu_register

void test_add(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_addi(r_t1, r_zero, 582), // 0x246
		inst_add(r_t2, r_t0, r_t1)    // 0x369
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x369);
}

void test_sub(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_addi(r_t1, r_zero, 291), // 0x123
		inst_sub(r_t2, r_t0, r_t1)    // 0x123
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x123);
	assert(cpu->registers[r_t2] == 0x123);
}

void test_sll(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 582), // 0x246
		inst_addi(r_t1, r_zero, 16),  // 0x010
		inst_sll(r_t2, r_t0, r_t1)    // 0x2460000
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x246);
	assert(cpu->registers[r_t1] == 0x010);
	assert(cpu->registers[r_t2] == 0x2460000);
}

void test_slt(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, -127), // 0xffffff81
		inst_addi(r_t1, r_zero, 291),  // 0x123
		inst_slt(r_t2, r_t0, r_t1)    
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0xffffff81);
	assert(cpu->registers[r_t1] == 0x123);
	assert(cpu->registers[r_t2] == 0x1);
}

void test_sltu(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 2456), // 0x998
		inst_addi(r_t1, r_zero, 3456), // 0xd80
		inst_sltu(r_t2, r_t0, r_t1)
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0xfffff998);
	assert(cpu->registers[r_t1] == 0xfffffd80);
	assert(cpu->registers[r_t2] == 0x1);
}

void test_xor(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_addi(r_t1, r_zero, 582), // 0x246
		inst_xor(r_t2, r_t0, r_t1)    // 0x365
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x365);
}

void test_srl(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_lui(r_t0, 0x2460),      // 0x2460000
		inst_addi(r_t1, r_zero, 16), // 0x010
		inst_srl(r_t2, r_t0, r_t1)   // 0x246
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x2460000);
	assert(cpu->registers[r_t1] == 0x010);
	assert(cpu->registers[r_t2] == 0x246);
}

void test_sra(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		//inst_"li"(r_t0, -2147483647), // 0x80000001
		inst_lui(r_t0, 0x80000),        // 0x80000000
		inst_addi(r_t0, r_t0, 1),       // 0x80000001
		inst_addi(r_t1, r_zero, 16),    // 0x010
		inst_sra(r_t2, r_t0, r_t1)      // 0xffff8000
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x80000001);
	assert(cpu->registers[r_t1] == 0x010);
	assert(cpu->registers[r_t2] == 0xffff8000);
}

void test_or(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_addi(r_t1, r_zero, 582), // 0x246
		inst_or(r_t2, r_t0, r_t1)     // 0x367
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x367);
}

void test_and(riscv_cpu* cpu)
{
	instruction inst_list[] =
	{
		inst_addi(r_t0, r_zero, 291), // 0x123
		inst_addi(r_t1, r_zero, 582), // 0x246
		inst_and(r_t2, r_t0, r_t1)    // 0x002
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(cpu, inst_list, inst_list_size);

	assert(cpu->registers[r_t0] == 0x123);
	assert(cpu->registers[r_t1] == 0x246);
	assert(cpu->registers[r_t2] == 0x002);
}

#pragma endregion

void tests_inst(riscv_cpu* cpu)
{
	test_lui(cpu);
	test_auipc(cpu);

	test_jal(cpu);
	test_jalr(cpu);

	test_beq(cpu);
	test_bne(cpu);
	test_blt(cpu);
	test_bge(cpu);
	test_bltu(cpu);
	test_bgeu(cpu);

	test_lbsb(cpu);
	test_lhsh(cpu);
	test_lwsw(cpu);
	test_lbu(cpu);
	test_lhu(cpu);

	test_addi(cpu);
	test_slti(cpu);
	test_sltiu(cpu);
	test_xori(cpu);
	test_ori(cpu);
	test_andi(cpu);
	test_slli(cpu);
	test_srli(cpu);
	test_srai(cpu);

	test_add(cpu);
	test_sub(cpu);
	test_sll(cpu);
	test_slt(cpu);
	test_sltu(cpu);
	test_xor(cpu);
	test_srl(cpu);
	test_sra(cpu);
	test_or(cpu);
	test_and(cpu);
}