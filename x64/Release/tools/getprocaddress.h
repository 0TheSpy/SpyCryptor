#pragma once

#include "winternl.h"
#include "DataObfuscator.h"

DWORD_PTR get_kernel32base()
{
	void* vp;
#ifndef WIN64
	PPEB peb = (PPEB)__readfsdword(0x30);
	DWORD test = (DWORD)peb->Ldr->InMemoryOrderModuleList.Flink[0].Flink->Flink + 0x10;
	vp = *(void**)test;
#else
	PPEB peb = (PPEB)__readgsqword(0x60);
	DWORD64 test = (DWORD64)peb->Ldr->InMemoryOrderModuleList.Flink[0].Flink->Flink + 0x20;
	vp = *(void**)test;
#endif
	return (DWORD_PTR)vp;
}


DWORD_PTR get_ntdllbase()
{
	void* vp;
#ifndef WIN64
	PPEB peb = (PPEB)__readfsdword(0x30);
	DWORD test = (DWORD)peb->Ldr->InMemoryOrderModuleList.Flink[0].Flink + 0x10;
	vp = *(void**)test;
#else
	PPEB peb = (PPEB)__readgsqword(0x60);
	DWORD64 test = (DWORD64)peb->Ldr->InMemoryOrderModuleList.Flink[0].Flink + 0x20;
	vp = *(void**)test;
#endif
	return (DWORD_PTR)vp;
}


DWORD get_hash(const char* str) {
	DWORD h;
	h = 0;
	while (*str) {
		h = (h >> 13) | (h << (32 - 13));       // ROR h, 13
		h += *str >= 'a' ? *str - 32 : *str;    //
		str++;
	}
	return h;
}


DWORD_PTR get_proc_address(DWORD_PTR pDLL, DWORD dwAPI)
{
	IMAGE_DOS_HEADER* pIDH = (IMAGE_DOS_HEADER*)pDLL;
	IMAGE_NT_HEADERS* pINH = (IMAGE_NT_HEADERS*)((BYTE*)pDLL + pIDH->e_lfanew);
	IMAGE_EXPORT_DIRECTORY* pIED = (IMAGE_EXPORT_DIRECTORY*)((BYTE*)pDLL + pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	DWORD* dwNames = (DWORD*)((BYTE*)pDLL + pIED->AddressOfNames);
	DWORD* dwFunctions = (DWORD*)((BYTE*)pDLL + pIED->AddressOfFunctions);
	WORD* wNameOrdinals = (WORD*)((BYTE*)pDLL + pIED->AddressOfNameOrdinals);
	for (DWORD i = 0; i < pIED->NumberOfNames; i++)
	{
		if (get_hash((char*)((BYTE*)pDLL + dwNames[i])) == dwAPI)
		{
			return (DWORD_PTR)((BYTE*)pDLL + dwFunctions[wNameOrdinals[i]]);
		}
	}

	return 0;
}

DWORD_PTR hKernel32;
DWORD_PTR hNtdll;

// find process ID by process name
#include <tlhelp32.h>
int FindProcByName(const char* processname) {
  
	printfdbg("proc %s\n",processname);
	HANDLE hSnapshot;
	PROCESSENTRY32 pe;
	int pid = 0;
	BOOL hResult;

	// snapshot of all processes in the system
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

	// initializing size: needed for using Process32First
	pe.dwSize = sizeof(PROCESSENTRY32);

	// info about first process encountered in a system snapshot
	hResult = Process32First(hSnapshot, &pe);

	// retrieve information about the processes
	// and exit if unsuccessful
	while (hResult) {
		// if we find the process: return process ID
		if (strcmp(processname, pe.szExeFile) == 0) {  //procname
			pid = pe.th32ProcessID;
			break;
		}
		hResult = Process32Next(hSnapshot, &pe);
	}

	// closes an open handle (CreateToolhelp32Snapshot)
	CloseHandle(hSnapshot);
	return pid;
} 

#include <iostream>
#include <string>
#include <sstream> 
char* GetLastErrorAsText()
{
    DWORD errorMessageID = ::GetLastError();
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);  
    //return messageBuffer; 
	std::ostringstream out;  
    out << errorMessageID << " (0x" << hex << errorMessageID << "): " << messageBuffer;
    return (char*)(out.str().c_str()); 
}

BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES oldtp;    /* old token privileges */
	TOKEN_PRIVILEGES tp;
	DWORD dwSize = sizeof(TOKEN_PRIVILEGES);
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %s\n", GetLastErrorAsText());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)&oldtp,
		(PDWORD)&dwSize))
	{
		printf("AdjustTokenPrivileges error: %s\n", GetLastErrorAsText()); //Get error 6 here (ie invalid handle)
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}
 


#include <Psapi.h> 
void GetRandomProcessPathW(wchar_t* kernelPath)
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe;
	PROCESSENTRY32 pid[1024];
	BOOL hResult;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) return;
	pe.dwSize = sizeof(PROCESSENTRY32);
	hResult = Process32First(hSnapshot, &pe);
	int iter = 0;
	while (hResult) { 
		pid[iter]=pe;
		iter++;
		hResult = Process32Next(hSnapshot, &pe);
	}
	srand((unsigned)time(NULL));
	int random = rand() % iter; 
	printfdbg("Took %d of %d | PID %d: %s\n", random, iter, pid[random].th32ProcessID, pid[random].szExeFile);
	HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, 0, pid[random].th32ProcessID);
	if (h) {
		if (GetModuleFileNameExW(h, NULL, kernelPath, MAX_PATH) != 0) 
		{
			printfdbg("OK\n"); 
		}	
		else printfdbg("(GetModuleFileNameExW error: %s\n",GetLastErrorAsText());
		CloseHandle(h);
	}
	else printfdbg("OpenProcess error: %s\n",GetLastErrorAsText());

	CloseHandle(hSnapshot);
}
