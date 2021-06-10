// TableCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"
#include "TableCtrl.h"
#include "ComboItem.h"
#include "EditItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableCtrl

CTableCtrl::CTableCtrl()
{
	for( int i = 0; i<64; i++ )
	{
		m_nColumnCtrl[i] = eLast;
	}	
}

CTableCtrl::~CTableCtrl()
{
}


BEGIN_MESSAGE_MAP(CTableCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTableCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableCtrl message handlers

BOOL CTableCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CHILD |LVS_REPORT |
				LVS_SINGLESEL | LVS_SHOWSELALWAYS |CBS_OWNERDRAWVARIABLE|CBS_DROPDOWN ;	

	return CListCtrl::PreCreateWindow(cs);
}

int CTableCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	SetExtendedStyle( GetExtendedStyle() | LVS_EX_CHECKBOXES );
	
	return 0;
}

void CTableCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( GetFocus() != this )
	{
		SetFocus();
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (  pNMListView->iSubItem >= 0 && m_nColumnCtrl[pNMListView->iSubItem] == eCombo )
		ComboItem(pNMListView->iItem, pNMListView->iSubItem);
	else if (  pNMListView->iSubItem >= 0 && m_nColumnCtrl[pNMListView->iSubItem] == eEdit )
		EditItem (pNMListView->iItem, pNMListView->iSubItem);

	*pResult = 0;
}

void CTableCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
 	LV_ITEM *plvItem = &plvDispInfo->item;

	if( plvItem->iItem != -1 &&  // valid item
		plvItem->pszText )		// valid text
	{
		SetItemText( plvItem->iItem, plvItem->iSubItem, plvItem->pszText);				
		
		// this will invoke an ItemChanged handler in parent
		if ( plvItem->iSubItem != 0 )
		{
			TCHAR/*char*/ szText[0x10+1];
			GetItemText( plvItem->iItem, 0, szText, 0x10);				
			SetItemText( plvItem->iItem, 0, szText);				
		}	
	}

	*pResult = 0;
}

void CTableCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	switch(pLVKeyDow->wVKey)
	{
		case VK_DELETE: 
			{
				int nItem = GetSelectionMark();
				if(nItem!=-1) // valid item 					
				{
					DeleteItem( nItem );
				}
			}	break;

		default :break;
	}
	*pResult = 0;
}

CEdit *CTableCtrl::EditItem(int nItem, int nSubItem)
{
	if (!GetCheck(nItem))
	{
		return NULL;
	}

	CRect rect;
	int offset = 0;
	// 确保该子项可见
	if(!EnsureVisible(nItem, TRUE))
	{ 
		return NULL;	
	}

	SetItemState(nItem, 0, LVIS_SELECTED);  


	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	// 为了能完全显示该子项需要进行滚动
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size(offset + rect.left,0);		
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;

	// 获得列的对齐格式	
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nSubItem, &lvcol);

	DWORD dwStyle;
	if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;	

	
	dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	CString szContent = GetItemText(nItem, nSubItem);
	CEdit *pEdit = new CEditItem(nItem, nSubItem, szContent/*GetItemText(nItem, nSubItem)*/);

	#define IDC_EDITCTRL 0x2222
	pEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);	
	pEdit->SetWindowText(szContent);
	return pEdit;
}

CComboBox * CTableCtrl::ComboItem(int nItem, int nSubItem)
{
	#define IDC_COMBOBOXINLISTVIEW 0x1111  // 定义组合框ID
	
// 	if (!GetCheck(nItem))
// 	{
// 		return NULL;
// 	}

	CString strFind = GetItemText(nItem, nSubItem);

	//basic code start
	CRect rect;
	int offset = 0;
	// 确保该子项可见
	if( !EnsureVisible(nItem, TRUE)) 
		return NULL;

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	// 为了能完全显示该子项需要进行滚动
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;
	//basic code end

	rect.bottom += 10 * rect.Height();//下拉区域
	
	
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL|CBS_DROPDOWNLIST;
	CComboBox *pList = new CComboItem(nItem, nSubItem, &m_strList[nItem]);
	// 创建组合框
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	// 显示下拉列表部分
	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));
	// The returned pointer should not be saved
	return pList;
}


void CTableCtrl::SetComBoxTextCtrl(int nRow, CString sztext)
{
	m_strList[nRow].AddTail(sztext);
}

void CTableCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);    
	LVITEM lvi = {0}; 
	lvi.mask = LVIF_STATE;//|LVIF_IMAGE; 
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED ; 
	lvi.iItem = lpDrawItemStruct->itemID; 
	BOOL bGet = GetItem(&lvi); 
	//高亮显示
	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)||((lvi.state & LVIS_SELECTED) && 
		((GetFocus() == this)|| (GetStyle() & LVS_SHOWSELALWAYS)))); 
	// 画文本背景 
	CRect rcBack = lpDrawItemStruct->rcItem; 
	pDC->SetBkMode(TRANSPARENT); 
	if( 0)//bHighlight ) //如果被选中
	{ 
		pDC->SetTextColor(RGB(255,255,255)); //文本为白色
		pDC->FillRect(rcBack, &CBrush(RGB(49,106,179))); 
	} 
	else 
	{ 
		pDC->SetTextColor(RGB(0,100,100));       //文本为黑色
		pDC->FillRect(rcBack, &CBrush(RGB(255,255,255))); 
	} 
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) 
	{ 
		//写文本 
		CString szText; 
		int nCollumn = GetHeaderCtrl()->GetItemCount();//列数
		for (int i = 0; i < nCollumn; i++) 
		{ //循环得到文本 
			CRect rcItem; 
			if ( !GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem )) 
				continue; 
			szText = GetItemText( lpDrawItemStruct->itemID, i ); 
			rcItem.left += 5; rcItem.right -= 1; 
			DWORD dwStyle = (DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			if (i==2)
				dwStyle = (LVCFMT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			pDC->DrawText(szText, lstrlen(szText), &rcItem,  dwStyle);
		} 
	} 
}

void CTableCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle( 0,LVS_OWNERDRAWFIXED );
	CListCtrl::PreSubclassWindow();
}

void CTableCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	int m_nRowHeight=30;
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

void CTableCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}