#pragma once

#include "winternl.h"

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
int FindProcByName(const char* procname) {

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
		if (strcmp(procname, pe.szExeFile) == 0) {  
			pid = pe.th32ProcessID;
			break;
		}
		hResult = Process32Next(hSnapshot, &pe);
	}

	// closes an open handle (CreateToolhelp32Snapshot)
	CloseHandle(hSnapshot);
	return pid;
} 