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

// addi srli srai slli slti sltiu xori ori andi
#define opcode_alu_and_shift_imm 0b0010011

// add sub srl sra sll slt sltu xor or and
#define opcode_alu_register 0b0110011

// ecall ebreak csrrw csrrs csrrc csrrwi csrrsi csrrci
#define opcode_e_and_system 0b1110011

// beq jalr lb sb add addi sub
#define funct3_000 0b000

// bne lh sh sll slli csrrw
#define funct3_001 0b001

// lw sw slt slti csrrs
#define funct3_010 0b010

// sltu sltiu csrrc
#define funct3_011 0b011

// blt lbu xor xori  
#define funct3_100 0b100

// bge lhu srl srli sra srai csrrwi
#define funct3_101 0b101

// bltu ori or csrrsi
#define funct3_110 0b110

// bgeu andi and csrrci
#define funct3_111 0b111

// sll slli srl srli slt sltu add xor or and
#define funct7_0000000 0b0000000

// sra srai sub 
#define funct7_0100000 0b0100000