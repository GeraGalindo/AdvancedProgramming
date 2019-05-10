#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

#include "coordinates.h"

// Default message to be sent to clients
char serverMessage[] = "Message X";
int numRacers = 0;
int numLaps = 0;

void clearMessage(){
    for(int i = 0; i < 10; ++i){
        serverMessage[i] = 0;
    }
}

static void * connectionThread(void *arg){
    char *s = (char *) arg;
    printf("%s\n", s);

    char msgIdx = '0';

    // create a socket
    int serverSocket;
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
    int clientSocket = accept(serverSocket, NULL, NULL);

    int cmd = getchar();
    while(cmd != 'q'){
        serverMessage[8] = msgIdx;
        // Send Single Message
        send(clientSocket, serverMessage, sizeof(serverMessage), 0);
        msgIdx++;
        char serverResponse[64];
        recv(clientSocket, &serverResponse, sizeof(serverResponse), 0);
        printf("From Raspberry: %s", serverResponse);
        cmd = getchar();
        if (cmd == 'q'){
            clearMessage();
            stpcpy(serverMessage, "Quit");
            send(clientSocket, serverMessage, sizeof(serverMessage), 0);
            printf("%s\n", serverMessage);
            break;
        }
    }

    printf("Closing Socket\n");

    // Close the socket
    close(serverSocket);

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

    pthread_t serverThread;
    void *res;
    int s;

    s = pthread_create(&serverThread, NULL, connectionThread, "Initializing TCP Server Thread");
    if (s != 0){
        printf("Error while creating thread\n");
    }

    system("python graphics.py");

    // TODO: Delete this while loop and instead run this Matrix traverse in each thread
    while(1){
        for(int i = 0; i < 8; ++i){
            for (int j = 0; j < 100; ++j){
                // TODO: Check if corresponding file exists; If (file exist) -> Wait ... else write coordinates

            }
        }
        break;
    }

    s = pthread_join(serverThread, &res);
    if (s != 0){
        printf("Error while joining thread\n");
    }

    printf("Thread Joined\n");

    return 0;
}
