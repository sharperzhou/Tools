// KillProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KillProcess.h"
#include "KillProcessDlg.h"
#include "Process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKillProcessDlg �Ի���




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


// CKillProcessDlg ��Ϣ�������

BOOL CKillProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//����״̬��
	{
		CRect rect;
		UINT indicator = 2000;
		m_statusbar.CreateEx(this);
		GetClientRect(&rect);
		m_statusbar.SetIndicators(&indicator, 1);
		m_statusbar.SetPaneInfo(0, 2000, SBPS_STRETCH, 0);
		m_statusbar.MoveWindow(rect.left, rect.bottom - 20, rect.Width(), 20);
	}

	//�����ı�������
	{
		m_font.CreatePointFont(140, _T("Arial"));
		GetDlgItem(IDC_EDIT_CONTENT)->SetFont(&m_font);
	}

	//�����ı��򽹵�
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_CONTENT);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
	}



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKillProcessDlg::OnPaint()
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
HCURSOR CKillProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKillProcessDlg::OnBnClickedBtnExec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	_stprintf(str, _T("��ɱ�����̸�����%d"), killedCount);
	m_statusbar.SetPaneText(0, str);
	GetDlgItem(IDC_BTN_EXEC)->EnableWindow(FALSE);
}

BOOL CKillProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	bool bRet = FALSE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		bRet = TRUE;
	else
		bRet = CDialog::PreTranslateMessage(pMsg);

	return bRet;
}

void CKillProcessDlg::OnEnSetfocusEditContent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_statusbar.SetPaneText(0, _T(""));
	GetDlgItem(IDC_BTN_EXEC)->EnableWindow(TRUE);
}
