// WifiToolDlg.h : ͷ�ļ�

#pragma once
#include "NetShare.h"
#include "HostedNetwork.h"
#include "afxwin.h"

#define SERV_PENDING   -1   //WiFi���߷���״̬δ֪
#define SERV_STARTED   0   //WiFi���߷���������
#define SERV_STOPPED   1   //WiFi���߷�����ֹͣ

#define svcName        L"WiFi Tool Service"

// CWifiToolDlg �Ի���
class CWifiToolDlg : public CDialog
{
// ����
public:
	CWifiToolDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CWifiToolDlg();

// �Ի�������
	enum { IDD = IDD_WIFITOOL_DIALOG };

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
	void SetServMenuStatus(int status); //���÷���˵������ü���ѡ״��
	HostedNetwork hostedNetwork;
};
