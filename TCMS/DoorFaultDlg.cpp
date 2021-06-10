// TcmsDoor.cpp : implementation file
//

#include "stdafx.h"
#include "TCMS.h"
#include "DoorFaultDlg.h"

#define BTN_DOOR_ID	1200
// CDoorFaultDlg dialog

IMPLEMENT_DYNAMIC(CDoorFaultDlg, CDialog)

CDoorFaultDlg::CDoorFaultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoorFaultDlg::IDD, pParent)
{

}

CDoorFaultDlg::~CDoorFaultDlg()
{
}

void CDoorFaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDoorFaultDlg, CDialog)
END_MESSAGE_MAP()


// CDoorFaultDlg message handlers

BOOL CDoorFaultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

#define BTN_WIDHT 120
#define BTN_HEIGHT 40
	
	char szTxtCar[6][32]={_T("A1"), _T("B1"), _T("C1"), _T("C2"), _T("B2"), _T("A2")};
	int nIndexID=0;
	for (int i=0; i<6; i++)
	{
		for (int j=0; j<8; j++)//门紧急解锁
		{
			CString strTxt;
			strTxt.Format("%s 车 %d 门",szTxtCar[i], j+1);
						
			m_btnDoor[i][j].Create(strTxt, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, 
				CRect(20+i*BTN_WIDHT+5,20+j*(BTN_HEIGHT-10)+10,BTN_WIDHT+i*BTN_WIDHT,BTN_HEIGHT+j*(BTN_HEIGHT-10)+10),
				this, BTN_DOOR_ID+nIndexID);

			m_btnDoor[i][j].SetFont(GetFont());
		}
		for(int j=0; j<5; j++)//探测器火警
		{
			CString strTxt;
			strTxt.Format("%s 车 %d 门",szTxtCar[i], j+1);

			m_btnDoor[i][j+8].Create(strTxt, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
				CRect(20+i*BTN_WIDHT+5,20+(j+9)*(BTN_HEIGHT-10)+5, BTN_WIDHT+i*BTN_WIDHT,BTN_HEIGHT+(j+9)*(BTN_HEIGHT-10)),
				this, BTN_DOOR_ID+nIndexID);

			m_btnDoor[i][j+8].SetFont(GetFont());
		}
		nIndexID++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDoorFaultDlg::GetDlgBtnCheck(int nCar, int nDoor)
{
	return m_btnDoor[nCar][nDoor].GetCheck();
}