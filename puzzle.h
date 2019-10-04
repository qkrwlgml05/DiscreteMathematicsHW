#include <stdio.h>
#include <stdlib.h>

int numbrix(char* filename); 
int sudoku(char* argv);
int fillapix(char* filename) ;
void fill_matrix_num (FILE* file, int *N, int *M, int *num, int input[][1000]);
void parse_num(int M, int N);
void fill_matrix_fill (FILE* file, int *N, int *M, int *num, int input[][1000], int input2[][1000]);
int parse_fill (int M, int N, int sol, int ***sol_mat);
void print(int M, int N, int sol, int ***sol_mat);
void copy (int M, int N, int input[][1000], int input2[][1000]);
