#include <wiringPi.h>          
#include <lcd.h>                

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

int main()
{
	int lcd;
	wiringPiSetup();
	lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

	int n;
	struct ifreq ifr;
	char iface[] = "wlan0"; //Change this to the network of your choice (eth0, wlan0, etc.)

	n = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , iface , IFNAMSIZ - 1);
	ioctl(n, SIOCGIFADDR, &ifr);
	close(n);

	// Display IP Address in the LCD
	lcdPosition(lcd, 0, 0);
	lcdPrintf(lcd, "IP Address: ");
	lcdPosition(lcd, 0, 1);
	lcdPrintf(lcd, ("%s - %s\n" , iface , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr)));


	// create the server socket
	int serverSocket;
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Define the server Address
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9090);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket to our specified IP Address and Port
	bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	// Listen for connections
	listen(serverSocket, 1);
	printf("Listening...");

	// Handle connections
	int clientSocket = accept(serverSocket, NULL, NULL);
	printf("Connection Accepted...");

	// Receive a message
	char clientMsg[64];
	recv(clientSocket, &clientMsg, sizeof(clientMsg), 0);

	// Print received message
	printf("Received: %s\n", clientMsg);

	// Close the Socket
	close(serverSocket);

	return 0;
}

