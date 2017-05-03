// CalcTempDlg.h : 头文件
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


// CCalcTempDlg 对话框
class CCalcTempDlg : public CDialog
{
// 构造
public:
	CCalcTempDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CALCTEMP_DIALOG };

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
	afx_msg void OnTextChange(UINT nID); //多个控件的消息处理
	afx_msg void OnLostFocus(UINT nID);
	afx_msg void OnGetFocus(UINT nID);
public:
//	virtual void OnOK();
//	virtual void OnCancel();
//	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedbtncopy();
	// 通过失去焦点的消息响应来记录最近一个文本框的ID号
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
