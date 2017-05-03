// EditEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx() : m_bkColor(GetSysColor(COLOR_WINDOW)), m_frameColor(NULL_PEN)
{
	m_bkBrush.CreateSolidBrush(m_bkColor);
	m_framePen.CreatePen(PS_SOLID, 1, m_frameColor);
}

CEditEx::~CEditEx()
{
	m_bkBrush.DeleteObject();
	m_framePen.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CEditEx ��Ϣ�������



HBRUSH CEditEx::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻طǿջ���

	pDC->SetBkColor(m_bkColor);

	return m_bkBrush;
}

void CEditEx::SetbkColor(COLORREF bkColor)
{
	m_bkColor = bkColor;
	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(m_bkColor);
	Invalidate();
}

void CEditEx::SetFrameColor(COLORREF frameColor)
{
	m_frameColor  = frameColor;
	m_framePen.DeleteObject();
	m_framePen.CreatePen(PS_SOLID, 1, frameColor);
	Invalidate();
}


void CEditEx::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������
	SetFrameColor(RGB(0, 0, 255));
}

void CEditEx::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
	SetFrameColor(NULL_PEN);
}

BOOL CEditEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_frameColor == NULL_PEN)
	{
		return TRUE;
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);
		rc.InflateRect(1, 1);
		HBRUSH pOldBrush = (HBRUSH)pDC->SelectStockObject(NULL_BRUSH);
		CPen *pOldPen = pDC->SelectObject(&m_framePen);
		pDC->Rectangle(&rc);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}

	return TRUE;
	//return CEdit::OnEraseBkgnd(pDC);
}
