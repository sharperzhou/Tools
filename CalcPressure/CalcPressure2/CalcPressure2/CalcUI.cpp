#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")
#include "resource.h"
#include "Calculation.h"
#include "StringAndDouble.h"

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

#ifdef UNICODE
#define CF_TCHARTEXT CF_UNICODETEXT
#else
#define CF_TCHARTEXT CF_TEXT
#endif

#define STATUSBARID  999
#define EDITNORMALCOLOR GetSysColor(COLOR_WINDOW)  //文本框正常背景色
#define EDITCOPIEDCOLOR RGB(0, 255, 255)   //文本框复制后的颜色
#define OUTOFFRANGE TEXT("超出范围")


HINSTANCE g_hInstance;
HWND      g_hwndMain;

typedef struct
{
	int iShowPatten;           //小数位数显示方式
	int iDigit, iLength;       //小数位数及数字长度
	BOOL bMainDlgTopmost;      //主窗口是否为Topmost样式
} SettingData, *pSettingData;

INT_PTR CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK WndProcSetting(HWND, UINT, WPARAM, LPARAM);
void CopyText(double value, HWND hwndStatusBar);
void PasteText(HWND hwndEdit);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    g_hInstance = hInstance;
    InitCommonControls();
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, WndProcMain);
}

INT_PTR CALLBACK WndProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static SettingData sd = {IDC_RD_FIXEDDIGIT, 6, 0, TRUE};
	static StringAndDouble::FormatType formattype = {(StringAndDouble::Type)(sd.iShowPatten - IDC_RD_FIXEDDIGIT),
		sd.iLength, sd.iDigit};
    static Calculation pressure;
    static HWND hwndStatus;
    static HBRUSH hbrEditNormal, hbrEditCopied;
    static int iColoredID = -1;  //标识需要着色的文本框ID
    double input = 0;  //从文本框取数double值
    TCHAR szBuff[31] = {0};  //文本框取数buff
    RECT rcClient;
    int id;
    HICON hIcon;

	switch (uMsg)
	{
    case WM_INITDIALOG:
        g_hwndMain = hwnd;

        hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON));
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);

        hbrEditNormal = CreateSolidBrush(EDITNORMALCOLOR);  
        hbrEditCopied = CreateSolidBrush(EDITCOPIEDCOLOR); 

        hwndStatus = CreateWindowEx(0, STATUSCLASSNAME, TEXT(""), WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, hwnd, (HMENU) STATUSBARID, g_hInstance, 0);
        GetClientRect(hwnd, &rcClient);
        MoveWindow(hwndStatus, 0, rcClient.bottom - 18, rcClient.right, 18, TRUE);
        SendMessage(hwndStatus, SB_SETTEXT, 0, (LPARAM) TEXT("就绪"));
        return TRUE;

	case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
            switch (LOWORD(wParam))
            {
            case IDM_SETTING:
                if (DialogBoxParam((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                    MAKEINTRESOURCE(IDD_DLG_SETTING), hwnd, WndProcSetting, (LPARAM)&sd))
                {
                    SetWindowPos(hwnd, sd.bMainDlgTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
                        0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                    formattype.type = (StringAndDouble::Type)(sd.iShowPatten - IDC_RD_FIXEDDIGIT);
                    formattype.length = sd.iLength;
                    formattype.digits = sd.iDigit;
                    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
                    style = sd.bMainDlgTopmost ? style |= WS_EX_TOPMOST : style &= ~WS_EX_TOPMOST;
                    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
                }
                break;

            case IDM_COPY:
                if (GetWindowText(GetFocus(), szBuff, 31)) {
                    id = GetDlgCtrlID(GetFocus());
                    if (id >= IDC_EDIT_MPAG && id <= IDC_EDIT_KPA) {
                        input = pressure.value[id - IDC_EDIT_MPAG];
                        iColoredID = id;
                        CopyText(input, hwndStatus);
                        InvalidateRect(GetDlgItem(hwnd, id), NULL, TRUE);
                    }
                }
                break;

            case IDM_PASTE:
                id = GetDlgCtrlID(GetFocus());
                if (id >= IDC_EDIT_MPAG && id <= IDC_EDIT_KPA) {
                    PasteText(GetDlgItem(hwnd, id));
                }

                break;
            }
            break;

        case EN_CHANGE:
            switch (LOWORD(wParam))
            {
            case IDC_EDIT_MPAG:
                if (GetFocus() == (HWND) lParam) {
                    GetDlgItemText(hwnd, LOWORD(wParam), szBuff, 31);
                    if (StringAndDouble::StrtoD(szBuff, input)) {
                        pressure.calc(MPag, input);

                        if (pressure.value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[1], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_KPAG, szBuff);

                        if (pressure.value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[2], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_MMHG, szBuff);

                        if (pressure.value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[3], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_KPA, szBuff);
                    } else {
                        SetDlgItemText(hwnd, IDC_EDIT_KPAG, TEXT(""));
                        SetDlgItemText(hwnd, IDC_EDIT_MMHG, TEXT(""));
                        SetDlgItemText(hwnd, IDC_EDIT_KPA, TEXT(""));
                    }
                }
                break;

            case IDC_EDIT_KPAG:
                if (GetFocus() == (HWND) lParam) {
                    GetDlgItemText(hwnd, LOWORD(wParam), szBuff, 31);
                    if (StringAndDouble::StrtoD(szBuff, input)) {
                        pressure.calc(kPag, input);

                        if (pressure.value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[0], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_MPAG, szBuff);

                        if (pressure.value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[2], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_MMHG, szBuff);

                        if (pressure.value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[3], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_KPA, szBuff);
                    } else {
                        SetDlgItemText(hwnd, IDC_EDIT_MPAG, TEXT(""));
                        SetDlgItemText(hwnd, IDC_EDIT_MMHG, TEXT(""));
                        SetDlgItemText(hwnd, IDC_EDIT_KPA, TEXT(""));
                    }
                }
                break;

            case IDC_EDIT_MMHG:
                if (GetFocus() == (HWND) lParam) {
                    GetDlgItemText(hwnd, LOWORD(wParam), szBuff, 31);
                    if (StringAndDouble::StrtoD(szBuff, input)) {
                        pressure.calc(mmHg, input);

                        if (pressure.value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[0], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_MPAG, szBuff);

                        if (pressure.value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[1], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_KPAG, szBuff);

                        if (pressure.value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                        else StringAndDouble::DtoFormatStr(pressure.value[3], szBuff, formattype);
                        SetDlgItemText(hwnd, IDC_EDIT_KPA, szBuff);
                    } else {
                        SetDlgItemText(hwnd, IDC_EDIT_MPAG, TEXT(""));
                        SetDlgItemText(hwnd, IDC_EDIT_KPAG, TEXT(""));
                        SetDlgItemText(hwnd, IDC_EDIT_KPA, TEXT(""));
                    }
                }
                break;

            case IDC_EDIT_KPA:
                if (GetFocus() == (HWND) lParam) {
                    GetDlgItemText(hwnd, LOWORD(wParam), szBuff, 31);
                    if (StringAndDouble::StrtoD(szBuff, input)) {
                    pressure.calc(kPa, input);

                    if (pressure.value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                    else StringAndDouble::DtoFormatStr(pressure.value[0], szBuff, formattype);
                    SetDlgItemText(hwnd, IDC_EDIT_MPAG, szBuff);

                    if (pressure.value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                    else StringAndDouble::DtoFormatStr(pressure.value[1], szBuff, formattype);
                    SetDlgItemText(hwnd, IDC_EDIT_KPAG, szBuff);

                    if (pressure.value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                    else StringAndDouble::DtoFormatStr(pressure.value[2], szBuff, formattype);
                    SetDlgItemText(hwnd, IDC_EDIT_MMHG, szBuff);
                    } else {
                    SetDlgItemText(hwnd, IDC_EDIT_MPAG, TEXT(""));
                    SetDlgItemText(hwnd, IDC_EDIT_KPAG, TEXT(""));
                    SetDlgItemText(hwnd, IDC_EDIT_MMHG, TEXT(""));
                    }
                }
                break;
            }
            break;

        case EN_SETFOCUS:
            switch (LOWORD(wParam))
            {
            case IDC_EDIT_MPAG:
                iColoredID = IDC_EDIT_MPAG == iColoredID ? IDC_EDIT_MPAG : -1; break;
            case IDC_EDIT_KPAG:
                iColoredID = IDC_EDIT_KPAG == iColoredID ? IDC_EDIT_KPAG : -1; break;
            case IDC_EDIT_MMHG:
                iColoredID = IDC_EDIT_MMHG == iColoredID ? IDC_EDIT_MMHG : -1; break;
            case IDC_EDIT_KPA:
                iColoredID = IDC_EDIT_KPA == iColoredID ? IDC_EDIT_KPA : -1; break;
            }
            break;
        }

		return TRUE;

    case WM_CTLCOLOREDIT:
        id = GetWindowLongPtr((HWND) lParam, GWL_ID);
        if (id >= IDC_EDIT_MPAG && id <= IDC_EDIT_KPA) {
            if (id == iColoredID) {
                SetBkColor((HDC) wParam, EDITCOPIEDCOLOR);
                return (INT_PTR) hbrEditCopied;
            }
            else {
                SetBkColor((HDC) wParam, EDITNORMALCOLOR);
                return (INT_PTR) hbrEditNormal;
            }
        }

	case WM_CLOSE:
        DeleteObject(hbrEditNormal);
        DeleteObject(hbrEditCopied);
		EndDialog(hwnd, 0);
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK WndProcSetting(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static SettingData sd, *psd;
	BOOL is_fixedlength_enable;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		psd = (SettingData *)lParam;
		sd = *psd;
		CheckRadioButton(hwnd, IDC_RD_FIXEDDIGIT, IDC_RD_FIXEDLENGTH, sd.iShowPatten);
		CheckDlgButton(hwnd, IDC_CHK_ONTOP, sd.bMainDlgTopmost);
		SetDlgItemInt(hwnd, IDC_EDIT_DIGIT, sd.iDigit, FALSE);
		SetDlgItemInt(hwnd, IDC_EDIT_LENGTH, sd.iLength, FALSE);

		is_fixedlength_enable = BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_RD_FIXEDDIGIT),
			BM_GETCHECK, 0, 0);

		EnableWindow(GetDlgItem(hwnd, IDC_STATIC_LENGTH), !is_fixedlength_enable);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT_LENGTH), !is_fixedlength_enable);

		SetFocus(GetDlgItem(hwnd, sd.iShowPatten));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{	
		case IDOK:
			sd.iDigit = GetDlgItemInt(hwnd, IDC_EDIT_DIGIT, FALSE, FALSE);
			sd.iLength = GetDlgItemInt(hwnd, IDC_EDIT_LENGTH, FALSE, FALSE);
			sd.bMainDlgTopmost = IsDlgButtonChecked(hwnd, IDC_CHK_ONTOP);
			*psd = sd;
			EndDialog(hwnd,TRUE);
			break;

		case IDCANCEL:
			EndDialog(hwnd, FALSE);
			break;

		case IDC_RD_FIXEDDIGIT:
		case IDC_RD_FIXEDLENGTH:
			sd.iShowPatten = LOWORD(wParam);
			CheckRadioButton(hwnd, IDC_RD_FIXEDDIGIT, IDC_RD_FIXEDLENGTH, sd.iShowPatten);

			is_fixedlength_enable = BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_RD_FIXEDDIGIT),
				BM_GETCHECK, 0, 0);

			EnableWindow(GetDlgItem(hwnd, IDC_STATIC_LENGTH), !is_fixedlength_enable);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT_LENGTH), !is_fixedlength_enable);

			break;

		}
		return TRUE;
	}
	return FALSE;
}

void CopyText(double value, HWND hwndStatusBar)
{
    if (OpenClipboard(g_hwndMain))
    {
        TCHAR szResult[30] = {0};
        TCHAR *pBuff = NULL;
        
        EmptyClipboard();

        StringAndDouble::DtoWholeStr(value, szResult);

        HANDLE hClip = GlobalAlloc(GHND, (lstrlen(szResult) + 1) * sizeof(TCHAR));
        pBuff = (TCHAR *) GlobalLock(hClip);

        lstrcpy(pBuff, szResult);

        GlobalUnlock(hClip);
        SetClipboardData(CF_TCHARTEXT, hClip);

        lstrcpy(szResult, TEXT("已复制值："));
        lstrcat(szResult, pBuff);

        CloseClipboard();
        pBuff = NULL;

        SendMessage(hwndStatusBar, SB_SETTEXT, 0, (LPARAM) szResult);
    }
}

void PasteText(HWND hwndEdit)
{
    if (OpenClipboard(g_hwndMain)) {
        if (IsClipboardFormatAvailable(CF_TCHARTEXT)) {
            HANDLE hClip = GetClipboardData(CF_TCHARTEXT);
            TCHAR *pBuff = (TCHAR*) GlobalLock(hClip);
            
            SetWindowText(hwndEdit, pBuff);

            GlobalUnlock(hClip);
            CloseClipboard();
            pBuff = NULL;
        }
    }
}