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



// hard-wired zero
#define r_zero 0b00000

// return address
#define r_ra 0b00001

// stack pointer
#define r_sp 0b00010

// global pointer
#define r_gp 0b00011

// thread pointer
#define r_tp 0b00100

// temporaries
#define r_t0 0b00101
#define r_t1 0b00110
#define r_t2 0b00111

// saved register / frame pointer
#define r_s0 0b01000
#define r_fp 0b01000

// saved register
#define r_s1 0b01001

// function arguments / return values
#define r_a0 0b01010
#define r_a1 0b01011

// function arguments
#define r_a2 0b01100
#define r_a3 0b01101
#define r_a4 0b01110
#define r_a5 0b01111
#define r_a6 0b10000
#define r_a7 0b10001

// saved registers
#define r_s2 0b10010
#define r_s3 0b10011
#define r_s4 0b10100
#define r_s5 0b10101
#define r_s6 0b10110
#define r_s7 0b10111
#define r_s8 0b11000
#define r_s9 0b11001
#define r_s10 0b11010
#define r_s11 0b11011

// temporaries
#define r_t3 0b11100
#define r_t4 0b11101
#define r_t5 0b11110
#define r_t6 0b11111