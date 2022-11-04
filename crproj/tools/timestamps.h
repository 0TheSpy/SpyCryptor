#pragma once

#include "windows.h"
#include "time.h"
#include "stdio.h"

#include <iostream>
#include <fstream>
#include <filesystem> 
#include <random> 

#define MAX_RESOURCE_TYPE_STRING_LENGTH 100
#define MAX_RESOURCE_LANG_STRING_LENGTH 50

struct WSTRINGW
{
	unsigned short Length;
	wchar_t Buffer[2];
};


struct CV_HEADER
{
	unsigned long Signature;
	unsigned long Offset;
};

typedef struct _CV_INFO_PDB20
{
	CV_HEADER CvHeader;
	unsigned long TimeDateStamp;
	unsigned long Age;
	char PdbFileName[MAX_PATH];
}CV_INFO_PDB20;


#define INVALID_SET_FILE_POINTER 0xFFFFFFFF

bool ResolveIsWow64Process();
bool ResolveWow64DisableWow64FsRedirection();
unsigned char* DecorateTime(unsigned char* pTimeStr);
char* PrintTimeDateStamp(char* argStr);
char* PrintTimeDateStamp(unsigned long vTimeDateStamp);
char* GetBasicResourceType(unsigned long Type);
char* GetTimeDateStamp_s(unsigned long vTimeDateStamp);
void CloseHandles(HANDLE hMapping, HANDLE hFile);
void UnmapInputFileAndCloseHandles(void* pMap, HANDLE hMapping, HANDLE hFile);
unsigned char* MapInputFile(char* pFileName, HANDLE* phFile, HANDLE* phMapping);
void PrintDebugType(unsigned long Type);
void PrintWithLevel(unsigned long Level, unsigned char* pString);
size_t VirtualToRaw(size_t VA, IMAGE_SECTION_HEADER* pSects, int iSectCount);


typedef BOOL(__stdcall* lpIsWow64Process)(HANDLE, BOOL*);
typedef BOOL(__stdcall* lpWow64DisableWow64FsRedirection)(BOOL*);

lpIsWow64Process pIsWow64Process;
lpWow64DisableWow64FsRedirection pWow64DisableWow64FsRedirection;

bool ResolveIsWow64Process()
{
	HMODULE hKern32 = GetModuleHandle(L"kernel32.dll");
	if (!hKern32) return false;
	pIsWow64Process = (lpIsWow64Process)GetProcAddress(hKern32, "IsWow64Process");
	if (!IsWow64Process) return false;
	return true;
}

bool ResolveWow64DisableWow64FsRedirection()
{
	HMODULE hKern32 = GetModuleHandle(L"kernel32.dll");
	if (!hKern32) return false;
	pWow64DisableWow64FsRedirection = (lpWow64DisableWow64FsRedirection)GetProcAddress(hKern32, "Wow64DisableWow64FsRedirection");
	if (!pWow64DisableWow64FsRedirection) return false;
	return true;
}

unsigned char* DecorateTime(unsigned char* pTimeStr)
{
	if (!pTimeStr) return 0;
	unsigned long length = strlen((char*)pTimeStr);
	if (length == 0) return 0;
	if ((pTimeStr[length - 1] == 0x0D) || (pTimeStr[length - 1] == 0x0A)) pTimeStr[length - 1] = 0;
	return pTimeStr;
}

//Input is a string
char* PrintTimeDateStamp(char* argStr)
{
	unsigned long Length = strlen(argStr);
	if (Length > 10)
	{
		printf("Invalid input\r\n");
		return 0;
	}
	unsigned long Value = strtoul(argStr, 0, 0x10);
	time_t TimeX = (time_t)Value;

	tm* pGMT = gmtime(&TimeX);//remove this if incompatible

	//char* pTime = (char*)DecorateTime((unsigned char*)ctime(&TimeX));
	char* pTime = (char*)DecorateTime((unsigned char*)asctime(pGMT));
	if (!pTime)
	{
		pTime = (char*)"(Invalid or Spoofed TimeDateStamp)";
		printf("%s %X\r\n", pTime, Value);
	}
	else   printf("%s \r\n", pTime);
	return pTime;
}


char* PrintTimeDateStamp(unsigned long vTimeDateStamp)
{
	if (!vTimeDateStamp)
	{
		printf("TimeDateStamp: Empty\r\n");
		return 0;
	}

	time_t TimeX = (time_t)vTimeDateStamp;
	tm* pGMT = gmtime(&TimeX); //remove this if incompatible

	//char* pTime = (char*)DecorateTime((unsigned char*)ctime(&TimeX));
	char* pTime = (char*)DecorateTime((unsigned char*)asctime(pGMT));

	if (!pTime)
	{
		pTime = (char*)"(Invalid or Spoofed TimeDateStamp)";
		printf("%s %X\r\n", pTime, vTimeDateStamp);
	}
	else	printf(" %s\r\n", pTime);
	return pTime;
}

#define RT_MANIFEST 24
char* GetBasicResourceType(unsigned long Type)
{
	char* pType = 0;
	if (Type == 1) pType = (char*)"RT_CURSOR";   //1
	else if (Type == 2) pType = (char*)"RT_BITMAP"; //2
	else if (Type == 3) pType = (char*)"RT_ICON"; //3
	else if (Type == 4) pType = (char*)"RT_MENU"; //4
	else if (Type == 5) pType = (char*)"RT_DIALOG"; //5
	else if (Type == 6) pType = (char*)"RT_STRING"; //6
	else if (Type == 7) pType = (char*)"RT_FONTDIR"; //7
	else if (Type == 8) pType = (char*)"RT_FONT"; //8
	else if (Type == 9) pType = (char*)"RT_ACCELERATOR"; //9
	else if (Type == 10) pType = (char*)"RT_RCDATA"; //10
	else if (Type == 11) pType = (char*)"RT_MESSAGETABLE"; //11
	else if (Type == 12) pType = (char*)"RT_GROUP_CURSOR"; //12

	else if (Type == 14) pType = (char*)"RT_GROUP_ICON"; //14

	else if (Type == 16) pType = (char*)"RT_VERSION"; //16
	else if (Type == 17) pType = (char*)"RT_DLGINCLUDE"; //17

	else if (Type == 19) pType = (char*)"RT_PLUGPLAY"; //19
	else if (Type == 20) pType = (char*)"RT_VXD"; //20
	else if (Type == 21) pType = (char*)"RT_ANICURSOR"; //21
	else if (Type == 22) pType = (char*)"RT_ANIICON"; //22
	else if (Type == 23) pType = (char*)"RT_HTML"; //23
	else if (Type == 24) pType = (char*)"RT_MANIFEST"; //24
	else pType = 0;
	return pType;
}

char* GetTimeDateStamp_s(unsigned long vTimeDateStamp)
{
	if (!vTimeDateStamp)
	{
		printf("TimeDateStamp: Empty\r\n");
		return 0;
	}

	time_t TimeX = (time_t)vTimeDateStamp;

	tm* pGMT = gmtime(&TimeX);//remove this if incompatible

	//char* pTime = (char*)DecorateTime((unsigned char*)ctime(&TimeX));
	char* pTime = (char*)DecorateTime((unsigned char*)asctime(pGMT));

	if (!pTime)
	{
		pTime = (char*)"(Invalid or Spoofed TimeDateStamp)";
	}
	return pTime;
}

void CloseHandles(HANDLE hMapping, HANDLE hFile)
{
	if (hMapping) CloseHandle(hMapping);
	if (hFile != INVALID_HANDLE_VALUE)  CloseHandle(hFile);
}

void UnmapInputFileAndCloseHandles(void* pMap, HANDLE hMapping, HANDLE hFile)
{
	if (pMap) UnmapViewOfFile(pMap);
	CloseHandles(hMapping, hFile);
}

unsigned char* MapInputFile(char* pFileName, HANDLE* phFile, HANDLE* phMapping)
{
	HANDLE hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Can't Open Input File Error: %#x\r\n", GetLastError());
		return 0;
	}
	else { if (phFile) { *phFile = hFile; } }

	unsigned long FileSizeHigh = 0;
	unsigned long InputFileSize = GetFileSize(hFile, &FileSizeHigh);
	if (FileSizeHigh)
	{
		printf("Input File Size Is Too Huge Try Splitting It\r\n");
		CloseHandle(hFile);
		return 0;
	}
	else
	{
		if (InputFileSize == 0)
		{
			printf("Input File Is Empty\r\n");
			CloseHandle(hFile);
			return 0;
		}
	}
	HANDLE hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY | SEC_IMAGE, 0, 0, 0);
	if (hMapping == 0)
	{
		printf("Can't Create A Memory Mapping Object For Input File Error: %#x\r\n", GetLastError());
		CloseHandle(hFile);
		return 0;
	}
	else { if (phMapping) { *phMapping = hMapping; } }

	unsigned char* pMapX = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (pMapX == 0)
	{
		printf("Can't Map Input File Into Memory: %#x\r\n", GetLastError());
		CloseHandles(hMapping, hFile);
		return 0;
	}
	return pMapX;
}

void PrintDebugType(unsigned long Type)
{
	if (Type == 0)         printf("%s", "UNKNOWN");
	else if (Type == 1)    printf("%s", "COFF");
	else if (Type == 2)    printf("%s", "CODEVIEW");
	else if (Type == 3)    printf("%s", "FPO");
	else if (Type == 4)    printf("%s", "MISC");
	else if (Type == 5)    printf("%s", "EXCEPTION");
	else if (Type == 6)    printf("%s", "FIXUP");
	else if (Type == 7)    printf("%s", "OMAP_TO_SRC");
	else if (Type == 8)    printf("%s", "OMAP_FROM_SRC");
	else if (Type == 9)    printf("%s", "BORLAND");
	else                printf("%s", "RESERVED");
	return;
}

//For printing resource directories
void PrintWithLevel(unsigned long Level, unsigned char* pString)
{
	unsigned char Buffer[102] = { 0 };
	Buffer[0] = '>';

	for (unsigned long i = 0; Level < 100, i <= Level; i++)
	{
		Buffer[i + 1] = '>';
	}

	printf("%s %s\r\n", Buffer, pString);

	return;
}

size_t VirtualToRaw(size_t VA, IMAGE_SECTION_HEADER* pSects, int iSectCount)
{
	for (int i = 0; i < iSectCount; i++)
	{
		if (VA > (size_t)pSects[i].VirtualAddress && VA < size_t(pSects[i].VirtualAddress + pSects[i].SizeOfRawData))
		{
			size_t addr = VA - pSects[i].VirtualAddress + pSects[i].PointerToRawData;
			return addr;
		}
	}
	printf("VA %08x doesn't belong anything\n", VA);
	return 0;
}