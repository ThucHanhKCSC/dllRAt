@echo off
g++ dllMain.cpp -shared -lwsock32 -lWs2_32 -o wsc.dll