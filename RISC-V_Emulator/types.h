#pragma once

#define opcode_lui 0b0110111
#define opcode_auipc 0b0010111

#define opcode_jal 0b1101111
#define opcode_jalr 0b1100111

// beq bne blt bge bltu bgeu
#define opcode_branch 0b1100011

// lb lh lw lbu lhu
#define opcode_load 0b0000011

// sb sh sw
#define opcode_store 0b0100011

// addi slti sltiu xori ori andi slli srli srai
#define opcode_alu_and_shift_imm 0b0010011

// add sub sll sla slt sltu xor srl sra or and
#define opcode_alu_register 0b0110011

// ecall ebreak csrrw csrrs csrrc csrrwi csrrsi csrrci
#define opcode_e_and_system 0b1110011

// jalr beq lb sb addi add sub
#define funct3_0_000 0b000

// bne lh sh slli sll sla csrrw
#define funct3_1_001 0b001

// lw sw slti slt csrrs
#define funct3_2_010 0b010

// sltiu sltu csrrc
#define funct3_3_011 0b011

// blt lbu xori xor 
#define funct3_4_100 0b100

// bge lhu srli srai srl sra csrrwi
#define funct3_5_101 0b101

// bltu lwu ori or csrrsi
#define funct3_6_110 0b110

// bgeu andi and csrrci
#define funct3_7_111 0b111

// slli srli add sll slt sltu xor srl or and
#define funct7_0_0000000 0b0000000

// srai sub sra
#define funct7_32_0100000 0b0100000