#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE    1000
#define PGO     MAX_LINE/30
#define PLEAVE  MAX_LINE/30
#define PCLOSE  0.3 
#define ROUND 3

int main()
{

    int i; 
    int g, r, p, gid, rid;
    long long int id;
    int pclose;
    int K[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000};
    int M[7] = { 1000000, 100000, 10000, 1000, 100, 10, 1};
 
    int *ROOM = malloc(M[ROUND]*sizeof(int));

    srand( (unsigned)time( NULL ) );
    pclose = MAX_LINE / (M[ROUND] * PCLOSE) ;
    printf("%d %d %d\n",K[ROUND],MAX_LINE,M[ROUND]);
    for (i=0;i<MAX_LINE;i++) {
        rid = rand()%M[ROUND];         
        id = i;
        gid = rand()%K[ROUND];
        if (i>50) {
            if (ROOM[rid]>0) {

                if ((p=rand()%PGO)==0) { //go 
                    printf("go %d\n",rid);
                    ROOM[rid]--;
                    continue;
                } else if ((p=rand()%PLEAVE)==0) { //leave 
                    printf("leave %d\n",rid);
                    ROOM[rid]--;
                    continue;
                } else if ((p=rand()%pclose)==0) { //close 
                
                    printf("close %d\n",rid);
                    ROOM[rid]=-1;
                    continue;
                }
            }
        }
        while (ROOM[rid]== -1){
            rid = rand()%M[ROUND];  
        }
        printf("enter %d %d %d\n",gid,i,rid);
        ROOM[rid]++;
    }

}

