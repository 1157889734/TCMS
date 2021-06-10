// TcmsCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TCMS.h"
#include "TCMSDlg.h"

// CTCMSCtrlDlg dialog

IMPLEMENT_DYNAMIC(CTCMSCtrlDlg, CDialog)

CTCMSCtrlDlg::CTCMSCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCMSCtrlDlg::IDD, pParent)
{
	m_bPause = TRUE;
}

CTCMSCtrlDlg::~CTCMSCtrlDlg()
{
}

void CTCMSCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTCMSCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_START, &CTCMSCtrlDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CTCMSCtrlDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTCMSCtrlDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CTCMSCtrlDlg message handlers

void CTCMSCtrlDlg::OnBnClickedBtnStart()
{
	CString strComName;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_COMSTR))->GetCurSel();
	
	if(-1 == nSel)
	{
		AfxMessageBox(_T("串口参数错误！"),MB_OK|MB_ICONSTOP);
		return;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_COMSTR))->GetLBText(nSel, strComName);

	CString strTemp = strComName.Mid(3, strComName.GetLength()-3);

	if (CTCMSDlg::GetTCMSDlgApp()->OnStartTcms(atoi(strTemp)))
	{
		//BH_StartRunTMS();

		GetDlgItem(IDC_COMBO_COMSTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	}	
}

//停车
void CTCMSCtrlDlg::OnBnClickedBtnPause()
{
	BH_PauseRunTMS(m_bPause);
	
	SetDlgItemText(IDC_BTN_PAUSE, m_bPause?_T("继续"):_T("中止"));

	//CTCMSDlg::GetTCMSDlgApp()->OnPause(m_bPause);
	
	m_bPause = !m_bPause;	
}

void CTCMSCtrlDlg::OnBnClickedBtnStop()
{
	CTCMSDlg::GetTCMSDlgApp()->OnStopTcms();
// 	SetDlgItemText(IDC_EDIT_RECV_DATE, _T(""));
// 	SetDlgItemText(IDC_EDIT_SEND_DATE, _T(""));
	GetDlgItem(IDC_COMBO_COMSTR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
}

BOOL CTCMSCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HKEY hKEY;
	DWORD dIndex = 0; 
	TCHAR strKey[MAX_PATH] = {0};
	TCHAR strValue[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	DWORD type = REG_SZ;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), 0, KEY_READ, &hKEY) == ERROR_SUCCESS)
	{
		while (RegEnumValue(hKEY, dIndex, strKey, &dwLen, NULL, NULL, (LPBYTE)strValue, &dwLen) != ERROR_NO_MORE_ITEMS)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_COMSTR))->InsertString(dIndex, strValue);
			dIndex++;
			dwLen = MAX_PATH;
		}
		RegCloseKey(hKEY);
	}

	

	((CComboBox*)GetDlgItem(IDC_COMBO_COMSTR))->SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
