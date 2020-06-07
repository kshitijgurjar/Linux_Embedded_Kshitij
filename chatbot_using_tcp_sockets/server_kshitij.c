#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> //The header file netinet/in.h contains constants and structures needed for internet domain addresses.
#include <stdlib.h> 
#include<unistd.h>
#include <string.h> 
#include <sys/socket.h> //The header file socket.h includes a number of definitions of structures needed for sockets.
#include <sys/types.h>  //This header file contains definitions of a number of data types used in system calls. These types are used in the next two include files.
#define LENGTH 256 
#define SOCKET_ADDRESS struct sockaddr 
  
// function for chat between client and server. 
void chat_function (int socket1) 
{ 
    char buffer[LENGTH];   //The server reads characters from the socket connection into this buffer.
    int count ;            // counter to count number of characters in the message

    // infinite loop to continue the conversation 
    while(1) 
    { 
        bzero(buffer, LENGTH); 
  
        // read the message from client and copy it in buffer 
        read(socket1, buffer, sizeof(buffer));

        // print buffer which contains the client contents 
        printf("From client: %s             Server : ", buffer); 

        bzero(buffer, LENGTH); 
        count = 0; 

        // server message is copied to the buffer using continous iteration
        // here each character is taken from message using getchar() and stored in proper index of buffer array 
        while ((buffer[count++] = getchar()) != '\n')  ; 
  
        // and send that buffer to client 
        write(socket1, buffer, sizeof(buffer)); 
  
        // if msg contains "exit" then server exit and chat ended. 
        if (strncmp("exit", buffer, 4) == 0) 
        { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 
  
// MAIN function 
int main() 
{ 
    int socket1, connection, length; 
    struct sockaddr_in server_address, client; //A "sockaddr_in" is a structure containing an internet address. This structure is defined in <netinet/in.h>.
    int PORT;
  
    // socket create and verification 
    socket1 = socket(AF_INET, SOCK_STREAM, 0); 
    /* The socket() system call creates a new socket.It takes three arguments.
        1. The first is the address domain of the socket. AF_INET is symbol constant for the latter.
        2. The second argument is the type of socket. SOCK_STREAM is a stream socket in which characters are read in a continuous stream as if from a file or pipe.
        3.  The third argument is the protocol. It will choose TCP protocol if set to '0'.*/

    if (socket1 == -1) 
    { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 

    //The function bzero() sets all values in a buffer to zero. It takes two arguments, 
    //the first is a pointer to the buffer and the second is the size of the buffer.
    bzero(&server_address, sizeof(server_address)); 
    
  
    printf("Enter port number.It must be between 2000-65535\n");
    scanf("%d",&PORT);

    // set the IP and PORT 
    //The variable serv_address is a structure of type struct sockaddr_in.This structure has four fields.

    //1.The first field is short sin_family, which contains a code for the address family. It should always be set to the symbolic constant AF_INET.
    server_address.sin_family = AF_INET; 

    //The second field of sockaddr_in is a structure of type struct in_addr which contains only a single field unsigned long s_addr.
    // This field contains the IP address of the host. For server code, this will always be the IP address of the machine on which the server is running,
    // and there is a symbolic constant INADDR_ANY which gets this address.
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 

    //The third field of serv_addr is unsigned short sin_port , which contain the port number.
    server_address.sin_port = htons(PORT); 
  
    // The bind() system call binds a socket to an address, in this case the address of the current host and port number on which the server will run.
    // It takes three arguments, the socket file descriptor, the address to which is bound, and the size of the address to which it is bound.
    // The second argument is a pointer to a structure of type sockaddr, but what is passed in is a structure of type sockaddr_in, and so this must be cast to the correct type.
    if ((bind(socket1, (SOCKET_ADDRESS*)&server_address, sizeof(server_address))) != 0) { 
        printf("Socket binding failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    //The listen system call allows the process to listen on the socket for connections.
    // The first argument is the socket file descriptor, and the second is the size of the backlog queue,
    // i.e., the number of connections that can be waiting while the process is handling a particular connection.
    if ((listen(socket1, 5)) != 0)  //This should be set to 5, the maximum size permitted by most systems.
    { 
        printf("Listining failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server is listening..\n"); 

    length = sizeof(client); 
  
    // Accept the data packet from client and verification 
    // The accept() system call causes the process to block until a client connects to the server.
    // Thus, it wakes up the process when a connection from a client has been successfully established.
    // It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor.
    // The second argument is a reference pointer to the address of the client on the other end of the connection, and the third argument is the size of this structure.
    connection = accept(socket1, (SOCKET_ADDRESS*)&client, &length); 


    if (connection < 0) { 
        printf("Server failed to accept...\n"); 
        exit(0); 
    } 
    else
        printf("Server Client connection established...\n"); 
  
    // Function for chatting between client and server 
    chat_function (connection); 
  
    // After chatting close the socket 
    close(socket1); 
} 
