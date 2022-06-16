#pragma once

#include <inttypes.h>

#include "cpu.h"
#include "types.h"
#include "instructions.h"
#include "imm_enc_dec.h"

void execute_opcode_branch(instruction inst, riscv_cpu* cpu)
{
	switch (inst.B.funct3)
	{
		case funct3_0_000: // beq
		{
			if (cpu->registers[inst.B.rs1] == cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // bne
		{
			if (cpu->registers[inst.B.rs1] != cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_4_100: // blt
		{
			if ((int32_t)cpu->registers[inst.B.rs1] < (int32_t)cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_5_101: // bge
		{
			if ((int32_t)cpu->registers[inst.B.rs1] >= (int32_t)cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_6_110: // bltu
		{
			if (cpu->registers[inst.B.rs1] < cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_7_111: // bgeu
		{
			if (cpu->registers[inst.B.rs1] >= cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_load(instruction inst, riscv_cpu* cpu)
{
	switch (inst.I.funct3)
	{
		case funct3_0_000: // lb
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			uint8_t value = cpu->memory[address];
			cpu->registers[inst.I.rd] = sign_extend((uint32_t)value, 8);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // lh
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			//uint16_t value = ((uint16_t*)cpu->memory)[address];
			uint16_t value = *memory_uint16(cpu, address);
			cpu->registers[inst.I.rd] = sign_extend((uint32_t)value, 16);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_2_010: // lw
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			//uint32_t value = ((uint32_t*)cpu->memory)[address];
			cpu->registers[inst.I.rd] = *memory_uint32(cpu, address);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_4_100: // lbu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->registers[inst.I.rd] = (uint32_t)cpu->memory[address];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_5_101: // lhu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			//uint16_t value = ((uint16_t*)cpu->memory)[address];
			cpu->registers[inst.I.rd] = *memory_uint16(cpu, address);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_store(instruction inst, riscv_cpu* cpu)
{
	switch (inst.S.funct3)
	{
		case funct3_0_000: // sb
		{
			uint32_t address = cpu->registers[inst.S.rs1] + inst_s_imm(inst);
			cpu->memory[address] = (uint8_t)cpu->registers[inst.S.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // sh
		{
			uint32_t address = cpu->registers[inst.S.rs1] + inst_s_imm(inst);
			//((uint16_t*)cpu->memory)[address] = (uint16_t)cpu->registers[inst.S.rs2];
			*memory_uint16(cpu, address) = (uint16_t)cpu->registers[inst.S.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_2_010: // sw
		{
			uint32_t address = cpu->registers[inst.S.rs1] + inst_s_imm(inst);
			//((uint32_t*)cpu->memory)[address] = cpu->registers[inst.S.rs2];
			*memory_uint32(cpu, address) = cpu->registers[inst.S.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_alu_and_shift_imm(instruction inst, riscv_cpu* cpu)
{
	switch (inst.I.funct3)
	{
		case funct3_0_000: // addi
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // slli
		{
			cpu->registers[inst.shift.rd] = cpu->registers[inst.shift.rs1] << inst.shift.shamt_4_0;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_2_010: // slti
		{
			cpu->registers[inst.I.rd] = (int32_t)cpu->registers[inst.I.rs1] < inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_3_011: // sltiu
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] < (uint32_t)inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_4_100: // xori
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] ^ inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_5_101: // srli srai
		{
			switch (inst.shift.func7)
			{
				case funct7_0_0000000: // srli
				{
					cpu->registers[inst.shift.rd] = cpu->registers[inst.shift.rs1] >> inst.shift.shamt_4_0;
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_32_0100000: // srai
				{
					cpu->registers[inst.shift.rd] = (int32_t)cpu->registers[inst.shift.rs1] >> inst.shift.shamt_4_0;
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
			break;
		}


		case funct3_6_110: // ori
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] | inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_7_111: // andi
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] & inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_alu_register(instruction inst, riscv_cpu* cpu)
{
	switch (inst.R.funct3)
	{
		case funct3_0_000: // add sub
		{
			switch (inst.R.funct7)
			{
				case funct7_0_0000000: // add
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] + cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_32_0100000: // sub
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] - cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
			break;
		}

		case funct3_1_001: // sll
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] << cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_2_010: // slt
		{
			cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] < (int32_t)cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_3_011: // sltu
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] < cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_4_100: // xor
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] ^ cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_5_101: // srl sra
		{
			switch (inst.R.funct7)
			{
				case funct7_0_0000000: // srl
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] >> cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_32_0100000: // sra
				{
					cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] >> cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
			break;
		}

		case funct3_6_110: // or
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] | cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_7_111: // and
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] & cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_inst(instruction inst, riscv_cpu* cpu)
{
	switch (inst.opcode)
	{
		case opcode_lui:
		{
			cpu->registers[inst.U.rd] = inst_u_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case opcode_auipc:
		{
			cpu->registers[inst.U.rd] = cpu->program_counter + inst_u_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case opcode_jal:
		{
			cpu->registers[inst.J.rd] = cpu->program_counter + 4;
			cpu->program_counter = (int32_t)cpu->program_counter + inst_j_imm(inst);
			break;
		}

		case opcode_jalr:
		{
			cpu->registers[inst.I.rd] = cpu->program_counter + 4;
			int32_t pc = (int32_t)cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->program_counter = (int32_t)(pc & (~1)); // 1111_1111_1111_1111_1111_1111_1111_1110
			break;
		}

		case opcode_branch:
		{
			execute_opcode_branch(inst, cpu);
			break;
		}

		case opcode_load:
		{
			execute_opcode_load(inst, cpu);
			break;
		}

		case opcode_store:
		{
			execute_opcode_store(inst, cpu);
			break;
		}

		case opcode_alu_and_shift_imm:
		{
			execute_opcode_alu_and_shift_imm(inst, cpu);
			break;
		}

		case opcode_alu_register:
		{
			execute_opcode_alu_register(inst, cpu);
			break;
		}

		case opcode_e_and_system:
		{
			break;
		}

		default:
			break;
	}
}