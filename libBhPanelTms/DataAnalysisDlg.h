#pragma once
#include "DlgDataView.h"
#include "afxwin.h"

// CDataAnalysisDlg dialog

class CDataAnalysisDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataAnalysisDlg)

public:
	CDataAnalysisDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataAnalysisDlg();

// Dialog Data
	enum { IDD = IDD_DLG_DATA_ANALYSIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDlgDataView* m_pDataViewDlg;

	virtual BOOL OnInitDialog();
	void InitDataView();

	CStatic m_stcSendViewInfo;
};
