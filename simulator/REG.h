#include <stdio.h>

unsigned char REGDATA[0x80];
//32개의 4byte 데이터를 저장하기 위해 0x80의 크기 지정


int REG(unsigned int address, unsigned int V, int nRW) {
    unsigned int offset;
    offset = address;

    if (address > 0x80) {
        return 1;
    }

    if (nRW == 0) { // read
        return REGDATA[offset] << 24 | REGDATA[offset + 1] << 16 | REGDATA[offset + 2] << 8 | REGDATA[offset + 3];
    } else if (nRW == 1) { // write
        REGDATA[offset] = (V >> 24 & 0x000000FF);
        REGDATA[offset + 1] = (V >> 16 & 0x000000FF);
        REGDATA[offset + 2] = ((V >> 8) & 0x000000FF);
        REGDATA[offset + 3] = (V & 0x000000FF);
    }

    return 0;
}