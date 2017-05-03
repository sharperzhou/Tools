// KillProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KillProcess.h"
#include "KillProcessDlg.h"
#include "Process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKillProcessDlg 对话框




CKillProcessDlg::CKillProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKillProcessDlg::IDD, pParent)
	, m_radio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKillProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RD_PID, m_radio);
}

BEGIN_MESSAGE_MAP(CKillProcessDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_EXEC, &CKillProcessDlg::OnBnClickedBtnExec)
	ON_EN_SETFOCUS(IDC_EDIT_CONTENT, &CKillProcessDlg::OnEnSetfocusEditContent)
END_MESSAGE_MAP()


// CKillProcessDlg 消息处理程序

BOOL CKillProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//设置状态栏
	{
		CRect rect;
		UINT indicator = 2000;
		m_statusbar.CreateEx(this);
		GetClientRect(&rect);
		m_statusbar.SetIndicators(&indicator, 1);
		m_statusbar.SetPaneInfo(0, 2000, SBPS_STRETCH, 0);
		m_statusbar.MoveWindow(rect.left, rect.bottom - 20, rect.Width(), 20);
	}

	//设置文本框字体
	{
		m_font.CreatePointFont(140, _T("Arial"));
		GetDlgItem(IDC_EDIT_CONTENT)->SetFont(&m_font);
	}

	//设置文本框焦点
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_CONTENT);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
	}



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKillProcessDlg::OnPaint()
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
HCURSOR CKillProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKillProcessDlg::OnBnClickedBtnExec()
{
	// TODO: 在此添加控件通知处理程序代码

	CProcess proc;
	UINT killedCount = 0;
	TCHAR str[MAX_PATH] = {0};
	GetDlgItemText(IDC_EDIT_CONTENT, str, MAX_PATH - 1);

	UpdateData(TRUE);
	switch (m_radio)
	{
	case 0:
		killedCount = proc.Kill(_tstoi(str));
		break;
	case 1:
		killedCount = proc.Kill(str);
		break;
	default:
		break;
	}
	
	memset(str, 0, sizeof(str));
	_stprintf(str, _T("已杀掉进程个数：%d"), killedCount);
	m_statusbar.SetPaneText(0, str);
	GetDlgItem(IDC_BTN_EXEC)->EnableWindow(FALSE);
}

BOOL CKillProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	bool bRet = FALSE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		bRet = TRUE;
	else
		bRet = CDialog::PreTranslateMessage(pMsg);

	return bRet;
}

void CKillProcessDlg::OnEnSetfocusEditContent()
{
	// TODO: 在此添加控件通知处理程序代码
	m_statusbar.SetPaneText(0, _T(""));
	GetDlgItem(IDC_BTN_EXEC)->EnableWindow(TRUE);
}
