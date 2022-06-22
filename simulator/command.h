#include "instruction.h"


unsigned int load(char *path, unsigned int *PC);

int jump(unsigned int val, unsigned int *PC, const unsigned int *IC);

void go(unsigned int *PC, const unsigned int *BP, unsigned int *IC, char *path);

int step(unsigned int *PC, const unsigned int *IC, char *path);

int viewRegister(unsigned int start, unsigned int end);

int viewMemory(unsigned int start, unsigned int end);

void setRegister(unsigned int regNumber, int value);

void setMemory(unsigned int addr, int value);

void viewBreakPoint(unsigned int IC, const unsigned int *BP);

void setBreakPoint(int pos, unsigned int IC, unsigned int *BP);

void setBreakPointRegister(int pos, unsigned int *VR);

void viewBreakPointRegister(const unsigned int *VR);

unsigned int load(char *path, unsigned int *PC) {
    FILE *loadFile;
    errno_t err;
    unsigned int file_format[2];
    int data; //데이터를 저장할 변수

//    if ((err = fopen_s(&loadFile, path, "rb"))) {
//        printf("Cannot open %s\n", path);
//        return 1;
//    }

    if (!(loadFile = fopen(path, "rb"))) {
        printf("Cannot open %s\n", path);
        return 1;
    }

    *PC = 0x00400000;

    fread(&file_format, sizeof(unsigned int), 2, loadFile); // 명령어 개수와 데이터 개수 확인
    file_format[0] = invertEndian(file_format[0]);
    file_format[1] = invertEndian(file_format[1]);

    // instruction 저장
    for (int addr = 0; addr < (4 * file_format[0]); addr += 4) {
        fread(&data, sizeof(unsigned int), 1, loadFile);
        if (MEM((0x004 << 20) + addr, invertEndian(data), 1, 2) != 0) {
            printf("load error\n");
        }
    }

    // data 저장
    for (int addr = 0; addr < (4 * file_format[1]); addr += 4) {
        fread(&data, sizeof(unsigned int), 1, loadFile);
        if (MEM((0x100 << 20) + addr, invertEndian(data), 1, 2) != 0) {
            printf("load error\n");
        }
    }

    // 출력 확인
    unsigned int out;
    printf("Number of Instructions: %d\n", file_format[0]);
    for (int addr = 0; addr < (4 * file_format[0]); addr += 4) {
        out = MEM((0x004 << 20) + addr, 0, 0, 2);
        printf("%08x\n", out);
    }

    printf("Number of Data: %d\n", file_format[1]);
    for (int addr = 0; addr < (4 * file_format[1]); addr += 4) {
        out = MEM((0x100 << 20) + addr, 0, 0, 2);
        printf("%08x\n", out);
    }
    printf("\n");
    REG(29 << 2, 0x80000000, 1);
    REG(31 << 2, 0x00400000, 1);

    fclose(loadFile);
    return file_format[0];
}


int jump(unsigned int val, unsigned int *PC, const unsigned int *IC) {
    if (val > 0x00400000 + (*IC * 4) || val < 0x00400000) {
        printf("wrong value\n");
        return 1;
    }
    setPC(val, PC);
    return 0;
}

void go(unsigned int *PC, const unsigned int *BP, unsigned int *IC, char *path) {
    unsigned int num;
    while (1) {
        num = ((*PC & 0xFFFFF) >> 2);
        if (!step(PC, IC, path)) {
            break;
        } else if (BP[num]) {
            printf("Break Point\n");
            break;
        }
    }
}

int step(unsigned int *PC, const unsigned int *IC, char *path) {
    // instruction fetch
    IR ir;

    if (*PC > 0x00400000 + (*IC * 4) || *PC == 0) {
        printf("must load File\n");
        return 0;
    }

    ir.I = MEM(*PC, 0, 0, 2);
    *PC += 4;

    // instruction decode
    unsigned int Opt = ir.RI.opcode;
    unsigned int Fct = ir.RI.func;
    int imm = ir.Imm.imm;
    unsigned int Link =
            (((ir.RI.func) + (ir.RI.sh << 6) + (ir.RI.rd << 11) + (ir.RI.rt << 16) + (ir.RI.rs << 21)) << 2) +
            (*PC & 0xF0000000);

    printf("%s ", (Opt == 0) ? Rformat[Fct] : instruction[Opt]);
    // -------------------------------------------------------------------------------------------------

    if (Opt == 0) { // R-format
        if (Fct == 0) { // SLL
            SLL(ir.RI.rd, ir.RI.rt, ir.RI.sh);
        } else if (Fct == 2) { // SRL
            SRL(ir.RI.rd, ir.RI.rt, ir.RI.sh);
        } else if (Fct == 3) { // SRA
            SRA(ir.RI.rd, ir.RI.rt, ir.RI.sh);
        } else if (Fct == 8) { // jr
            jr(ir.RI.rs, PC);
        } else if (Fct == 12) { // syscall
            *PC = 0x00400000;
            FILE *loadFile;
            errno_t err;
            unsigned int file_format[2];
            int data; //데이터를 저장할 변수

            loadFile = fopen(path, "rb");
            *PC = 0x00400000;

            fread(&file_format, sizeof(unsigned int), 2, loadFile); // 명령어 개수와 데이터 개수 확인
            file_format[0] = invertEndian(file_format[0]);
            file_format[1] = invertEndian(file_format[1]);
            for (int addr = 0; addr < (4 * file_format[0]); addr += 4) {
                fread(&data, sizeof(unsigned int), 1, loadFile);
                if (MEM((0x004 << 20) + addr, invertEndian(data), 1, 2) != 0) {
                    printf("load error\n");
                }
            }

            // data 저장
            for (int addr = 0; addr < (4 * file_format[1]); addr += 4) {
                fread(&data, sizeof(unsigned int), 1, loadFile);
                if (MEM((0x100 << 20) + addr, invertEndian(data), 1, 2) != 0) {
                    printf("load error\n");
                }
            }
            fclose(loadFile);
            REG(29 << 2, 0x80000000, 1);
            REG(31 << 2, 0x00400000, 1);
            printf("\n");
            return 0;
        } else if (Fct == 16) { // mfhi
            mfhi(ir.RI.rd);
        } else if (Fct == 18) { // mflo
            mflo(ir.RI.rd);
        } else if (Fct == 24) { // mul
            mul(ir.RI.rs, ir.RI.rt);
        } else if (Fct == 32) { // add
            add(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else if (Fct == 34) { // sub
            sub(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else if (Fct == 36) { // and
            and(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else if (Fct == 37) { // or
            or(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else if (Fct == 38) { // xor
            xor(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else if (Fct == 39) { // nor
            nor(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else if (Fct == 42) { // slt
            slt(ir.RI.rd, ir.RI.rs, ir.RI.rt);
        } else {
            printf("\nUndefined Instruction\n");
            return 0;
        }
    } else if (Opt == 1) { // bltz
        bltz(ir.RI.rs, imm, PC);
    } else if (Opt == 2) { // j
        j(Link, PC);
    } else if (Opt == 3) { // jal
        jal(Link, PC);
    } else if (Opt == 4) { // beq
        beq(ir.RI.rs, ir.RI.rt, imm, PC);
    } else if (Opt == 5) { // bne
        bne(ir.RI.rs, ir.RI.rt, imm, PC);
    } else if (Opt == 8) { // addi
        addi(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 10) { // slti
        slti(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 12) { // andi
        andi(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 13) { // ori
        ori(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 14) { // xori
        xori(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 15) { // lui
        lui(ir.RI.rt, imm);
    } else if (Opt == 32) { // lb
        lb(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 35) { // lw
        lw(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 36) { // lbu
        lbu(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 40) { // sb
        sb(ir.RI.rt, ir.RI.rs, imm);
    } else if (Opt == 43) { // sw
        sw(ir.RI.rt, ir.RI.rs, imm);
    } else {
        printf("Opt %d, Fct %d", Opt, Fct);
        printf("\nUndefined Instruction\n");
        return 0;
    }

    printf("\n");
    // -------------------------------------------------------------------------------------------------

    return 1;
}

int viewRegister(unsigned int start, unsigned int end) {
    if (start < 0 || 31 < start || end < 0 || 31 < end || end < start) {
        printf("wrong register number\n");
        return 1;
    }
    printf("[REGISTER]\n");
    for (unsigned int num = start; num <= end; num++) {
        printf("$%2d: %08x\n", num, REG(num << 2, 0, 0));
    }
    return 0;
}

int viewMemory(unsigned int start, unsigned int end) {
    unsigned int s = start >> 20;
    unsigned int e = end >> 20;

    if (s == 0x004 && e == 0x004) printf("[Program Memory]\n");  //
    else if (s == 0x100 && e == 0x100) printf("[Data Memory]\n");  // data memory
    else if (s == 0x7FF && e == 0x7FF) printf("[Stack]\n");
    else {
        printf("wrong address\n");
        return 1;
    }

    for (unsigned int addr = start; addr <= end; addr += 4) {
        printf("0x%08x: 0x%08x\n", addr, MEM(addr, 0, 0, 2));
    }

    return 0;
}

void setRegister(unsigned int regNumber, int value) {
    if (REG(regNumber << 2, value, 1) == 1) {
        return;
    }
    int v = REG(regNumber << 2, 0, 0);
    printf("register number : %d\n", regNumber);
    printf("value : %08x (%d)\n", v, v);
}

void setMemory(unsigned int addr, int value) {
    //aligned 해야 함
    if (MEM(addr, value, 1, 2) == 1) {
        return;
    }
    int v = MEM(addr, 0, 0, 2);
    printf("address : %08x\n", addr);
    printf("value : %08x (%d)\n", v, v);
}

void viewBreakPoint(unsigned int IC, const unsigned int *BP) {
    int num, out;
    for (int addr = 0; addr < (4 * IC); addr += 4) {
        num = addr >> 2;
        out = MEM((0x004 << 20) + addr, 0, 0, 2);
        printf("%c %02d : %08x\n", (BP[num] ? 'B' : ' '), num, out);
    }
}

void setBreakPoint(int pos, unsigned int IC, unsigned int *BP) {
    if (IC <= pos || pos < 0) {
        printf("wrong position");
        return;
    }
    if (BP[pos])
        BP[pos] = 0;
    else
        BP[pos] = 1;
}

void setBreakPointRegister(int pos, unsigned int *VR) {
    if (32 <= pos || pos < 0) {
        printf("wrong position");
        return;
    }
    if (VR[pos])
        VR[pos] = 0;
    else
        VR[pos] = 1;
}

void viewBreakPointRegister(const unsigned int *VR) {
    printf("[REGISTER]\n");
    for (unsigned int num = 0; num <= 31; num++) {
        if (VR[num] != 0)
            printf("$%2d: %08x\n", num, REG(num << 2, 0, 0));
    }
    printf("\n");
}
