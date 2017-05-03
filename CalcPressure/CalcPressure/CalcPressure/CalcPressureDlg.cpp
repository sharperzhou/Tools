// CalcPressureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CalcPressure.h"
#include "CalcPressureDlg.h"
#include "Calculation.h"
#include <math.h>
#include "StringAndDouble.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCalcPressureDlg 对话框




CCalcPressureDlg::CCalcPressureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcPressureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalcPressureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalcPressureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_GAGE, &CCalcPressureDlg::OnEnChangeGage)
	ON_EN_CHANGE(IDC_VACUUM, &CCalcPressureDlg::OnEnChangeVacuum)
	ON_EN_CHANGE(IDC_MMHG, &CCalcPressureDlg::OnEnChangeMmhg)
	ON_EN_CHANGE(IDC_ABS, &CCalcPressureDlg::OnEnChangeAbs)
	ON_BN_CLICKED(IDC_CHECK1, &CCalcPressureDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDOK, &CCalcPressureDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCalcPressureDlg 消息处理程序

BOOL CCalcPressureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	((CButton *)GetDlgItem(IDC_CHECK))->SetCheck(BST_CHECKED);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCalcPressureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCalcPressureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCalcPressureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

Calculation calcP;
CString str[4];
StringAndDouble::FormatType ft = {StringAndDouble::FixedLength, 9, 4};

void CCalcPressureDlg::OnEnChangeGage()
{
	if (GetDlgItem(IDC_GAGE) != GetFocus()) return;
	double input = 0;
	GetDlgItemTextW(IDC_GAGE, str[0]);
	bool re = StringAndDouble::StrtoD(str[0], input);
	if (re)
	{
		calcP.calc(MPag, input);
		if (fabs(calcP.value[1] - NA) < EPS) str[1] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[1], str[1], ft);
		if (fabs(calcP.value[2] - NA) < EPS) str[2] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[2], str[2], ft);
		if (fabs(calcP.value[3] - NA) < EPS) str[3] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[3], str[3], ft);
		SetDlgItemTextW(IDC_VACUUM, str[1]);
		SetDlgItemTextW(IDC_MMHG, str[2]);
		SetDlgItemTextW(IDC_ABS, str[3]);
	}
	else
	{
		SetDlgItemTextW(IDC_VACUUM, _T(""));
		SetDlgItemTextW(IDC_MMHG,  _T(""));
		SetDlgItemTextW(IDC_ABS,  _T(""));
	}
}

void CCalcPressureDlg::OnEnChangeVacuum()
{
	if (GetDlgItem(IDC_VACUUM) != GetFocus()) return;
	double input = 0;
	GetDlgItemTextW(IDC_VACUUM, str[1]);
	bool re = StringAndDouble::StrtoD(str[1], input);
	if (re)
	{
		calcP.calc(kPag, input);
		if (fabs(calcP.value[0] - NA) < EPS) str[0] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[0], str[0], ft);
		if (fabs(calcP.value[2] - NA) < EPS) str[2] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[2], str[2], ft);
		if (fabs(calcP.value[3] - NA) < EPS) str[3] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[3],str[3], ft);
		SetDlgItemTextW(IDC_GAGE, str[0]);
		SetDlgItemTextW(IDC_MMHG, str[2]);
		SetDlgItemTextW(IDC_ABS, str[3]);
	}
	else
	{
		SetDlgItemTextW(IDC_GAGE, _T(""));
		SetDlgItemTextW(IDC_MMHG, _T(""));
		SetDlgItemTextW(IDC_ABS, _T(""));
	}
}

void CCalcPressureDlg::OnEnChangeMmhg()
{
	if (GetDlgItem(IDC_MMHG) != GetFocus()) return;
	double input = 0;
	GetDlgItemTextW(IDC_MMHG, str[2]);
	bool re = StringAndDouble::StrtoD(str[2], input);
	if (re)
	{
		calcP.calc(mmHg, input);
		if (fabs(calcP.value[0] - NA) < EPS) str[0] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[0], str[0], ft);
		if (fabs(calcP.value[1] - NA) < EPS) str[1] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[1], str[1], ft);
		if (fabs(calcP.value[3] - NA) < EPS) str[3] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[3], str[3], ft);
		SetDlgItemTextW(IDC_GAGE, str[0]);
		SetDlgItemTextW(IDC_VACUUM, str[1]);
		SetDlgItemTextW(IDC_ABS, str[3]);
	}
	else
	{
		SetDlgItemTextW(IDC_GAGE, _T(""));
		SetDlgItemTextW(IDC_VACUUM, _T(""));
		SetDlgItemTextW(IDC_ABS, _T(""));
	}
}

void CCalcPressureDlg::OnEnChangeAbs()
{
	if (GetDlgItem(IDC_ABS) != GetFocus()) return;
	double input = 0;
	GetDlgItemTextW(IDC_ABS, str[3]);
	bool re = StringAndDouble::StrtoD(str[3], input);
	if (re)
	{
		calcP.calc(kPa, input);
		if (fabs(calcP.value[0] - NA) < EPS) str[0] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[0], str[0], ft);
		if (fabs(calcP.value[1] - NA) < EPS) str[1] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[1], str[1], ft);
		if (fabs(calcP.value[2] - NA) < EPS) str[2] = _T("超出范围");
		else StringAndDouble::DtoFormatStr(calcP.value[2], str[2], ft);
		SetDlgItemTextW(IDC_GAGE, str[0]);
		SetDlgItemTextW(IDC_VACUUM, str[1]);
		SetDlgItemTextW(IDC_MMHG, str[2]);
	}
	else
	{
		SetDlgItemTextW(IDC_GAGE, _T(""));
		SetDlgItemTextW(IDC_VACUUM, _T(""));
		SetDlgItemTextW(IDC_MMHG, _T(""));
	}
}

void CCalcPressureDlg::OnBnClickedCheck()
{
	if (((CButton *)GetDlgItem(IDC_CHECK))->GetCheck() == BST_CHECKED)
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CCalcPressureDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	GetNextDlgTabItem(GetFocus())->SetFocus();
}

void CCalcPressureDlg::WinHelp(DWORD dwData, UINT nCmd)
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::WinHelp(dwData, nCmd);     //注释后按F1按钮不显示系统帮助
}

BOOL CCalcPressureDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//屏蔽按下ESC键关闭对话框
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
