// WifiToolDlg.cpp : 实现文件

#include "stdafx.h"
#include "WifiTool.h"
#include "WifiToolDlg.h"
#include "OperateService.h"
#include "SettingDlg.h"
#include <vector>

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


// CWifiToolDlg 对话框




CWifiToolDlg::CWifiToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWifiToolDlg::IDD, pParent), netShare()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CWifiToolDlg::~CWifiToolDlg()
{
}

void CWifiToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NETWORKLIST, m_networkList);
}

BEGIN_MESSAGE_MAP(CWifiToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_MENU_SETTING, IDM_STOP, &CWifiToolDlg::OnMenuCommand)
	ON_BN_CLICKED(IDC_OPENWIFI, &CWifiToolDlg::OnBnClickedOpenwifi)
	ON_BN_CLICKED(IDC_CLOSEWIFI, &CWifiToolDlg::OnBnClickedClosewifi)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CWifiToolDlg 消息处理程序

BOOL CWifiToolDlg::OnInitDialog()
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
	mainMenu.LoadMenu(IDR_MENU);
	SetMenu(&mainMenu);
	OperateService svc(svcName);
	switch (svc.QuerySerciceStatus())
	{
	case -1:
		SetServMenuStatus(SERV_PENDING);
		break;
	case 0:
		SetServMenuStatus(SERV_STARTED);
		break;
	case 1:
		SetServMenuStatus(SERV_STOPPED);
	default: break;
	}
	
	m_pstatus = (CStatic *)GetDlgItem(IDC_STATUS);

	if (hostedNetwork.QueryStatus())
	{
		GetDlgItem(IDC_OPENWIFI)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLOSEWIFI)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_OPENWIFI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSEWIFI)->EnableWindow(TRUE);
		m_pstatus->SetWindowText(_T("提示：当前WiFi已开启"));
	}

	std::vector<CString> networkList;
	netShare.GetNetworkList(networkList);
	m_networkList.AddString(_T("自动选择"));
	for (int i = 0; i < networkList.size(); i++)
		m_networkList.AddString(networkList[i]);

	m_networkList.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWifiToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWifiToolDlg::OnPaint()
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
HCURSOR CWifiToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWifiToolDlg::OnMenuCommand(UINT nID)
{
	// TODO: 在此添加命令处理程序代码
	OperateService svc(svcName);
	CSettingDlg settingDlg;
	switch (nID)
	{
	case ID_MENU_SETTING:
		hostedNetwork.GetSSIDandKey(settingDlg.m_ssid, settingDlg.m_key);
		UpdateData(FALSE);
		settingDlg.DoModal();
		break;
	case IDM_START:
		if (svc.StartService())
			SetServMenuStatus(SERV_STARTED);
		else m_pstatus->SetWindowText(_T("提示：开启失败，可能服务被禁用"));
		break;
	case IDM_RESTART:
		if (! svc.RestartService())
			SetServMenuStatus(SERV_STOPPED);
		break;
	case IDM_STOP:
		if (svc.StopService())
			SetServMenuStatus(SERV_STOPPED);
		else m_pstatus->SetWindowText(_T("提示：关闭失败，可能服务被禁用"));
		break;
	default: break;
	}
}

void CWifiToolDlg::OnBnClickedOpenwifi()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_OPENWIFI)->EnableWindow(FALSE);
	m_pstatus->SetWindowText(_T("提示：正在打开WiFi，请稍等..."));
	if (netShare.UnSharingNet()) return;
	if (hostedNetwork.StartHostedNetwork()) return;
	if (hostedNetwork.QueryStatus()) return;
	CString networkName;
	m_networkList.GetWindowText(networkName);
	if (netShare.AddSharingNet(networkName)) return;
	m_pstatus->SetWindowText(_T("提示：WiFi已成功打开"));
	GetDlgItem(IDC_CLOSEWIFI)->EnableWindow(TRUE);
}

void CWifiToolDlg::OnBnClickedClosewifi()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_CLOSEWIFI)->EnableWindow(FALSE);
	m_pstatus->SetWindowText(_T("提示：正在关闭WiFi，请稍等..."));
	if (netShare.UnSharingNet()) return;
	if (hostedNetwork.StopHostedNetwork()) return;
	m_pstatus->SetWindowText(_T("提示：WiFi已成功关闭"));
	GetDlgItem(IDC_OPENWIFI)->EnableWindow(TRUE);
}
int CWifiToolDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	    // 设置寻找标记
    ::SetProp(m_hWnd, AfxGetApp()->m_pszExeName, (HANDLE)1);

	return 0;
}

void CWifiToolDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	// 删除寻找标记
    ::RemoveProp(m_hWnd, AfxGetApp()->m_pszExeName);
}

void CWifiToolDlg::SetServMenuStatus(int status)
{
	switch (status)
	{
	case SERV_PENDING:
		mainMenu.EnableMenuItem(IDM_START, MF_GRAYED);
		mainMenu.EnableMenuItem(IDM_RESTART, MF_GRAYED);
		mainMenu.EnableMenuItem(IDM_STOP, MF_GRAYED);
		mainMenu.CheckMenuItem(IDM_START, MF_UNCHECKED);
		mainMenu.CheckMenuItem(IDM_STOP, MF_UNCHECKED);
		break;
	case SERV_STARTED:
		mainMenu.EnableMenuItem(IDM_RESTART, MF_ENABLED);
		mainMenu.EnableMenuItem(IDM_STOP, MF_ENABLED);
		mainMenu.CheckMenuItem(IDM_STOP, MF_UNCHECKED);
		mainMenu.EnableMenuItem(IDM_START, MF_GRAYED);
		mainMenu.CheckMenuItem(IDM_START, MF_CHECKED);
		break;
	case SERV_STOPPED:
		mainMenu.EnableMenuItem(IDM_START, MF_ENABLED);
		mainMenu.CheckMenuItem(IDM_START, MF_UNCHECKED);
		mainMenu.EnableMenuItem(IDM_RESTART, MF_GRAYED);
		mainMenu.EnableMenuItem(IDM_STOP, MF_GRAYED);
		mainMenu.CheckMenuItem(IDM_STOP, MF_CHECKED);
		break;
	default: break;
	}
}

