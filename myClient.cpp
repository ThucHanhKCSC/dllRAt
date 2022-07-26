#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
//#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int main(int argc, char **argv){


	 WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char sendbuf[] = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    char buffer[30000];
    char receive[30000];
    memset(receive, 0, sizeof(receive));
    while(1){
    	memset(buffer, 0, sizeof(buffer));	
    	memset(receive, 0, sizeof(receive));
    	gets(buffer);

    	iResult = send( ConnectSocket, buffer, (int)strlen(buffer), 0 );
       //std::cout << "Send: " << iResult;
    	//Sleep(100);
        iResult = 0;
    	iResult = recv(ConnectSocket, receive, 30000, 0);

    	std::cout << "\nMessage receive from server: " << receive << "\n";
    	//fflush(stdin); 
    	if(!iResult){
    		std::cout << receive << " " << strlen(receive);
    		std::cout << "\nLost Connection";
    		TerminateProcess(GetCurrentProcess(), 0);
    	}


    }




    freeaddrinfo(result);




	return 0;
}