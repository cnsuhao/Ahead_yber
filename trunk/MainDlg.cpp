


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ԥ����

#include "StdAfx.h"
#include "Commdlg.h"
#include "Main.h"
#include "MainDlg.h"
#include "shellapi.h"
#include <DIRECT.H>
#include <string>
using namespace std;
#pragma  comment(lib,"shell32.lib")
#pragma warning(disable:4244)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainDlg �ྲ̬��Ա����
HWND CMainDlg::m_hWnd = NULL;			// �Ի�����
BOOL CMainDlg::m_bShowOptions = TRUE;	// �Ƿ���ʾѡ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// �϶��ļ�
VOID WINAPI CMainDlg::OnDropFiles(WPARAM wParam)
{
	TCHAR tzFileName[MAX_PATH];
	DragQueryFile((HDROP) wParam, 0, tzFileName, MAX_PATH);
	DragFinish((HDROP) wParam);
	SetDlgItemText(m_hWnd, IDC_DllFile, tzFileName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ص�����
INT_PTR CALLBACK CMainDlg::MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		// �Ի��򱻴���
		m_hWnd = hWnd;
		OnInitDialog(lParam);
		break;

	case WM_COMMAND:
		// �����
		OnCommand(wParam);
		break;

	case WM_SYSCOMMAND:
		// ϵͳ�����
		OnSysCommand(wParam);
		break;

	case WM_SIZE:
		// ��С�ı�
		OnSize(wParam, lParam);
		break;

	case WM_DROPFILES:
		// �϶��ļ�
		OnDropFiles(wParam);
		break;

	case WM_CLOSE:
		// �Ի�������
		OnClose();
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʼ��
VOID WINAPI CMainDlg::OnInitDialog(LPARAM lParam)
{
	UINT uTab = 16;

	// ����ͼ��
//	SetClassLongPtr(m_hWnd, GCL_HICON, (LONG_PTR) LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main)));

	// ����ѡ������
	CAheadLib::LoadOptions();

	// ����ѡ��
	if (CIni::GetInt(INI_ShowOptions, TRUE) == FALSE)
	{
		OnOptions();
	}

	// ���ñ༭��
	SendDlgItemMessage(m_hWnd, IDC_DllFile, EM_LIMITTEXT, MAX_PATH, 0);
	SendDlgItemMessage(m_hWnd, IDC_CppFile, EM_LIMITTEXT, MAX_PATH, 0);
	SendDlgItemMessage(m_hWnd, IDC_OriginDll, EM_LIMITTEXT, MAX_PATH, 0);
	SendDlgItemMessage(m_hWnd, IDC_Preview, EM_SETTABSTOPS, 1, (LPARAM) &uTab);
	SetDlgItemText(m_hWnd, IDC_DllFile, (PTSTR) lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����
VOID WINAPI CMainDlg::OnCommand(WPARAM wParam)
{
	PSTR p;
	ALRESULT alResult;

	switch (LOWORD(wParam))
	{
	case IDOK:
		// �����ļ�
		alResult = CAheadLib::Generate(TRUE);
		MsgBox(CAheadLib::GetResult(alResult), (alResult <= ALResult_NotAllRedirect) ? MB_ICONINFORMATION : MB_ICONSTOP);
		break;

	case IDC_BrowseDll:
		OnBrowse(TRUE);
		break;

	case IDC_BrowseCpp:
		OnBrowse(FALSE);
		break;

	case IDC_Options:
		OnOptions();
		break;

	case IDC_About:
	//	CAboutDlg::Show(m_hWnd);
		OnGenDSW();
		break;
	case IDC_TESTBtn1:
         CAheadLib::GetOptions();
         OnGenRc();
		break;
	case IDC_TESTBtnDSW:
		CAheadLib::GetOptions();
         OnGenApiDbgDsw();
		break;
	case IDCANCEL:
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;

	case IDC_DllFile:
		if (HIWORD(wParam) == EN_CHANGE)
		{
			// �Զ���������ļ�����
			if (GetDlgItemText(m_hWnd, IDC_DllFile, CAheadLib::m_tzDllFile, MAX_PATH))
			{
                //dd
			    DWORD dwSize = 0;
			    HANDLE hFile = _CreateFileForRead(CAheadLib::m_tzDllFile);
				if (hFile == INVALID_HANDLE_VALUE)  break;
	             dwSize = GetFileSize(hFile, NULL);// ��ȡ�ļ���С
                if(dwSize<512) 
				{	
					SetDlgItemText(m_hWnd, IDC_DllFile, "");
					CloseHandle(hFile);
					break;
				}
				WORD dwSignature=0; 
				DWORD nBytesRead=2;
                ReadFile(hFile, &dwSignature, nBytesRead, &nBytesRead, NULL) ;
	
				 if(dwSignature!= 0x5A4D)
				 {
					 	SetDlgItemText(m_hWnd, IDC_DllFile, "");
					 	CloseHandle(hFile);
						break;
				 }

				CloseHandle(hFile);
				//end dd

				TCHAR tzTemp[MAX_PATH];
				lstrcpy(tzTemp, CAheadLib::m_tzDllFile);
				///////bbb
				//[12104] E:\SVN7\pojie2\Ahead\LSCDLL_3L8_V22
				lstrcpy(CAheadLib::m_tzPROJECTNAME, CAheadLib::m_tzDllFile);
				PTSTR Ap = _StrRChr(CAheadLib::m_tzPROJECTNAME, '.');
				_ExIf(Ap, *Ap = 0);
				//apidbg dsw
				lstrcpy(CAheadLib::m_tzAPIDbgDSWPATH,CAheadLib::m_tzPROJECTNAME);
				StrCat(CAheadLib::m_tzAPIDbgDSWPATH,"APIDbg");
				//
				PTSTR Bp = _AStrRChr(CAheadLib::m_tzPROJECTNAME, '\\');
				lstrcpyA(CAheadLib::m_PRJNAME, Bp ? Bp + 1 : CAheadLib::m_tzPROJECTNAME);
				Bp = _AStrRChr(CAheadLib::m_PRJNAME, '.');
				_ExIf(Bp, *Bp = 0);//����ȥ���˺�׺
			//	[8264] LSCDLL_3L8_V22
			
				//���½�Ŀ¼
				//[12808] E:\SVN7\pojie2\Ahead\LSCDLL_3L8_V22\LSCDLL_3L8_V22
				StrCat(CAheadLib::m_tzPROJECTNAME,"\\");
				StrCat(CAheadLib::m_tzPROJECTNAME,CAheadLib::m_PRJNAME);
		      
				//////////////
			

				//////////
				PTSTR p = _StrRChr(tzTemp, '.');
				_ExIf(p == NULL, p = _StrEnd(tzTemp));
				lstrcpy(p, EXT_Cpp);
				SetDlgItemText(m_hWnd, IDC_CppFile, tzTemp);
				if (CAheadLib::m_bSystemPath)
				{
					*p = 0;
				}
				else
				{
					lstrcpy(p, TEXT("Org.dll"));
				}
				p = _StrRChr(tzTemp, '\\');
				SetDlgItemText(m_hWnd, IDC_OriginDll, p ? p + 1 : tzTemp);
			}
			else
			{
				SetDlgItemText(m_hWnd, IDC_CppFile, NULL);
				SetDlgItemText(m_hWnd, IDC_OriginDll, NULL);
			}
		}
		break;

	case IDC_CppFile:
	case IDC_OriginDll:
		if (HIWORD(wParam) == EN_CHANGE)
		{
			// ��ȡԭʼ DLL ���ƣ����ð�ť״̬
			GetDlgItemText(m_hWnd, IDC_CppFile, CAheadLib::m_tzCppFile, MAX_PATH);
			GetDlgItemTextA(m_hWnd, IDC_OriginDll, CAheadLib::m_szOriginDll, MAX_PATH);
			p = _AStrRChr(CAheadLib::m_szOriginDll, '\\');
			lstrcpyA(CAheadLib::m_szOrigin, p ? p + 1 : CAheadLib::m_szOriginDll);
			p = _AStrRChr(CAheadLib::m_szOrigin, '.');
	 
			//_ExIf(p, *p = 0);//����ȥ���˺�׺
			EnableWindow(GetDlgItem(m_hWnd, IDOK), CAheadLib::m_tzCppFile[0] && CAheadLib::m_szOrigin[0]);
			if ((LOWORD(wParam) == IDC_OriginDll) && m_bShowOptions)
			{
				CAheadLib::Generate();
			}
		}
		break;

	case IDC_CallRedirect:
	case IDC_CallInTime:
	case IDC_LoadInEntry:
	case IDC_LoadIfNeed:
	case IDC_JumpToOrigin:
	case IDC_CallOrigin:
	case IDC_MultiThread:
	case IDC_SystemPath:
	case IDC_GenerateHook:
		CAheadLib::GetOptions();
		if (m_bShowOptions)
		{
			CAheadLib::Generate();
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ի���ϵͳ������Ϣ
VOID WINAPI CMainDlg::OnSysCommand(WPARAM wParam)
{
	static BOOL s_bRestoreOptions = FALSE;

	if (wParam == SC_MAXIMIZE)
	{
		if (m_bShowOptions == FALSE)
		{
			m_bShowOptions = TRUE;
			s_bRestoreOptions = TRUE;
			CAheadLib::Generate();
		}
	}
	else if (wParam == SC_RESTORE)
	{
		if (s_bRestoreOptions)
		{
			m_bShowOptions = FALSE;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��С�ı�
VOID WINAPI CMainDlg::OnSize(WPARAM wParam, LPARAM lParam)
{
	HWND hCtrl;
	RECT rtRect;

	if (wParam != SIZE_MINIMIZED)
	{
		hCtrl = GetDlgItem(m_hWnd, IDC_Preview);
		GetWindowRect(hCtrl, &rtRect);
		MapWindowPoints(NULL, m_hWnd, (PPOINT) &rtRect, 2);
		rtRect.right = LOWORD(lParam) - 10;
		rtRect.bottom = HIWORD(lParam) - 10;
		MoveWindow(hCtrl, rtRect.left, rtRect.top, _RectWidth(rtRect), _RectHeight(rtRect), TRUE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ի���������Ϣ
VOID WINAPI CMainDlg::OnClose()
{
	//WINDOWPLACEMENT wpWindow;

	// ����Ի���λ����Ϣ
	//wpWindow.length = sizeof(WINDOWPLACEMENT);
	//GetWindowPlacement(m_hWnd, &wpWindow);
	//CIni::SetInt(INI_WindowLeft, wpWindow.rcNormalPosition.left);
	//CIni::SetInt(INI_WindowTop, wpWindow.rcNormalPosition.top);
	//CIni::SetInt(INI_WindowWidth, _RectWidth(wpWindow.rcNormalPosition));
	//CIni::SetInt(INI_WindowHeight, _RectHeight(wpWindow.rcNormalPosition));
	//CIni::SetInt(INI_WindowState, wpWindow.showCmd);

	// ����ѡ��
	CAheadLib::SaveOptions();
	CIni::SetInt(INI_ShowOptions, m_bShowOptions);

	// �رնԻ���
	EndDialog(m_hWnd, 0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//����DSW
VOID WINAPI CMainDlg:: OnGenDSW()
{
    //	_LeaveExIf(WriteCppFile(pszBuffer, dwSize) == FALSE, alResult = ALResult_WriteCppFile);
		if(strlen(CAheadLib::m_PRJNAME)<1) return;
		TCHAR strDir[MAX_PATH] = {0};
		CopyMemory(strDir,CAheadLib::m_tzPROJECTNAME,MAX_PATH);

		PTSTR Bp = _AStrRChr(strDir, '\\');
		if(Bp==NULL) MsgBox(strDir);
		_ExIf(Bp, *Bp = 0);//����ȥ���˺�׺

		_mkdir(strDir);

		CAheadLib::GenDSW();
}
VOID WINAPI CMainDlg::OnGenRc()
{
     	CAheadLib::GenRc();
}	
//��Դд��
BOOL WINAPI CMainDlg::RCSouceWrite(DWORD rcID,const char * path)
{	
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(rcID), TEXT("APIDBG"));
	if (0 == hRsrc)
		return 0 ;

	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
		return 0 ;

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (0 == hGlobal)	
		return 0 ;

	LPVOID pBuffer = LockResource(hGlobal);
	if (0 == pBuffer)
		return 0 ;

	CAheadLib::WriteSzFile((PCSTR)pBuffer,dwSize,path);
	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	return 1;
}
VOID WINAPI CMainDlg:: OnGenApiDbgDsw()
{
	if(strlen(CAheadLib::m_PRJNAME)<1) return;
	TCHAR strDir[MAX_PATH] = {0};
	CopyMemory(strDir,CAheadLib::m_tzAPIDbgDSWPATH,MAX_PATH);  //ĩβ���� б��
	_mkdir(strDir);

 
	//end ��Դд��
	/*
	AdHookApi.cpp	
	AdHookApi.h	    
	ApiDebugger.cpp	
	ApiDebugger.h	
	boAPIDebug.cpp	
	boAPIDebug.dsp	
	boAPIDebug.dsw	
	Common.cpp	    
	Common.h	  
	StdAfx.cpp	  
    StdAfx.h
	readme.txt
	*/
	/*
	"APIDBG"
	AdHookApi_CPP
	IDR_AdHookApi_H
	IDR_ApiDebugger1_H
	IDR_ApiDebugger_H
	IDR_boAPIDebug_DSP
	IDR_boAPIDebug_DSW
	IDR_Common_CPP
         IDR_Common_H
	*/
	string   strPath=CAheadLib::m_tzAPIDbgDSWPATH;
	strPath += "\\";
	RCSouceWrite(AdHookApi_CPP,string(strPath+"AdHookApi.cpp").c_str()); 
	RCSouceWrite(IDR_AdHookApi_H,string(strPath+"AdHookApi.h").c_str()); 
	RCSouceWrite(IDR_Common_CPP,string(strPath+"Common.cpp").c_str()); 
	RCSouceWrite(IDR_Common_H,string(strPath+"Common.h").c_str()); 
	RCSouceWrite(IDR_boAPIDebug_DSW,string(strPath+"boAPIDebug.dsw").c_str()); 
	RCSouceWrite(IDR_boAPIDebug_DSP,string(strPath+"boAPIDebug.dsp").c_str()); 
	RCSouceWrite(IDR_ApiDebugger1_H,string(strPath+"ApiDebugger.h").c_str()); 
	char buf[10]={"hello"};
	CAheadLib::WriteSzFile((PCSTR)buf,5,string(strPath+"ReadMe.txt").c_str());
	CAheadLib::WriteApiDebugger_CPP();
	//�����ص� д	ApiDebugger.cpp	
	



}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʾ����ѡ��
VOID WINAPI CMainDlg::OnOptions()
{
	if (IsZoomed(m_hWnd))
	{
		ShowWindow(m_hWnd, SW_RESTORE);
		m_bShowOptions = FALSE;
	}
	else
	{
		m_bShowOptions = !m_bShowOptions;
	}

	if (m_bShowOptions)
	{
		CAheadLib::Generate();
		CenterWindow(714, 486);
	}
	else
	{
		CenterWindow(334, 162);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ļ�
VOID WINAPI CMainDlg::OnBrowse(BOOL bDllFile)
{
	OPENFILENAME ofnName;
	TCHAR tzFileName[MAX_PATH];

	tzFileName[0] = 0;
	ZeroMemory(&ofnName, sizeof(OPENFILENAME));
	ofnName.lStructSize = sizeof(OPENFILENAME);
	ofnName.hwndOwner = m_hWnd;
	ofnName.lpstrFile = tzFileName;
	ofnName.nMaxFile = MAX_PATH;
	ofnName.lpstrDefExt = bDllFile ? EXT_Dll : EXT_Cpp;
	ofnName.lpstrFilter = bDllFile ? FILTER_Dll FILTER_All: FILTER_Cpp FILTER_All;
	if (bDllFile)
	{
		ofnName.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		_ReturnIf(GetOpenFileName(&ofnName) == FALSE);
	}
	else
	{
		ofnName.Flags = OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
		_ReturnIf(GetSaveFileName(&ofnName) == FALSE);
	}
	SetDlgItemText(m_hWnd, bDllFile ? IDC_DllFile : IDC_CppFile, tzFileName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
