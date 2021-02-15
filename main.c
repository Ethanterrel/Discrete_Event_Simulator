#define MAX 60000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int randNum(int min, int max)
{

    int random = (rand() %
      (max - min + 1)) + min;
    return random; // Helping Generate # for seed
}

enum eventtype {  narrival,nfinish,parrival,cpuarrival,cpufinish,exitsys,d1arrival,d2arrival,d1finish,d2finish,
    pfinish
};


struct event { int time;
    enum eventtype
            type;
    int id;
};

int id()
{static int count; count++;
    return count;
}

//struct event newEvent();


void enqueue(int n, int ID); int dequeue(int n); void pqSort();

void pEnqueue(struct event newEvent);
struct event pDequeue();

int SEED, INIT_TIME,
        FIN_TIME, ARRIVE_MIN,
        ARRIVE_MAX, QUIT_PROB,
        NETWORK_PROB, CPU_MIN,
        CPU_MAX,DISK1_MIN,         //All the config names
DISK1_MAX, DISK2_MIN,
        DISK2_MAX, NETWORK_MIN,
        NETWORK_MAX;

bool cpuBusy, d1Busy, d2Busy, netBusy; // boolean true or false to see if certain jobs are busy

