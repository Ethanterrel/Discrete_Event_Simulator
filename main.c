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

double cpuAvg, cpuUtil, cpuCount, cpuResponse, cpuThroughput;
double d1Avg, d1Util, d1Count, d1Response, d1Throughput;
double d2Avg, d2Util, d2Count, d2Response, d2Throughput;
double netAvg, netUtil, netCount, netResponse, netThroughput;
int cpuMax = 0, d1Max = 0, d2Max = 0, netMax = 0;
int cpuTotal, d1Total, d2Total, netTotal;
int cpuMaxResponse = 0, d1MaxResponse = 0, d2MaxResponse = 0, netMaxResponse = 0;

struct event pq[MAX];
int pFront = -1, pRear = -1;

FILE *logFile,
        *statsFile;


int main() {

    logFile = fopen("log.txt", "w");
    statsFile = fopen("stats.txt", "w");
    FILE *file = fopen("config.txt", "r");

    if(file == NULL) {
        printf("could not open file\n");
        exit(1);
    }

    fscanf(file, "SEED %d INIT_TIME "
                 "%d FIN_TIME %d ARRIVE_MIN %d "
                 "ARRIVE_MAX %d QUIT_PROB %d"
                 " NETWORK_PROB %d CPU_MIN %d CPU_MAX"
                 " %d DISK1_MIN %d DISK1_MAX %d DISK2_MIN"
                 " %d DISK2_MAX %d NETWORK_MIN %d NETWORK_MAX %d",
           &SEED,
           &INIT_TIME,
           &FIN_TIME, &ARRIVE_MIN,
           &ARRIVE_MAX, &QUIT_PROB, &NETWORK_PROB,
           &CPU_MIN, &CPU_MAX, &DISK1_MIN, &DISK1_MAX,
           &DISK2_MIN, &DISK2_MAX, &NETWORK_MIN,
           &NETWORK_MAX);
