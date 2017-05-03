#include <tlhelp32.h>

class CProcess
{
public :
	/*****************************************************************************************\
	* �� �� ��:   Kill
	* ��    ;:   ���ս�����ɱ����
	* ��������:   2016/07/16
	* �� �� ��:   public 
	* �� �� ֵ:   UINT
	* ��    ��:   PCTSTR exeName
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
					if (TerminateProcess(hProcess,0))   //�رս���
						count++;
			} 
		}   
		while(Process32Next(hSnapshot,&pe)); 
		CloseHandle(hSnapshot); 

		return count;
	}

	/*****************************************************************************************\
	* �� �� ��:   Kill
	* ��    ;:   ���ս��̺ţ�PID��ɱ����
	* ��������:   2016/07/16
	* �� �� ��:   public 
	* �� �� ֵ:   UINT
	* ��    ��:   DWORD pid
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