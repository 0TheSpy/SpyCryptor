#include "config.h"
#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <iostream>

#include "funcs.h"

using namespace std;

#ifdef CONFIG_DEBUG
TCHAR	debug_buffer[1024];
DWORD	debug_ticks;
#endif

////////////////////////
int mystrstr(wchar_t *txt1, wchar_t *txt2)
{
	wchar_t *posstr = wcsstr(txt1, txt2);
	if (posstr != NULL)
	{
		return (posstr - txt1);
	}
	else
	{
		return -1;
	}
}

// assume: supplied buff is enough to hold generated text
void StringReplace(wchar_t *buff, wchar_t *txt1, wchar_t *txt2)
{
	wchar_t *tmp;
	wchar_t *nextStr;
	int pos;

	tmp = wcsdup(buff);

	pos = mystrstr(tmp, txt1);
	if (pos != -1)
	{
		buff[0] = 0;
		wcsncpy(buff, tmp, pos);
		buff[pos] = 0;

		wcscat(buff, txt2);

		nextStr = tmp + pos + wcslen(txt1);

		while (wcslen(nextStr) != 0)
		{
			pos = mystrstr(nextStr, txt1);

			if (pos == -1)
			{
				wcscat(buff, nextStr);
				break;
			}

			wcsncat(buff, nextStr, pos);
			wcscat(buff, txt2);

			nextStr = nextStr + pos + wcslen(txt1);
		}
	}

	free(tmp);
}
////////////////////////

BOOL is_dir(PCHAR Path)
{
	DWORD dwAttr = GetFileAttributesA(Path);
	if(dwAttr != 0xffffffff && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) 
		return TRUE;
	return FALSE;
}

PCHAR *word_list = NULL;
DWORD word_list_size = 0;

BOOL file_exists(PCHAR file_path)
{
	HANDLE file = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, NULL, NULL);
	if( file!=INVALID_HANDLE_VALUE )
	{
		CloseHandle(file);
		return true;
	}
	return false;
}

string str_replace(PCHAR find_text, PCHAR replace_with, string& text)
{
	int n, len;

	len = lstrlenA(find_text);
	while( (n = text.find(find_text))!= string::npos )
	{
		text.replace(n, len, replace_with);
	}

	return text;
}


PTCHAR basename(PTCHAR Path)
{
	DWORD Len = lstrlen(Path);
	if( Path && Len )
	{
		for(int i = Len - 1; i > 0; i--)
		{
			if( Path[i]==_T('/') || Path[i]==_T('\\') )
			{
				return &Path[i + 1];
			}
		}
	}
	return NULL;
}

PCHAR dirnameA(PCHAR Path)
{
	DWORD Len = lstrlenA(Path);
	if( Path && Len )
	{
		for(int i = Len - 1; i > 0; i--)
		{
			if( Path[i]==_T('/') || Path[i]==_T('\\') )
			{
				Path[i + 1] = 0;
				return Path;
			}
		}
	}
	return NULL;
}

void mem_free(PMBUF buf)
{
	if( buf )
	{
		LocalFree(buf);
	}
}

PMBUF mem_realloc(PMBUF buf,DWORD Size)
{
	if( buf )
	{
		if( buf->free < Size )
		{
			PMBUF nbuf = (PMBUF)halloc(sizeof(MBUF) + (buf->size + Size) );
			if( nbuf )
			{
				nbuf->size = buf->size;
				nbuf->free = Size;

				mem_copy(nbuf->data,buf->data,buf->size);
			}

			mem_free(buf);
			buf = nbuf;
		}
	}else{
		buf = (PMBUF)halloc(sizeof(MBUF) + Size);
		if( buf )
		{
			buf->size = 0;
			buf->free = Size;
		}
	}

	return buf;
}

PMBUF mem_alloc(DWORD Size)
{
	return mem_realloc(NULL,Size);
}

PMBUF file_get_contentsW(PWCHAR File)
{
	PMBUF Result = NULL;

	if( File )
	{
		HANDLE hFile = CreateFileW(File,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if( hFile!=INVALID_HANDLE_VALUE )
		{
			DWORD FileSize = GetFileSize(hFile,NULL);

			Result = mem_alloc(FileSize);
			if( Result )
			{
				if( !ReadFile(hFile,Result->data,FileSize,&Result->size,NULL) || FileSize!=Result->size )
				{
					mem_free(Result);
					Result = NULL;
				}else{
					Result->free = 0;
				}
			}

			CloseHandle(hFile);
		}
	}

	return Result;
}
PMBUF file_get_contentsA(PCHAR File)
{
	PMBUF Result = NULL;

	if( File )
	{
		HANDLE hFile = CreateFileA(File,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if( hFile!=INVALID_HANDLE_VALUE )
		{
			DWORD FileSize = GetFileSize(hFile,NULL);

			Result = mem_alloc(FileSize);
			if( Result )
			{
				if( !ReadFile(hFile,Result->data,FileSize,&Result->size,NULL) || FileSize!=Result->size )
				{
					mem_free(Result);
					Result = NULL;
				}else{
					Result->free = 0;
				}
			}

			CloseHandle(hFile);
		}
	}

	return Result;
}

BOOL file_put_contentsW(PWCHAR File,PVOID buf,DWORD size)
{
	BOOL Result = FALSE;

	if( buf )
	{
		HANDLE hFile = CreateFileW(File,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
		if( hFile!=INVALID_HANDLE_VALUE )
		{
			DWORD Written;

			if( WriteFile(hFile,buf,size,&Written,NULL) && Written==size )
			{
				Result = TRUE;
			}

			CloseHandle(hFile);
		}
	}
	return Result;
}

BOOL file_put_contentsA(PCHAR File,PVOID buf,DWORD size)
{
	BOOL Result = FALSE;

	if( buf )
	{
		HANDLE hFile = CreateFileA(File,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
		if( hFile!=INVALID_HANDLE_VALUE )
		{
			DWORD Written;

			if( WriteFile(hFile,buf,size,&Written,NULL) && Written==size )
			{
				Result = TRUE;
			}

			CloseHandle(hFile);
		}
	}
	return Result;
}

PMBUF load_resource(DWORD Id)
{
	PMBUF Result = NULL;
	HRSRC res = FindResource(NULL, MAKEINTRESOURCE(Id), RT_RCDATA);
	if( res )
	{
		DWORD Size = SizeofResource(NULL, res); 
		if( Size )
		{
			Result = mem_alloc(Size);
			if( Result )
			{
				HGLOBAL lres = LoadResource(NULL,res);
				PVOID   data = LockResource(lres);

				mem_copy(Result->data, data, Size);

				Result->size = Size;
				Result->free = 0;

				UnlockResource(data);
			}
		}
	}
	return Result;
}

HMODULE file2image(PVOID data)
{
	if( data )
	{
		PIMAGE_DOS_HEADER pDos;
		PIMAGE_NT_HEADERS pNt;
		PIMAGE_SECTION_HEADER pSec;
		PBYTE Result;
		int i;

		pDos = (PIMAGE_DOS_HEADER)data;

		if( pDos && pDos->e_magic==IMAGE_DOS_SIGNATURE )
		{
			pNt = (IMAGE_NT_HEADERS*)((size_t)pDos + (size_t)((IMAGE_DOS_HEADER*)pDos)->e_lfanew);
			if( pNt->Signature==IMAGE_NT_SIGNATURE )
			{
				if( (Result = (PBYTE)VirtualAlloc(NULL,pNt->OptionalHeader.SizeOfImage,MEM_COMMIT,PAGE_EXECUTE_READWRITE)) )
				{
					memcpy(Result,pDos,pNt->OptionalHeader.SizeOfHeaders);

					pSec = IMAGE_FIRST_SECTION(pNt);
					for(i=0;i<pNt->FileHeader.NumberOfSections;i++)
					{
						memcpy((PVOID)((size_t)Result + pSec->VirtualAddress),(PVOID)((size_t)pDos + pSec->PointerToRawData),pSec->SizeOfRawData);
						pSec++;
					}

					return (HMODULE)Result;
				}
			}
		}
	}
	return NULL;
}

PVOID find_proc_by_name(PVOID ModuleBase,PCHAR proc_name)
{
	PIMAGE_DOS_HEADER pDos;
	PIMAGE_NT_HEADERS pNt;
	PIMAGE_EXPORT_DIRECTORY pExport;
	PDWORD	AddressOfNames;
	PDWORD	AddressOfFunctions;
	PWORD	AddressOfNameOrdinals;
	PCHAR	CurrentName;
	DWORD_PTR	Address;
	DWORD	ExportSize;
	WORD	Index,Ordinal;
	WORD	MinOrdinal,MaxOrdinal;
	int i;

	Address = NULL;

	if( ModuleBase && ((PIMAGE_DOS_HEADER)ModuleBase)->e_magic==IMAGE_DOS_SIGNATURE )
	{
		pNt = (PIMAGE_NT_HEADERS)((PBYTE)ModuleBase + ((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew);
		if( pNt->Signature==IMAGE_NT_SIGNATURE )
		{
			pExport = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)ModuleBase + ((PIMAGE_NT_HEADERS)pNt)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
			ExportSize = pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

			if( pExport!=(PIMAGE_EXPORT_DIRECTORY)ModuleBase ) 
			{
				AddressOfNames = (PDWORD)((PBYTE)ModuleBase + pExport->AddressOfNames);
				AddressOfFunctions = (PDWORD)((PBYTE)ModuleBase + pExport->AddressOfFunctions);
				AddressOfNameOrdinals = (PWORD)((PBYTE)ModuleBase + pExport->AddressOfNameOrdinals);

				MinOrdinal = 0xFFFF;
				for(i=0;i<pExport->NumberOfNames;i++)
				{
					if( AddressOfNameOrdinals[i]<MinOrdinal)
					{
						MinOrdinal = AddressOfNameOrdinals[i];
					}
				}

				MaxOrdinal = MinOrdinal + pExport->NumberOfNames;

				for(i=0;i<pExport->NumberOfFunctions;i++)
				{
					if( i>=MinOrdinal && i<MaxOrdinal )
					{	
						Index = AddressOfNameOrdinals[i];
					}else{
						Index = i;
					}

					//ignore spaces
					if( !AddressOfFunctions[ Index ] )
						continue;

					if( i<pExport->NumberOfNames )
					{					
						CurrentName = (PCHAR)ModuleBase + AddressOfNames[i];

						if( !lstrcmpA(CurrentName,proc_name) )
						{
							//is forwarding ?
							Address = (AddressOfFunctions[ Index ] + (DWORD_PTR)ModuleBase);
							break;
						}
					}else{
						break;
					}
				}

				if( Address )
				{
					//check forward
					if( Address > (DWORD_PTR)pExport && Address < (DWORD_PTR)pExport + ExportSize )
					{
						CHAR DllName[MAX_PATH];

						for(i = 0; i < MAX_PATH - 1 && *((PCHAR)Address + i)!='.'; i++)
						{
							DllName[i] = *((PCHAR)Address + i);
						}

						*(PDWORD)&DllName[i] = 'lld.';
						DllName[i + 4] = 0;

						ModuleBase = GetModuleHandleA(DllName);

						return find_proc_by_name(ModuleBase,proc_name);
					}
				}
			}
		}
	}


	return (PVOID)Address;
}
