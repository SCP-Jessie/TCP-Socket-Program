#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	// store server and client addresses
	struct sockaddr_in server_addr , client_addr;
	char client_message[2000];

	//STEP 1: Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created successfully");

	//Set port and IP:
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons( 9999 );

	//STEP 2: Bind socket to server_addr
	if( bind(socket_desc,(struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
	{
		perror("bind failed. Error");
		return 1;
	}
	puts("bind completed");

	//STEP 3: Listen for incoming connections - mark socket_desc as available for connections
	//once called, socket_desc can no longer be used as an active socket to innitiate connection requests
	// socket_desc becomes passive socket
	// Max length of pending queues is 3
	listen(socket_desc , 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//STEP 4: Accept connection from an incoming client_addr.  Server will wait here for client connection from connect()
	// Return new socket descriptor with same properties as socket_desc. client_sock cannot accept new connections
	// socket_desc becomes avaiable to accept new connections again
	// Take first connection for given socket socket_desc
	// Return -1 in case of failure
	// Accept can take multiple connection requests, so it keeps socket_desc open for other
	//requests while client_sock is used ro participate in the current connection interaction
	client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");

	//Receive a message from client_addr
	// If successful, return length of message. Else -1
	// stops at recv() to wait for message from client - specifically wait for send() from client
	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	{

		//Send the message back to client_addr - modified write to just read_size for accurate messages
		// write(client_sock , client_message , strlen(client_message));
		//similar to send() method
    write(client_sock , client_message , read_size);

    memset(client_message, 0, 2000);
	}

	if(read_size == 0)
	{
		puts("client_addr disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}

	close(socket_desc);
	return 0;
}
