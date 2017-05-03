#include <windows.h>
#include <WtsApi32.h>
#include <tchar.h>
#include <stdio.h>
#include <tlhelp32.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//����ʾ����̨
#pragma comment(lib, "WtsApi32.lib")

#define LOGFILE "AutoLogOff.log"       //��־�ļ�����
#define INI_FILE ".\\Config.ini"       //�����ļ�����·��

wchar_t pBuff[MAX_PATH];               //��־�ļ��������ִ���ļ��µ�·��
unsigned int sleepTime;                //˯��ʱ��

/**
/////////////////////////////////////
* ��������GetSessionUserName
* ��  ;�����ݻỰ��ʶ��ȡ�Ự�û���
* ��  �룺[in]dwsessionId, [out]userName
* ��  ������
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
* ��������GetDomainName
* ��  ;�����ݻỰ��ʶ��ȡ�Ự����
* ��  �룺[in]dwsessionId, [out]domainName
* ��  ������
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
* ��������GetClientName
* ��  ;�����ݻỰ��ʶ��ȡ�Ự�ͻ���
* ��  �룺[in]dwsessionId, [out]clientName
* ��  ������
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
* ��������WriteLog
* ��  ;�������־�ļ�
* ��  �룺[in]str
* ��  ����0��ʾ����ɹ�
* ȫ�ֱ�����pBuff �ļ���
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
* ��������KillProcess
* ��  ;��ɱ��ָ�����ƵĽ��̣������ر�ɱ����
* ��  �룺[in]exeName
* ��  ������ɱ���̸���
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
				TerminateProcess(hProcess,0);//�رս���
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
* ��������AutoTSC
* ��  ;���Զ���¼��ע������Զ��
* ��  �룺��
* ��  ������
////////////////////////////////////
*/
void AutoTSC()
{
	WinExec((LPCSTR)("mstsc /v:127.0.0.1"), SW_HIDE);
	WriteLog(_T("Զ��������������"));

	Sleep(sleepTime);  //�̵߳ȴ�3����

	//��ȡ�Ự
	WTS_SESSION_INFO *sessionInfo;
	DWORD dwsessionInfoCount;
	BOOL bRet = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessionInfo, &dwsessionInfoCount);

	TCHAR userName[256];
	TCHAR domainName[256];
	TCHAR clientName[256];

	//��ȡ��������������û���
	TCHAR computerName[30];
	DWORD size;
	GetComputerName(computerName, &size);
	
	TCHAR nativeUserName[30];
	GetUserName(nativeUserName, &size);

	//�����Ự
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
				wcscpy(pBuff, _T("��ע���û�����"));
				wcscat(pBuff, userName);
				WriteLog(pBuff);
				wmemset(pBuff, 0, 30);

				wcscpy(pBuff, _T("��ע���ͻ�������"));
				wcscat(pBuff, clientName);
				WriteLog(pBuff);
				wmemset(pBuff, 0, 30);

				TCHAR str[6];
				wsprintf(str, _T("%d"),sessionInfo[i].SessionId);

				wcscpy(pBuff, _T("��ע����ʶ��"));
				wcscat(pBuff, str);
				WriteLog(pBuff);
			}
		}
	}
	WTSFreeMemory(&sessionInfo);
}

/**
/////////////////////////////////////
* ��������QueryAsmStatus
* ��  ;����ѯ�������ֵ���֤״̬
* ��  �룺����������־�ļ���
* ��  ������ѯ״̬��1��success��0��fault��-1��δ֪
////////////////////////////////////
*/
int QueryAsmStatus(TCHAR *asmLogFile)
{
	FILE *pFile = _tfopen(asmLogFile, _T("r"));
	if (pFile == NULL) return -1;
	TCHAR buff[256];
	fseek(pFile, -243, SEEK_END);  //-243�ǵ������ŵ��ַ���������Ҫ��ȷ
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
		if (IDOK == MessageBox(NULL, L"�����Ѿ����У���������������̣�", L"������Ϣ", MB_OK))
			return 0;

	GetModuleFileName(NULL, pBuff, MAX_PATH);
	(_tcsrchr(pBuff, L'\\'))[1] = 0;
	wcscat(pBuff, _T(LOGFILE));
	
	TCHAR AsmLogFile[MAX_PATH];
	GetPrivateProfileString(_T("Settings"), _T("AsmLogFile"), _T(""), AsmLogFile, sizeof(AsmLogFile), _T(INI_FILE));
	
	unsigned int maxStartCount;          //Զ�����������������
	maxStartCount = GetPrivateProfileInt(_T("Settings"), _T("MaxStartCount"), 1, _T(INI_FILE));
	sleepTime = GetPrivateProfileInt(_T("Settings"), _T("SleepTime"), 180000, _T(INI_FILE));

	WriteLog(_T("----------��ʼ----------"));

	//ɱ����
	int count = KillProcess(_T("mstsc.exe"));

	TCHAR plogBuff[30];
	TCHAR scount[3];
	wsprintf(scount, _T("%d"), count);

	wcscpy(plogBuff, _T("��ɱ�����̣�"));
	wcscat(plogBuff, _T("mstsc.exe"));
	wcscat(plogBuff, _T("������Ϊ��"));
	wcscat(plogBuff, scount);
	WriteLog(plogBuff);

	int state;
	int startCount = 0;
	do
	{
		//��¼��ע��Զ������
		AutoTSC();

		//��ѯ�������ֵ���֤״̬
		state = QueryAsmStatus(AsmLogFile);
		switch (state)
		{
		case 1:
			WriteLog(_T("��������״̬��success"));
			break;
		case 0:
			WriteLog(_T("��������״̬��fault"));
			Sleep(sleepTime);
			break;
		default:
			WriteLog(_T("��������״̬��δ֪"));
		}
	} while (++startCount < maxStartCount && state == 0);
		
	WriteLog(_T("----------����----------"));
	return 0;
}