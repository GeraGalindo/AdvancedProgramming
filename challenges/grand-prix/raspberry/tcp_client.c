#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wiringPi.h>          
#include <lcd.h>  
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>        
 
//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

char serverMessage[16];

void clearMessage(){
	for(int i = 0; i < 16; ++i){
		serverMessage[i] = 0;
	}
}

static void * connectionThread(void *arg){
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
		return (void *)1;
	}
	
	while(1){
		// receive data from server
		clearMessage();
		recv(networkSocket, &serverMessage, sizeof(serverMessage), 0);
		printf("Message Received\n");

		if (!strcmp(serverMessage, "Quit")){
			printf("Quit\n");
			lcdClear(lcd);
			lcdPosition(lcd, 0, 0);
			lcdPuts(lcd, serverMessage);
			break;
		}

		lcdClear(lcd);
		lcdPosition(lcd, 0, 0);
		lcdPuts(lcd, serverMessage);

		char clientMessage[] = "Received";
		send(networkSocket, clientMessage, sizeof(clientMessage), 0);
		printf("Message Sent\n");
	}
	

	// Close the socket
	printf("Closing the Socket\n");
	close(networkSocket);
}

int main() {
	printf("Welcome to Raspberry Pi!\n");
	
	pthread_t clientThread;
	void *res;
	int s;

	s = pthread_create(&clientThread, NULL, connectionThread, "Initializing TCP Client Thread");
        if (s != 0){
		printf("Error in Thread creation\n");
	}

	s = pthread_join(clientThread, &res);
	if (s != 0){
		printf("Error in Thread joining\n");
	}

	printf("Thread Joined\n");
	
	return 0;
}











