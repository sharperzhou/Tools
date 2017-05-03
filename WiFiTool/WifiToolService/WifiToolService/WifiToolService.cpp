// WifiToolService.cpp : �������̨Ӧ�ó������ڵ㡣
//
#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <wlanapi.h>
#pragma comment(lib, "Wlanapi.lib")
#include <netcon.h>
#include <locale.h>
#include <stdarg.h>

/*      /////////////�����ܱ�������������ԭ��////////////////   */

#define SERVICE_NAME "WiFi Tool Service"    //��������
#define LOGFILE "WifiToolService.log"       //��־�ļ�����
wchar_t pBuff[MAX_PATH];                    //��־�ļ��������ִ���ļ��µ�·��
SERVICE_STATUS ServiceStatus;               //����״̬
SERVICE_STATUS_HANDLE hStatus;              //����״̬���

int writeLog(wchar_t *Format, ...);                //�����־
void WINAPI ServiceMain(char *argc, char** argv);  //�������̺߳�����ע���ββ���ʡ����
void WINAPI ServiceHandle(DWORD ctrlHandle);       //������ƾ������
void start();                                      //����ʼִ�д���
void stop();                                       //����ִֹͣ�д���


/*      /////////////���尸���ı�������������ԭ��////////////////   */
#define SLEEP_TIME 5000                                       //�̼߳������ʱ��(ms)
typedef void (CALLBACK *GETPROCADDRESS)(NETCON_PROPERTIES *); //����ص��������ͷ�NETCON_PROPERTIES�ṹ��
GETPROCADDRESS fun;                                           //���������ͷ�NETCON_PROPERTIES�ṹ��
HANDLE hClient = NULL;                                        //wlan���
INetSharingManager* pNSM = NULL;                              //����ICS����Ļ�������
INetConnection * pNC = NULL;                                  //����ICS����Ļ�������
INetSharingConfiguration * pNSC = NULL;                       //����ICS����Ļ�������
INetSharingEveryConnectionCollection * pNSECC = NULL;         //����ICS����Ļ�������
IEnumVARIANT * pEV = NULL;                                    //����ICS����Ļ�������
IUnknown * pUnk = NULL;                                       //����ICS����Ļ�������
VARIANT v;                                                    //����ICS����Ļ�������
int connFlag;                                                 //���ӱ�ʶ����0��ʾ���߳��������ӣ�1��ʾ���߳���δ���ӣ�5��ʾ�رշ���ʱ�ر����߳���
int shareFlag;                                                //��ʾ��������״̬��3��ʾ�Ѿ���������й���4��ʾ��ǰ�ѵ��ߣ���Ҫ�����Ƴ���������5��ʾ�رշ���ʱȡ�����繲��
DWORD dwVersion = 0;
DWORD dwResult = 0;
PWLAN_HOSTED_NETWORK_REASON pFailReason = NULL;
HANDLE task_handle = NULL;                                    //ѭ���߳̾��

void Detector();                                              //�������״��
DWORD WINAPI loop_thread(LPVOID param);                       //ѭ������߳�
HRESULT UnSharingNet();                                       //ȡ��ICS����
HRESULT AddSharingNet();                                      //����ICS����


int main()
{
	GetModuleFileName(NULL, pBuff, MAX_PATH);
	(_tcsrchr(pBuff, L'\\'))[1] = 0;
	wcscat(pBuff, _T(LOGFILE));

	//test
	//start();
	//loop_thread(NULL);
	//stop();

	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = _T(SERVICE_NAME);
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	// ��������Ŀ��Ʒ��ɻ��߳�
	StartServiceCtrlDispatcher(ServiceTable);	
	return 0;
}

void WINAPI ServiceHandle(DWORD ctrlHandle)
{
	switch (ctrlHandle)
	{	
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		writeLog(L"����ֹͣ����...");

		//����ֹͣ
		stop();
		ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
		ServiceStatus.dwWaitHint      = 0;
		SetServiceStatus(hStatus, &ServiceStatus);
		writeLog(L"������ֹͣ��");
		break;
	default:
		return;
	}
}

void WINAPI ServiceMain(char *argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	hStatus = RegisterServiceCtrlHandler(_T(SERVICE_NAME), ServiceHandle);  //ע�������ƾ������
	if (hStatus == 0) writeLog(L"����ע��ʧ�ܣ�"); 

	//��������
	start();
	SetServiceStatus(hStatus, &ServiceStatus);
	writeLog(L"�߳��Ѵ������������������У�");
}

void start()
{
	dwResult = WlanOpenHandle(2, 0, &dwVersion, &hClient);
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"��ȡWlan���ʧ�ܣ�");
		return;
	}

	dwResult = WlanHostedNetworkInitSettings(hClient, pFailReason, NULL); 
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"����������س�ʼ��ʧ�ܣ�");
		return;
	}

	BOOL pvData = TRUE;
	dwResult = WlanHostedNetworkSetProperty(hClient, wlan_hosted_network_opcode_enable, sizeof(BOOL), &pvData, pFailReason, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"����������ؿ���ʧ�ܣ�");
		return;
	}

	dwResult = WlanHostedNetworkForceStart(hClient, pFailReason, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"����WiFiǿ�ƿ���ʧ�ܣ�");
		return;
	}
	else
		connFlag = 0; //�ɹ��������߳���

	shareFlag = 4;    //������״̬���Ϊ�ѵ���

	
	task_handle = CreateThread(NULL, 0, loop_thread, NULL, 0, NULL);
	if (! task_handle) writeLog(L"ѭ���̴߳���ʧ�ܣ�");

	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
}

void stop()
{
	shareFlag = 5;  //���Ϊֹͣ����״̬
	connFlag =5;    //���Ϊֹͣ����״̬
	WaitForSingleObject(task_handle, INFINITE);
	CloseHandle(task_handle);
}

int writeLog(wchar_t *Format, ...)
{
	FILE *pfile = NULL;
	pfile = _wfopen(pBuff, L"a+");
	SYSTEMTIME time;
	GetLocalTime(&time);
	_wsetlocale(0, L"chs");
	fwprintf(pfile, L"[%04d/%02d/%02d %02d:%02d:%02d:%03d]", 
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	va_list ap;
	wchar_t *p;
	wchar_t *sval;
	int ival;
	double dval;
	va_start(ap, Format);
	for (p = const_cast<wchar_t *>(Format); *p; p++)
	{
		if (*p != L'%')
		{
			fputwc(*p, pfile);
			continue;
		}
		switch (*++p)
		{
		case L'd':
			ival = va_arg(ap, int);
			fwprintf(pfile, L"%d", ival);
			break;
		case L'f':
			dval = va_arg(ap, double);
			fwprintf(pfile, L"%f", dval);
			break;
		case L's':
			for (sval = va_arg(ap, wchar_t *); *sval; sval++)
				fputwc(*sval, pfile);
			break;
		default:
			fputwc(*p, pfile);
			break;
		}
	}
	va_end(ap);

	fputwc(L'\n', pfile);

	fclose(pfile);
	return 0;
}


//---------------�Զ��庯��-----------------

HRESULT UnSharingNet()
{
	HRESULT hr = 0;
	while (S_OK == pEV->Next(1, &v, NULL))
	{
		if (V_VT(&v) == VT_UNKNOWN)
		{
			pUnk = V_UNKNOWN(&v);
			hr = pUnk->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
			pUnk->Release();
			pUnk = NULL;
			if (pNC)
			{
				NETCON_PROPERTIES* pNP = NULL;
				pNC->GetProperties(&pNP);
				if ((pNP->MediaType < NCM_DIRECT) || (pNP->MediaType > NCM_PPPOE))
				{
					if (fun) fun(pNP);
					pNC->Release();
					pNC = NULL;
					continue;
				}

				if (pNP->dwCharacter & NCCF_SHARED)
				{
					hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
					hr = pNSC->DisableSharing();
					writeLog(L"%s��ȡ��PUBLIC����", pNP->pszwName);
					pNSC->Release();
					pNSC = NULL;
				}
				if (pNP->dwCharacter & NCCF_SHARED_PRIVATE)
				{
					hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
					hr = pNSC->DisableSharing();
					pNSC->Release();
					pNSC = NULL;
				}
				if (fun) fun(pNP);
				pNC->Release();
				pNC = NULL;
			}
		}
	}
	pEV->Reset();
	return hr;
}

HRESULT AddSharingNet()
{
	HRESULT hr = 0;
	while (S_OK == pEV->Next(1, &v, NULL))
	{
		if (V_VT(&v) == VT_UNKNOWN)
		{
			pUnk = V_UNKNOWN(&v);
			hr = pUnk->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
			pUnk->Release();
			pUnk = NULL;
			if (pNC)
			{
				NETCON_PROPERTIES* pNP = NULL;
				pNC->GetProperties(&pNP);
				if ((pNP->MediaType < NCM_DIRECT) || (pNP->MediaType > NCM_PPPOE))
				{
					if (fun) fun(pNP);
					pNC->Release();
					pNC = NULL;
					continue;
				}

				//Share
				if(strncmp((char*)(pNP->pszwDeviceName),(char*)(L"Microsoft Virtual WiFi Miniport Adapter"), 39)
					 && pNP->Status == NCS_CONNECTED)
				{
					hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
					hr = pNSC->EnableSharing(ICSSHARINGTYPE_PUBLIC);
					writeLog(L"%s�ѽ���PUBLIC����", pNP->pszwName);
					if (pNSC) pNSC->Release();
					pNSC = NULL;
					shareFlag = 3;
				}
				
				if(!strncmp((char*)(pNP->pszwDeviceName),(char*)(L"Microsoft Virtual WiFi Miniport Adapter"), 39))
				{
					hr = pNSM->get_INetSharingConfigurationForINetConnection(pNC, &pNSC);
					hr = pNSC->EnableSharing(ICSSHARINGTYPE_PRIVATE);
					if (pNSC) pNSC->Release();
					pNSC = NULL;
				}
				if (fun) fun(pNP);
				pNC->Release();
				pNC = NULL;
			}
		}
	}
	pEV->Reset();
	return hr;
}

void Detector()
{
	if (shareFlag == 5 && connFlag == 5) return;
	while (S_OK == pEV->Next (1, &v, NULL))
	{
		if (V_VT(&v) == VT_UNKNOWN)
		{
			pUnk = V_UNKNOWN(&v);
			pUnk->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
			pUnk->Release();
			pUnk = NULL;
			if (pNC)
			{
				NETCON_PROPERTIES* pNP = NULL;
				pNC->GetProperties(&pNP);
				if ((pNP->MediaType < NCM_DIRECT) || (pNP->MediaType > NCM_PPPOE))
				{
					if (fun) fun(pNP);
					pNC->Release();
					pNC = NULL;
					continue;
				}

				if(!strncmp((char*)(pNP->pszwDeviceName),(char*)(L"Microsoft Virtual WiFi Miniport Adapter"), 39)
					&& pNP->Status != NCS_CONNECTED)
				{
					if (connFlag == 0)
						writeLog(L"����WiFi�ѵ��ߣ�����ִ�лָ�...");

					connFlag = 1;
				}

				if ((pNP->dwCharacter & NCCF_SHARED)
					&& (pNP->Status == NCS_MEDIA_DISCONNECTED)
					)
				
				{
					writeLog(L"%s�ѶϿ�������Ѱ�ҿɹ��������...", pNP->pszwName);

					shareFlag = 4;
				}
				if (fun) fun(pNP);
				pNC->Release();
				pNC = NULL;
			}
		}
	}
	pEV->Reset();
}

DWORD WINAPI loop_thread(LPVOID param)
{
	CoInitialize (NULL);
	CoInitializeSecurity (NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);    
	::CoCreateInstance (__uuidof(NetSharingManager), NULL, CLSCTX_ALL, __uuidof(INetSharingManager), (void**)&pNSM);

	HMODULE hModule = NULL;
	hModule = ::LoadLibrary(L"NetShell.dll");  //��ȡDLL����ڵ�ַ
	fun = (GETPROCADDRESS)::GetProcAddress(hModule, (LPCSTR)"NcFreeNetconProperties"); //��ȡ���������
	FreeLibrary(hModule);

	if (pNSM->get_EnumEveryConnection (&pNSECC))
	{
		writeLog(L"��ȡ�������Ӷ���ʧ�ܣ�");
		return 1;
	}
	pNSECC->get__NewEnum (&pUnk);
	if (pUnk)
	{
		pUnk->QueryInterface(__uuidof(IEnumVARIANT), (void**)&pEV);
		pUnk->Release(); pUnk = NULL;
	}
	VariantInit(&v);
	setlocale(LC_ALL, "");

	while (true)
	{		
		Detector();
		if (connFlag == 1)
		{
			if (! WlanHostedNetworkForceStart(hClient, pFailReason, NULL))
			{
				writeLog(L"����WiFi�����´򿪣�");
				connFlag = 0;
			}
		}
		else if (shareFlag == 4)
		{
			UnSharingNet();
			AddSharingNet();
		}
		else if (shareFlag == 5 && connFlag == 5)
		{
			WlanHostedNetworkForceStop(hClient, pFailReason, NULL);
			WlanCloseHandle(hClient, NULL);
			writeLog(L"����WiFi��ǿ�ƹرգ�");
			UnSharingNet();
			if (pNSM) pNSM->Release();
			if (pNC) pNC->Release();
			if (pNSC) pNSC->Release();
			if (pNSECC) pNSECC->Release();
			if (pEV) pEV->Release();
			if (pUnk) pUnk->Release();
			VariantClear(&v);
			CoUninitialize();
			break;
		}
		Sleep(SLEEP_TIME);	
	}

	return 0;
}
