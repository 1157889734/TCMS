// PanelMainFram.cpp : 实现文件
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "PanelMainFram.h"


// CPanelMainFram 对话框

IMPLEMENT_DYNAMIC(CPanelMainFram, CDialog)

CPanelMainFram::CPanelMainFram(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelMainFram::IDD, pParent)
{
	m_nTabPageIndex = 0;
	for (int i=0; i<TAB_PAGE_MAX; i++)
	{
		m_pTabArryDlg[i] = NULL;
	}
	memset(m_szStationInfoPath, 0x00, 255);
	memset(m_szLocalInfoPath, 0x00, 255);
}

CPanelMainFram::~CPanelMainFram()
{
}

void CPanelMainFram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGE, m_tMainTab);
}


BEGIN_MESSAGE_MAP(CPanelMainFram, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, &CPanelMainFram::OnTcnSelchangeTabPage)
END_MESSAGE_MAP()


// CPanelMainFram 消息处理程序

BOOL CPanelMainFram::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strAppPath;
	::GetModuleFileName(NULL, strAppPath.GetBuffer(256), 256);
	strAppPath.ReleaseBuffer();
	int nPos = strAppPath.ReverseFind('\\');
	strAppPath = strAppPath.Left(nPos);
	m_strAppPath = strAppPath;

	CString strIni = _T("\\StationFile\\Station.bin");
	strcpy_s(m_szStationInfoPath, m_strAppPath + strIni);
	strIni = _T("\\StationFile\\Station_Hand.bin");
	strcpy_s(m_szHandStationInfoPath, m_strAppPath + strIni);
	

	strIni = LOCAL_FILE_PAHT_SET_CONNAME;
	strcpy_s(m_szLocalInfoPath, m_strAppPath + strIni);

	ReadStationFile();
	ReadLoaclInfo();

	OnInitFrame();

	OnRefreshSystemData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPanelMainFram::DoTab(int nTab)
{
	if (TAB_PAGE_MAX<nTab)
		return;

	for (int i=0; i<TAB_PAGE_MAX; i++)
	{
		if (m_pTabArryDlg[i])
		{
			m_pTabArryDlg[i]->ShowWindow(SW_HIDE);
		}		
	}
	if (m_pTabArryDlg[nTab])
	{
		m_pTabArryDlg[nTab]->ShowWindow(SW_SHOW);
	}	
}

void CPanelMainFram::MoveTabPage()
{
	CRect childRect;
	m_tMainTab.GetClientRect(childRect);

	childRect.top += 22;
	childRect.bottom -= 4;
	childRect.left += 2;
	childRect.right -= 4;
	for (int i=0; i<TAB_PAGE_MAX; i++)
	{
		
		if(i == 0)
		{
			childRect.bottom += 1;
		}
		if (m_pTabArryDlg[i])
		{
			m_pTabArryDlg[i]->MoveWindow(childRect);
		}			
	}
}

void CPanelMainFram::WriteStationFile(std::vector<StationInfo> vtInfo, BOOL bAuto)
{
	FILE *pfile = NULL;
	if(bAuto)
	{
		pfile = fopen(m_szStationInfoPath, "wb+");
	}
	else
	{
		pfile = fopen(m_szHandStationInfoPath, "wb+");
	}
	if (pfile)
	{
		std::vector<StationInfo>::iterator it = vtInfo.begin();
		for(; it!=vtInfo.end(); it++)
		{
			StationInfo TempStation = (StationInfo)*it;
			fwrite(&TempStation, sizeof(StationInfo), 1, pfile);
		}	
		fclose(pfile);
	}	
	if (bAuto)
	{
		m_vtStationInfo.clear();
		m_vtStationInfo = vtInfo;
	}
	else
	{
		m_vtHandStationInfo.clear();
		m_vtHandStationInfo = vtInfo;
	}
	m_pageRunDlg.RefreshRunDataPage();
}

void CPanelMainFram::ReadStationFile()
{
	m_vtStationInfo.clear();

	FILE *pfile = NULL;
	pfile = fopen(m_szStationInfoPath, "rb+");
	if (pfile)
	{
		while(!feof(pfile))
		{
			StationInfo TempStation;
			if (fread(&TempStation, sizeof(StationInfo), 1, pfile))
			{
				//TRACE("%d \n", TempStation.nStationId);
				m_vtStationInfo.push_back(TempStation);
			}
		}
		fclose(pfile);
	}

	pfile = fopen(m_szHandStationInfoPath, "rb+");
	if (pfile)
	{
		while(!feof(pfile))
		{
			StationInfo TempStation;
			if (fread(&TempStation, sizeof(StationInfo), 1, pfile))
			{
				//TRACE("%d \n", TempStation.nStationId);
				m_vtHandStationInfo.push_back(TempStation);
			}
		}
		fclose(pfile);
	}
}

void CPanelMainFram::OnRefreshSystemData()
{
	m_pageRunDlg.RefreshRunDataPage();
	m_PageStationDlg.OnRefreshStationData();
}

void CPanelMainFram::WriteLocalInfo()
{
	FILE *pfile = NULL;
	pfile = fopen(m_szLocalInfoPath, "wb+");
	if (pfile)
	{
		fwrite(&m_LocalInfo, sizeof(LocalInfoSet), 1, pfile);
		fclose(pfile);
	}
}

void CPanelMainFram::ReadLoaclInfo()
{
	FILE *pfile = NULL;
	pfile = fopen(m_szLocalInfoPath, "rb+");
	if (pfile)
	{
		fread(&m_LocalInfo, sizeof(LocalInfoSet), 1, pfile);
		fclose(pfile);
	}
}

void CPanelMainFram::OnInitFrame()
{
	// TODO:  在此添加额外的初始化
	m_pageRunDlg.Create(IDD_DIALOG_PAGE_RUN, &m_tMainTab);
	m_PageFeedBackDlg.Create(IDD_DIALOG_PAGE_FEEDBACK, &m_tMainTab);
	m_PageStationDlg.Create(IDD_DIALOG_PAGE_STATION, &m_tMainTab);
	m_PageLocalConfig.Create(IDD_DIALOG_PAGE_LOCAL_SET, &m_tMainTab);

	InsertPageTab(&m_pageRunDlg,	  _T("     运  行  界  面     "));
//	InsertPageTab(&m_PageFeedBackDlg, _T("     设  备  状  态     "));
	InsertPageTab(&m_PageStationDlg,  _T("     站  点  管  理     "));
	InsertPageTab(&m_PageLocalConfig, _T("     本  地  配  置     "));

	DoTab(0);
}

void CPanelMainFram::OnEnableMainFrame()
{
	char szValue[512];
	char m_strIniPath[255];
	CString strIni = LOCAL_FILE_PAHT_CONFIG_NAME;
	strcpy_s(m_strIniPath, m_strAppPath + strIni);

	GetPrivateProfileString("SysCfg", "EnableLocal", "", szValue, 32, m_strIniPath);
	m_LocalInfo.bEnableLocalSetInfo = atoi(szValue);

	RefreshLocalSet(m_LocalInfo, FALSE);
	m_PageLocalConfig.OnRefreshLocalItem(m_LocalInfo);
}

void CPanelMainFram::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_tMainTab)
	{
		m_tMainTab.MoveWindow(1, 1, cx-2, cy-2);
		MoveTabPage();		
	}
}

void CPanelMainFram::OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nSel = m_tMainTab.GetCurSel();
	if(nSel>=0)
	{
		DoTab(nSel);
	}

	*pResult = 0;
}

void CPanelMainFram::InsertPageTab(CDialog *pDlg, char szItemTitle[32])
{
	m_tMainTab.InsertItem(m_nTabPageIndex, szItemTitle);
	m_pTabArryDlg[m_nTabPageIndex] = pDlg;
	m_nTabPageIndex++;
}

void CPanelMainFram::RefreshLocalSet(LocalInfoSet stlocalInfo, BOOL bReplace)
{
	if (bReplace)
	{
		m_LocalInfo = stlocalInfo;
	}
	
	if (!stlocalInfo.bEnableFeedBack && IsTabItemWindowVisible(&m_PageFeedBackDlg))
	{
		DeleteTabItem(&m_PageFeedBackDlg);
	}
	else if(stlocalInfo.bEnableFeedBack && !IsTabItemWindowVisible(&m_PageFeedBackDlg))
	{
		int nCount = m_tMainTab.GetItemCount()-1;
		InsertTabItem(nCount, _T("     设  备  状  态     "), &m_PageFeedBackDlg);
	}

	if (!stlocalInfo.bEnableStationManage && IsTabItemWindowVisible(&m_PageStationDlg))
	{
		DeleteTabItem(&m_PageStationDlg);
	}
	else if(stlocalInfo.bEnableStationManage && !IsTabItemWindowVisible(&m_PageStationDlg))
	{
		int nCount = m_tMainTab.GetItemCount()-1;
		InsertTabItem(nCount, _T("     站  点  管  理     "), &m_PageStationDlg);
	}

	if (!stlocalInfo.bEnableLocalSetInfo && IsTabItemWindowVisible(&m_PageLocalConfig))
	{
		DeleteTabItem(&m_PageLocalConfig);
	}
	else if(stlocalInfo.bEnableLocalSetInfo && !IsTabItemWindowVisible(&m_PageLocalConfig))
	{
		int nCount = m_tMainTab.GetItemCount()-1;
		InsertTabItem(nCount, _T("     本  地  配  置     "), &m_PageLocalConfig);
	}

	m_pageRunDlg.OnEnablePulse(stlocalInfo.bEnableEmcPulse);
	m_PageFeedBackDlg.SetFeedbackPanel(stlocalInfo.nCarNum, stlocalInfo.nFeedBackNum);
	m_PageFeedBackDlg.SetFeedbackEableBorder(stlocalInfo.bEnableFeedBackBorder);

	MoveTabPage();

	int nSel = m_tMainTab.GetCurSel();
	if(nSel>=0)
	{
		DoTab(nSel);
	}
}

void CPanelMainFram::DeleteTabItem(CDialog  *pDlg)
{
	int nTabIndex=0;
	BOOL bFalg = FALSE;
	for (int i=0; i<TAB_PAGE_MAX; i++)
	{
		if (pDlg==m_pTabArryDlg[i])
		{
			nTabIndex = i;
			m_pTabArryDlg[i] = NULL;
			bFalg = TRUE;
			continue;
		}
		if (m_pTabArryDlg[i] && bFalg && m_pTabArryDlg[i-1]==NULL)
		{
			CDialog *pTemDlg=m_pTabArryDlg[i];
			m_pTabArryDlg[i] = NULL;
			m_pTabArryDlg[i-1] = pTemDlg;
		}		
	}	
	m_tMainTab.DeleteItem(nTabIndex);
}

void CPanelMainFram::InsertTabItem(int nItemIndex,LPCTSTR lpszItem, CDialog  *pDlg)
{
	m_tMainTab.InsertItem(nItemIndex, lpszItem);

	BOOL bFalg = FALSE;
	CDialog *pTemDlg=NULL;
	for (int i=0; i<TAB_PAGE_MAX; i++)
	{
		if (nItemIndex==i)
		{
			pTemDlg = m_pTabArryDlg[i];
			m_pTabArryDlg[i] = pDlg;
			bFalg = TRUE;
			continue;
		}
		if (bFalg && pTemDlg)
		{
			if (m_pTabArryDlg[i])
			{
				CDialog *pTemDlg1=m_pTabArryDlg[i];
				m_pTabArryDlg[i] = pTemDlg;
				pTemDlg = pTemDlg1;
			}
			else
			{
				m_pTabArryDlg[i] = pTemDlg;
			}	
		}
	}	
}

BOOL CPanelMainFram::IsTabItemWindowVisible(CDialog  *pDlg)
{
	BOOL bIsVisible=FALSE;
	for (int i=0; i<TAB_PAGE_MAX; i++)
	{
		if (m_pTabArryDlg[i]==pDlg)
		{
			bIsVisible = TRUE;
		}
	}	
	return bIsVisible;
}