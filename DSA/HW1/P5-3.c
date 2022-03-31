#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _USB {
    long long int id;
    int         gid;
    struct _USB *prev;
    struct _USB *next;
} USB;

typedef struct _GROUP {
    int   gid;
    struct _USB     *first_u;
    USB             *last_u;
    struct _GROUP   *prev;
    struct _GROUP   *next;
} GROUP;

typedef struct _ROOM {
    int state;  //open(1) or close(0)
    int no;
    GROUP *first_g;
    GROUP *last_g;
    GROUP **index;
} ROOM;

USB *Init_USB(long long int id, int gid)
{
    USB *u = malloc(sizeof(USB));
    u->id = id;
    u->gid = gid;
    u->prev = u->next = NULL;
    return u;
}

GROUP *Init_Group(int gid)
{
    GROUP *g = malloc(sizeof(GROUP));
    g->gid = gid;
    g->prev = g->next = NULL;
    return g;
}

GROUP *Room_find_group(ROOM *r, int gid)
{
    GROUP *g;

    g = r->index[gid];
    return g;
}

void Room_add_group(ROOM *r, GROUP *g)
{
    r->index[g->gid] = g;
    return;
}

void Room_remove_group(ROOM *r, GROUP *g)
{
    r->index[g->gid] = NULL;
    return;
}

void Room_add_user(ROOM *r, USB *u)
{
    GROUP *g, *gptr;
    USB   *ptr;

    //check group
    g = Room_find_group(r, u->gid);
    if (g != NULL) {
        ptr=g->last_u;
        ptr->next = g->last_u = u;
        u->prev = ptr;
        u->next = NULL;
        return;
    }

    g = Init_Group(u->gid);
    g->first_u = g->last_u = u;
    g->gid = u->gid;
    if (r->first_g == NULL) { 
        r->first_g = r->last_g = g;
    } else {
        gptr=r->last_g;
        gptr->next = r->last_g = g;
        g->prev = gptr;
        g->next = NULL;
    }
    Room_add_group(r, g);
    return ;
}

USB *Room_leave_user(ROOM *r)
{
    GROUP *g, *prev_g;
    USB   *u, *prev_u;
    
    /* no illegal leave, g and u exist. */
    if (((g = r->last_g)==NULL) || ((u = g->last_u)==NULL))
        return NULL;

    //remove u from g
    if ((prev_u = u->prev) == NULL) {
        if ((prev_g = g->prev) == NULL) {
            r->first_g = r->last_g = NULL;
        } else {    
            r->last_g = prev_g;
            prev_g->next = NULL;
        }    
        Room_remove_group(r, g);
        free(g);
    } else {
        g->last_u = prev_u;
        prev_u->next = u->next =u->prev = NULL;
    }
    return u;
}

USB *Room_go_user(ROOM *r)
{
    GROUP *g, *next_g;
    USB   *u, *next_u;
    
    /* no illegal leave, g and u exist. */
    if (((g = r->first_g)==NULL) || ((u = g->first_u)==NULL))
        return NULL;
    
    //remove u from g
    if ((next_u = u->next) == NULL) { // turn to next group
        if ((next_g = g->next) == NULL) {
            r->first_g = r->last_g = NULL;
        } else {    
            r->first_g = next_g;
            next_g->prev = NULL;
        }    
        Room_remove_group(r, g);
        free(g);
    } else {
        g->first_u = next_u;
        next_u->prev = u->next =u->prev = NULL;
    }

    return u;
}

void Group_move( GROUP *g1, GROUP *g2)
{
    USB *u1, *u2, *uptr;

    u1=g1->last_u;
    u2=g2->last_u;
    
    while (u1) {
        uptr = u1->prev;
        if (u2==NULL) {
            g2->first_u = g2->last_u = u1;
            u1->prev = u1->next = NULL;
            
        } else {
            u1->next = NULL;
            u1->prev = u2;
            u2->next = u1;    
        }
        u2 = u1;
        u1 = uptr;
    }
    g2->last_u = u2;

}

void Room_close(ROOM *r1, ROOM *r2)
{
    USB *u1;
    GROUP *g1, *g2, *gptr;

    g1=r1->last_g;
//printf("1\n");    
    while (g1!=NULL) {
//printf("2\n");
        g2 = Room_find_group(r2, g1->gid);
        if (g2==NULL){
            g2 = Init_Group(g1->gid);
            if (r2->first_g == NULL) {  
                r2->first_g = r2->last_g = g2;
            } else {    
                g2->prev = r2->last_g;
                g2->next = NULL;
                g2->first_u = g2->last_u = NULL;
                r2->last_g->next = g2;
                r2->last_g = g2;
            }
            Room_add_group(r2, g2);
        }
//printf("22\n");
        Group_move(g1,g2);
        gptr = g1;
        g1=g1->prev;
//printf("23\n");
        Room_remove_group(r1, gptr);
        free(gptr);         
    }
//printf("3\n");
    r1->state = 0;
}

void Room_print(ROOM *r)
{
    GROUP *g = r->first_g;
    USB *u;
    
    while (g && r->state) {
        u = g->first_u;
        while (u) {
            printf("%lld",u->id);
            if ((u = u->next)!=NULL)
                printf(" ");
        }
        if ((g = g->next)!=NULL)
            printf(" ");                
    }
    printf("\n");
}


int main()
{
    int M,N,K;
    char cmd[32];
    long long int j; 
    int i, m, r1, r2, loop;
    USB *u;

    scanf("%d%d%d",&M,&N,&K);  //Room, Lines and Group. 

    ROOM BRoom[M];  
    for (loop=0; loop< M; loop++) {
        BRoom[loop].first_g=BRoom[loop].last_g=NULL;
        BRoom[loop].state=1;
        BRoom[loop].no = loop;
        BRoom[loop].index = malloc(sizeof(GROUP*)*1000000/M);
        memset(BRoom[loop].index,0,sizeof(GROUP*)*1000000/M);
    }

    for (loop=0; loop< N; loop++) {
        scanf("%s",cmd);

        if (!strcmp(cmd,"enter")) {
            scanf("%d%lld%d",&i,&j,&m);
            u = Init_USB(j,i);
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
