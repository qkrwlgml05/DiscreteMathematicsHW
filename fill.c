#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "puzzle.h"

void fill_matrix_fill (FILE* file, int *N, int *M, int *num, int input[][1000], int input2[][1000]);
int parse_fill (int M, int N, int sol, int ***sol_mat);
void print(int M, int N, int sol, int ***sol_mat);
void copy (int M, int N, int input[][1000], int input2[][1000]);
int fillapix(char* filename) ;

int fillapix(char* filename) {
    printf("\n\n---FILL A PIX---\n");
    FILE* file = fopen(filename, "r");
    
    //FILE* file = fopen(argv[1], "r");
    int input[1000][1000];
    int input2[1000][1000];
    
    int N = 0, M = 0, num = 0;
    int i = 0, j = 0;
    int sol =0;
    fill_matrix_fill(file, &N, &M, &num, input, input2);

        
//=================================================
    //multiple solution을 저장하기 위한 solution array
    int ***solution;
    solution = (int ***)malloc(5 * sizeof(int **)); 
    for (int  n= 0; n < 5; n++)
    {
        solution[n] = (int **)malloc(M * sizeof(int *));
        for (int x = 0; x < M; x++)
        {
        solution[n][x] = (int *)malloc(N * sizeof(int)); 
        }
    }
    for (int n = 0; n < 5; n++)
    {
        for (int i = 0; i < M; i++)
        {
        for (int j = 0; j < N; j++)
        {
            solution[n][i][j] = 0;
        }
        }
    }
  
//=========================================================
	

// ---------------------------------------------------------------------- //
    
     //multiple solution 일단 5개만 찾기....
    while(sol <5){
    
        FILE * fp = fopen("formula", "w");
        for (int k = 1; k <=5; k++)
            for (i = 1; i <= M; i++)
                for (j = 1; j <= N; j++)
                    fprintf(fp, "(declare-const a%d%d%d Int)\n", k, i, j) ;

        for (i = 1; i <= M; i++)
            for (j = 1; j <= N; j++)
                fprintf(fp, "(declare-const a%d%d Int)\n", i, j) ;
        
        if (sol > 0){
            for (int k = 0; k < sol; k++)
                for (i = 0; i < M; i++)
                    for (j = 0; j < N; j++)
                        fprintf(fp, "(assert (and (<= a%d%d%d %d) (<= %d a%d%d%d)))\n", k+1, i+1, j+1, solution[k][i][j], solution[k][i][j], k+1, i+1, j+1);
        }

        for (i = 1; i <= M; i++)
            for (j = 1; j <= N; j++)
                fprintf(fp, "(assert (and (<= a%d%d 1) (<= 0 a%d%d)))\n", i, j, i, j) ;
        
        for (i = 0; i < M; i++){
            for (j = 0; j < N; j++){
                
                if (input[i][j] < 0)
                    continue;
                if ((input[i][j] == 9)  && j > 0 && j < N-1 && i > 0 && i < M-1){
                    fprintf(fp, "(assert ( and");
                    int k = 0;
                    input[i][j] = -1;
                    for (k = 0; k < 9; k++){
                        if (j+k%3 > N || i+k/3 > M || j+k%3 <= 0 || i+k/3 <= 0)
                            continue;
                        fprintf(fp, " (<= 1 a%d%d) ", i+k/3, j+k%3) ;
                    }
                    fprintf(fp, "))\n") ;
                }
                else if (input[i][j] == 0){
                    fprintf(fp, "(assert (and ");
                    int k = 0;
                    input[i][j] = -1;
                    for (k = 0; k < 9; k++){
                        if (j+k%3 > N || i+k/3 > M || j+k%3 <= 0 || i+k/3 <= 0)
                            continue;
                        fprintf(fp, " (<= a%d%d 0) ", i+k/3, j+k%3) ;
                    }
                    fprintf(fp, "))\n") ;
                }
                else if ((i == 0 || i == M-1) && j > 1 && j < N-1 && input[i][j] == 6){ // 위 아래 사이드
                    fprintf(fp, "(assert (and ");
                    int k = 0;
                    input[i][j] = -1;
                    for (k = 0; k < 9; k++){
                        if (j+k%3 > N || i+k/3 > M || j+k%3 <= 0 || i+k/3 <= 0)
                            continue;
                        fprintf(fp, " (<= 1 a%d%d) ", i+k/3, j+k%3) ;
                    }
                    fprintf(fp, "))\n") ;
                }
                else if ((j == 0 || j == N-1) && i > 0 && i < M-1 && input[i][j] == 6){ // 양 옆사이드
                    fprintf(fp, "(assert (and ");
                    int k = 0;
                    input[i][j] = -1;
                    for (k = 0; k < 9; k++){
                        if (j+k%3 > N || i+k/3 > M || j+k%3 <= 0 || i+k/3 <= 0)
                            continue;
                        fprintf(fp, " (<= 1 a%d%d) ", i+k/3, j+k%3) ;
                    }
                    fprintf(fp, "))\n") ;
                }
                else if ((i == 0 && j == 0) || (i == M-1 && j == 0) || (i == 0 && j == N-1) || (i == M-1 && j == N-1)){
                    if (input[i][j] == 4){ // 모서리
                        fprintf(fp, "(assert (and ");
                        int k = 0;
                        input[i][j] = -1;
                        for (k = 0; k < 9; k++){
                            if (j+k%3 > N || i+k/3 > M || j+k%3 <= 0 || i+k/3 <= 0)
                                continue;
                            fprintf(fp, " (<= 1 a%d%d) ", i+k/3, j+k%3) ;
                        }
                        fprintf(fp, "))\n") ;

                    }
                }
            }
        }

        for (i = 0; i < M; i++){
            for (j = 0; j < N; j++){
                if (input[i][j] < 0) continue;   
                fprintf(fp, "(assert(= (+ ") ;
                    for (int k = 0 ; k < 9 ; k++){
                        if (j+k%3 > M || i+k/3 > N || j+k%3 < 1 || i+k/3 < 1)
                            continue;
                        fprintf(fp, "a%d%d ", i+k/3, j+k%3);
                    }
                fprintf(fp, ") %d))\n", input[i][j]) ;

                input[i][j] = -1;
            }
        }
//===================================================
        //그 전 solution과 겹치지 않도록
        if (sol != 0)
        {
            for (int n = 0; n < sol; n++)
            {
                fprintf(fp, ("(assert (not (and "));
                for (i = 0; i < M; i++)
                {
                	for (j = 0; j < N; j++)
                	{
                    	fprintf(fp, "(= a%d%d %d)", i+1, j+1, solution[n][i][j]);
                	}
                }
                fprintf(fp, ")))\n");
            }
        }
    //====================================================


        fprintf(fp, "(check-sat)\n(get-model)\n") ;

        fclose(fp) ;

        FILE * fin = popen("z3 formula", "r") ; //FIXME
        FILE * fps = fopen("solution", "w") ; //FIXME
        char buf[128] ;
        fscanf(fin, "%s %s", buf, buf) ;
        while (!feof(fin)) {
            fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
            fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
            fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
            fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
            fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
        }
        fclose(fps);
        pclose(fin) ;

        int answer = parse_fill(M, N, sol, solution);
        if (answer > 0)
            sol++;
        else if (sol == 0){
            printf("No solution.\n");
            return 0;
        }else
            break;
        copy(M, N, input, input2);
    }
    print(M, N, sol, solution);
    return 0;
}

void fill_matrix_fill (FILE* file, int *N, int *M, int *num, int input[][1000], int input2[][1000]){
    char line[500];
    int num_char = sizeof(line)/sizeof(char);
    
    while (fgets(line, num_char, file) != NULL){
        int k = 0;
        (*N) = 0;
        while(line[k++] != '\0'){
            
            if ((line[k-1] >= '0' && line[k-1] <= '9') || line[k-1] == '?'){
                (*num)++;
                input[*M][*N] = line[k-1]-'0';
                input2[*M][(*N)++] = line[k-1]-'0';
                if (line[k-1] == 63){
                    input[*M][(*N)-1] = -1;
                    input2[*M][(*N)-1] = -1;
                    (*num)--;
                }
            }
        }
        (*M)++;
    }
}

int getSize (int input[][1000], int N, int M, int i, int j){
    if ((i == 0 || i == M-1) && j > 0 && j < N-1){
        return 6;
    }else if ((j == 0 || j == N-1) && i > 0 && i < M-1){
        return 6;
    }else if ((i == 0 && j == 0) || (i == M-1 && j == 0) || (i == 0 && j == N-1) || (i == M-1 && j == N-1)){
        return 4;
    }
    return 9;
}

int parse_fill (int M, int N, int sol, int ***sol_mat) {
    FILE* file = fopen("solution", "r");
    int i, j, k;
    //int board[10][100][100];

    char b[128] = "";
    char s[128] = "";
    char t[128] = "";

    for (k = 0; k < M*N*sol+M*N; k++){
        fscanf(file, "%s %s %s %s %s", b, s, b, b, t);
        if (s[3] >= '0' && s[3] <= '9') continue;
        i = (s[1] >= '0' && s[1] <= '9' && t[0] >= '0' && t[0] <= '9') ? s[1]-'0' : 0;
        j = (s[2] >= '0' && s[2] <= '9' && t[0] >= '0' && t[0] <= '9') ? s[2]-'0' : 0;

        sol_mat[sol][i-1][j-1] = 0;
        int a = 0;
        if (t[0] != '0')
            sol_mat[sol][i-1][j-1] = 1;
    }
    if (i==0 || j==0){
        return 0;
    }
    return 1;
}

void print(int M, int N, int sol, int ***sol_mat){
    for (int k = 0; k < sol; k++){
        printf("\n%d-st solution \n", k+1);
        for (int i = 0; i < M; i++){
            for (int j = 0; j < N; j++){
                printf(" %d", sol_mat[k][i][j]);
            }
            printf("\n");
        }
    }
}

void copy (int M, int N, int input[][1000], int input2[][1000]){
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            input[i][j] = input2[i][j];
        }
    }
}