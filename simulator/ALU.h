#include <stdio.h>

int logicOperation(int v1, int v2, unsigned int s1s0);

int shiftOperation(int V, int v2, unsigned int s1s0);

int addSubtract(int v1, int v2, unsigned int s0);

int checkSetLess(int v1, int v2, unsigned int s1s0);

int ALU(int Fct, int v1, int v2, int *isZero);

int logicOperation(int v1, int v2, unsigned int s1s0) {
    if (s1s0 < 0 || s1s0 > 3) {
        printf("error in logic\n");
        return -1;
    }
    if (s1s0 == 0) {  //AND
        return v1 & v2;
    } else if (s1s0 == 1) {    //OR
        return v1 | v2;
    } else if (s1s0 == 2) {    //XOR
        return v1 ^ v2;
    } else {    //NOR
        return ~(v1 ^ v2);
    }
}

int shiftOperation(int V, int v2, unsigned int s1s0) {
    if (s1s0 < 0 || s1s0 > 3) {
        printf("error in shift\n");
    }
    if (s1s0 == 0) {  // No shift
        return v2;
    } else if (s1s0 == 1) {    //SLL
        return v2 << V;
    } else if (s1s0 == 2) {    //SRL
        return (v2 >> V) & (0x7fffffff >> (V - 1));
    } else {    //SRA
        return v2 >> V;
    }
}

int addSubtract(int v1, int v2, unsigned int s0) {
    int ret;
    if (s0 < 0 || s0 > 1) {
        printf("error in addSubtract\n");
    }
    if (s0 == 0) {  //add
        ret = v1 + v2;
    } else {    //subtract
        v2 = ~v2 + 1; //2의 보수처리
        ret = v1 + v2;
    }
    return ret;
}

int checkSetLess(int v1, int v2, unsigned int s1s0) {
    if ((s1s0 & 0x1) == 1)
        return (addSubtract(v1, v2, 1) >> 31) & 0x00000001;
    return -1;
}

int ALU(int Fct, int v1, int v2, int *isZero) {
    int s32 = Fct >> 2; // 앞 두 bit
    int s10 = Fct & 0x3; // 뒤 두 bit
    int ret;

    if (s32 == 0x0) // 앞 두 bit가 0이면 shift
        ret = shiftOperation(v1, v2, s10);
    else if (s32 == 0x1) // 앞 두 bit가 1이면 checkSetLess
        ret = checkSetLess(v1, v2, s10);
    else if (s32 == 0x2) { // 앞 두 bit가 2면 addSubtract & checkZero
        ret = addSubtract(v1, v2, s10 & 0x1);
        *isZero = (ret == 0 ? 0 : 1);
    } else { // 앞 두 bit가 4이면 logic
        ret = logicOperation(v1, v2, s10);
    }

    return ret;

}