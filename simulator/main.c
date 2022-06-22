#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"

unsigned int PC;


int main() {
    char command[3] = ""; // 명령어

    char path[30] = ""; // 경로 (load)
    unsigned int jumpAddress; // jump 뛸 주소

    unsigned int registerNumber; // 0 ~ 31 (setRegister)
    unsigned int location; // (setMemory)
    int value; // (setMemory & setRegister)
    unsigned int start, end; // viewRegister & viewMemory

    unsigned int IC = 0; // Instruction Count
    int pos = 0;
    unsigned int *BP; // break point
    unsigned int *VR = malloc(sizeof(unsigned char) * 32);
    int debug = 0;

    printf("*-------------------MIPS Simulator-----------------*\n");
    printf("| [command]                                        |\n");
    printf("| l   : Load binary file                           |\n");
    printf("| j   : Jump address                               |\n");
    printf("| g   : Go                                         |\n");
    printf("| s   : Step                                       |\n");
    printf("| r   : View Register                              |\n");
    printf("| m   : View Memory                                |\n");
    printf("| sr  : Set Register                               |\n");
    printf("| sm  : Set Memory                                 |\n");
    printf("| sb   : set Breakpoint                            |\n");
    printf("| b   : View Breakpoint                            |\n");
    printf("| p   : View Program Counter                       |\n");
    printf("| d   : Debug mode on/off                          |\n");
    printf("| (Debug mode)                                     |\n");
    printf("| sbr : Set register to be seen when break point.  |\n");
    printf("| vbr : View register to be seen when break point. |\n");
    printf("*--------------------------------------------------*\n\n");


    while (1) {
        // 명령어 입력
        printf("command :");
        scanf("%s", command);

        if (!strcmp(command, "l")) {
            // 출력 및 입력
            printf("path :");
            scanf("%s", path);
            printf("load file : %s\n", path);

            // 함수 실행
            IC = load(path, &PC);
            BP = malloc(sizeof(unsigned char) * IC);
            for (int i = 0; i < IC; i++) {
                BP[i] = 0;
            }
            for (int i = 0; i < 32; i++) {
                VR[i] = 0;
            }

        } else if (!strcmp(command, "j")) {
            // 출력 및 입력
            printf("Range: 0x%08x ~ 0x%08x \n", 0x00400000, 0x00400000 + (IC * 4));
            printf("Jump Address : 0x");
            scanf("%x", &jumpAddress);

            // 함수 실행
            jump(jumpAddress, &PC, &IC);

        } else if (!strcmp(command, "g")) {
            printf("Go\n");
            go(&PC, BP, &IC, path);
            if (debug == 1) {
                viewBreakPointRegister(VR);
            }

        } else if (!strcmp(command, "s")) {
            printf("Step\n");
            step(&PC, &IC, path);
            if (debug == 1) {
                viewBreakPointRegister(VR);
            }

        } else if (!strcmp(command, "r")) {
            // 출력 및 입력
            printf("View Register\n");
            printf("start Register number :");
            scanf("%u", &start);
            printf("end   Register number :");
            scanf("%u", &end);

            // 함수 실행
            viewRegister(start, end);
            printf("\n");

        } else if (!strcmp(command, "m")) {
            // 출력 및 입력
            printf("View Memory\n");
            printf("Prog: 0x00400000 Data: 0x10000000 Stack: 0x7FF00000\n");
            printf("start address : 0x");
            scanf("%x", &start);
            printf("end   address : 0x");
            scanf("%x", &end);

            // 함수 실행
            viewMemory(start, end);
            printf("\n");
        } else if (!strcmp(command, "x")) {
            // 프로그램 종료
            printf("Program Exit\n");
            break;

        } else if (!strcmp(command, "sr")) {
            // 출력 및 입력
            printf("Set Register\n");
            printf("register number :");
            scanf("%u", &registerNumber);
            printf("value :");
            scanf("%d", &value);

            //함수 실행
            setRegister(registerNumber, value);
            printf("\n");

        } else if (!strcmp(command, "sm")) {
            // 출력 및 입력
            printf("Set Memory\n");
            printf("location : 0x");
            scanf("%x", &location);
            printf("value :");
            scanf("%d", &value);


            //함수 실행
            setMemory(location, value);
            printf("\n");

        } else if (!strcmp(command, "p")) {
            // 출력
            printf("PC: 0x%08x\n", PC);
            printf("\n");
        } else if (!strcmp(command, "b")) {
            viewBreakPoint(IC, BP);
            printf("\n");
        } else if (!strcmp(command, "sb")) {
            printf("pos :");
            scanf("%d", &pos);
            setBreakPoint(pos, IC, BP);
            printf("\n");
        } else if (!strcmp(command, "sbr")) {
            printf("reg num:");
            scanf("%d", &pos);
            setBreakPointRegister(pos, VR);
            printf("\n");
        } else if (!strcmp(command, "vbr")) {
            viewBreakPointRegister(VR);
        } else if (!strcmp(command, "d")) {
            //debug mode
            if (debug == 1) {
                debug = 0;
                printf("[debug off]\n");
            } else {
                debug = 1;
                printf("[debug on]\n");
            }
            printf("\n");
        }
    }
    return 0;
}
