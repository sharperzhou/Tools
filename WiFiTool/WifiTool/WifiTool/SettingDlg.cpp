// SettingDlg.cpp : ʵ���ļ�
//
#pragma once
#include "stdafx.h"
#include "SettingDlg.h"
#include "WifiToolDlg.h"

// CSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
	, m_ssid(_T(""))
	, m_key(_T(""))
{
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SETSSID, m_ssid);
	DDV_MaxChars(pDX, m_ssid, 63);
	DDX_Text(pDX, IDC_SETKEY, m_key);
	DDV_MaxChars(pDX, m_key, 31);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_SAVE, &CSettingDlg::OnBnClickedSave)
END_MESSAGE_MAP()


void CSettingDlg::OnBnClickedSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if (m_ssid.IsEmpty())
	{
		MessageBoxW(L"SSID����Ϊ1~63���ַ�");
		GetDlgItem(IDC_SETSSID)->SetFocus();
		return;
	}
	if (m_key.GetLength() < 8)
	{
		MessageBoxW(L"Key����Ϊ8~31���ַ�");
		GetDlgItem(IDC_SETKEY)->SetFocus();
		return;
	}
	HostedNetwork hostedNetwork;
	if (hostedNetwork.SetSSIDandKey(m_ssid, m_key))
		MessageBoxW(L"���ô���");
	else
		OnOK();
}
