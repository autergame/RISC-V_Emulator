#pragma once

#include "types.h"
#include "imm_enc_dec.h"

void execute_opcode_branch(const instruction inst, riscv_cpu* cpu)
{
	switch (inst.B.funct3)
	{
		case funct3_000: // beq
		{
			if (cpu->registers[inst.B.rs1] == cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_001: // bne
		{
			if (cpu->registers[inst.B.rs1] != cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_100: // blt
		{
			if ((int32_t)cpu->registers[inst.B.rs1] < (int32_t)cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_101: // bge
		{
			if ((int32_t)cpu->registers[inst.B.rs1] >= (int32_t)cpu->registers[inst.B.rs2])
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_110: // bltu
		{
			uint32_t rs1 = cpu->registers[inst.B.rs1] & 0b00000000000000000000111111111111;
			uint32_t rs2 = cpu->registers[inst.B.rs2] & 0b00000000000000000000111111111111;
			if (rs1 < rs2)
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_111: // bgeu
		{
			uint32_t rs1 = cpu->registers[inst.B.rs1] & 0b00000000000000000000111111111111;
			uint32_t rs2 = cpu->registers[inst.B.rs2] & 0b00000000000000000000111111111111;
			if (rs1 >= rs2)
				cpu->program_counter = (int32_t)cpu->program_counter + inst_b_imm(inst);
			else
				cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_load(const instruction inst, riscv_cpu* cpu)
{
	switch (inst.I.funct3)
	{
		case funct3_000: // lb
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			uint8_t value = cpu->memory[address];
			cpu->registers[inst.I.rd] = sign_extend((uint32_t)value, 8);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_001: // lh
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			uint16_t value = *memory_uint16(cpu, address);
			cpu->registers[inst.I.rd] = sign_extend((uint32_t)value, 16);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_010: // lw
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->registers[inst.I.rd] = *memory_uint32(cpu, address);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_100: // lbu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->registers[inst.I.rd] = (uint32_t)cpu->memory[address];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_101: // lhu
		{
			uint32_t address = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->registers[inst.I.rd] = *memory_uint16(cpu, address);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_store(const instruction inst, riscv_cpu* cpu)
{
	switch (inst.S.funct3)
	{
		case funct3_000: // sb
		{
			uint32_t address = cpu->registers[inst.S.rs1] + inst_s_imm(inst);
			cpu->memory[address] = (uint8_t)cpu->registers[inst.S.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_001: // sh
		{
			uint32_t address = cpu->registers[inst.S.rs1] + inst_s_imm(inst);
			*memory_uint16(cpu, address) = (uint16_t)cpu->registers[inst.S.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_010: // sw
		{
			uint32_t address = cpu->registers[inst.S.rs1] + inst_s_imm(inst);
			*memory_uint32(cpu, address) = cpu->registers[inst.S.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_alu_and_shift_imm(const instruction inst, riscv_cpu* cpu)
{
	switch (inst.I.funct3)
	{
		case funct3_000: // addi
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_001: // slli
		{
			cpu->registers[inst.Shift.rd] = cpu->registers[inst.Shift.rs1] << inst.Shift.shamt_4_0;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_010: // slti
		{
			cpu->registers[inst.I.rd] = (int32_t)cpu->registers[inst.I.rs1] < inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_011: // sltiu
		{
			uint32_t rs1 = cpu->registers[inst.I.rs1] & 0b00000000000000000000111111111111;
			cpu->registers[inst.I.rd] = rs1 < inst.I.imm11_0;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_100: // xori
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] ^ inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_101: // srli srai
		{
			switch (inst.Shift.funct7)
			{
				case funct7_0000000: // srli
				{
					cpu->registers[inst.Shift.rd] = cpu->registers[inst.Shift.rs1] >> inst.Shift.shamt_4_0;
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_0100000: // srai
				{
					cpu->registers[inst.Shift.rd] = (int32_t)cpu->registers[inst.Shift.rs1] >> inst.Shift.shamt_4_0;
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
			break;
		}


		case funct3_110: // ori
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] | inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_111: // andi
		{
			cpu->registers[inst.I.rd] = cpu->registers[inst.I.rs1] & inst_i_imm(inst);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_alu_register(const instruction inst, riscv_cpu* cpu)
{
	switch (inst.R.funct3)
	{
		case funct3_000: // add sub
		{
			switch (inst.R.funct7)
			{
				case funct7_0000000: // add
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] + cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_0100000: // sub
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

		case funct3_001: // sll
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] << cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_010: // slt
		{
			cpu->registers[inst.R.rd] = (int32_t)cpu->registers[inst.R.rs1] < (int32_t)cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_011: // sltu
		{
			uint32_t rs1 = cpu->registers[inst.R.rs1] & 0b00000000000000000000111111111111;
			uint32_t rs2 = cpu->registers[inst.R.rs2] & 0b00000000000000000000111111111111;
			cpu->registers[inst.R.rd] = rs1 < rs2;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_100: // xor
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] ^ cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_101: // srl sra
		{
			switch (inst.R.funct7)
			{
				case funct7_0000000: // srl
				{
					cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] >> cpu->registers[inst.R.rs2];
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case funct7_0100000: // sra
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

		case funct3_110: // or
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] | cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_111: // and
		{
			cpu->registers[inst.R.rd] = cpu->registers[inst.R.rs1] & cpu->registers[inst.R.rs2];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_opcode_e_and_system(const instruction inst, riscv_cpu* cpu)
{
	switch (inst.I.funct3)
	{
		case funct3_000: // ecall ebreak
		{
			switch (inst.I.imm11_0)
			{
				case imm11_0_000000000000: // ecall
				{
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				case imm11_0_000000000001: // ebreak
				{
					#ifdef DEBUG
						__debugbreak();
					#endif			
					dump_registers(cpu);
					cpu->program_counter = cpu->program_counter + 4;
					break;
				}

				default:
					break;
			}
			break;
		}

		case funct3_001: // csrrw
		{
			cpu->registers[inst.I.rd] = (uint32_t)cpu->csrs[inst.I.imm11_0];
			cpu->csrs[inst.I.imm11_0] = cpu->registers[inst.I.rs1];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_010: // csrrs
		{
			cpu->csrs[inst.I.imm11_0] = cpu->csrs[inst.I.imm11_0] | cpu->registers[inst.I.rs1];
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_011: // csrrc
		{
			cpu->csrs[inst.I.imm11_0] = cpu->csrs[inst.I.imm11_0] & (!cpu->registers[inst.I.rs1]);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_101: // csrrwi
		{
			cpu->csrs[inst.I.imm11_0] = inst.I.rs1;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_110: // csrrsi
		{
			cpu->csrs[inst.I.imm11_0] = cpu->csrs[inst.I.imm11_0] | inst.I.rs1;
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		case funct3_111: // csrrci
		{
			cpu->csrs[inst.I.imm11_0] = cpu->csrs[inst.I.imm11_0] & (!inst.I.rs1);
			cpu->program_counter = cpu->program_counter + 4;
			break;
		}

		default:
			break;
	}
}

void execute_inst(const instruction inst, riscv_cpu* cpu)
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
			uint32_t oldpc = cpu->program_counter + 4;
			int32_t pc = (int32_t)cpu->registers[inst.I.rs1] + inst_i_imm(inst);
			cpu->program_counter = pc & 0b11111111111111111111111111111110;
			cpu->registers[inst.I.rd] = oldpc;
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
			execute_opcode_e_and_system(inst, cpu);
			break;
		}

		default:
			break;
	}
}