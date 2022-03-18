#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_OP_NUM  9
#define MAXLEN      1000000

typedef struct _entry {
    int op;
    long long int num;
} ENTRY;

ENTRY S[100000];

char pri[MAX_OP_NUM] = { '=', '(', ')', ' ', '+', '-', '*', '/', '%' };
int priority(char in, char stack)
{
    int pi, ps;

    if ((in=='(') || (stack=='('))
        return 0;

    for (int i=0; i<MAX_OP_NUM;i++) {
        if (in == pri[i])
            pi=i;
        if (stack == pri[i])
            ps=i;    
    }
    if (pi/3 <= ps/3)
        return 1;
    return 0;    
}
#define OP_MODE  0
#define NUM_MODE 1
void main()
{
    int top=-1, ctr;
    char stack[MAXLEN],out[MAXLEN];
    int  s_top=-1, out_top=-1;
    char c;
    int temp, prev_mode=OP_MODE;

    while ((c=getchar())!='\n') {
        if (isdigit(c)) {
            if (prev_mode=OP_MODE) {
                temp=0;
            }
            temp = temp * 10 + c - '0';
            prev_mode = NUM_MODE;
        } else {
            if (prev_mode== NUM_MODE) {
         //       push(temp)
            }
            switch (c) {
                case ')':  //dump stack to out until ( found
                    while (stack[s_top]!= '(') 
     //push op                   out[++out_top] = stack[s_top--]; 
                    s_top--;      
                    break;
                case '%': 
                case '/': 
                case '*':
                case '-': 
                case '+': 
                case '(':
                case '=': 
                    while ((s_top>=0) && (priority(c, stack[s_top]))) {
      //push op                  out[++out_top]=stack[s_top--];
                    }
                    stack[++s_top]=c;
                    if (c=='=')
      //push                out[++out_top]=stack[s_top--]; 
                    break;    
                default:
                    break;    
            }    
    }
    //postfix to cal
 
}