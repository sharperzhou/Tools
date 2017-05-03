#pragma once
#include <windows.h>
#include <netcon.h>
#include <stdio.h>
#include <locale.h>
#include <vector>

class NetShare
{
public:
	HRESULT AddSharingNet(CString networkName);

	HRESULT UnSharingNet();

	NetShare();

	~NetShare();

	HRESULT GetNetworkList(std::vector<CString> &);
private:
	INetSharingManager* pNSM;
};