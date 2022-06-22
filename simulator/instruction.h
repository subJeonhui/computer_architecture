#include "MEM.h"
#include "REG.h"
#include "ALU.h"
#include "util.h"

int zero = 0;
int res;
int rsValue;
int rtValue;

unsigned char HI[4];
unsigned char LO[4];

void SLL(int rd, int rt, int sh);

void SRL(int rd, int rt, int sh);

void SRA(int rd, int rt, int sh);

void jr(int rs, unsigned int *PC);

void mfhi(int rd);

void mflo(int rd);

void mul(int rs, int rt);

void add(int rd, int rs, int rt);

void sub(int rd, int rs, int rt);

void or(int rd, int rs, int rt);

void xor(int rd, int rs, int rt);

void nor(int rd, int rs, int rt);

void slt(int rd, int rs, int rt);

void bltz(int rs, int imm, unsigned int *PC);


void SLL(int rd, int rt, int sh) {
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(1, rtValue, sh, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rt, sh);
    printf("$%d <- 0x%08x ($%d(%08x) << 0x%08x)\n", rd, res, rt, rtValue, sh);
}

void SRL(int rd, int rt, int sh) {
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(2, rtValue, sh, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rt, sh);
    printf("$%d <- 0x%08x ($%d(%08x) >> 0x%08x)\n", rd, res, rt, rtValue, sh);
}

void SRA(int rd, int rt, int sh) {
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(3, rtValue, sh, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rt, sh);
    printf("$%d <- 0x%08x ($%d(%08x) >> 0x%08x)\n", rd, res, rt, rtValue, sh);
}

void jr(int rs, unsigned int *PC) {
    rsValue = REG(rs << 2, 0, 0);
    setPC(rsValue, PC);
    printf("$%d\n", rs);
    printf("PC <- $%d(0x%08x)\n", rs, rsValue);
}

void mfhi(int rd) {
    int HiValue = HI[0] << 24 | HI[1] << 16 | HI[2] << 8 | HI[3];
    REG(rd << 2, HiValue, 1);
    printf("$%d\n", rd);
    printf("$%d <- HI(0x%08x)\n", rd, HiValue);
}

void mflo(int rd) {
    int LoValue = LO[0] << 24 | LO[1] << 16 | LO[2] << 8 | LO[3];
    REG(rd << 2, LoValue, 1);
    printf("$%d\n", rd);
    printf("$%d <- LO(0x%08x)\n", rd, LoValue);
}

void mul(int rs, int rt) {
    int n = REG(rt << 2, 0, 0);
    unsigned int HiNum;
    unsigned int LoNum;
    if ((n / 2) > 32) {
        HiNum = REG(rs << 2, 0, 0) >> ((int) (n / 2) - 32);
        HI[0] = (HiNum >> 24 & 0xFF);
        HI[1] = (HiNum >> 16 & 0xFF);
        HI[2] = ((HiNum >> 8) & 0xFF);
        HI[3] = (HiNum & 0xFF);
    }
    LoNum = REG(rs << 2, 0, 0) >> n;
    if (n % 2 == 1) {
        LoNum += REG(rs << 2, 0, 0);
    }
    LO[0] = (LoNum >> 24 & 0xFF);
    LO[1] = (LoNum >> 16 & 0xFF);
    LO[2] = ((LoNum >> 8) & 0xFF);
    LO[3] = (LoNum & 0xFF);

    int HiValue = HI[0] << 24 | HI[1] << 16 | HI[2] << 8 | HI[3];
    int LoValue = LO[0] << 24 | LO[1] << 16 | LO[2] << 8 | LO[3];
    printf("$%d, $%d\n", rs, rt);
    printf("HI : 0x%08x, LO : 0x%08x\n", HiValue, LoValue);
}

void add(int rd, int rs, int rt) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(8, rsValue, rtValue, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ($%d(0x%08x) + $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);
}

void sub(int rd, int rs, int rt) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(9, rsValue, rtValue, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ($%d(0x%08x) - $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);

}

void and(int rd, int rs, int rt) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(12, rsValue, rtValue, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ($%d(0x%08x) & $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);
}

void or(int rd, int rs, int rt) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(13, rsValue, rtValue, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ($%d(0x%08x) | $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);
}

void xor(int rd, int rs, int rt) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(14, rsValue, rtValue, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ($%d(0x%08x) ^ $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);
}

void nor(int rd, int rs, int rt) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    res = ALU(15, rsValue, rtValue, &zero);
    REG(rd << 2, res, 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ~($%d(0x%08x) | $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);
}

void slt(int rd, int rs, int rt) {
    rtValue = REG(rt << 2, 0, 0);
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(9, rtValue, rsValue, &zero);
    REG(rd << 2, (res > 0 ? 1 : 0), 1);
    printf("$%d, $%d, $%d\n", rd, rs, rt);
    printf("$%d <- 0x%08x ($%d(0x%08x) < $%d(0x%08x))\n", rd, res, rs, rsValue, rt, rtValue);
}

void bltz(int rs, int imm, unsigned int *PC) {
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(9, 0, rsValue, &zero);
    printf("$%d, 0x%08x\n", rs, *PC + imm);
    if (res < 0) { // 0 이면 두 값이 같음
        setPC(*PC + imm, PC);
        printf("PC <- 0x%08x, 1 ($%d(%08x) < 0)\n", rs, rsValue, *PC + imm);
    } else {
        printf("0 ($%d(%08x) > 0 (0x%08x))\n", rs, rsValue, *PC + imm);
    }
}

void j(unsigned int Link, unsigned int *PC) {
    setPC(Link, PC);
    printf("0x%08x\n", Link);
    printf("PC <- 0x%08x\n", *PC);
}

void jal(unsigned int Link, unsigned int *PC) {
    unsigned int origin = *PC;
    REG(31 << 2, origin, 1);
    setPC(Link, PC);
    printf("%d\n", Link);
    printf("$ra <- 0x%08x, PC <- 0x%08x\n", origin, Link);
}

void beq(int rs, int rt, int imm, unsigned int *PC) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    ALU(9, rsValue, rtValue, &zero);

    printf("$%d, $%d, %d\n", rs, rt, imm);

    if (!zero) { // 0 이면 두 값이 같음
        setPC(*PC + imm, PC);
        printf("PC <- 0x%08x, 1 ($%d(0x%08x) == $%d(0x%08x))\n", *PC, rs, rsValue, rt, rtValue);
    } else {
        printf("0 ($%d(0x%08x) != $%d(0x%08x))\n", rs, rsValue, rt, rtValue);
    }
}

void bne(int rs, int rt, int imm, unsigned int *PC) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    ALU(9, rsValue, rtValue, &zero);

    printf("$%d, $%d, %d\n", rs, rt, imm);

    if (zero) { // 0 이면 두 값이 다름
        setPC(*PC + imm, PC);
        printf("PC <- 0x%08x, 1 ($%d(0x%08x) != $%d(0x%08x))\n", *PC, rs, rsValue, rt, rtValue);
    } else {
        printf("0 ($%d(0x%08x) == $%d(0x%08x))\n", rs, rsValue, rt, rtValue);
    }
}

void addi(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(8, rsValue, imm, &zero);
    REG(rt << 2, res, 1);
    printf("$%d, $%d, %d\n", rt, rs, imm);
    printf("$%d <- 0x%08x ($%d(0x%08x) + 0x%08x)\n", rt, res, rs, rsValue, imm);
}

void slti(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(9, imm, rsValue, &zero);
    REG(rt << 2, (res > 0 ? 1 : 0), 1);
    printf("$%d, $%d, %d\n", rt, rs, imm);
    printf("$%d <- 0x%08x ($%d(0x%08x) < 0x%08x)\n", rt, res, rs, rsValue, imm);
}

void andi(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(12, rsValue, imm, &zero);
    REG(rt << 2, res, 1);
    printf("$%d, $%d, %d\n", rt, rs, imm);
    printf("$%d <- 0x%08x ($%d(0x%08x) < 0x%08x)\n", rt, res, rs, rsValue, imm);
}

void ori(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(13, rsValue, imm, &zero);
    REG(rt << 2, res, 1);
    printf("$%d, $%d, %d\n", rt, rs, imm);
    printf("$%d <- 0x%08x ($%d(0x%08x) | 0x%08x)\n", rt, res, rs, rsValue, imm);
}

void xori(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    res = ALU(14, rsValue, imm, &zero);
    REG(rt << 2, res, 1);
    printf("$%d, $%d, 0x%08x\n", rt, rs, imm);
    printf("$%d <- 0x%08x ($%d(0x%08x) ^ 0x%08x)\n", rt, res, rs, rsValue, imm);
}

void lui(int rt, int imm) {
    REG(rt << 2, imm << 16, 1);
    printf("$%d, %d\n", rt, imm);
    printf("$%d <- 0x%08x (0x%08x << 16) \n", rt, imm << 16, imm);
}

void lb(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    unsigned int b = MEM(imm + rsValue, 0, 0, 1);
    REG(rt << 2, b, 1);
    printf("$%d, %d($%d)\n", rt, imm, rs);
    printf("$%d <- 0x%01x MEM(&0x%08x)\n", rt, b, imm + rsValue);
}

void lw(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    unsigned int w = MEM(imm + rsValue, 0, 0, 2);
    REG(rt << 2, w, 1);
    printf("$%d, %d($%d)\n", rt, imm, rs);
    printf("$%d <- 0x%08x MEM(&0x%08x) \n", rt, w, imm + rsValue);

}

void lbu(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    unsigned int b = MEM(imm + rsValue, 0, 0, 1);
    REG(rt << 2, b, 1);
    printf("$%d, %d($%d)\n", rt, imm, rs);
    printf("$%d <- 0x%01x MEM(&0x%08x)\n", rt, b, imm + rsValue);
}

void sb(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    MEM(imm + rsValue, rtValue, 1, 1);
    printf("$%d, %d($%d)\n", rt, imm, rs);
    printf("MEM(&0x%08x) <- 0x%01x\n", imm + rsValue, rtValue);
}

void sw(int rt, int rs, int imm) {
    rsValue = REG(rs << 2, 0, 0);
    rtValue = REG(rt << 2, 0, 0);
    MEM(imm + rsValue, rtValue, 1, 2);
    printf("$%d, %d($%d)\n", rt, imm, rs);
    printf("MEM(&0x%08x) <- 0x%08x\n", imm + rsValue, rtValue);
}


