#pragma once


// CEditEx

class CEditEx : public CEdit
{
	DECLARE_DYNAMIC(CEditEx)

public:
	CEditEx();
	virtual ~CEditEx();

protected:
	DECLARE_MESSAGE_MAP()
	static CBrush m_bkBrush;
	COLORREF m_bkColor;
	static CPen m_framePen;
	COLORREF m_frameColor;
public:
	afx_msg HBRUSH CtlColor(CDC* , UINT /*nCtlColor*/);
	void SetbkColor(COLORREF bkColor);
	void SetFrameColor(COLORREF frameColor);

public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


