// WifiTool.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WifiTool.h"
#include "WifiToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWifiToolApp

BEGIN_MESSAGE_MAP(CWifiToolApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWifiToolApp ����

CWifiToolApp::CWifiToolApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWifiToolApp ����

CWifiToolApp theApp;

// CWifiToolApp ��ʼ��

BOOL CWifiToolApp::InitInstance()
{
	//�жϲ���ϵͳ�汾����������wifi������ΪWindows 7��Windows Servier 2008 R2

	if(!IsWin7OrLater())
	{
		::MessageBox(NULL, L"��֧��Windows 7��Windows Server 2008 R2���ϰ汾�Ĳ���ϵͳ��", NULL, MB_OK);
		return false;
	}

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��Ӧ�ó����������ź���
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, m_pszExeName);

	// �ź����Ѵ��ڣ�
	// �ź������ڣ����������һ��ʵ������
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// �ر��ź������
		CloseHandle(hSem);

		// Ѱ����ǰʵ����������
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		while (::IsWindow(hWndPrevious))
		{
			// ��鴰���Ƿ���Ԥ��ı��?
			// �У���������Ѱ�ҵ�����
			if (::GetProp(hWndPrevious, m_pszExeName))
			{
				// ����������С������ָ����С
				if (::IsIconic(hWndPrevious))
					::ShowWindow(hWndPrevious, SW_RESTORE);
				// ����������
				::SetForegroundWindow(hWndPrevious);
				// �������ĶԻ��򼤻�
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
				// �˳���ʵ��
				return FALSE;
			}

			// ����Ѱ����һ������
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
		}

		// ǰһʵ���Ѵ��ڣ����Ҳ���������
		// ���ܳ�����
		// �˳���ʵ��
		return FALSE;
	}

	AfxEnableControlContainer();


	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CWifiToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	return FALSE;
}


bool IsWin7OrLater()
{
    // Initialize the OSVERSIONINFOEX structure.
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 6;
    osvi.dwMinorVersion = 1;  //win7��server 2008 R2���ڲ��汾��Ϊ6.1

    // Initialize the condition mask.
    DWORDLONG dwlConditionMask = 0;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

    // Perform the test.
    return VerifyVersionInfo(&osvi, 
                             VER_MAJORVERSION | VER_MINORVERSION,
                             dwlConditionMask);
}
