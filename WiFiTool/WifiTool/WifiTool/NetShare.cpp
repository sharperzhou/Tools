#pragma once
#include <stdafx.h>
#include "NetShare.h"

NetShare::NetShare()
{
	CoInitialize (NULL);
	// init security to enum RAS connections
	CoInitializeSecurity (NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);    
	HRESULT hr = ::CoCreateInstance (__uuidof(NetSharingManager), NULL, CLSCTX_ALL, __uuidof(INetSharingManager), (void**)&pNSM);
}

NetShare::~NetShare()
{
	if (pNSM) pNSM->Release();
	::CoUninitialize();
}

HRESULT NetShare::AddSharingNet(CString networkName)
{
	INetConnection * pNC = NULL;
	INetSharingEveryConnectionCollection * pNSECC = NULL;
	HRESULT hr = pNSM->get_EnumEveryConnection (&pNSECC);

	if (!pNSECC)
		AfxMessageBox(L"failed to get EveryConnectionCollection!");

	else 
	{
		IEnumVARIANT * pEV = NULL;
		IUnknown * pUnk = NULL;
		hr = pNSECC->get__NewEnum (&pUnk);

		if (pUnk)
		{
			hr = pUnk->QueryInterface (__uuidof(IEnumVARIANT), (void**)&pEV);
			pUnk->Release();
			pUnk = NULL;
		}

		if (pEV)
		{
			setlocale(LC_ALL, "");

			VARIANT v;
			VariantInit (&v);
			INetSharingConfiguration * pNSC = NULL;
			INetSharingConfiguration * pVWifiNSC = NULL;
			NETCON_PROPERTIES* pVWifi = NULL;
			while (S_OK == pEV->Next (1, &v, NULL))
			{
				if (V_VT (&v) == VT_UNKNOWN)
				{
					pUnk = V_UNKNOWN(&v);
					pUnk->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
					pUnk->Release();
					pUnk = NULL;
					if (pNC)
					{
						NETCON_PROPERTIES* pNP = NULL;
						pNC->GetProperties(&pNP);

						CString str(pNP->pszwName);
						bool isAutomatic = networkName == L"×Ô¶¯Ñ¡Ôñ" ? true : (str == networkName ? true : false);
						
						if(isAutomatic && pNP->Status == NCS_CONNECTED)
						{
							hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
							hr = pNSC->EnableSharing(ICSSHARINGTYPE_PUBLIC);
							if (pNSC) pNSC->Release();
							pNSC = NULL;
						}
						
						if(!strncmp((char*)(pNP->pszwDeviceName),(char*)(L"Microsoft Virtual WiFi Miniport Adapter"), 39))
						{
							pVWifi = pNP;
							hr = pNSM->get_INetSharingConfigurationForINetConnection(pNC,&pVWifiNSC);
							pVWifiNSC->EnableSharing(ICSSHARINGTYPE_PRIVATE);
							if (pVWifiNSC) pVWifiNSC->Release();
							pVWifiNSC = NULL;
						}
					}
				}
			}
			::VariantClear(&v);
			if (pNSC) pNSC->Release();
			if (pVWifiNSC) pVWifiNSC->Release();
			pEV->Release();
		}
	}
	if (pNC) pNC->Release();
	if (pNSECC) pNSECC->Release();
	return hr;
}

HRESULT NetShare::UnSharingNet()
{
	INetConnection * pNC = NULL;
	INetSharingEveryConnectionCollection * pNSECC = NULL;
	HRESULT hr = pNSM->get_EnumEveryConnection (&pNSECC);

	if (!pNSECC)
		AfxMessageBox(L"failed to get EveryConnectionCollection!");

	else 
	{
		IEnumVARIANT * pEV = NULL;
		IUnknown * pUnk = NULL;
		hr = pNSECC->get__NewEnum (&pUnk);

		if (pUnk)
		{
			hr = pUnk->QueryInterface (__uuidof(IEnumVARIANT), (void**)&pEV);
			pUnk->Release();
			pUnk = NULL;
		}

		if (pEV)
		{
			setlocale(LC_ALL, "");

			VARIANT v;
			VariantInit (&v);
			INetSharingConfiguration * pNSC = NULL;
			while (S_OK == pEV->Next (1, &v, NULL))
			{
				if (V_VT (&v) == VT_UNKNOWN)
				{
					pUnk = V_UNKNOWN(&v);
					pUnk->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
					pUnk->Release();
					pUnk = NULL;
					if (pNC)
					{
						NETCON_PROPERTIES* pNP = NULL;
						pNC->GetProperties(&pNP);					
						if(strncmp((char*)(pNP->pszwDeviceName),(char*)(L"Microsoft Virtual WiFi Miniport Adapter") ,39)
							|| pNP->Status != NCS_CONNECTED)
						{
							hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
							hr = pNSC->DisableSharing();
							if (pNSC) pNSC->Release();
							pNSC = NULL;
						}
					}
				}
			}
			::VariantClear(&v);
			if (pNSC) pNSC->Release();
			pEV->Release();
		}
	}
	if (pNC) pNC->Release();
	if (pNSECC) pNSECC->Release();
	return hr;
}

HRESULT NetShare::GetNetworkList(std::vector<CString> &networkList)
{
	INetConnection * pNC = NULL;
	INetSharingEveryConnectionCollection * pNSECC = NULL;
	HRESULT hr = pNSM->get_EnumEveryConnection (&pNSECC);

	if (!pNSECC)
		AfxMessageBox(L"failed to get EveryConnectionCollection!");

	else 
	{
		IEnumVARIANT * pEV = NULL;
		IUnknown * pUnk = NULL;
		hr = pNSECC->get__NewEnum (&pUnk);

		if (pUnk)
		{
			hr = pUnk->QueryInterface (__uuidof(IEnumVARIANT), (void**)&pEV);
			pUnk->Release();
			pUnk = NULL;
		}

		if (pEV)
		{
			setlocale(LC_ALL, "");

			VARIANT v;
			VariantInit (&v);
			INetSharingConfiguration * pNSC = NULL;
			while (S_OK == pEV->Next (1, &v, NULL))
			{
				if (V_VT (&v) == VT_UNKNOWN)
				{
					pUnk = V_UNKNOWN(&v);
					pUnk->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
					pUnk->Release();
					pUnk = NULL;
					if (pNC)
					{
						NETCON_PROPERTIES* pNP = NULL;
						pNC->GetProperties(&pNP);					
						if(pNP->Status == NCS_CONNECTED
							&& strncmp((char*)(pNP->pszwDeviceName),(char*)(L"Microsoft Virtual WiFi Miniport Adapter"), 39))
						{
							networkList.push_back(pNP->pszwName);
							if (pNSC) pNSC->Release();
							pNSC = NULL;
						}
					}
				}
			}
			::VariantClear(&v);
			if (pNSC) pNSC->Release();
			pEV->Release();
		}
	}
	if (pNC) pNC->Release();
	if (pNSECC) pNSECC->Release();
	return hr;
}