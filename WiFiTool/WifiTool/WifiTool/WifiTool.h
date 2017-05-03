// WifiTool.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CWifiToolApp:
// 有关此类的实现，请参阅 WifiTool.cpp
//

bool IsWin7OrLater();  //判断操作系统版本，开启虚拟wifi，至少为Windows 7或Windows Servier 2008 R2

class CWifiToolApp : public CWinApp
{
public:
	CWifiToolApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

};

extern CWifiToolApp theApp;