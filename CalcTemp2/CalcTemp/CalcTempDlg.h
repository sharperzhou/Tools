// CalcTempDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Temperature.h"
#include "StringAndDouble.h"
#include "EditEx.h"

#define IDC_C(N) (IDC_C0 + N)
#define IDC_F(N) (IDC_F0 + N)
#define IDC_K(N) (IDC_K0 + N)
#define IDC_R(N) (IDC_R0 + N)
#define MAX 4


// CCalcTempDlg �Ի���
class CCalcTempDlg : public CDialog
{
// ����
public:
	CCalcTempDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CALCTEMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	CStatusBar m_status;

public:
	double m_txtC[MAX];
	double m_txtF[MAX];
	double m_txtK[MAX];
	double m_txtR[MAX];

	CEditEx m_CtrlC[MAX];
	CEditEx m_CtrlF[MAX];
	CEditEx m_CtrlK[MAX];
	CEditEx m_CtrlR[MAX];

	Temperature temper[MAX];
public:
	afx_msg void OnTextChange(UINT nID); //����ؼ�����Ϣ����
	afx_msg void OnLostFocus(UINT nID);
	afx_msg void OnGetFocus(UINT nID);
public:
//	virtual void OnOK();
//	virtual void OnCancel();
//	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedbtncopy();
	// ͨ��ʧȥ�������Ϣ��Ӧ����¼���һ���ı����ID��
	int LastTBfocusedID;
public:
	afx_msg void OnBnClickedbtnpaste();
public:
	afx_msg void OnBnClickedbtnfwd();
public:
	afx_msg void OnBnClickedbtnnext();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
