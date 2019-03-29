#include <stdio.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>

#include "logger.h"

void clear();

/*
 * Algorithm to follow:
 * 1. Create a file monitor for /proc
 * 2. List all directories within /proc
 * 3. PIDs -> all listed directories that are only numbers
 * 4. Store all PIDs in a List of 'process structs'
 *      Process struct contains:
 *          int pid
 *          int parent
 *          char* name
 *          char* state
 *          char* memory
 *          int threads
 *          int open_files
 * 5. For each PID in the List:
 *      Go to /proc/<PID>/status and:
 *          Parse: Parent
 *          Parse: Name
 *          Parse: State
 *          Parse: Number of Threads
 *          Calculate: Memory
 *      Go to /proc/<PID>/fd and obtain Open Files
 * 6. Print formatted output for each PID in the List
 *
 * */

static void displayEvents(struct inotify_event *i);
static int counter = 0;

typedef struct process{
    int pid;
    int parent_pid;
    char name[NAME_MAX];
    char state[NAME_MAX];
    char memory[NAME_MAX];
    int threads;
    int open_files;
    struct process* next;
} PROCESSES;

PROCESSES *currentHead, *temp, *prev;

void initSignalHandler();
void signalHandler(int);
void addProcess(int pidNo);
void clearProcessList();
void displayProcesses();
void updateDirectory();
void populateProcesses();
void parseData(struct process*);
void parseParentPid(struct process*);
void parseName(struct process*);
void parseState(struct process*);
void calculateMemory(struct process*);
void parseThreads(struct process*);
void parseOpenFiles(struct process*);
void saveFile();

int main(int argc, char* argv[]) {
    initSignalHandler();

    currentHead = NULL;

    while (!counter){
        clear();

        updateDirectory();

        populateProcesses();

        displayProcesses();

        clearProcessList();

        sleep(15);
    }

    return 0;
}

void initSignalHandler(){
    // Signal Registration SIGINT
    if(signal(SIGINT, signalHandler) == SIG_ERR){
        errorf("Unable to set the SIGINT signal handler\n");
        exit(1);
    }
}

void signalHandler(int signal){
    infof("Signal Caught: ");
    ++counter;
    if (signal ==  SIGINT){
        infof("SIGINT \n");
    } else {
        infof("Signal: %d\n", signal);
    }
    saveFile();
}

void saveFile(){
    if (currentHead == NULL){
        printf("Signal caught in wrong moment: Empty List\n");
        return;
    }

    time_t rawTime;
    struct tm * timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    char* fileName = (char*)calloc(64, sizeof(char));
    sprintf(fileName, "%s%s", "mytop-status-", asctime(timeInfo));
    FILE *fd;
    fd = fopen(fileName, "w");
    if (fd == NULL){
        errorf("Couldn't write mytop-status-XXXX.txt\n");
        exit(1);
    }

    temp = currentHead;
    fprintf(fd, "| PID      \t| Parent  \t| Name             \t\t| State    \t\t| Memory \t| # Threads | Open Files |\n");
    while(temp != NULL){
        fprintf(fd, "| %d\t\t | %d\t\t | %s\t\t\t | %s\t\t | %s\t\t | %d\t | %d\t\n",
               temp->pid, temp->parent_pid, temp->name, temp->state, temp->memory, temp->threads, temp->open_files);
        temp = temp->next;
    }

    fclose(fd);

}

void calculateMemory(struct process *p){
    char cmd1[] = "grep -i rssanon /proc/";
    char cmd2[7];
    sprintf(cmd2, "%d", p->pid);
    char cmd3[] = "/status > mem.txt";
    char command[sizeof(cmd1) + sizeof(cmd2) + sizeof(cmd3)];
    sprintf(command, "%s%s%s", cmd1, cmd2, cmd3);
    FILE *fp;
    int ch;
    int i = 0;
    char* mem;

    system(command);

    fp = fopen("mem.txt", "r");
    if (fp == NULL){
        errorf("Couldn't read mem.txt\n");
        exit(1);
    }

    mem = (char*)calloc(NAME_MAX, sizeof(char));

    for (int i = 0; i < 14; ++i){
        ch = getc(fp);
        if(ch == -1){
            p->memory[i] = 'U';
            p->memory[i+1] = 'k';
            p->memory[i+2] = 'n';
            free(mem);
            fclose(fp);
            system("rm mem.txt");
            return;
        }
    }

    for (int i = 0; ch != EOF; ++i){
        ch = getc(fp);
        mem[i] = (char)ch;
        if (ch == '\n'){
            break;
        }
    }
    i = 0;
    while(mem[i] != '\n'){
        p->memory[i] = mem[i];
        ++i;
    }

    free(mem);
    fclose(fp);
    system("rm mem.txt");
}
void parseOpenFiles(struct process *p){ // ls /proc/2626/fd | wc -l
    char cmd1[] = "ls /proc/";
    char cmd2[7];
    sprintf(cmd2, "%d", p->pid);
    char cmd3[] = "/fd | wc -l > openFiles.txt";
    char command[sizeof(cmd1) + sizeof(cmd2) + sizeof(cmd3)];
    sprintf(command, "%s%s%s", cmd1, cmd2, cmd3);
    FILE *fp;
    int ch;
    int i = 0;
    char* openFiles;

    system(command);

    fp = fopen("openFiles.txt", "r");
    if (fp == NULL){
        errorf("Couldn't read openFiles.txt\n");
        exit(1);
    }

    openFiles = (char*)calloc(5, sizeof(char));

    ch = getc(fp);
    while (ch != EOF){
        if (ch >= '0' && ch <= '9'){
            openFiles[i] = (char)ch;
            ++i;
        }
        ch = getc(fp);
    }

    p->open_files = atoi(openFiles);

    free(openFiles);
    fclose(fp);
    system("rm openFiles.txt");
}
void parseParentPid(struct process *p){
    char cmd1[] = "grep -i ppid /proc/";
    char cmd2[7];
    sprintf(cmd2, "%d", p->pid);
    char cmd3[] = "/status > ppid.txt";
    char command[sizeof(cmd1) + sizeof(cmd2) + sizeof(cmd3)];
    sprintf(command, "%s%s%s", cmd1, cmd2, cmd3);
    FILE *fp;
    int ch;
    int i = 0;
    char* ppid;

    system(command);

    fp = fopen("ppid.txt", "r");
    if (fp == NULL){
        errorf("Couldn't read ppid.txt\n");
        exit(1);
    }

    ppid = (char*)calloc(5, sizeof(char));

    ch = getc(fp);
    while (ch != EOF){
        if (ch >= '0' && ch <= '9'){
            ppid[i] = (char)ch;
            ++i;
        }
        ch = getc(fp);
    }

    p->parent_pid = atoi(ppid);

    free(ppid);
    fclose(fp);
    system("rm ppid.txt");
}
void parseName(struct process *p){
    char cmd1[] = "grep -i name /proc/";
    char cmd2[7];
    sprintf(cmd2, "%d", p->pid);
    char cmd3[] = "/status > name.txt";
    char command[sizeof(cmd1) + sizeof(cmd2) + sizeof(cmd3)];
    sprintf(command, "%s%s%s", cmd1, cmd2, cmd3);
    FILE *fp;
    int ch;
    int i = 0;
    char* name;

    system(command);

    fp = fopen("name.txt", "r");
    if (fp == NULL){
        errorf("Couldn't read name.txt\n");
        exit(1);
    }

    name = (char*)calloc(NAME_MAX, sizeof(char));

    for (int i = 0; i < 6; ++i){
        ch = getc(fp);
    }

    for (int i = 0; ch != EOF; ++i){
        ch = getc(fp);
        name[i] = (char)ch;
        if (ch == '\n') break;
    }
    i = 0;
    while(name[i] != '\n'){
        p->name[i] = name[i];
        ++i;
    }

    free(name);
    fclose(fp);
    system("rm name.txt");
}
void parseState(struct process *p){
    char cmd1[] = "grep -i state /proc/";
    char cmd2[10];
    sprintf(cmd2, "%d", p->pid);
    char cmd3[] = "/status > state.txt";
    char command[sizeof(cmd1) + sizeof(cmd2) + sizeof(cmd3)];
    sprintf(command, "%s%s%s", cmd1, cmd2, cmd3);
    FILE *fp;
    int ch;
    int i = 0;
    char* state;

    system(command);

    fp = fopen("state.txt", "r");
    if (fp == NULL){
        errorf("Couldn't read state.txt\n");
        exit(1);
    }

    state = (char*)calloc(30, sizeof(char));

    for (int i = 0; i < 7; ++i){
        ch = getc(fp);
    }

    for (int i = 0; ch != EOF; ++i){
        ch = getc(fp);
        state[i] = (char)ch;
        if (ch == '\n') break;
    }
    i = 0;
    while(state[i] != '\n'){
        p->state[i] = state[i];
        ++i;
    }

    free(state);
    fclose(fp);
    system("rm state.txt");
}
void parseThreads(struct process *p){
    char cmd1[] = "grep -i threads /proc/";
    char cmd2[7];
    sprintf(cmd2, "%d", p->pid);
    char cmd3[] = "/status > threads.txt";
    char command[sizeof(cmd1) + sizeof(cmd2) + sizeof(cmd3)];
    sprintf(command, "%s%s%s", cmd1, cmd2, cmd3);
    FILE *fp;
    int ch;
    int i = 0;
    char* threads;

    system(command);

    fp = fopen("threads.txt", "r");
    if (fp == NULL){
        errorf("Couldn't read threads.txt\n");
        exit(1);
    }

    threads = (char*)calloc(5, sizeof(char));

    ch = getc(fp);
    while (ch != EOF){
        if (ch >= '0' && ch <= '9'){
            threads[i] = (char)ch;
            ++i;
        }
        ch = getc(fp);
    }

    p->threads = atoi(threads);

    free(threads);
    fclose(fp);
    system("rm threads.txt");
}

void parseData(struct process *p){
    parseParentPid(p);
    parseName(p);
    parseState(p);
    calculateMemory(p);
    parseThreads(p);
    parseOpenFiles(p);
}

void populateProcesses(){
    temp = currentHead;
    while(temp != NULL){
        parseData(temp);
        temp = temp->next;
    }
}

void updateDirectory(){
    int value;
    DIR *fd;
    struct dirent *dir;
    fd = opendir("/proc");
    if (fd){
        while ((dir = readdir(fd)) != NULL){
            if (dir->d_name[0] > '0' && dir->d_name[0] <= '9'){
                value = atoi(dir->d_name);
                addProcess(value);
            }
        }
        closedir(fd);
    }
}

void displayProcesses(){
    if (currentHead == NULL){
        printf("Empty List\n");
        return;
    }
    temp = currentHead;
    printf("| PID      \t| Parent  \t| Name             \t\t| State    \t\t| Memory \t| # Threads | Open Files |\n");
    while(temp != NULL){
        printf("| %d\t\t | %d\t\t | %s\t\t\t | %s\t\t | %s\t\t | %d\t | %d\t\n",
                temp->pid, temp->parent_pid, temp->name, temp->state, temp->memory, temp->threads, temp->open_files);
        temp = temp->next;
    }
}

void addProcess(int pidNo){
    temp = malloc(sizeof(PROCESSES));
    temp->pid = pidNo;
    temp->next = currentHead;
    currentHead = temp;
}

void clearProcessList(){
    prev = currentHead;
    while(currentHead){
        currentHead = currentHead->next;
        free(prev);
        prev = currentHead;
    }
}

void clear() {
  printf("\e[1;1H\e[2J"); 
}
