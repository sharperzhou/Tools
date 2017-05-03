#include "stdafx.h"
#include "Calculation.h"
#include "StringAndDouble.h"
#include "resource.h"

typedef struct
{
    int iShowPatten;           //小数位数显示方式
    int iDigit, iLength;       //小数位数及数字长度
    BOOL bMainDlgTopmost;      //主窗口是否为Topmost样式
} SettingData, *pSettingData;

#define EDITNORMALCOLOR GetSysColor(COLOR_WINDOW)  //文本框正常背景色
#define EDITCOPIEDCOLOR RGB(0, 255, 255)   //文本框复制后的颜色
#define OUTOFFRANGE TEXT("超出范围")

static const TCHAR szAppName[] = _T("压力计算");
static const TCHAR szMainWndClsName[] = _T("MainWndCls");
static const TCHAR szSettingDlgClsName[] = _T("SettingDlgCls");

static HINSTANCE g_hInstance;
static HFONT g_hFont;
static SettingData g_sd = {IDC_RD_FIXEDDIGIT, 6, 0, TRUE};
static BOOL g_fEndModel; //判断模态对话框关闭时是否保存数据

LRESULT CALLBACK WndProcMain(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProcSetting(HWND, UINT, WPARAM, LPARAM);

void CenterWindow(HWND hwnd, int nWidth, int nHeight);
void DoModel(HWND hwndParent);
void CopyText(double value, HWND hwndStatusBar);
void PasteText(HWND hwndEdit);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEX wndcls;
    MSG msg;
    HWND hwnd;

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);
    
    wndcls.cbSize = sizeof(WNDCLASSEX);
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = 0;
    wndcls.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndcls.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wndcls.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndcls.hInstance = hInstance;
    wndcls.lpfnWndProc = (WNDPROC) WndProcMain;
    wndcls.lpszClassName = szMainWndClsName;
    wndcls.lpszMenuName = NULL;
    wndcls.style = CS_HREDRAW | CS_VREDRAW;

    //注册主窗口
    if (!RegisterClassEx(&wndcls)) {
        MessageBox(NULL, _T("This is not a invalid Windows NT application!"), szAppName, MB_OK | MB_ICONERROR);
        return 0;
    }

    //注册设置窗口
    wndcls.lpszClassName = szSettingDlgClsName;
    wndcls.lpfnWndProc = (WNDPROC) DlgProcSetting;
    RegisterClassEx(&wndcls);

    hwnd = CreateWindowEx(WS_EX_TOPMOST, szMainWndClsName, szAppName, WS_OVERLAPPEDWINDOW & (~(WS_MAXIMIZEBOX | WS_THICKFRAME)),
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nShowCmd);

    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_KEYDOWN && (msg.wParam == VK_TAB || msg.wParam == VK_RETURN))
        {
            HWND hWndCtrlNext = GetNextDlgTabItem(hwnd, GetFocus(),
                (GetKeyState(VK_SHIFT) & 0x8000) ? TRUE : FALSE);//取得当前焦点控件的下一个控件的句柄
            if (hWndCtrlNext)
                SetFocus(hWndCtrlNext);//设置下一件控件得到输入焦点
            continue;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(wndcls.hbrBackground);

    return msg.wParam;
}

LRESULT CALLBACK WndProcMain(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static StringAndDouble::FormatType formattype = {(StringAndDouble::Type)(g_sd.iShowPatten - IDC_RD_FIXEDDIGIT),
        g_sd.iLength, g_sd.iDigit};
    static Calculation pressure;
    static HBRUSH hbrEditNormal, hbrEditCopied;
    static int iColoredID = -1;  //标识需要着色的文本框ID
    static TCHAR *szLabel[4] = {_T("MPa (表)"), _T("kPa (真)"), _T("mm汞柱"), _T("kPa (绝)")};

    HDC hdc;
    PAINTSTRUCT ps;
    double input = 0;  //从文本框取数double值
    TCHAR szBuff[31] = {0};  //文本框取数buff
    int i;

    switch (uMsg) {
        case WM_CREATE:
            {
                HWND hwndEdit[4], hwndStatus;
                HMENU hMenu;
                LOGFONT lf;

                g_hInstance = ((LPCREATESTRUCT) lParam)->hInstance;

                CenterWindow(hwndMain, 231, 201);
                
                ZeroMemory(&lf, sizeof(LOGFONT));
                hdc = GetDC(hwndMain);
                lf.lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
                ReleaseDC(hwndMain, hdc);
                lf.lfWeight = 400;
                lstrcpy(lf.lfFaceName, _T("MS Shell Dlg"));
                g_hFont = CreateFontIndirect(&lf);

                hbrEditNormal = CreateSolidBrush(EDITNORMALCOLOR);  
                hbrEditCopied = CreateSolidBrush(EDITCOPIEDCOLOR); 
                
                hMenu = CreateMenu();
                AppendMenu(hMenu, MF_STRING, IDM_SETTING, _T("设置(&S)"));
                AppendMenu(hMenu, MF_STRING, IDM_COPY, _T("复制(&C)"));
                AppendMenu(hMenu, MF_STRING, IDM_PASTE, _T("粘贴(&V)"));
                SetMenu(hwndMain, hMenu);

                for (i = 0; i < 4; ++i) {
                    hwndEdit[i] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), _T(""),
                        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
                        60, 10 + i * 30, 153, 23, hwndMain, (HMENU) i + IDC_EDIT_MPAG, g_hInstance, NULL);

                    SendMessage(hwndEdit[i], WM_SETFONT, (WPARAM) g_hFont, TRUE);
                }

                hwndStatus = CreateWindowEx(0, STATUSCLASSNAME, _T(""), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
                    hwndMain, (HMENU) STATUSBARID, g_hInstance, NULL);
                SendMessage(hwndStatus, SB_SETTEXT, 0, (LPARAM) _T("就绪"));

                SetFocus(hwndEdit[0]);
            }
            break;

        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
            case BN_CLICKED:
                switch (LOWORD(wParam))
                {
                case IDM_SETTING:
                    DoModel(hwndMain);
                    if (g_fEndModel)
                    {
                        SetWindowPos(hwndMain, g_sd.bMainDlgTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
                            0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                        formattype.type = (StringAndDouble::Type)(g_sd.iShowPatten - IDC_RD_FIXEDDIGIT);
                        formattype.length = g_sd.iLength;
                        formattype.digits = g_sd.iDigit;
                        LONG_PTR style = GetWindowLongPtr(hwndMain, GWL_EXSTYLE);
                        style = g_sd.bMainDlgTopmost ? style |= WS_EX_TOPMOST : style &= ~WS_EX_TOPMOST;
                        SetWindowLongPtr(hwndMain, GWL_EXSTYLE, style);
                    }
                    break;

                case IDM_COPY:
                    if (GetWindowText(GetFocus(), szBuff, 31)) {
                        i = GetDlgCtrlID(GetFocus());
                        if (i >= IDC_EDIT_MPAG && i <= IDC_EDIT_KPA) {
                            input = pressure.value[i - IDC_EDIT_MPAG];
                            iColoredID = i;
                            CopyText(input, GetDlgItem(hwndMain, STATUSBARID));
                            InvalidateRect(GetDlgItem(hwndMain, i), NULL, TRUE);
                        }
                    }
                    break;

                case IDM_PASTE:
                    i = GetDlgCtrlID(GetFocus());
                    if (i >= IDC_EDIT_MPAG && i <= IDC_EDIT_KPA) {
                        PasteText(GetDlgItem(hwndMain, i));
                    }

                    break;
                }
                break;

            case EN_CHANGE:
                switch (LOWORD(wParam))
                {
                case IDC_EDIT_MPAG:
                    if (GetFocus() == (HWND) lParam) {
                        GetDlgItemText(hwndMain, LOWORD(wParam), szBuff, 31);
                        if (StringAndDouble::StrtoD(szBuff, input)) {
                            pressure.calc(MPag, input);

                            if (pressure.value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[1], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_KPAG, szBuff);

                            if (pressure.value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[2], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_MMHG, szBuff);

                            if (pressure.value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[3], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_KPA, szBuff);
                        } else {
                            SetDlgItemText(hwndMain, IDC_EDIT_KPAG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_MMHG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_KPA, TEXT(""));
                        }
                    }
                    break;

                case IDC_EDIT_KPAG:
                    if (GetFocus() == (HWND) lParam) {
                        GetDlgItemText(hwndMain, LOWORD(wParam), szBuff, 31);
                        if (StringAndDouble::StrtoD(szBuff, input)) {
                            pressure.calc(kPag, input);

                            if (pressure.value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[0], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_MPAG, szBuff);

                            if (pressure.value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[2], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_MMHG, szBuff);

                            if (pressure.value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[3], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_KPA, szBuff);
                        } else {
                            SetDlgItemText(hwndMain, IDC_EDIT_MPAG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_MMHG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_KPA, TEXT(""));
                        }
                    }
                    break;

                case IDC_EDIT_MMHG:
                    if (GetFocus() == (HWND) lParam) {
                        GetDlgItemText(hwndMain, LOWORD(wParam), szBuff, 31);
                        if (StringAndDouble::StrtoD(szBuff, input)) {
                            pressure.calc(mmHg, input);

                            if (pressure.value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[0], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_MPAG, szBuff);

                            if (pressure.value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[1], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_KPAG, szBuff);

                            if (pressure.value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[3], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_KPA, szBuff);
                        } else {
                            SetDlgItemText(hwndMain, IDC_EDIT_MPAG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_KPAG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_KPA, TEXT(""));
                        }
                    }
                    break;

                case IDC_EDIT_KPA:
                    if (GetFocus() == (HWND) lParam) {
                        GetDlgItemText(hwndMain, LOWORD(wParam), szBuff, 31);
                        if (StringAndDouble::StrtoD(szBuff, input)) {
                            pressure.calc(kPa, input);

                            if (pressure.value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[0], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_MPAG, szBuff);

                            if (pressure.value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[1], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_KPAG, szBuff);

                            if (pressure.value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
                            else StringAndDouble::DtoFormatStr(pressure.value[2], szBuff, formattype);
                            SetDlgItemText(hwndMain, IDC_EDIT_MMHG, szBuff);
                        } else {
                            SetDlgItemText(hwndMain, IDC_EDIT_MPAG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_KPAG, TEXT(""));
                            SetDlgItemText(hwndMain, IDC_EDIT_MMHG, TEXT(""));
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

            break;

        case WM_CTLCOLOREDIT:
            i = GetWindowLongPtr((HWND) lParam, GWL_ID);
            if (i >= IDC_EDIT_MPAG && i <= IDC_EDIT_KPA) {
                if (i == iColoredID) {
                    SetBkColor((HDC) wParam, EDITCOPIEDCOLOR);
                    return (INT_PTR) hbrEditCopied;
                }
                else {
                    SetBkColor((HDC) wParam, EDITNORMALCOLOR);
                    return (INT_PTR) hbrEditNormal;
                }
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwndMain, &ps);

            SetBkMode(hdc, TRANSPARENT);
            SelectObject(hdc, g_hFont);
            for (i = 0; i < 4; ++i)
                TextOut(hdc, 10, 10 + i * 30 + 3, szLabel[i], lstrlen(szLabel[i])); 
            SelectObject(hdc, GetStockObject(SYSTEM_FONT));

            EndPaint(hwndMain,&ps);

            break;

        case WM_DESTROY:
            DeleteObject(g_hFont);
            DeleteObject(hbrEditNormal);
            DeleteObject(hbrEditCopied);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwndMain, uMsg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK DlgProcSetting(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    BOOL is_fixedlength_enable;
    static SettingData sd; 


    switch (uMsg) {
        case WM_CREATE:
            {
                HWND hwndGrpPatten, hwndGrpFormat;
                HWND hwndRdoFixedDigit, hwndRdoFixedLength, hwndChkTopMost;
                HWND hwndEdtDigit, hwndEdtLength;
                HWND hwndBtnOK, hwndBtnCanel;

                CenterWindow(hDlg, 258, 194);

                //GroupBox
                hwndGrpPatten = CreateWindowEx(0, _T("Button"), _T("显示方式"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX,
                    8, 10, 115, 80, hDlg, NULL, g_hInstance, NULL);
                SendMessage(hwndGrpPatten, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                hwndGrpFormat = CreateWindowEx(0, _T("Button"), _T("显示格式"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX,
                    131, 10, 115, 80, hDlg, NULL, g_hInstance, NULL);
                SendMessage(hwndGrpFormat, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                //RadioBox
                hwndRdoFixedDigit = CreateWindowEx(0, _T("Button"), _T("固定位数"),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON | WS_GROUP,
                    15, 30, 80, 20, hDlg, (HMENU) IDC_RD_FIXEDDIGIT, g_hInstance, NULL);
                SendMessage(hwndRdoFixedDigit, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                hwndRdoFixedLength = CreateWindowEx(0, _T("Button"), _T("固定长度"),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON,
                    15, 60, 80, 20, hDlg, (HMENU) IDC_RD_FIXEDLENGTH, g_hInstance, NULL);
                SendMessage(hwndRdoFixedLength, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                //Edit
                hwndEdtDigit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), _T(""),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER,
                    195, 30, 40, 22, hDlg, (HMENU) IDC_EDIT_DIGIT, g_hInstance, NULL);
                SendMessage(hwndEdtDigit, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                hwndEdtLength = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Edit"), _T(""),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER,
                    195, 60, 40, 22, hDlg, (HMENU) IDC_EDIT_LENGTH, g_hInstance, NULL);
                SendMessage(hwndEdtLength, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                //CheckBox
                hwndChkTopMost = CreateWindowEx(0, _T("Button"), _T("窗口顶置显示"),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
                    8, 100, 150, 20, hDlg, (HMENU) IDC_CHK_ONTOP, g_hInstance, NULL);
                SendMessage(hwndChkTopMost, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                //Button
                hwndBtnOK = CreateWindowEx(0, _T("Button"), _T("确定"),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                    45, 130, 75, 25, hDlg, (HMENU) IDC_BTN_OK, g_hInstance, NULL);
                SendMessage(hwndBtnOK, WM_SETFONT, (WPARAM) g_hFont, TRUE);
                hwndBtnCanel = CreateWindowEx(0, _T("Button"), _T("取消"),
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                    134, 130, 75, 25, hDlg, (HMENU) IDC_BTN_CANCEL, g_hInstance, NULL);
                SendMessage(hwndBtnCanel, WM_SETFONT, (WPARAM) g_hFont, TRUE);

                sd = g_sd;  //初始化时传递数据

                CheckRadioButton(hDlg, IDC_RD_FIXEDDIGIT, IDC_RD_FIXEDLENGTH, sd.iShowPatten);
                CheckDlgButton(hDlg, IDC_CHK_ONTOP, sd.bMainDlgTopmost);
                SetDlgItemInt(hDlg, IDC_EDIT_DIGIT, sd.iDigit, FALSE);
                SetDlgItemInt(hDlg, IDC_EDIT_LENGTH, sd.iLength, FALSE);

                is_fixedlength_enable = BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_RD_FIXEDDIGIT),
                    BM_GETCHECK, 0, 0);

                EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LENGTH), !is_fixedlength_enable);

                SetFocus(GetDlgItem(hDlg, sd.iShowPatten));

            }

            
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {	
            case IDC_BTN_OK:
                sd.iDigit = GetDlgItemInt(hDlg, IDC_EDIT_DIGIT, FALSE, FALSE);
                sd.iLength = GetDlgItemInt(hDlg, IDC_EDIT_LENGTH, FALSE, FALSE);
                sd.bMainDlgTopmost = IsDlgButtonChecked(hDlg, IDC_CHK_ONTOP);
                g_sd = sd;
                g_fEndModel = TRUE;
                DestroyWindow(hDlg);
                break;

            case IDC_BTN_CANCEL:
                g_fEndModel = FALSE;
                DestroyWindow(hDlg);
                break;

            case IDC_RD_FIXEDDIGIT:
            case IDC_RD_FIXEDLENGTH:
                sd.iShowPatten = LOWORD(wParam);
                CheckRadioButton(hDlg, IDC_RD_FIXEDDIGIT, IDC_RD_FIXEDLENGTH, sd.iShowPatten);

                is_fixedlength_enable = BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_RD_FIXEDDIGIT),
                    BM_GETCHECK, 0, 0);

                EnableWindow(GetDlgItem(hDlg, IDC_EDIT_LENGTH), !is_fixedlength_enable);

                break;
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hDlg, &ps);
            SetBkMode(hdc, TRANSPARENT);
            SelectObject(hdc, g_hFont);
            TextOut(hdc, 138, 32, _T("小数位数:"), 5);
            TextOut(hdc, 138, 62, _T("数字长度:"), 5);
            SelectObject(hdc, GetStockObject(SYSTEM_FONT));
            EndPaint(hDlg, &ps);

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hDlg, uMsg, wParam, lParam);
    }

    return 0;
}

void CenterWindow(HWND hwnd, int nWidth, int nHeight)
{
    RECT rcWnd, rcParent, rcWorkArea;
    int X, Y;
    HWND hwndParent;

    GetWindowRect(hwnd, &rcWnd);
    SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID) &rcWorkArea, 0);

    if (nWidth == 0) nWidth = rcWnd.right - rcWnd.left;
    else if (nWidth > rcWorkArea.right - rcWorkArea.left) nWidth = rcWorkArea.right - rcWorkArea.left;
    if (nHeight == 0) nHeight = rcWnd.bottom - rcWnd.top;
    else if (nHeight > rcWorkArea.bottom - rcWorkArea.top) nHeight = rcWorkArea.bottom - rcWorkArea.top;

    if ((hwndParent = GetParent(hwnd)) != NULL)
        GetWindowRect(hwndParent, &rcParent);
    else
        CopyRect(&rcParent, &rcWorkArea);

    X = (rcParent.right - rcParent.left) / 2 - nWidth / 2 + rcParent.left;
    Y = (rcParent.bottom - rcParent.top) / 2 - nHeight / 2 + rcParent.top;

    if (X < rcWorkArea.left) X = rcWorkArea.left;
    if (Y < rcWorkArea.top) Y = rcWorkArea.top;
    if (X + nWidth > rcWorkArea.right) X = rcWorkArea.right - nWidth;
    if (Y + nHeight > rcWorkArea.bottom) Y = rcWorkArea.bottom - nHeight;

    MoveWindow(hwnd, X, Y, nWidth, nHeight, FALSE);
}

void DoModel(HWND hwndParent)
{
    HWND hDlg;
    MSG msg;
    HMENU hSysMenu;

    EnableWindow(hwndParent, FALSE);

    hDlg = CreateWindowEx(0, szSettingDlgClsName, _T("设置"), WS_POPUP | WS_CAPTION,
        0, 0, 0, 0, hwndParent, NULL,
        (HINSTANCE) GetWindowLongPtr(hwndParent, GWL_HINSTANCE), NULL);

    ShowWindow(hDlg, SW_SHOW);
    UpdateWindow(hDlg);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EnableWindow(hwndParent, TRUE);

    SetForegroundWindow(hwndParent);
}

void CopyText(double value, HWND hwndStatusBar)
{
    if (OpenClipboard(GetParent(hwndStatusBar)))
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
    if (OpenClipboard(GetParent(hwndEdit))) {
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