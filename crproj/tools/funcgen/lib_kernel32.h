#ifndef __LIB_KERNEL32__
#define __LIB_KERNEL32__

#include "mImport.h"

typedef struct _MIMPORT_KERNEL32_GROUP_FILES
{
	BYTE perm:2;
	BYTE is_locked:5;
	BYTE is_lock_ex:1;
	DWORD lock_vars[4];
}MIMPORT_KERNEL32_GROUP_FILES,*PMIMPORT_KERNEL32_GROUP_FILES;

class mImportKernel32Sysinfo : public mImportLibraryGroup
{
public:
	mImportKernel32Sysinfo(mImport* import);

	bool get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash);
};

class mImportKernel32Files : public mImportLibraryGroup
{
	mChunkUnitAllocator<MIMPORT_KERNEL32_GROUP_FILES> units;
public:
	mImportKernel32Files(mImport *import);

	bool get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash);
};

typedef struct _MIMPORT_KERNEL32_GROUP_SYNCHRONIZATION
{
	DWORD is_in;
}MIMPORT_KERNEL32_GROUP_SYNCHRONIZATION,*PMIMPORT_KERNEL32_GROUP_SYNCHRONIZATION;

class mImportKernel32Synchronization : public mImportLibraryGroup
{
	mChunkUnitAllocator<MIMPORT_KERNEL32_GROUP_SYNCHRONIZATION> units;
public:
	mImportKernel32Synchronization(mImport *import);

	bool get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash);
};

class mImportKernel32Atom : public mImportLibraryGroup
{
public:
	mImportKernel32Atom(mImport *import);

	bool get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash);
};

class mImportKernel32_crypt : public mImportLibraryGroup
{
public:
	mImportKernel32_crypt(mImport *import);

	bool get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash);
};

class mImportKernel32 : public mImportLibrary
{
public:
	mImportKernel32(mImport* import);
};

#endif