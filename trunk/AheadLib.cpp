


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "StdAfx.h"
#include "Main.h"
#include "AheadLib.h"
#include <string>
 
#include <TIME.H>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 常量
const CHAR STR_BlockHeader[] = "\r\n\r\n\r\n////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\r\n";
const CHAR STR_BlockTail[] = "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\r\n";

const CHAR STR_Preprocessor[] = "// 头文件\r\n#include <Windows.h>\r\n#include \"StdAfx.h\"\r\n";

const CHAR STR_ExportComment[] = "// 导出函数\r\n";
const CHAR STR_ExportRedirect[] = "#pragma comment(linker, \"/EXPORT:%s=%s.%s,@%d\")\r\n";
const CHAR STR_ExportNormal[] = "#pragma comment(linker, \"/EXPORT:%s%s=_AheadLib_%s,@%d\")\r\n";
const CHAR STR_ExportNoname[] = "#pragma comment(linker, \"/EXPORT:Noname%d=_AheadLib_Noname%d,@%d,NONAME\")\r\n";
const CHAR STR_VC6DSW[]=
"Microsoft Developer Studio Workspace File, Format Version 6.00\r\n"
"# 警告: 不能编辑或删除该工作区文件！\r\n"
"\r\n"
"###############################################################################\r\n"
"\r\n"
"Project: \"%s\"=.\\%s.dsp - Package Owner=<4>\r\n"
"\r\n"		 
"		 Package=<5>\r\n"	
"{{{\r\n"	
"}}}\r\n"	
"\r\n"	
"Package=<4>\r\n"	
"{{{\r\n"	
"}}}\r\n"	
"\r\n"
"###############################################################################\r\n"
"\r\n"
"Global:\r\n"
"\r\n"
"Package=<5>\r\n"
"{{{\r\n"
"}}}\r\n"
"\r\n"
"Package=<3>\r\n"
"{{{\r\n"
"}}}\r\n"
"\r\n"
"###############################################################################\r\n"
"\r\n";
//_________________________________
//buffer 最大1024
const CHAR STR_VC6DSP1[] =
"# Microsoft Developer Studio Project File - Name=\"%s\" - Package Owner=<4>\r\n"
"# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n"
"# ** DO NOT EDIT **\r\n"
"\r\n"
"# TARGTYPE \"Win32 (x86) Dynamic-Link Library\" 0x0102\r\n"
"\r\n"
"CFG=%s - Win32 Debug\r\n"
"!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n"
"!MESSAGE use the Export Makefile command and run\r\n"
"!MESSAGE \r\n"
"!MESSAGE NMAKE /f \"%s.mak\".\r\n"
"!MESSAGE \r\n"
"!MESSAGE You can specify a configuration when running NMAKE\r\n"
"!MESSAGE by defining the macro CFG on the command line. For example:\r\n"
"!MESSAGE \r\n"
"!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\r\n"
"!MESSAGE \r\n"
"!MESSAGE Possible choices for configuration are:\r\n"
"!MESSAGE \r\n"
"!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) Dynamic-Link Library\")\r\n";

const CHAR STR_VC6DSP2[] =
"!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Dynamic-Link Library\")\r\n"
"!MESSAGE \r\n"
"\r\n"
"# Begin Project\r\n"
"# PROP AllowPerConfigDependencies 0\r\n"
"# PROP Scc_ProjName \"\"\r\n"
"# PROP Scc_LocalPath \"\"\r\n"
"CPP=cl.exe\r\n"
"MTL=midl.exe\r\n"
"RSC=rc.exe\r\n"
"\r\n"
"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\r\n"
"\r\n"
"# PROP BASE Use_MFC 0\r\n"
"# PROP BASE Use_Debug_Libraries 0\r\n"
"# PROP BASE Output_Dir \"Release\"\r\n"
"# PROP BASE Intermediate_Dir \"Release\"\r\n"
"# PROP BASE Target_Dir \"\"\r\n"
"# PROP Use_MFC 0\r\n"
"# PROP Use_Debug_Libraries 0\r\n"
"# PROP Output_Dir \"Release\"\r\n"
"# PROP Intermediate_Dir \"Release\"\r\n"
"# PROP Target_Dir \"\"\r\n"
"# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /D \"_USRDLL\" /D \"%s_EXPORTS\" /Yu\"stdafx.h\" /FD /c\r\n";

const CHAR STR_VC6DSP3[] =
"# ADD CPP /nologo /MT /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /D \"_USRDLL\" /D \"%s_EXPORTS\" /FR /Yu\"stdafx.h\" /FD /c\r\n"
"# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
"# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
"# ADD BASE RSC /l 0x804 /d \"NDEBUG\"\r\n"
"# ADD RSC /l 0x804 /d \"NDEBUG\"\r\n"
"BSC32=bscmake.exe\r\n"
"# ADD BASE BSC32 /nologo\r\n"
"# ADD BSC32 /nologo\r\n"
"LINK32=link.exe\r\n"
"# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386\r\n"
"# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386\r\n"
"\r\n"
"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n";

const CHAR STR_VC6DSP4[] =
"\r\n"
"# PROP BASE Use_MFC 0\r\n"
"# PROP BASE Use_Debug_Libraries 1\r\n"
"# PROP BASE Output_Dir \"Debug\"\r\n"
"# PROP BASE Intermediate_Dir \"Debug\"\r\n"
"# PROP BASE Target_Dir \"\"\r\n"
"# PROP Use_MFC 0\r\n"
"# PROP Use_Debug_Libraries 1\r\n"
"# PROP Output_Dir \"Debug\"\r\n"
"# PROP Intermediate_Dir \"Debug\"\r\n"
"# PROP Target_Dir \"\"\r\n"
"# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /D \"_USRDLL\" /D \"%s_EXPORTS\" /Yu\"stdafx.h\" /FD /GZ /c\r\n"
"# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /D \"_USRDLL\" /D \"%s_EXPORTS\" /FR /Yu\"stdafx.h\" /FD /GZ /c\r\n"
"# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
"# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
"# ADD BASE RSC /l 0x804 /d \"_DEBUG\"\r\n"
"# ADD RSC /l 0x804 /d \"_DEBUG\"\r\n"
"BSC32=bscmake.exe\r\n";

const CHAR STR_VC6DSP5[] =
"\r\n"
"# PROP BASE Use_MFC 0\r\n"
"# PROP BASE Use_Debug_Libraries 1\r\n"
"# PROP BASE Output_Dir \"Debug\"\r\n"
"# PROP BASE Intermediate_Dir \"Debug\"\r\n"
"# PROP BASE Target_Dir \"\"\r\n"
"# PROP Use_MFC 0\r\n"
"# PROP Use_Debug_Libraries 1\r\n"
"# PROP Output_Dir \"Debug\"\r\n"
"# PROP Intermediate_Dir \"Debug\"\r\n"
"# PROP Target_Dir \"\"\r\n"
"# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /D \"_USRDLL\" /D \"%s_EXPORTS\" /Yu\"stdafx.h\" /FD /GZ /c\r\n"
"# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /D \"_USRDLL\" /D \"%s_EXPORTS\" /FR /Yu\"stdafx.h\" /FD /GZ /c\r\n"
"# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
"# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
"# ADD BASE RSC /l 0x804 /d \"_DEBUG\"\r\n"
"# ADD RSC /l 0x804 /d \"_DEBUG\"\r\n";

const CHAR STR_VC6DSP6[] =
"BSC32=bscmake.exe\r\n"
"# ADD BASE BSC32 /nologo\r\n"
"# ADD BSC32 /nologo\r\n"
"LINK32=link.exe\r\n"
"# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept\r\n"
"# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept\r\n"
"\r\n"
"!ENDIF \r\n"
"\r\n"
"# Begin Target\r\n"
"\r\n"
"# Name \"%s - Win32 Release\"\r\n"
"# Name \"%s - Win32 Debug\"\r\n"
"# Begin Group \"Source Files\"\r\n"
"\r\n"
"# PROP Default_Filter \"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\"\r\n"
"# Begin Source File\r\n"
"\r\n"
"SOURCE=.\\%s.cpp\r\n"
"# End Source File\r\n"
"# Begin Source File\r\n";

const CHAR STR_VC6DSP7[] =
"\r\n"
"SOURCE=.\\StdAfx.cpp\r\n"
"# ADD CPP /Yc\"stdafx.h\"\r\n"
"# End Source File\r\n"
"# End Group\r\n"
"# Begin Group \"Header Files\"\r\n"
"\r\n"
"# PROP Default_Filter \"h;hpp;hxx;hm;inl\"\r\n"
"# Begin Source File\r\n"
"\r\n"
"SOURCE=.\\StdAfx.h\r\n"
"# End Source File\r\n"
"# End Group\r\n"
"# Begin Group \"Resource Files\"\r\n"
"\r\n"
"# PROP Default_Filter \"ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe\"\r\n"
"# End Group\r\n"
"# Begin Source File\r\n"
"\r\n"
"SOURCE=.\\ReadMe.txt\r\n"
"# End Source File\r\n"
"# End Target\r\n"
"# End Project\r\n"
"\r\n";

//1-->6  2--->3 3-->2  
//4-->2  5-->2  6-->3
//total 18
//__________________________________
const CHAR STR_VC6STDAFXCPP[] =
"// stdafx.cpp : source file that includes just the standard includes\r\n"
"//	%s.pch will be the pre-compiled header\r\n"
"//	stdafx.obj will contain the pre-compiled type information\r\n"
"\r\n"
"#include \"stdafx.h\"\r\n"
"\r\n"
"// TODO: reference any additional headers you need in STDAFX.H\r\n"
"// and not in this file\r\n"
"\r\n";
//______________________________________
const CHAR STR_VC6STDAFXH[] =
"// stdafx.h : include file for standard system include files,\r\n"
"//  or project specific include files that are used frequently, but\r\n"
"//      are changed infrequently\r\n"
"//\r\n"
"\r\n"
"#if !defined(AFX_STDAFX_H__%s__INCLUDED_)\r\n"
"#define AFX_STDAFX_H__%s__INCLUDED_\r\n"
"\r\n"
"#if _MSC_VER > 1000\r\n"
"#pragma once\r\n"
"#endif // _MSC_VER > 1000\r\n"
"\r\n"
"\r\n"
"// Insert your headers here\r\n"
"#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers\r\n"
"\r\n"
"#include <windows.h>\r\n"
"\r\n"
"// TODO: reference additional headers your program requires here\r\n"
"\r\n"
"//{{AFX_INSERT_LOCATION}}\r\n"
"// Microsoft Visual C++ will insert additional declarations immediately before the previous line.\r\n"
"\r\n"
"#endif // !defined(AFX_STDAFX_H__%s__INCLUDED_)\r\n"
"\r\n"
"\r\n";
//____________________________________
//内存加载实例
// 导出函数
//先统一返回int
//%s --->call type   %s func
//对于非C++类没有ecx 注释掉
const CHAR STR_HookFUNCSTR[] =
			  "typedef int  (%s * %s_funcPro)(arg);\r\n"
			  "%s\r\n"
			  "{\r\n"
			  "// 	//找到ECX\r\n"
			  "// 	unsigned int uECXThis=0;\r\n"
			  "// 	_asm\r\n"
			  "// 	{\r\n"
			  "// 		mov uECXThis,ecx;\r\n"
			  "// 	}\r\n"
			  "	CAdAutoHookApi autoHook(&gHooks, My_%s);\r\n"
			  "	%s_funcPro %s = (%s_funcPro)( gHooks.NewAddr2OldAddr(My_%s));\r\n"
			  "// 	_asm\r\n"
			  "// 	{\r\n"
			  "// 		mov ecx,uECXThis;\r\n"
			  "// 	}\r\n"
			  "	int   ret = %s();\r\n"
			  "	logOutput(formatString(\"ApiDebugger 2 - ret=%%d My_%s(arg)\",ret));\r\n"
			  "\r\n"	
			  "	return ret;\r\n"
"}\r\n";
//_______________________
//dll name  ,and function name
//_________________
const CHAR STR_HookADDCSTR[] =
"	gHooks.Add(_T(\"%s.dll\"), \"%s\", My_%s);\r\n";

//end private test

//apidbg 
const CHAR STR_boAPIDebug_cpp[] =
"// boAPIDebug.cpp : Defines the entry point for the DLL application.\r\n"
"//\r\n"
"\r\n"
"#include \"stdafx.h\"\r\n"
"\r\n"
"BOOL APIENTRY DllMain( HANDLE hModule, \r\n"
"                       DWORD  ul_reason_for_call, \r\n"
"                       LPVOID lpReserved\r\n"
"					 )\r\n"
"{\r\n"
"    return TRUE;\r\n"
"}\r\n";

const CHAR STR_ApiDebuggerP1_cpp[]=
"/////////////////////////////////////////////////////////////////////\r\n"
"// Author : CodeLive\r\n"
"// WeiBo  : http://weibo.com/1655194010\r\n"
"// Email  : dongfa@yeah.net\r\n"
"// QQ     : 5584173\r\n"
"// Date   : 2014.04.24\r\n"
"/////////////////////////////////////////////////////////////////////\r\n"
"\r\n"
"#include \"stdafx.h\"\r\n"
"#include \"ApiDebugger.h\"\r\n"
"#include \"AdHookApi.h\"\r\n"
"#include <tlhelp32.h>\r\n"
"#include <wincrypt.h>\r\n"
"#include \"Common.h\"\r\n"
"#include <Windows.h>\r\n"
"\r\n"
"static const char *   gCopyright = \"ApiDebugger by CodeLive, email : dongfa@yeah.net\";\r\n"
"static CAdHookApi     gHooks;\r\n"
"\r\n"
"bool gEnableLogOutput = true;\r\n"
"\r\n"
"extern \"C\" APIDEBUGGER const char * ApiDebugger()\r\n"
"{\r\n"
"    return gCopyright;\r\n"
"}\r\n";

const CHAR STR_ApiDebuggerP2_cpp[]=
"///////////////////////////////////////////////////////////////////////////////\r\n"
"//eg5  360 crake me\r\n"
"BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)\r\n"
"{\r\n"
"	switch (ul_reason_for_call)\r\n"
"	{\r\n"
"	case DLL_PROCESS_ATTACH:\r\n"
"        {\r\n"
"		   //head add hook\r\n";


const CHAR STR_ApiDebuggerP3_cpp[]=
"		\r\n"
"		    gHooks.BeginAll();\r\n"
"            logOutput(\"ApiDebugger Loaded.\\r\\n\");\r\n"
"       }\r\n"
"        break ;\r\n"
"	case DLL_THREAD_ATTACH:\r\n"
"	case DLL_THREAD_DETACH:\r\n"
"        {\r\n"
"        }\r\n"
"        break ;\r\n"
"	case DLL_PROCESS_DETACH:\r\n"
"        {\r\n"
"            ApiDebugferShutdown();\r\n"
"            logOutput(\"ApiDebugger Unloaded.\\r\\n\");\r\n"
"        }\r\n"
"		break;\r\n"
"	}\r\n"
"	return TRUE;\r\n"
"}\r\n"
"\r\n";



//end apidbg
//____________________________________
const CHAR STR_Macro[] =
	"// 宏定义\r\n"
	"#define EXTERNC extern \"C\"\r\n"
	"#define NAKED __declspec(naked)\r\n"
	"#define EXPORT __declspec(dllexport)\r\n"
	"\r\n"
	"#define ALCPP EXPORT NAKED\r\n"
	"#define ALSTD EXTERNC EXPORT NAKED void __stdcall\r\n"
	"#define ALCFAST EXTERNC EXPORT NAKED void __fastcall\r\n"
	"#define ALCDECL EXTERNC NAKED void __cdecl\r\n";

const CHAR STR_Hook[] =
	"// Hook 命名空间\r\n"
	"namespace Hook\r\n"
	"{\r\n"
	"	HHOOK m_hHook;\r\n			// HOOK 句柄\r\n"
	"\r\n"
	"\r\n"
	"	// HOOK 函数\r\n"
	"	LRESULT CALLBACK HookProc(INT iCode, WPARAM wParam, LPARAM lParam)\r\n"
	"	{\r\n"
	"		if (iCode > 0)\r\n"
	"		{\r\n"
	"			;\r\n"
	"		}\r\n"
	"\r\n"
	"		return CallNextHookEx(m_hHook, iCode, wParam, lParam);\r\n"
	"	}\r\n"
	"\r\n"
	"	// Hook\r\n"
	"	inline BOOL WINAPI Hook(INT iHookId = WH_CALLWNDPROC)\r\n"
	"	{\r\n"
	"		m_hHook = SetWindowsHookEx(iHookId, HookProc, NULL, GetCurrentThreadId());\r\n"
	"		return (m_hHook != NULL);\r\n"
	"	}\r\n"
	"\r\n"
	"	// Unhook\r\n"
	"	inline VOID WINAPI Unhook()\r\n"
	"	{\r\n"
	"		if (m_hHook)\r\n"
	"		{\r\n"
	"			UnhookWindowsHookEx(m_hHook);\r\n"
	"		}\r\n"
	"	}\r\n"
	"}\r\n";

const CHAR STR_AheadLib1[] =
	"// AheadLib 命名空间\r\n"
	"namespace AheadLib\r\n"
	"{\r\n"
	"	HMODULE m_hModule = NULL;	// 原始模块句柄\r\n"
	"	DWORD m_dwReturn[%d] = {0};	// 原始函数返回地址\r\n"
	"\r\n"
	"\r\n"
	"	// 加载原始模块\r\n"
	"	inline BOOL WINAPI Load()\r\n"
	"	{\r\n"
	"		TCHAR tzPath[MAX_PATH];\r\n"
	"		TCHAR tzTemp[MAX_PATH * 2];\r\n"
	"\r\n"
	"		%s%s\"));\r\n"
	"		m_hModule = LoadLibrary(tzPath);\r\n"
	"		if (m_hModule == NULL)\r\n"
	"		{\r\n"
	"			wsprintf(tzTemp, TEXT(\"无法加载 %%s，程序无法正常运行。\"), tzPath);\r\n"
	"			MessageBox(NULL, tzTemp, TEXT(\"AheadLib\"), MB_ICONSTOP);\r\n"
	"		}\r\n"
	"\r\n"
	"		return (m_hModule != NULL);	\r\n"
	"	}\r\n"
	"		\r\n"
	"	// 释放原始模块\r\n"
	"	inline VOID WINAPI Free()\r\n"
	"	{\r\n"
	"		if (m_hModule)\r\n"
	"		{\r\n"
	"			FreeLibrary(m_hModule);\r\n"
	"		}\r\n"
	"	}\r\n"
	"\r\n";

const CHAR STR_AheadLib2[] =
	"	// 获取原始函数地址\r\n"
	"	FARPROC WINAPI GetAddress(PCSTR pszProcName)\r\n"
	"	{\r\n"
	"		FARPROC fpAddress;\r\n"
	"		CHAR szProcName[16];\r\n"
	"		TCHAR tzTemp[MAX_PATH];\r\n"
	"\r\n"
	"%s"
	"		fpAddress = GetProcAddress(m_hModule, pszProcName);\r\n"
	"		if (fpAddress == NULL)\r\n"
	"		{\r\n"
	"			if (HIWORD(pszProcName) == 0)\r\n"
	"			{\r\n"
	"				wsprintf(szProcName, \"%%d\", pszProcName);\r\n"
	"				pszProcName = szProcName;\r\n"
	"			}\r\n"
	"\r\n"
	"			wsprintf(tzTemp, TEXT(\"无法找到函数 %%hs，程序无法正常运行。\"), pszProcName);\r\n"
	"			MessageBox(NULL, tzTemp, TEXT(\"AheadLib\"), MB_ICONSTOP);\r\n"
	"			ExitProcess(-2);\r\n"
	"		}\r\n"
	"\r\n"
	"		return fpAddress;\r\n"
	"	}\r\n"
	"}\r\n"
	"using namespace AheadLib;\r\n";

const CHAR STR_SystemPath[] = "GetSystemDirectory(tzPath, MAX_PATH);\r\n		lstrcat(tzPath, TEXT(\"\\\\";
const CHAR STR_DefaultPath[] = "lstrcpy(tzPath, TEXT(\"";
const CHAR STR_LoadInNeed[] =
	"		if (m_hModule == NULL)\r\n"
	"		{\r\n"
	"			if (Load() == FALSE)\r\n"
	"			{\r\n"
	"				ExitProcess(-1);\r\n"
	"			}\r\n"
	"		}\r\n"
	"\r\n";

const CHAR STR_DllMain[] =
	"// 入口函数\r\n"
	"BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)\r\n"
	"{\r\n"
	"	if (dwReason == DLL_PROCESS_ATTACH)\r\n"
	"	{\r\n"
	"		DisableThreadLibraryCalls(hModule);\r\n"
	"%s%s%s"
	"	}\r\n"
	"	else if (dwReason == DLL_PROCESS_DETACH)\r\n"
	"	{"
	"%s%s%s"
	"\r\n"
	"	}\r\n"
	"\r\n"
	"	return TRUE;\r\n"
	"}\r\n";

const CHAR STR_TlsAlloc[] =
"\r\n"
"		for (INT i = 0; i < sizeof(m_dwReturn) / sizeof(DWORD); i++)\r\n"
"		{\r\n"
"			m_dwReturn[i] = TlsAlloc();\r\n"
"		}\r\n";

const CHAR STR_TlsFree[] =
"\r\n"
"		for (INT i = 0; i < sizeof(m_dwReturn) / sizeof(DWORD); i++)\r\n"
"		{\r\n"
"			TlsFree(m_dwReturn[i]);\r\n"
"		}\r\n";

const CHAR STR_HookHook[] = "\r\n		Hook::Hook();\r\n";
const CHAR STR_HookUnhook[] = "\r\n		Hook::Unhook();\r\n";
const CHAR STR_LoadInEntry[] = "\r\n		return Load();\r\n";
const CHAR STR_FreeInEntry[] = "\r\n		Free();";

const CHAR STR_JumpToOrigin[] =
	"// 导出函数\r\n"
	"%s\r\n"
	"{\r\n"
	"	GetAddress(%s);\r\n"
	"	__asm JMP EAX;\r\n"
	"}\r\n";
//普通调用是这里
const CHAR STR_CallOrigin[] =
	"// 导出函数\r\n"
	"%s\r\n"
	"{\r\n"
	"	// 保存返回地址\r\n"
	"	__asm POP m_dwReturn[%d * TYPE long];\r\n\r\n"
	"	// 调用原始函数\r\n"
	"	OutputDebugString(\"Ahead:%s);\r\n\r\n"
	"	GetAddress(%s)();\r\n\r\n"

	"	// 转跳到返回地址\r\n"
	"	__asm JMP m_dwReturn[%d * TYPE long];\r\n"
	"}\r\n";
//支持多线程的
const CHAR STR_CallOriginTls[] =
	"// 导出函数\r\n"
	"%s\r\n"
	"{\r\n"
	"	// 保存返回地址到 TLS\r\n"
	"	__asm PUSH m_dwReturn[%d * TYPE long];\r\n"
	"	__asm CALL DWORD PTR [TlsSetValue];\r\n\r\n"
	"	// 调用原始函数\r\n"
	"	OutputDebugString(\"Ahead:%s);\r\n\r\n"
	"	GetAddress(%s)();\r\n\r\n"
	"	// 获取返回地址并返回\r\n"
	"	__asm PUSH EAX;\r\n"
	"	__asm PUSH m_dwReturn[%d * TYPE long];\r\n"
	"	__asm CALL DWORD PTR [TlsGetValue];\r\n"
	"	__asm XCHG EAX, [ESP];\r\n"
	"	__asm RET;\r\n"
	"}\r\n";

const CHAR STR_ExportVariable[] = "// 导出变量\r\nEXPORT %s;\r\n";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAheadLib 类静态成员变量
BOOL CAheadLib::m_bCallRedirect = FALSE;
BOOL CAheadLib::m_bLoadIfNeed = FALSE;
BOOL CAheadLib::m_bJumpToOrigin = FALSE;
BOOL CAheadLib::m_bMultiThread = FALSE;
BOOL CAheadLib::m_bSystemPath = FALSE;
BOOL CAheadLib::m_bGenerateHook = FALSE;
//静态变量可以初始化一次
TCHAR CAheadLib::m_tzPROJECTNAME[MAX_PATH]={0};
TCHAR CAheadLib::m_PRJNAME[MAX_PATH]={0};
TCHAR CAheadLib::m_tzAPIDbgDSWPATH[MAX_PATH]={0};
TCHAR CAheadLib::m_tzDllFile[MAX_PATH] = {0};
TCHAR CAheadLib::m_tzCppFile[MAX_PATH] = {0};
CHAR CAheadLib::m_szOriginDll[MAX_PATH] = {0};
CHAR CAheadLib::m_szOrigin[MAX_PATH] = {0};
HMODULE CAheadLib::m_hDbgHelp = NULL;
UNDECORATESYMBOLNAME CAheadLib::m_pUnDecorateSymbolName = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ALRESULT WINAPI CAheadLib::Generate(BOOL bWriteCppFile,BOOL bdirCustomer)
{
	//预览时候bWriteCppFile =0
	DWORD i;
	PSTR pszFile;
	DWORD dwSize;
	PSTR pszBuffer;
	PSTR *ppszNames;
	PDWORD pdwNames;
	BOOL bLoadOrigin;
	ALRESULT alResult;
	PWORD pwNamesOrdinals;
	PIMAGE_NT_HEADERS pInh;
	PIMAGE_EXPORT_DIRECTORY pIed;

	_Try
	{
		// 初始化
		pszFile = NULL;
		ppszNames = NULL;
		pszBuffer = NULL;
		SetDlgItemTextA(CMainDlg::m_hWnd, IDC_Preview, NULL);

		// 判断参数是否完整
		_LeaveExIf((m_szOrigin[0] == 0) || (m_tzDllFile[0] == 0) || (bWriteCppFile && (m_tzDllFile[0] == 0)),
			alResult = ALResult_InvalidArgs);

		// 打开 DLL 文件
		CMapFile mfFile(m_tzDllFile);
		pszFile = (PSTR) mfFile.m_pvFile;
		_LeaveExIf(pszFile == NULL, alResult = ALResult_OpenDllFile);
		if(mfFile.m_dwSize<512) return ALResult_InvalidPeFile;
		_LeaveExIf(mfFile.m_dwSize < 512, alResult = ALResult_InvalidPeFile);
             //这里仅仅通过dll size 判断是否是PE有效文件
		// 判断是否为 PE 文件
		pInh = (PIMAGE_NT_HEADERS) (pszFile + ((PIMAGE_DOS_HEADER) pszFile)->e_lfanew);
        if(pInh->Signature != IMAGE_NT_SIGNATURE) return ALResult_InvalidPeFile;
		_LeaveExIf(pInh->Signature != IMAGE_NT_SIGNATURE, alResult = ALResult_InvalidPeFile);
 
		// 定位导出表  从数据目录中
		dwSize = pInh->OptionalHeader.DataDirectory[0].VirtualAddress;
		_LeaveExIf(dwSize == 0, alResult = ALResult_NoExportTable);
		pIed = (PIMAGE_EXPORT_DIRECTORY) (pszFile + RvaToOffset(pInh, dwSize));

		// 获取 Names 和 Ordinals
		pdwNames = (PDWORD) (pszFile + RvaToOffset(pInh, pIed->AddressOfNames));
		pwNamesOrdinals = (PWORD) (pszFile + RvaToOffset(pInh, pIed->AddressOfNameOrdinals));

		// 分配内存
		ppszNames = (PSTR *) _HeapAllocZero(sizeof(PSTR) * pIed->NumberOfFunctions);
		_LeaveExIf(ppszNames == NULL, alResult = ALResult_HeapAlloc);

		// 获取导出函数名称
		for (i = 0; i < pIed->NumberOfNames; i++)
		{
			ppszNames[pwNamesOrdinals[i]] = pszFile + RvaToOffset(pInh, pdwNames[i]);
		}

		// 分配内存
		pszBuffer = (PSTR) _HeapAlloc(4096 + 1024 * pIed->NumberOfFunctions);
		_LeaveExIf(pszBuffer == NULL, alResult = ALResult_HeapAlloc);

		// 生成代码并设置到预览编辑框
		dwSize = GenerateCode(pszBuffer, ppszNames, pIed->Base, pIed->NumberOfFunctions, bLoadOrigin);
		SetDlgItemTextA(CMainDlg::m_hWnd, IDC_Preview, pszBuffer);

		// 写入到文件
		if (bWriteCppFile)
		{
			_LeaveExIf(WriteCppFile(pszBuffer, dwSize,bdirCustomer) == FALSE, alResult = ALResult_WriteCppFile);
		}

		// 执行成功
		alResult = (bLoadOrigin && m_bCallRedirect) ? ALResult_NotAllRedirect : ALResult_Success;
	}
	_Finally
	{
		// 释放
		_ExIf(pszBuffer, _HeapFree(pszBuffer));
		_ExIf(ppszNames, _HeapFree(ppszNames));

		return alResult;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成代码
DWORD WINAPI CAheadLib::GenerateCode(PSTR pszBuffer, PSTR* ppszNames, DWORD dwBase, DWORD dwCount, BOOL& bLoadOrigin)
{
	PSTR p;
	DWORD i;

	// 预处理
	p = pszBuffer;
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	CopyString(p, STR_Preprocessor, _LengthOf(STR_Preprocessor));
	CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));

	// 导出函数
	bLoadOrigin = GenerateExport(p, ppszNames, dwBase, dwCount);

	// 宏定义
	if (bLoadOrigin)
	{
		CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
		CopyString(p, STR_Macro, _LengthOf(STR_Macro));
		CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
	}

	// HOOK 代码
	if (m_bGenerateHook)
	{
		CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
		CopyString(p, STR_Hook, _LengthOf(STR_Hook));
		CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
	}

	// AheadLib 代码
	if (bLoadOrigin)
	{
	
		CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
		p += wsprintfA(p, STR_AheadLib1,
			dwCount,
			m_bSystemPath ? STR_SystemPath : STR_DefaultPath,
			m_szOrigin);
		p += wsprintfA(p, STR_AheadLib2, m_bLoadIfNeed ? STR_LoadInNeed : NULL);
		CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
	}
//改了dll后缀 到此
	// 入口函数
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	p += wsprintfA(p, STR_DllMain,
		m_bMultiThread ? STR_TlsAlloc: NULL,
		m_bGenerateHook ? STR_HookHook : NULL,
		bLoadOrigin && (m_bLoadIfNeed == FALSE) ? STR_LoadInEntry : NULL,

		m_bMultiThread ? STR_TlsFree: NULL,
		bLoadOrigin ? STR_FreeInEntry : NULL,
		m_bGenerateHook ? STR_HookUnhook : NULL);
	CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));

	// 函数
	for (i = 0; i < dwCount; i++)
	{
		// 如果可以直接转发
		if (CanRedirect(ppszNames[i]) == FALSE)
		{
			GenerateFunction(p, ppszNames[i], dwBase, i);
		}
	}

	*p = 0;

	return (DWORD) (p - pszBuffer);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成代码
DWORD WINAPI CAheadLib::GenerateMemHookCode(PSTR pszBuffer, PSTR* ppszNames, DWORD dwBase, DWORD dwCount, BOOL& bLoadOrigin)
{
	PSTR p;
	DWORD i;
	
	// 预处理
	p = pszBuffer;
// 	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
// 	CopyString(p, STR_Preprocessor, _LengthOf(STR_Preprocessor));
// 	CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
// 	
// 	// 导出函数
// 	bLoadOrigin = GenerateExport(p, ppszNames, dwBase, dwCount);
	
	// 宏定义
// 	if (bLoadOrigin)
// 	{
// 		CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
// 		CopyString(p, STR_Macro, _LengthOf(STR_Macro));
// 		CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
// 	}
// 	
// 	// HOOK 代码
// 	if (m_bGenerateHook)
// 	{
// 		CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
// 		CopyString(p, STR_Hook, _LengthOf(STR_Hook));
// 		CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
// 	}
	
	// AheadLib 代码
// 	if (bLoadOrigin)
// 	{
// 		
// 		CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
// 		p += wsprintfA(p, STR_AheadLib1,
// 			dwCount,
// 			m_bSystemPath ? STR_SystemPath : STR_DefaultPath,
// 			m_szOrigin);
// 		p += wsprintfA(p, STR_AheadLib2, m_bLoadIfNeed ? STR_LoadInNeed : NULL);
// 		CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
// 	}
	//改了dll后缀 到此
	// 入口函数
// 	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
// 	p += wsprintfA(p, STR_DllMain,
// 		m_bMultiThread ? STR_TlsAlloc: NULL,
// 		m_bGenerateHook ? STR_HookHook : NULL,
// 		bLoadOrigin && (m_bLoadIfNeed == FALSE) ? STR_LoadInEntry : NULL,
// 		
// 		m_bMultiThread ? STR_TlsFree: NULL,
// 		bLoadOrigin ? STR_FreeInEntry : NULL,
// 		m_bGenerateHook ? STR_HookUnhook : NULL);
// 	CopyString(p, STR_BlockTail, _LengthOf(STR_BlockTail));
	
	// 函数
	for (i = 0; i < dwCount; i++)
	{	
		GenerateMemHookFunction(p, ppszNames[i], dwBase, i);
	}

	//再生成    add

	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	for (i = 0; i < dwCount; i++)
	{
         //如果是 无导出名则 。。。
		p += wsprintfA(p,
			STR_HookADDCSTR,
			m_PRJNAME,
			ppszNames[i], ppszNames[i]);
	}
	*p = 0;
	
	return (DWORD) (p - pszBuffer);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI CAheadLib::GenerateMemHookCPPFullCode(PSTR pszBuffer, PSTR* ppszNames, DWORD dwBase, DWORD dwCount, BOOL& bLoadOrigin)
{
	PSTR p;
	DWORD i;
	p = pszBuffer;
    CopyString(p, STR_ApiDebuggerP1_cpp, _LengthOf(STR_ApiDebuggerP1_cpp));

	// 函数
	for (i = 0; i < dwCount; i++)
	{	
		GenerateMemHookFunction(p, ppszNames[i], dwBase, i);
	}
	 CopyString(p, STR_ApiDebuggerP2_cpp, _LengthOf(STR_ApiDebuggerP1_cpp));
	//再生成    add
	
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	CopyString(p, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	for (i = 0; i < dwCount; i++)
	{
		//如果是 无导出名则 。。。
		p += wsprintfA(p,
			STR_HookADDCSTR,
			m_PRJNAME,
			ppszNames[i], ppszNames[i]);
	}
	 CopyString(p, STR_ApiDebuggerP3_cpp, _LengthOf(STR_ApiDebuggerP1_cpp));
	*p = 0;
	
	return (DWORD) (p - pszBuffer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 BOOL WINAPI CAheadLib:: WriteApiDebugger_CPP()
 {

	 	if(strlen(m_PRJNAME)<1) return 0;
		PSTR pszBuffer=NULL;
		PTSTR p=NULL;
	 
 
		std::string   strPath=m_tzAPIDbgDSWPATH;
		strPath = strPath+ "\\";
	//stdcpp
	{
		pszBuffer=NULL;
		p=NULL;

			pszBuffer=NULL;
			pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
			//_LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
			p=pszBuffer;
			p += wsprintfA(p, STR_VC6STDAFXCPP,
				m_PRJNAME			
				);
			WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),std::string(strPath+"StdAfx.cpp").c_str());
		// 释放
			 _HeapFree(pszBuffer);	
	}
	//stdh
	{
		pszBuffer=NULL;
		p=NULL;
		TCHAR randStr[MAX_PATH]={0};

		/* Seed the random-number generator with current time so that
		·  the numbers will be different every time we run.
		*/
		srand( (unsigned)time( NULL ) );
		int iRand=rand();
		wsprintfA(randStr,"H%X%XH",iRand,iRand);
	
			pszBuffer=NULL;
			pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
			//_LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
			p=pszBuffer;
			p += wsprintfA(p, STR_VC6STDAFXH,
				randStr,randStr,randStr			
				);
		
			WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),std::string(strPath+"StdAfx.h").c_str());			
			// 释放
			 _HeapFree(pszBuffer);	
	}

		//boapidebugcpp
	{
		pszBuffer=NULL;
		p=NULL;
		
		pszBuffer=NULL;
		pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
		//_LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
		p=pszBuffer;
		p += wsprintfA(p, STR_boAPIDebug_cpp		
			);
		WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),std::string(strPath+"boAPIDebug.cpp").c_str());
		// 释放
			 _HeapFree(pszBuffer);	
	}
	GenRcDswApiDbg(std::string(strPath+"ApiDebugger.cpp").c_str());
	return 1;
 
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 设置选项可用状态
VOID WINAPI CAheadLib::SetOptionsEnabled()
{
	EnableWindow(GetDlgItem(CMainDlg::m_hWnd, IDC_LoadInEntry), m_bCallRedirect == FALSE);
	EnableWindow(GetDlgItem(CMainDlg::m_hWnd, IDC_LoadIfNeed), m_bCallRedirect == FALSE);
	EnableWindow(GetDlgItem(CMainDlg::m_hWnd, IDC_JumpToOrigin), m_bCallRedirect == FALSE);
	EnableWindow(GetDlgItem(CMainDlg::m_hWnd, IDC_CallOrigin), m_bCallRedirect == FALSE);
	EnableWindow(GetDlgItem(CMainDlg::m_hWnd, IDC_MultiThread), (m_bCallRedirect == FALSE) && (m_bJumpToOrigin == FALSE));
	EnableWindow(GetDlgItem(CMainDlg::m_hWnd, IDC_SystemPath), m_bCallRedirect == FALSE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 从 INI 中载入选项设置
VOID WINAPI CAheadLib::LoadOptions()
{
	m_bCallRedirect = CIni::GetInt(INI_CallRedirect);
	m_bLoadIfNeed = CIni::GetInt(INI_LoadIfNeed);
	m_bJumpToOrigin = CIni::GetInt(INI_JumpToOrign);
	m_bMultiThread = CIni::GetInt(INI_MultiThread);
	m_bSystemPath = CIni::GetInt(INI_SystemPath);
	m_bGenerateHook = CIni::GetInt(INI_GenerateHook);

	CheckRadioButton(CMainDlg::m_hWnd, IDC_CallRedirect, IDC_CallInTime,
		m_bCallRedirect ? IDC_CallRedirect : IDC_CallInTime);
	CheckRadioButton(CMainDlg::m_hWnd, IDC_LoadInEntry, IDC_LoadIfNeed,
		m_bLoadIfNeed ? IDC_LoadIfNeed : IDC_LoadInEntry);
	CheckRadioButton(CMainDlg::m_hWnd, IDC_JumpToOrigin, IDC_CallOrigin,
		m_bJumpToOrigin ? IDC_JumpToOrigin : IDC_CallOrigin);
	CheckDlgButton(CMainDlg::m_hWnd, IDC_MultiThread, m_bMultiThread);
	CheckDlgButton(CMainDlg::m_hWnd, IDC_SystemPath, m_bSystemPath);
	CheckDlgButton(CMainDlg::m_hWnd, IDC_GenerateHook, m_bGenerateHook);

	SetOptionsEnabled();

	m_hDbgHelp = LoadLibrary(TEXT("DbgHelp"));
	if (m_hDbgHelp)
	{
		m_pUnDecorateSymbolName = (UNDECORATESYMBOLNAME) GetProcAddress(m_hDbgHelp, "UnDecorateSymbolName");
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 保存选项设置
VOID WINAPI CAheadLib::SaveOptions()
{
	CIni::SetInt(INI_CallRedirect, m_bCallRedirect);
	CIni::SetInt(INI_LoadIfNeed, m_bLoadIfNeed);
	CIni::SetInt(INI_JumpToOrign, m_bJumpToOrigin);
	CIni::SetInt(INI_MultiThread, m_bMultiThread);
	CIni::SetInt(INI_SystemPath, m_bSystemPath);
	CIni::SetInt(INI_GenerateHook, m_bGenerateHook);

	_SafeFreeLibrary(m_hDbgHelp);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 读取选项设置
VOID WINAPI CAheadLib::GetOptions()
{
	m_bCallRedirect = IsDlgButtonChecked(CMainDlg::m_hWnd, IDC_CallRedirect);
	m_bLoadIfNeed = IsDlgButtonChecked(CMainDlg::m_hWnd, IDC_LoadIfNeed);
	m_bJumpToOrigin = IsDlgButtonChecked(CMainDlg::m_hWnd, IDC_JumpToOrigin);
	m_bMultiThread = IsDlgButtonChecked(CMainDlg::m_hWnd, IDC_MultiThread);
	m_bSystemPath = IsDlgButtonChecked(CMainDlg::m_hWnd, IDC_SystemPath);
	m_bGenerateHook = IsDlgButtonChecked(CMainDlg::m_hWnd, IDC_GenerateHook);

	SetOptionsEnabled();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成导出指示符
BOOL WINAPI CAheadLib::GenerateExport(PSTR& pszBuffer, PSTR* ppszNames, DWORD dwBase, DWORD dwCount)
{
	PSTR p;
	DWORD i;
	BOOL bLoadOrigin;
	BOOL bPragmaExport;

	bLoadOrigin = FALSE;
	bPragmaExport = FALSE;
	for (i = 0; i < dwCount; i++)
	{
		// 如果可以直接转发
		if (CanRedirect(ppszNames[i]))
		{
			// 转发导出函数
			PragmaExport(pszBuffer, bPragmaExport, dwBase + i, ppszNames[i], TRUE);
		}
		else
		{
			switch (GetExportType(ppszNames[i], p))
			{
			case ET_NONAME:
				// 无名称导出函数
				PragmaExport(pszBuffer, bPragmaExport, dwBase + i);
				break;

			case ET_CDECL:
				// 常规导出函数
				PragmaExport(pszBuffer, bPragmaExport, dwBase + i, ppszNames[i]);
				break;

			//default:
				// 直接用 __declspec(dllexport) 导出
			}

			bLoadOrigin = TRUE;
		}
	}

	if (bPragmaExport)
	{
		CopyString(pszBuffer, STR_BlockTail, _LengthOf(STR_BlockTail));
	}

	return bLoadOrigin;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成函数
VOID WINAPI CAheadLib::GenerateFunction(PSTR& pszBuffer, PSTR pszName, DWORD dwBase, DWORD dwIndex)
{
	CHAR szCallName[MAX_PATH];
	CHAR szDeclaration[MAX_PATH * 2];

	CopyString(pszBuffer, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	GenerateDeclaration(szDeclaration, szCallName, pszName, dwBase + dwIndex);
	if (szDeclaration[lstrlenA(szDeclaration) - 1] == ')')
	{
		// 函数
		if (m_bJumpToOrigin)
		{
			// 直接转跳到原始函数
			pszBuffer += wsprintfA(pszBuffer, STR_JumpToOrigin, szDeclaration, szCallName);
		}
		else
		{
			// 调用后返回
            //如果是C++则	  更改2 bob
		  PSTR p;
	      EXPORTTYPE etType;
	      etType = GetExportType(pszName, p);
		  CHAR TempszDeclaration[MAX_PATH * 2]={0};
		  if(etType== ET_CPP)
		  {
				m_pUnDecorateSymbolName(pszName, TempszDeclaration, MAX_PATH * 2, 0);
				INT len=strlen(TempszDeclaration);
				if(len<1)
				{
					//解析不成功
					pszBuffer += wsprintfA(pszBuffer,
						(m_bMultiThread ? STR_CallOriginTls : STR_CallOrigin),
				szDeclaration, dwIndex, szCallName+1,szCallName ,dwIndex);//+1 去前引号

				}else
				{
                   TempszDeclaration[len]='\"';
				   TempszDeclaration[len+1]=0;
				   pszBuffer += wsprintfA(pszBuffer,
					   (m_bMultiThread ? STR_CallOriginTls : STR_CallOrigin),
				   szDeclaration, dwIndex, TempszDeclaration,szCallName ,dwIndex);//+多少视情况而定 去前引号
				}

		  }else
		  {
			
			pszBuffer += wsprintfA(pszBuffer,
				(m_bMultiThread ? STR_CallOriginTls : STR_CallOrigin),
				szDeclaration, dwIndex, szCallName+1,szCallName ,dwIndex);//+1 去前引号
		  }
			/*
			// 导出函数
			ALCDECL AheadLib_SetUserLensRI_3L8(void)
			{
			// 保存返回地址
			__asm POP m_dwReturn[8 * TYPE long];
			
			  // 调用原始函数
			  GetAddress("SetUserLensRI_3L8")();
			  
				// 转跳到返回地址
				__asm JMP m_dwReturn[8 * TYPE long];
}
			*/
               //szDeclaration=ALCDECL
				 //szCallName=  AheadLib_SetUserLensRI_3L8(void)
				   //	GetAddress("SetUserLensRI_3L8")();
			   //dwIndex=8
		} 
	}
	else
	{
		// 变量
		pszBuffer += wsprintfA(pszBuffer, STR_ExportVariable, szDeclaration);
	}
	CopyString(pszBuffer, STR_BlockTail, _LengthOf(STR_BlockTail));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成自定义  memhook
VOID WINAPI CAheadLib::GenerateMemHookFunction(PSTR& pszBuffer, PSTR pszName, DWORD dwBase, DWORD dwIndex)
{
	CHAR szCallName[MAX_PATH];
	CHAR szDeclaration[MAX_PATH * 2];

	CopyString(pszBuffer, STR_BlockHeader, _LengthOf(STR_BlockHeader));
	GenerateMemHookDeclaration(szDeclaration, szCallName, pszName, dwBase + dwIndex);
	if (szDeclaration[lstrlenA(szDeclaration) - 1] == ')')
	{
		{
			// 调用后返回
            //如果是C++则	  更改2 bob
		  PSTR p;
	      EXPORTTYPE etType;
	      etType = GetExportType(pszName, p);
		  CHAR TempszDeclaration[MAX_PATH * 2]={0};
		  if(etType== ET_CPP)
		  {
				m_pUnDecorateSymbolName(pszName, TempszDeclaration, MAX_PATH * 2, 0);
				INT len=strlen(TempszDeclaration);
				pszBuffer += wsprintfA(pszBuffer,
					STR_HookFUNCSTR,
						(m_bGenerateHook ? "__stdcall" : "__cdecl"),
					pszName,
					szDeclaration, pszName, 
					pszName,pszName ,pszName,
				pszName,pszName,pszName);

		  }else
		  {	
			pszBuffer += wsprintfA(pszBuffer,
				 STR_HookFUNCSTR,
				 	(m_bGenerateHook ? "__stdcall" : "__cdecl"),
				  pszName,
				szDeclaration, pszName, 
				pszName,pszName ,pszName,
				pszName,pszName,pszName);
		
		  }
		} 
	}
	else
	{
		// 变量
		pszBuffer += wsprintfA(pszBuffer, STR_ExportVariable, szDeclaration);
	}
	CopyString(pszBuffer, STR_BlockTail, _LengthOf(STR_BlockTail));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成合法的函数名称
VOID WINAPI CAheadLib::GenerateDeclaration(PSTR pszDeclaration, PSTR pszCallName, PSTR pszName, DWORD Ordinals)
{
	PSTR p;
	EXPORTTYPE etType;

	etType = GetExportType(pszName, p);
	switch (etType)
	{
	case ET_CPP:
		// C++ 方式导出
		if (pszName[lstrlenA(pszName) - 1] != 'A')
		{
			// 如果是导出函数
			CopyString(pszDeclaration, "ALCPP ", _LengthOf("ALCPP "));
		}
		m_pUnDecorateSymbolName(pszName, pszDeclaration, MAX_PATH * 2, 0);
		break;

	case ET_STD:
	case ET_FAST:
		// __fastcall/__stdcall 方式导出
		if (etType == ET_STD)
		{
			CopyString(pszDeclaration,  "ALSTD ", _LengthOf("ALSTD "));
		}
		else
		{
			CopyString(pszDeclaration,  "ALFAST ", _LengthOf("ALFAST "));
		}
		CopyString(pszDeclaration, pszName + 1, (INT) (p - pszName - 1));
		GenerateParam(pszDeclaration, (_AStrToInt(p + 1) / 4));
		break;

	case ET_CDECL:
		// __cdecl 方式导出
		wsprintfA(pszDeclaration, "ALCDECL AheadLib_%s(void)", pszName);
		break;

	case ET_NONAME:
		// __cdecl 方式导出
		wsprintfA(pszDeclaration, "ALCDECL AheadLib_Noname%d(void)", Ordinals);

		// 调用名称
		wsprintfA(pszCallName, "MAKEINTRESOURCE(%d)", Ordinals);
		return;
	}

	// 调用名称
	wsprintfA(pszCallName, "\"%s\"", pszName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//无 AHead前缀
VOID WINAPI CAheadLib::GenerateMemHookDeclaration(PSTR pszDeclaration, PSTR pszCallName, PSTR pszName, DWORD Ordinals)
{
	PSTR p;
	EXPORTTYPE etType;
	
	etType = GetExportType(pszName, p);
	switch (etType)
	{
	case ET_CPP:
		// C++ 方式导出
		if (pszName[lstrlenA(pszName) - 1] != 'A')
		{
			
			// 如果是导出函数 c++ 用 stdcall 注意空格
			CopyString(pszDeclaration, "int __stdcall ", _LengthOf("int __stdcall "));
		}
		m_pUnDecorateSymbolName(pszName, pszDeclaration, MAX_PATH * 2, 0);  //要提取的
		break;
		
	case ET_STD:
	case ET_FAST:
		// __fastcall/__stdcall 方式导出
		if (etType == ET_STD)
		{
			CopyString(pszDeclaration,  "int __stdcall ", _LengthOf("int __stdcall "));
		}
		else
		{
			CopyString(pszDeclaration,  "int __fastcall ", _LengthOf("int __fastcall "));
		}
		CopyString(pszDeclaration, pszName + 1, (INT) (p - pszName - 1));
		GenerateParam(pszDeclaration, (_AStrToInt(p + 1) / 4)); //对于stdcall可以自动生成参数表
		break;
		
	case ET_CDECL:
		// __cdecl 方式导出
		if(m_bGenerateHook)//stdcall 
		{
         	wsprintfA(pszDeclaration, "int __stdcall My_%s(arg)", pszName);
		}else
		{
             wsprintfA(pszDeclaration, "int __cdecl My_%s(arg)", pszName);

		}
	
		break;
		
	case ET_NONAME:
		// __cdecl 方式导出
	if(m_bGenerateHook)//stdcall 
	{
			wsprintfA(pszDeclaration, "int __stdcall My_Noname%d(arg)", Ordinals);
	}else
	{
	wsprintfA(pszDeclaration, "int __cdecl My_Noname%d(arg)", Ordinals);
	}
	
		
		// 调用名称
		wsprintfA(pszCallName, "MAKEINTRESOURCE(%d)", Ordinals);
		return;
	}
	
	// 调用名称
	wsprintfA(pszCallName, "%s", pszName);//不要引号
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 把 RVA 转换为文件偏移
DWORD WINAPI CAheadLib::RvaToOffset(const PIMAGE_NT_HEADERS pInh, DWORD dwRva)
{
	INT i;
	PIMAGE_SECTION_HEADER pIsh;

	pIsh = (PIMAGE_SECTION_HEADER) ((PBYTE) pInh + sizeof(IMAGE_NT_HEADERS));
	for (i = 0; i < pInh->FileHeader.NumberOfSections; i++, pIsh++)
	{
		if (dwRva >= (pIsh->VirtualAddress))
		{
			if (((pIsh->VirtualAddress) + (pIsh->SizeOfRawData)) > dwRva)
			{
				return dwRva - pIsh->VirtualAddress + pIsh->PointerToRawData;
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 判断导出函数类型
EXPORTTYPE WINAPI CAheadLib::GetExportType(PCSTR pszName, PSTR& pAt)
{
	if(pszName == NULL)
	{
		return ET_NONAME;
	}
	else if (m_pUnDecorateSymbolName && (pszName[0] == '?'))
	{
		return ET_CPP;
	}
	else if ((pszName[0] == '@') || (pszName[0] == '_'))
	{
		pAt = _AStrChr(pszName + 1, '@');
		if (pAt && IsStringNumberic(pAt + 1))
		{
			return (pszName[0] == '@') ? ET_FAST : ET_STD;
		}
	}
	return ET_CDECL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 生成参数列表
VOID WINAPI CAheadLib::GenerateParam(PSTR pszBuffer, INT iParamCount)
{
	if (iParamCount)
	{
		// 参数表
		*pszBuffer++ = '(';
		for (INT i = 0; i < iParamCount; i++)
		{
			CopyString(pszBuffer, "long, ", _LengthOf("long, "));
		}
		*(pszBuffer - 2) = ')';
		*(pszBuffer - 1) = 0;
	}
	else
	{
		// 无参数表
		lstrcpyA(pszBuffer, "(void)");
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 生成 #pragma 导出指示符
//static VOID WINAPI PragmaExport(PSTR& pszBuffer, BOOL& bPragmaExport, DWORD Ordinals,
//		PCSTR pszName = NULL, BOOL bRedirect = FALSE);
// 生成 #pragma 导出指示符
VOID WINAPI CAheadLib::PragmaExport(PSTR& pszBuffer, BOOL& bPragmaExport, DWORD Ordinals, PCSTR pszName, BOOL bRedirect)
{
	if (bPragmaExport == FALSE)
	{
		CopyString(pszBuffer, STR_BlockHeader, _LengthOf(STR_BlockHeader));
		CopyString(pszBuffer, STR_ExportComment, _LengthOf(STR_ExportComment));
		bPragmaExport = TRUE;
	}

	if (bRedirect)
	{

		
		CHAR tempSzOrigin[MAX_PATH]={0};
		CopyMemory(tempSzOrigin,m_szOrigin,MAX_PATH);
		PSTR p = _AStrRChr(tempSzOrigin, '.');
		_ExIf(p, *p = 0);//这里去除了后缀
		pszBuffer += wsprintfA(pszBuffer, STR_ExportRedirect, pszName, tempSzOrigin, pszName, Ordinals);
	}
	else
	{
		if (pszName)
		{
			pszBuffer += wsprintfA(pszBuffer, STR_ExportNormal,
				(pszName[0] == '_') ? "_" :NULL,
				pszName, pszName, Ordinals);
		}
		else
		{
			pszBuffer += wsprintfA(pszBuffer, STR_ExportNoname, Ordinals, Ordinals, Ordinals);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 写入到 CPP 文件
BOOL WINAPI CAheadLib::WriteCppFile(PCSTR pszBuffer, DWORD dwSize,BOOL bdirCustomer)
{
	BOOL bResult;
	HANDLE hFile;
   // [10540] E:\SVN7\pojie2\Ahead\LSCDLL_3L8_V22.cpp

   if(bdirCustomer==TRUE)
   {
	   TCHAR strPath[MAX_PATH]={0};
	   CopyMemory(strPath,CAheadLib::m_tzPROJECTNAME,MAX_PATH);
	   PTSTR Bp = _AStrRChr(strPath, '\\');
	   Bp =Bp +1 ;//保留
       *Bp = 0;//这里去除了后缀
	   strcat(strPath,m_PRJNAME);
	   strcat(strPath,".cpp");
	   	hFile = _CreateFileForWrite(strPath);
   }
   else
   {
	hFile = _CreateFileForWrite(m_tzCppFile);
   }
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	bResult = WriteFile(hFile, pszBuffer, dwSize, &dwSize, NULL);
	CloseHandle(hFile);
	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 写入到  文件
BOOL WINAPI CAheadLib::WriteSzFile(PCSTR pszBuffer, DWORD dwSize,const char * szPath)
{
	BOOL bResult;
	HANDLE hFile;
	// [10540] E:\SVN7\pojie2\Ahead\LSCDLL_3L8_V22.cpp
	if(strlen(szPath)<1) return 0;
 
	hFile = _CreateFileForWrite(szPath);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	bResult = WriteFile(hFile, pszBuffer, dwSize, &dwSize, NULL);
	CloseHandle(hFile);
	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//生成DSW
BOOL WINAPI CAheadLib::GenDSW()
{
    //	_LeaveExIf(WriteCppFile(pszBuffer, dwSize) == FALSE, alResult = ALResult_WriteCppFile);
 
		if(strlen(m_PRJNAME)<1) return 0;
		PSTR pszBuffer=NULL;
		PTSTR p=NULL;
		TCHAR strPath[MAX_PATH] = {0};
	//dsw
	{
		pszBuffer=NULL;
		p=NULL;
		
		pszBuffer=NULL;
			pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
	       // _LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
			p=pszBuffer;
			p += wsprintfA(p, STR_VC6DSW,
				m_PRJNAME,
				m_PRJNAME);
        //strcat会改变原值
            CopyMemory(strPath,CAheadLib::m_tzPROJECTNAME,MAX_PATH);
			WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),strcat(strPath,".dsw"));
		
		// 释放
		_HeapFree(pszBuffer);	
	}

	//dsp
	{
		pszBuffer=NULL;
		p=NULL;
		
	
			pszBuffer=NULL;
			pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
		//	_LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
			p=pszBuffer;
			//1-->6  2--->3 3-->2  
			//4-->2  5-->2  6-->3  7-->0
            //total 18
			p += wsprintfA(p, STR_VC6DSP1,
				m_PRJNAME,m_PRJNAME,m_PRJNAME,m_PRJNAME,m_PRJNAME,m_PRJNAME
				);
			p += wsprintfA(p, STR_VC6DSP2,
				m_PRJNAME,m_PRJNAME,m_PRJNAME
				);
			p += wsprintfA(p, STR_VC6DSP3,
				m_PRJNAME,m_PRJNAME
				);
			p += wsprintfA(p, STR_VC6DSP4,
				m_PRJNAME,m_PRJNAME
				);
			p += wsprintfA(p, STR_VC6DSP5,
				m_PRJNAME,m_PRJNAME
				);
			p += wsprintfA(p, STR_VC6DSP6,
				m_PRJNAME,m_PRJNAME,m_PRJNAME
				);
			p += wsprintfA(p, STR_VC6DSP7
				);

	        CopyMemory(strPath,CAheadLib::m_tzPROJECTNAME,MAX_PATH);
			WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),strcat(strPath,".dsp"));
			
		
			// 释放
			 _HeapFree(pszBuffer);	
		
	}
	/////////////////////
 
	TCHAR strDir[MAX_PATH] = {0};
	CopyMemory(strDir,CAheadLib::m_tzPROJECTNAME,MAX_PATH);
	PTSTR Bp = _AStrRChr(strDir, '\\');
	Bp =Bp +1 ;//保留
	*Bp = 0;//这里去除了后缀

	//stdcpp
	{
		pszBuffer=NULL;
		p=NULL;

			pszBuffer=NULL;
			pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
			//_LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
			p=pszBuffer;
			p += wsprintfA(p, STR_VC6STDAFXCPP,
				m_PRJNAME			
				);
		    CopyMemory(strPath,strDir,MAX_PATH);
			WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),strcat(strPath,"StdAfx.cpp"));
	
		
		// 释放
			 _HeapFree(pszBuffer);	
	}
	//stdh
	{
		pszBuffer=NULL;
		p=NULL;
		TCHAR randStr[MAX_PATH]={0};

		/* Seed the random-number generator with current time so that
		·  the numbers will be different every time we run.
		*/
		srand( (unsigned)time( NULL ) );
		int iRand=rand();
		wsprintfA(randStr,"H%X%XH",iRand,iRand);
	
			pszBuffer=NULL;
			pszBuffer = (PSTR) _HeapAlloc(4096*4);//16k
			//_LeaveExIf(pszBuffer == NULL, pszBuffer=NULL);
			p=pszBuffer;
			p += wsprintfA(p, STR_VC6STDAFXH,
				randStr,randStr,randStr			
				);
			 CopyMemory(strPath,strDir,MAX_PATH);
			WriteSzFile(pszBuffer,(DWORD) (p - pszBuffer),strcat(strPath,"StdAfx.h"));
			
			
			// 释放
			 _HeapFree(pszBuffer);	
	}


	//target cpp
	GetOptions();
	Generate(TRUE,TRUE);
	return 1;
}

BOOL WINAPI CAheadLib::GenRc()
{



   //预览时候bWriteCppFile =0
   DWORD i;
   PSTR pszFile;
   DWORD dwSize;
   PSTR pszBuffer;
   PSTR *ppszNames;
   PDWORD pdwNames;
   BOOL bLoadOrigin;
   ALRESULT alResult;
   PWORD pwNamesOrdinals;
   PIMAGE_NT_HEADERS pInh;
   PIMAGE_EXPORT_DIRECTORY pIed;
   
   _Try
   {
	   // 初始化
	   pszFile = NULL;
	   ppszNames = NULL;
	   pszBuffer = NULL;
	   SetDlgItemTextA(CMainDlg::m_hWnd, IDC_Preview, NULL);
	   
	   // 判断参数是否完整
	   _LeaveExIf((m_szOrigin[0] == 0) || (m_tzDllFile[0] == 0) || ( (m_tzDllFile[0] == 0)),
		   alResult = ALResult_InvalidArgs);
	   
	   // 打开 DLL 文件
	   CMapFile mfFile(m_tzDllFile);
	   pszFile = (PSTR) mfFile.m_pvFile;
	   _LeaveExIf(pszFile == NULL, alResult = ALResult_OpenDllFile);
	   if(mfFile.m_dwSize<512) return ALResult_InvalidPeFile;
	   _LeaveExIf(mfFile.m_dwSize < 512, alResult = ALResult_InvalidPeFile);
	   //这里仅仅通过dll size 判断是否是PE有效文件
	   // 判断是否为 PE 文件
	   pInh = (PIMAGE_NT_HEADERS) (pszFile + ((PIMAGE_DOS_HEADER) pszFile)->e_lfanew);
	   if(pInh->Signature != IMAGE_NT_SIGNATURE) return ALResult_InvalidPeFile;
	   _LeaveExIf(pInh->Signature != IMAGE_NT_SIGNATURE, alResult = ALResult_InvalidPeFile);
	   
	   // 定位导出表  从数据目录中
	   dwSize = pInh->OptionalHeader.DataDirectory[0].VirtualAddress;
	   _LeaveExIf(dwSize == 0, alResult = ALResult_NoExportTable);
	   pIed = (PIMAGE_EXPORT_DIRECTORY) (pszFile + RvaToOffset(pInh, dwSize));
	   
	   // 获取 Names 和 Ordinals
	   pdwNames = (PDWORD) (pszFile + RvaToOffset(pInh, pIed->AddressOfNames));
	   pwNamesOrdinals = (PWORD) (pszFile + RvaToOffset(pInh, pIed->AddressOfNameOrdinals));
	   
	   // 分配内存
	   ppszNames = (PSTR *) _HeapAllocZero(sizeof(PSTR) * pIed->NumberOfFunctions);
	   _LeaveExIf(ppszNames == NULL, alResult = ALResult_HeapAlloc);
	   
	   // 获取导出函数名称
	   for (i = 0; i < pIed->NumberOfNames; i++)
	   {
		   ppszNames[pwNamesOrdinals[i]] = pszFile + RvaToOffset(pInh, pdwNames[i]);
	   }
	   
	   // 分配内存
	   pszBuffer = (PSTR) _HeapAlloc(4096 + 1024 * pIed->NumberOfFunctions);
	   _LeaveExIf(pszBuffer == NULL, alResult = ALResult_HeapAlloc);
	   
	   // 生成代码并设置到预览编辑框  关键
	   dwSize = GenerateMemHookCode(pszBuffer, ppszNames, pIed->Base, pIed->NumberOfFunctions, bLoadOrigin);

	   SetDlgItemTextA(CMainDlg::m_hWnd, IDC_Preview, pszBuffer);
	   
	   // 写入到文件
// 	   if (bWriteCppFile)
// 	   {
// 		   _LeaveExIf(WriteCppFile(pszBuffer, dwSize,bdirCustomer) == FALSE, alResult = ALResult_WriteCppFile);
// 	   }
	   
	   // 执行成功
	   alResult = (bLoadOrigin && m_bCallRedirect) ? ALResult_NotAllRedirect : ALResult_Success;
   }
   _Finally
   {
	   // 释放
	   _ExIf(pszBuffer, _HeapFree(pszBuffer));
	   _ExIf(ppszNames, _HeapFree(ppszNames));
	   
	   return alResult;
	}
return 1;
}   


BOOL WINAPI CAheadLib::GenRcDswApiDbg(const char * cppApiDbgPath)
{
	
	
	if(cppApiDbgPath==NULL) return 0;
	//预览时候bWriteCppFile =0
	DWORD i;
	PSTR pszFile;
	DWORD dwSize;
	PSTR pszBuffer;
	PSTR *ppszNames;
	PDWORD pdwNames;
	BOOL bLoadOrigin;
	ALRESULT alResult;
	PWORD pwNamesOrdinals;
	PIMAGE_NT_HEADERS pInh;
	PIMAGE_EXPORT_DIRECTORY pIed;
	
	_Try
	{
		// 初始化
		pszFile = NULL;
		ppszNames = NULL;
		pszBuffer = NULL;
		SetDlgItemTextA(CMainDlg::m_hWnd, IDC_Preview, NULL);
		
		// 判断参数是否完整
		_LeaveExIf((m_szOrigin[0] == 0) || (m_tzDllFile[0] == 0) || ( (m_tzDllFile[0] == 0)),
			alResult = ALResult_InvalidArgs);
		
		// 打开 DLL 文件
		CMapFile mfFile(m_tzDllFile);
		pszFile = (PSTR) mfFile.m_pvFile;
		_LeaveExIf(pszFile == NULL, alResult = ALResult_OpenDllFile);
		if(mfFile.m_dwSize<512) return ALResult_InvalidPeFile;
		_LeaveExIf(mfFile.m_dwSize < 512, alResult = ALResult_InvalidPeFile);
		//这里仅仅通过dll size 判断是否是PE有效文件
		// 判断是否为 PE 文件
		pInh = (PIMAGE_NT_HEADERS) (pszFile + ((PIMAGE_DOS_HEADER) pszFile)->e_lfanew);
		if(pInh->Signature != IMAGE_NT_SIGNATURE) return ALResult_InvalidPeFile;
		_LeaveExIf(pInh->Signature != IMAGE_NT_SIGNATURE, alResult = ALResult_InvalidPeFile);
		
		// 定位导出表  从数据目录中
		dwSize = pInh->OptionalHeader.DataDirectory[0].VirtualAddress;
		_LeaveExIf(dwSize == 0, alResult = ALResult_NoExportTable);
		pIed = (PIMAGE_EXPORT_DIRECTORY) (pszFile + RvaToOffset(pInh, dwSize));
		
		// 获取 Names 和 Ordinals
		pdwNames = (PDWORD) (pszFile + RvaToOffset(pInh, pIed->AddressOfNames));
		pwNamesOrdinals = (PWORD) (pszFile + RvaToOffset(pInh, pIed->AddressOfNameOrdinals));
		
		// 分配内存
		ppszNames = (PSTR *) _HeapAllocZero(sizeof(PSTR) * pIed->NumberOfFunctions);
		_LeaveExIf(ppszNames == NULL, alResult = ALResult_HeapAlloc);
		
		// 获取导出函数名称
		for (i = 0; i < pIed->NumberOfNames; i++)
		{
			ppszNames[pwNamesOrdinals[i]] = pszFile + RvaToOffset(pInh, pdwNames[i]);
		}
		
		// 分配内存
		pszBuffer = (PSTR) _HeapAlloc(4096 + 1024 * pIed->NumberOfFunctions);
		_LeaveExIf(pszBuffer == NULL, alResult = ALResult_HeapAlloc);
		
		// 生成代码并设置到预览编辑框  关键
		dwSize = GenerateMemHookCPPFullCode(pszBuffer, ppszNames, pIed->Base, pIed->NumberOfFunctions, bLoadOrigin);
		
		SetDlgItemTextA(CMainDlg::m_hWnd, IDC_Preview, pszBuffer);
		
		// 写入到文件

	     WriteSzFile(pszBuffer,dwSize,cppApiDbgPath);
	
		
		// 执行成功
		alResult = (bLoadOrigin && m_bCallRedirect) ? ALResult_NotAllRedirect : ALResult_Success;
	}
	_Finally
	{
		// 释放
		_ExIf(pszBuffer, _HeapFree(pszBuffer));
		_ExIf(ppszNames, _HeapFree(ppszNames));
		
		return alResult;
	}
	return 1;
}   
