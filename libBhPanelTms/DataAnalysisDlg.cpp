// DataAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "DataAnalysisDlg.h"


// CDataAnalysisDlg dialog

IMPLEMENT_DYNAMIC(CDataAnalysisDlg, CDialog)

CDataAnalysisDlg::CDataAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataAnalysisDlg::IDD, pParent)
{
	m_pDataViewDlg = NULL;
}

CDataAnalysisDlg::~CDataAnalysisDlg()
{
	if(NULL != m_pDataViewDlg)
	{
		m_pDataViewDlg->DestroyWindow();
		delete m_pDataViewDlg;
		m_pDataViewDlg = NULL;
	}
}

void CDataAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_SENDVIEW_INFO, m_stcSendViewInfo);
}


BEGIN_MESSAGE_MAP(CDataAnalysisDlg, CDialog)
END_MESSAGE_MAP()


// CDataAnalysisDlg message handlers

BOOL CDataAnalysisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

 	InitDataView();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataAnalysisDlg::InitDataView(void)
{
	if(NULL == m_pDataViewDlg)
	{
		CRect rt;
		GetClientRect(&rt);
		rt.left += 5;
		rt.top += 5;
		rt.right -= 180;
		rt.bottom -=10;
		m_pDataViewDlg = new CDlgDataView;
		m_pDataViewDlg->CreateEx(NULL, NULL, NULL, WS_CHILD|WS_VISIBLE, rt, this, IDD_DLG_DATA_ANALYSIS, NULL);
		m_pDataViewDlg->ShowWindow(SW_SHOW);
	}
 	m_pDataViewDlg->InitDataView(8,16,8);
}