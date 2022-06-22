#include <stdio.h>

unsigned char Regi[0x80]; //128 -> 80(16진수) - 32*4byte

unsigned int REG(unsigned int A, unsigned int V, unsigned int nRW)
{
    unsigned int Offset;

    Offset = A;

    if(A > 0x80) //주소값 초과
    {
        return 1;
    }

    if(nRW == 0) //write X
    {
        return Regi[Offset] << 24 | Regi[Offset+1] << 16 | Regi[Offset+2] << 8 | Regi[Offset+3];
     }
    else // nRW ==1 , Write O
    {
        Regi[Offset] = (V >> 24) & 0x000000FF;
        Regi[Offset+1] = (V >> 16) & 0x000000FF;
        Regi[Offset+2] = (V >> 8) & 0x000000FF;
        Regi[Offset+3] = V & 0x000000FF;
        //FF=255 (256-1)
    }
    
    return 0;

}
