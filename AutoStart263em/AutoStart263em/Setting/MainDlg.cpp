#include <Windows.h>
#include <tchar.h>
#include <memory.h>
#include <CommDlg.h>
#include "resource.h"


//xp样式支持
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#define AUTOSTART263EM          TEXT("AutoStart263em")
#define AUTOSTART263EM_PROCESS  TEXT("AutoStart263em.exe")
#define OFN_FILTER              TEXT("263em.exe\0 263em.exe\0\0")
#define REG_PATH                TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
#define ERROR_AST263EM          TEXT("AutoStart263em.exe文件不存在或无法访问")
#define ERROR_263EM             TEXT("263em.exe文件不存在或无法访问")
#define ERROR_TIMEFORMAT        TEXT("时间格式设置不正确：时（0-23）、分（0-59）")
#define SUCCESS_SETTING         TEXT("设置成功，下次重启后生效")

typedef struct 
{
    int nHour;
    int nMinute;
    TCHAR szPath_AutoStart263em[MAX_PATH];
    TCHAR szPath_263em[MAX_PATH];
} DATA;

HWND g_hDlg;
HINSTANCE g_hInstance;

INT_PTR WINAPI DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL QueryParam(DATA *pdata);
BOOL SetParam(DATA *pdata);
void SplitString(TCHAR *src, TCHAR dest[][MAX_PATH], int n);

int _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
    g_hInstance = hInstance;
    return DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}

INT_PTR WINAPI DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static DATA data = {0};
    HICON hIcon = NULL;
    switch (uMsg)
    {
    case WM_INITDIALOG:
        
        hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_263EM_RED));
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
        SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);

        g_hDlg = hwndDlg;
        SendDlgItemMessage(hwndDlg, IDC_EDT_HOUR, EM_LIMITTEXT, (WPARAM) 2, 0);
        SendDlgItemMessage(hwndDlg, IDC_EDT_MINUTE, EM_LIMITTEXT, (WPARAM) 2, 0);

        if (QueryParam(&data)) {
            SetDlgItemText(hwndDlg, IDC_EDT_PATH, data.szPath_263em);
            SetDlgItemInt(hwndDlg, IDC_EDT_HOUR, data.nHour, FALSE);
            SetDlgItemInt(hwndDlg, IDC_EDT_MINUTE, data.nMinute, FALSE);
        }
        SetFocus(GetDlgItem(hwndDlg, IDC_BTN_CANCEL));
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_SELECT:
            {
                TCHAR szFileName[MAX_PATH] = {0};
                OPENFILENAME ofn = {0};
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrFilter = OFN_FILTER;
                ofn.lpstrFile = szFileName;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                if (GetOpenFileName(&ofn))
                    SetDlgItemText(hwndDlg, IDC_EDT_PATH, szFileName);
            }
            break;

        case IDC_BTN_SETTING:
            if (SetParam(&data))
                MessageBox(hwndDlg, SUCCESS_SETTING, TEXT("提示"), MB_OK);
            else break;

        case IDC_BTN_CANCEL:
            SendMessage(hwndDlg, WM_CLOSE, 0, 0);
            break;
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL QueryParam(DATA *pdata)
{
    HKEY hKey;
    LPCTSTR subKey = REG_PATH;
    LPCTSTR valName = AUTOSTART263EM;
    BOOL bRet = FALSE;

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &hKey))
    {
        DWORD dwType = REG_SZ;
        DWORD dwSize = sizeof(DWORD);
        RegQueryValueEx(hKey, valName, NULL, &dwType, NULL, &dwSize);
        if (dwSize)
        {
            TCHAR *szBuff = new TCHAR[dwSize / sizeof(TCHAR) + 1];
            if (ERROR_SUCCESS == RegQueryValueEx(hKey, valName, NULL, &dwType, (LPBYTE) szBuff, &dwSize))
            {
                TCHAR sztemp[4][MAX_PATH];
                SplitString(szBuff, sztemp, 4);
                _tcscpy(pdata->szPath_AutoStart263em,sztemp[0]);
                _tcscpy(pdata->szPath_263em, sztemp[1]);
                pdata->nHour = _ttoi(sztemp[2]);
                pdata->nMinute = _ttoi(sztemp[3]);
                delete [] szBuff;
                bRet = TRUE;
            }
        }
    }
    RegCloseKey(hKey);
    return bRet;
}

BOOL SetParam(DATA *pdata)
{
    BOOL bRet = FALSE;
    TCHAR szFileName[MAX_PATH] = {0};

    GetModuleFileName(NULL, szFileName, MAX_PATH);
    (_tcsrchr(szFileName, TEXT('\\')))[1] = 0;
    _tcscat(szFileName, AUTOSTART263EM_PROCESS);

    if (GetFileAttributes(szFileName) == -1) {
        MessageBox(g_hDlg, ERROR_AST263EM, NULL, MB_OK | MB_ICONERROR); return bRet;
    }
    _tcscpy(pdata->szPath_AutoStart263em, szFileName);

    GetDlgItemText(g_hDlg, IDC_EDT_PATH, szFileName, MAX_PATH);
    if (GetFileAttributes(szFileName) == -1) {
        MessageBox(g_hDlg, ERROR_263EM, NULL, MB_OK | MB_ICONERROR); return bRet;
    }
    _tcscpy(pdata->szPath_263em, szFileName);

    int nHour = GetDlgItemInt(g_hDlg, IDC_EDT_HOUR, NULL, FALSE);
    int nMinute = GetDlgItemInt(g_hDlg, IDC_EDT_MINUTE, NULL, FALSE);
    if (nHour < 0 || nHour > 23 || nMinute < 0 || nMinute > 59) {
        MessageBox(g_hDlg, ERROR_TIMEFORMAT, NULL, MB_OK | MB_ICONERROR); return bRet;
    }
    pdata->nHour = nHour;
    pdata->nMinute = nMinute;
    
    memset(szFileName, 0, MAX_PATH);  //use for register value

    TCHAR szDigit[4]; //nHour, nMinute trans to string

    szFileName[0] = TEXT('\"');
    _tcscat(szFileName, pdata->szPath_AutoStart263em);
    _tcscat(szFileName, TEXT("\" \""));
    _tcscat(szFileName, pdata->szPath_263em);
    _tcscat(szFileName, TEXT("\" \""));
    _tcscat(szFileName, _itot(pdata->nHour, szDigit, 10));
    _tcscat(szFileName, TEXT("\" \""));
    _tcscat(szFileName, _itot(pdata->nMinute, szDigit, 10));
    _tcscat(szFileName, TEXT("\""));

    HKEY hKey;
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH, 0, KEY_ALL_ACCESS, &hKey))
        if (ERROR_SUCCESS == RegSetValueEx(hKey, AUTOSTART263EM, 0, REG_SZ,
            (const BYTE*) szFileName, (_tcslen(szFileName)+1) * sizeof(TCHAR)))
            bRet = TRUE;

    RegCloseKey(hKey);

    return bRet;
}

void SplitString(TCHAR *src, TCHAR dest[][MAX_PATH], int n)
{
    TCHAR *p, *q;

    p = src;
    for (int i = 0; i < n; i++)
    {
        while (*p == TEXT(' ')) ++p;
        q = p + 1;
        while (*q != TEXT('\"')) ++q;
        memcpy(dest + i, p + 1, (q - p - 1) * sizeof(TCHAR));
        dest[i][q - p - 1] = TEXT('\0');
        p = q + 1;
    }
}

