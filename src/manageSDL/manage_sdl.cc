// ManageSDL.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#define  ManageMSPSupport

char szSDLName[] = "smartDeviceLinkCore.exe";
char szManagesDLName[] = "ManageSDL.exe";
char szSDL[MAX_PATH];
char szEvent[] = "ManageSDLStartStopEvent";

#ifdef ManageMSPSupport
#define MAX_HANDLE   4
char szMSPName[] = "SDLMspVR.exe";
char szMSP[MAX_PATH];
char szMEvent[] = "ManageMSPStartStopEvent";
#else
#define MAX_HANDLE   2
#endif


void BuildSDLCmd()
{
	::GetModuleFileNameA(0, szSDL, MAX_PATH);
	int endpos = strlen(szSDL);
	while (szSDL[endpos - 1] != '\\')
		endpos--;
	szSDL[endpos] = '\0';
	strcat(szSDL, szSDLName);
}

#ifdef ManageMSPSupport
void BuildMSPCmd()
{
	::GetModuleFileNameA(0, szMSP, MAX_PATH);
	int endpos = strlen(szMSP);
	while (szMSP[endpos - 1] != '\\')
		endpos--;
	szMSP[endpos] = '\0';
	strcat(szMSP, szMSPName);
}
#endif

void CloseExist(char * pName)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	bool bFind = false;
	do
	{
		bFind = false;

		// Take a snapshot of all processes in the system.
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			return;
		}

		// Set the size of the structure before using it.
		pe32.dwSize = sizeof(PROCESSENTRY32);

		// Retrieve information about the first process,
		// and exit if unsuccessful
		if (!Process32First(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap);          // clean the snapshot object
			return;
		}

		// Now walk the snapshot of processes, and
		// display information about each process in turn
		do
		{
			// Retrieve the priority class.
			dwPriorityClass = 0;
			if (::strstr(pe32.szExeFile, pName) != NULL)
			{
				bFind = true;
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				::TerminateProcess(hProcess, 0);
				printf("Terminated one process %s\n", pName);
				Sleep(1000);
				CloseHandle(hProcess);
			}
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);
	} while (bFind);
}


int main(int argc, char* argv[])
{
	STARTUPINFO info;
	memset(&info, 0, sizeof(info));
	info.cb = sizeof(info);
	HANDLE hEvent = ::CreateEventA(0, 0, 0, szEvent);

#ifdef ManageMSPSupport
	STARTUPINFO msp_info;
	memset(&msp_info, 0, sizeof(msp_info));
	msp_info.cb = sizeof(msp_info);
	HANDLE msp_hEvent = ::CreateEventA(0, 0, 0, szMEvent);
#endif

	if (argc >= 2 && ((strcmp(argv[1], "start") == 0) || (strcmp(argv[1], "startW") == 0)))
	{
		::SetEvent(hEvent);
		::ResetEvent(hEvent);
		CloseExist(szSDLName);
		BuildSDLCmd();
#ifdef ManageMSPSupport
		::SetEvent(msp_hEvent);
		::ResetEvent(msp_hEvent);
		CloseExist(szMSPName);
		BuildMSPCmd();
#endif

		int iWin = 0;
		if (strcmp(argv[1], "start") == 0)
			iWin = CREATE_NO_WINDOW;
		PROCESS_INFORMATION SDLProcess;
		if (false == ::CreateProcessA(0, szSDL, NULL, NULL, false, iWin, 0, NULL, (LPSTARTUPINFOA)&info, &SDLProcess))
		{
			::printf("failed to create the process\r\n");
			::printf("%d", GetLastError());
			::CloseHandle(hEvent);
			return 1;
		}
#ifdef ManageMSPSupport
		PROCESS_INFORMATION MSPProcess;
		if (false == ::CreateProcessA(0, szMSP, NULL, NULL, false, iWin, 0, NULL, (LPSTARTUPINFOA)&msp_info, &MSPProcess))
		{
			::printf("failed to create the process\r\n");
			::printf("%d", GetLastError());
			::CloseHandle(msp_hEvent);
			return 1;
		}
#endif

		HANDLE pHandles[MAX_HANDLE];
		while (1)
		{
			int i = 0;
			pHandles[i++] = hEvent;
			pHandles[i++] = SDLProcess.hProcess;
#ifdef ManageMSPSupport
			pHandles[i++] = msp_hEvent;
			pHandles[i++] = MSPProcess.hProcess;
#endif
			DWORD dwRet = ::WaitForMultipleObjects(MAX_HANDLE, pHandles, false, INFINITE);

			if (dwRet == (WAIT_OBJECT_0 + 1))
			{
				::printf("SDL crashed, restart.\r\n");
				int count = 10;
				while (count > 0)
				{
					if (false == ::CreateProcessA(0, szSDL, NULL, NULL, false, iWin, 0, NULL, (LPSTARTUPINFOA)&info, &SDLProcess))
					{
						count--; Sleep(1000);
					}
					else
						break;
				}
				if (count == 0)
				{
					::printf("failed to create the process\r\n");
					break;
				}
				continue;
			}
#ifdef ManageMSPSupport
			else if ((dwRet == (WAIT_OBJECT_0 + 3)))
			{
				::printf("MSP vr crashed, restart.\r\n");
				int count = 10;
				while (count > 0)
				{
					if (false == ::CreateProcessA(0, szMSP, NULL, NULL, false, iWin, 0, NULL, (LPSTARTUPINFOA)&info, &MSPProcess))
					{
						count--; Sleep(1000);
					}
					else
						break;
				}
				if (count == 0)
				{
					::printf("failed to create the process\r\n");
					break;
				}
				continue;
			}
#endif
			::TerminateProcess(pHandles[1], 0);
			::printf("Killed SDL\r\n");
#ifdef ManageMSPSupport
			::TerminateProcess(pHandles[3], 0);
			::printf("Killed MSP\r\n");
#endif
			break;
		}

	}
	else
	{
		::SetEvent(hEvent);
#ifdef ManageMSPSupport
		::SetEvent(msp_hEvent);
#endif
		::printf("Send out close event.\r\n");
	}
	::CloseHandle(hEvent);
	return 0;
}

