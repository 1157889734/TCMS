// ComboItem.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"
#include "ComboItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboItem

CComboItem::CComboItem( int nItem, int nSubItem, CStringList *psList)
{
	m_nItem		= nItem;
	m_nSubItem	= nSubItem;

	if ( psList )
		m_sList.AddTail( psList );

	m_bVK_ESCAPE = FALSE;
}

CComboItem::~CComboItem()
{
}


BEGIN_MESSAGE_MAP(CComboItem, CComboBox)
	//{{AFX_MSG_MAP(CComboItem)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboItem message handlers

BOOL CComboItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)	
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);			
			return 1;
		}	
	}	
	
	return CComboBox::PreTranslateMessage(pMsg);
}

int CComboItem::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont* font = GetParent()->GetFont();	
	SetFont(font);
	//add the items from CStringlist
	POSITION pos = m_sList.GetHeadPosition();
	while(pos != NULL)
		AddString((LPCTSTR)(m_sList.GetNext(pos)));	
	SetFocus();	
	
	return 0;
}

void CComboItem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_ESCAPE || nChar == VK_RETURN)	
	{		
		if( nChar == VK_ESCAPE)
			m_bVK_ESCAPE = 1;
		GetParent()->SetFocus();		
		return;	
	}	
	
	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CComboItem::OnCloseup() 
{
	// TODO: Add your control notification handler code here
	GetParent()->SetFocus();	
	
}

void CComboItem::OnKillFocus(CWnd* pNewWnd) 
{
	int nIndex = GetCurSel();

	CComboBox::OnKillFocus(pNewWnd);

	CString str;	
	GetWindowText(str);
	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispinfo;
	lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
	lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;	
	lvDispinfo.item.iItem = m_nItem;
	lvDispinfo.item.iSubItem = m_nSubItem;
	lvDispinfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispinfo.item.cchTextMax = str.GetLength();
	lvDispinfo.item.lParam = GetItemData(GetCurSel());
	if(nIndex!=CB_ERR)
		GetParent()->GetParent()->SendMessage(
			WM_NOTIFY,
			GetParent()->GetDlgCtrlID(),
			(LPARAM)&lvDispinfo);
	PostMessage(WM_CLOSE);
}

void CComboItem::OnNcDestroy() 
{
	CComboBox::OnNcDestroy();
	
	// TODO: Add your message handler code here
	delete this;	
	
}


void CComboItem::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	ASSERT(lpMeasureItemStruct->CtlType == ODT_COMBOBOX);    
	if (lpMeasureItemStruct->itemID != (UINT) -1)
	{       
 		LPCTSTR lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
 		ASSERT(lpszText != NULL);
 		CSize   sz;       
 		CDC*    pDC = GetDC();
 		sz = pDC->GetTextExtent(lpszText);
 		ReleaseDC(pDC);         
		lpMeasureItemStruct->itemHeight = 2*sz.cy;    
	} 
}

BOOL CComboItem::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���

	//cs.style |= CBS_OWNERDRAWVARIABLE|CBS_DROPDOWN;

	return CComboBox::PreCreateWindow(cs);
}
