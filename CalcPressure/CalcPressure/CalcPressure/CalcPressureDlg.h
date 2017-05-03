// CalcPressureDlg.h : ͷ�ļ�
//

#pragma once


// CCalcPressureDlg �Ի���
class CCalcPressureDlg : public CDialog
{
// ����
public:
	CCalcPressureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CALCPRESSURE_DIALOG };

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
public:
	afx_msg void OnEnChangeGage();
public:
	afx_msg void OnEnChangeVacuum();
public:
	afx_msg void OnEnChangeMmhg();
public:
	afx_msg void OnEnChangeAbs();
public:
	afx_msg void OnBnClickedCheck();
public:
	afx_msg void OnBnClickedOk();
public:
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
