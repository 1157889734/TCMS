#pragma once


// CTCMSCtrlDlg dialog

class CTCMSCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CTCMSCtrlDlg)

public:
	CTCMSCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTCMSCtrlDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bPause;
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();

public:
	virtual BOOL OnInitDialog();
};
