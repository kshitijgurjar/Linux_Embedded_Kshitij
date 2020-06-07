#include <netdb.h> //This header shall define macros to evaluate o bitwise-distinct integers constants.
//comman input-output,strings header files
#include <stdio.h> 
#include <arpa/inet.h>
#include <stdlib.h> 
#include<unistd.h>
#include <string.h> 
#include <sys/socket.h> //The header file socket.h includes a number of definitions of structures needed for sockets.
#define LENGTH 256 
#define SOCKET_ADDRESS struct sockaddr 

void chat_function(int socket1) 
{ 
	char buffer[LENGTH]; // The client reads characters from the socket connection into this buffer.
	int count;           // counter to count number of characters in the message
	while(1) 
    { 
        //The function bzero() sets all values in a buffer to zero.
		bzero(buffer, sizeof(buffer)); 


		printf("Enter the string : "); 
		count = 0; 

        // server message is copied to the buffer using continous iteration
        // here each character is taken from message using getchar() and stored in proper index of buffer array 
		while ((buffer[count++] = getchar()) != '\n') ;

        // and send that buffer to server
		write(socket1, buffer, sizeof(buffer));

        //The function bzero() sets all values in a buffer to zero.
        //the read() will block until there is something for it to read in the socket, i.e. after the client has executed a write().
        // It will read either the total number of characters in the socket or 255, whichever is less, and return the number of characters read. 
		bzero(buffer, sizeof(buffer)); 
		read(socket1, buffer, sizeof(buffer)); 

		printf("From Server : %s", buffer); 
		if ((strncmp(buffer, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 

// MAIN function
int main() 
{ 
	int socket1, connfd; 
	struct sockaddr_in server_address, client; 
    int PORT ;

    //The socket() system call creates a new socket.
	socket1 = socket(AF_INET, SOCK_STREAM, 0); 

    //Check whether the socket is sucessfully created or NOT
	if (socket1 == -1) 
    { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 

    //The function bzero() sets all values in a buffer to zero.
	bzero(&server_address, sizeof(server_address)); 

    //read the port number
    printf("Enter port number which was entered with server\n");
    scanf("%d",&PORT);

	// mention the IP and PORT 
	server_address.sin_family = AF_INET; 
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");   //IP address of the host
	server_address.sin_port = htons(PORT); 

	// connect the client socket to server socket.
    // The connect function is called by the client to establish a connection to the server.
    // It takes three arguments, the socket file descriptor, the address of the host to which it wants to connect
    // (including the port number), and the size of this address. This function returns 0 on success and -1 if it fails. 
	if (connect(socket1, (SOCKET_ADDRESS*)&server_address, sizeof(server_address)) != 0) 
    { 
		printf("Connection FAILED with the Server...\n"); 
		exit(0); 
	} 
	else
		printf("SUCESSFULLY connected to Server..\n"); 

	// function for chat 
	chat_function(socket1); 

	// close the socket 
	close(socket1); 
} 
