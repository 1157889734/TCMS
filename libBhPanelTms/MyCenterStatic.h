#if !defined(AFX_MYCENTERSTATIC_H__7D3CD235_E5F4_48BD_A100_FAC0C6D54A37__INCLUDED_)
#define AFX_MYCENTERSTATIC_H__7D3CD235_E5F4_48BD_A100_FAC0C6D54A37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCenterStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCenterStatic window

class CMyCenterStatic : public CStatic
{
// Construction
public:
	CMyCenterStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCenterStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCenterText(CString strText);
	virtual ~CMyCenterStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCenterStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CString m_strCenterText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCENTERSTATIC_H__7D3CD235_E5F4_48BD_A100_FAC0C6D54A37__INCLUDED_)
