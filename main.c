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


struct Event {
    int eventNum;
    int time;
    int eventType;
    struct Event* nextPQ;
    struct Event* nextFIFO;

};

struct FIFOQueue {    //FIFO
    int totalJobs;
    int totalsize;
    int occupied;
    struct Event *front, *back;
};

struct Event* newEvent(int number)
{
    struct Event* temp = (struct Event*)malloc(sizeof(struct Event));
    temp->eventNum = number;
    temp->nextPQ = NULL;

}



int main() {
    printf("");
    return 0;
}
