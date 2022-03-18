#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _USB {
    long long int id;
    long long int group;
    struct _USB *prev;
    struct _USB *next;
} USB;

typedef struct _ROOM {
    int state;  //open(1) or close(0)
    USB *first;
    USB *last;
} ROOM;

void Room_add_user(ROOM *r, USB *u)
{
    USB *ptr, *prev_ptr ;
    int gmatch = 0;

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
    u->next = NULL;
}

USB *Room_leave_user(ROOM *r)
{
    USB *ptr, *prev_ptr;
    
    ptr = r->last;
    prev_ptr = ptr->prev;

    if (prev_ptr) {
        prev_ptr->next = NULL;
        r->last = prev_ptr;
    } else {
        r->last = r->first = NULL;
    }
    return ptr;
}

USB *Room_go_user(ROOM *r)
{
    USB *ptr, *next_ptr;
    
    ptr = r->last;
    next_ptr = ptr->next;

    if (next_ptr) {
        next_ptr->prev = NULL;
        r->first = next_ptr;
    } else {
        r->last = r->first = NULL;
    }
    return ptr;
}

int Room_close(ROOM *r, ROOM *r1)
{
    // Init a pointer USB *u
    // Loop
        // Find and Split the last user by Room_leave_user(r, u)
        // Append user to Room (r1) by Room_add_user(r1, u)

    // Mark Room r->state = 0  // closed
}

void Room_print(ROOM *r)
{


}

int main()
{
    int M,N,K;
    char cmd[32];
    int i, j, m, loop;
    USB *u;

    scanf("%d%d%d",&M,&N,&K);  //Room, Lines and Group. 

    ROOM BRoom[M];  
    for (loop=0; loop< M; loop++) {
        BRoom[loop].first=BRoom[loop].last=NULL;
        BRoom[loop].state=1;
    }

    for (loop=0; loop< N; loop++) {
        scanf("%s",cmd);

        if (!strcmp(cmd,"enter")) {
            scanf("%d%d%d",&i,&j,&m);
            //printf("repeat:enter %d %d %d\n",i,j,m);
            u = Init_USB(i,j);
            Room_add_user(&BRoom[m], u);
        }

        if (!strcmp(cmd,"leave")) {
            scanf("%d",&m);
            //printf("repeat:leave %d\n",m);
            Room_leave_user(&BRoom[m]);
        }

        if (!strcmp(cmd,"go")) {
            scanf("%d",&m);
            //printf("repeat:go %d\n",m);
            Room_go_user(&BRoom[m]);
        }

        if (!strcmp(cmd,"close")) {
            scanf("%d",&m);
            //printf("repeat:close %d\n",m);
            for (int step=1; step < M/2; step++) {
                if (BRoom[(M+m-step)%M].state==1) {
                    Room_close(m, (M+m-step)%M);
                    break;
                } else if (BRoom[(M+m+step)%M].state==1) {
                    Room_close(m, (M+m+step)%M);
                    break;
                } 
            }
        }
    }
    for (loop=0;loop<M;loop++)
        Room_print(&BRoom[loop]);
}
