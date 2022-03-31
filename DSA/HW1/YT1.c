#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define XOR(a, b)  (USB *)((unsigned long long int)(a)^(unsigned long long int)(b))

typedef struct _USB {
    long long int uid;
    int gid;
    struct _USB *link;
} USB;

typedef struct _GROUP {
    int gid;
    struct _GROUP *next;
    struct _GROUP *prev;
    USB *first_usr;
    USB *last_usr;
} GROUP;

typedef struct _ROOM{
    int state;
    GROUP** index;
    GROUP *first_grp;
    GROUP *last_grp;  
} ROOM;

USB *USB_init(long long int uid,int gid)
{
    USB *u=malloc(sizeof(USB));
    if (!u) { printf("OOM\n"); exit(0);}
    u->gid=gid;
    u->uid=uid;
    u->link=NULL;
    return u;
}

GROUP *GROUP_Init(USB *u)
{
    GROUP *g=malloc(sizeof(GROUP));
    g->gid=u->gid;
    g->first_usr=u;
    g->last_usr=u;
    g->prev=NULL;
    g->next=NULL;
    return g;
} 

void Room_add_user(ROOM *r,  USB *u)
{
    GROUP *g, *gptr;
    USB *u_ptr;

    g=r->index[u->gid];
    if (g) {
        u_ptr = g->last_usr;
        if (u_ptr) {
            u_ptr->link= XOR(u_ptr->link, u);
            u->link = u_ptr; // = XOR(u_ptr, NULL)
            g->last_usr = u;
        } else {
            g->first_usr = g->last_usr = u;
            u->link = 0;
        }
        return;
    }
    //no g
    g = GROUP_Init(u);
    if ((gptr=r->last_grp)!=NULL) {
        gptr->next=g;
        g->prev=gptr; 
        g->next=NULL;
        r->last_grp=g;
        r->index[g->gid]=g;
    }else{
        r->first_grp = g;
        r->last_grp =  g;
        r->index[g->gid]=g;
    }
}

void Room_go_user(ROOM *r)
{
    GROUP *g=r->first_grp;
    USB *u=g->first_usr, *u2=u->link;

    if(u2){
        u2->link=XOR(u2->link,u);
        g->first_usr=u2;
        free(u);
    }else{
        GROUP *g2=g->next;
        if (g2) {
            g2->prev=NULL;
            r->first_grp=g2;
        } else {
            r->first_grp = r->last_grp = NULL;
        }
        r->index[g->gid]=0;
        free(u);
        free(g);
    }
}

void Room_leave_user(ROOM *r)
{
    GROUP *g=r->last_grp;
    USB *u=g->last_usr, *u2=u->link;
    if(u2){
        u2->link=XOR(u2->link,u);
        g->last_usr=u2;
        free(u);
    }else{
        GROUP *g2=g->prev;
        if (g2) {
            g2->next=NULL;
            r->last_grp=g2;
        } else {
            r->first_grp = r->last_grp = NULL;
        }
        r->index[g->gid]=0;
        free(u);
        free(g);
    }

}

void Room_close(ROOM *r, ROOM *rnew)
{
    GROUP *g, *gnew, *gptr;
    USB *u, *unew;

    g=r->last_grp;
    while (g) {
        gnew = rnew->index[g->gid];
        if (gnew) {
            u = g->last_usr;
            unew = gnew->last_usr;
            u->link = XOR(u->link, unew);
            unew->link = XOR(unew->link, u);
            gnew->last_usr = g->first_usr;
        } else {
            gnew = malloc(sizeof (GROUP));
            gnew->gid = g->gid;
            gnew->next = NULL;
            gnew->prev = rnew->last_grp;
            if (rnew->last_grp) 
                rnew->last_grp->next = gnew;
            else
                rnew->first_grp =gnew;    
            rnew->last_grp =gnew;
            rnew->index[gnew->gid] = gnew;
            gnew->first_usr = g->last_usr;
            gnew->last_usr = g->first_usr;
        }
        gptr=g;
        g=g->prev;        ///
        r->index[gptr->gid]=0;
        free(gptr);
    }
    r->first_grp = r->last_grp = NULL;
    r->state = 0;
}

void Room_print_user(ROOM *r)
{
    GROUP *g = r->first_grp;
    USB *u1, *u2, *u3;
    int first=1;

    while (g) {
        u2=g->first_usr;
        u1=NULL;
        while (u2) {
            if (!first) 
                printf(" ");
            first =0;
            printf("%lld",u2->uid);
            u3=XOR(u1,u2->link);
            u1=u2;
            u2=u3;
        }
        g=g->next;
    }
    printf("\n");
}

int main()
{

    int M, N, K;
    int gid, rno;
    long long int uid;
    USB *u;
    
    scanf("%d%d%d", &M, &N, &K);
    ROOM Room[M];

    for (int i=0;i<M;i++) {
        Room[i].state = 1;
        Room[i].first_grp = NULL; 
        Room[i].last_grp = NULL;
        Room[i].index = malloc(sizeof(GROUP**)*1000000/M);
        memset(Room[i].index,0,sizeof(GROUP**)*1000000/M);
    }

    char cmd[8];
    int new;

    for(int i=0; i<N; i++){
        scanf("%s",cmd);
        switch(cmd[0]){
            case 'e':
                scanf("%d%lld%d", &gid, &uid, &rno);
                u=USB_init(uid, gid);
                Room_add_user(&Room[rno], u);
                break;
            case 'g':
                scanf("%d",&rno);
                Room_go_user(&Room[rno]);
                break;
            case 'l':
                scanf("%d",&rno);
                Room_leave_user(&Room[rno]);
                break;
            case 'c':
                scanf("%d",&rno);
                for(new=(rno+M-1);new>rno;new--)
                    if(Room[new%M].state==1)  ///
                        break;
//printf("Close %d, to %d\n", rno, new%M);                        
                Room_close(&Room[rno],&Room[new%M]); ///
                break;
        }
    }

    for (int i=0; i<M; i++) {
        Room_print_user(&Room[i]);
    }
}   
