#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define SEED 1
#define INIT_TIME 0
#define FIN_TIME 10000
#define ARRIVE_MIN 10
#define ARRIVE_MAX 10
#define QUIT_PROB .1
#define NETWORK_PROB .1
#define CPU_MIN 50
#define CPU_MAX 55
#define DISK1_MIN 30
#define DISK1_MAX 25
#define DISK2_MIN 15
#define DISK2_MAX 35
#define NETWORK_MIN 30
#define NETWORK_MAX 50


//FIFO DATA

struct FIFOQueue {
    int numofjobs;
    int totalsize;
    int useddata;
     struct Event
     *front, *back;
}
;


struct Event {    // Start of LL
    int eventNum;
    int time;
    int eventType;
    struct Event*
     nextPQ;
    struct Event*
      nextFIFO;

};

struct Event* newEvent(int number)
{
    struct Event* temp =
   (struct Event*)malloc(sizeof(struct Event));
    temp->eventNum = number;
    temp->nextPQ = NULL;

}

struct FIFOQueue* createQueue()   //Creating empty FIFO
{
    struct FIFOQueue* que =
     (struct FIFOQueue*)malloc(sizeof(struct FIFOQueue));
    que->front = que->back = NULL;
    que->totalsize = 0;
    que->numofjobs = 0;
    return que;
}


// The function to add an event to a FIFOQueue
void add(struct FIFOQueue* quelength, struct Event* event)
{

    // If FIFOQueue is empty, then new node is front and rear both
    if (quelength->back == NULL) {
        quelength->front = event;
        quelength->back = event;
        quelength->totalsize =1;
        quelength->numofjobs =
       (quelength->numofjobs)+1;
        return;
    }

    if(quelength->front == NULL) {
        quelength->front =
        event;
        quelength->back =
        event;
        quelength->totalsize = 1;
    }

    quelength->totalsize = quelength->totalsize +1;   // add new node to back of FIFO queue
    quelength->back->nextFIFO = event;
    quelength->back = event;
    quelength->numofjobs = (quelength->numofjobs)+1;
}


// Function to remove a key from given FIFOQueue q
struct Event* removeQ(struct FIFOQueue* q)
{
    if (q->front == NULL) {
        return NULL;
    }
    struct Event* temp = q->front;
    q->front = q->front->nextFIFO;
    q->totalsize = q->totalsize -1;
    if (q->front == NULL) {
        q->back = NULL;
        q->totalsize = 0;
    }
    return temp;
}


// Priority DATA

struct PQueue {
    struct Event *front, *back;
};

//creating empty Que again
struct PQueue* createPQueue()
{
    struct PQueue* q =
    (struct PQueue*)malloc(sizeof(struct PQueue));
    q->front = q->back = NULL;
    return q;
}


void addPQ(struct PQueue* q, struct Event* event)
{

    if((q->front == NULL) && (q->back == NULL)){   // If empty, create new queue in front and back
        q->front = q->back = event;
        return;
    }

    // If the queue is not empty, then add the new event to the queue based off the time
    int sorted = 1;
    int timeOfNew = event->time;


    struct Event* placement = q->front;

    int loop = 0;
    while(sorted == 1) {
        loop++;
        if(placement->time < timeOfNew) {
            if(q->back == placement)  {
          placement->nextPQ = event;
           q->back =event;
           sorted = 0;
            return;
            }
            if(placement->nextPQ->time >= timeOfNew) {
                struct Event* temp = placement->nextPQ;
                placement->nextPQ = event;
                event->nextPQ = temp;
                sorted = 0;
                return;
            }
            sorted = 1;
            placement = placement->nextPQ;
            continue;
        }
        if(placement->time == timeOfNew) {
            if(q->back != placement) {
                struct Event* temp = placement->nextPQ;
                placement->nextPQ = event;
                event->nextPQ = temp;
                sorted = 0;
                return;
            }
            else {
                placement->nextPQ = event;
                q->back = event;
                sorted = 0;
                return;
            }
        }
        //pretty sure this is unnecessary but...
        if(placement->time > timeOfNew) {  //
            struct Event* temp = placement;//
            q->front = event;//
            event->nextPQ = temp;//
            sorted = 0;//
            return;//
        }
    }

}

struct Event* removePQ(struct PQueue* q)
{
    if (q->front == NULL) {
        return NULL;
    }
    struct Event* temp = q->front; // move previous queue point one node again
    q->front = q->front->nextPQ;

    if (q->front == NULL) {
        q->back = NULL;
    }
    return temp;
}


// creation of log process .txt file

FILE* createLogFile(char* name) {
    FILE *logFile = fopen(name, "w");
    if(logFile == NULL) {
        printf("Unable to create log file.\n");
        exit(1);
    }
    else {
        return logFile;
    }
}
