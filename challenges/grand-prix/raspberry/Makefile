output: tcp_client.o
	gcc tcp_client.o -lwiringPi -lwiringPiDev -lpthread -o output

tcp_client.o: tcp_client.c
	gcc -c tcp_client.c 

clean:
	rm *.o output
