/////////////////////////////////////////////////////////////////////
// Author : CodeLive
// WeiBo  : http://weibo.com/1655194010
// Email  : dongfa@yeah.net
// QQ     : 5584173
// Date   : 2004.04.21
/////////////////////////////////////////////////////////////////////

#ifndef __ADHOOKAPI_H__
#define __ADHOOKAPI_H__
#include <windows.h>
#include <tchar.h>
#include <vector>
using namespace std;

// class CAdAutoHookApi
class CAdHookApi;
class CAdAutoHookApi
{
public:
	CAdAutoHookApi(CAdHookApi *pHookApi, void *pAddr);
	virtual ~CAdAutoHookApi();

	private:
	CAdHookApi *m_pHookApi;
	void       *m_pAddr;


};

// class CAdAutoHook
class CAdHookApi  
{
public:
	CAdHookApi();
	virtual ~CAdHookApi();
	
protected:
	struct HookMap
	{
		HANDLE hProcess;   //��ǰ�߳�
		void  *pOldAddr;   //patch �ĺ�����ַ
		void  *pNewAddr;   //�Զ���ĺ�����ַ
		BYTE   chOldCode[8];  //��oldAddr����������ASM
		BYTE   chNewCode[8];  //��������� ��תASM
		BOOL   bHooked;      //�Ƿ��Ѿ����ASM��־
		DWORD  dwData;      //���Ӳ���
	};
public:
	HANDLE Add(LPCTSTR lpszModule, LPCSTR lpcFuncName, void *pNewAddr, DWORD dwData = 0);
	HANDLE Add(void *pOldAddr, void *pNewAddr, const BYTE *verifyData = NULL, DWORD verifySize = 0, DWORD dwData = 0);
	BOOL   Remove(HANDLE hHook);
	BOOL   Begin(HANDLE hHook);
	BOOL   End(HANDLE hHook);
	BOOL   Begin2(void *pNewAddr);
	BOOL   End2(void *pNewAddr);
	int    BeginAll();
	int    EndAll();
	int    GetCount();
	void  *OldAddr2NewAddr(void *pOldAddr);
	void  *NewAddr2OldAddr(void *pNewAddr);

public:
    static BOOL VerifyAddress(void *pAddr, const BYTE *verifyData, DWORD verifySize);

    static BOOL PatchCode(void *pAddr,  BYTE *pCode, DWORD dwCode, 
         BYTE *verifyData = NULL, DWORD verifySize = 0);
	
protected:
	CAdHookApi::HookMap *FromNewAddr(void *pNewAddr);
	CAdHookApi::HookMap *FromOldAddr(void *pOldAddr);
	BOOL HasHook(HANDLE hHook);

protected:
	vector<HookMap *> m_obHooks;
};

#endif // __ADHOOKAPI_H__
