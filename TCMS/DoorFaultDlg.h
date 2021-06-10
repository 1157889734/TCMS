#pragma once


// CDoorFaultDlg dialog

class CDoorFaultDlg : public CDialog
{
	DECLARE_DYNAMIC(CDoorFaultDlg)

public:
	CDoorFaultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDoorFaultDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TCMS_DORR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CButton m_btnDoor[6][13];

	BOOL GetDlgBtnCheck(int nCar, int nDoor);
};
