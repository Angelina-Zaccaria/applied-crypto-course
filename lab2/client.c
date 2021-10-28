/*
This is a basic client program that connects to a server at the given
IP address and port number, then exchanges one message before exiting.

Compile the program: gcc -g client.c -o client

Run the program: client serverIP portnumber

If the server is running on the same machine, the serverIP will be
the localhost 127.0.0.1. Otherwise, the IP address argument should match the 
server host machine. The portnumber will be whatever port the server
program is listening on. The server should be running before starting 
this client program.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Variables for our socket
int client_fd; 
const int BUF_SIZE = 1025;
struct sockaddr_in serv_addr;

int tcp_connect(const char * hostIP, const char *port);

int main(int argc, const char *argv[]) {

	// Check for correct number of arguments
	if (argc != 3) {
		printf("ERROR: Incorrect number of arguments.\n");
		printf("Usage: client hostIP portnumber\n");
		return 1;
	}

	// Create TCP socket
	client_fd = tcp_connect(argv[1], argv[2]);
	if (client_fd < 0) {
		printf("ERROR: Socket creation failed.\n");
		return 1;
	}

	// Connect to server
	if (connect(client_fd, (struct sockaddr *)&serv_addr, 
			sizeof(serv_addr)) < 0) {
		printf("ERROR: Failed to connect to server.\n");
		close(client_fd);
		return 1;
	}
	printf("Connected to server!\n");

	// Read welcome message from server
	char buffer[BUF_SIZE];
	int valread = read(client_fd, buffer, BUF_SIZE);
	// Make sure the received data is a null-terminated string
	buffer[valread] = '\0';
	printf("Received: \n\t%s = %d bytes\n", buffer, valread);

	// Write message to server
	char *mesg = "Hello to server!";
	send(client_fd, mesg, strlen(mesg), 0);
	printf("Sent: \n\t%s = %d bytes\n", mesg, strlen(mesg));

	// Close connection
	close(client_fd);
	return 0;
}

// Function that sets up a TCP socket connection and
// returns the file descriptor (number) assosciated with the socket
int tcp_connect(const char *hostIP, const char *port) {
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// If socket creation failed, return now
	if (sockfd < 0)
		return sockfd;

	// Otherwise, continue setting connection variables
	// using given IP address and port number
	int port_num = atoi(port);
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_num);
	serv_addr.sin_addr.s_addr = inet_addr(hostIP);

	return sockfd;
}