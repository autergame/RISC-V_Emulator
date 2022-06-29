#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inst_defs.h"

#define array_size(v_array) (sizeof(v_array) / sizeof(v_array[0]))

static const char* keywords[] = {
	"lui", "auipc",
	"jal", "jalr",
	"beq", "bne", "blt", "bge", "bltu", "bgeu",
	"lb", "lh", "lw", "lbu", "lhu", "sb", "sh", "sw",
	"addi", "slti", "sltiu", "xori", "ori", "andi", "slli", "srli", "srai",
	"add", "sub", "sll", "slt", "sltu", "xor","srl", "sra", "or", "and",
	"ecall", "ebreak",
	"csrrw", "csrrs", "csrrc", "csrrwi", "csrrsi", "csrrci"
};
static const int keywords_size = array_size(keywords);

typedef enum inst_types {
	inst_R, inst_I, inst_Shift, inst_S, inst_B, inst_U, inst_J, inst_E
} inst_types;

static const inst_types opcode_type[] = {
	inst_U, inst_U,
	inst_J, inst_I,
	inst_B, inst_B, inst_B, inst_B, inst_B, inst_B,
	inst_I, inst_I, inst_I, inst_I, inst_I, inst_S, inst_S, inst_S,
	inst_I, inst_I, inst_I, inst_I, inst_I, inst_I, inst_Shift, inst_Shift, inst_Shift,
	inst_R, inst_R, inst_R, inst_R, inst_R, inst_R, inst_R, inst_R, inst_R, inst_R,
	inst_E, inst_E,
	inst_I, inst_I ,inst_I, inst_I, inst_I, inst_I
};

typedef instruction(*inst_funct_0_args)();
typedef instruction(*inst_funct_2_args)(uint32_t, uint32_t);
typedef instruction(*inst_funct_3_args)(uint32_t, uint32_t, uint32_t);

static const void* opcode_functs[] = {
	&inst_lui, &inst_auipc,
	&inst_jal, &inst_jalr,
	&inst_beq, &inst_bne, &inst_blt, &inst_bge, &inst_bltu, &inst_bgeu,
	&inst_lb, &inst_lh, &inst_lw, &inst_lbu, &inst_lhu, &inst_sb, &inst_sh, &inst_sw,
	&inst_addi, &inst_slti, &inst_sltiu, &inst_xori, &inst_ori, &inst_andi, &inst_slli, &inst_srli, &inst_srai,
	&inst_add, &inst_sub, &inst_sll, &inst_slt, &inst_sltu, &inst_xor, &inst_srl, &inst_sra, &inst_or, &inst_and,
	&inst_ecall, &inst_ebreak,
	&inst_csrrw, &inst_csrrs, &inst_csrrc, &inst_csrrwi, &inst_csrrsi, &inst_csrrci
};

static const char* registers[] = {
	"zero",
	"ra", "sp", "gp", "tp", "fp",
	"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
	"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
	"t0", "t1", "t2", "t3", "t4", "t5", "t6",

	 "x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",  "x8",  "x9",
	"x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19",
	"x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29",
	"x30", "x31"
};
static const int registers_size = array_size(registers);

static const int registers_index[] = {
	 0,
	 1,  2,  3,  4,  8,
	10, 11, 12, 13, 14, 15, 16, 17,
	 8,  9, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
	 5,  6,  7, 28, 29, 30, 31,

	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31
};

int string_is_in_list(const char* list[], const int list_size, const char* string)
{
	for (int i = 0; i < list_size; i++)
	{
		if (strcmp(string, list[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

uint32_t hex_or_decimal_from_string(const char* string)
{
	uint32_t sum = 0;
	size_t string_len = strlen(string);

	if (string_len >= 2 && (string[0] == '0' && (string[1] == 'x' || string[1] == 'X'))) // hex
	{
		sum = strtoul(string, NULL, 16);
	}
	else // decimal
	{
		sum = strtol(string, NULL, 10);
	}

	return sum;
}

void string_copy_limited(char** destination, const char* source, const int string_len)
{
	*destination = (char*)calloc(string_len + 1, sizeof(char));
	memcpy(*destination, source, string_len);
	(*destination)[string_len] = '\0';
}

uint32_t* assemble(const char* insts, const int inst_size, int* compiled_insts_size)
{
	int newline_count = 1;
	for (int i = 0; i < inst_size - 1; i++)
	{
		if (insts[i] == '\n' && insts[i + 1] != '\n' && insts[i + 1] != '#')
		{
			newline_count += 1;
		}
	}

	int split_pointer = 0;
	int string_pointer = 0;
	char** inst_splitted = (char**)calloc(newline_count, sizeof(char*));

	while (string_pointer < inst_size)
	{
		int string_len = strcspn(&insts[string_pointer], "\n");

		if (string_len != 0)
		{
			int string_len_old = string_len;
			int string_len_comment = strcspn(&insts[string_pointer], "#\n");

			if (insts[string_pointer] != '#')
			{
				if (string_len_comment != string_len)
					string_len = string_len_comment;

				string_copy_limited(&inst_splitted[split_pointer], &insts[string_pointer], string_len);

				split_pointer++;
			}

			string_len = string_len_old;
		}

		string_pointer += string_len + 1;

		while (insts[string_pointer] == '\n')
			string_pointer++;
	}

	newline_count = split_pointer;

	int tokens_count = newline_count;
	for (int i = 0; i < newline_count; i++)
	{
		for (size_t j = 0; j < strlen(inst_splitted[i]) - 1; j++)
		{
			if (inst_splitted[i][j] == ' ' && inst_splitted[i][j + 1] != ' ')
			{
				tokens_count += 1;
			}
		}
	}

	int token_pointer = 0;
	char** tokens = (char**)calloc(tokens_count, sizeof(char*));

	for (int i = 0; i < newline_count; i++)
	{
		for (size_t string_pointer = 0; string_pointer < strlen(inst_splitted[i]);)
		{
			int string_len = strcspn(&inst_splitted[i][string_pointer], " ,");

			if (string_len != 0)
			{
				string_copy_limited(&tokens[token_pointer], &inst_splitted[i][string_pointer], string_len);

				token_pointer += 1;
			}

			string_pointer += string_len + 1;
		}
	}

	int compiled_insts_pointer = 0;
	uint32_t* compiled_insts = (uint32_t*)calloc(newline_count, sizeof(uint32_t));

	for (int i = 0; i < tokens_count; i++)
	{
		char* token_1 = tokens[i];
		int opcode = string_is_in_list(keywords, keywords_size, token_1);

		switch (opcode_type[opcode])
		{
			case inst_U:
			case inst_J:
			{
				char* token_2 = tokens[++i];
				int rd = string_is_in_list(registers, registers_size, token_2);

				if (rd != -1)
				{
					char* token_3 = tokens[++i];
					uint32_t imm = hex_or_decimal_from_string(token_3);

					inst_funct_2_args inst_func = (inst_funct_2_args)opcode_functs[opcode];

					instruction inst = inst_func(registers_index[rd], imm);
					compiled_insts[compiled_insts_pointer] = inst.bits;

					compiled_insts_pointer += 1;
				}

				break;
			}

			case inst_R:
			{
				char* token_2 = tokens[++i];
				int rd = string_is_in_list(registers, registers_size, token_2);

				if (rd != -1)
				{
					char* token_3 = tokens[++i];
					int rs1 = string_is_in_list(registers, registers_size, token_3);

					if (rs1 != -1)
					{
						char* token_4 = tokens[++i];
						int rs2 = string_is_in_list(registers, registers_size, token_4);

						if (rs2)
						{
							inst_funct_3_args inst_func = (inst_funct_3_args)opcode_functs[opcode];

							instruction inst = inst_func(registers_index[rd],
								registers_index[rs1], registers_index[rs2]);
							compiled_insts[compiled_insts_pointer] = inst.bits;

							compiled_insts_pointer += 1;
						}
					}
				}

				break;
			}

			case inst_I:
			case inst_Shift:
			{
				char* token_2 = tokens[++i];
				int rd = string_is_in_list(registers, registers_size, token_2);

				if (rd != -1)
				{
					char* token_3 = tokens[++i];
					int rs1 = string_is_in_list(registers, registers_size, token_3);

					if (rs1 != -1)
					{
						char* token_4 = tokens[++i];
						uint32_t imm = hex_or_decimal_from_string(token_4);

						inst_funct_3_args inst_func = (inst_funct_3_args)opcode_functs[opcode];

						instruction inst = inst_func(registers_index[rd], registers_index[rs1], imm);
						compiled_insts[compiled_insts_pointer] = inst.bits;

						compiled_insts_pointer += 1;
					}
				}

				break;
			}

			case inst_S:
			case inst_B:
			{
				char* token_2 = tokens[++i];
				int rs1 = string_is_in_list(registers, registers_size, token_2);

				if (rs1 != -1)
				{
					char* token_3 = tokens[++i];
					int rs2 = string_is_in_list(registers, registers_size, token_3);

					if (rs2 != -1)
					{
						char* token_4 = tokens[++i];
						uint32_t imm = hex_or_decimal_from_string(token_4);

						inst_funct_3_args inst_func = (inst_funct_3_args)opcode_functs[opcode];

						instruction inst = inst_func(registers_index[rs1], registers_index[rs2], imm);
						compiled_insts[compiled_insts_pointer] = inst.bits;

						compiled_insts_pointer += 1;
					}
				}

				break;
			}

			case inst_E:
			{
				inst_funct_0_args inst_func = (inst_funct_0_args)opcode_functs[opcode];

				instruction inst = inst_func();
				compiled_insts[compiled_insts_pointer] = inst.bits;

				compiled_insts_pointer += 1;

				break;
			}

			default:
				break;
		}
	}

	for (int i = 0; i < tokens_count; i++)
	{
		free(tokens[i]);
	}
	free(tokens);

	for (int i = 0; i < newline_count; i++)
	{
		free(inst_splitted[i]);
	}
	free(inst_splitted);

	*compiled_insts_size = compiled_insts_pointer;

	return compiled_insts;
}