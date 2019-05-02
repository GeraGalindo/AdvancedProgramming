#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    printf("Welcome to Grand Prix!\n");
    // Default message to be sent to clients
    char serverMessage[] = "Message from Grand Prix";

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

    // Send Message
    send(clientSocket, serverMessage, sizeof(serverMessage), 0);

    // Close the socket
    close(serverSocket);
    return 0;
}
