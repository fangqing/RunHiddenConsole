//Hide console windows of started program in windows
//
//the raw souce code is copyed from  https ://forums.hak5.org/topic/7857-possible-alternate-app-for-hiding-console-windows/   
//and modify as follow :
//1.change the code to unicode supported
//2.delete the 't' argument from the raw code(I really don't know what role does the argument  play)
// https://github.com/fangqing/RunHiddenConsole
#include <process.h>
#include <windows.h>
#define HELP_INFO  L"About: hide console windows of started program.\n \
Usage: RunHiddenConsole.exe [/w] commandline \n\
for example:\n RunHiddenConsole.exe  /w  C:\\PHP\\php-cgi.exe -b 127.0.0.1:9123 \n\
 RunHiddenConsole.exe  /w  python  d:\\test.py \n \
the /w is optional which  means waiting for termination of the program launched by the commandline"
					
void main()
{
	int bWait = 0;
	DWORD exitcode = 0;
	WCHAR stopchar = L' ';
	WCHAR* lpszCmd = GetCommandLineW();
	if (lpszCmd[0] == L'"') 
		stopchar = L'"';
	do { 
		lpszCmd++; 
	} while ((lpszCmd[0] != stopchar)&&(lpszCmd[0] != 0));

	if (lpszCmd[0] != 0)
	{
		do {
			lpszCmd++; 
		} while ((lpszCmd[0] != 0)&&((lpszCmd[0] == L' ')));
	};
	if (lpszCmd[0] == 0)
	{
		MessageBoxW(0, HELP_INFO,L"Incorrect usage", 0);
		ExitProcess(0);
	};

	//search the "/w" option
	if((lpszCmd[0] == L'/')&& (lpszCmd[1]== L'w'|| lpszCmd[1] == L'W')&&(lpszCmd[2] == L' '))
	{
		bWait = 1;
		lpszCmd += 3;
	};

	//skip the space
	while ((lpszCmd[0] != 0) && (lpszCmd[0] == ' '))
		lpszCmd++;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	if (CreateProcessW(NULL, lpszCmd,
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE,NULL, NULL,&si, &pi))
	{
		if (bWait) 
			WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
		exitcode = GetLastError();

	ExitProcess(exitcode);
}
