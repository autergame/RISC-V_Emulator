#pragma once

#include <inttypes.h>

#include "cpu.h"
#include "types.h"
#include "instructions.h"
#include "imm_enc_dec.h"

void execute_opcode_branch(instruction inst, riscv_cpu* cpu)
{
	switch (inst.SB.funct3)
	{
		case funct3_0_000: // beq
		{
			if (cpu->registers[inst.SB.rs1] == cpu->registers[inst.SB.rs2])
			{
				cpu->program_counter = (int32_t)cpu->program_counter + b_imm(inst.bits);
			}
			else
			{
				cpu->program_counter = cpu->program_counter + 4;
			}
			break;
		}

		case funct3_1_001: // bne
		{
			if (cpu->registers[inst.SB.rs1] != cpu->registers[inst.SB.rs2])
			{
				cpu->program_counter = (int32_t)cpu->program_counter + b_imm(inst.bits);
			}
			else
			{
				cpu->program_counter = cpu->program_counter + 4;
			}
			break;
		}

		case funct3_4_100: // blt
		{
			if ((int32_t)cpu->registers[inst.SB.rs1] < (int32_t)cpu->registers[inst.SB.rs2])
			{
				cpu->program_counter = (int32_t)cpu->program_counter + b_imm(inst.bits);
			}
			else
			{
				cpu->program_counter = cpu->program_counter + 4;
			}
			break;
		}

		case funct3_5_101: // bge
		{
			if ((int32_t)cpu->registers[inst.SB.rs1] >= (int32_t)cpu->registers[inst.SB.rs2])
			{
				cpu->program_counter = (int32_t)cpu->program_counter + b_imm(inst.bits);
			}
			else
			{
				cpu->program_counter = cpu->program_counter + 4;
			}
			break;
		}

		case funct3_6_110: // bltu
		{
			if (cpu->registers[inst.SB.rs1] < cpu->registers[inst.SB.rs2])
			{
				cpu->program_counter = (int32_t)cpu->program_counter + b_imm(inst.bits);
			}
			else
			{
				cpu->program_counter = cpu->program_counter + 4;
			}
			break;
		}

		case funct3_7_111: // bgeu
		{
			if (cpu->registers[inst.SB.rs1] >= cpu->registers[inst.SB.rs2])
			{
				cpu->program_counter = (int32_t)cpu->program_counter + b_imm(inst.bits);
			}
			else
			{
				cpu->program_counter = cpu->program_counter + 4;
			}
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
			uint32_t address = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			cpu->registers[inst.I.rd] = sign_extend((uint32_t)cpu->memory[address], 8);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // lh
		{
			uint32_t address = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			uint32_t memory =
				((uint32_t)cpu->memory[address + 1] << 8) |
				(uint32_t)cpu->memory[address];
			cpu->registers[inst.I.rd] = sign_extend(memory, 16);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_2_010: // lw
		{
			uint32_t address = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			uint32_t memory =
				((uint32_t)cpu->memory[address + 3] << 24) |
				((uint32_t)cpu->memory[address + 2] << 16) |
				((uint32_t)cpu->memory[address + 1] << 8) |
				(uint32_t)cpu->memory[address];
			cpu->registers[inst.I.rd] = sign_extend(memory, 32);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_4_100: // lbu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			cpu->registers[inst.I.rd] = (uint32_t)cpu->memory[address];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_5_101: // lhu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			uint32_t memory =
				((uint32_t)cpu->memory[address + 1] << 8) |
				(uint32_t)cpu->memory[address];
			cpu->registers[inst.I.rd] = memory;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_6_110: // lwu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			uint32_t memory =
				((uint32_t)cpu->memory[address + 3] << 24) |
				((uint32_t)cpu->memory[address + 2] << 16) |
				((uint32_t)cpu->memory[address + 1] << 8) |
				(uint32_t)cpu->memory[address];
			cpu->registers[inst.I.rd] = memory;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_store(instruction inst, riscv_cpu* cpu)
{
	switch (inst.SB.funct3)
	{
		case funct3_0_000: // sb
		{
			uint32_t address = cpu->registers[inst.SB.rs1] + s_imm(inst.bits);
			cpu->memory[address] = cpu->registers[inst.SB.rs2] & 0b00000000000000000000000011111111;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // sh
		{
			uint32_t address = cpu->registers[inst.SB.rs1] + s_imm(inst.bits);
			uint32_t value = cpu->registers[inst.SB.rs2];
			cpu->memory[address] = value & 0b00000000000000000000000011111111;
			cpu->memory[address + 1] = (value >> 8) & 0b00000000000000000000000011111111;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_2_010: // sw
		{
			uint32_t address = cpu->registers[inst.SB.rs1] + s_imm(inst.bits);
			uint32_t value = cpu->registers[inst.SB.rs2];
			cpu->memory[address] = value & 0b00000000000000000000000011111111;
			cpu->memory[address + 1] = value & 0b00000000000000001111111100000000;
			cpu->memory[address + 2] = value & 0b00000000111111110000000000000000;
			cpu->memory[address + 3] = value & 0b11111111000000000000000000000000;
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
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_1_001: // slli slai
		{
			switch (inst.R.funct7)
			{
				case funct7_0_0000000: // slli
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] << imm_shamt_i(inst.bits);
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_32_0100000: // slai
				{
					cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] << imm_shamt_i(inst.bits);
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
			break;
		}

		case funct3_2_010: // slti
		{
			cpu->registers[inst.I.rd] = (int32_t)cpu->registers[inst.I.rs1] < i_imm(inst.bits);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_3_011: // sltiu
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] < (uint32_t)i_imm(inst.bits);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_4_100: // xori
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] ^ i_imm(inst.bits);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_5_101: // srli srai
		{
			switch (inst.R.funct7)
			{
				case funct7_0_0000000: // srli
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] >> imm_shamt_i(inst.bits);
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_32_0100000: // srai
				{
					cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] >> imm_shamt_i(inst.bits);
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
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] | i_imm(inst.bits);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_7_111: // andi
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] & i_imm(inst.bits);
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

		case funct3_1_001: // sll sla
		{
			switch (inst.R.funct7) // sll
			{
				case funct7_0_0000000:
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] << cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_32_0100000: // sla
				{
					cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] << (int32_t)cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
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
					cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] >> (int32_t)cpu->registers[inst.R.rs2];
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
			cpu->registers[inst.UJ.rd] = u_imm(inst.bits) << 12;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case opcode_auipc:
		{
			cpu->registers[inst.UJ.rd] = cpu->program_counter + (u_imm(inst.bits) << 12);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case opcode_jal:
		{
			cpu->registers[inst.UJ.rd] = cpu->program_counter + 4;
			cpu->program_counter = (int32_t)cpu->program_counter + j_imm(inst.bits);
			break;
		}

		case opcode_jalr:
		{
			cpu->registers[inst.I.rd] = cpu->program_counter + 4;
			int32_t pc = (int32_t)cpu->registers[inst.I.rs1] + i_imm(inst.bits);
			cpu->program_counter = (int32_t)(pc & 0b11111111111111111111111111111110);
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