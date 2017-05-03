#include <windows.h>

class CPrevilege
{
public:
	/*****************************************************************************************\
	* 函 数 名:   Raise
	* 用    途:   提升进程权限
	* 创建日期:   2016/07/16
	* 访 问 性:   public 
	* 返 回 值:   BOOL
	* 参    数:   HANDLE ProcessHandle
	* 参    数:   LPCTSTR lpPrivilegeName
	\*****************************************************************************************/
	BOOL Raise(HANDLE ProcessHandle, LPCTSTR lpPrivilegeName)
	{
		BOOL bRet = FALSE;
		if (OpenProcessToken(ProcessHandle, TOKEN_ALL_ACCESS, &m_hToken))
			if (LookupPrivilegeValue(NULL, lpPrivilegeName, &m_tkp.Privileges[0].Luid))
				bRet = AdjustTokenPrivileges(m_hToken, FALSE, &m_tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
		return bRet;
	}
	CPrevilege() : m_hToken(NULL)
	{
		m_tkp.PrivilegeCount = 1;
		m_tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	~CPrevilege()
	{
		if (m_hToken)
			::CloseHandle(m_hToken);
	}
protected:
	HANDLE m_hToken;
	TOKEN_PRIVILEGES m_tkp;
};