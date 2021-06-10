#pragma once


// CDlgDataView 对话框

class CDlgDataView;
typedef void (__stdcall* LPDataViewCallback)(DWORD dwItem, /*CDlgDataView*/void* pView, void *pParam);

typedef struct structViewCell{
	CRect rc;
	BYTE val;
	bool bfresh;
	bool bhighlight;
	structViewCell(){
		memset(this, 0, sizeof(structViewCell));
	};
}ViewCell;

class CDlgDataView : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataView)

public:
	CDlgDataView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataView();

	enum { IDD = IDD_DIALOG_DATAVIEW };
	
	void InitDataView(UINT uRow, UINT uColumn, UINT uColPerGroup);
	void ReleaseDataView(void );
	void UpdateDataView(const BYTE *pData, UINT uLen);
	bool CheckDataBytes(DWORD dwLen);
	BYTE GetDataByte(DWORD dwItem) const;
	void SetDataAnalysisCallback(LPDataViewCallback pf, void *pParam);

	BOOL GetElementRect(CRect *pRC);
	void SetElementHighlight(UINT uIndex);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	void InitMemoryDC(CDC *pDC, const CRect &rc);
	void ReleaseMemoryDC(void );
	void InitRawDC(CDC *pDC, const CRect &rc);
	void ReleaseRawDC(void );

private:
	CDC *m_pMemDC;
	CDC *m_pRawDC;
	COLORREF m_clBkg;
	COLORREF m_clText;
	CFont m_fontHeader;
	CFont m_fontElement;

	UINT m_uCellNum;
	UINT m_uMaxCellNum;
	CSize m_sizeCell;

	BOOL m_bDataViewUpdated;
	ViewCell *m_pColumns;
	ViewCell *m_pRows;
	ViewCell *m_pElements;
	UINT m_uColumn;
	UINT m_uRow;
	UINT m_uColPerGroup;
	LPDataViewCallback m_pCallback;
	void *m_pCallbackParam;
};
