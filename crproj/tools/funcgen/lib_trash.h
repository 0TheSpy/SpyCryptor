#ifndef __LIB_TRASH__
#define __LIB_TRASH__

typedef struct _TRASH_IMPORT_DLL_REFERENCE
{
	PAPI_TEMPLATE	templates;
	DWORD			count;
}TRASH_IMPORT_DLL_REFERENCE,*PTRASH_IMPORT_DLL_REFERENCE;

class mImportTrashGroup : public mImportLibraryGroup
{
	PAPI_TEMPLATE	templates;
	DWORD			templates_count;
public:
	mImportTrashGroup(mImport *import, PAPI_TEMPLATE templates, DWORD templates_count);

	bool get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash);
};

class mImportTrashLibrary : public mImportLibrary
{
public:
	mImportTrashLibrary(mImport* import, PAPI_TEMPLATE templates, DWORD templates_count);
};

#endif