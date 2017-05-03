#include "StdAfx.h"
#include "HostedNetwork.h"

#if (_WIN32_WINNT >= 0x0601)
HostedNetwork::HostedNetwork(void)
{
	hClient = NULL;
	DWORD dwVersion = 0;
	DWORD dwResult = WlanOpenHandle(2, 0, &dwVersion, &hClient);
}

HostedNetwork::~HostedNetwork(void)
{
	WlanCloseHandle(hClient, NULL);
}

DWORD HostedNetwork::GetSSIDandKey(CString &ssid, CString &key)
{
	DWORD dwResult = 0;
	PWLAN_HOSTED_NETWORK_REASON pFailReason = NULL;
	//初始化
	dwResult = WlanHostedNetworkInitSettings(hClient, pFailReason, NULL); 
	//获取SSID
	DWORD ddatasize = NULL;
	void *pvdata = NULL;
	WLAN_OPCODE_VALUE_TYPE wovt;
	dwResult = WlanHostedNetworkQueryProperty(hClient, wlan_hosted_network_opcode_connection_settings, &ddatasize, &pvdata, &wovt, NULL);
	if (ERROR_SUCCESS == dwResult)
	{
		WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS *p = (WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS*)pvdata;
		char str_ssid[32] = {'\0'};
		memcpy(str_ssid, p->hostedNetworkSSID.ucSSID, p->hostedNetworkSSID.uSSIDLength);
		ssid = str_ssid;
	}

	//获取Key
	DWORD dwKeyLength = 0;//key长度
	unsigned char *pucKeyData = NULL;
	BOOL bIsPassPhrase,bPersistent;
	dwResult = WlanHostedNetworkQuerySecondaryKey(hClient,&dwKeyLength, &pucKeyData, &bIsPassPhrase, &bPersistent, pFailReason, NULL); 
	if (ERROR_SUCCESS == dwResult)
	{
		key = pucKeyData;
	}
	return dwResult;
}

DWORD HostedNetwork::SetSSIDandKey(const CString &ssid, const CString &key)
{
	DWORD dwResult = 0;
	PWLAN_HOSTED_NETWORK_REASON pFailReason = NULL;

	//开启无线承载
	//BOOL pvData = TRUE;
	//dwResult = WlanHostedNetworkSetProperty(hClient, wlan_hosted_network_opcode_enable, sizeof(BOOL), &pvData, pFailReason, NULL);
	//if (ERROR_SUCCESS != dwResult) return dwResult;

	//初始化
	dwResult = WlanHostedNetworkInitSettings(hClient, pFailReason, NULL);
	if (ERROR_SUCCESS != dwResult) return dwResult;

	char *_ssid = WideCharToMultiByte(ssid);
	//设置SSID
	WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS conn_set;
	memset(conn_set.hostedNetworkSSID.ucSSID,0,32);
	memcpy(conn_set.hostedNetworkSSID.ucSSID,_ssid,strlen(_ssid));
	conn_set.hostedNetworkSSID.uSSIDLength = strlen(_ssid);
	conn_set.dwMaxNumberOfPeers = 10;
	dwResult = WlanHostedNetworkSetProperty(hClient, wlan_hosted_network_opcode_connection_settings, sizeof(conn_set), (PVOID)&conn_set, pFailReason, NULL);
	delete[] _ssid;
	if (ERROR_SUCCESS != dwResult) return dwResult;
	
	char *_key = WideCharToMultiByte(key);
	//设置密码
	dwResult = WlanHostedNetworkSetSecondaryKey(hClient, strlen(_key) + 1, (PUCHAR)_key, TRUE, TRUE, pFailReason, NULL);   //注意长度需要包含"\0"
	delete[] _key;
	return dwResult;
}

DWORD HostedNetwork::StartHostedNetwork()
{
	DWORD dwResult = 0;
	PWLAN_HOSTED_NETWORK_REASON pFailReason = NULL;

	//开启无线承载
	BOOL pvData = TRUE;
	dwResult = WlanHostedNetworkSetProperty(hClient, wlan_hosted_network_opcode_enable, sizeof(BOOL), &pvData, pFailReason, NULL);
	if (ERROR_SUCCESS != dwResult) return dwResult;

	//初始化
	dwResult = WlanHostedNetworkInitSettings(hClient, pFailReason, NULL);
	if (ERROR_SUCCESS != dwResult) return dwResult;

	WinExec((LPCSTR)("netsh wlan start hostednetwork"), SW_HIDE);

	return dwResult;
}

DWORD HostedNetwork::StopHostedNetwork()
{
	DWORD dwResult = 0;

	WinExec((LPCSTR)("netsh wlan stop hostednetwork"), SW_HIDE);

	return dwResult;
}

DWORD HostedNetwork::QueryStatus()
{
	DWORD dwResult = 0;
	PWLAN_HOSTED_NETWORK_STATUS pWlanHostedNetworkStatus = NULL;
	dwResult = WlanHostedNetworkQueryStatus(hClient,&pWlanHostedNetworkStatus,NULL);
	if (ERROR_SUCCESS != dwResult) return dwResult;

	if (pWlanHostedNetworkStatus->HostedNetworkState == wlan_hosted_network_unavailable)
		dwResult = 1;
	else
		dwResult = 0;
	return dwResult;
}
#endif