// CalcPressureDlg.h : 头文件
//

#pragma once


// CCalcPressureDlg 对话框
class CCalcPressureDlg : public CDialog
{
// 构造
public:
	CCalcPressureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CALCPRESSURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
