#include <sys/socket.h>//The header file socket.h includes a number of definitions of structures needed for sockets.
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> //definitions for internet operations // for including inet_ntop() function
#include <pthread.h> //POSIX thread(pthread) standard API(Application program Interface) for all thread related functions. It allows us to create multiple threads for concurrent process flow. 

pthread_mutex_t MUTEX;  vv //a mutex is created
int connected_CLIENTS[20]; //max 20 clients are made available
int client_count=0;        // counter to keep track of number of clients


//this function is used to broadcast message to other clients
//the current client is the owner of server by creating a MUTEX
void broadcast_message(char *message,int current_client)
{
    int loop_count;  //loop counter till client count ends

    pthread_mutex_lock(&MUTEX); 
    //Locks a mutex object, which identifies a mutex. If the mutex is already locked by another thread, the thread waits for the mutex to become available. 
    //The thread that has locked a mutex becomes its current owner and remains the owner until the same thread has unlocked it.


    //this loop runs as number of clients are added(connected)
    for(loop_count = 0; loop_count < client_count; loop_count++) 
    {
        if(connected_CLIENTS[loop_count] != current_client) {
            if(send(connected_CLIENTS[loop_count],message,strlen(message),0) < 0) //send function checks that message is send or NOT, returns -1 if fails
            {
                printf("Fail to send message \n");
                continue;
            }
        }
    }
    pthread_mutex_unlock(&MUTEX);
    //Releases a mutex object.
    // If one or more threads are waiting to lock the mutex, pthread_mutex_unlock() causes one of those threads
    // to return from pthread_mutex_lock() with the mutex object acquired
}


//function to recieve message and broadcast it to other clients (this is what server do)
void *reciever_message(void *client_socket)   //broadcaster client socket address is passed as reference
{
    int socketX = *((int *)client_socket);  //type casted to integer

    char message[500];   //message buffer

    int message_length;  //recieved message length from broadcaster

    while((message_length = recv(socketX,message,500,0)) > 0) 
    //recv() function recieve a message from a connection-mode socket
    //arguments are 1.socket_file_descriptor  2. message_buffer  3.buffer(msg)_length   4.flags..if not use 0
    {
        message[message_length] = '\0';  //adding a null character at the end of string
        broadcast_message(message,socketX);  //broadcasting the message to other sockets(client)
    }

}

int main()
{
    struct sockaddr_in SERVER_ADDR; //A "sockaddr_in" is a structure containing an internet address. This structure is defined in <netinet/in.h>.
    pthread_t reciever_thread;       
    int socketX=0 , client_socket=0;


    //The variable 'SERVER_ADDR' is a structure of type struct sockaddr_in.This structure has four fields.

    //1.The first field is short sin_family, which contains a code for the address family. 
    //It should always be set to the symbolic constant AF_INET.
    SERVER_ADDR.sin_family = AF_INET;

    ///The third field of serv_addr is unsigned short sin_port , which contain the port number.
    SERVER_ADDR.sin_port = htons(1234); //htons: integer byte order to network byte order(port)

    //The second field of sockaddr_in is a structure of type struct in_addr which contains only a single field unsigned long s_addr.
    //This field contains the IP address of the host. For server code, this will always be the IP address of the machine on which the server is running,
    //and there is a symbolic constant INADDR_ANY which gets this address.
    SERVER_ADDR.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* The socket() system call creates a new socket.It takes three arguments.
        1. The first is the address domain of the socket. AF_INET is symbol constant for the latter.
        2. The second argument is the type of socket. SOCK_STREAM is a stream socket in which characters are read in a continuous stream as if from a file or pipe.
        3.  The third argument is the protocol. It will choose TCP protocol if set to '0'.*/
    socketX = socket( AF_INET , SOCK_STREAM, 0 );

    // The bind() system call binds a socket to an address, in this case the address of the current host and port number on which the server will run.
    // It takes three arguments, the socket file descriptor, the address to which is bound, and the size of the address to which it is bound.
    // The second argument is a pointer to a structure of type sockaddr, but what is passed in is a structure of type sockaddr_in, and so this must be cast to the correct type.
    if( bind( socketX, (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) == -1 )
        printf("Binding Error!! \n");
    else
    {
        printf("Server is ONLINE...\nConnections are being established\n");
        printf("Create other clients now\n");
        printf(" Help: compile the client code as follows:\n");
        printf("1. gcc client_ksh.c -pthread\n");
        printf("2. ./client_ksh client_name\n");
        printf("\twhere client_name must be replaced by string say Kshitij");
    }

    // Now server is ready to listen and verification 
    // The listen system call allows the process to listen on the socket for connections.
    // The first argument is the socket file descriptor, and the second is the size of the backlog queue,
    // i.e., the number of connections that can be waiting while the process is handling a particular connection.
    if( listen( socketX ,20 ) == -1 )
        printf("listening failed \n");

    while(1)
    {
    // Accept the data packet from client and verify it
    // The accept() system call causes the process to block until a client connects to the server.
    // Thus, it wakes up the process when a connection from a client has been successfully established.
    // It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor.
    // The second argument is a reference pointer to the address of the client on the other end of the connection, and the third argument is the size of this structure.
        if( (client_socket = accept(socketX, (struct sockaddr *)NULL,NULL)) < 0 )
            printf("accept failed  \n");

        pthread_mutex_lock(&MUTEX);

        //store the socket number
        connected_CLIENTS[client_count]= client_socket;

        //increment client count
        client_count++;

        // creating a thread for each client 
        pthread_create(&reciever_thread,NULL,(void *)reciever_message,&client_socket);
        //The pthread_create() function starts a new thread in the calling process.
        //1.    &receiver_thread is address of client which is connected to server as thread
        //2.    NULL is default attribute
        //3.    "reciever_message" is the ||||start routine||||
        //4.    "&client_socket" is the argument to ||||start routine||||



        pthread_mutex_unlock(&MUTEX);
        //unlock the mutex and transfer its(server's) ownership
    }
    return 0; 

}
