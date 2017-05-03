#pragma once
#include <Winsvc.h>

class OperateService
{
public:
	OperateService(const wchar_t *servName);
	int QuerySerciceStatus();
	bool StartService();
	bool StopService();
	bool RestartService();
public:
	~OperateService(void);
private:
	SC_HANDLE hSCM;
	SC_HANDLE hService;
};
