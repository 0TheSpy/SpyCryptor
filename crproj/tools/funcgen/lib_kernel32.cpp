#include "config.h"

#include <windows.h>
#include <tchar.h>
#include <intrin.h>

#include "globals.h"
#include "lib_kernel32.h"

void files_lock_constr(mImport *apis, mCode &str, API_TEMPLATE *tmpl, MVAR_INFO* arg, bool is_trash)
{
	MIMPORT_KERNEL32_GROUP_FILES* info = (MIMPORT_KERNEL32_GROUP_FILES*)arg->userdata;

	if( info->is_locked ) // locked
	{
		info->is_locked = false;

		PMVAR_INFO var = apis->get_or_add_var(MVT_BOOL);

		if( info->is_lock_ex )
		{
			//bugfix (0xC0000005): we must set reverved!=0, because hFile in code will be 0xffffffff, but LockFileEx check for console handle (hfile & 0x10000003)==3, and then take data vrom (hfile + 8), but it will be < 0x10000
			str << var->name << " = UnlockFileEx(" << arg->name << ", " << random.get_equal(1,0xFF) << ", " << info->lock_vars[0]<< ", " << info->lock_vars[1]<< ", NULL);";
		}else{
			str << var->name << " = UnlockFile(" << arg->name << ", " << info->lock_vars[0]<< ", " << info->lock_vars[1]<< ", " << info->lock_vars[2]<< ", " << info->lock_vars[3] << ");";
		}
	}else{
		info->is_locked = true;
		info->is_lock_ex = random.get_equal(0,1);

		PMVAR_INFO var = apis->get_or_add_var(MVT_BOOL);

		if( info->is_lock_ex )
		{
			for(int i = 0; i < 2; i++)
			{
				info->lock_vars[i] = random.get_less(0,0x4000);
			}

			//bugfix (0xC0000005): we must set reverved!=0, because hFile in code will be 0xffffffff, but LockFileEx check for console handle (hfile & 0x10000003)==3, and then take data vrom (hfile + 8), but it will be < 0x10000
			str << var->name << " = LockFileEx(" << arg->name << ", " << random.get_equal(1,2) << ", " << random.get_equal(1,0xFFFFFFFF) << ", " << info->lock_vars[0]<< ", " << info->lock_vars[1]<< ", NULL);";
		}else{
			for(int i = 0; i < 4; i++)
			{
				info->lock_vars[i] = random.get_less(0,0x4000);
			}

			str << var->name << " = LockFile(" << arg->name << ", " << info->lock_vars[0]<< ", " << info->lock_vars[1]<< ", " << info->lock_vars[2]<< ", " << info->lock_vars[3] << ");";
		}
	}
}


API_TEMPLATE files_read_write[] = {
	{NULL, NULL, {NULL, PT_PTR_ADDVAR, MVT_BOOL}, "WriteFile",{{NULL,PT_ARG},{NULL,PT_PTR_ANY},{NULL, PT_RAND,0,512},{NULL,PT_PTR_ADDVAR,MVT_UINT32},{NULL,PT_NULL,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_PTR_ADDVAR, MVT_BOOL}, "WriteFileEx",{{NULL,PT_ARG},{NULL,PT_PTR_ANY},{NULL,PT_RAND,0,512},{NULL,PT_NULL},{NULL,PT_NULL,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_PTR_ADDVAR, MVT_BOOL}, "WriteFileGather",{{NULL,PT_ARG},{NULL,PT_PTR_ADDVAR,MVT_FILE_SEGMENT_ELEMENT},{NULL,PT_RAND,0,512},{NULL,PT_NULL},{NULL,PT_NULL,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},

	{NULL, NULL, {NULL, PT_PTR_ADDVAR, MVT_BOOL}, "ReadFile",{{NULL,PT_ARG},{NULL,PT_PTR_ANY,0},{NULL,PT_RAND,0,512},{NULL,PT_PTR_ADDVAR,MVT_UINT32},{NULL,PT_NULL,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_PTR_ADDVAR, MVT_BOOL}, "ReadFileEx",{{NULL,PT_ARG},{NULL,PT_PTR_ANY,0},{NULL,PT_RAND,0,512},{NULL,PT_NULL},{NULL,PT_NULL,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_PTR_ADDVAR, MVT_BOOL}, "ReadFileScatter",{{NULL,PT_ARG},{NULL,PT_PTR_ADDVAR,MVT_FILE_SEGMENT_ELEMENT},{NULL,PT_RAND,0,512},{NULL,PT_NULL},{NULL,PT_NULL,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
};

void files_read_write_constr(mImport *apis, mCode &str, API_TEMPLATE *tmpl, MVAR_INFO* arg, bool is_trash)
{
	MIMPORT_KERNEL32_GROUP_FILES *info = (MIMPORT_KERNEL32_GROUP_FILES*)arg->userdata;

	DWORD from, to;

	switch( info->perm )
	{
	case 3: // read & write
		from = 0;
		to = 5;
		break;
	case 2: // write
		from = 0;
		to = 2;
		break;
	case 1:
		from = 3;
		to = 5;
		break;
	}

	apis->parse(str, &files_read_write[ random.get_equal(from,to) ], arg, is_trash);
}

// {"",PT_ARG} - "" is string before param (example: (HANDLE)), PT_ARG argument will taking
// {NULL, PT_NONE} - no parameter

API_TEMPLATE files_templates[] = {
	// {"Name_of_proc",FALSE,{{NULL,PT_ARG},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},

	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetFileSize",{{NULL,PT_ARG},{"(LPDWORD)",PT_PTR_ADDVAR,MVT_UINT32},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "GetFileSizeEx",{{NULL,PT_ARG},{NULL,PT_PTR_ADDVAR,MVT_LARGE_INTEGER},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetFileType",{{NULL,PT_ARG},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "GetFileInformationByHandle",{{NULL,PT_ARG},{NULL,PT_PTR_ADDVAR,MVT_BY_HANDLE_FILE_INFORMATION},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "SetEndOfFile",{{NULL,PT_ARG},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, files_lock_constr, {NULL, PT_ADDVAR, MVT_BOOL}, NULL, {{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "SetFilePointer",{{NULL,PT_ARG},{NULL,PT_RAND,0,0xFFFF},{NULL,PT_PTR_GETVAR_OR_NULL,MVT_INT32},{NULL,PT_RAND,0,2},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "SetFilePointerEx",{{NULL,PT_ARG},{NULL,PT_ADDVAR,MVT_LARGE_INTEGER},{NULL,PT_PTR_GETVAR_OR_NULL,MVT_LARGE_INTEGER},{NULL,PT_RAND,0,2},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "SetFileValidData",{{NULL,PT_ARG},{NULL,PT_RAND,0,0x7FFFFF},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
};

API_TEMPLATE sysinfo_templates[] = {
	{NULL, NULL, {NULL, PT_NONE}, "GetNativeSystemInfo",{{NULL,PT_PTR_ADDVAR,MVT_SYSTEMINFO},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_NONE}, "GetSystemInfo",{{NULL,PT_PTR_ADDVAR,MVT_SYSTEMINFO},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetVersion",{{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "IsProcessorFeaturePresent",{{NULL,PT_RAND,0,15},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE},{NULL,PT_NONE,0},{NULL,PT_NONE,0},{NULL,PT_NONE,0}}},
	{ATF_A_OR_W, NULL, {NULL, PT_ADDVAR, MVT_PTCHAR}, "GetCommandLine",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_PROCESS}, "GetCurrentProcess",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetCurrentProcessId",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_THREAD}, "GetCurrentThread",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetCurrentThreadId",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetLogicalDrives",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetLastError",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_HEAP}, "GetProcessHeap",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetVersion",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "AreFileApisANSI",{{NULL,PT_NONE}}}, 
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_TIMER}, "CreateTimerQueue",{{NULL,PT_NONE}}}, 
};

mImportKernel32Sysinfo::mImportKernel32Sysinfo(mImport* import) : mImportLibraryGroup(import,2 + sizeof(sysinfo_templates)/sizeof(API_TEMPLATE))
{
}

bool mImportKernel32Sysinfo::get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash)
{
	PMVAR_INFO var1, var2;

	switch( api_id )
	{
		case 0:
			if( procs->get_procs_type()==IPT_A )
			{
				var1 = procs->get_or_add_var(MVT_OSVERSIONINFOA);
			}else{
				var1 = procs->get_or_add_var(MVT_OSVERSIONINFOW);
			}

			switch( var1->userdata )
			{
				case 0: code << var1->name << ".dwOSVersionInfoSize = sizeof(OSVERSIONINFO" << procs->get_procs_string() << ");"; break;
				default:
					var2 = procs->get_or_add_var(MVT_BOOL);
					code << var2->name << " = GetVersionEx" << procs->get_procs_string() << "(&" << var1->name << ");";
					break;
			}
			var1->userdata++;
			break;
		case 1:
			if( procs->get_procs_type()==IPT_A )
			{
				var1 = procs->get_or_add_var(MVT_OSVERSIONINFOEXA);
			}else{
				var1 = procs->get_or_add_var(MVT_OSVERSIONINFOEXW);
			}
			switch( var1->userdata )
			{
				case 0: code << var1->name << ".dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX" << procs->get_procs_string() << ");"; break;
				case 1: 
					var2 = procs->get_or_add_var(MVT_BOOL);
					code << var2->name << " = GetVersionEx" << procs->get_procs_string() << "((OSVERSIONINFO" << procs->get_procs_string() << "*)&" << var1->name << ");"; break;
				default:
					var2 = procs->get_or_add_var(MVT_BOOL);
					code << var2->name << " = VerifyVersionInfo" << procs->get_procs_string() << "(&" << var1->name << ", " << (1 << random.get_less(0,8)) << ", NULL);";
					break;
			}
			var1->userdata++;
			break;
		case 2:
			var1 = procs->get_or_add_var(MVT_LARGE_INTEGER);
			if( var1->userdata==0 )
			{
				var1->userdata++;

				code <<"QueryPerformanceFrequency(&" << var1->name << ");";
			}else{
				code <<"QueryPerformanceCounter(&" << var1->name << ");";
			}

			break;
		default:
			api_id -= 3;
			PAPI_TEMPLATE temp = &sysinfo_templates[ api_id ];
			procs->parse(code, temp, NULL, is_trash);
			break;
	}

	return true;
}

mImportKernel32Files::mImportKernel32Files(mImport *import) : mImportLibraryGroup(import, sizeof(files_templates)/sizeof(API_TEMPLATE)), units(NULL)
{
}

bool mImportKernel32Files::get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash)
{
	PMVAR_INFO var;

	if( !is_loop )
	{
		var = procs->get_or_add_var(MVT_FILE);
	}else{
		var = procs->get_var(MVT_FILE);
		if( !var )
			return false;
	}

	if( !var->userdata )
	{
		MIMPORT_KERNEL32_GROUP_FILES* info = units.get_new_chunk();

#ifdef _WIN64
		var->userdata = (DWORD64)info;
#else
		var->userdata = (DWORD)info;
#endif

		info->perm = random.get_less(1,4); // 1 - read , 2 - write

		if( procs->get_procs_type()==IPT_A )
		{
			code << var->name << " = CreateFileA(";
		}else{
			code << var->name << " = CreateFileW(";
		}

		add_path(code, 0);

		PCHAR Permissions[] = {
			"GENERIC_READ",
			"GENERIC_WRITE"
		};

		add_bits(code, TRUE, info->perm, Permissions);
		add_shift(code, TRUE, 0, 3); // file share
		add_null(code, TRUE);  // sec attr
		add_rand(code, TRUE, 0, 5); // create attr
		add_shift(code, TRUE, 1, 10); // file attr
		add_null(code, TRUE);  // template

		code << ");";

		return true;
	}
	
	PAPI_TEMPLATE temp = &files_templates[ api_id ];

	procs->parse(code, temp, var, is_trash);

	return true;
}

API_TEMPLATE critical_section_templates[] = {
	{NULL, NULL, {NULL, PT_NONE}, "EnterCriticalSection",{{NULL,PT_PTR_ARG},{NULL,PT_NONE}}},
	{NULL, NULL, {NULL, PT_NONE}, "LeaveCriticalSection",{{NULL,PT_PTR_ARG},{NULL,PT_NONE}}},
};

API_TEMPLATE event_templates[] = {
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_BOOL}, "SetEvent",{{NULL,PT_ARG},{NULL,PT_NONE}}},
};

API_TEMPLATE wait_templates[] = {
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "WaitForSingleObject",{{NULL,PT_ARG},{NULL,PT_RAND,0,10000},{NULL,PT_NONE}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "WaitForMultipleObjects",{{NULL,PT_RAND,1},{NULL,PT_PTR_ARG},{NULL,PT_RAND,0},{NULL,PT_RAND,0,10000},{NULL,PT_NONE}}},
};

mImportKernel32Synchronization::mImportKernel32Synchronization(mImport *import) : mImportLibraryGroup(import, 5), units(NULL)
{

}

bool mImportKernel32Synchronization::get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash)
{
	PMVAR_INFO	left_var;
	MVAR_TYPE	left_type;

	switch( api_id )
	{
		case 0: 
			left_type = MVT_CRITICAL_SECTION; 
			break;
		case 1:
		case 2:
			if( is_loop )
				return false;

			left_type = MVT_MUTEX;
			break;
		case 3:
		case 4:
			left_type = MVT_EVENT;
			break;
		default:
			__debugbreak();
			break;
	}

	if( !is_loop )
	{
		left_var = procs->get_or_add_var(left_type);
	}else{
		left_var = procs->get_var(left_type);
		if( !left_var )
			return false;
	}

	if( !left_var->userdata )
	{

		PMIMPORT_KERNEL32_GROUP_SYNCHRONIZATION sync = units.get_new_chunk();

		sync->is_in = false;

		switch( left_type )
		{
			case MVT_CRITICAL_SECTION:
				code << "InitializeCriticalSection(&" << left_var->name << ");";
				break;
			case MVT_MUTEX:
				code << left_var->name << " = CreateMutex" << procs->get_procs_string() << "(NULL, FALSE, NULL);";
				break;
			case MVT_EVENT:
				code << left_var->name << " = CreateEvent" << procs->get_procs_string() << "(NULL, FALSE, FALSE, NULL);";
				break;
		}

#ifdef _WIN64
		left_var->userdata = (DWORD64)sync;
#else
		left_var->userdata = (DWORD)sync;
#endif
		return true;
	}

	PMIMPORT_KERNEL32_GROUP_SYNCHRONIZATION sync = (PMIMPORT_KERNEL32_GROUP_SYNCHRONIZATION)left_var->userdata;

	switch( left_type )
	{
		case MVT_CRITICAL_SECTION:
			if( !sync->is_in )
			{
				procs->parse(code, &critical_section_templates[0], left_var, is_trash);
			}else{
				procs->parse(code, &critical_section_templates[1], left_var, is_trash);
			}
			break;
		case MVT_MUTEX:
			if( !sync->is_in )
			{
				DWORD pos = api_id==1 ? 0 : 1;
				procs->parse(code, &wait_templates[pos], left_var, is_trash);
			}else{
				code << "ReleaseMutex(" << left_var->name << ");";
			}
			break;
		case MVT_EVENT:
			if( !sync->is_in )
			{
				procs->parse(code, &event_templates[0], left_var, is_trash);
			}else{
				code << "ResetEvent(" << left_var->name << ");";
			}
			break;
	}

	sync->is_in = ~sync->is_in;

	return true;
}

API_TEMPLATE atom_templates[] = {
	{ATF_A_OR_W, NULL, {NULL, PT_ARG}, "AddAtom",{{NULL,PT_STRING},{NULL,PT_NONE}}},
	{NULL, NULL, {NULL, PT_ADDVAR, MVT_ATOM}, "DeleteAtom",{{NULL,PT_ARG},{NULL,PT_NONE}}},
	//{NULL, NULL, {NULL, PT_ADDVAR, MVT_ATOM}, "FindAtom",{{NULL,PT_STRING},{NULL,PT_NONE}}},
	//{ATF_A_OR_W, NULL, {NULL, PT_ADDVAR, MVT_UINT32}, "GetAtomName",{{NULL,PT_GETVAR_OR_NULL, MVT_ATOM},{NULL,PT_ADDVAR, MVT_TCHAR_ARRAY},{NULL,PT_VAR_SIZE, 1}}},
};

mImportKernel32Atom::mImportKernel32Atom(mImport *import) : mImportLibraryGroup(import, 3)
{

}

bool mImportKernel32Atom::get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash)
{
	PMVAR_INFO left_var;

	if( !is_loop )
	{
		left_var = procs->get_or_add_var(MVT_ATOM);
	}else{
		left_var = procs->get_var(MVT_ATOM);
		if( !left_var )
			return false;
	}

	if( !left_var->userdata )
	{
		left_var->userdata++;

		procs->parse(code, &atom_templates[0], left_var, is_trash);
	}else{
		DWORD pos = random.get_less(1, sizeof(atom_templates)/sizeof(API_TEMPLATE));

		procs->parse(code, &atom_templates[pos], left_var, is_trash);
	}

	return true;
}

API_TEMPLATE crypt_kernel32_templates[] = {
	{ NULL, NULL, { NULL, PT_NONE }, "k_AssignProcessToJobObject", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CancelWaitableTimer", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ClearCommBreak", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ClearCommError", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ConvertFiberToThread", { { NULL, PT_NONE } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ConvertThreadToFiber", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CreateFiber", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CreateFiberEx", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CreateFileMappingW", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CreateIoCompletionPort", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CreateMemoryResourceNotification", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_CreateTapePartition", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_DecodePointer", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_DecodeSystemPointer", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_DisableThreadLibraryCalls", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_DisconnectNamedPipe", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_EncodePointer", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_EncodeSystemPointer", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_EraseTape", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_EscapeCommFunction", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_FindFirstFileExW", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_FindNextChangeNotification", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_FlushFileBuffers", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_FlushInstructionCache", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_FlushViewOfFile", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_FreeResource", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetCommMask", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetCommModemStatus", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetCommTimeouts", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetFileAttributesExW", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetFileInformationByHandle", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetFileTime", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetFileType", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetMailslotInfo", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetModuleFileNameA", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetModuleFileNameW", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetNamedPipeHandleStateA", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetNamedPipeHandleStateW", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetNamedPipeInfo", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetPriorityClass", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessAffinityMask", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessHandleCount", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessId", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessIoCounters", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessPriorityBoost", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessTimes", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetProcessVersion", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetStdHandle", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetTapeStatus", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetThreadContext", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetThreadIOPendingFlag", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetThreadPriority", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetThreadPriorityBoost", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetThreadTimes", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GetWriteWatch", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GlobalCompact", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GlobalDeleteAtom", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GlobalUnWire", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GlobalUnfix", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_GlobalUnlock", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_InitAtomTable", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_IsProcessInJob", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_IsWow64Process", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_LocalCompact", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_LocalShrink", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_LocalUnlock", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_LockResource", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_MapUserPhysicalPagesScatter", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_MulDiv", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_OpenProcess", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_PeekNamedPipe", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_PostQueuedCompletionStatus", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_PrepareTape", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_PulseEvent", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ReleaseMutex", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ReleaseSemaphore", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ResetEvent", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_ResetWriteWatch", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetHandleCount", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetHandleInformation", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetInformationJobObject", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetLastError", { { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetMailslotInfo", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetMessageWaitingIndicator", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetPriorityClass", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetProcessPriorityBoost", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetProcessWorkingSetSize", { { NULL, PT_ZERO }, { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetStdHandle", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_SetSystemTimeAdjustment", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
	{ NULL, NULL, { NULL, PT_NONE }, "k_TransmitCommChar", { { NULL, PT_ZERO }, { NULL, PT_ZERO } } },
};

mImportKernel32_crypt::mImportKernel32_crypt(mImport *import) : mImportLibraryGroup(import, sizeof(crypt_kernel32_templates) / sizeof(API_TEMPLATE))
{
}

bool mImportKernel32_crypt::get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash)
{
	PMVAR_INFO left_var;

	if (!is_loop)
	{
		left_var = procs->get_or_add_var(MVT_INT8);
	}
	else{
		left_var = procs->get_var(MVT_INT8);
		if (!left_var)
			return false;
	}

	if (!left_var->userdata)
	{
		left_var->userdata++;

		procs->parse(code, &crypt_kernel32_templates[0], left_var, is_trash);
	}
	else{
		DWORD pos = random.get_less(1, sizeof(crypt_kernel32_templates) / sizeof(API_TEMPLATE));

		procs->parse(code, &crypt_kernel32_templates[pos], left_var, is_trash);
	}

	return true;
}

mImportKernel32::mImportKernel32(mImport* import) : mImportLibrary(import)
{
	groups_count = 0;

	groups[groups_count++] = new mImportKernel32_crypt(import);
	groups[groups_count++] = new mImportKernel32Sysinfo(import);
	groups[groups_count++] = new mImportKernel32Files(import);
	groups[groups_count++] = new mImportKernel32Synchronization(import);
	groups[groups_count++] = new mImportKernel32Atom(import);

	update_api_count();
}
