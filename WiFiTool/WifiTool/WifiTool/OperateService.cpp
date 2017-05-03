#include "StdAfx.h"
#include "OperateService.h"

OperateService::OperateService(const wchar_t *servName)
{
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	hService = OpenService(hSCM, servName, SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP);
}

OperateService::~OperateService(void)
{
	::CloseServiceHandle(hSCM);
	::CloseServiceHandle(hService);
}

int OperateService::QuerySerciceStatus()
{
	if (! hService) return -1;
	
	//查询服务是否被禁用
	QUERY_SERVICE_CONFIG svcConfig;
	DWORD cbBytesNeeded;
	int k = ::QueryServiceConfig(hService, &svcConfig, sizeof(svcConfig), &cbBytesNeeded);
	if (svcConfig.dwStartType == 4) return -1;
	
	//查询服务的运行状态
	SERVICE_STATUS svcStatus;
	if (::QueryServiceStatus(hService, &svcStatus))
	{
		switch (svcStatus.dwCurrentState)
		{
		case SERVICE_STOPPED:
			return 1;
		case SERVICE_RUNNING:
			return 0;
		default: return -1;
		}
	}
	else return -1;
}

bool OperateService::StartService()
{
	if (this->QuerySerciceStatus() == 1)
	{
		if (::StartService(hService, 0, NULL)) return true;
		else return false;
	}
	else return false;
}

bool OperateService::StopService()
{
	if (this->QuerySerciceStatus() == 0)
	{
		SERVICE_STATUS svcStatus;
		if (::ControlService(hService, SERVICE_CONTROL_STOP, &svcStatus)) return true;
		else return false;
	}
	else return false;
}

bool OperateService::RestartService()
{
	if (this->StopService() && this->StartService()) return true;
	else return false;
}
