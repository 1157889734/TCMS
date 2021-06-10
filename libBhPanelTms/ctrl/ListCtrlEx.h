#pragma once


// CListCtrlEx
#define IDC_PROGRESS_LIST WM_USER

class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

	// the array of the Progress Controls in the list control
	CArray<CProgressCtrl*,CProgressCtrl*> m_ProgressList;
	// the column which should contain the progress bars
	int m_ProgressColumn;

	int m_iUpdateColumn;
	int m_iCurrentRow;

	int m_iTopIndex;
	BOOL	m_bIsFullDisplay;
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
	// initialize the column containing the bars
	void InitProgressColumn(int ColNum=0);
	void SetProgressValueColumn(int iColumn);

	int GetCurrentProgressRow();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void SetDisplayMode(BOOL bFullDisplay);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


