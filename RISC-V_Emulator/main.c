#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//#include "cpu.h"
//#include "inst_defs.h"

#include "tests_inst.h"

int main(int argc, char* *argv)
{
	riscv_cpu cpu = create_riscv_cpu();

	//tests_inst(&cpu);

	instruction inst_list[] =
	{
		inst_jal(r_zero, ((24 - __LINE__) * 4)), //jump to main
			inst_bge(r_a2, r_zero, 12), //abs
			inst_sub(r_a0, r_zero, r_a2), 
			inst_jalr(r_zero, r_ra, 0), 
			inst_addi(r_a0, r_a2, 0), 
			inst_jalr(r_zero, r_ra, 0), 
		inst_ebreak,
			inst_addi(r_sp, r_sp, -12), //main
			inst_sw(r_sp, r_ra, 8),
			inst_sw(r_sp, r_a0, 4),
			inst_sw(r_sp, r_a2, 0),
				inst_addi(r_a2, r_zero, -5),
				inst_jal(r_ra, -((__LINE__ - 19) * 4)), //jump to abs
				inst_addi(r_a0, r_a0, 5), 
			inst_lw(r_ra, r_sp, 8),
			inst_lw(r_a0, r_sp, 4),
			inst_lw(r_a2, r_sp, 0),
			inst_addi(r_sp, r_sp, 12),
		inst_ebreak,
			inst_addi(r_sp, r_sp, -12),
			inst_sw(r_sp, r_ra, 8),
			inst_sw(r_sp, r_a0, 4),
			inst_sw(r_sp, r_a2, 0),
				inst_addi(r_a2, r_zero, 5),
				inst_jal(r_ra, -((__LINE__ - 19) * 4)), //jump to abs
				inst_addi(r_a0, r_a0, -5),
			inst_lw(r_ra, r_sp, 8),
			inst_lw(r_a0, r_sp, 4),
			inst_lw(r_a2, r_sp, 0),
			inst_addi(r_sp, r_sp, 12),
		inst_ebreak
	};

	uint32_t inst_list_size = sizeof(inst_list) / sizeof(inst_list[0]);

	load_and_run(&cpu, inst_list, inst_list_size);

	destroy_riscv_cpu(&cpu);

	return 0;
}