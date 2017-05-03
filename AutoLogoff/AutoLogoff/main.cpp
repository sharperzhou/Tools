#include <windows.h>
#include <WtsApi32.h>
#include <tchar.h>
#include <stdio.h>
#include <tlhelp32.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//不显示控制台
#pragma comment(lib, "WtsApi32.lib")

#define LOGFILE "AutoLogOff.log"       //日志文件名称
#define INI_FILE ".\\Config.ini"       //配置文件所在路径

wchar_t pBuff[MAX_PATH];               //日志文件输出到可执行文件下的路径
unsigned int sleepTime;                //睡眠时间

/**
/////////////////////////////////////
* 函数名：GetSessionUserName
* 用  途：根据会话标识获取会话用户名
* 输  入：[in]dwsessionId, [out]userName
* 输  出：无
////////////////////////////////////
*/
void GetSessionUserName(DWORD dwsessionId, TCHAR *userName)
{
	LPWSTR pUserName;
	DWORD dwUserNameSize;
	BOOL bRet = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, dwsessionId, WTSUserName, &pUserName, &dwUserNameSize);
	if (bRet)
		_tcscpy(userName, pUserName);
	WTSFreeMemory(pUserName);
}

/**
/////////////////////////////////////
* 函数名：GetDomainName
* 用  途：根据会话标识获取会话域名
* 输  入：[in]dwsessionId, [out]domainName
* 输  出：无
////////////////////////////////////
*/
void GetDomainName(DWORD dwsessionId, TCHAR *domainName)
{
	LPWSTR pDomainName;
	DWORD dwDomainNameSize;
	BOOL bRet = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, dwsessionId, WTSDomainName, &pDomainName, &dwDomainNameSize);
	if (bRet)
		_tcscpy(domainName, pDomainName);
	WTSFreeMemory(pDomainName);
}

/**
/////////////////////////////////////
* 函数名：GetClientName
* 用  途：根据会话标识获取会话客户端
* 输  入：[in]dwsessionId, [out]clientName
* 输  出：无
////////////////////////////////////
*/
void GetClientName(DWORD dwsessionId, TCHAR *clientName)
{
	LPWSTR pClientName;
	DWORD dwClientName;
	BOOL bRet = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, dwsessionId, WTSClientName, &pClientName, &dwClientName); 
	if (bRet)
		_tcscpy(clientName, pClientName);
	WTSFreeMemory(pClientName);
}

/**
/////////////////////////////////////
* 函数名：WriteLog
* 用  途：输出日志文件
* 输  入：[in]str
* 输  出：0表示输出成功
* 全局变量：pBuff 文件名
////////////////////////////////////
*/
int WriteLog(TCHAR *str)
{
	FILE *pfile = NULL;
	pfile = _wfopen(pBuff, _T("a+"));
	SYSTEMTIME time;
	GetLocalTime(&time);
	_wsetlocale(0, L"chs");
	fwprintf(pfile, L"[%04d/%02d/%02d %02d:%02d:%02d:%03d]%s\n", 
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, str);
	fclose(pfile);
	return 0;
}

/**
/////////////////////////////////////
* 函数名：KillProcess
* 用  途：杀掉指定名称的进程，并返回被杀个数
* 输  入：[in]exeName
* 输  出：被杀进程个数
////////////////////////////////////
*/
UINT KillProcess(TCHAR *exeName)
{
	HANDLE hSnapshot; 
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 pe = {sizeof(pe)}; 
	Process32First(hSnapshot,&pe); 
	int count = 0;
	do 
	{ 
		if(_tccmp(pe.szExeFile, exeName)==0) 
		{ 
			HANDLE   hProcess; 
			hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,pe.th32ProcessID); 
			if   (hProcess) 
			{ 
				TerminateProcess(hProcess,0);//关闭进程
				count++;
			}
		} 
	}   
	while(Process32Next(hSnapshot,&pe)); 
	CloseHandle(hSnapshot); 

	return count;
}

/**
/////////////////////////////////////
* 函数名：AutoTSC
* 用  途：自动登录和注销本机远程
* 输  入：无
* 输  出：无
////////////////////////////////////
*/
void AutoTSC()
{
	WinExec((LPCSTR)("mstsc /v:127.0.0.1"), SW_HIDE);
	WriteLog(_T("远程桌面正在运行"));

	Sleep(sleepTime);  //线程等待3分钟

	//获取会话
	WTS_SESSION_INFO *sessionInfo;
	DWORD dwsessionInfoCount;
	BOOL bRet = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessionInfo, &dwsessionInfoCount);

	TCHAR userName[256];
	TCHAR domainName[256];
	TCHAR clientName[256];

	//获取本机计算机名和用户名
	TCHAR computerName[30];
	DWORD size;
	GetComputerName(computerName, &size);
	
	TCHAR nativeUserName[30];
	GetUserName(nativeUserName, &size);

	//遍历会话
	for (int i = 0; i < dwsessionInfoCount; i++)
	{
		GetSessionUserName(sessionInfo[i].SessionId, userName);
		GetDomainName(sessionInfo[i].SessionId, domainName);
		GetClientName(sessionInfo[i].SessionId, clientName);

		if (! wcscmp(userName, nativeUserName) && ! wcscmp(clientName, computerName))
		{
			bRet = WTSLogoffSession(WTS_CURRENT_SERVER_HANDLE, sessionInfo[i].SessionId, FALSE);
			if (bRet)
			{
				TCHAR pBuff[30];
				wcscpy(pBuff, _T("已注销用户名："));
				wcscat(pBuff, userName);
				WriteLog(pBuff);
				wmemset(pBuff, 0, 30);

				wcscpy(pBuff, _T("已注销客户端名："));
				wcscat(pBuff, clientName);
				WriteLog(pBuff);
				wmemset(pBuff, 0, 30);

				TCHAR str[6];
				wsprintf(str, _T("%d"),sessionInfo[i].SessionId);

				wcscpy(pBuff, _T("已注销标识："));
				wcscat(pBuff, str);
				WriteLog(pBuff);
			}
		}
	}
	WTSFreeMemory(&sessionInfo);
}

/**
/////////////////////////////////////
* 函数名：QueryAsmStatus
* 用  途：查询上网助手的验证状态
* 输  入：上网助手日志文件名
* 输  出：查询状态，1：success，0：fault，-1：未知
////////////////////////////////////
*/
int QueryAsmStatus(TCHAR *asmLogFile)
{
	FILE *pFile = _tfopen(asmLogFile, _T("r"));
	if (pFile == NULL) return -1;
	TCHAR buff[256];
	fseek(pFile, -243, SEEK_END);  //-243是倒数后大概的字符数，不需要精确
	while (_fgetts(buff, sizeof(buff), pFile) != NULL)
	{
		if (_tcsstr(buff, _T("success")) != NULL)
		{
			return 1;
		}
		if (_tcsstr(buff, _T("fault")) != NULL)
		{
			return 0;
		}
	}
	return -1;
}

int main()
{
	HANDLE hMutex;
	hMutex = CreateMutex(NULL, TRUE, L"AutoLogoff");
	if (ERROR_ALREADY_EXISTS == GetLastError())
		if (IDOK == MessageBox(NULL, L"程序已经运行，不允许开启多个进程！", L"弹出消息", MB_OK))
			return 0;

	GetModuleFileName(NULL, pBuff, MAX_PATH);
	(_tcsrchr(pBuff, L'\\'))[1] = 0;
	wcscat(pBuff, _T(LOGFILE));
	
	TCHAR AsmLogFile[MAX_PATH];
	GetPrivateProfileString(_T("Settings"), _T("AsmLogFile"), _T(""), AsmLogFile, sizeof(AsmLogFile), _T(INI_FILE));
	
	unsigned int maxStartCount;          //远程桌面最大启动次数
	maxStartCount = GetPrivateProfileInt(_T("Settings"), _T("MaxStartCount"), 1, _T(INI_FILE));
	sleepTime = GetPrivateProfileInt(_T("Settings"), _T("SleepTime"), 180000, _T(INI_FILE));

	WriteLog(_T("----------开始----------"));

	//杀进程
	int count = KillProcess(_T("mstsc.exe"));

	TCHAR plogBuff[30];
	TCHAR scount[3];
	wsprintf(scount, _T("%d"), count);

	wcscpy(plogBuff, _T("已杀掉进程："));
	wcscat(plogBuff, _T("mstsc.exe"));
	wcscat(plogBuff, _T("，个数为："));
	wcscat(plogBuff, scount);
	WriteLog(plogBuff);

	int state;
	int startCount = 0;
	do
	{
		//登录并注销远程桌面
		AutoTSC();

		//查询上网助手的验证状态
		state = QueryAsmStatus(AsmLogFile);
		switch (state)
		{
		case 1:
			WriteLog(_T("上网助手状态：success"));
			break;
		case 0:
			WriteLog(_T("上网助手状态：fault"));
			Sleep(sleepTime);
			break;
		default:
			WriteLog(_T("上网助手状态：未知"));
		}
	} while (++startCount < maxStartCount && state == 0);
		
	WriteLog(_T("----------结束----------"));
	return 0;
}