

//#ifdef APIDEBUGGER_EXPORTS
//#define APIDEBUGGER __declspec(dllexport)
//#else
//#define APIDEBUGGER __declspec(dllimport)
//#endif

#define APIDEBUGGER __declspec(dllexport)
extern "C" APIDEBUGGER const char * ApiDebugger();
void ApiDebugferShutdown();