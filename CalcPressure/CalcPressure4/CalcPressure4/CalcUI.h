#ifndef __CALCPRESSURE4_CALCUI_H__
#define __CALCPRESSURE4_CALCUI_H__

#include <afxwin.h>
#include <afxext.h>

class CSettingWindow;
class Calculation;

typedef struct
{
    int iShowPatten;           //С��λ����ʾ��ʽ
    int iDigit, iLength;       //С��λ�������ֳ���
    BOOL bMainDlgTopmost;      //�������Ƿ�ΪTopmost��ʽ
} SettingData, *pSettingData;

class CalcUIApp : public CWinApp
{
public:
    virtual BOOL InitInstance();
};

class CMainWindow : public CWnd
{
public:
    friend class CSettingWindow;
    CMainWindow();
    ~CMainWindow();

private:
    DECLARE_MESSAGE_MAP()
#define OUTOFFRANGE _T("������Χ")

    virtual void PostNcDestroy();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetting();
    afx_msg void OnCopy();
    afx_msg void OnPaste();
    afx_msg void OnCalculation(UINT Id);
    afx_msg void OnFocus(UINT Id);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
    CFont m_font;
    CBrush m_hbrEditNormal;
    CBrush m_hbrEditCopied;
    int m_iColoredId;
    Calculation *m_pPressure;
    SettingData m_sd;

    CEdit m_editValue[4];
    CStatic m_staticLable[4];
    CMenu m_menuMain;
    CStatusBar m_statusMain;
};

class CSettingWindow : public CWnd
{
public:
    explicit CSettingWindow(CWnd *pWndParent = NULL);
    int GetExitWindowCode() const { return m_exitCode; }
    const SettingData & GetSettingData() const { return m_sd; }

private:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnCtrlClicked(UINT id);

private:
    CButton m_groupPatten;
    CButton m_groupFormat;
    CButton m_radioFixedDigit;
    CButton m_radioFixedLength;
    CStatic m_staticDigit;
    CStatic m_staticLength;
    CEdit   m_editDigit;
    CEdit   m_editLength;
    CButton m_checkTopMost;
    CButton m_btnOk;
    CButton m_btnCancel;

    SettingData m_sd;
    int m_exitCode;
};

#endif //__CALCPRESSURE4_CALCUI_H__
