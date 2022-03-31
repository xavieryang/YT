#include <stdio.h>
#include <stdlib.h>

#define BLACK 1
#define SPACE 0
#define WHITE -1

typedef struct _Board {
    int m;
    int n;
    int row_n[25];
    int col_m[25];   
    int black_n[25][25]; 
    int black_m[25][25];
    int *cell;
    int fillcell;
} Board;

void Print_Board(Board *brd)
{
    int n,m;

    for (n=0; n<brd->n;n++) {
        for (m=0; m<brd->m; m++) {
            if (brd->cell[n * brd->m + m] == BLACK) 
                printf("o");
            else
                printf("_");
        }
        printf("\n");
    }
}

int Check_Board(Board *brd)
{
    int i,j;

    //check row black
    for (i=0;i<brd->n;i++) {
        int shift = 0, black=0, black_row[25];
        for (j=0;j<brd->m;j++) {
            if (*(brd->cell+i*brd->m+j) == BLACK) {
                black++;
            } else {
                if (black > 0) { //turn to white. check the accu black
                    black_row[shift] = black;
                    black = 0;
                    if (black_row[shift] != brd->black_n[i][shift])
                        return 0; //False
                    shift++;
                }
            }
        }
        if (black > 0) { //turn to white. check the accu black
            black_row[shift] = black;
            if (black_row[shift] != brd->black_n[i][shift])
                return 0;          
            shift++;      
        }
        if (shift != brd->row_n[i])
            return 0;    

    }

    //check column black
    for (i=0;i<brd->m;i++) {
        int shift = 0, black=0, black_col[25];
        for (j=0;j<brd->n;j++) {
            if (*(brd->cell+j*brd->m+i) == BLACK) {
                black++;
            } else {
                if (black > 0) { //turn to white. check the accu black
                    black_col[shift] = black;
                    black = 0;
                    if (black_col[shift] != brd->black_m[i][shift])
                        return 0; //False
                    shift++;
                }
            }
        }
        if (black > 0)  { //turn to white. check the accu black
            black_col[shift] = black;
            if (black_col[shift] != brd->black_m[i][shift])
                return 0;   
            shift++;             
        }
        if (shift != brd->col_m[i])
            return 0;    
    }
    return 1; //True
}

int Recur(Board *brd)
{
    int filled = brd->fillcell;
    int total = (brd->m * brd->n);

    if (filled == total) {
        if (Check_Board(brd)) {
            Print_Board(brd);
            return 1;
        } else {
            return 0;
        }
    } else {
        *(brd->cell+filled) = BLACK;
        brd->fillcell++;
        if (Recur(brd)) {
            return 1;
        } 
        *(brd->cell+filled) = WHITE;
        if (Recur(brd)) {
            return 1;
        }
        *(brd->cell+filled) = SPACE;
        brd->fillcell--;
        return 0;
    }
}

int main() 
{
    int n, m, cnt, i, j;
    Board *brd;

    scanf("%d%d",&n,&m);
    brd = malloc(sizeof(Board));
    brd->cell = calloc(n*m, sizeof(int));
    brd->m = m;
    brd->n = n;
    
    for (i=0; i<n; i++) {
        scanf("%d",&cnt);
        brd->row_n[i] = cnt;
        j=0;
        while (j < cnt) {
            scanf("%d",&brd->black_n[i][j++]);
        }
    }

    for (i=0; i<m; i++) {
        scanf("%d",&cnt);
        brd->col_m[i]=cnt;
        j=0;
        while (j < cnt) {
            scanf("%d",&brd->black_m[i][j++]);
        }
    }
    Recur(brd);
}
