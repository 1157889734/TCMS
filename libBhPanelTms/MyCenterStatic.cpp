// MyCenterStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MyCenterStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCenterStatic

CMyCenterStatic::CMyCenterStatic()
{
	m_strCenterText.Empty();
}

CMyCenterStatic::~CMyCenterStatic()
{
}


BEGIN_MESSAGE_MAP(CMyCenterStatic, CStatic)
	//{{AFX_MSG_MAP(CMyCenterStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCenterStatic message handlers

void CMyCenterStatic::SetCenterText(CString strText)
{
	m_strCenterText = strText;
	CRect Rect;
	GetWindowRect(&Rect);
	GetParent()->ScreenToClient(&Rect);
	GetParent()->InvalidateRect(&Rect);
}

void CMyCenterStatic::OnPaint() 
{
	CPaintDC dc1(this); // device context for painting
	
	// TODO: Add your message handler code here
	CFont *pOldfont;
	CFont font;
	CFont fontTime;
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	int oldmode=dc.SetBkMode(TRANSPARENT);

	font.CreateFont(30, //这个数字就是字体大小了
					0,0,0,FW_NORMAL,
					FALSE,FALSE,
					0,
					ANSI_CHARSET, // nCharSet
					OUT_DEFAULT_PRECIS, // nOutPrecision
					CLIP_DEFAULT_PRECIS, // nClipPrecision
					DEFAULT_QUALITY, // nQuality
					FIXED_PITCH | FF_SWISS, "Arial");
	pOldfont = dc.SelectObject(&font);
	dc1.SetBkColor(RGB(0, 0, 255));
	dc.SetTextColor(RGB(0, 0, 255));
	dc.DrawText(m_strCenterText, &rect, DT_CENTER|DT_WORDBREAK);

	dc.SelectObject(pOldfont);
	dc.SetBkMode(oldmode);
	// Do not call CStatic::OnPaint() for painting messages
}
