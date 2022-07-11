#pragma once

#include "types.h"
#include "imm_enc_dec.h"
#include "instructions.h"

#define inst_r(opcode_v, funct3_v, funct7_v, rd_v, rs1_v, rs2_v) \
(instruction) \
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
			.rs1 = rs1_v \
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
			.funct7 = funct7_v \
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
			.rs2 = rs2_v \
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
			.rs2 = rs2_v \
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
			.rd = rd_v \
		} \
	}, \
	imm_v << 12 \
)

#define inst_j(opcode_v, rd_v, imm_v) \
inst_j_imm_enc \
( \
	(instruction) \
	{ \
		.J = { \
			.opcode = opcode_v, \
			.rd = rd_v \
		} \
	}, \
	imm_v \
)

instruction inst_lui  (uint32_t v_rd, uint32_t v_imm) { return inst_u(opcode_lui, v_rd, v_imm); }
instruction inst_auipc(uint32_t v_rd, uint32_t v_imm) { return inst_u(opcode_auipc, v_rd, v_imm); }

instruction inst_jal (uint32_t v_rd, uint32_t v_imm) { return inst_j(opcode_jal, v_rd, v_imm); }
instruction inst_jalr(uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm) { return inst_i(opcode_jalr, funct3_000, v_rd, v_rs1, v_imm); }

instruction inst_beq (uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_b(opcode_branch, funct3_000, v_rs1, v_rs2, v_imm); }
instruction inst_bne (uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_b(opcode_branch, funct3_001, v_rs1, v_rs2, v_imm); }
instruction inst_blt (uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_b(opcode_branch, funct3_100, v_rs1, v_rs2, v_imm); }
instruction inst_bge (uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_b(opcode_branch, funct3_101, v_rs1, v_rs2, v_imm); }
instruction inst_bltu(uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_b(opcode_branch, funct3_110, v_rs1, v_rs2, v_imm); }
instruction inst_bgeu(uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_b(opcode_branch, funct3_111, v_rs1, v_rs2, v_imm); }

instruction inst_lb (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm) { return inst_i(opcode_load, funct3_000, v_rd, v_rs1, v_imm); }
instruction inst_lh (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm) { return inst_i(opcode_load, funct3_001, v_rd, v_rs1, v_imm); }
instruction inst_lw (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm) { return inst_i(opcode_load, funct3_010, v_rd, v_rs1, v_imm); }
instruction inst_lbu(uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm) { return inst_i(opcode_load, funct3_100, v_rd, v_rs1, v_imm); }
instruction inst_lhu(uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm) { return inst_i(opcode_load, funct3_101, v_rd, v_rs1, v_imm); }

instruction inst_sb(uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_s(opcode_store, funct3_000, v_rs1, v_rs2, v_imm); }
instruction inst_sh(uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_s(opcode_store, funct3_001, v_rs1, v_rs2, v_imm); }
instruction inst_sw(uint32_t v_rs1, uint32_t v_rs2, uint32_t v_imm) { return inst_s(opcode_store, funct3_010, v_rs1, v_rs2, v_imm); }

instruction inst_addi (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)   { return inst_i(opcode_alu_and_shift_imm, funct3_000, v_rd, v_rs1, v_imm); }
instruction inst_slti (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)   { return inst_i(opcode_alu_and_shift_imm, funct3_010, v_rd, v_rs1, v_imm); }
instruction inst_sltiu(uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)   { return inst_i(opcode_alu_and_shift_imm, funct3_011, v_rd, v_rs1, v_imm); }
instruction inst_xori (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)   { return inst_i(opcode_alu_and_shift_imm, funct3_100, v_rd, v_rs1, v_imm); }
instruction inst_ori  (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)   { return inst_i(opcode_alu_and_shift_imm, funct3_110, v_rd, v_rs1, v_imm); }
instruction inst_andi (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)   { return inst_i(opcode_alu_and_shift_imm, funct3_111, v_rd, v_rs1, v_imm); }
instruction inst_slli (uint32_t v_rd, uint32_t v_rs1, uint32_t v_shamt) { return inst_shift(opcode_alu_and_shift_imm, funct3_001, funct7_0000000, v_rd, v_rs1, v_shamt); }
instruction inst_srli (uint32_t v_rd, uint32_t v_rs1, uint32_t v_shamt) { return inst_shift(opcode_alu_and_shift_imm, funct3_101, funct7_0000000, v_rd, v_rs1, v_shamt); }
instruction inst_srai (uint32_t v_rd, uint32_t v_rs1, uint32_t v_shamt) { return inst_shift(opcode_alu_and_shift_imm, funct3_101, funct7_0100000, v_rd, v_rs1, v_shamt); }

instruction inst_add (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_000, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_sub (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_000, funct7_0100000, v_rd, v_rs1, v_rs2); }
instruction inst_sll (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_001, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_slt (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_010, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_sltu(uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_011, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_xor (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_100, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_srl (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_101, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_sra (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_101, funct7_0100000, v_rd, v_rs1, v_rs2); }
instruction inst_or  (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_110, funct7_0000000, v_rd, v_rs1, v_rs2); }
instruction inst_and (uint32_t v_rd, uint32_t v_rs1, uint32_t v_rs2) { return inst_r(opcode_alu_register, funct3_111, funct7_0000000, v_rd, v_rs1, v_rs2); }

instruction inst_ecall()  { return inst_i(opcode_e_and_system, 0, 0, 0, 0); }
instruction inst_ebreak() { return inst_i(opcode_e_and_system, 0, 0, 0, 1); }

instruction inst_csrrw (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)  { return inst_i(opcode_e_and_system, funct3_001, v_rd, v_rs1, v_imm); }
instruction inst_csrrs (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)  { return inst_i(opcode_e_and_system, funct3_010, v_rd, v_rs1, v_imm); }
instruction inst_csrrc (uint32_t v_rd, uint32_t v_rs1, uint32_t v_imm)  { return inst_i(opcode_e_and_system, funct3_011, v_rd, v_rs1, v_imm); }
instruction inst_csrrwi(uint32_t v_rd, uint32_t v_zimm, uint32_t v_imm) { return inst_i(opcode_e_and_system, funct3_101, v_rd, v_zimm, v_imm); }
instruction inst_csrrsi(uint32_t v_rd, uint32_t v_zimm, uint32_t v_imm) { return inst_i(opcode_e_and_system, funct3_110, v_rd, v_zimm, v_imm); }
instruction inst_csrrci(uint32_t v_rd, uint32_t v_zimm, uint32_t v_imm) { return inst_i(opcode_e_and_system, funct3_111, v_rd, v_zimm, v_imm); }