// CalcPressure.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCalcPressureApp:
// �йش����ʵ�֣������ CalcPressure.cpp
//

class CCalcPressureApp : public CWinApp
{
public:
	CCalcPressureApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCalcPressureApp theApp;