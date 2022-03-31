#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_ARRAY   100000
#define MAX_OP_NUM  8
#define STATE_NUM   1
#define STATE_OP    2
#define HIGH_OP     1
#define LOW_OP      0

int gprev_op=LOW_OP;

typedef struct stack_ {
    long long int array[MAX_ARRAY];
    int index;
} Stack;


char op_array[MAX_OP_NUM] = { '(', '*', '/', '%', '=', '+', '-', ')'};
void dump_stack(Stack *s,Stack *op)
{
    int i;
    for(i=0;i<=op->index;i++)
        printf("%c ",op_array[op->array[i]]);
    printf("\n");  
    for(i=0;i<=s->index;i++)
        printf("%lld ",s->array[i]);
    printf("\n");  
}
int get_stack(Stack *s,long long int *data)
{
    if ((s == NULL) || (s->index <0))
        return 0;

    *data = s->array[s->index];
    return 1;
}

int push(Stack *s, long long int data){
    if ((s == NULL) || (s->index >= MAX_ARRAY-1 ))
        return 0;

    s->array[++s->index] = data; //index init is -1,  index++ first.
    return 1;
}

int pop(Stack *s, long long int *data){
    if ((s == NULL) || (s->index<0))
        return 0;

    *data = s->array[s->index--];  
    return 1;
}

void calculate(Stack *stack_op, Stack *stack_num, char cur_op)
{
    long long int buffer, buffer2;
    long long int op_num=-1;
    int priority = LOW_OP;

    if ((stack_num->index<1) && (cur_op!=')'))
        return;

    if ((cur_op=='*') ||(cur_op=='/') ||(cur_op=='%')) 
        priority=HIGH_OP;

    while (pop(stack_op, &op_num)>0) {   
        switch (op_array[op_num]) {
            case '(':
                if (cur_op!=')')
                    push(stack_op, op_num); //push back '('
                return;

            case '+':
                if (priority==HIGH_OP) { //
                    push(stack_op, op_num);
                    return;
                }                    
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer += buffer2;
                    push(stack_num,buffer);
                }
                break;

            case '-':
                if (priority==HIGH_OP) { //
                    push(stack_op, op_num);
                    return;
                }
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer -= buffer2;
                    push(stack_num,buffer);
                }
                break;
            case '*':
                if (pop(stack_num, &buffer2)) {
                    if (pop(stack_num, &buffer)) 
                        buffer = buffer * buffer2;
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
//printf("calculate\n");
//dump_stack(stack_num, stack_op);                
    }
}

int main()
{
    Stack stack_op, stack_num;
    char c;
    int pre_state=0;
    long long int temp=0;
    int op_num;

    stack_op.index=-1;
    stack_num.index = -1;

    while ((c=getchar())!=EOF) {  
        if (isdigit(c)) {
            if (pre_state != STATE_NUM){
                pre_state = STATE_NUM;
                temp = 0;                
            }             
            temp = temp *10 + (c-'0');
        } else {
            if (pre_state == STATE_NUM) {
                push(&stack_num, temp);
//printf("push num :%lld\n",temp);
//dump_stack(&stack_num, &stack_op);                
            }
            op_num=0;
            switch (c) {
                case '=': op_num++;     //4
                case '%': op_num++;     //3
                case '/': op_num++;     //2
                case '*': op_num++;     //1
                    if (gprev_op == HIGH_OP) {                        
                        calculate(&stack_op, &stack_num, c);
                    }
                    gprev_op = HIGH_OP;    
                    break;    

                case ')': op_num++;     //7
                case '-': op_num++;     //6
                case '+': op_num+=5;    //5
                    gprev_op = LOW_OP; 
                    calculate(&stack_op, &stack_num, c);
                    break;

                case '(':               //0 
                    break;
                       
                default:
                    exit(0);
            }    
            if (c == '=') {
                calculate(&stack_op, &stack_num, c);
                if (get_stack(&stack_num, &temp))
                    printf("Print: %lld\n",temp);
            } else if (c != ')' ) {
                push(&stack_op, op_num);
//printf("push op:%c\n",c);
//dump_stack(&stack_num, &stack_op);                
            }    
            pre_state = STATE_OP;
        }
    }
}
