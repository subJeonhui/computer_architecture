#include <stdio.h>

unsigned char progMEM[0x100000], dataMEM[0x100000], stackMEM[0x100000];

unsigned int MEM(unsigned int address, unsigned int value, int nRW, int size) {
    unsigned int sel, offset;
    unsigned char *pM;

    sel = address >> 20; // 004 Text 100 Data 7FF Stack 
    offset = address & 0xFFFFF;

    if (sel == 0x004) pM = progMEM;  // program memory
    else if (sel == 0x100) pM = dataMEM;  // data memory
    else if (sel == 0x7FF) pM = stackMEM;  // stack
    else {
        printf("Empty memory\n");
        return -1;
    }
    if (size == 0) {  // byte
        if (nRW == 0) { // read
            return pM[offset];
        } else if (nRW == 1) { // write
            pM[offset] = value & 0x000000FF;
        }
    } else if (size == 1) { // half word
        offset = offset & 0xfffffffe; // aligned access
        if (nRW == 0) { // read
            return pM[offset] << 8 | pM[offset + 1];
        } else if (nRW == 1) { // write
            pM[offset] = ((value >> 8) & 0x000000FF);
            pM[offset + 1] = (value & 0x000000FF);
        }
    } else if (size == 2) { // word
        offset = offset & 0xfffffffc; // aligned access
        if (nRW == 0) { // read
            return pM[offset] << 24 | pM[offset + 1] << 16 | pM[offset + 2] << 8 | pM[offset + 3];
        } else if (nRW == 1) { // write
            pM[offset] = (value >> 24 & 0xFF);
            pM[offset + 1] = (value >> 16 & 0xFF);
            pM[offset + 2] = ((value >> 8) & 0xFF);
            pM[offset + 3] = (value & 0xFF);
        }
    } else { // error
        printf("Processing Error\n");
        return -1;
    }
    return 0;
}