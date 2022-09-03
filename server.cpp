#include "header.h"
//tren may hacker


int main(){
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	//if(iResult){
	//	std::cout << "Failed to create wsaData: " << iResult;
	//}

	SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;


    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    ClientSocket = accept(ListenSocket, NULL, NULL);
    std::cout << "\nDETECTED\n";
    string rs;

    char buffer[30000];
    char receive[30000];
    memset(receive, 0, sizeof(receive));
    while(1){
        memset(buffer, 0, sizeof(buffer));  
        memset(receive, 0, sizeof(receive));
        gets(buffer);

        iResult = send( ClientSocket, buffer, (int)strlen(buffer), 0 );
       //std::cout << "Send: " << iResult;
        //Sleep(100);
        iResult = 0;
        iResult = recv(ClientSocket, receive, 30000, 0);

        std::cout << "\nMessage receive from server: " << receive << "\n";
        //fflush(stdin); 
        if(!iResult){
            std::cout << receive << " " << strlen(receive);
            std::cout << "\nLost Connection";
            TerminateProcess(GetCurrentProcess(), 0);
        }


    }

    WSACleanup();
    return 0;
}