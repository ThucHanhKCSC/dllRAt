@echo off

g++ client.cpp -shared -lwsock32 -lWs2_32 -o wsc.dll
g++ server.cpp -lwsock32 -lWs2_32 -o server.exe