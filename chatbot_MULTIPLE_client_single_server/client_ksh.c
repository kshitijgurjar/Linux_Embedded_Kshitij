#include <sys/socket.h> //The header file socket.h includes a number of definitions of structures needed for sockets.
#include <stdio.h>
#include <string.h>
#include <unistd.h> //used for close() function
#include <arpa/inet.h>//definitions for internet operations // for including inet_ntop() function
#include <pthread.h> //POSIX thread(pthread) standard API(Application program Interface) for all thread related functions. It allows us to create multiple threads for concurrent process flow. 

char message[500];

//function to recieve the message from sender but NOT broadcasting
void *reciever_message(void *this_sock)
{
	int socketX = *((int *)this_sock);  //type casted to integer
	int msg_length;

	// client thread always ready to receive message
	while((msg_length = recv(socketX,message,500,0)) > 0) 
	//recv() function recieve a message from a connection-mode socket
    //arguments are 1.socket_file_descriptor  2. message_buffer  3.buffer(msg)_length   4.flags..if not use 0
	{
		message[msg_length] = '\0'; ////adding a null character at the end of string
		
		fputs(message,stdout);  //put the recieved string into "message"

	}
}

int main(int argc,char *argv[])
{
	pthread_t reciever_thread;
	int msg_length; //message length counter
	int socketX;
	char message_broadcaster[500];  // a message broadcaster buffer
	struct sockaddr_in SERVER_ADDR; //A "sockaddr_in" is a structure containing an internet address. This structure is defined in <netinet/in.h>.
	char client_name[100];  //a client name string buffer
	
	strcpy(client_name, argv[1]); //copy the string which ws entered compile time into "client_name"
	
	 /* The socket() system call creates a new socket.It takes three arguments.
        1. The first is the address domain of the socket. AF_INET is symbol constant for the latter.
        2. The second argument is the type of socket. SOCK_STREAM is a stream socket in which characters are read in a continuous stream as if from a file or pipe.
        3.  The third argument is the protocol. It will choose TCP protocol if set to '0'.*/
	socketX = socket( AF_INET, SOCK_STREAM,0);
	
	 //The variable 'SERVER_ADDR' is a structure of type struct sockaddr_in.This structure has four fields.

    //1.The first field is short sin_family, which contains a code for the address family. 
    //It should always be set to the symbolic constant AF_INET.
	SERVER_ADDR.sin_family= AF_INET;

	///The third field of serv_addr is unsigned short sin_port , which contain the port number.
	SERVER_ADDR.sin_port = htons(1234);

	//The second field of sockaddr_in is a structure of type struct in_addr which contains only a single field unsigned long s_addr.
    //This field contains the IP address of the host. For server code, this will always be the IP address of the machine on which the server is running,
    //and there is a symbolic constant INADDR_ANY which gets this address.
	SERVER_ADDR.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	
	// connect the client socket to server socket.
    // The connect function is called by the client to establish a connection to the server.
    // It takes three arguments, the socket file descriptor, the address of the host to which it wants to connect
    // (including the port number), and the size of this address. This function returns 0 on success and -1 if it fails. 
	if( (connect( socketX ,(struct sockaddr *)&SERVER_ADDR,sizeof(SERVER_ADDR))) == -1 )
		printf("Connection Failed : Socket");
	
	//creating a client thread which is always waiting for a message
	pthread_create(&reciever_thread,NULL,(void *)reciever_message,&socketX);
	//The pthread_create() function starts a new thread in the calling process.
        //1.    &receiver_thread is address of client which is connected to server as thread
        //2.    NULL is default attribute
        //3.    "reciever_message" is the ||||start routine||||
        //4.    "&client_socket" is the argument to ||||start routine||||
	


	//some display messages
	printf("_______________________\n");
	printf("%s 's CHAT ROOM \n",client_name);
	printf("_______________________\n");
	printf( "Waiting for msgs...\n");
	printf("\nYour msg should be typed in the empty console :\n");
	
	
	//ready to read a message from console
	while(fgets(message,500,stdin) > 0) //get the message from sender(stdin file) and store it into "message" buffer string
	{
		
		strcpy(message_broadcaster,client_name); //copy the client name into message_broadcaster
		
		strcat(message_broadcaster,"  says : "); //concatenate with " says : "

		strcat(message_broadcaster,message);     // concatenate with the message to be broadcasted

		//broadcast the message to other clients via server
		msg_length = write(socketX,message_broadcaster,strlen(message_broadcaster));
		//a variable to ensure message is broadcasted or not
		
		if(msg_length < 0)  // it must not be -1, which means FAILed to send
			printf("Message NOT SENT");
	}
	
	//thread is closed
	pthread_join(reciever_thread,NULL);
	//The pthread_join() function shall suspend execution of the calling thread until the target thread terminates,
	// unless the target thread has already terminated.

	close(socketX); //closing the socket
	return 0;
}
