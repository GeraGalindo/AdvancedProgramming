#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    printf("Welcome to Raspberry Pi!\n");
    // create a socket
    int networkSocket;
    networkSocket = socket(AF_INET, SOCK_STREAM, 0); // Using default protocol (TCP)

    // specify an address for the socket
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9009);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Connect and check for connection errors
    int connectionStatus = connect(networkSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if(connectionStatus == -1){
        printf("Error while trying to connect to remote address\n");
        return 1;
    }

    // receive data from server
    char serverResponse[64];
    recv(networkSocket, &serverResponse, sizeof(serverResponse), 0);

    printf("From server: %s\n", serverResponse);

    // Close the socket
    close(networkSocket);
    return 0;
}


