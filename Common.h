/////////////////////////////////////////////////////////////////////
// Author : CodeLive
// WeiBo  : http://weibo.com/1655194010
// Email  : dongfa@yeah.net
// QQ     : 5584173
// Date   : 2014.04.24
/////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
using namespace std;
#define BUFFER_SIZE  1024
//是否使用全局 文件  
#define ISUSEFILE   0
extern bool gEnableLogOutput;
string  formatString(const char *format, ...);
string  toHexString( char *data, int size);
int     logOutput(const char *text);
int     logOutput(const string &text);


#if ISUSEFILE
int     appendDataToFile(const char *fileName, void *buffer, int size);
int     writeDataToFile(const char *fileName, void *buffer, int size);
int     readDataFromFile(const char *fileName, void *buffer, int size);
string  getLogString();
string  getFileName(const string &filePath);
wstring getAppNameW();
wstring getAppPath();
string  getModuleNameA(HMODULE hModule);
string  getAppNameA();
string  getAppPathA();
#endif