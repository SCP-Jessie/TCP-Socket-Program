#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
//gcc -Wall -Werror -std=c99 -o Client Client.c -lm
int main(int argc , char *argv[])
{
	int sock;
	// sockaddr_in struct holds address info
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	//htons() - host to network - store in network byte order
	server.sin_port = htons( 9999 );

	//Connect to remote server
	// unsuccessful socket connect returns -1 success: 0
	// Corresponds to accept() in server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	//keep communicating with server
	while(1)
	{
		printf("Enter message : ");
		//scanf("%s" , message);
		//gets continues through whitespaces while scanf does not
		gets(message);

		//Send some data
		// Received by recv() on server side 
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}

		//Receive a reply from the server - specifically server's send()
		if(recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}

		puts("Server reply :");
		puts(server_reply);
    memset(server_reply, 0, 2000);
	}

  //Close server if client is closed
	close(sock);
	return 0;
}
