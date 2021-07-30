//First come first serve scheduling algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Function declarations*/
void schedule(char process[]);
float averageWaitingTime(char processQueue[][256]);
float averageTurnaroundTime(char processQueue[][256]);
void display();

/*Final answers*/
char processQueue[256][256];
float avgWT = 0.0;
float avgTT = 0.0;

/*helping global variables*/
char process[256] = "";
char ps[256][256];
float bts[256];
float ats[256];
float cmpTime[256];
int numOfProcesses = 0;
int tt_bt = 0;

int main(int argc, char *argv[])
{
    //sscanf(argv[1], "%s", &process);
    if(argc<2)
    {
        printf("%s","No input passed. Please remember to pass input via terminal. Program will terminate now...");
        return 0;
    }
    strcpy(process,argv[1]);
    schedule(process);
    avgWT = averageWaitingTime(processQueue);
    avgTT = averageTurnaroundTime(processQueue);
    display();
    return 0;
}

/*Schedules processes and returns and creates a queue of the schedule*/
void schedule(char process[])
{
    //Opening process file
    FILE *processfile;
    processfile = fopen (process, "r");
    if (processfile == NULL)
    {
    printf ("\nCould not open file \"%s\", Please specify an available file.\n",
        process);
        exit(0);
    }
    char line[256];
    char p[256] = "";
    float bt = 0.0;
    float at = 0.0;
    
    int index = 0;
    fgets(line, sizeof(line), processfile); //gets rid of title
    while(fgets(line, sizeof(line), processfile))
    {
        /*take off \n character and splits line by space */
        char *curLine = strtok (line, "\n");
        curLine = strtok (line, " ");
        strcpy(p,curLine);
        curLine = strtok (NULL, " ");
        bt = atof(curLine);
        curLine = strtok (NULL, " ");
        at = atof(curLine);
        curLine = strtok (NULL, " ");
        
        strcpy(ps[index],p);
        bts[index] = bt;
        ats[index] = at;
        index++;
    }
    numOfProcesses = index;
    fclose (processfile);
    
    //creating queue
    int minIndex = -1;
    float prevMin = -1;
        
    int processIndex = 0;
    for(int i = 0; i<numOfProcesses;i++)
    {
        float min = INFINITY;
        int j;
        //finds the process with the earlierst arrival time to execute
        for(j = 0; j<numOfProcesses;j++)
        {
            if(ats[j]>prevMin && ats[j]<min)
            {
                min = ats[j];
                minIndex = j;
            }
        }
        //adds process to queue
        prevMin = min;
        for(j=0;j<bts[minIndex];j++)
        {
            strcpy(processQueue[processIndex],ps[minIndex]);
            processIndex++;
        }
    }
    tt_bt = processIndex + 1;
}

/*computes and returns average waiting time*/
float averageWaitingTime(char processQueue[][256])
{
    int i;
    for(i = 0;i<numOfProcesses; i++)//computes completion time of all processes
    {
        for(int j = tt_bt-1; j>0; j--)
        {
            if(strcmp(ps[i],processQueue[j]) == 0)
            {
                cmpTime[i] = (float)j+1;
                break;
            }
        }
    }
    float sum = 0.0;
    for(i=0; i <numOfProcesses; i++)
    {
        sum += cmpTime[i] - ats[i] - bts[i]; //summing waiting times
    }
    return sum/((float)numOfProcesses);//returns average waiting time
}

/*computes and returns average turnaround time*/
float averageTurnaroundTime(char processQueue[][256]) 
{
    float sum = 0.0;
    for(int i=0; i <numOfProcesses; i++)
    {
        sum += cmpTime[i] - ats[i]; //summing turnaround times
    }
    return sum/((float)numOfProcesses);//returns average turnaround time
}

/*Displays scheduling order, average waiting time and average turnaround time*/
void display()
{
    printf("%s","< SCHEDULING ALGORITHM: First Come First Serve (FCFS) >\n\n");
    printf("%s\t\t","Scheduling order: ");
    for(int i = 0; i<tt_bt;i++)
    {
        printf("%s ",processQueue[i]);
    }
    printf("\nAverage waiting time:\t\t%f",avgWT);
    printf("\nAverage turnaround time:\t%f",avgTT);
}

/*I used onlinegdb.com to write and test my code. I use a windows machine.
To run this code, pass the name of the input file via terminal */