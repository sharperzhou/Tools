// WifiTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CWifiToolApp:
// �йش����ʵ�֣������ WifiTool.cpp
//

bool IsWin7OrLater();  //�жϲ���ϵͳ�汾����������wifi������ΪWindows 7��Windows Servier 2008 R2

class CWifiToolApp : public CWinApp
{
public:
	CWifiToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CWifiToolApp theApp;