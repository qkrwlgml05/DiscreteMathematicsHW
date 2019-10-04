#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "puzzle.h"

int main (int argc, char** argv){
    char filename[3][100];
    if (argc < 4){
        printf("sudoku filename : ");
        fscanf(stdin, " %s", filename[0]);
        printf("fillapix filename : ");
        fscanf(stdin, " %s", filename[1]);
        printf("numbrix filename : ");
        fscanf(stdin, " %s", filename[2]);
    }else{
        strcpy(filename[0], argv[1]);
        strcpy(filename[1], argv[2]);
        strcpy(filename[2], argv[3]);
    }
    sudoku(filename[0]);
    fillapix(filename[1]);
    numbrix(filename[2]);
    printf("\n");
    return 0;
}