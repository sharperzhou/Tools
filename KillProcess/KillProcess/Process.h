#include <tlhelp32.h>

class CProcess
{
public :
	/*****************************************************************************************\
	* 函 数 名:   Kill
	* 用    途:   按照进程名杀进程
	* 创建日期:   2016/07/16
	* 访 问 性:   public 
	* 返 回 值:   UINT
	* 参    数:   PCTSTR exeName
	\*****************************************************************************************/
	UINT Kill(PCTSTR exeName)
	{
		HANDLE hSnapshot = NULL; 
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		PROCESSENTRY32 pe = {sizeof(pe)}; 
		Process32First(hSnapshot,&pe); 
		UINT count = 0;
		do 
		{ 
			if(_tccmp(pe.szExeFile, exeName) == 0) 
			{ 
				HANDLE hProcess; 
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID); 
				if (hProcess) 
					if (TerminateProcess(hProcess,0))   //关闭进程
						count++;
			} 
		}   
		while(Process32Next(hSnapshot,&pe)); 
		CloseHandle(hSnapshot); 

		return count;
	}

	/*****************************************************************************************\
	* 函 数 名:   Kill
	* 用    途:   按照进程号（PID）杀进程
	* 创建日期:   2016/07/16
	* 访 问 性:   public 
	* 返 回 值:   UINT
	* 参    数:   DWORD pid
	\*****************************************************************************************/
	UINT Kill(DWORD pid)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		UINT count = 0;
		if (hProcess)
			if (TerminateProcess(hProcess, 0))
				count = 1;

		return count;
	}
};