#ifndef __FUNCS__
#define __FUNCS__

#define err_critical(txt) _tprintf(_T("Critical Error: %s, call to support!\r\n"),txt);

#define halloc(size) HeapAlloc(GetProcessHeap(),NULL,size)
#define hfree(ptr) HeapFree(GetProcessHeap(),NULL,ptr)

#ifdef CONFIG_DEBUG
extern TCHAR debug_buffer[1024];
extern DWORD debug_ticks;

#define dbg(...) wsprintf(debug_buffer,##__VA_ARGS__); OutputDebugString(debug_buffer);
#define dbg_tset() debug_ticks = GetTickCount()
#define dbg_tget() ((GetTickCount() - debug_ticks)/1000)

#else

#define dbg(...)
	
#endif

#define mem_zero(dest,size) __stosb((PBYTE)dest,0,size)
#define mem_copy(dest,source,size) __movsb((PBYTE)dest,(PBYTE)source,size)
#define mem_set(dest,bt,size) __stosb((PBYTE)dest,bt,size)

#ifndef ALIGN_UP
#define ALIGN_DOWN(x, align) (x & ~(align-1))
#define ALIGN_UP(x, align) ((x & (align-1)) ? ALIGN_DOWN(x,align)+align : x)
#endif

#ifndef InitializeListHead
//
//  VOID
//  InitializeListHead(
//      PLIST_ENTRY ListHead
//      );
//

#define InitializeListHead(ListHead) (\
	(ListHead)->Flink = (ListHead)->Blink = (ListHead))

//
//  BOOLEAN
//  IsListEmpty(
//      PLIST_ENTRY ListHead
//      );
//

#define IsListEmpty(ListHead) \
	((ListHead)->Flink == (ListHead))

//
//  PLIST_ENTRY
//  RemoveHeadList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveHeadList(ListHead) \
	(ListHead)->Flink;\
{RemoveEntryList((ListHead)->Flink)}

//
//  PLIST_ENTRY
//  RemoveTailList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveTailList(ListHead) \
	(ListHead)->Blink;\
{RemoveEntryList((ListHead)->Blink)}

//
//  VOID
//  RemoveEntryList(
//      PLIST_ENTRY Entry
//      );
//

#define RemoveEntryList(Entry) {\
	PLIST_ENTRY _EX_Blink;\
	PLIST_ENTRY _EX_Flink;\
	_EX_Flink = (Entry)->Flink;\
	_EX_Blink = (Entry)->Blink;\
	_EX_Blink->Flink = _EX_Flink;\
	_EX_Flink->Blink = _EX_Blink;\
}

//
//  VOID
//  InsertTailList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertTailList(ListHead,Entry) {\
	PLIST_ENTRY _EX_Blink;\
	PLIST_ENTRY _EX_ListHead;\
	_EX_ListHead = (ListHead);\
	_EX_Blink = _EX_ListHead->Blink;\
	(Entry)->Flink = _EX_ListHead;\
	(Entry)->Blink = _EX_Blink;\
	_EX_Blink->Flink = (Entry);\
	_EX_ListHead->Blink = (Entry);\
}

//
//  VOID
//  InsertHeadList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertHeadList(ListHead,Entry) {\
	PLIST_ENTRY _EX_Flink;\
	PLIST_ENTRY _EX_ListHead;\
	_EX_ListHead = (ListHead);\
	_EX_Flink = _EX_ListHead->Flink;\
	(Entry)->Flink = _EX_Flink;\
	(Entry)->Blink = _EX_ListHead;\
	_EX_Flink->Blink = (Entry);\
	_EX_ListHead->Flink = (Entry);\
}

#endif //InitializeListHead

typedef struct _MBUF
{
	DWORD	size;
	DWORD	free;
	BYTE	data[0];
}MBUF,*PMBUF;

PTCHAR basename(PTCHAR Path);
PCHAR dirnameA(PCHAR Path);
BOOL is_dir(PCHAR Path);
BOOL file_exists(PCHAR path);
void mem_free(PMBUF buf);
PMBUF mem_realloc(PMBUF buf,DWORD Size);
PMBUF mem_alloc(DWORD Size);
PMBUF file_get_contentsA(PCHAR File);
PMBUF file_get_contentsW(PWCHAR File);
BOOL file_put_contentsW(PWCHAR File,PVOID buf,DWORD size);
BOOL file_put_contentsA(PCHAR File,PVOID buf,DWORD size);
#ifdef UNICODE
#define file_put_contents file_put_contentsW
#define file_get_contents file_get_contentsW
#else
#define file_put_contents file_put_contentsA
#define file_get_contents file_get_contentsA
#endif
PMBUF load_resource(DWORD Id);
HMODULE file2image(PVOID data);
PVOID find_proc_by_name(PVOID ModuleBase,PCHAR proc_name);

PCHAR* get_wordlist(PDWORD size);

#include <iostream>

void StringReplace(wchar_t *buff, wchar_t *txt1, wchar_t *txt2);

std::string str_replace(PCHAR find_text, PCHAR replace_with, std::string& text);

#endif