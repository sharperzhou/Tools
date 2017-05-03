// WifiToolDlg.h : 头文件

#pragma once
#include "NetShare.h"
#include "HostedNetwork.h"
#include "afxwin.h"

#define SERV_PENDING   -1   //WiFi工具服务状态未知
#define SERV_STARTED   0   //WiFi工具服务已启动
#define SERV_STOPPED   1   //WiFi工具服务已停止

#define svcName        L"WiFi Tool Service"

// CWifiToolDlg 对话框
class CWifiToolDlg : public CDialog
{
// 构造
public:
	CWifiToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CWifiToolDlg();

// 对话框数据
	enum { IDD = IDD_WIFITOOL_DIALOG };

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

private:
	CMenu mainMenu;
	afx_msg void OnMenuCommand(UINT nID);
	NetShare netShare;
	afx_msg void OnBnClickedOpenwifi();
	afx_msg void OnBnClickedClosewifi();
	CStatic *m_pstatus;
	CComboBox m_networkList;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	void SetServMenuStatus(int status); //设置服务菜单的启用及勾选状况
	HostedNetwork hostedNetwork;
};
