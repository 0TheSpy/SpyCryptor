#include "config.h"

#include <windows.h>
#include <tchar.h>
#include <intrin.h>

#include "globals.h"
#include "lib_trash.h"

mImportTrashGroup::mImportTrashGroup(mImport *import, PAPI_TEMPLATE templates, DWORD templates_count) : mImportLibraryGroup(import, templates_count)
{
	this->templates = templates;
	this->templates_count = templates_count;
}

bool mImportTrashGroup::get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash)
{
	DWORD pos = random.get_less(0, templates_count);

	procs->parse(code, &templates[pos], NULL, is_trash);

	return true;
}

mImportTrashLibrary::mImportTrashLibrary(mImport* import, PAPI_TEMPLATE templates, DWORD templates_count) : mImportLibrary(import)
{
	groups_count = 1;
	groups[0] = new mImportTrashGroup(import, templates, templates_count);

	update_api_count();
}


