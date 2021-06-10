// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)
CListCtrlEx::CListCtrlEx() : m_ProgressColumn(0)
{
	m_iCurrentRow = 0;
	m_iTopIndex = -1;
	m_bIsFullDisplay = FALSE;
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()



// CListCtrlEx message handlers


void CListCtrlEx::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages
	int Top=GetTopIndex();
	int Total=GetItemCount();
	int PerPage=GetCountPerPage();
	int LastItem=((Top+PerPage)>Total)?Total:Top+PerPage;


// 	CString szItemCount = _T("");
// 	szItemCount.Format(_T("TopIndex: %d, Total: %d, PerPage: %d, LastItem; %d\r\n"), Top, Total, PerPage, LastItem);
// 	OutputDebugString(szItemCount);

	if (!m_bIsFullDisplay)
	{
		CHeaderCtrl *pHeader = GetHeaderCtrl();
		for (int i = Top; i < LastItem; i++)
		{
			CRect ColRt;
			pHeader->GetItemRect(m_ProgressColumn,&ColRt);
			CRect rt;
			GetItemRect(i, &rt, LVIR_LABEL);
			//rt.top+=2;
			rt.top += 2 + ColRt.top;
			rt.bottom-=2;
			rt.left+=ColRt.left;
			int Width=ColRt.Width();
			rt.right=rt.left+Width-4;

			CProgressCtrl *pControl = NULL;
			if ((m_ProgressList.GetCount() == PerPage + 1) && (m_ProgressList.GetCount() > 0))
			{
				pControl = m_ProgressList.GetAt(i - Top);
			}

			if (NULL == pControl)
			{
				pControl=new CProgressCtrl();
				pControl->Create(NULL,rt,this,IDC_PROGRESS_LIST+i);

				pControl->ShowWindow(SW_SHOWNORMAL);
				m_ProgressList.Add(pControl);
			}
			pControl->SetPos(0);


			CString	szFileLength = _T("");
			szFileLength = GetItemText(i, 2);
			int iFileLength  = _ttoi(szFileLength.GetBuffer(szFileLength.GetLength()));
			pControl->SetRange32(0, iFileLength);


			if ((m_iUpdateColumn == i))
			{
				CString Data=GetItemText(i,4);
				int Percent=  _ttoi(Data.GetBuffer(Data.GetLength()));//atoi(Data);
				m_iCurrentRow = i;


				CString	szFileLength = _T("");
				if (m_iTopIndex != Top)
				{
					szFileLength = GetItemText(m_iUpdateColumn, 2);
					int iFileLength  = _ttoi(szFileLength.GetBuffer(szFileLength.GetLength()));
					pControl->SetRange32(0, iFileLength);

					m_iTopIndex = Top;
				}

				pControl->SetPos(Percent);

				CString szItem = GetItemText(m_iUpdateColumn, 5);
				if ((szItem.Compare(_T("完成")) == 0) || (szItem.Compare(_T("已下载")) == 0))
				{
					pControl->SetRange32(0, 100);
					pControl->SetPos(100);
				} 
			}
			else
			{
				CString szItem = GetItemText(i, 5);
				if ((szItem.Compare(_T("完成")) == 0) || (szItem.Compare(_T("已下载")) == 0))
				{
					pControl->SetRange32(0, 100);
					pControl->SetPos(100);
				} 
				else
				{
					CString Data=GetItemText(i,4);
					if (Data.Compare(_T("0")) != 0)
					{
						SetItemText(i, 4, _T("0"));
					}
					pControl->SetPos(0);
				}

			}
		}
	}
	CListCtrl::OnPaint();
}

void CListCtrlEx::InitProgressColumn(int ColNum/*=0*/)
{
	m_ProgressColumn=ColNum;
}

void CListCtrlEx::SetProgressValueColumn(int iColumn)
{
	m_iUpdateColumn = iColumn;

	int Count=(int)m_ProgressList.GetCount();
	for(int i=0;i<Count;i++)
	{
		CProgressCtrl* pControl=m_ProgressList.GetAt(0);
		pControl->DestroyWindow();
		m_ProgressList.RemoveAt(0);
	}
}

int CListCtrlEx::GetCurrentProgressRow()
{
	return m_iCurrentRow;
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::SetDisplayMode(BOOL bFullDisplay)
{
	m_bIsFullDisplay = bFullDisplay;
}

void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	
}

void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
}

BOOL CListCtrlEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CListCtrl::PreCreateWindow(cs);
}
