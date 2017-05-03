#pragma once
#include <windows.h>
#if (_WIN32_WINNT >= 0x0601)
#include <wlanapi.h>
#pragma comment(lib, "wlanapi.lib")
#endif

class HostedNetwork
{
public:
	HostedNetwork(void);
public:
	~HostedNetwork(void);
	DWORD GetSSIDandKey(CString &ssid, CString &key);
	DWORD SetSSIDandKey(const CString &ssid, const CString &key);
	DWORD StartHostedNetwork();
	DWORD StopHostedNetwork();
	DWORD QueryStatus();
private:
	HANDLE hClient;

	char *WideCharToMultiByte(const CString &str)
	{
		int iSize;
		char* pszMultiByte;
		iSize = ::WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		pszMultiByte = (char*)malloc((iSize+1));
		::WideCharToMultiByte(CP_ACP, 0, str, -1, pszMultiByte, iSize, NULL, NULL);
		return pszMultiByte;
	}
};
