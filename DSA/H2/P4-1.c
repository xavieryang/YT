#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct _entry {
    int num;
    char op;
} ENTRY;

typedef struct _stack {
    ENTRY s[10000];
    int top=-1;
} STACK;

STACK store, out;

ENTRY* pop (STACK *s)
{
    ENTRY *ret;

    if (s->top>=0) {
        ret=s->
        gTop--;
    } else
        s=NULL;
}

ENTRY push(ENTRY *s) 
{
    gTop++;
    gSTACK[gTop].num = s->num;
    gSTACK[gTop].op = s->op;
}

int main()
{


}