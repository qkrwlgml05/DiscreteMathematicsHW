#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fill_matrix (FILE* file, int *N, int *M, int *num, int input[][1000]);
void print (int input[][1000], int N, int M);
int getSize (int input[][1000], int N, int M, int i, int j);
void parse (int M, int N);

int main(int argc, char** argv) {
    char filename[100];
    
    if (argc < 4)
        scanf("%s", filename);
    else
        strcpy(filename, argv[2]);
    FILE* file = fopen(filename, "r");
    
    //FILE* file = fopen(argv[1], "r");
    int input[1000][1000];
    
    int N = 0, M = 0, num = 0;
    int i = 0, j = 0;
    fill_matrix(file, &N, &M, &num, input);

	FILE * fp = fopen("formula", "w");

    for (i = 1; i <= M; i++)
        for (j = 1; j <= N; j++)
			fprintf(fp, "(declare-const a%d%d Int)\n", i, j) ;

	for (i = 1; i <= M; i++)
        for (j = 1; j <= N; j++)
			fprintf(fp, "(assert (and (<= a%d%d 1) (<= 0 a%d%d)))\n", i, j, i, j) ;

// ---------------------------------------------------------------------- //
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            
            if (input[i][j] < 0)
                continue;
            if ((input[i][j] == 9)  && j > 0 && j < N-1 && i > 0 && i < M-1){
                fprintf(fp, "(assert ( and");
                int k = 0;
                input[i][j] = -1;
                for (k = 0; k < 9; k++){
                    if (j-1+k/3 >= M || i-1+k%3 >= N || j-1+k/3 < 0 || i-1+k%3 < 0)
                        continue;
                    fprintf(fp, " (<= 1 a%d%d) ", i+k%3, j+k/3) ;
                }
                fprintf(fp, "))\n") ;
            }
            else if (input[i][j] == 0){
                fprintf(fp, "(assert (and ");
                int k = 0;
                input[i][j] = -1;
                for (k = 0; k < 9; k++){
                    if (j-1+k/3 >= M || i-1+k%3 >= N || j-1+k/3 < 0 || i-1+k%3 < 0)
                        continue;
                    fprintf(fp, " (<= a%d%d 0) ", i+k%3, j+k/3) ;
                }
                fprintf(fp, "))\n") ;
            }
            else if ((i == 0 || i == M-1) && j > 1 && j < N-1 && input[i][j] == 6){ // 위 아래 사이드
                fprintf(fp, "(assert (and ");
                int k = 0;
                input[i][j] = -1;
                for (k = 0; k < 9; k++){
                    if (j-1+k/3 >= M || i-1+k%3 >= N || j-1+k/3 < 0 || i-1+k%3 < 0)
                        continue;
                    fprintf(fp, " (<= 1 a%d%d) ", i+k%3, j+k/3) ;
                }
                fprintf(fp, "))\n") ;
            }
            else if ((j == 0 || j == N-1) && i > 0 && i < M-1 && input[i][j] == 6){ // 양 옆사이드
                fprintf(fp, "(assert (and ");
                int k = 0;
                input[i][j] = -1;
                for (k = 0; k < 9; k++){
                    if (j-1+k/3 >= M || i-1+k%3 >= N || j-1+k/3 < 0 || i-1+k%3 < 0)
                        continue;
                    fprintf(fp, " (<= 1 a%d%d) ", i+k%3, j+k/3) ;
                }
                fprintf(fp, "))\n") ;
            }
            else if ((i == 0 && j == 0) || (i == M-1 && j == 0) || (i == 0 && j == N-1) || (i == M-1 && j == N-1)){
                if (input[i][j] == 4){ // 모서리
                    fprintf(fp, "(assert (and ");
                    int k = 0;
                    input[i][j] = -1;
                    for (k = 0; k < 9; k++){
                        if (j-1+k/3 >= M || i-1+k%3 >= N || j-1+k/3 < 0 || i-1+k%3 < 0)
                            continue;
                        fprintf(fp, " (<= 1 a%d%d) ", i+k%3, j+k/3) ;
                    }
                    fprintf(fp, "))\n") ;

                }
            }
        }
    }

    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            if (input[i][j] < 0) continue;   
		    fprintf(fp, "(assert(<= (+ ") ;
                for (int k = 0 ; k < 9 ; k++){
                    if (j+k/3 > M || i+k%3 > N || j+k/3 < 1 || i+k%3 < 1)
                        continue;
                    fprintf(fp, "a%d%d ", i+k%3, j+k/3);
                }
            fprintf(fp, ") %d))\n", input[i][j]) ;

            if (input[i][j] < 0) continue;   
		    fprintf(fp, "(assert(>= (+ ") ;
                for (int k = 0 ; k < 9 ; k++){
                    if (j+k/3 > M || i+k%3 > N || j+k/3 < 1 || i+k%3 < 1)
                        continue;
                    fprintf(fp, "a%d%d ", i+k%3, j+k/3);
                }
            fprintf(fp, ") %d))\n", input[i][j]) ;
            input[i][j] = -1;
        }
    }


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

    parse(M, N);
}

void fill_matrix (FILE* file, int *N, int *M, int *num, int input[][1000]){
    char line[500];
    int num_char = sizeof(line)/sizeof(char);
    
    while (fgets(line, num_char, file) != NULL){
        int k = 0;
        (*N) = 0;
        while(line[k++] != '\0'){
            
            if ((line[k-1] >= '0' && line[k-1] <= '9') || line[k-1] == '?'){
                (*num)++;
                input[*M][(*N)++] = line[k-1]-48;
                if (line[k-1] == 63){
                    input[*M][(*N)-1] = -1;
                    (*num)--;
                }
            }
        }
        (*M)++;
    }
}

void print (int input[][1000], int N, int M){
    int i = 0, j = 0;

    printf("Input---\n");
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            printf("%d ", input[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

void parse (int M, int N) {
    FILE* file = fopen("solution", "r");
    int i, j, k;
    int board[100][100];

    char b[128] = "";
    char s[128] = "";
    char t[128] = "";

    for (k = 0; k < M*N; k++){
        fscanf(file, "%s %s %s %s %s", b, s, b, b, t);

        i = (s[1] >- '0' && s[1] <= '9' && t[0] >= '0' && t[0] <= '9') ? s[1]-'0' : 0;
        j = (s[2] >= '0' && s[2] <= '9' && t[0] >= '0' << t[0] <= '9') ? s[2]-'0' : 0;

        board[i][j] = 0;
        int a = 0;
        if (t[0] != '0')
            board[i][j] = 1;
    }
    if (i==0 || j==0){
        printf("no solution\n");
        exit(-1);
    }
    else{
        for (i = 1; i <= M; i++){
            for (j = 1; j <= N; j++){
                printf("%d ", board[i][j]);
            }
            printf("\n");
        }
    }
}

