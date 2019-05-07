#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wiringPi.h>          
#include <lcd.h>               
 
//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

int main() {
    printf("Welcome to Raspberry Pi!\n");
    int lcd;               
    wiringPiSetup();        
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    // create a socket
    int networkSocket;
    networkSocket = socket(AF_INET, SOCK_STREAM, 0); // Using default protocol (TCP)

    // specify an address for the socket
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9009);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.72");

    // Connect and check for connection errors
    int connectionStatus = connect(networkSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if(connectionStatus == -1){
        printf("Error while trying to connect to remote address\n");
        return 1;
    }

    // receive data from server
    char serverResponse[64];
    /*for(int i = 0; i < 10; ++i){
	recv(networkSocket, &serverResponse, sizeof(serverResponse), 0);
	lcdPuts(lcd, serverResponse);
    }*/
    recv(networkSocket, &serverResponse, sizeof(serverResponse), 0);
    lcdPuts(lcd, serverResponse);

    char serverMessage[] = "Raspberry puto";
    send(networkSocket, serverMessage, sizeof(serverMessage), 0);

    // Close the socket
    printf("Closing the Socket\n");
    close(networkSocket);
    return 0;
}


