#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

typedef instruction(*inst_funct_0_args)();
typedef instruction(*inst_funct_2_args)(uint32_t, uint32_t);
typedef instruction(*inst_funct_3_args)(uint32_t, uint32_t, uint32_t);

static const char* registers[] = {
	"zero",
	"ra",
	"sp", "gp", "tp", "fp",
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
	 1,
	 2,  3,  4,  8,
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
	size_t string_len = strlen(string);
	if (string_len == 0)
		return 0;

	int negative = 0;
	if (string[0] == '-')
	{
		string++;
		negative = 1;
		string_len -= 1;
	}

	uint32_t sum = 0;

	if (string_len >= 1)
	{
		if (string_len > 2 && (string[0] == '0' && (string[1] == 'x' || string[1] == 'X'))) // hex
		{
			sum = strtoul(string, NULL, 16);
		}
		else // decimal
		{
			sum = strtol(string, NULL, 10);
		}

		if (negative)
			sum = -((int32_t)sum);
	}

	return sum;
}

int check_valid_hex_or_decimal(const char* string)
{
	int string_len = strlen(string);
	if (string_len == 0)
		return 0;

	if (string[0] == '-')
	{
		string++;
		string_len -= 1;
	}

	int valid = 1;

	if (string_len >= 1)
	{
		if (string_len > 2 && (string[0] == '0' && (string[1] == 'x' || string[1] == 'X'))) // hex
		{
			for (int j = 2; j < string_len; j++)
			{
				if (!((string[j] >= '0' && string[j] <= '9')
					||(string[j] >= 'a' && string[j] <= 'f')
					||(string[j] >= 'A' && string[j] <= 'F')))
				{
					valid = 0;
					break;
				}
			}
		}
		else // decimal
		{
			for (int j = 0; j < string_len; j++)
			{
				if (!(string[j] >= '0' && string[j] <= '9'))
				{
					valid = 0;
					break;
				}
			}
		}
	}
	else
	{
		valid = 0;
	}

	return valid;
}

char* string_copy_limited(const char* source, const int string_len)
{
	char* destination = (char*)calloc(string_len + 1, sizeof(char));
	assert(memcpy(destination, source, string_len) == destination);
	destination[string_len] = '\0';
	return destination;
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
		int string_len = strcspn(insts + string_pointer, "\n");

		if (string_len != 0)
		{			
			int string_len_comment = strcspn(insts + string_pointer, "#\n");

			if (insts[string_pointer] != '#')
			{
				int string_len_ignore = string_len;

				if (string_len_comment != string_len)
					string_len_ignore = string_len_comment;

				inst_splitted[split_pointer] = string_copy_limited(insts + string_pointer, string_len_ignore);

				split_pointer++;
			}
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
		size_t string_pointer = 0;
		while (string_pointer < strlen(inst_splitted[i]))
		{
			int string_len = strcspn(inst_splitted[i] + string_pointer, " ,\t");

			if (string_len != 0)
			{
				tokens[token_pointer] = string_copy_limited(inst_splitted[i] + string_pointer, string_len);

				token_pointer += 1;
			}

			string_pointer += string_len + 1;
		}
	}

	tokens_count = token_pointer;

	int compiled_insts_pointer = 0;
	uint32_t* compiled_insts = (uint32_t*)calloc(newline_count, sizeof(uint32_t));

	typedef struct label_node 
	{
		char* key;
		int value;
		struct label_node* node;
	} label_node;

	int pointer_for_label = 0;
	label_node* linked_main = NULL;

	for (int i = 0; i < tokens_count; i++)
	{
		char* token = tokens[i];

		size_t string_len = strlen(token);
		if (token[string_len - 1] == ':')
		{
			label_node* new_linked = (label_node*)calloc(1, sizeof(label_node));

			new_linked->key = string_copy_limited(token, string_len - 1);
			new_linked->value = pointer_for_label * 4;
			new_linked->node = linked_main;

			linked_main = new_linked;
		}

		int opcode = string_is_in_list(keywords, keywords_size, token);
		if (opcode != -1)
			pointer_for_label += 1;
	}

	for (int i = 0; i < tokens_count; i++)
	{
		char* token_1 = tokens[i];
		int opcode = string_is_in_list(keywords, keywords_size, token_1);

		if (opcode != -1)
		{
			switch (opcode)
			{
				// U
				case 0: // lui
				case 1: // auipc
				{
					if (i + 2 > tokens_count)
					{
						printf("Opcode: %s need more 2 tokens: rd imm\n", keywords[opcode]);
						break;
					}

					char* token_2 = tokens[++i];
					int rd = string_is_in_list(registers, registers_size, token_2);

					if (rd == -1)
					{
						printf("Unknown register rd: %s\n", token_2);
						break;
					}
					if (i + 1 > tokens_count)
					{
						printf("Opcode: %s need more 1 token: imm\n", keywords[opcode]);
						break;
					}

					char* token_3 = tokens[++i];

					if (check_valid_hex_or_decimal(token_3) == 0)
					{
						printf("Invalid number / hex: %s\n", token_3);
						break;
					}

					uint32_t imm = hex_or_decimal_from_string(token_3);

					inst_funct_2_args inst_func = (inst_funct_2_args)opcode_functs[opcode];

					instruction inst = inst_func(registers_index[rd], imm);
					compiled_insts[compiled_insts_pointer] = inst.bits;

					compiled_insts_pointer += 1;

					break;
				}

				// J
				case 2: // jal
				{
					if (i + 2 > tokens_count)
					{
						printf("Opcode: %s need more 2 tokens: rd imm\n", keywords[opcode]);
						break;
					}

					char* token_2 = tokens[++i];
					int rd = string_is_in_list(registers, registers_size, token_2);

					if (rd == -1)
					{
						printf("Unknown register rd: %s\n", token_2);
						break;
					}
					if (i + 1 > tokens_count)
					{
						printf("Opcode: %s need more 1 token: imm\n", keywords[opcode]);
						break;
					}

					char* token_3 = tokens[++i];

					uint32_t imm = 0;

					label_node* current = linked_main;
					while (current != NULL)
					{
						if (strcmp(current->key, token_3) == 0)
						{
							imm = current->value;
							break;
						}
						current = current->node;
					}

					if (current == NULL)
					{
						if (check_valid_hex_or_decimal(token_3) == 0)
						{
							printf("Unknown label or invalid number / hex: %s\n", token_3);
							break;
						}

						imm = hex_or_decimal_from_string(token_3);
					}
					else
					{
						uint32_t insts_pointer = compiled_insts_pointer * 4;
						if (imm < insts_pointer)
							imm = -((int32_t)(insts_pointer - imm));
					}

					inst_funct_2_args inst_func = (inst_funct_2_args)opcode_functs[opcode];

					instruction inst = inst_func(registers_index[rd], imm);
					compiled_insts[compiled_insts_pointer] = inst.bits;

					compiled_insts_pointer += 1;

					break;
				}
				
				// R
				case 27: // add
				case 28: // sub
				case 29: // sll
				case 30: // slt
				case 31: // sltu
				case 32: // xor
				case 33: // srl
				case 34: // sra
				case 35: // or
				case 36: // and
				{
					if (i + 3 > tokens_count)
					{
						printf("Opcode: %s need more 3 tokens: rd rs1 rs2\n", keywords[opcode]);
						break;
					}

					char* token_2 = tokens[++i];
					int rd = string_is_in_list(registers, registers_size, token_2);

					if (rd == -1)
					{
						printf("Unknown register rd: %s\n", token_2);
						break;
					}
					if (i + 2 > tokens_count)
					{
						printf("Opcode: %s need more 2 tokens: rs1 rs2\n", keywords[opcode]);
						break;
					}

					char* token_3 = tokens[++i];
					int rs1 = string_is_in_list(registers, registers_size, token_3);

					if (rs1 == -1)
					{
						printf("Unknown register rs1: %s\n", token_3);
						break;
					}
					if (i + 1 > tokens_count)
					{
						printf("Opcode: %s need more 1 token: rs2\n", keywords[opcode]);
						break;
					}

					char* token_4 = tokens[++i];
					int rs2 = string_is_in_list(registers, registers_size, token_4);

					if (rs2 == -1)
					{
						printf("Unknown register rs2: %s\n", token_4);
						break;
					}

					inst_funct_3_args inst_func = (inst_funct_3_args)opcode_functs[opcode];

					instruction inst = inst_func(registers_index[rd],
						registers_index[rs1], registers_index[rs2]);
					compiled_insts[compiled_insts_pointer] = inst.bits;

					compiled_insts_pointer += 1;

					break;
				}

				// I
				case 3: // jalr
				case 10: // lb
				case 11: // lh
				case 12: // lw
				case 13: // lbu
				case 14: // lhu
				case 18: // addi
				case 19: // slti
				case 20: // sltiu
				case 21: // xori
				case 22: // ori
				case 23: // andi
				// Shift
				case 24: // slli
				case 25: // srli
				case 26: // srai
				{
					if (i + 3 > tokens_count)
					{
						printf("Opcode: %s need more 3 tokens: rd rs1 imm\n", keywords[opcode]);
						break;
					}

					char* token_2 = tokens[++i];
					int rd = string_is_in_list(registers, registers_size, token_2);

					if (rd == -1)
					{
						printf("Unknown register rd: %s\n", token_2);
						break;
					}
					if (i + 2 > tokens_count)
					{
						printf("Opcode: %s need more 2 tokens: rs1 imm\n", keywords[opcode]);
						break;
					}

					char* token_3 = tokens[++i];
					int rs1 = string_is_in_list(registers, registers_size, token_3);

					if (rs1 == -1)
					{
						printf("Unknown register rs1: %s\n", token_3);
						break;
					}
					if (i + 1 > tokens_count)
					{
						printf("Opcode: %s need more 1 token: imm\n", keywords[opcode]);
						break;
					}

					char* token_4 = tokens[++i];

					if (check_valid_hex_or_decimal(token_4) == 0)
					{
						printf("Invalid number / hex: %s\n", token_4);
						break;
					}

					uint32_t imm = hex_or_decimal_from_string(token_4);

					inst_funct_3_args inst_func = (inst_funct_3_args)opcode_functs[opcode];

					instruction inst = inst_func(registers_index[rd], registers_index[rs1], imm);
					compiled_insts[compiled_insts_pointer] = inst.bits;

					compiled_insts_pointer += 1;

					break;
				}

				// B
				case 4: // beq
				case 5: // bne
				case 6: // blt
				case 7: // bge
				case 8: // bltu
				case 9: // bgeu
				// S
				case 15: // sb
				case 16: // sh
				case 17: // sw
				{
					if (i + 3 > tokens_count)
					{
						printf("Opcode: %s need more 3 tokens: rs1 rs2 imm\n", keywords[opcode]);
						break;
					}

					char* token_2 = tokens[++i];
					int rs1 = string_is_in_list(registers, registers_size, token_2);

					if (rs1 == -1)
					{
						printf("Unknown register rs1: %s\n", token_2);
					}
					if (i + 2 > tokens_count)
					{
						printf("Opcode: %s need more 2 tokens: rs2 imm\n", keywords[opcode]);
					}

					char* token_3 = tokens[++i];
					int rs2 = string_is_in_list(registers, registers_size, token_3);

					if (rs2 == -1)
					{
						printf("Unknown register rs2: %s\n", token_3);
					}
					if (i + 1 > tokens_count)
					{
						printf("Opcode: %s need more 1 token: imm\n", keywords[opcode]);
					}

					char* token_4 = tokens[++i];

					if (check_valid_hex_or_decimal(token_4) == 0)
					{
						printf("Invalid number / hex: %s\n", token_4);
						break;
					}

					uint32_t imm = hex_or_decimal_from_string(token_4);

					inst_funct_3_args inst_func = (inst_funct_3_args)opcode_functs[opcode];

					instruction inst = inst_func(registers_index[rs1], registers_index[rs2], imm);
					compiled_insts[compiled_insts_pointer] = inst.bits;

					compiled_insts_pointer += 1;

					break;
				}
				
				// E
				case 37: // ecall
				case 38: // ebreak
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
		else
		{
			if (token_1[strlen(token_1) - 1] != ':')
			{
				printf("Skipping unknown opcode: %s\n", token_1);
			}
		}
	}

	label_node* current = linked_main;
	while (current != NULL)
	{
		label_node* tmp = current->node;
		free(current->key);
		free(current);
		current = tmp;
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