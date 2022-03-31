#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_ARRAY   1000
#define MAX_OP_NUM  8
#define STATE_NUM   1
#define STATE_OP    2
#define HIGH_OP     1
#define LOW_OP      0

int g_prev_op=LOW_OP;

typedef struct stack_ {
    int array[MAX_ARRAY];
    int index;
} Stack;

char op_array[MAX_OP_NUM] = { '*', '/', '%', '=', '+', '-', ')', '('};

void dump_stack(Stack *s)
{
    int i;

    for(i=s->index;i>=0;i--)
        printf("%d:",s->array[i]);
    printf("\n");    

}
int get_stack(Stack *s,int *data)
{
    if ((s == NULL) || (s->index <0))
        return 0;

    *data = s->array[s->index];
    return 1;
}

int push(Stack *s, int data){
    if ((s == NULL) || (s->index >= MAX_ARRAY-1 ))
        return 0;

    s->array[++s->index] = data; //index init is -1,  index++ first.
    return 1;
}

int pop(Stack *s, int *data){
    if ((s == NULL) || (s->index<0))
        return 0;

    *data = s->array[s->index--];  
    return 1;
}

void calculate(Stack *stack_op, Stack *stack_num, char cur_op)
{
    int buffer, buffer2, op_num=-1;

    if ((stack_num->index<1)&&(cur_op!=')')) 
        return;

    while (pop(stack_op, &op_num)>0) {   
        switch (op_array[op_num]) {
            case '(':
                if (cur_op!=')') { 
                    push(stack_op, op_num); //push back '('
                    return;
                } else {
                    get_stack(stack_op, &op_num);
                    if ((op_array[op_num]=='*')||(op_array[op_num]=='/')||(op_array[op_num]=='%')) {
                        g_prev_op = HIGH_OP; 
                    } else {
                        g_prev_op = LOW_OP;     
                        return;
                    }
                }    
            case '+':
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer += buffer2;
                    push(stack_num,buffer);
                }
                break;

            case '-':
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer -= buffer2;
                    push(stack_num,buffer);
                }
                break;
            case '*':
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer *= buffer2;
                    push(stack_num,buffer);
                }
                break;
            case '/':
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer /= buffer2;
                    push(stack_num,buffer);
                }
                break;            
            case '%':
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer = buffer % buffer2;
                    push(stack_num,buffer);
                }
                break;
        }   
    }
}

int main()
{
    Stack stack_op, stack_num;
    char c;
    int pre_state=0;
    int temp, op_num;

    stack_op.index=-1;
    stack_num.index = -1;

    while ((c=getchar())!=EOF) {
//    printf("%c\n",c);    
        if (isdigit(c)) {
            if (pre_state != STATE_NUM){
                pre_state = STATE_NUM;
                temp = 0;                
            }             
            temp = temp *10 + (c-'0');
        } else {
            if (pre_state == STATE_NUM) 
                push(&stack_num, temp);

            op_num=0;
            switch (c) {
                case '(': 
                    op_num=7;     
                    dump_stack(&stack_num);
                    break;
                case '=': op_num++;     //3
                case '%': op_num++;     //2
                case '/': op_num++;     //1
                case '*':               //0
                    if (g_prev_op == HIGH_OP) {                        
                        calculate(&stack_op, &stack_num, c);
                        dump_stack(&stack_num);
                    }
                    g_prev_op = HIGH_OP;    
                    break;    

                case ')': op_num++;     //6
                case '-': op_num++;     //5
                case '+': op_num+=4;    //4
                    g_prev_op = LOW_OP; 
                    calculate(&stack_op, &stack_num, c);
                    dump_stack(&stack_num);
                    break;
                default:
                    exit(0);
            }    
            if (c == '=') {
                calculate(&stack_op, &stack_num, c);
                dump_stack(&stack_num);
                if (get_stack(&stack_num, &temp))
                    printf("%d\n",temp);
            } else if (c != ')' ) {
                push(&stack_op, op_num);
            }    
            pre_state = STATE_OP;
        }
    }
}

