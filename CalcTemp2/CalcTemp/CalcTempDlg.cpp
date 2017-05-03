// CalcTempDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CalcTemp.h"
#include "CalcTempDlg.h"


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


// CCalcTempDlg �Ի���




CCalcTempDlg::CCalcTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcTempDlg::IDD, pParent)
	, LastTBfocusedID(0)
{
	for (int i = 0; i < MAX; i++)
	{
		m_txtC[i] = -32767;
		m_txtF[i] = -32767;
		m_txtK[i] = -32767;
		m_txtR[i] = -32767;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalcTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for (int i = 0; i < MAX; i++)
	{
		DDX_Control(pDX, IDC_C(i), m_CtrlC[i]);
		DDX_Control(pDX, IDC_F(i), m_CtrlF[i]);
		DDX_Control(pDX, IDC_K(i), m_CtrlK[i]);
		DDX_Control(pDX, IDC_R(i), m_CtrlR[i]);
	}
}

BEGIN_MESSAGE_MAP(CCalcTempDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(EN_CHANGE, IDC_C(0), IDC_R(MAX), &CCalcTempDlg::OnTextChange)  //����ؼ�����Ϣ����
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_C(0), IDC_R(MAX), &CCalcTempDlg::OnLostFocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_C(0), IDC_R(MAX), &CCalcTempDlg::OnGetFocus)
	ON_BN_CLICKED(IDC_btnCopy, &CCalcTempDlg::OnBnClickedbtncopy)
	ON_BN_CLICKED(IDC_btnPaste, &CCalcTempDlg::OnBnClickedbtnpaste)
	ON_BN_CLICKED(IDC_btnFWD, &CCalcTempDlg::OnBnClickedbtnfwd)
	ON_BN_CLICKED(IDC_btnNEXT, &CCalcTempDlg::OnBnClickedbtnnext)
END_MESSAGE_MAP()


// CCalcTempDlg ��Ϣ�������

BOOL CCalcTempDlg::OnInitDialog()
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

	m_status.Create(this);
	UINT nIDs = 10000;
	m_status.SetIndicators(&nIDs, 1);
	m_status.SetPaneInfo(0, 10000, SBPS_STRETCH, 0);
	CRect rect;
	GetClientRect(&rect);
	m_status.MoveWindow(0, rect.bottom - 20, rect.right, 20);


	for (int i = 0; i < MAX; i++)
	{
		m_CtrlC[i].SetWindowTextW(L"");
		m_CtrlF[i].SetWindowTextW(L"");
		m_CtrlK[i].SetWindowTextW(L"");
		m_CtrlR[i].SetWindowTextW(L"");
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCalcTempDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCalcTempDlg::OnPaint()
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
HCURSOR CCalcTempDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCalcTempDlg::OnTextChange(UINT nID)
{
	if (nID != ::GetDlgCtrlID(::GetFocus())) return;  //��ID���ڵ��ı���ǽ��㣬�����˳�
	double input= -32767;
	wchar_t ch1[25], ch2[25], ch3[25], ch4[25];
	bool isNumValid;
	StringAndDouble::FormatType ft = {StringAndDouble::FixedDigit, 0, 2};
	for (int i = 0; i < MAX; i++)
	{
		if (GetFocus() == GetDlgItem(IDC_C(i)))
		{
			m_CtrlC[i].GetWindowTextW(ch1, sizeof(ch1) / sizeof(wchar_t));
			isNumValid = StringAndDouble::StrtoD(ch1, input);
			if (! isNumValid)
			{
				m_CtrlF[i].SetWindowTextW(_T(""));
				m_CtrlK[i].SetWindowTextW(_T(""));
				m_CtrlR[i].SetWindowTextW(_T(""));
				m_txtC[i] = m_txtF[i] = m_txtK[i] = m_txtR[i] = -32767;
			}
			else
			{
				temper[i].SetValue(input, C);
				m_txtC[i] = temper[i].GetValue(C);
				m_txtF[i] = temper[i].GetValue(F);
				m_txtK[i] = temper[i].GetValue(K);
				m_txtR[i] = temper[i].GetValue(R);
				StringAndDouble::DtoFormatStr(m_txtF[i], ch2, ft);
				StringAndDouble::DtoFormatStr(m_txtK[i], ch3, ft);
				StringAndDouble::DtoFormatStr(m_txtR[i], ch4, ft);
				m_CtrlF[i].SetWindowTextW(ch2);
				m_CtrlK[i].SetWindowTextW(ch3);
				m_CtrlR[i].SetWindowTextW(ch4);
			}
			break;
		}
		else if (GetFocus() == GetDlgItem(IDC_F(i)))
		{
			m_CtrlF[i].GetWindowTextW(ch2, sizeof(ch2) / sizeof(wchar_t));
			isNumValid = StringAndDouble::StrtoD(ch2, input);
			if (! isNumValid)
			{
				m_CtrlC[i].SetWindowTextW(_T(""));
				m_CtrlK[i].SetWindowTextW(_T(""));
				m_CtrlR[i].SetWindowTextW(_T(""));
				m_txtC[i] = m_txtF[i] = m_txtK[i] = m_txtR[i] = -32767;
			}
			else
			{
				temper[i].SetValue(input, F);
				m_txtC[i] = temper[i].GetValue(C);
				m_txtF[i] = temper[i].GetValue(F);
				m_txtK[i] = temper[i].GetValue(K);
				m_txtR[i] = temper[i].GetValue(R);
				StringAndDouble::DtoFormatStr(m_txtC[i], ch1, ft);
				StringAndDouble::DtoFormatStr(m_txtK[i], ch3, ft);
				StringAndDouble::DtoFormatStr(m_txtR[i], ch4, ft);
				m_CtrlC[i].SetWindowTextW(ch1);
				m_CtrlK[i].SetWindowTextW(ch3);
				m_CtrlR[i].SetWindowTextW(ch4);
			}
			break;
		}
		else if (GetFocus() == GetDlgItem(IDC_K(i)))
		{
			m_CtrlK[i].GetWindowTextW(ch3, sizeof(ch3) / sizeof(wchar_t));
			isNumValid = StringAndDouble::StrtoD(ch3, input);
			if (! isNumValid)
			{
				m_CtrlC[i].SetWindowTextW(_T(""));
				m_CtrlF[i].SetWindowTextW(_T(""));
				m_CtrlR[i].SetWindowTextW(_T(""));
				m_txtC[i] = m_txtF[i] = m_txtK[i] = m_txtR[i] = -32767;
			}
			else
			{
				temper[i].SetValue(input, K);
				m_txtC[i] = temper[i].GetValue(C);
				m_txtF[i] = temper[i].GetValue(F);
				m_txtK[i] = temper[i].GetValue(K);
				m_txtR[i] = temper[i].GetValue(R);
				StringAndDouble::DtoFormatStr(m_txtC[i], ch1, ft);
				StringAndDouble::DtoFormatStr(m_txtF[i], ch2, ft);
				StringAndDouble::DtoFormatStr(m_txtR[i], ch4, ft);
				m_CtrlC[i].SetWindowTextW(ch1);
				m_CtrlF[i].SetWindowTextW(ch2);
				m_CtrlR[i].SetWindowTextW(ch4);
			}
			break;
		}
		else if (GetFocus() == GetDlgItem(IDC_R(i)))
		{
			m_CtrlR[i].GetWindowTextW(ch4, sizeof(ch4) / sizeof(wchar_t));
			isNumValid = StringAndDouble::StrtoD(ch4, input);
			if (! isNumValid)
			{
				m_CtrlC[i].SetWindowTextW(_T(""));
				m_CtrlF[i].SetWindowTextW(_T(""));
				m_CtrlK[i].SetWindowTextW(_T(""));
				m_txtC[i] = m_txtF[i] = m_txtK[i] = m_txtR[i] = -32767;
			}
			else
			{
				temper[i].SetValue(input, R);
				m_txtC[i] = temper[i].GetValue(C);
				m_txtF[i] = temper[i].GetValue(F);
				m_txtK[i] = temper[i].GetValue(K);
				m_txtR[i] = temper[i].GetValue(R);
				StringAndDouble::DtoFormatStr(m_txtC[i], ch1, ft);
				StringAndDouble::DtoFormatStr(m_txtF[i], ch2, ft);
				StringAndDouble::DtoFormatStr(m_txtK[i], ch3, ft);
				m_CtrlC[i].SetWindowTextW(ch1);
				m_CtrlF[i].SetWindowTextW(ch2);
				m_CtrlK[i].SetWindowTextW(ch3);
			}
			break;
		}
	}
}


void CCalcTempDlg::OnBnClickedbtncopy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CEditEx *pEdit = (CEditEx*)GetDlgItem(LastTBfocusedID);
	pEdit->SetbkColor(RGB(0, 255, 255));
	double temp;
	for (int i = 0; i < MAX; i++)
	{
		if (LastTBfocusedID ==IDC_C(i))
		{
			temp = m_txtC[i];
			break;
		}
		else if (LastTBfocusedID == IDC_F(i))
		{
			temp = m_txtF[i];
			break;
		}
		else if (LastTBfocusedID == IDC_K(i))
		{
			temp = m_txtK[i];
			break;
		}
		else if (LastTBfocusedID == IDC_R(i))
		{
			temp = m_txtR[i];
			break;
		}
	}
	if (OpenClipboard())
	{
		wchar_t str[25];
		HANDLE hClip;
		wchar_t * pBuf;
		EmptyClipboard();  //��ռ�����
		StringAndDouble::DtoWholeStr(temp, str);
		hClip = GlobalAlloc(GHND, 2 * wcslen(str) + sizeof(wchar_t));
		pBuf = (wchar_t *)GlobalLock(hClip);
		wcscpy(pBuf, str);
		GlobalUnlock(hClip);
		SetClipboardData(CF_UNICODETEXT, hClip);
		CloseClipboard();

		CString cstr(_T("�Ѹ���ֵ��"));
		cstr.Append(str);
		m_status.SetPaneText(0, cstr);
		
	}
}

void CCalcTempDlg::OnLostFocus(UINT nID)
{
	this->LastTBfocusedID = nID;
}

void CCalcTempDlg::OnGetFocus(UINT nID)
{
	if (LastTBfocusedID == 0) return;
	CEditEx *pEdit = (CEditEx*)GetDlgItem(LastTBfocusedID);
	pEdit->SetbkColor(RGB(255, 255, 255));
}

void CCalcTempDlg::OnBnClickedbtnpaste()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (OpenClipboard())
	{
		if (IsClipboardFormatAvailable(CF_UNICODETEXT))
		{
			HANDLE hClip;
			wchar_t * pBuf;
			hClip = GetClipboardData(CF_UNICODETEXT);
			pBuf = (wchar_t *)GlobalLock(hClip);
			SetDlgItemTextW(LastTBfocusedID, pBuf);
			CloseClipboard();
			
			//ʹ��ճ���ı����ȡ���㣬�����¼���
			((CEditEx*)GetDlgItem(LastTBfocusedID))->SetFocus();
			OnTextChange(LastTBfocusedID);
		}
	}
}

void CCalcTempDlg::OnBnClickedbtnfwd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CEditEx *pEdit;
	if (LastTBfocusedID == IDC_C0)
		pEdit = (CEditEx *)GetDlgItem(LastTBfocusedID);
	else
		pEdit = (CEditEx *)GetDlgItem(LastTBfocusedID - 1);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
}

void CCalcTempDlg::OnBnClickedbtnnext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CEditEx *pEdit;
	if (LastTBfocusedID == IDC_R3)
		pEdit = (CEditEx *)GetDlgItem(LastTBfocusedID);
	else
		pEdit = (CEditEx *)GetDlgItem(LastTBfocusedID + 1);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
}


BOOL CCalcTempDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
