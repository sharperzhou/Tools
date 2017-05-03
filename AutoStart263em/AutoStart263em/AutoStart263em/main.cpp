#include <windows.h>
#include <tlhelp32.h>
#pragma comment(lib, "Kernel32.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")//不显示控制台（宽字符版）
#define SLEEP_TIME 20000

int wmain(int argc, wchar_t * argv[])
{
	//for (int i = 1; i < argc; i++)
	//	MessageBox(NULL, argv[i], L"弹出消息", MB_OK);
	
	if (argc != 4)
	{
		MessageBox(NULL, L"命令参数设置不正确，程序无法启动！", L"弹出消息", MB_OK);
		return 0;
	}

	SYSTEMTIME now, settime;
	settime.wHour = _wtoi(argv[2]);
	settime.wMinute = _wtoi(argv[3]);

	HANDLE hMutex;
	hMutex = CreateMutex(NULL, TRUE, L"AutoStart263em");
	if (ERROR_ALREADY_EXISTS == GetLastError())
		if (IDOK == MessageBox(NULL, L"程序已经运行，不允许开启多个进程！", L"弹出消息", MB_OK))
			return 0;

	while (true)
	{		
		GetLocalTime(&now);
		if (now.wHour == settime.wHour && now.wMinute == settime.wMinute)
		{
			HWND hwnd;
			hwnd = FindWindow(NULL, L"263云通信");
			if (! hwnd)
				{
					if (IDYES == MessageBox(NULL, L"是否打开263云通信？", L"弹出消息", MB_YESNO | MB_ICONQUESTION))
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
									TerminateProcess(hProcess,0);//关闭进程
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
						//Sleep(82800000); //线程暂停23小时
					}
					else
						Sleep(SLEEP_TIME * 3);  //保证取消后一分钟之内不再弹出对话框
				}
			else
				CloseHandle(hwnd);
		}
		Sleep(SLEEP_TIME);	
	}

	return 0;
}