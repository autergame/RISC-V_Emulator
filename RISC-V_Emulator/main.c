#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//#include "cpu.h"
//#include "inst_defs.h"

#include "tests_inst.h"

int main(int argc, char* *argv)
{
	riscv_cpu cpu = create_riscv_cpu();

	tests_inst(&cpu);

	destroy_riscv_cpu(&cpu);

	return 0;
}