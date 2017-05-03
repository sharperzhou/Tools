// WifiToolService.cpp : 定义控制台应用程序的入口点。
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

/*      /////////////服务框架变量声明及函数原型////////////////   */

#define SERVICE_NAME "WiFi Tool Service"    //服务名称
#define LOGFILE "WifiToolService.log"       //日志文件名称
wchar_t pBuff[MAX_PATH];                    //日志文件输出到可执行文件下的路径
SERVICE_STATUS ServiceStatus;               //服务状态
SERVICE_STATUS_HANDLE hStatus;              //服务状态句柄

int writeLog(wchar_t *Format, ...);                //输出日志
void WINAPI ServiceMain(char *argc, char** argv);  //服务框架线程函数【注意形参不能省！】
void WINAPI ServiceHandle(DWORD ctrlHandle);       //服务控制句柄函数
void start();                                      //服务开始执行代码
void stop();                                       //服务停止执行代码


/*      /////////////具体案例的变量声明及函数原型////////////////   */
#define SLEEP_TIME 5000                                       //线程间隔运行时间(ms)
typedef void (CALLBACK *GETPROCADDRESS)(NETCON_PROPERTIES *); //定义回调函数，释放NETCON_PROPERTIES结构体
GETPROCADDRESS fun;                                           //函数名，释放NETCON_PROPERTIES结构体
HANDLE hClient = NULL;                                        //wlan句柄
INetSharingManager* pNSM = NULL;                              //用于ICS共享的基本变量
INetConnection * pNC = NULL;                                  //用于ICS共享的基本变量
INetSharingConfiguration * pNSC = NULL;                       //用于ICS共享的基本变量
INetSharingEveryConnectionCollection * pNSECC = NULL;         //用于ICS共享的基本变量
IEnumVARIANT * pEV = NULL;                                    //用于ICS共享的基本变量
IUnknown * pUnk = NULL;                                       //用于ICS共享的基本变量
VARIANT v;                                                    //用于ICS共享的基本变量
int connFlag;                                                 //连接标识符，0表示无线承载已连接，1表示无线承载未连接，5表示关闭服务时关闭无线承载
int shareFlag;                                                //表示共享网络状态，3表示已经对网络进行共享，4表示当前已掉线，需要进行移除共享、共享，5表示关闭服务时取消网络共享
DWORD dwVersion = 0;
DWORD dwResult = 0;
PWLAN_HOSTED_NETWORK_REASON pFailReason = NULL;
HANDLE task_handle = NULL;                                    //循环线程句柄

void Detector();                                              //侦测连接状况
DWORD WINAPI loop_thread(LPVOID param);                       //循环侦测线程
HRESULT UnSharingNet();                                       //取消ICS共享
HRESULT AddSharingNet();                                      //进行ICS共享


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
	// 启动服务的控制分派机线程
	StartServiceCtrlDispatcher(ServiceTable);	
	return 0;
}

void WINAPI ServiceHandle(DWORD ctrlHandle)
{
	switch (ctrlHandle)
	{	
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		writeLog(L"正在停止服务...");

		//服务停止
		stop();
		ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
		ServiceStatus.dwWaitHint      = 0;
		SetServiceStatus(hStatus, &ServiceStatus);
		writeLog(L"服务已停止！");
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

	hStatus = RegisterServiceCtrlHandler(_T(SERVICE_NAME), ServiceHandle);  //注册服务控制句柄函数
	if (hStatus == 0) writeLog(L"服务注册失败！"); 

	//服务运行
	start();
	SetServiceStatus(hStatus, &ServiceStatus);
	writeLog(L"线程已创建，服务正在运行中！");
}

void start()
{
	dwResult = WlanOpenHandle(2, 0, &dwVersion, &hClient);
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"获取Wlan句柄失败！");
		return;
	}

	dwResult = WlanHostedNetworkInitSettings(hClient, pFailReason, NULL); 
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"无线网络承载初始化失败！");
		return;
	}

	BOOL pvData = TRUE;
	dwResult = WlanHostedNetworkSetProperty(hClient, wlan_hosted_network_opcode_enable, sizeof(BOOL), &pvData, pFailReason, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"无线网络承载开启失败！");
		return;
	}

	dwResult = WlanHostedNetworkForceStart(hClient, pFailReason, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		writeLog(L"虚拟WiFi强制开启失败！");
		return;
	}
	else
		connFlag = 0; //成功开启无线承载

	shareFlag = 4;    //将共享状态标记为已掉线

	
	task_handle = CreateThread(NULL, 0, loop_thread, NULL, 0, NULL);
	if (! task_handle) writeLog(L"循环线程创建失败！");

	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
}

void stop()
{
	shareFlag = 5;  //标记为停止服务状态
	connFlag =5;    //标记为停止服务状态
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


//---------------自定义函数-----------------

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
					writeLog(L"%s已取消PUBLIC共享！", pNP->pszwName);
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
					writeLog(L"%s已进行PUBLIC共享！", pNP->pszwName);
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
						writeLog(L"虚拟WiFi已掉线，正在执行恢复...");

					connFlag = 1;
				}

				if ((pNP->dwCharacter & NCCF_SHARED)
					&& (pNP->Status == NCS_MEDIA_DISCONNECTED)
					)
				
				{
					writeLog(L"%s已断开，正在寻找可共享的网络...", pNP->pszwName);

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
	hModule = ::LoadLibrary(L"NetShell.dll");  //获取DLL的入口地址
	fun = (GETPROCADDRESS)::GetProcAddress(hModule, (LPCSTR)"NcFreeNetconProperties"); //获取函数的入口
	FreeLibrary(hModule);

	if (pNSM->get_EnumEveryConnection (&pNSECC))
	{
		writeLog(L"获取网络连接对象失败！");
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
				writeLog(L"虚拟WiFi已重新打开！");
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
			writeLog(L"虚拟WiFi已强制关闭！");
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
