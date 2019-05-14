#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "coordinates.h"

#define LOWERLIMIT 200000
#define UPPERLIMIT 300000

int numRacers = 0;
int numLaps = 0;

int blueSpeed = 0;
int greenSpeed = 0;
int redSpeed = 0;

pthread_mutex_t lock;

// Server Variables
int serverSocket;
int clientSocket;
char serverMessage[16];

int winner = 0;

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
    int blueX, blueY;
    blueX = 60;
    blueY = 325;
    int upperLeftLimit = 60;
    int upperRightLimit = 815;
    int lowerRightLimit = 585;
    int lowerLeftLimit = 70;
    int raceGoalLimit = 240;

    while(1){
        FILE *file;
        for(int i = 0; i < numLaps; ++i){
            for(; blueY >= upperLeftLimit; blueY=blueY-5){
                usleep(blueSpeed);

                pthread_mutex_lock(&lock);
                file = fopen("blue.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Blue ");
                fprintf(file, "%d", blueX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", blueY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }
            usleep(blueSpeed);
            sendMessage("B: TURN RIGHT");

            pthread_mutex_lock(&lock);
            file = fopen("blue.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Blue ");
            fprintf(file, "%d", blueX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", blueY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; blueX <= upperRightLimit; blueX=blueX+5){
                usleep(blueSpeed);

                pthread_mutex_lock(&lock);
                file = fopen("blue.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Blue ");
                fprintf(file, "%d", blueX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", blueY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }
            usleep(blueSpeed);
            sendMessage("B: TURN DOWN");

            pthread_mutex_lock(&lock);
            file = fopen("blue.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Blue ");
            fprintf(file, "%d", blueX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", blueY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; blueY <= lowerRightLimit; blueY=blueY+5){
                usleep(blueSpeed);

                pthread_mutex_lock(&lock);
                file = fopen("blue.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Blue ");
                fprintf(file, "%d", blueX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", blueY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }
            usleep(blueSpeed);
            sendMessage("B: TURN LEFT");

            pthread_mutex_lock(&lock);
            file = fopen("blue.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Blue ");
            fprintf(file, "%d", blueX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", blueY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; blueX >= lowerLeftLimit; blueX=blueX-5){
                usleep(blueSpeed);

                pthread_mutex_lock(&lock);
                file = fopen("blue.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Blue ");
                fprintf(file, "%d", blueX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", blueY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

            usleep(blueSpeed);
            sendMessage("B: TURN UP");

            pthread_mutex_lock(&lock);
            file = fopen("blue.txt", "w");
            if(file == NULL){
                printf("Error while opening the status file\n");
            }
            fprintf(file, "%s", "Blue ");
            fprintf(file, "%d", blueX);
            fprintf(file, "%s", " ");
            fprintf(file, "%d", blueY);
            fprintf(file, "%s", " -90");
            fclose(file);
            pthread_mutex_unlock(&lock);

            for(; blueY >= raceGoalLimit; blueY=blueY-5){
                usleep(blueSpeed);

                pthread_mutex_lock(&lock);
                file = fopen("blue.txt", "w");
                if(file == NULL){
                    printf("Error while opening the status file\n");
                }
                fprintf(file, "%s", "Blue ");
                fprintf(file, "%d", blueX);
                fprintf(file, "%s", " ");
                fprintf(file, "%d", blueY);
                fprintf(file, "%s", " 0");
                fclose(file);
                pthread_mutex_unlock(&lock);
            }

	    if (!winner){
		sendMessage("WINNER: BLUE");
		winner = 1;
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
                usleep(redSpeed);

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
            usleep(redSpeed);
	sendMessage("R: TURN RIGHT");
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
                usleep(redSpeed);
                
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
            usleep(redSpeed);
	sendMessage("R: TURN DOWN");
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
                usleep(redSpeed);
             

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
            usleep(redSpeed);
	sendMessage("R: TURN LEFT");
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
                usleep(redSpeed);

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

            usleep(redSpeed);
	sendMessage("R: TURN UP");
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
                usleep(redSpeed);

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

		if (!winner){
			sendMessage("WINNER: RED");
			winner = 1;
		}
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
                usleep(greenSpeed);

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

            usleep(greenSpeed);
	    sendMessage("G: TURN RIGHT");
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
                usleep(greenSpeed);

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

            usleep(greenSpeed);
	    sendMessage("G: TURN DOWN");
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
                usleep(greenSpeed);

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

            usleep(greenSpeed);
	    sendMessage("G: TURN LEFT");
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
                usleep(greenSpeed);

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

            usleep(greenSpeed);
	    sendMessage("G: TURN UP");
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
                usleep(greenSpeed);

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
		if (!winner){
			sendMessage("WINNER: GREEN");
			winner = 1;
		}
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
    int initFile;
    char initialRacers[1], initialLaps[1];
    printf("Welcome to Grand Prix!\n");
    getRacers();
    getLaps();

    initServer();

    printf("Racers: %d\nLaps: %d\n", numRacers, numLaps);

    //Init file for python
    initFile = open("initFile.txt", O_WRONLY);
    if (initFile == -1){
        printf("Error creating file");
    }
    sprintf(initialRacers, "%d", numRacers);
    sprintf(initialLaps, "%d", numLaps);
    write(initFile, initialRacers, sizeof(initialRacers));
    write(initFile, initialLaps, sizeof(initialLaps));
    close(initFile);

    srand(time(0));
    blueSpeed = (rand() % (UPPERLIMIT - LOWERLIMIT + 1)) + LOWERLIMIT;
    printf("Blue car's speed: %d\n", blueSpeed);
    pthread_t blueCarThread;
    if (numRacers == 2 || numRacers == 3){
        greenSpeed = (rand() % (UPPERLIMIT - LOWERLIMIT + 1)) + LOWERLIMIT;
        printf("Green car's speed: %d\n", greenSpeed);
    }
    if (numRacers == 3){
        redSpeed = (rand() % (UPPERLIMIT - LOWERLIMIT + 1)) + LOWERLIMIT;
        printf("Red car's speed: %d\n", redSpeed);
    }

    pthread_t redCarThread;
    pthread_t greenCarThread;
    void *res;
    int s;

    system("python graphics.py &");

    // CARS CREATION -----------------------------------------------------------------------
    s = pthread_create(&blueCarThread, NULL, blueThread, "Running Blue Car Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }

    if(numRacers == 3){
        s = pthread_create(&redCarThread, NULL, redThread, "Running Red Car Thread");
        if (s != 0){
            printf("Error while creating thread\n");
        }
    }

    if(numRacers == 2 || numRacers == 3){
        s = pthread_create(&greenCarThread, NULL, greenThread, "Running Green Car Thread");
        if (s != 0){
            printf("Error while creating thread\n");
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////

    s = pthread_join(blueCarThread, &res);
    if (s != 0){
        printf("Error while joining thread\n");
    }

    if(numRacers == 3){
        s = pthread_join(redCarThread, &res);
        if (s != 0){
            printf("Error while joining thread\n");
        }
    }

    if(numRacers == 2 || numRacers == 3){
        s = pthread_join(greenCarThread, &res);
        if (s != 0){
            printf("Error while joining thread\n");
        }
    }

    printf("Threads Joined\n");

    freeServer();

    return 0;
}
