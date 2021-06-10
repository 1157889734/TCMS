#if !defined(AFX_EDITITEM_H__0DAAD41C_AD65_4DB0_B880_B1F5DB1E65D7__INCLUDED_)
#define AFX_EDITITEM_H__0DAAD41C_AD65_4DB0_B880_B1F5DB1E65D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditItem window

class CEditItem : public CEdit
{
// Construction
public:
	CEditItem(int nItem, int nSubItem, CString &sContent);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditItem();

	// Generated message map functions
protected:
	int		m_nItem;
	int		m_nSubItem;
	CString m_sContent;
	BOOL	m_bVK_ESCAPE;

	//{{AFX_MSG(CEditItem)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITITEM_H__0DAAD41C_AD65_4DB0_B880_B1F5DB1E65D7__INCLUDED_)
