#include <stdio.h>

unsigned int invertEndian(unsigned int inVal);

unsigned int checkAligned(unsigned int addr, unsigned char *MEM);

void setPC(unsigned int val, unsigned int *PC);

unsigned int invertEndian(unsigned int inVal) {
    return inVal >> 24 | inVal << 24 | (0x00FF0000 & inVal) >> 8 | (0x0000FF00 & inVal) << 8;
}

unsigned int checkAligned(unsigned int addr, unsigned char *MEM) {
    if (addr % 4 == 0)
        return addr;
    else {
        printf("Not aligned access %d\n", addr);
        // addr이 4의 배수가 되도록 주소를 truncate 시킨다: & 와 상수 사용
        return addr & 0xFFFFFFFC;
    }
}

void setPC(unsigned int val, unsigned int *PC) {
    *PC = val;
}


// 명령어 종류
char instruction[48][8] = {"R-format", "bltz", "j", "jal", "beq", "bne", "", "",
                           "addi", "", "slti", "", "andi", "ori", "xori", "lui",
                           "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
                           "lb", "", "", "lw", "lbu", "", "", "",
                           "sb", "", "", "sw", "", "", "", "",};
// R-format 명령어 종류
char Rformat[48][7] = {"sll", "", "srl", "sra", "", "", "", "",
                       "jr", "", "", "", "syscall", "", "", "",
                       "mfhi", "", "mflo", "", "", "", "", "",
                       "mul", "", "", "", "", "", "", "",
                       "add", "", "sub", "", "and", "or", "xor", "nor",
                       "", "", "slt", "", "", "", "", "",};


typedef union instructionRegister {
    unsigned int I;
    struct RFormat {
        unsigned int func: 6;
        unsigned int sh: 5;
        unsigned int rd: 5;
        unsigned int rt: 5;
        unsigned int rs: 5;
        unsigned int opcode: 6;
    } RI;
    struct Imm {
        int imm: 16;
        int func: 16;
    } Imm;
} IR;