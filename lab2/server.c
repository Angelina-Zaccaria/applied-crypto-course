/*
This is a basic server that sets up one listening socket and exchanges
data with one client before shutting down.

Compile the program: gcc -g server.c -o server

Run the program: server portnumber

The portnumber argument should be any value that isn't a reserved 
port number. Start the server program before starting the client.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Variables for our sockets
int serv_fd, newsock_fd, port_num;
const int BUF_SIZE = 1025;
struct sockaddr_in serv_addr, cli_addr;

int tcp_connect(const char *port);

int main(int argc, const char *argv[]) {

	// Check for correct number of arguments
	if (argc != 2) {
		printf("ERROR: Incorrect number of arguments.\n");
		printf("Usage: server portnumber\n");
		return 1;
	}

	// Establish main TCP socket
	serv_fd = tcp_connect(argv[1]);
	if (serv_fd < 0) {
		printf("ERROR: Socket creation failed.\n");
		return 1;
	}

	// Bind socket to specified address & port
	if (bind(serv_fd, (struct sockaddr *) &serv_addr, 
			sizeof(serv_addr)) < 0) {
		printf("ERROR: Socket bind failed.\n");
		return 1;
	}

	// Start listening for & accepting clients
	if (listen(serv_fd, 5) < 0) {
		printf("ERROR: Socket listening failed.\n");
		return 1;
	}
	int client_len = sizeof(cli_addr);
	newsock_fd = accept(serv_fd, (struct sockaddr *) &cli_addr, 
		(socklen_t *) &client_len);
	if (newsock_fd < 0) {
		printf("ERROR: Cannot accept client.\n");
		close(serv_fd);
		return 1;
	}
	printf("** New Connection **\n");
	printf("Socket fd : %d\nIP addr : %s\nPort : %d\n", newsock_fd,
		inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

	// Send data to the client
	char *mesg = "Hello to client!";
	send(newsock_fd, mesg, strlen(mesg), 0);
	printf("Sent: \n\t%s = %ld bytes\n", mesg, strlen(mesg));

	// Read data from client
	char buffer[BUF_SIZE];
	int valread = read(newsock_fd, buffer, BUF_SIZE);
	// Make sure the received message is a null-terminated string
	buffer[valread] = '\0';
	printf("Received: \n\t%s = %d bytes\n", buffer, valread);

	// Close connections
	close(newsock_fd);
	close(serv_fd);
	return 0;
}

// Function that sets up a TCP socket connection and
// returns the file descriptor (number) assosciated with the socket
int tcp_connect(const char *port) {
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// If socket creation failed, return now
	if (sockfd < 0)
		return sockfd;

	// Otherwise, continue setting connection variables
	// using given port number
	port_num = atoi(port);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port_num);

	// Setting the socket options to allow port reuse
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
			sizeof(opt)) < 0) {
		printf("ERROR: Setsockopt failed.\n");
	}

	return sockfd;
}
