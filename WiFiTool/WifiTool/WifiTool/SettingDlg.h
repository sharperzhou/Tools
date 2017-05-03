#pragma once
#include "HostedNetwork.h"
#include "WifiTool.h"

// CSettingDlg �Ի���

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

// �Ի�������
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	// SSID���ı����������
	CString m_ssid;
	// Key���ı����������
	CString m_key;
	afx_msg void OnBnClickedSave();
};
