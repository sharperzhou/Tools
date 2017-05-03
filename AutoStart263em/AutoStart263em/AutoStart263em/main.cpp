#include <windows.h>
#include <tlhelp32.h>
#pragma comment(lib, "Kernel32.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")//����ʾ����̨�����ַ��棩
#define SLEEP_TIME 20000

int wmain(int argc, wchar_t * argv[])
{
	//for (int i = 1; i < argc; i++)
	//	MessageBox(NULL, argv[i], L"������Ϣ", MB_OK);
	
	if (argc != 4)
	{
		MessageBox(NULL, L"����������ò���ȷ�������޷�������", L"������Ϣ", MB_OK);
		return 0;
	}

	SYSTEMTIME now, settime;
	settime.wHour = _wtoi(argv[2]);
	settime.wMinute = _wtoi(argv[3]);

	HANDLE hMutex;
	hMutex = CreateMutex(NULL, TRUE, L"AutoStart263em");
	if (ERROR_ALREADY_EXISTS == GetLastError())
		if (IDOK == MessageBox(NULL, L"�����Ѿ����У���������������̣�", L"������Ϣ", MB_OK))
			return 0;

	while (true)
	{		
		GetLocalTime(&now);
		if (now.wHour == settime.wHour && now.wMinute == settime.wMinute)
		{
			HWND hwnd;
			hwnd = FindWindow(NULL, L"263��ͨ��");
			if (! hwnd)
				{
					if (IDYES == MessageBox(NULL, L"�Ƿ��263��ͨ�ţ�", L"������Ϣ", MB_YESNO | MB_ICONQUESTION))
					{

						HANDLE hSnapshot; 
						hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
						PROCESSENTRY32 pe = {sizeof(pe)}; 
						Process32First(hSnapshot,&pe); 
						do 
						{ 
							if(wcscmp(pe.szExeFile, L"263em.exe")==0) 
							{ 
								HANDLE   hProcess; 
								hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,pe.th32ProcessID); 
								if   (hProcess) 
								{ 
									TerminateProcess(hProcess,0);//�رս���
									break;
								} 
							} 
						}   
						while(Process32Next(hSnapshot,&pe)); 
						CloseHandle(hSnapshot); 

						
						STARTUPINFO startupInfo;
						PROCESS_INFORMATION processInfo;
						memset(&startupInfo, 0, sizeof(STARTUPINFO));
						startupInfo.cb = sizeof(STARTUPINFO);
						startupInfo.dwFlags=STARTF_USESHOWWINDOW;
						startupInfo.wShowWindow = SW_HIDE;
						CreateProcess(argv[1], NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);
						Sleep(SLEEP_TIME * 2);
						if (processInfo.hThread)
							CloseHandle(processInfo.hThread);
						if (processInfo.hThread)
							CloseHandle(processInfo.hThread);
						//Sleep(82800000); //�߳���ͣ23Сʱ
					}
					else
						Sleep(SLEEP_TIME * 3);  //��֤ȡ����һ����֮�ڲ��ٵ����Ի���
				}
			else
				CloseHandle(hwnd);
		}
		Sleep(SLEEP_TIME);	
	}

	return 0;
}