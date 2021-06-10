#if !defined(AFX_COMBOITEM_H__6386B01E_23F0_4551_9AF7_153DC7166A51__INCLUDED_)
#define AFX_COMBOITEM_H__6386B01E_23F0_4551_9AF7_153DC7166A51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboItem window

class CComboItem : public CComboBox
{
// Construction
public:
	CComboItem( int nItem, int nSubItem, CStringList *psList = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboItem();

	// Generated message map functions
protected:
	CStringList m_sList;
	int m_nSubItem;
	int m_nItem;
	BOOL m_bVK_ESCAPE;

	//{{AFX_MSG(CComboItem)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOITEM_H__6386B01E_23F0_4551_9AF7_153DC7166A51__INCLUDED_)
