// DlgDataView.cpp : 实现文件
//

#include "stdafx.h"
// #include "TCMSFor26Cars.h"
#include "DlgDataView.h"




// CDlgDataView 对话框

const UINT G_CELL_GAP = 0; // G_CELL_GAP 
const UINT G_VIEW_BORDER = 1; // G_VIEW_BORDER

const UINT INT_MAX_RANKS = 999;
const UINT IDC_VIEW_ITEM_BASE = 1000;

IMPLEMENT_DYNAMIC(CDlgDataView, CDialog)

CDlgDataView::CDlgDataView(CWnd* pParent /*=NULL*/)
: CDialog(CDlgDataView::IDD, pParent)
{
	m_pMemDC = NULL;
	m_pRawDC = NULL;

	m_pColumns = NULL;
	m_pRows = NULL;
	m_pElements = NULL;
	m_pCallback = NULL;

	m_uCellNum = 0;
	m_uMaxCellNum = 0;

	m_bDataViewUpdated = FALSE;
	m_uColumn = 0;
	m_uRow = 0;
	m_uColPerGroup = 0;
	ZeroMemory(&m_sizeCell, sizeof(m_sizeCell));
}

CDlgDataView::~CDlgDataView()
{
	ReleaseMemoryDC();
	ReleaseRawDC();
	ReleaseDataView();
}

void CDlgDataView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDataView, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_MOUSELEAVE, &CDlgDataView::OnMouseLeave)
END_MESSAGE_MAP()


// CDlgDataView 消息处理程序

BOOL CDlgDataView::OnInitDialog()
{
	CDialog::OnInitDialog();

	EnableToolTips(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataView::InitMemoryDC(CDC *pDC, const CRect &rc)
{
	m_pMemDC = new CDC;

	CBitmap bitmapMem;
	m_pMemDC->CreateCompatibleDC(this->GetDC());
	bitmapMem.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap *pOldBmp = m_pMemDC->SelectObject(&bitmapMem);

	/* draw background image */
	m_pMemDC->BitBlt(0, 0, rc.Width(), rc.Height(), pDC, 0, 0, SRCCOPY);

 	/* draw window edge */
 	CPen newPen(PS_SOLID, 1, RGB(102, 102, 102));
 	CPen *pOldPen = (CPen *)m_pMemDC->SelectObject(newPen);
 	m_pMemDC->MoveTo(0, 0);
 	m_pMemDC->LineTo(0, rc.Height() - 1);
 	m_pMemDC->LineTo(rc.Width() - 1, rc.Height() - 1);
 	m_pMemDC->LineTo(rc.Width() - 1, 0);
 	m_pMemDC->LineTo(0, 0);
 
 	newPen.DeleteObject();
 	bitmapMem.DeleteObject();

	m_pMemDC->SetBkMode(TRANSPARENT);
}

void CDlgDataView::InitRawDC(CDC *pDC, const CRect &rc)
{
	m_pRawDC = new CDC;

	CBitmap bitmapMem;
	m_pRawDC->CreateCompatibleDC(this->GetDC());
	bitmapMem.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap *pOldBmp = m_pRawDC->SelectObject(&bitmapMem);

	/* draw background image */
	m_pRawDC->BitBlt(0, 0, rc.Width(), rc.Height(), pDC, 0, 0, SRCCOPY);

	bitmapMem.DeleteObject();
}

void CDlgDataView::ReleaseMemoryDC(void )
{
	if (NULL != m_pMemDC)
	{
		m_pMemDC->DeleteDC();
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

void CDlgDataView::ReleaseRawDC(void )
{
	if (NULL != m_pRawDC)
	{
		m_pRawDC->DeleteDC();
		delete m_pRawDC;
		m_pRawDC = NULL;
	}
}

BOOL CDlgDataView::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	
	if (NULL == m_pMemDC)
	{
		InitMemoryDC(pDC, rc);
		InitRawDC(pDC, rc);
	}

 	CString strVal;

	if (m_bDataViewUpdated)
	{
		// TommyTest
		m_pMemDC->SelectObject(m_fontElement);
		//m_pMemDC->SelectObject(m_fontHeader);
		/* draw column header text */
		CRect rcColumn(G_VIEW_BORDER, G_VIEW_BORDER, rc.Width() - 2 * G_VIEW_BORDER, m_pColumns[0].rc.Height());
		m_pMemDC->FillSolidRect(rcColumn, RGB(180, 180, 255));
		for (UINT n = 0; n < m_uColumn; n++)
		{
			strVal.Format(_T("%02d"), n);
			m_pMemDC->DrawText(strVal, m_pColumns[n].rc, DT_CENTER | DT_VCENTER);
		}

		/* draw row header text */
		CRect rcRow(G_VIEW_BORDER, G_VIEW_BORDER, m_pRows[0].rc.Width(), rc.Height() - 2 * G_VIEW_BORDER);
		m_pMemDC->FillSolidRect(rcRow, RGB(180, 180, 255));
		for (UINT n = 0; n < m_uRow; n++)
		{
			strVal.Format(_T("%02X"), n);
			m_pMemDC->DrawText(strVal, m_pRows[n].rc, DT_CENTER | DT_VCENTER);
		}
		m_bDataViewUpdated = FALSE;
	}
 
	m_pMemDC->SelectObject(m_fontElement);
	for (UINT n = 0; n < m_uColumn * m_uRow; n++)
 	{
		if (m_pElements[n].bfresh)
		{
			CRect *pRc = &(m_pElements[n].rc);
			if (m_pElements[n].bhighlight)
			{
				m_pMemDC->FillSolidRect(pRc, RGB(180, 180, 180));
			}
			else
			{
				m_pMemDC->BitBlt(pRc->left, pRc->top, pRc->Width(), pRc->Height(), 
					m_pRawDC, pRc->left, pRc->top, SRCCOPY);
			}
			strVal.Format(_T("%02X"), m_pElements[n].val);
			m_pMemDC->DrawText(strVal, m_pElements[n].rc, DT_CENTER | DT_VCENTER);
			m_pElements[n].bfresh = 0;
		}
 	}

 	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), m_pMemDC, 0, 0, SRCCOPY);

	return true;
}

void CDlgDataView::InitDataView(UINT uRow, UINT uColumn, UINT uColPerGroup)
{
	ASSERT(uRow < INT_MAX_RANKS);
	ASSERT(uColumn < INT_MAX_RANKS);

	m_uRow = uRow;
	m_uColumn = uColumn;
	m_uMaxCellNum = m_uRow * m_uColumn;

	CRect rc;
	GetClientRect(&rc);

	if (0 == uColPerGroup) uColPerGroup = uColumn;
	int nGroupNum = uColumn / uColPerGroup + uColumn % uColPerGroup;
	int nGapNum = nGroupNum == 0 ? 0 : (nGroupNum - 1);

	m_sizeCell.cx = (rc.Width() - G_VIEW_BORDER * 2) * 2 / ((uColumn + 1) * 2 + nGapNum);
	m_sizeCell.cy = (rc.Height() - G_VIEW_BORDER * 2) / (uRow + 1);

 	/* initialize font */
	m_fontHeader.DeleteObject();
 	LOGFONT lfh;
 	memset(&lfh, 0, sizeof(lfh));
	TCHAR *pFontName = _T("黑体");
	lfh.lfHeight = m_sizeCell.cy * 5 / 6;
 	_tcscpy_s(lfh.lfFaceName, sizeof(lfh.lfFaceName) / sizeof(TCHAR), pFontName);
 	VERIFY(m_fontHeader.CreateFontIndirect(&lfh));

	m_fontElement.DeleteObject();
	LOGFONT lfe;
	memset(&lfe, 0, sizeof(lfe));
	lfh.lfHeight = m_sizeCell.cy * 5 / 6;
	_tcscpy_s(lfe.lfFaceName, sizeof(lfe.lfFaceName) / sizeof(TCHAR), pFontName);
  	VERIFY(m_fontElement.CreateFontIndirect(&lfe));


	/* recreate members */
	ReleaseDataView();
	
	m_pColumns = new ViewCell[uColumn];
	m_pRows = new ViewCell[uRow];
	m_pElements = new ViewCell[uColumn * uRow];

	/* create column headers */
	for (UINT n = 0; n < uColumn; n++)
	{
		int nGroupGap = n / uColPerGroup;
		int nGapWidth = nGroupGap * m_sizeCell.cx / 2;
		m_pColumns[n].rc.top = 0 + G_CELL_GAP + G_VIEW_BORDER;
		m_pColumns[n].rc.bottom = m_sizeCell.cy - G_CELL_GAP + G_VIEW_BORDER;
		m_pColumns[n].rc.left = (1 + n) * m_sizeCell.cx + G_CELL_GAP + nGapWidth + G_VIEW_BORDER;
		m_pColumns[n].rc.right = (2 + n) * m_sizeCell.cx - G_CELL_GAP + nGapWidth + G_VIEW_BORDER;
	}

	/* create row headers */
	for (UINT n = 0; n < uRow; n++)
	{
		m_pRows[n].rc.top = (1 + n) * m_sizeCell.cy + G_CELL_GAP + G_VIEW_BORDER;
		m_pRows[n].rc.bottom = (2 + n) * m_sizeCell.cy - G_CELL_GAP + G_VIEW_BORDER;
		m_pRows[n].rc.left = 0 + G_CELL_GAP + G_VIEW_BORDER;
		m_pRows[n].rc.right = m_sizeCell.cx - G_CELL_GAP + G_VIEW_BORDER;
	}

	/* create items */
	for (UINT c = 0; c < uColumn; c++)
	{
		int nGroupGap = c / uColPerGroup;
		int nGapWidth = nGroupGap * m_sizeCell.cx / 2;
		for (UINT r = 0; r < uRow; r++)
		{
			m_pElements[m_uColumn * r + c].rc.top = (1 + r) * m_sizeCell.cy + G_CELL_GAP + G_VIEW_BORDER;
			m_pElements[m_uColumn * r + c].rc.bottom = (2 + r) * m_sizeCell.cy - G_CELL_GAP + G_VIEW_BORDER;
			m_pElements[m_uColumn * r + c].rc.left = (1 + c) * m_sizeCell.cx + G_CELL_GAP + nGapWidth + G_VIEW_BORDER;
 			m_pElements[m_uColumn * r + c].rc.right = (2 + c) * m_sizeCell.cx - G_CELL_GAP + nGapWidth + G_VIEW_BORDER;
			m_pElements[m_uColumn * r + c].bfresh = TRUE;
		}
	}

	m_bDataViewUpdated = TRUE;
	RedrawWindow();
}

void CDlgDataView::ReleaseDataView(void )
{
	if (NULL != m_pColumns)
	{
		delete [] m_pColumns;
		m_pColumns = NULL;
	}
	if (NULL != m_pRows)
	{
		delete [] m_pRows;
		m_pRows = NULL;
	}
	if (NULL != m_pElements)
	{
		delete []m_pElements;
		m_pElements = NULL;
	}
}

void CDlgDataView::SetDataAnalysisCallback(LPDataViewCallback pf, void *pParam)
{
	m_pCallback = pf;
	m_pCallbackParam = pParam;
}

void CDlgDataView::UpdateDataView(const BYTE *pData, UINT uLen)
{
	for (UINT n = 0; n < uLen; n++)
	{
		if (pData[n] != m_pElements[n].val)
		{
			m_pElements[n].bfresh = 1;
			m_pElements[n].val = pData[n];
			
			if(m_pElements[n].bhighlight && m_pCallback)
			{
				m_pCallback(n, this, m_pCallbackParam);
			}
		}
	}
	for (UINT n = uLen; n < m_uMaxCellNum; n++)
	{
		if (m_pElements[n].val != 0)
		{
			m_pElements[n].bfresh = 1;
			m_pElements[n].val = 0;
		}
	}

	m_uCellNum = uLen;
	RedrawWindow();
}

bool CDlgDataView::CheckDataBytes(DWORD dwLen)
{
	return (NULL != m_pElements && dwLen <= m_uMaxCellNum);
}

BYTE CDlgDataView::GetDataByte(DWORD dwItem) const
{
	ASSERT(NULL != m_pElements && dwItem < m_uMaxCellNum);
	return m_pElements[dwItem].val;
}

HBRUSH CDlgDataView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CDlgDataView::OnDestroy()
{
	CDialog::OnDestroy();

	ReleaseMemoryDC();
}

void CDlgDataView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (NULL == m_pCallback)
		return;

	int nIndex = -1;
	for (UINT m = 0; m < m_uRow; m++)
	{
		for (UINT n = 0; n < m_uColumn; n++)
		{
			bool res = (m_pElements[m * m_uColumn + n].rc.PtInRect(point) == TRUE);
			if (res != m_pElements[m * m_uColumn + n].bhighlight)
			{
				m_pElements[m * m_uColumn + n].bhighlight = res;
				m_pElements[m * m_uColumn + n].bfresh = true;
			}

			if (res)
			{
				nIndex = (m * m_uColumn + n);
			}
		}
	}

	if (nIndex >= 0)
	{
		m_pCallback(nIndex, this, m_pCallbackParam);
	}

	RedrawWindow();
	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CDlgDataView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	for (UINT m = 0; m < m_uRow; m++)
	{
		for (UINT n = 0; n < m_uColumn; n++)
		{
			m_pElements[m * m_uColumn + n].bhighlight = false;
			m_pElements[m * m_uColumn + n].bfresh = TRUE;
		}
	}

	m_pCallback(-1, this, m_pCallbackParam);

	RedrawWindow();
	return 0;
}

BOOL CDlgDataView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	TRACKMOUSEEVENT mouseEvent;
	mouseEvent.cbSize = sizeof(mouseEvent);
	mouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	mouseEvent.dwHoverTime = HOVER_DEFAULT;
	mouseEvent.hwndTrack = m_hWnd;
	_TrackMouseEvent(&mouseEvent);

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CDlgDataView::GetElementRect(CRect *pRC)
{
	return FALSE;
}

void CDlgDataView::SetElementHighlight(UINT uIndex)
{
	if (uIndex > m_uColumn * m_uRow)
		return;
	
	m_pElements[uIndex].bhighlight = true;
	m_pElements[uIndex].bfresh = true;
}
