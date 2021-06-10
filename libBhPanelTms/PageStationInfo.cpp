// PageStationInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "PageStationInfo.h"
#include "PanelMainFram.h"

extern CPanelMainFram *g_pMainFrame;

// CPageStationInfo 对话框


//Add by Johnny in 2017-08-01 18:55:21
//由成员变量改完静态成员变量
std::vector<StationInfo> CPageStationInfo::m_vtManageStationInfo;

IMPLEMENT_DYNAMIC(CPageStationInfo, CDialog)

CPageStationInfo::CPageStationInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CPageStationInfo::IDD, pParent)
{
	for (int i=0; i<STATION_ITEM_MAX; i++)
	{
		m_pItembuf[i] = NULL;
	}
}

CPageStationInfo::~CPageStationInfo()
{
	for (int i=0; i<STATION_ITEM_MAX; i++)
	{
		if (m_pItembuf[i])
		{
			free(m_pItembuf[i]);
			m_pItembuf[i] = NULL;
		}
	}
}

void CPageStationInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATION, m_listStation);
}


BEGIN_MESSAGE_MAP(CPageStationInfo, CDialog)
	ON_BN_CLICKED(IDC_BTN_S_ADD, &CPageStationInfo::OnBnClickedBtnSAdd)
	ON_BN_CLICKED(IDC_BTN_S_SAVE, &CPageStationInfo::OnBnClickedBtnSSave)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STATION, &CPageStationInfo::OnNMClickListStation)
	ON_BN_CLICKED(IDC_BTN_S_MODFIY, &CPageStationInfo::OnBnClickedBtnSModfiy)
	ON_BN_CLICKED(IDC_BTN_S_DEL, &CPageStationInfo::OnBnClickedBtnSDel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_HAND, &CPageStationInfo::OnBnClickedRadioHand)
	ON_BN_CLICKED(IDC_RADIO_AUTO, &CPageStationInfo::OnBnClickedRadioAuto)

END_MESSAGE_MAP()


// CPageStationInfo 消息处理程序

BOOL CPageStationInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化	
	
	m_listStation.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|
		LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);

	m_listStation.InsertColumn(0, "NO.", LVCFMT_LEFT, 50);
	m_listStation.InsertColumn(1, "站名称", LVCFMT_LEFT, 150);
	m_listStation.InsertColumn(2, "上行站代码", LVCFMT_LEFT, 110);
	m_listStation.InsertColumn(3, "下行站代码", LVCFMT_LEFT, 110);
	m_listStation.InsertColumn(4, "距离", LVCFMT_LEFT, 110);

	m_listStation.GetWindowRect(m_StationRt);	
	ScreenToClient(m_StationRt);

 	for (int i=0; i<STATION_ITEM_MAX; i++)
 	{
		if (m_pItembuf[i]==NULL)
		{
			m_pItembuf[i]=(BYTE*)malloc(sizeof(StationInfo));
		} 		
 	}
	((CButton *)GetDlgItem(IDC_RADIO_HAND))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageStationInfo::OnRefreshStationData()
{
	if (((CButton *)GetDlgItem(IDC_RADIO_AUTO))->GetCheck())
	{
		m_vtManageStationInfo = g_pMainFrame->m_vtStationInfo;
	}
	else
	{
		m_vtManageStationInfo = g_pMainFrame->m_vtHandStationInfo;
	}

	m_listStation.DeleteAllItems();

	std::vector<StationInfo>::iterator it = m_vtManageStationInfo.begin();
	for(; it!=m_vtManageStationInfo.end(); it++)
	{
		StationInfo pStation = (StationInfo)*it;
		SetListItemtext(pStation);
	}
}

int CPageStationInfo::GetStationOnlyID()
{	
	for (int i=0; i<STATION_ITEM_MAX; i++)
	{
		BOOL bFindId = FALSE;
		std::vector<StationInfo>::iterator it = m_vtManageStationInfo.begin();
		for(; it!=m_vtManageStationInfo.end(); it++)
		{
			StationInfo pStation = (StationInfo)*it;
			if (pStation.nStationId == i)
			{
				bFindId = TRUE;
				break;
			}
		}
		if (!bFindId)
		{
			return i;
		}
	}
	return -1;
}

void CPageStationInfo::OnBnClickedBtnSAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	StationInfo stTempInfo;
	memset(&stTempInfo, 0x00, sizeof(StationInfo));

	CString szTempbuf;
	GetDlgItemText(IDC_EDIT_S_NAME,szTempbuf);
	if ( strcmp(szTempbuf, _T(""))==0 )
	{
		MessageBox(_T("站点名不能为空"), _T("提示"));
		return;
	}

	int nUpCode = GetDlgItemInt(IDC_EDIT_S_UPCODE);
	int nDwonCode = GetDlgItemInt(IDC_EDIT_S_DOWNCODE);
	int nDistance = GetDlgItemInt(IDC_EDIT_S_DISTANCE);

	strcpy_s(stTempInfo.szStationName, szTempbuf);
	stTempInfo.nUpCode = nUpCode;
	stTempInfo.nDownCode = nDwonCode;
	stTempInfo.nDistance = nDistance;
	int nOnlyId = GetStationOnlyID();
	if (nOnlyId==-1)
	{
		MessageBox(_T("无相应空闲站台ID"), _T("提示"));
		return ;
	}
	stTempInfo.nStationId = nOnlyId;

	SetListItemtext(stTempInfo);

	m_vtManageStationInfo.push_back(stTempInfo);
}


void CPageStationInfo::SetListItemtext(StationInfo station)
{
	int nCount = m_listStation.GetItemCount();

	int nrow = m_listStation.InsertItem(nCount, "");

	char szTempbuf[32]={0};
	sprintf_s(szTempbuf,"%d", nrow+1);
	m_listStation.SetItemText(nrow, 0, szTempbuf);
	m_listStation.SetItemText(nrow, 1, station.szStationName);
	sprintf_s(szTempbuf,"%d", station.nUpCode);
	m_listStation.SetItemText(nrow, 2, szTempbuf);
	sprintf_s(szTempbuf,"%d", station.nDownCode);
	m_listStation.SetItemText(nrow, 3, szTempbuf);
	sprintf_s(szTempbuf,"%d", station.nDistance);
	m_listStation.SetItemText(nrow, 4, szTempbuf);

	memcpy(m_pItembuf[station.nStationId], &station, sizeof(StationInfo));
	m_listStation.SetItemData(nrow, (DWORD_PTR)m_pItembuf[station.nStationId]);
}

void CPageStationInfo::OnBnClickedBtnSSave()
{
	if (m_vtManageStationInfo.size()<2)
	{
		MessageBox(_T("线路站点必须大于2个站"), _T("提示"));
		return;
	}
	if (((CButton *)GetDlgItem(IDC_RADIO_AUTO))->GetCheck())
	{
		g_pMainFrame->WriteStationFile(m_vtManageStationInfo);
	}
	else
	{
		g_pMainFrame->WriteStationFile(m_vtManageStationInfo, FALSE);
	}
}

void CPageStationInfo::OnNMClickListStation(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;            
	if(pNMListView->iItem != -1)            
	{
		BYTE *pData = (BYTE *)m_listStation.GetItemData(pNMListView->iItem);
		if (NULL == pData)
			return;

		StationInfo TempStation;
		memcpy(&TempStation, pData, sizeof(StationInfo));

		CString szTempbuf;
 		SetDlgItemText(IDC_EDIT_S_NAME,TempStation.szStationName);
		szTempbuf.Format("%d", TempStation.nUpCode);
 		SetDlgItemText(IDC_EDIT_S_UPCODE, szTempbuf);
		szTempbuf.Format("%d", TempStation.nDownCode);
 		SetDlgItemText(IDC_EDIT_S_DOWNCODE,szTempbuf);
		szTempbuf.Format("%d", TempStation.nDistance);
 		SetDlgItemText(IDC_EDIT_S_DISTANCE,szTempbuf);
	}

	*pResult = 0;
}

void CPageStationInfo::OnBnClickedBtnSModfiy()
{
	int nMake = m_listStation.GetSelectionMark();
	if (nMake != -1)
	{
		BYTE *pData = (BYTE *)m_listStation.GetItemData(nMake);
		if (pData==NULL)
			return;

		StationInfo TempStation;
		memcpy(&TempStation, pData, sizeof(StationInfo));

		CString szTempbuf;
		GetDlgItemText(IDC_EDIT_S_NAME,szTempbuf);
		int nUpCode = GetDlgItemInt(IDC_EDIT_S_UPCODE);
		int nDwonCode = GetDlgItemInt(IDC_EDIT_S_DOWNCODE);
		int nDistance = GetDlgItemInt(IDC_EDIT_S_DISTANCE);

		StationInfo stTempInfo;
		memset(&stTempInfo, 0x00, sizeof(StationInfo));
		strcpy_s(stTempInfo.szStationName, szTempbuf);
		stTempInfo.nUpCode = nUpCode;
		stTempInfo.nDownCode = nDwonCode;
		stTempInfo.nDistance = nDistance;
		stTempInfo.nStationId = TempStation.nStationId;

		BOOL bModfiy=FALSE;
		std::vector<StationInfo>::iterator it = m_vtManageStationInfo.begin();
		for(; it!=m_vtManageStationInfo.end(); it++)
		{
			StationInfo pStation = (StationInfo)*it;
			if (TempStation.nStationId == pStation.nStationId)
			{
				StationInfo &a=*it;
				a=stTempInfo;
				bModfiy = TRUE;
				break;
			}
		}
		if (bModfiy)
		{
			char szTempbuf[32]={0};
			m_listStation.SetItemText(nMake, 1, stTempInfo.szStationName);
			sprintf_s(szTempbuf,"%d", stTempInfo.nUpCode);
			m_listStation.SetItemText(nMake, 2, szTempbuf);
			sprintf_s(szTempbuf,"%d", stTempInfo.nDownCode);
			m_listStation.SetItemText(nMake, 3, szTempbuf);
			sprintf_s(szTempbuf,"%d", stTempInfo.nDistance);
			m_listStation.SetItemText(nMake, 4, szTempbuf);

			memcpy(m_pItembuf[TempStation.nStationId], &stTempInfo, sizeof(StationInfo));
			m_listStation.SetItemData(nMake, (DWORD_PTR)m_pItembuf[TempStation.nStationId]);
		}
	}
	else
	{
		MessageBox(_T("请选择所要修改的站名"), _T("提示"));
	}
}

void CPageStationInfo::OnBnClickedBtnSDel()
{
	int nMake = m_listStation.GetSelectionMark();
	if (nMake != -1)
	{
		BYTE *pData = (BYTE *)m_listStation.GetItemData(nMake);

		StationInfo TempStation;
		memcpy(&TempStation, pData, sizeof(StationInfo));

 		std::vector<StationInfo>::iterator it = m_vtManageStationInfo.begin();
 		for(; it!=m_vtManageStationInfo.end(); it++)
 		{
 			StationInfo vtStation = (StationInfo)*it;
 			if (vtStation.nStationId == TempStation.nStationId)
 			{
 				m_vtManageStationInfo.erase(it);
 				break;
 			}
 		}
		m_listStation.DeleteItem(nMake);
	}
	else
	{
		MessageBox(_T("请选择所要删除的站名"), _T("提示"));
	}
}

void CPageStationInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_listStation)
	{
		m_listStation.MoveWindow(10,m_StationRt.top, cx-20, cy-m_StationRt.top-10, TRUE);
		int nCount = m_listStation.GetHeaderCtrl()->GetItemCount();
		int nW = (cx-40)/nCount;
		for (int i=0; i<nCount; i++)
		{
			m_listStation.SetColumnWidth(i, nW);
		}

		GetDlgItem(IDC_BTN_S_SAVE)->MoveWindow(cx-80, m_StationRt.top-30, 70, 25, TRUE);
		GetDlgItem(IDC_BTN_S_DEL)->MoveWindow(cx-80-75, m_StationRt.top-30, 70, 25, TRUE);
		GetDlgItem(IDC_BTN_S_MODFIY)->MoveWindow(cx-80-75*2, m_StationRt.top-30, 70, 25, TRUE);
		GetDlgItem(IDC_BTN_S_ADD)->MoveWindow(cx-80-75*3, m_StationRt.top-30, 70, 25, TRUE);
	}
}

void CPageStationInfo::OnRunTmsEnableWindowBtn(BOOL bEnable)
{
	GetDlgItem(IDC_BTN_S_SAVE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_S_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_S_MODFIY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_S_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_S_ADD)->EnableWindow(bEnable);
	
}
void CPageStationInfo::OnBnClickedRadioHand()
{
	// TODO: Add your control notification handler code here
	OnRefreshStationData();
}

void CPageStationInfo::OnBnClickedRadioAuto()
{
	// TODO: Add your control notification handler code here
	OnRefreshStationData();
}

