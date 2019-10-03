#include <stdio.h>
#include <string.h>
// #include <ctype.h>

int main(int argc, char** argv){
char fileName[100];
    if (argc < 4)
        scanf("%s", fileName);
    else 
        strcpy(fileName, argv[3]);
    FILE* input  = fopen(fileName, "r");
    FILE * fp = fopen("formula", "w");

    int x, y, n;

    //make variable
    for (y = 1 ; y <= 9 ; y++){
        for(x = 1 ; x <= 9 ; x++){
        for(n = 1 ; n <= 9; n++){
            fprintf(fp,"(declare-const p%d%d%d Int)\n", y, x, n);
        }
        }
    }

    //input
    char chr;
    int grid[10][10] = {0};
    for(y = 1; y <= 9 ; y++){
        for(x = 1; x <= 9 ; x++){

        fscanf(input," %c", &chr);
        if(chr == '*'){
            grid[y][x] = -1;
        }
        else if( chr >= '1' &&  chr <= '9'){
            grid[y][x] = chr - '0';
            fprintf(fp,"(assert (= p%d%d%d 1))\n", y, x, grid[y][x]);
        }
        }
    }

    fclose(input);

        //Q1 1 or 0
        for (y = 1 ; y <= 9 ; y++)
        for (x = 1 ; x <= 9 ; x++)
            for(n = 1 ; n <= 9 ; n++)
            fprintf(fp,"(assert (and (<= p%d%d%d 1) (<= 0 p%d%d%d)))\n", y, x, n, y, x, n) ;

        //Q1-2
        //n은 한개의 숫자만 가짐(1~9 중에서)
        for (y = 1 ; y <= 9 ; y++)
        {
        for (x = 1 ; x <= 9 ; x++)
        {
                fprintf(fp,"(assert(= (+ ") ;
                for(n = 1 ; n <= 9 ; n++)
                fprintf(fp,"p%d%d%d ", y, x, n) ;
                fprintf(fp,") 1))\n") ;
        }
        }

        //Q3 row에 총 9개
        fprintf(fp,"; Q3\n");
        for(x = 1 ; x <= 9 ; x++){
        for(n = 1; n <= 9 ; n++){
            fprintf(fp,"(assert(= (+ ");
            for(y = 1 ; y <= 9 ; y++)
            fprintf(fp,"p%d%d%d ", y, x, n);
            fprintf(fp,") 1))\n");
        }
        }


        //Q4 col에 총 9개
        //printf("\n\n\n\nQ4 col total = 9\n\n");
        fprintf(fp,"; Q4\n");
        for(y = 1 ; y <= 9 ; y++){
        for(n = 1; n <= 9 ; n++){
            fprintf(fp,"(assert(= (+ ");
            for(x = 1 ; x <= 9 ; x++)
            fprintf(fp,"p%d%d%d ", y, x, n);
            fprintf(fp,") 1))\n");
        }
        }

        //Q5 3*3 그리드에 총 9개
        fprintf(fp,"; Q5\n");
        for(int r = 0 ; r <= 2 ; r++){
        for(int s = 0 ; s <= 2 ; s++){
            for(n = 1 ; n <= 9 ; n++){
            fprintf(fp,"(assert(= (+ ") ;
                for(y = 1 ; y <= 3 ; y++)
                for(x = 1 ; x <= 3 ; x++)
                    fprintf(fp,"p%d%d%d ", 3*r + y,3*s + x, n) ;
            fprintf(fp,") 1))\n") ;
            }
        }
        }

    //Q6
    for(n = 1 ; n <= 9 ; n++){
        fprintf(fp, "(assert(<= (+ ");
        for (y = 1 ; y <= 9 ; y++){
        for (x = 1 ; x <= 9 ; x++){
            if(grid[y][x] == -1){
            fprintf(fp, "p%d%d%d ",y ,x ,n);
            }
        }
        }
        fprintf(fp, ") 1))\n");
    }


        fprintf(fp,"(check-sat)\n(get-model)\n") ;

        fclose(fp);






        //output
        FILE * fin = popen("z3 formula >  solution", "r");
        pclose(fin);

        FILE * output = fopen("solution", "r");

        //parse.c
        char a[128] ;
        char b[128] ;
        char s[128] ;
        char t[128] ;
        int i,j;

        fscanf(output,"%s %s", a, b) ;
        if(strcmp(a, "unsat") == 0){
        printf("No solution\n");
        fclose(output);

        return 0;
        }

        while (!feof(output)) {
            fscanf(output,"%s %s %s %s %s", b, s, b, b, t) ;

            i = s[1] - '0' ;
            j = s[2] - '0' ;
            n = s[3] - '0' ;

            if (strcmp(t, "0)") != 0) {
            grid[i][j] = n ;
            //printf("grid[%d][%d] = %d, ", i, j, n);
            }
        }

        printf("\n");

        for (i = 1 ; i <= 9 ; i++) {
            for (j = 1 ; j <= 9 ; j++) {
            printf("%d ", grid[i][j]) ;
            }
            printf("\n") ;
        }

        fclose(output);

        return 0;
    }

