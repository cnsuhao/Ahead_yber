


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����
#pragma once
#include "Define.h"
#include "Macro.h"
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CIni ��
class CIni
{
private:
	// INI �ļ���
	static TCHAR m_tzFileName[MAX_PATH];

public:
	// ���� INI �ļ���
	inline static VOID WINAPI SetIniName()
	{
		GetModuleFileName(NULL, m_tzFileName, MAX_PATH);
		lstrcpy(_StrEnd(m_tzFileName) - _LengthOf(EXT_Ini), EXT_Ini);
	}

	// ���� INI �ļ���
	inline static VOID WINAPI SetIniName(LPCSTR ptzFileName)
	{
		_StrToStr(m_tzFileName, ptzFileName);
	}

	// ��ȡ����
	inline static UINT WINAPI GetInt(LPCSTR ptzKeyName, INT iDefault = 0, LPCSTR ptzSectionName = INI_Main)
	{
		return GetPrivateProfileInt(ptzSectionName, ptzKeyName, iDefault, m_tzFileName);
	}

	// ��������
	inline static BOOL WINAPI SetInt(LPCSTR ptzKeyName, INT iValue = 0, LPCSTR ptzSectionName = INI_Main)
	{
		TCHAR tzString[16];

		wsprintf(tzString, TEXT("%d"), iValue);
		return WritePrivateProfileString(ptzSectionName, ptzKeyName, tzString, m_tzFileName);
	}

	// ��ȡ�ַ���
	inline static DWORD WINAPI GetString(LPCSTR ptzKeyName, PTSTR ptzReturnedString,
		DWORD dwSize = MAX_PATH, LPCSTR ptzDefault = NULL, LPCSTR ptzSectionName = INI_Main)
	{
		return GetPrivateProfileString(ptzSectionName, ptzKeyName, ptzDefault, ptzReturnedString,
			dwSize, m_tzFileName);
	}

	// �����ַ���
	inline static BOOL WINAPI SetString(LPCSTR ptzKeyName, LPCSTR ptzString = NULL, LPCSTR ptzSectionName = INI_Main)
	{
		return WritePrivateProfileString(ptzSectionName, ptzKeyName, ptzString, m_tzFileName);
	}

	// ��ȡ�ṹ
	inline static BOOL WINAPI GetStruct(LPCSTR ptzKeyName, PVOID pvStruct, UINT uSize, LPCSTR ptzSectionName = INI_Main)
	{
		return GetPrivateProfileStruct(ptzSectionName, ptzKeyName, pvStruct, uSize, m_tzFileName);
	}

	// ���ýṹ
	inline static BOOL WINAPI SetStruct(LPCSTR ptzKeyName, PVOID pvStruct, UINT uSize, LPCSTR ptzSectionName = INI_Main)
	{
		return WritePrivateProfileStruct(ptzSectionName, ptzKeyName, pvStruct, uSize, m_tzFileName);
	}

	// ��ȡ��
	inline static DWORD WINAPI GetSection(PTSTR ptzReturnBuffer, DWORD dwSize, LPCSTR ptzSectionName = INI_Main)
	{
		return GetPrivateProfileSection(ptzSectionName, ptzReturnBuffer, dwSize, m_tzFileName);
	}

	// ���ý�
	inline static DWORD WINAPI SetSection(LPCSTR ptzString, LPCSTR ptzSectionName = INI_Main)
	{
		return WritePrivateProfileSection(ptzSectionName, ptzString, m_tzFileName);
	}

	// ��ȡ����
	inline static DWORD WINAPI GetSectionNames(PTSTR ptzReturnBuffer, DWORD dwSize)
	{
		return GetPrivateProfileSectionNames(ptzReturnBuffer, dwSize, m_tzFileName);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
