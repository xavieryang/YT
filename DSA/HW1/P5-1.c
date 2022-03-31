#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _USB {
    long long int id;
    long long int gid;
    struct _USB *prev;
    struct _USB *next;
} USB;

typedef struct _GROUP {
    long long int   gid;
    struct _USB     *first_u;
    USB             *last_u;
    struct _GROUP   *prev;
    struct _GROUP   *next;
} GROUP;

typedef struct _FGROUP{
    GROUP           *g;
    struct _FGROUP  *next;
} FASTGROUP;

#define PRIME       97
#define HASH(x)     (x%PRIME)

typedef struct _ROOM {
    int state;  //open(1) or close(0)
    GROUP *first_g;
    GROUP *last_g;
    FASTGROUP *hash[PRIME];
    int no;
} ROOM;

USB *Init_USB(long long int gid, long long int id)
{
    USB *u = malloc(sizeof(USB));
    u->gid = gid;
    u->id = id;
    u->prev = u->next = NULL;

    return u;
}

GROUP *Init_Group(USB *u)
{
    GROUP *g = malloc(sizeof(GROUP));
    g->gid= u->gid;
    g->first_u = g->last_u = u;
    g->prev = g->next = NULL;

    return g;
}

GROUP *Room_find_fastgroup(ROOM *r, long long int gid)
{
    FASTGROUP *fg;    
    fg = r->hash[HASH(gid)];
    while (fg) {
        if (fg->g->gid == gid) 
            return fg->g;
        fg=fg->next;    
    }
    return NULL;
}

void Room_add_fastgroup(ROOM *r, GROUP *g)
{
    FASTGROUP *fg, *newfg;

    fg = r->hash[HASH(g->gid)];
    newfg = malloc(sizeof(FASTGROUP));
    newfg->g = g;
    newfg->next = fg;
    r->hash[HASH(g->gid)] = newfg;
    return;
}

void Room_remove_fastgroup(ROOM *r, GROUP *g)
{
   FASTGROUP *fg, *nextfg;

    fg = r->hash[HASH(g->gid)];
    nextfg = fg->next;
    if (fg->g == g) {
        r->hash[HASH(g->gid)] = nextfg;
        free(fg);
        return;
    }
    while (nextfg) {
        if (nextfg->g == g) {
            fg->next = nextfg->next;
            free(nextfg);
            return;
        }
        fg=nextfg;
        nextfg=fg->next;
    }
    return;
}

void Room_add_user(ROOM *r, USB *u)
{
    GROUP *g, *gptr;
    USB   *ptr;

    //check group
    g = Room_find_fastgroup(r, u->gid);
    if (g != NULL) {
        ptr=g->last_u;
        ptr->next = g->last_u = u;
        u->prev = ptr;
        u->next = NULL;
        return;
    }

    g = Init_Group(u);
    if (r->first_g == NULL) { 
        r->first_g = r->last_g = g;
    } else {
        gptr=r->last_g;
        gptr->next = r->last_g = g;
        g->prev = gptr;
        g->next = NULL;
    }
    g->first_u = g->last_u = u;
    Room_add_fastgroup(r, g);
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
        Room_remove_fastgroup(r, g);
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
        Room_remove_fastgroup(r, g);
        free(g);
    } else {
        g->first_u = next_u;
        next_u->prev = u->next =u->prev = NULL;
    }

    return u;
}
/*
void Room_close(ROOM *r, ROOM *r1)
{
    USB *u;
//printf("Room_close form %d to %d\n",r->no, r1->no);
    while ((u = Room_leave_user(r)) != NULL)
        Room_add_user(r1, u);
    
    r->state = 0;  //close
}
*/
void Room_close(ROOM *r1, ROOM *r2)
{
    USB *u1, *u2, *u, *uptr;
    GROUP *g1, *g2, *gptr;

    g1=r1->last_g;
    while (g1!=NULL) {
        
        u1=g1->last_u;
        g2 = Room_find_fastgroup(r2, g1->gid);
        if (g2==NULL){

            g2 = malloc(sizeof(GROUP));
            g2->gid= g1->gid;
            if (r2->first_g == NULL)  
                r2->first_g = r2->last_g = g2;
            g2->prev = r2->last_g;
            g2->next = r2->last_g->next;
            r2->last_g->next = g2;
            r2->last_g = g2;
            Room_add_fastgroup(r2, g2);
            uptr = u1;
            u1 = u1->next;
            g2->first_u = g2->last_u = uptr;
            uptr->prev = uptr->next = NULL;
        }
 
        u2=g2->last_u;
        while (u1!=NULL) {
 
            uptr = u1;   
            u1 = u1->prev;
            if (u1)
                u1->next = NULL;
            uptr->prev = u2;
            u2 = u2->next = uptr;
        }

        g2->last_u=u2;
        gptr=g1;
        g1=g1->prev;
        free(gptr);         
    }

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
    long long int i, j; 
    int m, r1, r2, loop;
    USB *u;

    scanf("%d%d%d",&M,&N,&K);  //Room, Lines and Group. 

    ROOM BRoom[M];  
    for (loop=0; loop< M; loop++) {
        BRoom[loop].first_g=BRoom[loop].last_g=NULL;
        BRoom[loop].state=1;
        BRoom[loop].no = loop;
        for (int h=0; h<PRIME;h++)
            BRoom[loop].hash[h]=NULL;
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
