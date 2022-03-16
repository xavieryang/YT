#include <stdio.h>
#include <stdlib.h>

typedef struct _USB {
    long long int id;
    long long int group;
    USB *prev;
    USB *next;
} USB;

typedef struct _ROOM {
    long long int room_no;
    int state;  //open(1) or close(0)
    USB *first;
    USB *last;
} ROOM;

int Room_add_user(ROOM *r, USB *u){
    //if r->first 不存在
        // init r->first = r->last = u
        // return

    //Get user from r->first, loop 
    // Loop 
        // if user-> group, group_match = 1
        // if group_match == 1,
            // if find user->group != group. 
                // append the USB before the first unmatch

    // if loop until tail, no found
    // Add USB into the tail, update r->last        
}

int Room_leave_user(ROOM *r, USB *u)
{
    // Get User u from r->last
     
    // Change the second last USB's next to null
    // Change r->last
    
}

int Room_go_user(ROOM *r, USB *u)
{
    // Get User u from r->first
     
    // Change the second USB's prev to null
    // Change r->first
    
}

int Room_close(ROOM *r, ROOM *r1)
{
    // Init a pointer USB *u
    // Loop
        // Find and Split the last user by Room_leave_user(r, u)
        // Append user to Room (r1) by Room_add_user(r1, u)

    // Mark Room r->state = 0  // closed
}

int main()
{
    //scanf M,N,K
    // Init Room stucture (M). 

    // scanf N line
    //Loop scan command
        //if command = "Enter" then
            // scan Id, Group, Room 
            // Init a USB structure and fill ID and Group
            // Append USB to Room Room_add_user()

        //if command = "Leave"       
            // scan Room (r)
            // Find and Split the last user by Room_leave_user(ROOM *r, USB *u)
            // Free u

        //if command = "Go"
            // scan Room (r)
            // Find and Split the first user by Room_Go_user(ROOM *r, USB *u)  
            // Free u
            
        // if command = "close"
            // scan Room (r)
            // Find the nearest smaller open room_no (r1)
            // Room_Close(r, r1)

    // Output resule
    //  Loop Room (0 to M-1)
        // Print USB

}
