#include "stdafx.h"
#include "CalcUI.h"
#include "resource.h"
#include "Calculation.h"
#include "StringAndDouble.h"

#pragma warning(disable: 4996)

CalcUIApp theApp;

//////////////////////////////////////////////////////////////////////////
// CalcUIApp

BOOL CalcUIApp::InitInstance()
{
    m_pMainWnd = new CMainWindow;
    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CMainWindow

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDM_SETTING, &CMainWindow::OnSetting)
    ON_BN_CLICKED(IDM_COPY, &CMainWindow::OnCopy)
    ON_BN_CLICKED(IDM_PASTE, &CMainWindow::OnPaste)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_MPAG, IDC_EDIT_KPA,
                     &CMainWindow::OnCalculation)
    ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_MPAG, IDC_EDIT_KPA,
                     &CMainWindow::OnFocus)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

CMainWindow::CMainWindow() : m_iColoredId(-1)
{
    m_hbrEditNormal.CreateSysColorBrush(COLOR_WINDOW);
    m_hbrEditCopied.CreateSolidBrush(RGB(0, 255, 255));

    CreateEx(0, NULL, _T("压力计算"), WS_OVERLAPPEDWINDOW,
        0, 0, 231, 201, NULL, NULL);
    CenterWindow();

    m_pPressure = new Calculation;

    m_sd.bMainDlgTopmost = TRUE;
    m_sd.iDigit = 6;
    m_sd.iLength = 0;
    m_sd.iShowPatten = IDC_RD_FIXEDDIGIT;
}

CMainWindow::~CMainWindow()
{
    delete m_pPressure;
}

void CMainWindow::PostNcDestroy()
{
    delete this;
}

BOOL CMainWindow::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &= ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
    cs.lpszClass = AfxRegisterWndClass(0, NULL,
        (HBRUSH) (COLOR_3DFACE + 1), 0);

    return CWnd::PreCreateWindow(cs);
}

int CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == CWnd::OnCreate(lpCreateStruct))
        return -1;
    
    CClientDC dc(this);
    LOGFONT lf = {0};
    lf.lfHeight = -MulDiv(8, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
    lf.lfWeight = 400;
    lstrcpy(lf.lfFaceName, _T("MS Shell Dlg"));

    m_font.CreateFontIndirect(&lf);

    if (m_menuMain.CreateMenu()) {
        m_menuMain.AppendMenu(MF_STRING, IDM_SETTING, _T("设置(&S)"));
        m_menuMain.AppendMenu(MF_STRING, IDM_COPY, _T("复制(&C)"));
        m_menuMain.AppendMenu(MF_STRING, IDM_PASTE, _T("粘贴(&V)"));
        SetMenu(&m_menuMain);
    }

    TCHAR *szLabel[4] = {_T("MPa (表)"),
                         _T("kPa (真)"),
                         _T("mm汞柱"),
                         _T("kPa (绝)")};
    CRect rect;

    for (int i = 0; i < 4; ++i) {
        // 一定要用CreateEx()，否则即使用ModifyStyleEx()也无法得到三维效果
        rect.SetRect(60, 10+i*30, 60+153, 10+i*30+23);
        m_editValue[i].CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
            rect, this, i+IDC_EDIT_MPAG);

        rect.SetRect(10, 10+i*30+3, 57, 10+i*30+23);
        m_staticLable[i].Create(szLabel[i], WS_CHILD | WS_VISIBLE | SS_LEFT,
            rect, this);

        m_editValue[i].SetFont(&m_font);
        m_staticLable[i].SetFont(&m_font);
    }

    GetClientRect(rect);
    m_statusMain.CreateEx(this);
    UINT indicate = STATUSBARID;
    m_statusMain.SetIndicators(&indicate, 1);
    m_statusMain.SetPaneInfo(0, indicate, SBPS_STRETCH, 0);
    m_statusMain.MoveWindow(0, rect.bottom-20, rect.Width(), 20);
    m_statusMain.SetPaneText(0, _T("就绪"));

    m_editValue[0].SetFocus();
    SetWindowPos(FromHandle(m_sd.bMainDlgTopmost ? HWND_TOPMOST : HWND_NOTOPMOST),
        0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    
    return 0;
}

void CMainWindow::OnSetting()
{
    EnableWindow(FALSE);

    CSettingWindow settingWnd(this);
    settingWnd.ShowWindow(SW_SHOW);
    settingWnd.UpdateWindow();

    MSG *pMsg = AfxGetCurrentMessage();
    while (::GetMessage(pMsg, NULL, 0, 0)) {
        ::TranslateMessage(pMsg);
        ::DispatchMessage(pMsg);
    }

    int nExitCode = settingWnd.GetExitWindowCode();
    if (IDOK == nExitCode) {
        m_sd = settingWnd.GetSettingData();

        SetWindowPos(FromHandle(m_sd.bMainDlgTopmost ? HWND_TOPMOST : HWND_NOTOPMOST),
            0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }


    EnableWindow(TRUE);
    SetForegroundWindow();
}

void CMainWindow::OnCopy()
{
    int id = GetFocus()->GetDlgCtrlID();
    if (id >= IDC_EDIT_MPAG && id <= IDC_EDIT_KPA) {

        if (!OpenClipboard()) return;
        EmptyClipboard();
        HANDLE hClip = ::GlobalAlloc(GHND, 31*sizeof(TCHAR));
        TCHAR *pBuff = (TCHAR*) ::GlobalLock(hClip);
        StringAndDouble::DtoWholeStr(m_pPressure->value[id-IDC_EDIT_MPAG], pBuff);
        ::GlobalUnlock(hClip);
        SetClipboardData(CF_TCHARTEXT, hClip);
        CloseClipboard();

        m_statusMain.SetPaneText(0, _T("已复制值：")+CString(pBuff));
        m_iColoredId = id;
        GetDlgItem(id)->Invalidate();
    }
}

void CMainWindow::OnPaste()
{
    int id = GetFocus()->GetDlgCtrlID();
    if (id >= IDC_EDIT_MPAG && id <= IDC_EDIT_KPA) {
        if (!OpenClipboard()) return;
        if (IsClipboardFormatAvailable(CF_TCHARTEXT)) {
            HANDLE hClip = GetClipboardData(CF_TCHARTEXT);
            TCHAR *pBuff = (TCHAR*) ::GlobalLock(hClip);
            GetDlgItem(id)->SetWindowText(pBuff);
            ::GlobalUnlock(hClip);
            CloseClipboard();
        }
    }
}

void CMainWindow::OnCalculation(UINT Id)
{
    StringAndDouble::FormatType formatType = {
        (StringAndDouble::Type) (m_sd.iShowPatten - IDC_RD_FIXEDDIGIT),
        m_sd.iLength, m_sd.iDigit
    };
    TCHAR szBuff[31] = {0};
    double input = 0.0;

    switch (Id) {
    case IDC_EDIT_MPAG:
        if (GetFocus() != GetDlgItem(IDC_EDIT_MPAG)) return;

        GetDlgItemText(IDC_EDIT_MPAG, szBuff, 31);
        if (StringAndDouble::StrtoD(szBuff, input)) {
            m_pPressure->calc(MPag, input);

            if (m_pPressure->value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[1], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_KPAG, szBuff);

            if (m_pPressure->value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[2], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_MMHG, szBuff);

            if (m_pPressure->value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[3], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_KPA, szBuff);
        } else {
            SetDlgItemText(IDC_EDIT_KPAG, TEXT(""));
            SetDlgItemText(IDC_EDIT_MMHG, TEXT(""));
            SetDlgItemText(IDC_EDIT_KPA, TEXT(""));
        }
        break;

    case IDC_EDIT_KPAG:
        if (GetFocus() != GetDlgItem(IDC_EDIT_KPAG)) return;

        GetDlgItemText(IDC_EDIT_KPAG, szBuff, 31);
        if (StringAndDouble::StrtoD(szBuff, input)) {
            m_pPressure->calc(kPag, input);

            if (m_pPressure->value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[0], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_MPAG, szBuff);

            if (m_pPressure->value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[2], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_MMHG, szBuff);

            if (m_pPressure->value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[3], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_KPA, szBuff);
        } else {
            SetDlgItemText(IDC_EDIT_MPAG, TEXT(""));
            SetDlgItemText(IDC_EDIT_MMHG, TEXT(""));
            SetDlgItemText(IDC_EDIT_KPA, TEXT(""));
        }
        break;

    case IDC_EDIT_MMHG:
        if (GetFocus() != GetDlgItem(IDC_EDIT_MMHG)) return;

        GetDlgItemText(IDC_EDIT_MMHG, szBuff, 31);
        if (StringAndDouble::StrtoD(szBuff, input)) {
            m_pPressure->calc(mmHg, input);

            if (m_pPressure->value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[0], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_MPAG, szBuff);

            if (m_pPressure->value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[1], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_KPAG, szBuff);

            if (m_pPressure->value[3] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[3], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_KPA, szBuff);
        } else {
            SetDlgItemText(IDC_EDIT_MPAG, TEXT(""));
            SetDlgItemText(IDC_EDIT_KPAG, TEXT(""));
            SetDlgItemText(IDC_EDIT_KPA, TEXT(""));
        }
        break;

    case IDC_EDIT_KPA:
        if (GetFocus() != GetDlgItem(IDC_EDIT_KPA)) return;

        GetDlgItemText(IDC_EDIT_KPA, szBuff, 31);
        if (StringAndDouble::StrtoD(szBuff, input)) {
            m_pPressure->calc(kPa, input);

            if (m_pPressure->value[0] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[0], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_MPAG, szBuff);

            if (m_pPressure->value[1] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[1], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_KPAG, szBuff);

            if (m_pPressure->value[2] >= NA) _tcscpy(szBuff, OUTOFFRANGE);
            else StringAndDouble::DtoFormatStr(m_pPressure->value[2], szBuff, formatType);
            SetDlgItemText(IDC_EDIT_MMHG, szBuff);
        } else {
            SetDlgItemText(IDC_EDIT_MPAG, TEXT(""));
            SetDlgItemText(IDC_EDIT_KPAG, TEXT(""));
            SetDlgItemText(IDC_EDIT_MMHG, TEXT(""));
        }
        break;

    default:
        break;
    }
}

void CMainWindow::OnFocus(UINT Id)
{
    switch (Id) {
       case IDC_EDIT_MPAG:
           m_iColoredId = IDC_EDIT_MPAG == m_iColoredId ? IDC_EDIT_MPAG : -1; break;
       case IDC_EDIT_KPAG:
           m_iColoredId = IDC_EDIT_KPAG == m_iColoredId ? IDC_EDIT_KPAG : -1; break;
       case IDC_EDIT_MMHG:
           m_iColoredId = IDC_EDIT_MMHG == m_iColoredId ? IDC_EDIT_MMHG : -1; break;
       case IDC_EDIT_KPA:
           m_iColoredId = IDC_EDIT_KPA == m_iColoredId ? IDC_EDIT_KPA : -1; break;
       default:
           m_iColoredId = -1; break;
    }
}

HBRUSH CMainWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    int id = pWnd->GetDlgCtrlID();
    if (id >= IDC_EDIT_MPAG && id <= IDC_EDIT_KPA) {
        LOGBRUSH lbr = {0};
        if (id == m_iColoredId) {
            m_hbrEditCopied.GetLogBrush(&lbr);
            pDC->SetBkColor(lbr.lbColor);
            return (HBRUSH) m_hbrEditCopied;
        } else {
            m_hbrEditNormal.GetLogBrush(&lbr);
            pDC->SetBkColor(lbr.lbColor);
            return (HBRUSH) m_hbrEditNormal;
        }
    }

    return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

//////////////////////////////////////////////////////////////////////////
// CSettingWindow

BEGIN_MESSAGE_MAP(CSettingWindow, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RD_FIXEDDIGIT, IDC_BTN_CANCEL,
    &CSettingWindow::OnCtrlClicked)
END_MESSAGE_MAP()

CSettingWindow::CSettingWindow(CWnd *pWndParent) : m_exitCode(IDCANCEL)
{
    LPCTSTR lpClassName = AfxRegisterWndClass(0, NULL,
        (HBRUSH) (COLOR_3DFACE+1));
    CreateEx(0, lpClassName, _T("设置"), WS_POPUP | WS_CAPTION
        | WS_SYSMENU, 0, 0, 258, 194,
        pWndParent->GetSafeHwnd(), NULL);
    CenterWindow(pWndParent);
}

int CSettingWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == CWnd::OnCreate(lpCreateStruct))
        return -1;

    CRect rect;
    
    // GroupBox
    rect.SetRect(8, 10, 115+8, 80+10);
    m_groupPatten.Create(_T("显示方式"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX,
        rect, this, IDC_GRP_PATTEN);
    rect.SetRect(131, 10, 115+131, 80+10);
    m_groupFormat.Create(_T("显示格式"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX,
        rect, this, IDC_GRP_FORMAT);

    // RadioBox
    rect.SetRect(15, 30, 80+15, 20+30);
    m_radioFixedDigit.Create(_T("固定位数"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON | WS_GROUP,
        rect, this, IDC_RD_FIXEDDIGIT);
    rect.SetRect(15, 60, 80+15, 20+60);
    m_radioFixedLength.Create(_T("固定长度"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON,
        rect, this, IDC_RD_FIXEDLENGTH);

    // Static
    rect.SetRect(138, 32, 193, 22+32);
    m_staticDigit.Create(_T("小数位数:"), WS_CHILD | WS_VISIBLE | SS_LEFT,
        rect, this);
    rect.SetRect(138, 62, 193, 22+62);
    m_staticLength.Create(_T("数字长度:"), WS_CHILD | WS_VISIBLE | SS_LEFT,
        rect, this);

    // Edit
    rect.SetRect(195, 30, 40+195, 22+30);
    m_editDigit.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER | WS_GROUP,
        rect, this, IDC_EDIT_DIGIT);
    rect.SetRect(195, 60, 40+195, 22+60);
    m_editLength.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER,
        rect, this, IDC_EDIT_LENGTH);

    // CheckBox
    rect.SetRect(8, 100, 150+8, 20+100);
    m_checkTopMost.Create(_T("窗口顶置显示"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
        rect, this, IDC_CHK_ONTOP);

    // Button
    rect.SetRect(45, 130, 75+45, 25+130);
    m_btnOk.Create(_T("确定"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        rect, this, IDC_BTN_OK);
    rect.SetRect(134, 130, 75+134, 25+130);
    m_btnCancel.Create(_T("取消"),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
        rect, this, IDC_BTN_CANCEL);

    // 设置控件字体
    CFont *pFont = &static_cast<CMainWindow*>(GetParent())->m_font;
    for (int i = IDC_RD_FIXEDDIGIT; i <= IDC_GRP_FORMAT; ++i) {
        GetDlgItem(i)->SetFont(pFont);
    }
    m_staticDigit.SetFont(pFont);
    m_staticLength.SetFont(pFont);

    // 初始化控件数据
    m_sd = static_cast<CMainWindow*>(GetParent())->m_sd;
    CheckRadioButton(IDC_RD_FIXEDDIGIT, IDC_RD_FIXEDLENGTH, m_sd.iShowPatten);
    m_staticLength.EnableWindow(IDC_RD_FIXEDLENGTH == m_sd.iShowPatten);
    m_editLength.EnableWindow(IDC_RD_FIXEDLENGTH == m_sd.iShowPatten);
    CheckDlgButton(IDC_CHK_ONTOP, m_sd.bMainDlgTopmost);

    SetDlgItemInt(IDC_EDIT_DIGIT, m_sd.iDigit, FALSE);
    SetDlgItemInt(IDC_EDIT_LENGTH, m_sd.iLength, FALSE);

    return 0;
}

void CSettingWindow::OnDestroy()
{
    CWnd::OnDestroy();
    PostQuitMessage(m_exitCode);
}

void CSettingWindow::OnCtrlClicked(UINT id)
{
    switch (id) {
        case IDC_RD_FIXEDDIGIT:
        case IDC_RD_FIXEDLENGTH:
            m_sd.iShowPatten = id;
            m_staticLength.EnableWindow(IDC_RD_FIXEDLENGTH == id);
            m_editLength.EnableWindow(IDC_RD_FIXEDLENGTH == id);
            break;

        case IDC_BTN_OK:
            m_sd.iDigit = GetDlgItemInt(IDC_EDIT_DIGIT, NULL, FALSE);
            m_sd.iLength = GetDlgItemInt(IDC_EDIT_LENGTH, NULL, FALSE);
            m_sd.bMainDlgTopmost = IsDlgButtonChecked(IDC_CHK_ONTOP);
            m_exitCode = IDOK;
            OnDestroy();
            break;

        case IDC_BTN_CANCEL:
            OnDestroy();
        
        default:
            break;
    }
}