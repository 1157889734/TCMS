#if !defined(AFX_TABLECTRL_H__D4A8207F_400D_4C70_B92A_537BB8A1CE20__INCLUDED_)
#define AFX_TABLECTRL_H__D4A8207F_400D_4C70_B92A_537BB8A1CE20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableCtrl window

class CTableCtrl : public CListCtrl
{
// Construction
public:
	CTableCtrl();

// Attributes
public:
	enum eType
	{
		eEdit,
		eCombo,
		eLast	
	};

	CStringList m_strList[22];

// Operations
public:
	void SetColumnCtrl(int nColumnCtrl[])
	{
		memcpy(m_nColumnCtrl,nColumnCtrl,64*sizeof(int));
	}
	void SetComBoxTextCtrl(int nRow, CString sztext);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTableCtrl();

	// Generated message map functions
protected:
	CComboBox * ComboItem( int nItem,  int nSubItem);
	CEdit * EditItem( int nItem, int nSubItem);

	int m_nColumnCtrl[64];

	//{{AFX_MSG(CTableCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLECTRL_H__D4A8207F_400D_4C70_B92A_537BB8A1CE20__INCLUDED_)
