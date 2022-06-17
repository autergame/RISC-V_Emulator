#pragma once

#include "imm_enc_dec.h"
#include "instructions.h"

#define inst_r(opcode_v, funct3_v, funct7_v, rd_v, rs1_v, rs2_v) \
{ \
	.R = { \
		.opcode = opcode_v, \
		.rd = rd_v, \
		.funct3 = funct3_v, \
		.rs1 = rs1_v, \
		.rs2 = rs2_v, \
		.funct7 = funct7_v \
	} \
}

#define inst_i(opcode_v, funct3_v, rd_v, rs1_v, imm_v) \
inst_i_imm_enc \
( \
	(instruction) \
	{ \
		.I = { \
			.opcode = opcode_v, \
			.rd = rd_v, \
			.funct3 = funct3_v, \
			.rs1 = rs1_v, \
		} \
	}, \
	imm_v \
)

#define inst_shift(opcode_v, funct3_v, funct7_v, rd_v, rs1_v, shamt_v) \
inst_i_shamt_imm_enc \
( \
	(instruction) \
	{ \
		.Shift = { \
			.opcode = opcode_v, \
			.rd = rd_v, \
			.funct3 = funct3_v, \
			.rs1 = rs1_v, \
			.funct7 = funct7_v, \
		} \
	}, \
	shamt_v \
)

#define inst_s(opcode_v, funct3_v, rs1_v, rs2_v, imm_v) \
inst_s_imm_enc \
( \
	(instruction) \
	{ \
		.S = { \
			.opcode = opcode_v, \
			.funct3 = funct3_v, \
			.rs1 = rs1_v, \
			.rs2 = rs2_v, \
		} \
	}, \
	imm_v \
)

#define inst_b(opcode_v, funct3_v, rs1_v, rs2_v, imm_v) \
inst_b_imm_enc \
( \
	(instruction) \
	{ \
		.B = { \
			.opcode = opcode_v, \
			.funct3 = funct3_v, \
			.rs1 = rs1_v, \
			.rs2 = rs2_v, \
		} \
	}, \
	imm_v \
)

#define inst_u(opcode_v, rd_v, imm_v) \
inst_u_imm_enc \
( \
	(instruction) \
	{ \
		.U = { \
			.opcode = opcode_v, \
			.rd = rd_v, \
		} \
	}, \
	imm_v \
)

#define inst_j(opcode_v, rd_v, imm_v) \
inst_j_imm_enc \
( \
	(instruction) \
	{ \
		.J = { \
			.opcode = opcode_v, \
			.rd = rd_v, \
		} \
	}, \
	imm_v \
)

#define inst_lui(v_rd, v_imm)   inst_u(opcode_lui, v_rd, v_imm)
#define inst_auipc(v_rd, v_imm) inst_u(opcode_auipc, v_rd, v_imm)

#define inst_jal(v_rd, v_imm)         inst_j(opcode_jal, v_rd, v_imm)
#define inst_jalr(v_rd, v_rs1, v_imm) inst_i(opcode_jalr, funct3_000, v_rd, v_rs1, v_imm)

#define inst_beq(v_rs1, v_rs2, v_imm)  inst_b(opcode_branch, funct3_000, v_rs1, v_rs2, v_imm)
#define inst_bne(v_rs1, v_rs2, v_imm)  inst_b(opcode_branch, funct3_001, v_rs1, v_rs2, v_imm)
#define inst_blt(v_rs1, v_rs2, v_imm)  inst_b(opcode_branch, funct3_100, v_rs1, v_rs2, v_imm)
#define inst_bge(v_rs1, v_rs2, v_imm)  inst_b(opcode_branch, funct3_101, v_rs1, v_rs2, v_imm)
#define inst_bltu(v_rs1, v_rs2, v_imm) inst_b(opcode_branch, funct3_110, v_rs1, v_rs2, v_imm)
#define inst_bgeu(v_rs1, v_rs2, v_imm) inst_b(opcode_branch, funct3_111, v_rs1, v_rs2, v_imm)

#define inst_lb(v_rd, v_rs1, v_imm)  inst_i(opcode_load, funct3_000, v_rd, v_rs1, v_imm)
#define inst_lh(v_rd, v_rs1, v_imm)  inst_i(opcode_load, funct3_001, v_rd, v_rs1, v_imm)
#define inst_lw(v_rd, v_rs1, v_imm)  inst_i(opcode_load, funct3_010, v_rd, v_rs1, v_imm)
#define inst_lbu(v_rd, v_rs1, v_imm) inst_i(opcode_load, funct3_100, v_rd, v_rs1, v_imm)
#define inst_lhu(v_rd, v_rs1, v_imm) inst_i(opcode_load, funct3_101, v_rd, v_rs1, v_imm)

#define inst_sb(v_rs1, v_rs2, v_imm) inst_s(opcode_store, funct3_000, v_rs1, v_rs2, v_imm)
#define inst_sh(v_rs1, v_rs2, v_imm) inst_s(opcode_store, funct3_001, v_rs1, v_rs2, v_imm)
#define inst_sw(v_rs1, v_rs2, v_imm) inst_s(opcode_store, funct3_010, v_rs1, v_rs2, v_imm)

#define inst_addi(v_rd, v_rs1, v_imm)   inst_i(opcode_alu_and_shift_imm, funct3_000, v_rd, v_rs1, v_imm)
#define inst_slti(v_rd, v_rs1, v_imm)   inst_i(opcode_alu_and_shift_imm, funct3_010, v_rd, v_rs1, v_imm)
#define inst_sltiu(v_rd, v_rs1, v_imm)  inst_i(opcode_alu_and_shift_imm, funct3_011, v_rd, v_rs1, v_imm)
#define inst_xori(v_rd, v_rs1, v_imm)   inst_i(opcode_alu_and_shift_imm, funct3_100, v_rd, v_rs1, v_imm)
#define inst_ori(v_rd, v_rs1, v_imm)    inst_i(opcode_alu_and_shift_imm, funct3_110, v_rd, v_rs1, v_imm)
#define inst_andi(v_rd, v_rs1, v_imm)   inst_i(opcode_alu_and_shift_imm, funct3_111, v_rd, v_rs1, v_imm)
#define inst_slli(v_rd, v_rs1, v_shamt) inst_shift(opcode_alu_and_shift_imm, funct3_001, funct7_0000000, v_rd, v_rs1, v_shamt)
#define inst_srli(v_rd, v_rs1, v_shamt) inst_shift(opcode_alu_and_shift_imm, funct3_101, funct7_0000000, v_rd, v_rs1, v_shamt)
#define inst_srai(v_rd, v_rs1, v_shamt) inst_shift(opcode_alu_and_shift_imm, funct3_101, funct7_0100000, v_rd, v_rs1, v_shamt)

#define inst_add(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_000, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_sub(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_000, funct7_0100000, v_rd, v_rs1, v_rs2)
#define inst_sll(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_001, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_slt(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_010, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_sltu(v_rd, v_rs1, v_rs2) inst_r(opcode_alu_register, funct3_011, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_xor(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_100, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_srl(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_101, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_sra(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_101, funct7_0100000, v_rd, v_rs1, v_rs2)
#define inst_or(v_rd, v_rs1, v_rs2)   inst_r(opcode_alu_register, funct3_110, funct7_0000000, v_rd, v_rs1, v_rs2)
#define inst_and(v_rd, v_rs1, v_rs2)  inst_r(opcode_alu_register, funct3_111, funct7_0000000, v_rd, v_rs1, v_rs2)

#define inst_ecall  inst_i(opcode_e_and_system, 0, 0, 0, 0)
#define inst_ebreak inst_i(opcode_e_and_system, 0, 0, 0, 1)

#define inst_csrrw(v_rd, v_rs1, v_imm)   inst_i(opcode_e_and_system, funct3_001, v_rd, v_rs1, v_imm)
#define inst_csrrs(v_rd, v_rs1, v_imm)   inst_i(opcode_e_and_system, funct3_010, v_rd, v_rs1, v_imm)
#define inst_csrrc(v_rd, v_rs1, v_imm)   inst_i(opcode_e_and_system, funct3_011, v_rd, v_rs1, v_imm)
#define inst_csrrwi(v_rd, v_zimm, v_imm) inst_i(opcode_e_and_system, funct3_101, v_rd, v_zimm, v_imm)
#define inst_csrrsi(v_rd, v_zimm, v_imm) inst_i(opcode_e_and_system, funct3_110, v_rd, v_zimm, v_imm)
#define inst_csrrci(v_rd, v_zimm, v_imm) inst_i(opcode_e_and_system, funct3_111, v_rd, v_zimm, v_imm)