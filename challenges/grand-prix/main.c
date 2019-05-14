#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

#include "coordinates.h"

int numRacers = 0;
int numLaps = 0;

pthread_mutex_t lock;

// Server Variables
int serverSocket;
int clientSocket;
char serverMessage[16];

void clearMessage(){
    for(int i = 0; i < 10; ++i){
        serverMessage[i] = 0;
    }
}

void initServer(){
    // create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Using default protocol (TCP)

    // define the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9009);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to our specified IP and port
    bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

    // Listen for connections
    listen(serverSocket, 1);

    // Handle connections
    clientSocket = accept(serverSocket, NULL, NULL);

    printf("Server Socket Initialized\n");

}

void freeServer(){
    // Close the socket
    close(serverSocket);
}

void sendMessage(char *msg){
    clearMessage();
    strcpy(serverMessage, msg);
    send(clientSocket, serverMessage, sizeof(serverMessage), 0);

    char serverResponse[64];
    recv(clientSocket, &serverResponse, sizeof(serverResponse), 0);
    printf("From Raspberry: %s\n", serverResponse);
}

static void * blueThread(void *arg){
    char *s = (char *) arg;
    printf("%s\n", s);

    while(1){
        for(int i = 0; i < 1; ++i){
            for (int j = 0; j < 500; ++j){
                usleep(100000);
                // TODO: Check if corresponding file exists; If (file exist) -> Wait ... else write coordinates
                //printf("X: %d\tY: %d\n", outer[j][i], outer[j][i+1]);
                pthread_mutex_lock(&lock);
                FILE *file;
                file = fopen("blue.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                    continue;
                }
                if(outer[j][i] == 50 && outer[j][i+1] == 60){           // FIRST TURN
                    fprintf(file, "%s", "Blue ");
                    fprintf(file, "%d", 50);
                    fprintf(file, "%s", " ");
                    fprintf(file, "%d", 60);
                    fprintf(file, "%s", " -90");
                } else if(outer[j][i] == 835 && outer[j][i+1] == 60){    // SECOND TURN
                    fprintf(file, "%s", "Blue ");
                    fprintf(file, "%d", 835);
                    fprintf(file, "%s", " ");
                    fprintf(file, "%d", 60);
                    fprintf(file, "%s", " -90");
                }
                else {                                                  // STRAIGHT LINE
                    fprintf(file, "%s", "Blue ");
                    fprintf(file, "%d", outer[j][i]);
                    fprintf(file, "%s", " ");
                    fprintf(file, "%d", outer[j][i+1]);
                    fprintf(file, "%s", " 0");
                }

                fclose(file);
                pthread_mutex_unlock(&lock);
            }
        }

        break;
    }

    return (void *) strlen(s);
}

static void * redThread(void *arg){
    char *s = (char *) arg;
    printf("%s\n", s);

    int redX, redY;
    redX = 140;
    redY = 325;
    int upperLeftLimit = 120;
    int upperRightLimit = 750;
    int lowerRightLimit = 540;
    int lowerLeftLimit = 140;
    int raceGoalLimit = 240;

    while(1){
        FILE *file;
        for(int i = 0; i < numLaps; ++i){
            for(; redY >= upperLeftLimit; redY=redY-5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("red.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Red ");
                fprintf(file, "%d", redX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", redY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);

            pthread_mutex_lock(&lock);
            file = fopen("red.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Red ");
            fprintf(file, "%d", redX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", redY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; redX <= upperRightLimit; redX=redX+5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("red.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Red ");
                fprintf(file, "%d", redX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", redY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);

            pthread_mutex_lock(&lock);
            file = fopen("red.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Red ");
            fprintf(file, "%d", redX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", redY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; redY <= lowerRightLimit; redY=redY+5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("red.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Red ");
                fprintf(file, "%d", redX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", redY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);

            pthread_mutex_lock(&lock);
            file = fopen("red.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Red ");
            fprintf(file, "%d", redX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", redY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; redX >= lowerLeftLimit; redX=redX-5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("red.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Red ");
                fprintf(file, "%d", redX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", redY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);

            pthread_mutex_lock(&lock);
            file = fopen("red.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Red ");
            fprintf(file, "%d", redX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", redY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; redY >= raceGoalLimit; redY=redY-5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("red.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Red ");
                fprintf(file, "%d", redX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", redY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }
//            switch (i){
//                case 0:
//                    sendMessage("LAP 1: RED");
//                    break;
//                case 1:
//                    sendMessage("LAP 2: RED");
//                    break;
//                case 2:
//                    sendMessage("LAP 3: RED");
//                    break;
//                default:
//                    sendMessage("FINISHER RED");
//            }
        }

        break;
    }
    return (void *) strlen(s);
}

static void * greenThread(void *arg){
    char *s = (char *) arg;
    printf("%s\n", s);
    int greenX, greenY;
    greenX = 100;
    greenY = 325;
    int upperLeftLimit = 100;
    int upperRightLimit = 790;
    int lowerRightLimit = 585;
    int lowerLeftLimit = 100;
    int raceGoalLimit = 240;

    while(1){
        FILE *file;
        for(int i = 0; i < numLaps; ++i){
            for(; greenY >= upperLeftLimit; greenY=greenY-5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("green.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Green ");
                fprintf(file, "%d", greenX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", greenY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }
            usleep(100000);

            pthread_mutex_lock(&lock);
            file = fopen("green.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Green ");
            fprintf(file, "%d", greenX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", greenY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; greenX <= upperRightLimit; greenX=greenX+5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("green.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Green ");
                fprintf(file, "%d", greenX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", greenY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);
            pthread_mutex_lock(&lock);
            file = fopen("green.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Green ");
            fprintf(file, "%d", greenX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", greenY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; greenY <= lowerRightLimit; greenY=greenY+5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("green.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Green ");
                fprintf(file, "%d", greenX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", greenY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);
            pthread_mutex_lock(&lock);
            file = fopen("green.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Green ");
            fprintf(file, "%d", greenX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", greenY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; greenX >= lowerLeftLimit; greenX=greenX-5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("green.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Green ");
                fprintf(file, "%d", greenX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", greenY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(100000);

            pthread_mutex_lock(&lock);
            file = fopen("green.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Green ");
            fprintf(file, "%d", greenX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", greenY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; greenY >= raceGoalLimit; greenY=greenY-5){
                usleep(100000);

                pthread_mutex_lock(&lock);
                file = fopen("green.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Green ");
                fprintf(file, "%d", greenX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", greenY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

//            switch (i){
//                case 0:
//                    sendMessage("LAP 1: GREEN");
//                    break;
//                case 1:
//                    sendMessage("LAP 2: GREEN");
//                    break;
//                case 2:
//                    sendMessage("LAP 3: GREEN");
//                    break;
//                default:
//                    sendMessage("FINISHER GREEN");
//            }
        }

        break;
    }
    return (void *) strlen(s);
}

void getRacers(){
    while(numRacers == 0){
        printf("Type the number of Racers (1 - 3)\n");
        int num = getchar();
        switch (num) {
            case '1':
                numRacers = 1;
                break;
            case '2':
                numRacers = 2;
                break;
            case '3':
                numRacers = 3;
                break;
            default:
                printf("ERROR: Only numbers from 1 to 4\n\n");
                break;
        }
    }
    getchar();
}

void getLaps(){
    while(numLaps == 0){
        printf("Type the number of Laps (1 - 4)\n");
        int laps = getchar();
        switch (laps) {
            case '1':
                numLaps = 1;
                break;
            case '2':
                numLaps = 2;
                break;
            case '3':
                numLaps = 3;
                break;
            case '4':
                numLaps = 4;
                break;
            default:
                printf("ERROR: Only numeric values from 1 to 4\n\n");
                break;
        }
    }
    getchar();
}

int main() {
    printf("Welcome to Grand Prix!\n");
    getRacers();
    getLaps();

    // TODO: Create a Thread for each numRacers

    printf("Racers: %d\nLaps: %d\n", numRacers, numLaps);

    pthread_t blueCarThread;
    pthread_t redCarThread;
    pthread_t greenCarThread;
    void *res;
    int s;

    //initServer();

    system("python graphics.py &");

    // CARS CREATION -----------------------------------------------------------------------
    s = pthread_create(&blueCarThread, NULL, blueThread, "Running Blue Car Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }

    s = pthread_create(&redCarThread, NULL, redThread, "Running Red Car Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }

    s = pthread_create(&greenCarThread, NULL, greenThread, "Running Green Car Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }
    /////////////////////////////////////////////////////////////////////////////////////////

    s = pthread_join(blueCarThread, &res);
    if (s != 0){
        printf("Error while joining thread\n");
    }

    s = pthread_join(redCarThread, &res);
    if (s != 0){
        printf("Error while joining thread\n");
    }

    s = pthread_join(greenCarThread, &res);
    if (s != 0){
        printf("Error while joining thread\n");
    }

    printf("Threads Joined\n");

    //freeServer();

    return 0;
}
