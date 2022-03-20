#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_OP_NUM  9
#define MAXLEN      10000

typedef struct _entry {
    int op_no;
    long long int num;
} ENTRY;

char op_char[MAX_OP_NUM] =  { ' ' , '(' , ')' , '=' , '+' , '-' , '*' , '/' , '%' };
int  op_prio[MAX_OP_NUM] =  { -1  , -1  , -1  ,  -1 ,  1  ,  1  ,  2  ,  2  ,  2  };

#define OP_MODE  0
#define NUM_MODE 1

int get_op_no(char c)
{
    for (int i=0;i<MAX_OP_NUM;i++)
        if (c == op_char[i])
            return i;
}  

void push(ENTRY *e, int op_no, long long int num)
{
    e->num = num;
    e->op_no = op_no;
}

void pop(ENTRY *e, int *op_no, long long int *num)
{
    *num = e->num ;
    *op_no = e->op_no ;
}
long long int do_op (int op_no,long long int num, long long int num2)
{
    long long int ret;
    switch (op_char[op_no])
    {
    case '+'/* constant-expression */:
        ret = num + num2;
        break;
    case '-'/* constant-expression */:
        ret = num - num2;
        break;
    case '*'/* constant-expression */:
        ret = num * num2;
        break;
    case '/'/* constant-expression */:
        if ((num>0)&&(num2<0)) {
            num = (-num);
            num2 = (-num2);
        }
        if ((num<0)&&(num2>0)) {
            ret = (num - (num2-1)) / num2;
        } else {
            ret = num / num2;
        }
        break;
    case '%'/* constant-expression */:
        ret = num % num2;
        break;
    
    default:
        break;
    }
    return ret;
}

long long int calculate (ENTRY in[], int in_top, ENTRY stack[], int s_top)
{
    int intop = in_top;
    int stop = s_top;
    long long int num, num2;
    int op_no, tmp;

    for (int i=0;i<=in_top;i++) {
        pop (&in[i], &op_no, &num);
        if (op_no) {
            pop (&stack[stop--], &tmp, &num2);
            pop (&stack[stop--], &tmp, &num);            
            num = do_op (op_no, num, num2);
            push ( &stack[++stop], 0, num);
        } else {
            push ( &stack[++stop], 0, num);
        }
    }
    pop(&stack[stop--], &op_no, &num);
    return num;
}

void main()
{
    ENTRY stack[MAXLEN], out[MAXLEN];
    int  s_top=-1, out_top=-1;
    char c;
    long long int num, temp=0; 
    int prev_mode=OP_MODE, cop_no;
 
    while ((c=getchar())!='\n') {
        if (isdigit(c)) {
            temp = temp * 10 + c - '0';
            prev_mode = NUM_MODE;
        } else {
            if (prev_mode== NUM_MODE) {
//printf("push num=%lld\n",temp);
                push(&out[++out_top], 0, temp);
                temp = 0;
            }
            prev_mode=OP_MODE;    
            cop_no = get_op_no(c);    
            switch (c) {
                case ')':  //dump stack to out until ( found
                    while (op_char[stack[s_top].op_no] != '(')
                        push(&out[++out_top], stack[s_top--].op_no, 0);
                    // decrease s_top to drop '(' from the stack
                    s_top--;     
                    break;
                case '%': 
                case '/': 
                case '*':
                case '-': 
                case '+': 
                case '(':
                    while ((s_top>=0) && (op_prio[cop_no] <= op_prio[stack[s_top].op_no]) && (op_prio[cop_no]>0)) {
                        // pop stack and push into out
                        push(&out[++out_top], stack[s_top].op_no, stack[s_top].num);
                        s_top--;  
                    }
                    //push c into stack
                    s_top++;
                    stack[s_top].op_no = cop_no;
                    stack[s_top].num = 0;
                    break;

                case '=':    
                    //dry out all op in stack and calculate
                    while (s_top>=0) {
                        // pop stack and push into out
                        push(&out[++out_top], stack[s_top].op_no, stack[s_top].num);
                        s_top--;  
                    }  
                    num = calculate(out, out_top, stack, s_top);  
                    printf("Print: %lld\n",num);                    
                    break;    
                default:
                    break;    
            } 
        }   
    }
}