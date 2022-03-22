#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _USB {
    long long int id;
    long long int group;
    struct _USB *prev;
    struct _USB *next;
} USB;

typedef struct _GROUP {
    long long int group;
    struct _USB     *first;
    USB             *last;
    struct _GROUP   *prev;
    struct _GROUP   *next;
} GROUP;

typedef struct _ROOM {
    int state;  //open(1) or close(0)
    GROUP *first;
    GROUP *last;
    int no;
} ROOM;

void Room_add_user(ROOM *r, USB *u)
{
    USB *ptr, *prev_ptr ;
    int gmatch = 0;

//printf("Room_add_user id:%d group:%d\n",u->id,u->group); 
    if (r->first == NULL) {
        r->first = r->last = u;
        return;
    }
    ptr=r->first;
    while (ptr != NULL) {
        if (ptr->group==u->group) 
            gmatch=1;
        if ((ptr->group!=u->group)&&(gmatch==1)) {
            prev_ptr->next = ptr->prev = u;
            u->next = ptr;
            u->prev = prev_ptr;
            return;
        }
        prev_ptr = ptr;
        ptr = ptr->next;
    }
    prev_ptr->next = r->last = u;
    u->prev = prev_ptr;
    u->next = NULL;
}

USB *Room_leave_user(ROOM *r)
{
    USB *ptr, *prev_ptr;
    
    ptr = r->last;
    if (ptr == NULL)
        return NULL;

    prev_ptr = ptr->prev;
    if (prev_ptr) {
        prev_ptr->next = NULL;
        r->last = prev_ptr;
    } else {
        r->last = r->first = NULL;
    }
//printf("Room_leave_user id:%d group:%d\n",ptr->id,ptr->group);    
    return ptr;
}

USB *Room_go_user(ROOM *r)
{
    USB *ptr, *next_ptr;
    
    ptr = r->first;
    if (ptr==NULL)
        return NULL;

    next_ptr = ptr->next;
    if (next_ptr) {
        next_ptr->prev = NULL;
        r->first = next_ptr;
    } else {
        r->last = r->first = NULL;
    }
    return ptr;
}

void Room_close(ROOM *r, ROOM *r1)
{
    USB *u;
//printf("Room_close form %d to %d\n",r->no, r1->no);
    while ((u = Room_leave_user(r)) != NULL)
        Room_add_user(r1, u);
    
    r->state = 0;  //close
  
}

void Room_print(ROOM *r)
{
    USB *u = r->first;
    
    while (u) {
        printf("%lld",u->id);
        if ((u = u->next)!=NULL)
            printf(" ");
    }
    printf("\n");

}

USB *Init_USB(long long int i, long long int j)
{
    USB *u = malloc(sizeof(USB));
    u->group = i;
    u->id = j;
    u->prev = u->next = NULL;
    return u;
}

int main()
{
    int M,N,K;
    char cmd[32];
    long long int i, j; 
    int m, r1, r2, loop;
    USB *u;

    scanf("%d%d%d",&M,&N,&K);  //Room, Lines and Group. 

    ROOM BRoom[M];  
    for (loop=0; loop< M; loop++) {
        BRoom[loop].first=BRoom[loop].last=NULL;
        BRoom[loop].state=1;
        BRoom[loop].no = loop;
    }

    for (loop=0; loop< N; loop++) {
        scanf("%s",cmd);

        if (!strcmp(cmd,"enter")) {
            scanf("%lld%lld%d",&i,&j,&m);
            u = Init_USB(i,j);
            Room_add_user(&BRoom[m], u);
        }

        if (!strcmp(cmd,"leave")) {
            scanf("%d",&m);
            u = Room_leave_user(&BRoom[m]);
            free(u);
        }

        if (!strcmp(cmd,"go")) {
            scanf("%d",&m);
            u = Room_go_user(&BRoom[m]);
            free(u);
        }

        if (!strcmp(cmd,"close")) {
            scanf("%d",&m);
            for (int step=1; step < M; step++) {
                r1 = (M+m-step)%M;
                if (BRoom[r1].state==1) {
                    Room_close(&BRoom[m], &BRoom[r1]);
                    break;
                }  
            }
        }
    }
    for (loop=0;loop<M;loop++)
        Room_print(&BRoom[loop]);
}
