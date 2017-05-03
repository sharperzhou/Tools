// WifiToolDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "WifiTool.h"
#include "WifiToolDlg.h"
#include "OperateService.h"
#include "SettingDlg.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWifiToolDlg �Ի���




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


// CWifiToolDlg ��Ϣ�������

BOOL CWifiToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
		m_pstatus->SetWindowText(_T("��ʾ����ǰWiFi�ѿ���"));
	}

	std::vector<CString> networkList;
	netShare.GetNetworkList(networkList);
	m_networkList.AddString(_T("�Զ�ѡ��"));
	for (int i = 0; i < networkList.size(); i++)
		m_networkList.AddString(networkList[i]);

	m_networkList.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWifiToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CWifiToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWifiToolDlg::OnMenuCommand(UINT nID)
{
	// TODO: �ڴ���������������
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
		else m_pstatus->SetWindowText(_T("��ʾ������ʧ�ܣ����ܷ��񱻽���"));
		break;
	case IDM_RESTART:
		if (! svc.RestartService())
			SetServMenuStatus(SERV_STOPPED);
		break;
	case IDM_STOP:
		if (svc.StopService())
			SetServMenuStatus(SERV_STOPPED);
		else m_pstatus->SetWindowText(_T("��ʾ���ر�ʧ�ܣ����ܷ��񱻽���"));
		break;
	default: break;
	}
}

void CWifiToolDlg::OnBnClickedOpenwifi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_OPENWIFI)->EnableWindow(FALSE);
	m_pstatus->SetWindowText(_T("��ʾ�����ڴ�WiFi�����Ե�..."));
	if (netShare.UnSharingNet()) return;
	if (hostedNetwork.StartHostedNetwork()) return;
	if (hostedNetwork.QueryStatus()) return;
	CString networkName;
	m_networkList.GetWindowText(networkName);
	if (netShare.AddSharingNet(networkName)) return;
	m_pstatus->SetWindowText(_T("��ʾ��WiFi�ѳɹ���"));
	GetDlgItem(IDC_CLOSEWIFI)->EnableWindow(TRUE);
}

void CWifiToolDlg::OnBnClickedClosewifi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_CLOSEWIFI)->EnableWindow(FALSE);
	m_pstatus->SetWindowText(_T("��ʾ�����ڹر�WiFi�����Ե�..."));
	if (netShare.UnSharingNet()) return;
	if (hostedNetwork.StopHostedNetwork()) return;
	m_pstatus->SetWindowText(_T("��ʾ��WiFi�ѳɹ��ر�"));
	GetDlgItem(IDC_OPENWIFI)->EnableWindow(TRUE);
}
int CWifiToolDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	    // ����Ѱ�ұ��
    ::SetProp(m_hWnd, AfxGetApp()->m_pszExeName, (HANDLE)1);

	return 0;
}

void CWifiToolDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

	// ɾ��Ѱ�ұ��
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

