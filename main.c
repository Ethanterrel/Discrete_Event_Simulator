#define MAX 60000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*void read_config() {

    FILE *f = fopen("CONFIG.txt", "r");

    if(f == NULL) {
        printf("could not open file\n");
        exit(1);
    }

    // scan all values from f
    fscanf(f, "SEED %d INIT_TIME %d FIN_TIME %d ARRIVE_MIN %d ARRIVE_MAX %d QUIT_PROB %d NETWORK_PROB %d CPU_MIN %d CPU_MAX %d DISK1_MIN %d DISK1_MAX %d DISK2_MIN %d DISK2_MAX %d NETWORK_MIN %d NETWORK_MAX %d",
           &SEED, &INIT_TIME, &FIN_TIME, &ARRIVE_MIN, &ARRIVE_MAX, &QUIT_PROB, &NETWORK_PROB, &CPU_MIN, &CPU_MAX, &DISK1_MIN, &DISK1_MAX, &DISK2_MIN, &DISK2_MAX, &NETWORK_MIN, &NETWORK_MAX);

    return;

}*/


int randNum(int min, int max)
{

    int random = (rand() %
                  (max - min + 1)) + min;
    return random; // Helping Generate # for seed
}

enum eventtype {  narrival,finish,arrival,
    cpuarrival,cpufinish,exitsys,
    d1arrival,d2arrival,d1finish,d2finish, pfinish
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


void Disk1finish(struct event oldEvent);
void Disk2finish(struct event oldEvent); void arriveatNetwork(struct event oldEvent);
void finishatNetwork(struct event oldEvent); void finishprocess(struct event oldEvent);


int netQ[MAX], netFront = -1,
        netRear = -1, netSize = 0;;
int cpuQ[MAX], cpuFront = -1,
        cpuRear = -1, cpuSize = 0;
int d1Q[MAX], d1Front = -1,
        d1Rear = -1, d1Size = 0;
int d2Q[MAX], d2Front = -1,
        d2Rear = -1, d2Size = 0;

void processarrival(struct event oldEvent);
void arriveatcpu(struct event oldEvent); void finishatcpu(struct event oldEvent);
void exitsystem(struct event oldEvent); void arriveatdisk1(struct event oldEvent);
void arriveatdisk2(struct event oldEvent);

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
        printf("error opening file\n");
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


    srand(SEED);

    struct event first, last;
    first.time = INIT_TIME;
    first.type = arrival;
    first.id = id();
    last.time = FIN_TIME;
    last.type = pfinish;
    last.id = 0;
    pEnqueue(first);
    pEnqueue(last);

    bool trueorflase = true;
    struct event e;


    while(trueorflase == true) {
        e = pDequeue();
        switch(e.type) {
            case arrival:
                processarrival(e); break; case cpuarrival: arriveatcpu(e); break;
            case cpufinish: finishatcpu(e); break;
            case exitsys: exitsystem(e); break;
            case d1arrival:arriveatdisk1(e); break;
            case d2arrival:arriveatdisk2(e); break;
            case d1finish: Disk1finish(e); break;
            case d2finish: Disk2finish(e); break;
            case narrival: arriveatNetwork(e); break;
            case finish: finishatNetwork(e); break;
            case pfinish: finishprocess(e);
                fclose(logFile);
                fclose(statsFile);
                trueorflase = false;
                exit(0);
        }

    }


}

struct event newEvent()
{
    struct event newEvent;
    newEvent.time = randNum(ARRIVE_MIN, ARRIVE_MAX);
    newEvent.type = arrival;
    newEvent.id = id();
    return newEvent;
}

void enqueue(int n, int id)
{
// adds queue
    if(n == 1)
    {if(cpuRear == -1){
            cpuFront = cpuRear = 0;
            cpuQ[cpuRear] = id;

        } else if (cpuRear == MAX-1)
        {
            printf("\nfull\n");
            return;

        } else { cpuRear++; cpuQ[cpuRear] = id;
        }cpuSize++; cpuAvg += cpuSize;cpuTotal++;
        if(cpuSize > cpuMax)
        {
            cpuMax = cpuSize;
        }
    } else if (n == 2)
    { if(d1Rear == -1)
        {
            d1Front = d1Rear = 0; d1Q[d1Rear] = id;
        } else if (d1Rear == MAX-1)
        {
            printf("\n full\n");
            return;
        } else {
            d1Rear++; d1Q[d1Rear] = id;
        }
        d1Size++; d1Avg += d1Size; d1Total++;
        if(d1Size > d1Max)
        {
            d1Max = d1Size;
        }
    } else if (n == 3) {
        if(d2Rear == -1) { d2Front = d2Rear = 0; d2Q[d2Rear] = id;
        } else if (d2Rear == MAX-1) {
            printf("\nfull\n");
            return;
        } else {
            d2Rear++; d2Q[d2Rear] = id;

        }
        d2Size++; d2Avg += d2Size; d2Total++;
        if(d2Size > d2Max)
        {
            d2Max = d2Size;
        }

    } else if (n == 4)
    {
        if(netRear == -1)
        {
            netFront = netRear = 0; netQ[netRear] = id;
        } else if (netRear == MAX-1)
        {
            printf("\n full\n");
            return;
        } else {
            netRear++; netQ[netRear] = id;
        }
        netSize++; netAvg += netSize; netTotal++;
        if(netSize > netMax)
        {
            netMax = netSize;
        }
    }
}

int dequeue(int n) {
    int a; // variable to front of queue
    if(n == 1) {
        if(cpuFront == -1)
        {
            printf("\n empty\n");
            return 0;
        } else if (cpuFront == cpuRear)
        {
            a = cpuQ[cpuFront]; cpuFront = cpuRear = -1;
        } else {
            a = cpuQ[cpuFront]; cpuFront++;
        }
        cpuSize--; cpuAvg += cpuSize; cpuTotal++;
    } else if (n == 2)
    {
        if(d1Front == -1)
        {
            printf("\nempty\n");
            return 0;
        } else if (d1Front == d1Rear)
        {
            a = d1Q[d1Front]; d1Front = d1Rear = -1;
        } else{
            a = d1Q[d1Front]; d1Front++;
        }
        d1Size--; d1Avg += d1Size;d1Total++;
    } else if (n == 3) {
        if(d2Front == -1) {
            printf("\nempty\n");
            return 0;
        } else if (d2Front == d2Rear) {
            a = d2Q[d2Front]; d2Front = d2Rear = -1;
        } else {
            a = d2Q[d2Front]; d2Front++;
        }
        d2Size--; d2Avg += d2Size; d2Total++;
    } else if (n == 4) {
        if(netFront == -1) {
            printf("\nempty\n");
            return 0;
        } else if (netFront == netRear) {
            a = netQ[netFront];netFront = netRear = -1;
        } else {
            a = netQ[netFront]; netFront++;
        }
        netSize--; netAvg += netSize;netTotal++;
    }
    return a;
}

void pEnqueue(struct event newEvent)
{
    if(pRear == -1)
    {
        pFront = pRear = 0; pq[pRear] = newEvent;
    } else if (pRear == MAX-1)
    {
        printf("\nfull\n");
        return;
    } else {
        pRear++; pq[pRear] = newEvent;
    }
    pqSort();
}

struct event pDequeue()
{
    struct event e;
    if(pFront == -1)
    {
        printf("\nqueue is empty\n");
        return e;
    } else if (pFront == pRear)
    {
        e = pq[pFront]; pFront = pRear = -1;
    } else {
        e = pq[pFront]; pFront++;
    }
    return e;
}
void pqSort()
{
    struct event temp;

    for(size_t i = pFront; i <= pRear; i++)
    {
        for(size_t j = pFront; j < pRear; j++)
        {
            if(pq[j].time > pq[j+1].time)
            {
                temp = pq[j]; pq[j] = pq[j+1]; pq[j+1] = temp;
            }
        }
    }

}


void processarrival(struct event oldEvent)
{

    fprintf(logFile, "\nFor time %d the process at %d arrives to the system",
            oldEvent.time, oldEvent.id);

    if(cpuBusy == 1 || cpuSize > 0)
    {
        fprintf(logFile, "\nFor time %d the process at %d enters the CPU queue",
                oldEvent.time, oldEvent.id); enqueue(1, oldEvent.id);

    } else {
        struct event newEvent; newEvent.time = oldEvent.time; newEvent.type = cpuarrival;
        newEvent.id = oldEvent.id; cpuBusy = 1;
        pEnqueue(newEvent);
    }

    // create new event
    struct event newEvent;
    newEvent.time = oldEvent.time + randNum(ARRIVE_MIN, ARRIVE_MAX); newEvent.type = arrival;
    newEvent.id = id(); pEnqueue(newEvent);

}

void arriveatcpu(struct event oldEvent) {
    fprintf(logFile, "\nFor time %d the process at %d enters the CPU",
            oldEvent.time, oldEvent.id); struct event newEvent; int num = randNum(CPU_MIN, CPU_MAX);

    cpuUtil += num; cpuCount++;
    if (num > cpuMaxResponse) {
        cpuMaxResponse = num;
    }

    newEvent.time = oldEvent.time + num; newEvent.type = cpufinish;
    newEvent.id = oldEvent.id; pEnqueue(newEvent);
}

void finishatcpu(struct event oldEvent) {
    cpuBusy = 0;
    int num = randNum(1, 100);

    fprintf(logFile, "\nFor time %d the process at %d exits the CPU",
            oldEvent.time, oldEvent.id);

    if (num < QUIT_PROB) {
        struct event newEvent; newEvent.time = oldEvent.time; newEvent.type = exitsys;
        newEvent.id = oldEvent.id; pEnqueue(newEvent);

    } else if(num < NETWORK_PROB) {
        if(netBusy == 0) {
            struct event newEvent; newEvent.time = oldEvent.time; newEvent.type = narrival;
            newEvent.id = oldEvent.id; netBusy = 1; pEnqueue(newEvent);
        } else {
            enqueue(4, oldEvent.id);
        }
    } else if (d1Busy == 0) {
        struct event newEvent; newEvent.time = oldEvent.time; newEvent.type = d1arrival;
        newEvent.id = oldEvent.id; d1Busy = 1; pEnqueue(newEvent);

    } else if (d2Busy == 0) { // if disk2 is not occupied
        struct event newEvent; newEvent.time = oldEvent.time; newEvent.type = d2arrival;
        newEvent.id = oldEvent.id; d2Busy = 1; pEnqueue(newEvent);

    } else if (d1Size <= d2Size){
        enqueue(2, oldEvent.id);
    } else {
        enqueue(3, oldEvent.id);
    }

    // if CPU queue is nonempty, create process_arrive_cpu event
    if(cpuSize > 0) {
        int newID = dequeue(1); // pull process off cpu queue
        struct event newEvent;
        newEvent.time = oldEvent.time;
        newEvent.type = cpuarrival;
        newEvent.id = newID;
        cpuBusy = 1; // set cpu to occupied
        pEnqueue(newEvent);
    }

}

void exitsystem(struct event oldEvent) {
    fprintf(logFile, "\nFor time %d the process at %d exits the system",
            oldEvent.time, oldEvent.id);

}

void arriveatdisk1(struct event oldEvent) {
    fprintf(logFile, "\nFor time %d the process at %d enters disk 1",
            oldEvent.time, oldEvent.id);
    struct event newEvent;
    int num = randNum(DISK1_MIN, DISK1_MAX);

    // stats file
    d1Util += num;
    d1Count++;
    if (num > d1MaxResponse) {
        d1MaxResponse = num;
    }

    newEvent.time = oldEvent.time + num;
    newEvent.type = d1finish;
    newEvent.id = oldEvent.id;
    pEnqueue(newEvent);
}

void arriveatdisk2(struct event oldEvent)
{
    fprintf(logFile, "\nFor time %d the process at %d enters disk 2",
            oldEvent.time, oldEvent.id);
    struct event newEvent;
    int num = randNum(DISK2_MIN, DISK2_MAX);

    d2Util += num;
    d2Count++;
    if (num > d2MaxResponse)
    {
        d2MaxResponse = num;
    }

    newEvent.time = oldEvent.time + num;
    newEvent.type = d2finish;
    newEvent.id = oldEvent.id;
    pEnqueue(newEvent);
}

void Disk1finish(struct event oldEvent) {

    d1Busy = 0;
    fprintf(logFile, "\nFor time %d the process at %d exits disk 1",
            oldEvent.time, oldEvent.id);

    if(cpuBusy == 1 || cpuSize > 0){
        fprintf(logFile, "\nFor time %d The process at %d enters the CPU queue",
                oldEvent.time, oldEvent.id);
        enqueue(1, oldEvent.id);

    } else {
        struct event newEvent;
        newEvent.time = oldEvent.time;
        newEvent.type = cpuarrival;
        newEvent.id = oldEvent.id;
        cpuBusy = 1;
        pEnqueue(newEvent);
    }

    if(d1Size > 0) {
        int newID = dequeue(2);
        struct event newEvent;
        newEvent.time = oldEvent.time;
        newEvent.type = d1arrival;
        newEvent.id = newID;
        d1Busy = 1;
        pEnqueue(newEvent);
    }
}

void Disk2finish(struct event oldEvent) {

    d2Busy = 0;
    fprintf(logFile, "\nfor time %d the process at %d exits disk 2",
            oldEvent.time, oldEvent.id);

    if(cpuBusy == 1 || cpuSize > 0){
        fprintf(logFile, "\nfor time %d the process at %d enters the CPU queue",
                oldEvent.time, oldEvent.id);
        enqueue(1, oldEvent.id);
    } else {
        struct event newEvent;
        newEvent.time = oldEvent.time; newEvent.type = cpuarrival; newEvent.id = oldEvent.id;
        cpuBusy = 1; pEnqueue(newEvent);
    }

    if(d2Size > 0) {
        int newID = dequeue(3);
        struct event newEvent; newEvent.time = oldEvent.time; newEvent.type = d2arrival;
        newEvent.id = newID; d2Busy = 1;
        pEnqueue(newEvent);
    }
}

void arriveatNetwork(struct event oldEvent) {
    fprintf(logFile, "\nfor time %d the process at %d enters the network",
            oldEvent.time, oldEvent.id);
    struct event newEvent;
    int num = randNum(NETWORK_MIN, NETWORK_MAX);

    netUtil += num;
    netCount++;
    if (num > netMaxResponse) {
        netMaxResponse = num;
    }
    newEvent.time = oldEvent.time + num; newEvent.type = finish;
    newEvent.id = oldEvent.id;pEnqueue(newEvent);
}

void finishatNetwork(struct event oldEvent) {

    netBusy = 0;
    fprintf(logFile, "\nFor time %d the process at %d exits the network",
            oldEvent.time, oldEvent.id);

    if(cpuBusy == 1 || cpuSize > 0){
        fprintf(logFile, "\nFor time %d the process at %d enters the CPU queue",
                oldEvent.time, oldEvent.id);
        enqueue(1, oldEvent.id);
    } else {
        struct event newEvent;
        newEvent.time = oldEvent.time; newEvent.type = cpuarrival;
        newEvent.id = oldEvent.id; cpuBusy = 1;
        pEnqueue(newEvent);
    }

    if(netSize > 0) {
        int newID = dequeue(4);
        struct event newEvent; newEvent.time = oldEvent.time;
        newEvent.type = narrival; newEvent.id = newID;
        netBusy = 1; pEnqueue(newEvent);
    }
}

void finishprocess(struct event oldEvent) {

    /*int SEED, INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, QUIT_PROB, NETWORK_PROB, CPU_MIN, CPU_MAX,
//DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX, NETWORK_MIN, NETWORK_MAX;*/

    cpuAvg = cpuAvg/cpuTotal; d1Avg = d1Avg/d1Total; d2Avg = d2Avg/d2Total; netAvg = netAvg/netTotal;

    cpuResponse = cpuUtil/cpuCount; d1Response = d1Util/d1Count; d2Response = d2Util/d2Count; netResponse = netUtil/netCount;

    cpuUtil = cpuUtil/(FIN_TIME - INIT_TIME); d1Util = d1Util/(FIN_TIME - INIT_TIME); d2Util = d2Util/(FIN_TIME - INIT_TIME); netUtil = netUtil/(FIN_TIME - INIT_TIME);

    cpuThroughput = cpuCount/(FIN_TIME - INIT_TIME); d1Throughput = d1Count/(FIN_TIME - INIT_TIME); d2Throughput = d2Count/(FIN_TIME - INIT_TIME); netThroughput = netCount/(FIN_TIME - INIT_TIME);

    fprintf(statsFile, "\nMax Size of the Queue was:\nCPU = %d\nDisk 1 = %d\nDisk 2 = %d\nNetwork = %d\n", cpuMax, d1Max, d2Max, netMax);
    fprintf(statsFile, "\nAverage Size of the Queue was:\nCPU = %f\nDisk 1 = %f\nDisk 2 = %f\nNetwork = %f\n", cpuAvg, d1Avg, d2Avg, netAvg);
    fprintf(statsFile, "\nUtilization:\nCPU = %f\nDisk 1 = %f\nDisk 2 = %f\nNetwork - %f\n", cpuUtil, d1Util, d2Util, netUtil);
    fprintf(statsFile, "\nThroughput:\nCPU = %f\nDisk 1 = %f\nDisk 2 = %f\nNetwork = %f\n", cpuThroughput, d1Throughput, d2Throughput, netThroughput);
    fprintf(logFile, "\n For time %d jobs have finished", oldEvent.time);

}