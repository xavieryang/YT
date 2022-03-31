#define LEN 256
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct _complex {
    int len;
    int digit[LEN];
} complex;

void output(complex *ans)
{
    int i;

    for (i=ans->len-1; i>=0; i--)
        printf("%d",ans->digit[i]);
    printf("\n");    
}

int is_zero(complex *m)
{
    if (m->len==0)
        return 1;
    return 0;
}

int is_even(complex *m)
{
    if (m->digit[0] %2 == 0)
        return 1;
    return 0;
}

void do_div(complex *m, complex *div)
{
    int i, new_len=0;
    int tmp,res = 0;
    int d=0;

    for (i=0;i<div->len;i++) {
        d = d*10;
        d += div->digit[i];
    }
    for (i=m->len-1;i>=0;i--) {
        tmp = (m->digit[i]+res*10) % d; 
        m->digit[i] = (m->digit[i]+res*10) / d;
        if ((m->digit[i]>0) && (new_len==0))
            new_len=i+1;
        res = tmp;
    }
    m->len=new_len;

}

void do_mul(complex *out, int power)
{
    int shift,i, mul=0;
    int tmp,res;

    for (shift=0 ; shift < power; shift++) {
        res = 0;
        for (i=0; i< out->len; i++) {
            tmp = res + out->digit[i] * 2;
            out->digit[i] = tmp % 10;
            res = tmp / 10;
        }
        while (res > 0){
            out->digit[out->len++] = res % 10;
            res = res / 10;
        }
    }
}
        
void swap (complex *n, complex *m) //let n<m
{
    int i,max;
    int tmp;

    max = (m->len > n->len)?m->len:n->len;
    for(i=0;i<max;i++) {
        tmp = n->digit[i];
        n->digit[i] = m->digit[i];
        m->digit[i] = tmp;
    }
    tmp = n->len;
    n->len = m->len;
    m->len = tmp;
    
}

void do_sub(complex *n, complex *m)   //m=m-n
{
    int i;
    int new_len=0,lend=0;

    for (i=0;i<m->len;i++) {
        if ((m->digit[i] - lend) < n->digit[i]) {
            m->digit[i] = m->digit[i] + 10 - lend - n->digit[i];
            lend = 1;
        } else {
            m->digit[i] = m->digit[i] - lend - n->digit[i];
            lend = 0;
        }
    }
    for (i=m->len-1;i>=0;i--) {
        if (m->digit[i]>0) {
            new_len=i+1;
            break;
        } 
    }
    m->len = new_len;
   
}// m=m-n

int cmp(complex *n, complex *m)  
{
    int i;

    if(n->len > m->len)
        return 1;
    if(n->len < m->len)
        return -1;
    for(int i= n->len-1; i>=0; i--){
        if (n->digit[i] > m->digit[i])
            return 1;
        if (n->digit[i] < m->digit[i])
            return -1;
    }
    return 0;
}

int main() {
    char c;
    complex m, n, ans, com_2;
    int i,tmp,flag=0;
    int compare;
    int gcd_2power = 0;

    m.len=n.len=0;
    ans.len=1;
    ans.digit[0]=1;
    com_2.digit[0]=2;
    com_2.len = 1;
    for (i=0;i<LEN;i++) {
        m.digit[i] = n.digit[i] = 0;
    }
    while(scanf("%c",&c)!=EOF){
        if(!isdigit(c)){
            flag=1;
            continue;
        }
        if(flag==0) {
            m.digit[m.len++]=c-'0';
        } else {
            n.digit[n.len++]=c-'0';
        }
    }
    for(int i=0; i<m.len/2; i++) {  //reverse
        tmp = m.digit[i];
        m.digit[i] = m.digit[m.len-1-i];
        m.digit[m.len-1-i] = tmp;
    }
    for(int i=0; i<n.len/2; i++) { //reverse
        tmp = n.digit[i];
        n.digit[i] = n.digit[n.len-1-i];
        n.digit[n.len-1-i] = tmp;
    }

    while (!is_zero(&m) && !is_zero(&n)) {
        if (is_even(&m) && is_even(&n)) {
            do_div(&m,&com_2);
            do_div(&n,&com_2);
            gcd_2power++;
        } else if (is_even(&m)) {
            do_div(&m,&com_2);
        } else if (is_even(&n)) {
            do_div(&n,&com_2);
        } 
        compare = cmp(&n, &m);
        if (compare > 0) {
            swap (&n,&m); //let n<m
        }
        do_sub(&n,&m); // m=m-n.
    }
 
    do_mul(&n,gcd_2power);
    output(&n);
}
