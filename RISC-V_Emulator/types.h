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

// add sub sll slt sltu xor srl sra or and 
#define opcode_alu_register 0b0110011

// ecall ebreak csrrw csrrs csrrc csrrwi csrrsi csrrci
#define opcode_e_and_system 0b1110011

// jalr beq lb sb addi add sub ecall ebreak
#define funct3_000 0b000

// bne lh sh slli sll csrrw
#define funct3_001 0b001

// lw sw slti slt csrrs
#define funct3_010 0b010

// sltiu sltu csrrc
#define funct3_011 0b011

// blt lbu xori xor
#define funct3_100 0b100

// bge lhu srli srai srl sra csrrwi
#define funct3_101 0b101

// bltu ori or csrrsi
#define funct3_110 0b110

// bgeu andi and csrrci
#define funct3_111 0b111

// slli srli add sll slt sltu xor or and srl
#define funct7_0000000 0b0000000

// srai sub sra
#define funct7_0100000 0b0100000

// ecall
#define imm11_0_000000000000 0b000000000000

// ebreak
#define imm11_0_000000000001 0b000000000001