/////////////////////////////////////////////////////////////////////
// Author : CodeLive
// WeiBo  : http://weibo.com/1655194010
// Email  : dongfa@yeah.net
// QQ     : 5584173
// Date   : 2014.04.21
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdHookApi.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>
#include "Common.h"

static BOOL gUseAPI = TRUE;

static BOOL WINAPI myReadMemory(HANDLE hProcess, LPVOID lpAddress, LPVOID lpBuffer, SIZE_T nSize)
{
    BOOL bRet = FALSE;
    DWORD dwOldProtect = 0;
    bRet = VirtualProtect(lpAddress, nSize, PAGE_READONLY, &dwOldProtect);
    if(gUseAPI)
    {
        DWORD dwRead = 0;
        bRet = ReadProcessMemory(hProcess, lpAddress, lpBuffer, nSize, &dwRead);
    }
    else
    {
        memcpy(lpBuffer, lpAddress, nSize);
    }
    VirtualProtect(lpAddress, nSize, dwOldProtect, &dwOldProtect);
    assert(bRet);
    return bRet;
}

static BOOL WINAPI myWriteMemory(HANDLE hProcess, LPVOID lpAddress, LPVOID lpBuffer, SIZE_T nSize)
{
    BOOL bRet = FALSE;
    DWORD dwOldProtect = 0;
    bRet = VirtualProtect(lpAddress, nSize, PAGE_READWRITE, &dwOldProtect);
    if(gUseAPI)
    {
        DWORD dwWrite = 0;
        bRet = WriteProcessMemory(hProcess, lpAddress, lpBuffer, nSize, &dwWrite);
    }
    else
    {
        memcpy(lpAddress, lpBuffer, nSize);
    }
    VirtualProtect(lpAddress, nSize, dwOldProtect, &dwOldProtect);
    assert(bRet);
    return bRet;
}

// class CAdAutoHookApi
CAdAutoHookApi::CAdAutoHookApi(CAdHookApi *pHookApi, void *pAddr)
{
	m_pHookApi = pHookApi;
	m_pAddr    = pAddr;   //�Զ��庯����ַ

	assert(m_pHookApi != NULL);

	if(m_pHookApi != NULL)
    {
		m_pHookApi->End2(m_pAddr);//ԭ�����뻹ԭ
    }
}

CAdAutoHookApi::~CAdAutoHookApi()
{
	if(m_pHookApi != NULL)
    {
		m_pHookApi->Begin2(m_pAddr);//����Ϊ���д���
    }
}

// class CAdHookApi
CAdHookApi::CAdHookApi()
{
}

CAdHookApi::~CAdHookApi()
{
	EndAll();
}

BOOL CAdHookApi::VerifyAddress(void *pAddr, const BYTE *verifyData, DWORD verifySize)
{
    BOOL isPassed = FALSE;
    if((verifyData != NULL) && (verifySize > 0))
    {
        BYTE *addrData = new BYTE[verifySize];
        if(myReadMemory(GetCurrentProcess(), pAddr, addrData, verifySize))
        {
            if(memcmp(addrData, verifyData, verifySize) == 0)
            {
                isPassed = TRUE;
            }
        }
        delete []addrData;
    }
    else
    {
        isPassed = TRUE;
    }
    return isPassed;
}
//���ʹ��PatchCode������
BOOL CAdHookApi::PatchCode(void *pAddr,  BYTE *pCode, DWORD dwCode, 
                            BYTE *verifyData, DWORD verifySize)
{
    if(!VerifyAddress(pAddr, verifyData, verifySize))
    {
        return FALSE;
    }
	//dwCode ��size 
    BOOL bRet = myWriteMemory(GetCurrentProcess(), pAddr, pCode, dwCode);
    return bRet;
}
//	gHooks.Add(_T("User32.DLL"), "MessageBoxA", My_MessageBox);
HANDLE CAdHookApi::Add(LPCTSTR lpszModule, LPCSTR lpcFuncName, void *pNewAddr, DWORD dwData)
{
	//ֻload һ��dll��ԭ��
    HMODULE hModule = LoadLibrary(lpszModule);
    if(hModule == NULL)
    {
			OutputDebugString("apidebug lib=null");
        return NULL;
    }
     //�Ӷ�̬����ȡ���Ϻ����ĵ�ַ
    void *pOldAddr = (void *)GetProcAddress(hModule, lpcFuncName);
    if(pOldAddr == NULL)
    {
		OutputDebugString("apidebug pold=null");
        return NULL;
    }
	//�����ǵ������÷�
	//const BYTE verifyData[] = { 0x55, 0x8B, 0xEC, 0x81, 0xEC, 0x2C, 0x01, 0x00, 0x00 };
    //void *addr = (void *)0x004026B0;
    // if(gHooks.Add(addr, my_sub_4026B0, verifyData, sizeof(verifyData), 0) != NULL)
    return Add(pOldAddr, pNewAddr, NULL, 0, dwData);
}

HANDLE CAdHookApi::Add(void *pOldAddr, void *pNewAddr, const BYTE *verifyData, DWORD verifySize, DWORD dwData)
{
	BOOL bRet = FALSE;
	HookMap *pHook = new HookMap;
	do
	{
		ZeroMemory(pHook, sizeof(HookMap));
		
		pHook->hProcess = GetCurrentProcess();//��ǰ���̵�Handle

		pHook->pOldAddr = pOldAddr;//ԭ��������ַ  ��һ��memory�ռ���
		if(pHook->pOldAddr == NULL)
        {
			break ;
        }

		DWORD dwRead = 8;
        if((verifyData != NULL) && (verifySize > 0) && (verifySize > dwRead))
        {
            dwRead = verifySize;//��Ӧ��С
        }
        BYTE *addrData = new BYTE[dwRead];//�����̱����غ��.text�����  ��dll�Ǹ����ڽ��̿ռ����
		if(!myReadMemory(pHook->hProcess, pHook->pOldAddr, addrData, dwRead))//��8��Bytes
        {
            delete []addrData;
			break ;
        }
		//���У������ �ǲ�����Ҫ����
        if((verifyData != NULL) && (verifySize > 0) && (memcmp(addrData, verifyData, verifySize) != 0))
        {
            delete []addrData;
            break ;
        }


        memcpy(pHook->chOldCode, addrData, 8);
        delete []addrData;

		DWORD dwTemp = (DWORD)pNewAddr;
		pHook->pNewAddr = pNewAddr;

		//��תָ������ ASM CODE
		// mov eax, pNewAddr
		// jmp eax
		pHook->chNewCode[0] = 0xB8;
		memcpy(pHook->chNewCode + 1, &dwTemp, sizeof(DWORD));
		pHook->chNewCode[5] = 0xFF;
		pHook->chNewCode[6] = 0xE0;			

		pHook->bHooked = FALSE;

		pHook->dwData = dwData;

		m_obHooks.push_back(pHook);

		bRet = TRUE;
	}while(0);//���д��������

	if(!bRet)
	{
		delete pHook;
		pHook = NULL;
	}

	return (HANDLE)pHook;
}

BOOL CAdHookApi::Remove(HANDLE hHook)
{
	BOOL bRet = FALSE;
	HookMap *pHook = (HookMap *)hHook;
	for(int i = 0; i < (int)m_obHooks.size(); i ++)
	{
		HookMap *pTemp = m_obHooks[i];
		if(pTemp == pHook)
		{
			End((HANDLE)pTemp);//�ָ�ԭ����  �� call�� jump
			delete pHook;
			m_obHooks.erase(m_obHooks.begin() + i);
			bRet = TRUE;
			break ;
		}
	}

	return bRet;
}

BOOL CAdHookApi::Begin(HANDLE hHook)
{
	if(!HasHook(hHook))
    {
		return FALSE;
    }
	HookMap *pHook = (HookMap *)hHook;
	if(pHook->bHooked)
    {
		return TRUE;
    }
	DWORD dwWrite = 8;    
    BOOL bRet = myWriteMemory(pHook->hProcess, pHook->pOldAddr, pHook->chNewCode, dwWrite);
	if(bRet)
    {
		pHook->bHooked = TRUE;
    }
	return bRet;
}

BOOL CAdHookApi::End(HANDLE hHook)
{
	if(!HasHook(hHook))
    {
		return FALSE;
    }
	HookMap *pHook = (HookMap *)hHook;
	if(!pHook->bHooked)
    {
		return FALSE;
    }
	DWORD dwWrite = 8;
	//���ｫԭ���Ĵ��뻹ԭ
    BOOL bRet = myWriteMemory(pHook->hProcess, pHook->pOldAddr, pHook->chOldCode, dwWrite);
	if(bRet)
    {
		pHook->bHooked = FALSE;
    }
	return bRet;
}

BOOL CAdHookApi::Begin2(void *pNewAddr)
{
	HookMap *pHook = FromNewAddr(pNewAddr);
	if(pHook == NULL)
    {
		return FALSE;
    }

	return Begin((HANDLE)pHook);
}

BOOL CAdHookApi::End2(void *pNewAddr)
{
	HookMap *pHook = FromNewAddr(pNewAddr);
	if(pHook == NULL)
    {
		return FALSE;
    }
	
	return End((HANDLE)pHook);
}

void *CAdHookApi::OldAddr2NewAddr(void *pOldAddr)
{
	HookMap *pHook = FromOldAddr(pOldAddr);
	if(pHook == NULL)
    {
		return NULL;
    }
	
	return pHook->pNewAddr;
}

void *CAdHookApi::NewAddr2OldAddr(void *pNewAddr)
{
	HookMap *pHook = FromNewAddr(pNewAddr);
	if(pHook == NULL)
    {
		return NULL;
    }
	
	return pHook->pOldAddr;
}

CAdHookApi::HookMap *CAdHookApi::FromNewAddr(void *pNewAddr)
{
	HookMap *pHook = NULL;
	for(int i = 0; i < (int)m_obHooks.size(); i ++)
	{
		HookMap *pTemp = m_obHooks[i];
		if(pTemp->pNewAddr == pNewAddr)
		{
			pHook = pTemp;
			break ;
		}
	}
	
	return pHook;
}

CAdHookApi::HookMap *CAdHookApi::FromOldAddr(void *pOldAddr)
{
	HookMap *pHook = NULL;
	for(int i = 0; i < (int)m_obHooks.size(); i ++)
	{
		HookMap *pTemp = m_obHooks[i];
		if(pTemp->pOldAddr == pOldAddr)
		{
			pHook = pTemp;
			break ;
		}
	}
	
	return pHook;
}

BOOL CAdHookApi::HasHook(HANDLE hHook)
{
	BOOL bRet = FALSE;
	HookMap *pHook = (HookMap *)hHook;
	for(int i = 0; i < (int)m_obHooks.size(); i ++)
	{
		HookMap *pTemp = m_obHooks[i];
		if(pTemp == pHook)
		{
			bRet = TRUE;
			break ;
		}
	}
	
	return bRet;
}

int CAdHookApi::BeginAll()
{
	int nRet = 0;
	for(int i = 0; i < (int)m_obHooks.size(); i ++)
	{
		HookMap *pTemp = m_obHooks[i];
		BOOL bRet = Begin((HANDLE)pTemp);
		if(bRet)
        {
			nRet ++;
        }
	}

	return nRet;
}

int CAdHookApi::EndAll()
{
	int nRet = 0;
	for(int i = 0; i < (int)m_obHooks.size(); i ++)
	{
		HookMap *pTemp = m_obHooks[i];
		BOOL bRet = End((HANDLE)pTemp);
		delete pTemp;
		if(bRet)
        {
			nRet ++;
        }
	}
	m_obHooks.clear();

	return nRet;
}

int CAdHookApi::GetCount()
{
	return (int)m_obHooks.size();
}
