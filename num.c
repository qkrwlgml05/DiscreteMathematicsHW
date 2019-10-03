#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fill_matrix (FILE* file, int *N, int *M, int *num, int input[][1000]);
void print (int input[][1000], int N, int M);
int getSize (int input[][1000], int N, int M, int i, int j);
void parse(int M, int N);

int main(int argc, char** argv) {
    char filename[100];
    if (argc < 4)
        scanf("%s", filename);
    else 
        strcpy(filename, argv[3]);
    FILE* file = fopen(filename, "r");
    int input[1000][1000];
    
    int N = 0, M = 0, num = 0;
    int i = 0, j = 0;
    fill_matrix(file, &N, &M, &num, input);

	FILE * fp = fopen("formula", "w");

    for (i = 1; i <= M; i++)
        for (j = 1; j <= N; j++)
			fprintf(fp, "(declare-const a%d%d Int)\n", i, j) ;

    // 모든 값의 범위 정해주기
    for (i = 1; i <= M; i++)
        for (j = 1; j <= N; j++)
			fprintf(fp, "(assert (and (<= 1 a%d%d) (<= a%d%d %d)))\n", i, j, i, j, N*M) ;

    // 입력 값들을 넣어주기
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++){
            if (input[i][j] > 0)
                fprintf(fp, "(assert (and (<= %d a%d%d) (<= a%d%d %d)))\n", input[i][j], i+1, j+1, i+1, j+1, input[i][j]) ;
        }
    
    // 모든 값들은 서로 다름.
    fprintf(fp, "(assert( and") ;
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            for (int a = 0; a < M; a++){
                for (int b = 0; b < N; b++){
                    if (a == i && b == j)
                        continue;
                    fprintf(fp, " ( or (< a%d%d a%d%d) (< a%d%d a%d%d))", i+1, j+1, a+1, b+1, a+1, b+1, i+1, j+1) ;
                }
            }
        }
    }
    fprintf(fp, " ))\n") ;

    // 하나의 수에 인접한 수 중 하나는 무조건 1만큼 크다. but, M*N은 제외.
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            fprintf(fp, "(assert(or ") ;
            for (int k = 1; k < 9; k=k+2){
                if (i+k/3 <= 0 || i+k/3 > M || j+k%3 <= 0 || j+k%3 > N){
                    continue;
                }
                fprintf(fp, "(or ( and (= (+ a%d%d 1) a%d%d) (< a%d%d %d)) (= a%d%d %d))", i+1, j+1, i+k/3, j+k%3, i+1, j+1, M*N, i+1, j+1, M*N) ;
            }
            fprintf(fp, "))\n") ;
        }
    }
/*
    // 하나의 수에 인접한 수 중 하나는 무조건 1만큼 작다. but, 1은 제외.
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            fprintf(fp, "(assert(or ") ;
            for (int k = 1; k < 9; k=k+2){
                if (i+k/3 <= 0 || i+k/3 > M || j+k%3 <= 0 || j+k%3 > N){
                    continue;
                }
                fprintf(fp, "( and (= (- a%d%d 1) a%d%d) (< 1 a%d%d))",i+1, j+1, i+k/3, j+k%3, i+1, j+1) ;
            }
            fprintf(fp, "))\n") ;
        }
    }
*/
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
                if (line[k-2] >= '0' && line[k-2] <= '9') {
                    input[*M][*N-1] = (line[k-2]-'0')*10 + line[k-1]-'0';
                }
                else input[*M][(*N)++] = line[k-1]-'0';
                (*num)++;
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

void parse(int M, int N){
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
        while (1) {
            if (t[a] < '0' || t[a] > '9')
                break;
            board[i][j] = board[i][j]*10 + t[a++]-'0';
        }
    }
    if (i==0 || j==0){
        printf("no solution\n");
        exit(-1);
    }
    else{
        for (i = 1; i <= M; i++){
            for (j = 1; j <= N; j++){
                if (board[i][j] < 10)
                    printf(" ");
                printf("%d ", board[i][j]);
            }
            printf("\n");
        }
    }
}

