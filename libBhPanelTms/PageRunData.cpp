// PageRunData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PageRunData.h"
#include "PanelMainFram.h"
#include <shlwapi.h>


#define JUST_RUN	1


#pragma comment(lib,"Shlwapi.lib")

extern CPanelMainFram *g_pMainFrame;

int g_CurIndex = 0;
int g_NxtIndex = 0;

//---------------------������ٶȵĵط� ------------------------------------------------

#define EACH_PACKET_TIME	 0.3		      // ��һ���ļ����λΪ�룬0.3s,300ms
#define A_C_C_E_L			 40				  // ÿ����� A_C_C_E_L km / h
#define ACCEL				 A_C_C_E_L / 3.6  // �г����ٶ�( m / s^2 )��ÿ����� A_C_C_E_L  km / h

#define DISTACNE_LEAVE			150			  // ����     150m ������վ�ź�
#define DISTACNE_COM_STATION	150			  // ����һվ 150m ����վ�ź�

//--------------------------------------------------------------------------------------


#define	TIMEPULSE				2000	// �����ź�����ʱ��	

#define TIME_RUN_STATION		101		//���ж�ʱ��
#define TIME_RE_RUN_STATION		102		//�����Ŷ�ʱ��
#define IDT_TIME_EMC_PLAYBC		103		// ���Ž����㲥���嶨ʱ��
#define IDT_TIME_EMC_STOPBC		104		// ֹͣ�����㲥���嶨ʱ��
#define	IDT_TIME_ARRIVE			105
#define	IDT_TIME_LEAVE			106
#define IDT_TIME_JUMP_UP		107
#define IDT_TIME_JUMP_DOWN		108
#define IDT_TIME_DATETIME_VALID	109
#define IDT_TIME_NEXT_STATION	110
#define IDT_TIME_START_STOP_STATION	111


#define IDT_TIME_KILL_OPEN_DOOR	 112
#define IDT_TIME_KILL_CLOSE_DOOR 113

#define IDT_TIME_KILL_ATC_OPEN_DOOR		 114
#define IDT_TIME_KILL_ATC_CLOSE_DOOR	 115

#define IDT_TIME_SET_ALL_CLOSE_DOOR		 116
//#define IDT_TIME_KILL_ALL_CLOSE_DOOR     117
	
#define RUN_NONRE_CLOLOR	RGB(0,255,0)		//����״̬
#define RUN_STATA_CLOLOR	RGB(255,0,0)		//���е�ǰ״̬


//Modify by Johnny in 2017-07-26 14:57:32
//#define TIME_RUN_INTERVAL     200		//���м��
#define TIME_RUN_INTERVAL     300		//���м��
#define TIME_STATION_STAY	  4000		//�����Ŷ���ʱ��

#define ERROR_AUTO	   -1	//ģʽ����
#define  SEMI_AUTO		0	//���Զ�ģʽ
#define  FULL_ATUO		1	//ȫ�Զ�ģʽ
#define   MAN_ATUO		2	//�ֶ�ģʽ

// CPageRunData �Ի���

IMPLEMENT_DYNAMIC(CPageRunData, CDialog)

CPageRunData::CPageRunData(CWnd* pParent /*=NULL*/)
	: CDialog(CPageRunData::IDD, pParent)
	, m_nLastSpeed(0)
	, m_nSumDist(0)
	, m_nLast_Run_Mode(0)
	, m_bOpen_Port(FALSE)
	, m_nLocationInfo(0)
{
	m_EnumCurrentState = Enum_OpenDoor;
	m_nFrontStation	 = 0;
	m_nNextStation	 = 0;
	
	//Modify by Johnny for test in 2017-07-31 19:57:58
	m_nEvenVel = 90;
	//m_nEvenVel		 = 150;
	m_bIsUpDirect	 = TRUE;
	m_nTimeCount	 = 0;
	m_nZoneDist		 = 0;
	m_nCurZoneDist	 = 0;
	m_nTimeCountWA	 = 0;
	m_nStartStation	 = 0;
	m_nEndStation	 = 0;
	m_bPlayBC		 = false;
	m_nAnounceIndex  = 0;
	m_nCurrentSpeed  = 0;
	m_nLastStationDist = 0;
	m_bEmcPulse	     = FALSE;
	m_pDataAnalysisDlg = NULL;

	m_bArriveBC		 = false;	// ��վ�㲥
	m_bLeaveBC		 = false;	// ��վ�㲥

	m_bDateTimeValid = 0;
	m_bJumpDown		 = 0;
	m_bJumpUp		 = 0;

	m_bClosedLeft	 = 0;
	m_bClosedRight	 = 0;

	m_bOpenRightDoor = 0;
	m_bOpenLeftDoor	 = 0;
	m_bClosedLeft	 = 0;
	m_bClosedRight	 = 0;
	m_bCloseLeftDoor = 0;
	m_bCloseRightDoor= 0;

	m_bATC_Open_A   = 0;  //ATC������
	m_bATC_Open_B  = 0;	 //ATC������
	m_bATC_Close_A  = 0;	 //ATC������
	m_bATC_Close_B = 0;  //ATC������

	//��ʼ��Ϊ���Զ�
	m_nRunMode		 = SEMI_AUTO;
	m_bPause		 = 0;

	m_nLastStart	 = 0;
	m_nLastEnd		 = 17;
	m_nSumDist1		 = 0;
	m_nHMI_Start	 = 0;
	m_nHMI_End		 = 0;
	m_nHMI_Next		 = 0;
}



CPageRunData::~CPageRunData()
{
	if(m_pDataAnalysisDlg)
	{
		m_pDataAnalysisDlg->DestroyWindow();
		delete m_pDataAnalysisDlg;
		m_pDataAnalysisDlg = NULL;
	}
}

void CPageRunData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FSTATION, m_sFrontStation);
	DDX_Control(pDX, IDC_STATIC_NSTATION, m_sNextStation);
	DDX_Control(pDX, IDC_STATIC_DIRECT, m_sDirect);
	DDX_Control(pDX, IDC_PROGRESS_DIST, m_pDistance);
	DDX_Control(pDX, IDC_LIST_BC, m_ListBc);
	DDX_Control(pDX, IDC_CHECK_LEFT_DOOR, m_BTN_Next_Open_Left);
	DDX_Control(pDX, IDC_CHECK_RIGHT_DOOR, m_BTN_Next_Open_Right);
	DDX_Control(pDX, IDC_STATIC_NEXT_OPEN_SIDE, m_STATIC_Next_Open_Side);
	DDX_Control(pDX, IDC_STATIC_DOOR_STATE, m_STATIC_Cur_Door_State);
}


BEGIN_MESSAGE_MAP(CPageRunData, CDialog)
 	ON_BN_CLICKED(IDC_RADIO_UP, &CPageRunData::OnBnClickedRadioUp)
 	ON_BN_CLICKED(IDC_RADIO_DWON, &CPageRunData::OnBnClickedRadioUp)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_APPRY, &CPageRunData::OnBnClickedBtnAppry)
	ON_BN_CLICKED(IDC_BTN_ADDVEL, &CPageRunData::OnBnClickedBtnAddvel)
	ON_BN_CLICKED(IDC_BTN_SUBVEL, &CPageRunData::OnBnClickedBtnSubvel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BC, &CPageRunData::OnNMClickListBc)
	ON_BN_CLICKED(IDC_BTN_STATION_UP, &CPageRunData::OnBnClickedBtnStationUp)
	ON_BN_CLICKED(IDC_BTN_STATION_NEXT, &CPageRunData::OnBnClickedBtnStationNext)
	ON_BN_CLICKED(IDC_BTN_RUN, &CPageRunData::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_START_EMC, &CPageRunData::OnBnClickedBtnStartEmc)
	ON_BN_CLICKED(IDC_BTN_STOP_EMC, &CPageRunData::OnBnClickedBtnStopEmc)
	ON_BN_CLICKED(IDC_RADIO_HMI, &CPageRunData::OnBnClickedRadioHmi)
	ON_BN_CLICKED(IDC_RADIO_ATC, &CPageRunData::OnBnClickedRadioAtc)
	ON_CBN_SELCHANGE(IDC_COMBO_LINEID, &CPageRunData::OnCbnSelchangeComboLineid)
	ON_BN_CLICKED(IDC_BTN_ARRIVE, &CPageRunData::OnBnClickedBtnArrive)
	ON_BN_CLICKED(IDC_BTN_LEAVE, &CPageRunData::OnBnClickedBtnLeave)
	ON_BN_CLICKED(IDC_BTN_DATA_VIEW, &CPageRunData::OnBnClickedBtnDataView)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DOOR, &CPageRunData::OnBnClickedButtonOpenDoor)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_DOOR, &CPageRunData::OnBnClickedButtonCloseDoor)
	ON_BN_CLICKED(IDC_BUTTON_SET_NEXT_STATION, &CPageRunData::OnBnClickedButtonSetNextStation)
	ON_BN_CLICKED(IDC_BUTTON_DATETIME_VALID, &CPageRunData::OnBnClickedButtonDatetimeValid)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP_STATION, &CPageRunData::OnBnClickedButtonStartStopStation)
	ON_BN_CLICKED(IDC_CHECK_ATO_MODE, &CPageRunData::OnBnClickedCheckAtoMode)
	ON_BN_CLICKED(IDC_CHECK_PM_MODE, &CPageRunData::OnBnClickedCheckPmMode)
	ON_BN_CLICKED(IDC_CHECK_ATC_MODE, &CPageRunData::OnBnClickedCheckAtcMode)
	ON_BN_CLICKED(IDC_CHECK_TCMS_MODE, &CPageRunData::OnBnClickedCheckTcmsMode)
	ON_BN_CLICKED(IDC_BTN_START_RUN, &CPageRunData::OnBnClickedBtnStartRun)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RIGHT_DOOR, &CPageRunData::OnBnClickedButtonOpenRightDoor)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_RIGHT_DOOR, &CPageRunData::OnBnClickedButtonCloseRightDoor)
	ON_BN_CLICKED(IDC_CHECK_LEFT_DOOR, &CPageRunData::OnBnClickedCheckLeftDoor)
	ON_BN_CLICKED(IDC_CHECK_RIGHT_DOOR, &CPageRunData::OnBnClickedCheckRightDoor)
END_MESSAGE_MAP()

//���Ų�
						   // 1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18
BOOL g_bUpDireOpenSide[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  0,  1,  1,  1,  1,  1,  1};
BOOL g_bDnDireOpenSide[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0,  0,  1,  1,  1,  1,  1,  1};
				 //index   // 0  1  2  3  4  5  6  7  8  9   10  11  12  13  14  15  16  17 

bool g_bLastOpenLeft = FALSE;
bool g_bLastOpenRight= FALSE;


// CPageRunData ��Ϣ�������

BOOL CPageRunData::OnInitDialog()
{
	CDialog::OnInitDialog();

	//init panel
	DWORD dwStyle = m_ListBc.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE;

	m_ListBc.SetExtendedStyle(dwStyle);	
	m_ListBc.InsertColumn(0, _T("�㲥��"), LVCFMT_CENTER, 60);
	m_ListBc.InsertColumn(1, _T("�㲥����"), LVCFMT_LEFT, 410);

	CheckDlgButton(IDC_RADIO_UP, TRUE);
	CheckDlgButton(IDC_RADIO_RUN_TC1, TRUE);
	CheckDlgButton(IDC_RADIO_ATC, TRUE);
	CheckDlgButton(IDC_CHECK_LEFT_DOOR, TRUE);
	GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);

	//Del by Johnny in 2017-08-03 08:53:11
	//((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(FALSE);
	//((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(FALSE);
	//((CButton *)GetDlgItem(IDC_BUTTON_OPEN_DOOR))->EnableWindow(FALSE);
	//((CButton *)GetDlgItem(IDC_BUTTON_CLOSE_DOOR))->EnableWindow(FALSE);
	
	
	CString strVel;
	strVel.Format(_T("%d km/h"), m_nEvenVel);
	SetDlgItemText(IDC_STATIC_VEL, strVel);

	//��ʼ���� 0 -> 1 �Ż���
	m_bOpenRightDoor  = 0;
	m_bOpenLeftDoor	  = 0;
	m_bClosedLeft	  = 0;
	m_bClosedRight	  = 0;
	m_bCloseLeftDoor  = 0;
	m_bCloseRightDoor = 0;

	ReadRunLocalInfo();

	InitDataAnalysisDlg();

	//Add by Johnny for test in 2017-07-31 19:55:27	
	//CheckDlgButton(IDC_CHECK_ATO_MODE, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPageRunData::InitDataAnalysisDlg()
{
	if(NULL == m_pDataAnalysisDlg)
	{
		CRect rt, rtScreen;
		GetDesktopWindow()->GetWindowRect(rtScreen);
		m_pDataAnalysisDlg = new CDataAnalysisDlg;
		m_pDataAnalysisDlg->Create(IDD_DLG_DATA_ANALYSIS,this);
		m_pDataAnalysisDlg->GetWindowRect(rt);
  		m_pDataAnalysisDlg->MoveWindow(rtScreen.Width()/2-rt.Width()/2,rtScreen.Height()/2-rt.Height()/2, rt.Width(),rt.Height());
		m_pDataAnalysisDlg->ShowWindow(SW_HIDE);
	}
}

void CPageRunData::InsertComBoxStation(BOOL bUpstream)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->ResetContent();

	char szStationbuf[64]={0};
	if (bUpstream)
	{
		std::vector<StationInfo>::iterator it = m_vtRunStationInfo.begin();
		for(int i=0; it!=m_vtRunStationInfo.end(); it++,i++)
		{
			StationInfo pStation = (StationInfo)*it;

			//Modify by Johnny in 2017-08-21 10:02:30
			//sprintf_s(szStationbuf, "%d.%s",pStation.nUpCode, pStation.szStationName);
			sprintf_s(szStationbuf, "%02d.%s", i + 1, pStation.szStationName);
			((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->InsertString(i, szStationbuf);
		}
	}
	else
	{
		std::vector<StationInfo>::reverse_iterator it = m_vtRunStationInfo.rbegin();
		for(int i=0; it!=m_vtRunStationInfo.rend(); it++,i++)
		{
			StationInfo pStation = (StationInfo)*it;
			//Modify by Johnny in 2017-08-21 10:02:45
			//sprintf_s(szStationbuf, "%d.%s",pStation.nDownCode, pStation.szStationName);
			sprintf_s(szStationbuf, "%02d.%s", 18 - i, pStation.szStationName);
			((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->InsertString(i, szStationbuf);
		}
	}
	

	//Add by Johnny in 2017-08-28 10:10:04
	if(m_bIsUpDirect)
	{
		int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->GetCurSel();
		((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(nIndex);
	}else
	{
		int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->GetCurSel();
		((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(17 - nIndex);
	}

}

void CPageRunData::InitNextStationCombox()
{
	char szStationbuf[64]={0};
	std::vector<StationInfo>::iterator it = m_vtRunStationInfo.begin();
	for(int i=0; it!=m_vtRunStationInfo.end(); it++,i++)
	{
		StationInfo pStation = (StationInfo)*it;
		sprintf_s(szStationbuf, "%s", pStation.szStationName);
		((CComboBox*)GetDlgItem(IDC_COMBO_NEXT_STATION))->InsertString(i, szStationbuf);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_NEXT_STATION))->SetCurSel(0);	
}

void CPageRunData::RefreshRunDataPage()
{
	m_vtRunStationInfo = g_pMainFrame->m_vtStationInfo;

	//run title
	m_sDirect.SetCenterText("��");

	int nvtStationCount = (int)m_vtRunStationInfo.size();
	if (nvtStationCount==0)
	{
		m_sFrontStation.SetCenterText("��һվ");
		m_sNextStation.SetCenterText("��һվ");
	}
	else
	{
		SetDlgItemText(IDC_EDIT_STAT_STATION, m_vtRunStationInfo.at(0).szStationName);
		SetDlgItemText(IDC_EDIT_END_STATION, m_vtRunStationInfo.at(nvtStationCount-1).szStationName);

		m_sFrontStation.SetCenterText(m_vtRunStationInfo.at(0).szStationName);
		m_sNextStation.SetCenterText(m_vtRunStationInfo.at(1).szStationName);
		SetDlgItemText(IDC_EDIT_SET_STATION, m_vtRunStationInfo.at(1).szStationName);
	}

	// Station Name
	InsertComBoxStation(TRUE);

	//���� ���
	char szItembuf[32]={0};
	((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->ResetContent();
	int i=0;
	for (; i<nvtStationCount;i++)
	{
		sprintf_s(szItembuf, "%d %s",i+1, m_vtRunStationInfo.at(i).szStationName);
		((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->InsertString(i, szItembuf);
		((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->InsertString(i, szItembuf);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->SetCurSel(i-1);

	InitNextStationCombox();
}

void CPageRunData::ReadRunLocalInfo()
{
	char szKey[100];
	char szValue[512];
	int  len = 512;
	CString tempValue;
	int  nCount1;

	char m_strIniPath[255];
	CString strIni = LOCAL_FILE_PAHT_CONFIG_NAME;
	strcpy_s(m_strIniPath, g_pMainFrame->m_strAppPath + strIni);

//BC
	GetPrivateProfileString("SysCfg", "BCNum", "", szValue, len, m_strIniPath);
	nCount1 = atoi(szValue);

	CStringArray m_aAnounce;
	CStringArray m_vBCContent;
	for (int i = 0; i < nCount1; i++)
	{
		sprintf_s(szKey, "%d", i);
		GetPrivateProfileString("BCContent", szKey, "", szValue, len, m_strIniPath);
		tempValue.Format("%s", szValue);
		int index = tempValue.Find(",", 0);
		CString strCode = tempValue.Left(index);
		m_aAnounce.Add(strCode);
		CString strVale = tempValue.Right(tempValue.GetLength()-index-1);
		m_vBCContent.Add(strVale);
	}


	for (UINT i = 0; i < (int)m_vBCContent.GetSize(); i++)
	{
		m_ListBc.InsertItem(i, m_aAnounce.GetAt(i));
		m_ListBc.SetItemText(i, 1, m_vBCContent.GetAt(i));
	}
	

	//��·ID
	GetPrivateProfileString("SysCfg", "LineCount", "", szValue, len, m_strIniPath);
	int nLineCount = atoi(szValue);
	
	char szItemText[32]={0};
	for (int n=0; n<nLineCount; n++)
	{
		sprintf_s(szKey, "%d", n);
		GetPrivateProfileString("LineName", szKey, "", szValue, len, m_strIniPath);
		sprintf_s(szItemText, "%s", szValue);

		((CComboBox*)GetDlgItem(IDC_COMBO_LINEID))->InsertString(n, szItemText);		
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_LINEID))->SetCurSel(0);
}

int nStata = 0;

void CPageRunData::OnBnClickedRadioUp()
{
	//Del in 2017-07-31 19:45:07
	//KillTimer(TIME_RE_RUN_STATION);
	//KillTimer(TIME_RUN_STATION);

	m_bIsUpDirect = IsDlgButtonChecked(IDC_RADIO_UP);
	InsertComBoxStation(m_bIsUpDirect);

	
	m_nStartStation = ((CComboBox*)GetDlgItem(m_bIsUpDirect ? IDC_COMBO_SECTION_STATA:IDC_COMBO_SECTION_END))->GetCurSel();
	m_nEndStation   = ((CComboBox*)GetDlgItem(m_bIsUpDirect ? IDC_COMBO_SECTION_END  :IDC_COMBO_SECTION_STATA))->GetCurSel();

	m_nFrontStation = m_nStartStation;
	m_nNextStation  = m_nFrontStation;

	g_CurIndex = m_nFrontStation;
	g_NxtIndex = m_nNextStation;

	SetDlgItemText(IDC_EDIT_STAT_STATION, m_vtRunStationInfo.at(m_nStartStation).szStationName);
	SetDlgItemText(IDC_EDIT_END_STATION,  m_vtRunStationInfo.at(m_nEndStation).szStationName);


	CString szNextStation = m_vtRunStationInfo.at(m_bIsUpDirect ? m_nFrontStation + 1 : m_nFrontStation - 1).szStationName;
	m_sFrontStation.SetCenterText(m_vtRunStationInfo.at(m_nFrontStation).szStationName);

	//Modify by Johnny in 2017-07-31 20:38:50    //�޸ĵ��յ�վ�� ת��֮����ȻΪ�յ�վ->�յ�վ
	//m_sNextStation.SetCenterText(szNextStation);
	m_sNextStation.SetCenterText(m_vtRunStationInfo.at(m_nFrontStation).szStationName);

	SetDlgItemText(IDC_EDIT_SET_STATION,   szNextStation);
	SetDlgItemText(IDC_STATIC_DESCRIPTION, m_bIsUpDirect ? _T("���з�������") : _T("���з�������") );

	m_nTimeCount = 0;
	m_EnumCurrentState = Enum_OpenDoor;
	nStata = 0;
}



#define OPEN_DOOR_TIME		7		//Time :  OPEN_DOOR_TIME  * SendPacketTime
#define CLOSE_DOOR_TIME		7		//Time :  CLOSE_DOOR_TIME * SendPacketTime

void CPageRunData::OnRunStation()
{

	if(m_nRunMode == MAN_ATUO)
	{
		return;
	}

	//nStata ����ÿ���źŵ�ͣ��ʱ�䣬ͣ����ʽΪ��nStata * SendPacketTime  ms
	switch (m_EnumCurrentState)
	{

	case Enum_CloseDoor:	//����
		{
			//Add by Johnny in 2017-07-27 10:16:20
			nStata ++;

			if(nStata > CLOSE_DOOR_TIME)
			{
				nStata = 0;
			}
			if(m_bPause)
			{
				break;
			}

			//Modify by Johnny in 2017-08-18 10:03:51,
			//Set OpenDoorSide

			if(nStata == 1)
			{
				if(m_bIsUpDirect)
				{
					//1 ������
					if(1 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 0;
						m_bClosedLeft     = 1;
						m_bCloseLeftDoor  = 1;

						m_STATIC_Cur_Door_State.SetWindowText(_T("������"));
					}
					//0	������
					else if(0 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenRightDoor  = 0;
						m_bClosedRight	  = 1;
						m_bCloseRightDoor = 1;

						m_STATIC_Cur_Door_State.SetWindowText(_T("�Ҳ����"));
					}
					//2	�������
					else if(2 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 0;
						m_bClosedLeft     = 1;
						m_bCloseLeftDoor  = 1;

						m_bOpenRightDoor  = 0;
						m_bClosedRight	  = 1;
						m_bCloseRightDoor = 1;

						m_STATIC_Cur_Door_State.SetWindowText(_T("�������"));
					}

				}
				else // ����
				{
					//1 ������
					if(1 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 0;
						m_bClosedLeft     = 1;
						m_bCloseLeftDoor  = 1;

						m_STATIC_Cur_Door_State.SetWindowText(_T("������"));
					}
					//0	������
					else if(0 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenRightDoor  = 0;
						m_bClosedRight	  = 1;
						m_bCloseRightDoor = 1;

						m_STATIC_Cur_Door_State.SetWindowText(_T("�Ҳ����"));
					}
					//2	���࿪��
					else if(2 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 0;
						m_bClosedLeft     = 1;
						m_bCloseLeftDoor  = 1;

						m_bOpenRightDoor  = 0;
						m_bClosedRight	  = 1;
						m_bCloseRightDoor = 1;

						m_STATIC_Cur_Door_State.SetWindowText(_T("�������"));
					}

				}
				SetTimer(IDT_TIME_KILL_CLOSE_DOOR, 3000, NULL);
			}
			
#if 0
			// Modify by Johnny in 2017-08-01 15:59:40�� if else -> if
			if(IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR))
			{
				m_bOpenLeftDoor  = 0;
				m_bClosedLeft    = 1;
				m_bCloseLeftDoor = 1;
			}
			if(IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR))
			{
				m_bOpenRightDoor  = 0;
				m_bClosedRight	  = 1;
				m_bCloseRightDoor = 1;
	
			}
#endif

			if(nStata == CLOSE_DOOR_TIME)
			{
				// Modify by Johnny in 2017-08-01 15:59:40�� if else -> if
#if 0
				if(IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR))
				{
					m_bClosedLeft    = 0;
					m_bCloseLeftDoor = 0;
					
				}
				if(IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR))
				{
					m_bClosedRight    = 0;
					m_bCloseRightDoor = 0;

				}
#endif
				// Modify by Johnny in 2017-08-18 11:42:10

				if(m_bIsUpDirect)
				{
					//1 ������
					if(1 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bClosedLeft    = 0;
						m_bCloseLeftDoor = 0;
					}
					//0	������
					else if(0 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bClosedRight    = 0;
						m_bCloseRightDoor = 0;
					}
					//2	�������
					else if(2 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bClosedLeft     = 0;
						m_bCloseLeftDoor  = 0;

						m_bClosedRight    = 0;
						m_bCloseRightDoor = 0;
					}

				}
				else // ����
				{
					//1 ������
					if(1 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bClosedLeft    = 0;
						m_bCloseLeftDoor = 0;

					}
					//0	������
					else if(0 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bClosedRight    = 0;
						m_bCloseRightDoor = 0;
					}
					//2	���࿪��
					else if(2 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bClosedLeft     = 0;
						m_bCloseLeftDoor  = 0;

						m_bClosedRight    = 0;
						m_bCloseRightDoor = 0;
					}
				}

				SetDlgItemText(IDC_STATIC_STATION_DIST, "0.000km");		// ��ʾ�г�����һվ�ľ���

				m_EnumCurrentState = Enum_NextStation;

				KillTimer(TIME_RUN_STATION);
				SetTimer(TIME_RE_RUN_STATION, TIME_STATION_STAY, NULL);//����ʱ��

				nStata = 0;

			}

		}		
		break;
	case Enum_NextStation:		//��һվ
		{
			//Add by Johnny in 2017-07-27 10:16:20
			if(m_bPause)
			{
				break;
			}

			nStata++;
#if 0
			if (nStata == 10)//2s ״̬
			{	
				if (m_bIsUpDirect)
				{
					m_nFrontStation = m_nNextStation;

					g_CurIndex = m_nFrontStation;
					
					//��ֹ���
					//Add  by Johnny in 2017-07-26 20:11:40
					if(17 == m_nFrontStation)//index 0
					{
						OnBnClickedRadioUp();
					}
					else
					{
						m_nNextStation  = m_nFrontStation + 1;

						g_NxtIndex = m_nNextStation;
					}
					
					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(m_nFrontStation);
				}
				else
				{
					int ncount = (int)m_vtRunStationInfo.size();
					m_nFrontStation = m_nNextStation;

					g_CurIndex = m_nFrontStation;

					//��ֹ���
					//Add  by Johnny in 2017-07-26 20:11:40
					if(0 == m_nFrontStation)//index 0
					{
						OnBnClickedRadioUp();
					}
					else
					{
						m_nNextStation  = m_nFrontStation - 1;

						g_NxtIndex = m_nNextStation;
					}
					
					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(ncount-m_nFrontStation-1);
				}


				//Add by Johnny in 2017-08-18 10:21:18
				if(m_bIsUpDirect)
				{
					//left
					if(1 == g_bUpDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Left.SetCheck(TRUE);
						m_BTN_Next_Open_Right.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("��࿪��"));
					}
					//left right
					else if(2 == g_bUpDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(TRUE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("���࿪��"));

					}
					//right
					else if(0 == g_bUpDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("�Ҳ࿪��"));
					}
				}
				else
				{
					//left
					if(1 == g_bDnDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Left.SetCheck(TRUE);
						m_BTN_Next_Open_Right.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("��࿪��"));
					}
					//left right
					else if(2 == g_bDnDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(TRUE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("���࿪��"));

					}
					//right
					else if(0 == g_bDnDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("�Ҳ࿪��"));
					}
				}



				//��ʾ��ǰվ�� ��һվ��
				CString szFrontStation = m_vtRunStationInfo.at(m_nFrontStation).szStationName;
				CString szNextStation  = m_vtRunStationInfo.at(m_nNextStation).szStationName;

				m_sFrontStation.SetCenterText(szFrontStation);
				m_sNextStation.SetCenterText(szNextStation);
				SetDlgItemText(IDC_EDIT_SET_STATION, szNextStation);

				//�������
				if (m_bIsUpDirect)
				{
					//Modify by Johnny in 2017-08-04 11:17:43
					for (int i = m_nFrontStation; i < m_nNextStation; i++)
					{
						//m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;
						m_nZoneDist += m_vtRunStationInfo.at(i + 1).nDistance;
					}
				}
				else
				{
					for (int i = m_nFrontStation; i > m_nNextStation; i--)
					{
						//Modify by Johnny in 2017-08-04 11:17:43
						//m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;

						m_nZoneDist += m_vtRunStationInfo.at(i).nDistance;
					}
				}			
				m_nCurZoneDist = m_nZoneDist;
				m_pDistance.SetRange(0, m_nZoneDist);

				m_nLastStationDist = 0;
				SetDlgItemText(IDC_STATIC_STATION_DIST, "0.000km");		// ��ʾ�г�����һվ�ľ���

				nStata = 0;
				
				m_EnumCurrentState = Enum_PreLeaveStation;
			}
#endif

			nStata = 0;

			m_EnumCurrentState = Enum_PreLeaveStation;
	}		
		break;
	case Enum_PreLeaveStation:	//Ԥ��վ
		{
			//Add by Johnny in 2017-07-27 10:16:20
			if(m_bPause)
			{
				break;
			}

			m_nLocationInfo = 4;

			m_STATIC_Cur_Door_State.SetWindowText(_T("�������"));

			nStata++;

			//Modify By Johnny for Test faster in 2017-07-24 14:18:02
			//if (nStata == 50)//2s ״̬
			if (nStata == 10)//2s ״̬
			{
				nStata = 0;
				m_EnumCurrentState = Enum_LeaveStation;
			}
		}
		break;
	case Enum_LeaveStation:		//��վ
		{
			//Add by Johnny in 2017-07-27 10:16:20
			if(m_bPause)
			{
				break;
			}

			m_nLocationInfo = 8;


			//nStata++;
			//if (nStata == 10)//2s ״̬
			//{
			//	nStata = 0;
			//	m_EnumCurrentState = Enum_UniformRun;
			//}
			//ƽ���ٶ��ź�Ӧ�ôﵽ����ٶȲŸ�����
			//Modify by Johnny in 2017-08-02 10:18:09
			if(m_nEvenVel == m_nCurrentSpeed)
			{
				nStata = 0;
				m_EnumCurrentState = Enum_UniformRun;
			}
		}
		break;
	case Enum_UniformRun:		//������ʻ
		{

		}
		break;
	case Enum_PreComStation:	//Ԥ��վ
		{
			//Ԥ��վ -> ��վ���жϱ�׼Ϊ�ٶȺ;���
			
			//Add by Johnny in 2017-07-27 10:16:20
			if(m_bPause)
			{
				break;
			}

			//�޸�Ϊ����m_nCurZoneDistΪ0
			if ( (m_nCurZoneDist <= 10 || m_nCurrentSpeed <= 10 ) && m_nCurZoneDist >= 0 )
			{
				m_nCurZoneDist = 0;
				m_nCurrentSpeed = 0;
				m_pDistance.SetPos(m_nZoneDist);
				m_EnumCurrentState = Enum_ComStation;

			}
			else if(m_nCurZoneDist < 0)
			{

				m_nCurZoneDist = 0;
				m_nCurrentSpeed = 0;
				m_pDistance.SetPos(m_nZoneDist);
				m_EnumCurrentState = Enum_ComStation;

			}

		}

		break;
	case Enum_ComStation:		//��վ
		{
			//Add by Johnny in 2017-07-27 10:16:20
			if(m_bPause)
			{
				break;
			}

#if JUST_RUN
			if(nStata == 0)
			{
				m_nLocationInfo = 0;

				g_NxtIndex = m_nNextStation;

				m_nNextStation  = -1;	
			}

#endif
			nStata++;
			
			//ֻҪ��һ�β�������
			if(nStata == 3)
			{
#if 0
				if(m_bIsUpDirect)
				{
					if(g_NxtIndex + 1 < m_vtRunStationInfo.size())
					{
						m_nNextStation = g_NxtIndex + 1;
					}
					
				}
				else
				{
					if(g_NxtIndex - 1 >= 0)
					{
						m_nNextStation = g_NxtIndex - 1;
					}					
				}
#endif


				//�ٶȣ����� ��0
				m_nCurrentSpeed = 0;
				SetDlgItemText(IDC_STATIC_LEAV_STATION, _T("0 km/h"));

				//���� ��0
				m_nLastStationDist = 0;		// ��վ֮������ź���0
				CString strShow;
				strShow.Format("%.3f km", m_nLastStationDist * 1.0 / 1000);
				SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);

				//Add by Johnny in 2017-07-24 14:38:44
				//��վ֮�󣬵�ǰվ->��һվ����һվ����һվ��

#if 0
				m_nFrontStation = m_nNextStation;
				g_CurIndex = m_nFrontStation;


				CString szFrontStation = m_vtRunStationInfo.at(m_nFrontStation).szStationName;
				m_sFrontStation.SetCenterText(szFrontStation);
#endif

#if 0
				//����Listѡ��Ŀ��
				if (m_bIsUpDirect)
				{
					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(m_nFrontStation);
				}
				else
				{
					int ncount = (int)m_vtRunStationInfo.size();
					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(ncount-m_nFrontStation-1);
				}

				//����
				m_nLastSpeed = 0;
				m_nSumDist = 0;
#endif

			}
			if (nStata == 10)//2s ״̬
			{
				nStata = 0;
				m_EnumCurrentState = Enum_OpenDoor;

			}
		}
		break;
	case Enum_OpenDoor:			//����
		{
			nStata ++;

			//��ֹ������ѭ��
			if(nStata > OPEN_DOOR_TIME * 4)// OPEN_DOOR_TIME * Send_Packet_Time
			{
				nStata = 0;
			}

			//Add by Johnny in 2017-07-27 10:16:20
			if(m_bPause)
			{
				break;
			}

			//ATC ����
			if(1 == nStata)
			{
				if(m_bIsUpDirect)
				{
					//if(g_NxtIndex + 1 < m_vtRunStationInfo.size())
					//{
					//	m_nNextStation = g_NxtIndex + 1;
					//	g_NxtIndex = m_nNextStation;
					//}

					//Del by Johnny in 2018-01-02 17_13

#if 0
					m_nFrontStation = m_nNextStation;
					g_CurIndex = m_nFrontStation;
#endif

					int nSize = m_vtRunStationInfo.size();


					//��ֹ���
					//Add  by Johnny in 2017-07-26 20:11:40
					if(nSize -1 == m_nFrontStation)//index 0
					{
						OnBnClickedRadioUp();
					}
					else
					{
						//m_nNextStation == 0
						m_nNextStation  = g_NxtIndex + 1;
						g_NxtIndex = m_nNextStation;
					}

					//�ٶȣ����� ��0
					m_nCurrentSpeed = 0;
					SetDlgItemText(IDC_STATIC_LEAV_STATION, _T("0 km/h"));

					//���� ��0
					m_nLastStationDist = 0;		// ��վ֮������ź���0
					CString strShow;
					strShow.Format("%.3f km", m_nLastStationDist * 1.0 / 1000);
					SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);

					//����Listѡ��Ŀ��
					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(m_nFrontStation);

					//����
					m_nLastSpeed = 0;
					m_nSumDist = 0;

					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(m_nFrontStation);

					CString szNxtStation = m_vtRunStationInfo.at(m_nNextStation).szStationName;
					m_sNextStation.SetCenterText(szNxtStation);


					//left
					if(1 == g_bUpDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Left.SetCheck(TRUE);
						m_BTN_Next_Open_Right.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("��࿪��"));
					}
					//left right
					else if(2 == g_bUpDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(TRUE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("���࿪��"));

					}
					//right
					else if(0 == g_bUpDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("�Ҳ࿪��"));
					}


					m_nZoneDist = 0;
						
					//Modify by Johnny in 2017-08-04 11:17:43
					for (int i = m_nFrontStation; i < m_nNextStation; i++)
					{
						//m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;
						m_nZoneDist += m_vtRunStationInfo.at(i + 1).nDistance;
					}

					m_nCurZoneDist = m_nZoneDist;
					m_pDistance.SetRange(0, m_nZoneDist);

					m_nLastStationDist = 0;
					SetDlgItemText(IDC_STATIC_STATION_DIST, "0.000km");		// ��ʾ�г�����һվ�ľ���
					

					//-------------------------------------------------

					//1 ������
					if(1 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bATC_Open_A    = 1;
					}
					//0	������
					else if(0 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bATC_Open_B    = 1;
					}
					//2	���࿪��
					else if(2 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bATC_Open_A    = 1;
						m_bATC_Open_B    = 1;
					}

				}
				else // ����
				{
					//if(g_NxtIndex - 1 >= 0)
					//{
					//	m_nNextStation = g_NxtIndex - 1;
					//	g_NxtIndex = m_nNextStation;
					//}

					int ncount = (int)m_vtRunStationInfo.size();

					if(m_nNextStation == -1)
					{
						m_nFrontStation = g_NxtIndex;
					}
					else
					{
						m_nFrontStation = m_nNextStation;
					}
					
					if(m_nFrontStation != -1)
					{
						g_CurIndex = m_nFrontStation;
					}
					
					//��ֹ���
					//Add  by Johnny in 2017-07-26 20:11:40
					if(0 == m_nFrontStation)//index 0
					{
						OnBnClickedRadioUp();
					}
					else
					{
						m_nNextStation  = g_NxtIndex - 1;
						g_NxtIndex = m_nNextStation;
					}

					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(ncount-m_nFrontStation-1);


					//left
					if(1 == g_bDnDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Left.SetCheck(TRUE);
						m_BTN_Next_Open_Right.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("��࿪��"));
					}
					//left right
					else if(2 == g_bDnDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(TRUE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("���࿪��"));

					}
					//right
					else if(0 == g_bDnDireOpenSide[m_nNextStation])
					{
						m_BTN_Next_Open_Right.SetCheck(TRUE);	
						m_BTN_Next_Open_Left.SetCheck(FALSE);

						m_STATIC_Next_Open_Side.SetWindowText(_T("�Ҳ࿪��"));
					}

					m_nZoneDist = 0;


					for (int i = m_nFrontStation; i > m_nNextStation; i--)
					{
						//Modify by Johnny in 2017-08-04 11:17:43
						//m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;

						m_nZoneDist += m_vtRunStationInfo.at(i).nDistance;
					}

					m_nCurZoneDist = m_nZoneDist;
					m_pDistance.SetRange(0, m_nZoneDist);

					m_nLastStationDist = 0;
					SetDlgItemText(IDC_STATIC_STATION_DIST, "0.000km");		// ��ʾ�г�����һվ�ľ���


					//�ٶȣ����� ��0
					m_nCurrentSpeed = 0;
					SetDlgItemText(IDC_STATIC_LEAV_STATION, _T("0 km/h"));


					//����
					m_nLastSpeed = 0;
					m_nSumDist = 0;

					((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(m_nFrontStation);

					CString szNxtStation = m_vtRunStationInfo.at(m_nNextStation).szStationName;
					m_sNextStation.SetCenterText(szNxtStation);


					//-------------------------------------------------------

					//1 ������
					if(1 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bATC_Open_A    = 1;
					}
					//0	������
					else if(0 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bATC_Open_B    = 1;
					}
					//2	���࿪��
					else if(2 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bATC_Open_A    = 1;
						m_bATC_Open_B    = 1;
					}

				}

				SetTimer(IDT_TIME_KILL_ATC_OPEN_DOOR, 3000, NULL);
				

			}
			//[ 3 , OPEN_DOOR_TIME ), �����ź�
			else if(3 <= nStata && nStata < OPEN_DOOR_TIME)
			{
				

				if( ( m_nNextStation == m_nEndStation || m_nNextStation == m_nStartStation )  && m_nTimeCount != 0 )
				{
					//�������֮�������������л�Ϊ����

					if (IsDlgButtonChecked(IDC_RADIO_UP))
					{
						CheckDlgButton(IDC_RADIO_UP, FALSE);
						CheckDlgButton(IDC_RADIO_DWON, TRUE);
					}
					else
					{
						CheckDlgButton(IDC_RADIO_UP, TRUE);
						CheckDlgButton(IDC_RADIO_DWON, FALSE);
					}

					//������һվ��Ϣ
					OnBnClickedRadioUp();
				}
				//Modify by Johnny in 2017-08-18 09:50:55
				//Set Open Side

				if(m_bIsUpDirect)
				{
					//1 ������
					if(1 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 1;
						m_bCloseLeftDoor  = 0;
						m_bClosedLeft     = 0;

						m_STATIC_Cur_Door_State.SetWindowText(_T("��࿪��"));
					}
					//0	������
					else if(0 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenRightDoor  = 1;
						m_bCloseRightDoor = 0;
						m_bClosedRight    = 0;

						m_STATIC_Cur_Door_State.SetWindowText(_T("�Ҳ࿪��"));
					}
					//2	���࿪��
					else if(2 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 1;
						m_bCloseLeftDoor  = 0;
						m_bClosedLeft     = 0;

						m_bOpenRightDoor  = 1;
						m_bCloseRightDoor = 0;
						m_bClosedRight    = 0;

						m_STATIC_Cur_Door_State.SetWindowText(_T("���࿪��"));
					}

				}
				else // ����
				{


					//1 ������
					if(1 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 1;
						m_bCloseLeftDoor  = 0;
						m_bClosedLeft     = 0;

						m_STATIC_Cur_Door_State.SetWindowText(_T("��࿪��"));
					}
					//0	������
					else if(0 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenRightDoor  = 1;
						m_bCloseRightDoor = 0;
						m_bClosedRight    = 0;

						m_STATIC_Cur_Door_State.SetWindowText(_T("�Ҳ࿪��"));
					}
					//2	���࿪��
					else if(2 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor   = 1;
						m_bCloseLeftDoor  = 0;
						m_bClosedLeft     = 0;

						m_bOpenRightDoor  = 1;
						m_bCloseRightDoor = 0;
						m_bClosedRight    = 0;

						m_STATIC_Cur_Door_State.SetWindowText(_T("���࿪��"));
					}
		
				}

#if 0
				if(IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR) == 1)
				{
					m_bOpenLeftDoor  = 1;
					m_bCloseLeftDoor = 0;
					m_bClosedLeft    = 0;
				}

				if(IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR) == 1)
				{
					m_bOpenRightDoor  = 1;
					m_bCloseRightDoor = 0;
					m_bClosedRight    = 0;
				}
#endif
				m_nTimeCount   = 0;

				//Del by Johnny in 2018-01-02 16_49
				//m_nZoneDist    = 0;
				//m_nCurZoneDist = 0;

				m_nTimeCountWA = 0;
				m_pDistance.SetPos(0);
			}

			//[ OPEN_DOOR_TIME�� OPEN_DOOR_TIME * 2��, ��վͣ��

			if( OPEN_DOOR_TIME <= nStata )
			{

				// Add by Johnny in 2017-08-18 11:44:30
				// �������ź���д����

				if(m_bIsUpDirect)
				{
					//1 ������
					if(1 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor  = 0;
						m_bCloseLeftDoor = 0;
						m_bClosedLeft    = 0;

						
					}
					//0	������
					else if(0 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenRightDoor  = 0;
						m_bCloseRightDoor = 0;
						m_bClosedRight	  = 0;

						
					}
					//2	���࿪��
					else if(2 == g_bUpDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor  = 0;
						m_bCloseLeftDoor = 0;
						m_bClosedLeft    = 0;

						m_bOpenRightDoor  = 0;
						m_bCloseRightDoor = 0;
						m_bClosedRight	  = 0;

					}

				}
				else // ����
				{
					//1 ������
					if(1 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor  = 0;
						m_bCloseLeftDoor = 0;
						m_bClosedLeft    = 0;

					}
					//0	������
					else if(0 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenRightDoor  = 0;
						m_bCloseRightDoor = 0;
						m_bClosedRight	  = 0;

					}
					//2	���࿪��
					else if(2 == g_bDnDireOpenSide[m_nFrontStation])
					{
						m_bOpenLeftDoor  = 0;
						m_bCloseLeftDoor = 0;
						m_bClosedLeft    = 0;

						m_bOpenRightDoor  = 0;
						m_bCloseRightDoor = 0;
						m_bClosedRight	  = 0;

					}

				}

#if 0
				if(IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR) == 1)
				{
					m_bOpenLeftDoor  = 0;
					m_bCloseLeftDoor = 0;
					m_bClosedLeft    = 0;
				}

				if(IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR) == 1)
				{
					m_bOpenRightDoor  = 0;
					m_bCloseRightDoor = 0;
					m_bClosedRight	  = 0;
				}
#endif
				if(nStata == OPEN_DOOR_TIME * 4)
				{
					nStata = 0;
					m_EnumCurrentState = Enum_CloseDoor;		
				}



				//if(nStata == 6)
				//{
				//	//��ʾ��ǰվ�� ��һվ��
				//	CString szFrontStation = m_vtRunStationInfo.at(m_nFrontStation).szStationName;
				//	CString szNextStation  = m_vtRunStationInfo.at(m_nNextStation).szStationName;

				//	m_sFrontStation.SetCenterText(szFrontStation);
				//	m_sNextStation.SetCenterText(szNextStation);
				//	SetDlgItemText(IDC_EDIT_SET_STATION, szNextStation);

				//	//�������
				//	if (m_bIsUpDirect)
				//	{
				//		//Modify by Johnny in 2017-08-04 11:17:43
				//		for (int i = m_nFrontStation; i < m_nNextStation; i++)
				//		{
				//			//m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;
				//			m_nZoneDist += m_vtRunStationInfo.at(i + 1).nDistance;
				//		}
				//	}
				//	else
				//	{
				//		for (int i = m_nFrontStation; i > m_nNextStation; i--)
				//		{
				//			//Modify by Johnny in 2017-08-04 11:17:43
				//			//m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;

				//			m_nZoneDist += m_vtRunStationInfo.at(i).nDistance;
				//		}
				//	}

				//	m_nCurZoneDist = m_nZoneDist;
				//	m_pDistance.SetRange(0, m_nZoneDist);

				//	m_nLastStationDist = 0;
				//	SetDlgItemText(IDC_STATIC_STATION_DIST, "0.000km");		// ��ʾ�г�����һվ�ľ���
				//}

			}		
			
		}		
		break;
	default:
		break;
	}

	if(MAN_ATUO == m_nRunMode)
	{
		int x = 3;
	}
	//��վ�����١�Ԥ��վ
	else if (m_EnumCurrentState == Enum_LeaveStation || m_EnumCurrentState == Enum_UniformRun || m_EnumCurrentState == Enum_PreComStation)
	{
		//��ʱ��
		if(!m_bPause)
		{
			m_nTimeCount++;
		}

		
		//����һվ����
		int nDist;

		//Modify By Johnny in 2017-07-25 20:07:43

		//����һվ����
		nDist = m_nZoneDist - m_nCurZoneDist;

		//��վ��ʱ�������ٶȸı�
		if(Enum_LeaveStation == m_EnumCurrentState)
		{
			GetDlgItem(IDC_BTN_ADDVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SUBVEL)->EnableWindow(FALSE);
		}

		if ( nDist <= m_nDisForAccl) //��վ�����
		{
			//�������
			CalcRunSpeed(1);
		}				
		else if ( m_nCurZoneDist <= m_nDisForAccl )//��վǰ����
		{
			m_nLocationInfo = 1;

			CalcRunSpeed(2);

			m_EnumCurrentState = Enum_PreComStation;
		}	
		else//����
		{
			//�����ٶȵ���
			CalcRunSpeed(0);
		}

		if ( nDist > 0 && nDist < DISTACNE_LEAVE )
		{
			m_nLocationInfo = 8;
		}

		else if ( nDist >= DISTACNE_LEAVE )
		{
			m_nLocationInfo = 0;

			if(m_nFrontStation != -1)
			{
				g_CurIndex = m_nFrontStation;
				m_nFrontStation = -1;
			}			
			

		}
		//�뵽վ���� 150m
		if ( m_nCurZoneDist <=  DISTACNE_COM_STATION)//��վǰ����
		{
			m_nLocationInfo = 2;

			if(m_nFrontStation != -1)
			{
				g_CurIndex = m_nFrontStation;
			}
			m_nFrontStation = m_nNextStation;


			CString strFrontSta;
			strFrontSta = m_vtRunStationInfo.at(m_nFrontStation).szStationName;
			m_sFrontStation.SetCenterText(strFrontSta);
		}


		if (m_nCurZoneDist == 0)//��վ
		{
			m_nLastStationDist = 0;		// ��վ֮������ź���0
			SetDlgItemText(IDC_STATIC_LEAV_STATION, _T("0 km/h"));
			//m_nCurrentSpeed = 0;
			//m_EnumCurrentState = Enum_ComStation;
		}

		//Modify by Johnny in 2017-07-25 20:10:34
		//int nCurPos = m_nZoneDist - m_nCurZoneDist;
		int nCurPos = m_nLastStationDist;
		m_pDistance.SetPos(nCurPos);
	}

	//��վ�˿��Խ���
	if(Enum_ComStation == m_EnumCurrentState)
	{
		GetDlgItem(IDC_BTN_ADDVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SUBVEL)->EnableWindow(TRUE);
	}

	if (m_EnumCurrentState != m_EnumOldState)
	{
		m_EnumOldState = m_EnumCurrentState;
		GetDlgItem(IDC_STATIC_RUN_CLOSE_DOOR)->RedrawWindow();
		GetDlgItem(IDC_STATIC_PRE_STATION)->RedrawWindow();
		//GetDlgItem(IDC_STATIC_LEAV_STATION)->RedrawWindow();
		GetDlgItem(IDC_STATIC_PRE_COM_STION)->RedrawWindow();
		GetDlgItem(IDC_STATIC_RUN_COM_STATION)->RedrawWindow();
	}	
}

void CPageRunData::OnTimer(UINT_PTR nIDEvent)
{

	switch (nIDEvent)
	{
	case TIME_RUN_STATION:
		{
			OnRunStation();
		}		
		break;
	case TIME_RE_RUN_STATION:
		{
			KillTimer(TIME_RE_RUN_STATION);
			SetTimer(TIME_RUN_STATION, TIME_RUN_INTERVAL, NULL);
		}
		break;
	case IDT_TIME_EMC_PLAYBC:
		{
			KillTimer(IDT_TIME_EMC_PLAYBC);
			((CButton *)GetDlgItem(IDC_BTN_START_EMC))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_BTN_STOP_EMC))->EnableWindow(TRUE);
			//m_bPlayBC = 0;
			m_nAnounceIndex = 0;
		}
		break;
	case IDT_TIME_EMC_STOPBC:
		{
			KillTimer(IDT_TIME_EMC_STOPBC);
			((CButton *)GetDlgItem(IDC_BTN_START_EMC))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_BTN_STOP_EMC))->EnableWindow(TRUE);
			m_bPlayBC = 1;
		}
		break;
	case IDT_TIME_ARRIVE:
		{
			KillTimer(IDT_TIME_ARRIVE);
			((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(TRUE);
			m_bArriveBC = 0;
		}
		break;
	case IDT_TIME_LEAVE:
		{
			KillTimer(IDT_TIME_LEAVE);
			((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(TRUE);
			m_bLeaveBC = 0;
		}
		break;
	case IDT_TIME_JUMP_UP:
		{
			KillTimer(IDT_TIME_JUMP_UP);
			//Add by Johnny in 2017-08-01 17:14:39
			GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(TRUE);
			m_bJumpUp = 0;
		}
		break;
	case IDT_TIME_JUMP_DOWN:
		{
			KillTimer(IDT_TIME_JUMP_DOWN);
			//Add by Johnny in 2017-08-01 17:14:39
			GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(TRUE);
			m_bJumpDown = 0;
		}
		//Modify by Johnny in2017-07-25 16:13:36
		break;
	case IDT_TIME_DATETIME_VALID:
		{
			KillTimer(IDT_TIME_DATETIME_VALID);
			m_bDateTimeValid = 0;
		}
		//Modify by Johnny in2017-07-25 16:13:36
		break;
	case IDT_TIME_NEXT_STATION:
		{
			
			m_nNextStation = 0;

			KillTimer(IDT_TIME_NEXT_STATION);
		}
		//Add by Johnny in 2017-07-25 16:12:07
		break;
	case IDT_TIME_START_STOP_STATION:
		{
			KillTimer(IDT_TIME_START_STOP_STATION);
			m_nStartStation = -1;
			m_nEndStation = -1;
		}
		break;
	
	//Add by Johnny in 2017-08-02 14:25:41
	case IDT_TIME_KILL_OPEN_DOOR:
		{
			KillTimer(IDT_TIME_KILL_OPEN_DOOR);
			m_bOpenLeftDoor  = 0;
			m_bOpenRightDoor = 0;

			//SetTimer(IDT_TIME_SET_ALL_CLOSE_DOOR, 3000);
		}
		break;
	case IDT_TIME_KILL_CLOSE_DOOR:
		{
			KillTimer(IDT_TIME_KILL_CLOSE_DOOR);
			m_bCloseLeftDoor = 0;
			m_bCloseRightDoor= 0;

			SetTimer(IDT_TIME_SET_ALL_CLOSE_DOOR, 3000, NULL);
		}
		break;

	case IDT_TIME_SET_ALL_CLOSE_DOOR:
		{
			KillTimer(IDT_TIME_SET_ALL_CLOSE_DOOR);
			
			m_bClosedLeft  = 1;
			m_bClosedRight = 1;
		}
		break;

	case IDT_TIME_KILL_ATC_OPEN_DOOR:
		{
			KillTimer(IDT_TIME_KILL_ATC_OPEN_DOOR);

			m_bATC_Open_A = 0;
			m_bATC_Open_B = 0;
		}
	case IDT_TIME_KILL_ATC_CLOSE_DOOR:
		{			
			KillTimer(IDT_TIME_KILL_ATC_CLOSE_DOOR);

			m_bATC_Close_A = 0;
			m_bATC_Close_B = 0;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CPageRunData::CalcRunSpeed(int nSpeed)
{

	//��200ms =>>  300ms  :  0.2 => 0.33
	float fDistance = 0;	  //����
	int nCurVel     = 0;	  //�ٶ�

	float s0;
	s0 = (m_nLastSpeed + m_nCurrentSpeed) / 3.6 / 2 * EACH_PACKET_TIME;
	m_nSumDist += s0;

	//����ļ����޸�Ϊ�����ٶ��ۼӵĹ��̣�Modify by Johnny in 2017-07-27 10:13:16
	switch (nSpeed)
	{


	case 0://ƽ���ٶ�
		{
			if(m_bPause)
			{
				break;
			}

#if 0
			// s = s0 + v * t			
			//fDistance = m_nDisForAccl + (m_nEvenVel * 1.0 / 3.6)*( m_nTimeCount * 0.2 - m_nEvenVel / 10);
			fDistance = m_nDisForAccl + (m_nEvenVel * 1.0 / 3.6)*( m_nTimeCount * EACH_PACKET_TIME - m_nEvenVel / A_C_C_E_L);
			//��һվ����
			m_nLastStationDist = fDistance;
			//����һվ����
			m_nCurZoneDist = m_nZoneDist - fDistance;
#endif
			m_nLastStationDist = m_nSumDist;
			m_nCurZoneDist = m_nZoneDist - m_nSumDist;

			//m_nCurZoneDist -= m_nPerDist;
			nCurVel = m_nEvenVel;

			//��¼�����˶��Ľ���ʱ�䣺 ����ʱ�� + ����ʱ��
			m_nTimeCountWA = m_nTimeCount;
		}
		break;
	case 1://����
		{
			if(m_bPause)
			{
				break;
			}

#if 0
			// s0 = 1/2 * a * t^2
			fDistance = ACCEL*( (m_nTimeCount * EACH_PACKET_TIME) * ( m_nTimeCount * EACH_PACKET_TIME ) ) / 2;
			//��һվ����
			m_nLastStationDist = fDistance;
			//��һվ����
			m_nCurZoneDist = m_nZoneDist - fDistance;
#endif

			//��һվ����
			m_nLastStationDist = m_nSumDist;
			//��һվ����
			m_nCurZoneDist = m_nZoneDist - m_nSumDist;

			//ԭ��Ϊ m/(s * s) = km/(h * s) * 3.6  ===>>  v = a * t * 3.6
			nCurVel = ACCEL * ( m_nTimeCount * EACH_PACKET_TIME) * 3.6;
		}	
		break;
	case 2://����
		{
			if(m_bPause)
			{
				break;
			}

			// s = s0 * 2 + v * t
			//int nCount;
			float nCount;
			// t = v / a , v = MaxSpeed, a = 10km /(h * s),  ˢ�£�200 ms
			// ==>>  nCount = t * ( 1000 ms / 200 ms ), 1 s == 1000 ms	

			// �⽫���ٶ��޸�Ϊ���Ե��ڣ�Modify by Johnny in 2017-07-26 10:51:28
			//nCount = (m_nEvenVel / 10 ) * 5 - (m_nTimeCount - m_nTimeCountWA);

			nCount = (m_nEvenVel / A_C_C_E_L )  / EACH_PACKET_TIME - (m_nTimeCount - m_nTimeCountWA);

#if 0
			//����˼ά�������ȼ��١����ɡ��ȼ��١��Ĺ���
			fDistance = ACCEL*((nCount * EACH_PACKET_TIME) * (nCount * EACH_PACKET_TIME) ) / 2;

			//��һվ����
			m_nLastStationDist = m_nZoneDist - fDistance;
			//��һվ����
			m_nCurZoneDist = fDistance;
#endif

			//��һվ����
			m_nLastStationDist = m_nSumDist;
			//��һվ����
			m_nCurZoneDist = m_nZoneDist - m_nSumDist;

			//ԭ��Ϊ m/(s * s) = km/(h * s) * 3.6  ===>>  v = a * t * 3.6

			int tmp = ACCEL * (nCount * EACH_PACKET_TIME) * 3.6;

			// Add by Johnny in 2017-08-01 10:30:15�� ��ֹ���������ܴ����ֵ
			if(tmp <= 0)
			{
				//��վ
				//m_EnumCurrentState = Enum_ComStation;
			}
			else
			{
				nCurVel = tmp;
			}
			
		}
		break;
	}

	//��ƽ���ٶȼ����

	//float s0;
	//s0 = (m_nLastSpeed + m_nCurrentSpeed) / 3.6 / 2 * EACH_PACKET_TIME;
	//m_nSumDist += s0;

	CString strShowSum;
	
	
	strShowSum.Format(_T("   %.3f km  LastSpeed = %d, CurSpeed = %d  Open =     \n"), m_nSumDist / 1000, m_nLastSpeed, m_nCurrentSpeed);
	m_nLastSpeed = m_nCurrentSpeed; // km /h
	

#if 0
	fDistance = m_nCurZoneDist*1.0/1000;
	CString strShow, strShow1;
	// 	strShow.Format("%.3f km", fDistance);
	// 	SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);
	strShow.Format("%.3f km", m_nLastStationDist*1.0/1000);
	SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);		// ��ʾ�г�����һվ�ľ���
	strShow1.Format("%d km/h", nCurVel);
	SetDlgItemText(IDC_STATIC_LEAV_STATION, strShow1);
#endif

	//Modify By Johnny in 2017-07-25 14:12:38
	
	m_nCurrentSpeed = nCurVel;
	fDistance = m_nCurZoneDist * 1.0 / 1000;	
	CString strShow, strShow1;

	// strShow.Format("%.3f km", fDistance);
	// SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);

	strShow.Format("%.3f km", m_nLastStationDist * 1.0 / 1000);
	SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);		// ��ʾ�г�����һվ�ľ���
	strShow1.Format("%d km/h", m_nCurrentSpeed);
	SetDlgItemText(IDC_STATIC_LEAV_STATION, strShow1);

	//TRACE(strShow + strShowSum);	
	
}

void CPageRunData::CalDisForAccel()
{
	int nSec;
	float fDis;
	
	//Modify By Johnny in 2017-07-26 10:52:27
	//nSec = m_nEvenVel / 10;
	nSec = m_nEvenVel / A_C_C_E_L;

	fDis = ACCEL*(nSec * nSec)/2; //���پ���   1/2 * a * t^2

	m_nDisForAccl = fDis;

	fDis = (m_nEvenVel*1.0 / 3.6)*(20 - nSec);//20s��ȥ��������ʱ�����ߵľ���

	//��վǰ20��ľ���
	m_nDisForArri = m_nDisForAccl + fDis;

	//��ʼ��վǰ20�����
	m_nDisForLeave = m_nDisForArri;

	m_nPerDist = m_nEvenVel*1.0/3.6;
}

HBRUSH CPageRunData::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//Modify by Johnny in 2017-07-25 09:05:09
	//���Ϊ�ֶ�ģʽ���ý���һ�²���
	if(MAN_ATUO == m_nRunMode)
	{
		//HBRUSH hbr_Normal;
		//CWnd*  pWnd_Set_Normal;

		//IDC_STATIC_PRE_COM_STION
		//	IDC_STATIC_RUN_COM_STATION
		//	IDC_STATIC_PRE_STATION
		//	IDC_STATIC_RUN_CLOSE_DOOR
		//IDC_STATIC_STATION_DIST


		//pWnd_Set_Normal = GetDlgItem(IDC_STATIC_RUN_CLOSE_DOOR);
		//hbr_Normal = CDialog::OnCtlColor(pDC, pWnd_Set_Normal, nCtlColor);
		//pDC->SetBkColor(RUN_NONRE_CLOLOR);

		//pWnd_Set_Normal = GetDlgItem(IDC_STATIC_PRE_STATION);
		//hbr_Normal = CDialog::OnCtlColor(pDC, pWnd_Set_Normal, nCtlColor);
		//pDC->SetBkColor(RUN_NONRE_CLOLOR);

		//pWnd_Set_Normal = GetDlgItem(IDC_STATIC_PRE_COM_STION);
		//hbr_Normal = CDialog::OnCtlColor(pDC, pWnd_Set_Normal, nCtlColor);`
		//pDC->SetBkColor(RUN_NONRE_CLOLOR);

		//pWnd_Set_Normal = GetDlgItem(IDC_STATIC_RUN_COM_STATION);
		//hbr_Normal = CDialog::OnCtlColor(pDC, pWnd_Set_Normal, nCtlColor);
		//pDC->SetBkColor(RUN_NONRE_CLOLOR);

		return hbr;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_RUN_CLOSE_DOOR)
	{
		if (m_EnumCurrentState == Enum_CloseDoor)
		{
			pDC->SetBkColor(RUN_STATA_CLOLOR);
		}
		else
		{
			pDC->SetBkColor(RUN_NONRE_CLOLOR);		
		}
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PRE_STATION)
	{
		
		if (m_EnumCurrentState == Enum_PreLeaveStation || m_EnumCurrentState == Enum_PreLeaveStation)
		{
			pDC->SetBkColor(RUN_STATA_CLOLOR);
		}
		else
		{
			pDC->SetBkColor(RUN_NONRE_CLOLOR);		
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PRE_COM_STION)
	{
		if ( m_EnumCurrentState == Enum_PreComStation )
		{
			pDC->SetBkColor(RUN_STATA_CLOLOR);
		}			
		else
		{
			pDC->SetBkColor(RUN_NONRE_CLOLOR);
		}
		
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_RUN_COM_STATION)
	{

		if ( m_EnumCurrentState == Enum_OpenDoor )
		{
			pDC->SetBkColor(RUN_STATA_CLOLOR);
		}			
		else
		{
			pDC->SetBkColor(RUN_NONRE_CLOLOR);
		}
	}
	return hbr;
}

void CPageRunData::StartRunStation()
{
	m_EnumCurrentState = Enum_OpenDoor;

	if(m_bIsUpDirect)
	{

		m_nStartStation = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->GetCurSel();
		m_nEndStation   = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->GetCurSel();

		//Add By Johnny in 2017-07-25 15:22:48
		((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(m_nStartStation);	


	}
	else
	{
		m_nStartStation = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->GetCurSel();
		m_nEndStation   = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->GetCurSel();

		//Add By Johnny in 2017-07-25 15:22:48
		((CComboBox*)GetDlgItem(IDC_COMBO_STATION))->SetCurSel(17 - m_nStartStation);
	}


	m_nFrontStation = m_nStartStation;
	m_nNextStation  = m_nFrontStation;

	if(m_nFrontStation != -1)
	{
		g_CurIndex = m_nFrontStation;
	}
	if(m_nNextStation != -1)
	{
		g_NxtIndex = m_nNextStation;
	}
	
	


	m_bIsUpDirect = IsDlgButtonChecked(IDC_RADIO_UP);
	

	//if (m_nStartStation > m_nEndStation)
	//{
	//	MessageBox(_T("����վ���Ҫ������ʼվ���"), _T("��ʾ"));
	//	return;
	//}

	//distance accel
	CalDisForAccel();

	SetTimer(TIME_RUN_STATION, TIME_RUN_INTERVAL, NULL);

	//��ʾ��ǰվ�� ��һվ��
	CString szFrontStation;
	CString szNextStation;

//	if (m_bIsUpDirect)
	{
		SetDlgItemText(IDC_EDIT_STAT_STATION, m_vtRunStationInfo.at(m_nStartStation).szStationName);
		SetDlgItemText(IDC_EDIT_END_STATION, m_vtRunStationInfo.at(m_nEndStation).szStationName);
		m_nNextStation = m_nStartStation;

		//��ʾ��ǰվ�� ��һվ��
		szFrontStation = m_vtRunStationInfo.at(m_nStartStation).szStationName;
		szNextStation  = m_vtRunStationInfo.at(m_nNextStation).szStationName;
	}
	//else
	//{
	//	SetDlgItemText(IDC_EDIT_STAT_STATION, m_vtRunStationInfo.at(m_nEndStation).szStationName);
	//	SetDlgItemText(IDC_EDIT_END_STATION, m_vtRunStationInfo.at(m_nStartStation).szStationName);
	//	m_nNextStation = m_nEndStation;

	//	//��ʾ��ǰվ�� ��һվ��
	//	szFrontStation = m_vtRunStationInfo.at(m_nEndStation).szStationName;
	//	szNextStation  = m_vtRunStationInfo.at(m_nNextStation).szStationName;
	//}

	m_sFrontStation.SetCenterText(szFrontStation);
	m_sNextStation.SetCenterText(szNextStation);

	RunEnableWindow(FALSE);
}

void CPageRunData::PauseRunStation(BOOL bPause)
{
	//Modify��by Johnny in 2017-07-25 13:44:13

	//�ٶ�Ϊ0
	//��ͣ
	if(bPause)
	{
		//nStata = 0;
		m_bPause = 1;
		m_nLastSpeed = m_nCurrentSpeed;
		m_nCurrentSpeed = 0;
		//����Ϊ�ٶ�Ϊ0
		GetDlgItem(IDC_STATIC_LEAV_STATION)->SetWindowText(_T("0 km/h"));

		//��վ�����š����š�Ԥ��վ  ����
		//��վ�����١�����վ ����
		if(
			Enum_LeaveStation  == m_EnumCurrentState ||		//��վ
		    Enum_UniformRun    == m_EnumCurrentState ||		//����
		    Enum_PreComStation == m_EnumCurrentState		//����վ
		   )
		{
			GetDlgItem(IDC_BTN_ADDVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SUBVEL)->EnableWindow(FALSE);
		}else
		{
			GetDlgItem(IDC_BTN_ADDVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SUBVEL)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_RADIO_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DWON)->EnableWindow(TRUE);
			
	}
	else
	{
		CalDisForAccel();
		m_bPause = 0;
		//����ѡ��Ŀ��
		m_nCurrentSpeed = m_nLastSpeed;
		//StartRunStation();
	}

	//�����ţ�

/*	if (bPause)
	{
		KillTimer(TIME_RUN_STATION);
	}
	else
	{
		SetTimer(TIME_RUN_STATION, TIME_RUN_INTERVAL, NULL);
	}*/	
}

void CPageRunData::OnBnClickedBtnAppry()
{
 	m_nNextStation = NextOrUpStation(2);
 	CString szNextStation = m_vtRunStationInfo.at(m_nNextStation).szStationName;
 	m_sNextStation.SetCenterText(szNextStation);

	//�������
	if (m_bIsUpDirect)
	{
		for (int i = m_nFrontStation; i < m_nNextStation; i++)
		{
			m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;
		}
	}
	else
	{
		for (int i = m_nFrontStation; i > m_nNextStation; i--)
		{
			m_nZoneDist += m_vtRunStationInfo.at(m_nNextStation).nDistance;
		}
	}

	m_nCurZoneDist = m_nZoneDist;

	m_pDistance.SetRange(0, m_nZoneDist);

	CString strShow;
	strShow.Format("%.3f km", m_nCurZoneDist * 1.0 / 1000);
	SetDlgItemText(IDC_STATIC_STATION_DIST, strShow);


}

void CPageRunData::OnBnClickedBtnAddvel()
{
	CString strVel;

	m_nEvenVel += 10;

	if (m_nEvenVel >= 150)
	{
		m_nEvenVel = 150;
		((CButton *)GetDlgItem(IDC_BTN_ADDVEL))->EnableWindow(FALSE);
	}

	if (m_nEvenVel == 20)
	{
		((CButton *)GetDlgItem(IDC_BTN_SUBVEL))->EnableWindow(TRUE);
	}

	strVel.Format("%d km/h", m_nEvenVel);
	((CStatic *)GetDlgItem(IDC_STATIC_VEL))->SetWindowText(strVel);

	CalDisForAccel();
}

void CPageRunData::OnBnClickedBtnSubvel()
{
	CString strVel;

	m_nEvenVel -= 10;

	if (m_nEvenVel <= 0)
	{
		m_nEvenVel = 10;
		((CButton *)GetDlgItem(IDC_BTN_SUBVEL))->EnableWindow(FALSE);
	}

	if (m_nEvenVel == 140)
	{
		((CButton *)GetDlgItem(IDC_BTN_ADDVEL))->EnableWindow(TRUE);
	}

	strVel.Format("%d km/h", m_nEvenVel);
	((CStatic *)GetDlgItem(IDC_STATIC_VEL))->SetWindowText(strVel);

	CalDisForAccel();
}

void CPageRunData::OnNMClickListBc(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   	int nCount = m_ListBc.GetItemCount();
   	for (int i=0; i<nCount; i++)
   	{
   		if (m_ListBc.GetCheck(i))
   			m_ListBc.SetCheck(i, FALSE);
   	}

 	int nMake = m_ListBc.GetSelectionMark();
 	if (nMake != -1)
 	{
 		m_ListBc.SetCheck(nMake, TRUE);
 	}

	*pResult = 0;
}

void CPageRunData::OnBnClickedBtnStationUp()
{	
	m_bJumpUp = 1;
	m_bJumpDown = 0;
	//Add by Johnny in 2017-08-01 17:15:28
	GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(FALSE);

	SetTimer(IDT_TIME_JUMP_UP, 2000, NULL);
	NextOrUpStation(0);
}

void CPageRunData::OnBnClickedBtnStationNext()
{
	m_bJumpDown = 1;
	m_bJumpUp = 0;
	//Add by Johnny in 2017-08-01 17:15:28
	GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(FALSE);

	SetTimer(IDT_TIME_JUMP_DOWN, 2000, NULL);
	NextOrUpStation(1);
}

// 0.up 1.next 2.get
int CPageRunData::NextOrUpStation(int nNext)
{
	static int nTempNextStation = m_nNextStation;
	static int nTempStartStation = m_nStartStation;
	static int nTempEndStation = m_nEndStation;
	int nTempCurrentStation = m_nFrontStation;

//	TRACE("nTempNextStation = %d \n", nTempNextStation);
	if (nNext==0)
	{
		if (m_bIsUpDirect)
		{
			nTempNextStation = nTempNextStation-1;
			if (nTempNextStation<=nTempStartStation)
			{
				nTempNextStation = nTempStartStation;
			}	
			if (nTempCurrentStation>=nTempNextStation)
			{
				nTempNextStation = nTempCurrentStation+1;
			}
		}
		else
		{
			nTempNextStation=nTempNextStation+1;
			if (nTempNextStation>=nTempEndStation)
			{
				nTempNextStation = nTempEndStation;
			}
			if (nTempCurrentStation<=nTempNextStation)
			{
				nTempNextStation = nTempCurrentStation-1;
			}
		}
	}
	else if (nNext==1)
	{
		if (m_bIsUpDirect)
		{
			nTempNextStation = nTempNextStation+1;
			if (nTempNextStation>=nTempEndStation)
			{
				nTempNextStation = nTempEndStation;
			}			
		}
		else
		{
			nTempNextStation=nTempNextStation-1;
			if (nTempNextStation<=nTempStartStation)
			{
				nTempNextStation = nTempStartStation;
			}			
		}
	}
	else if (nNext==2)
	{
// 		if (nTempNextStation==0)
// 		{
// 			if (m_bIsUpDirect)
// 			{
// 				nTempNextStation=m_nNextStation+1;
// 			}
// 			else
// 			{
// 				nTempNextStation=m_nNextStation-1;
// 			}
// 		}		
		return nTempNextStation;
	}
	CString szNextStation = m_vtRunStationInfo.at(nTempNextStation).szStationName;
	SetDlgItemText(IDC_EDIT_SET_STATION, szNextStation);
	return 0;
}

void CPageRunData::OnBnClickedBtnRun()
{
	//StartRunStation();

	m_EnumCurrentState = Enum_OpenDoor;
}

void CPageRunData::RunEnableWindow(BOOL bEnable)
{

	//Modify By Johnny in 2017-07-25 14:24:29
	//GetDlgItem(IDC_COMBO_SECTION_STATA)->EnableWindow(bEnable);
	//GetDlgItem(IDC_COMBO_SECTION_END)->EnableWindow(bEnable);
	
	//Modify by Johnny in 2017-07-26 09:21:03
	//GetDlgItem(IDC_BTN_ADDVEL)->EnableWindow(bEnable);
	//GetDlgItem(IDC_BTN_SUBVEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_LINEID)->EnableWindow(bEnable);

	GetDlgItem(IDC_RADIO_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_DWON)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_START_RUN)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BTN_APPRY)->EnableWindow(!bEnable);

	if(MAN_ATUO == m_nRunMode)
	{
		GetDlgItem(IDC_COMBO_SECTION_STATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SECTION_END)->EnableWindow(TRUE);
	}
}

void CPageRunData::OnBnClickedBtnStartEmc()
{
	CString szEmcId;
	UINT iSel = -1;
	for (int i = 0; i < m_ListBc.GetItemCount(); i++)
	{
		if(m_ListBc.GetCheck(i))
		{
 			szEmcId = m_ListBc.GetItemText(i, 0);
			iSel = i;
			break;
		}
	}

	if (-1 == iSel)
	{
		MessageBox(_T("����ѡ������㲥"), _T("��ʾ"), MB_OK);
		return;
	}
	else
	{
		//m_bAnounceFlag=TRUE;
		m_bPlayBC = true;
		m_nAnounceIndex=_ttoi(szEmcId);

		if (m_bEmcPulse)
		{
			//((CButton *)GetDlgItem(IDC_BTN_START_EMC))->EnableWindow(FALSE);
			//((CButton *)GetDlgItem(IDC_BTN_STOP_EMC))->EnableWindow(FALSE);
			//Modify by Johnny in 2017-08-18 17:14:19

			//SetTimer(IDT_TIME_EMC_PLAYBC, TIMEPULSE, NULL);
		}
	}
}

void CPageRunData::OnBnClickedBtnStopEmc()
{
	m_bPlayBC = FALSE;
	m_nAnounceIndex = 0;
// 	if (m_bEmcPulse)
// 	{
// 		((CButton *)GetDlgItem(IDC_BTN_START_EMC))->EnableWindow(FALSE);
// 		((CButton *)GetDlgItem(IDC_BTN_STOP_EMC))->EnableWindow(FALSE);
// 		SetTimer(IDT_TIME_EMC_STOPBC, TIMEPULSE, NULL);
// 	}
}

RunStationEnum CPageRunData::GetCurrentStationInfo(RunStationInfo *pStationInfo)
{
	int nSize = m_vtRunStationInfo.size();

	pStationInfo->bLineId		  =	 ((CComboBox*)GetDlgItem(IDC_COMBO_LINEID))->GetCurSel();
	pStationInfo->bUpOrDownStream =	 m_bIsUpDirect;

	pStationInfo->nLocationInfo   =   m_nLocationInfo;

	//Add by Johnny in 2017-12-14 08:53:04
	//ʹ������վ���룬�Խ��Ƶ���������������
	int nCurCode = 0;
	int nNxtCode = 0;
	int nStartCode = 0;
	int nEndCode = 0;

	pStationInfo->ATC_Close_A = m_bATC_Close_A;
	pStationInfo->ATC_Close_B = m_bATC_Close_B;

	pStationInfo->ATC_Open_A  = m_bATC_Open_A;
	pStationInfo->ATC_Open_B  = m_bATC_Open_B;

	int nCurIndex= m_nFrontStation;
	int nNxtIndex= m_nNextStation;

	CString strshow;
	strshow.Format(_T("LocationInfo = %d Statu:%d Cur = %d Nxt = %d Start = %d End =%d dist = %d \n"), m_nLocationInfo, m_EnumCurrentState, m_nFrontStation, m_nNextStation, m_nStartStation, m_nEndStation, m_nZoneDist - m_nCurZoneDist);
	OutputDebugString(strshow);


	if(m_nFrontStation < 0 || m_nFrontStation >= nSize)
	{
		nCurIndex = g_CurIndex;
		//return (RunStationEnum)-1;
	}
	if(m_nNextStation  < 0 || m_nNextStation  >= nSize)
	{
		nNxtIndex= g_NxtIndex;

		//return (RunStationEnum)-1;
	}
	if(m_nStartStation < 0 || m_nStartStation >= nSize)
	{
		return (RunStationEnum)-1;

	}
	if(m_nEndStation   < 0 || m_nEndStation >= nSize)
	{
		return (RunStationEnum)-1;
	}

	if(m_bIsUpDirect)
	{
		if(nCurIndex == -1)
		{
			nCurCode = 0;
		}
		else
		{
			nCurCode   = m_vtRunStationInfo.at(nCurIndex).nUpCode;
		}

		if(nNxtIndex == -1)
		{
			nNxtCode = 0;
		}
		else
		{
			nNxtCode   = m_vtRunStationInfo.at(nNxtIndex).nUpCode;
		}
		
		nStartCode = m_vtRunStationInfo.at(m_nStartStation).nUpCode;
		nEndCode   = m_vtRunStationInfo.at(m_nEndStation).nUpCode;
	}
	else
	{

		if(nCurIndex == -1)
		{
			nCurCode = 0;
		}
		else
		{
			nCurCode   = m_vtRunStationInfo.at(nCurIndex).nDownCode;
		}

		if(nNxtIndex == -1)
		{
			nNxtCode = 0;
		}
		else
		{
			nNxtCode   = m_vtRunStationInfo.at(nNxtIndex).nDownCode;
		}

		nStartCode = m_vtRunStationInfo.at(m_nStartStation).nDownCode;
		nEndCode   = m_vtRunStationInfo.at(m_nEndStation).nDownCode;
	}

	//-1
	bool bAuto = 0;
	if(MAN_ATUO != m_nRunMode)
	{	
		bAuto = 1;
	}

	//�Զ�
	pStationInfo->bRunMode		  =	 bAuto;//(BYTE)m_nRunMode;//IsDlgButtonChecked(IDC_RADIO_ATC);

	//TC1��TC2
	pStationInfo->bMainHMI		  =	 IsDlgButtonChecked(IDC_RADIO_RUN_TC1);

	//Add by Johnny in 2017-08-01 19:49:10


	pStationInfo->ATC_Cur_Info    =  m_vtRunStationInfo.at(nCurIndex );
	pStationInfo->ATC_Nxt_Info    =  m_vtRunStationInfo.at(nNxtIndex );

	//Add by Johnny in 2017-08-28 08:57:25
	if(m_bIsUpDirect)
	{
		pStationInfo->ATC_Start_Info  =  m_vtRunStationInfo.at(m_nStartStation );
		pStationInfo->ATC_End_Info    =  m_vtRunStationInfo.at(m_nEndStation );
	}
	else
	{	//Add by Johnny in 2017-08-28 09:27:43
		if(m_nStartStation > m_nEndStation)
		{
			pStationInfo->ATC_Start_Info  =  m_vtRunStationInfo.at(m_nStartStation );
			pStationInfo->ATC_End_Info    =  m_vtRunStationInfo.at(m_nEndStation );
		}
		else
		{
			pStationInfo->ATC_Start_Info  =  m_vtRunStationInfo.at(m_nEndStation );
			pStationInfo->ATC_End_Info    =  m_vtRunStationInfo.at(m_nStartStation);
		}

	}

	//Add by Johnny in 2017-08-02 13:39:37
	pStationInfo->bNextOpenSide_A = IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR);
	pStationInfo->bNextOpenSide_B = IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR);
	
	// Del by Johnny in 2017-07-31 16:39:53

	////�Զ������Զ�
	//if (pStationInfo->bRunMode == 1)
	//{
	//	//Modify By Johnny in 2017-07-28 10:30:38
	//	//pStationInfo->bOpenLeft		= IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR);
	//	//pStationInfo->bOpenRight	= IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR);

	//	//pStationInfo->bOpenLeft		= IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR);
	//	//pStationInfo->bOpenRight	= IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR);

	//	pStationInfo->bCloseLeft	 =	m_bCloseLeftDoor;
	//	pStationInfo->bCloseRight	 =	m_bCloseRightDoor;
	//	pStationInfo->bClosedLeft	 =	m_bClosedLeft;
	//	pStationInfo->bClosedRight	 =	m_bClosedRight;

	//	pStationInfo->bOpenLeft		 =	m_bOpenLeftDoor;
	//	pStationInfo->bOpenRight	 =	m_bOpenRightDoor;

	//}
	////�ֶ�
	////else
	{
		pStationInfo->bOpenLeft		 =	m_bOpenLeftDoor;
		pStationInfo->bOpenRight	 =	m_bOpenRightDoor;
		pStationInfo->bCloseLeft	 =	m_bCloseLeftDoor;
		pStationInfo->bCloseRight	 =	m_bCloseRightDoor;
		pStationInfo->bClosedLeft	 =	m_bClosedLeft;
		pStationInfo->bClosedRight	 =	m_bClosedRight;
	}

	//Modify by Johnny in 2017-08-02 17:36:16
	m_nHMI_Start = m_nStartStation + 1;
	m_nHMI_End	 = m_nEndStation   + 1;

	pStationInfo->nHMI_Start = m_nHMI_Start;
	pStationInfo->nHMI_End   = m_nHMI_End;
	pStationInfo->nHMI_Next  = m_nHMI_Next;

	////�Զ������Զ�
	//if (pStationInfo->bRunMode == 1)
	//{
	//	pStationInfo->bOpenLeft		= IsDlgButtonChecked(IDC_CHECK_LEFT_DOOR);
	//	pStationInfo->bOpenRight	= IsDlgButtonChecked(IDC_CHECK_RIGHT_DOOR);

	//}
	////�ֶ�
	//else
	//{
	//	pStationInfo->bOpenLeft		 =	m_bOpenLeftDoor;
	//	pStationInfo->bOpenRight	 =	m_bOpenRightDoor;
	//	pStationInfo->bCloseLeft	 =	m_bCloseLeftDoor;
	//	pStationInfo->bCloseRight	 =	m_bCloseRightDoor;
	//	pStationInfo->bClosedLeft	 =	m_bClosedLeft;
	//	pStationInfo->bClosedRight	 =	m_bClosedRight;
	//}

	if (nSize>0)
	{
		if (m_nStartStation == -1)
		{
			pStationInfo->bStartStation = 0;
		}
		else
		{
			pStationInfo->bStartStation = nStartCode;
		}
		
		if (m_nEndStation == -1)
		{
			pStationInfo->bEndStation = 0;
		}
		else
		{
			pStationInfo->bEndStation = nEndCode;
		}

		if (m_nNextStation == -1)
		{
			pStationInfo->bNextStation = 0;
		}
		else
		{
			pStationInfo->bNextStation = nNxtCode;
		}

		if (m_nEndStation == -1 )
		{
			pStationInfo->bCurrentStation = 0;
		}
		else
		{
			pStationInfo->bCurrentStation = nCurCode;
		}
		
		
	
	}
	//Modify By Johnny in 2017-07-27 10:51:15�� ʵ������ m_CurSpeed = 1 == 0.1km/h
	pStationInfo->bRunSpeed		 =	m_nCurrentSpeed * 10;
	pStationInfo->bPlayEmc		 =	m_bPlayBC;
	pStationInfo->dwEmcIndexId	 =	m_nAnounceIndex;
	pStationInfo->dwZoneDist	 =	m_nCurZoneDist;

	pStationInfo->bArriveBC		 =	m_bArriveBC;		// ��վ�㲥�ź�
	pStationInfo->bLeaveBC		 =	m_bLeaveBC;			// ��վ�㲥�ź�
	pStationInfo->dwLastDist	 =	m_nLastStationDist;	// �г�����һվ�ľ���
	pStationInfo->bJumpDown		 =	m_bJumpDown;
	pStationInfo->bJumpUp		 =	m_bJumpUp;
	pStationInfo->bDateTimeValid =	m_bDateTimeValid;
	pStationInfo->bCBTC			 =  IsDlgButtonChecked(IDC_CHECK_CBTC);
	pStationInfo->bLineMap       =  IsDlgButtonChecked(IDC_CHECK_LINE_MAP);

	//Add By Johnny in 2017-07-18 15:16:43
	pStationInfo->bAtcMode		= ((CButton*)GetDlgItem(IDC_CHECK_ATC_MODE))->GetCheck();
	pStationInfo->bTcmsMode		= ((CButton*)GetDlgItem(IDC_CHECK_TCMS_MODE))->GetCheck();
	pStationInfo->bAtoMode		= ((CButton*)GetDlgItem(IDC_CHECK_ATO_MODE))->GetCheck();
	pStationInfo->bPmMode		= ((CButton*)GetDlgItem(IDC_CHECK_PM_MODE))->GetCheck();


	return m_EnumCurrentState;
}

void CPageRunData::OnEnablePulse(BOOL bpulse)
{
	m_bEmcPulse = bpulse;
}

void CPageRunData::StopRunStation()
{
	Set_Open_Close_Zero();

	KillTimer(TIME_RE_RUN_STATION);
	KillTimer(TIME_RUN_STATION);
	m_pDistance.SetPos(0);
	//OnTimer��ʱ��
	nStata = 0;
	//�ٶ�����
	m_nCurrentSpeed = 0;
	RunEnableWindow(TRUE);
	SetDlgItemText(IDC_STATIC_LEAV_STATION, _T("0 km/h"));
}

void CPageRunData::OnBnClickedRadioHmi()
{
	StopRunStation();
	GetDlgItem(IDC_BTN_RUN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_APPRY)->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_BUTTON_OPEN_DOOR))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_BUTTON_CLOSE_DOOR))->EnableWindow(TRUE);

	m_nStartStation = -1;
	m_nEndStation = -1;
	m_nNextStation = -1;
}

void CPageRunData::OnBnClickedRadioAtc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_APPRY)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_BUTTON_OPEN_DOOR))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_BUTTON_CLOSE_DOOR))->EnableWindow(FALSE);
}

void CPageRunData::OnCbnSelchangeComboLineid()
{
	int nSelIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_LINEID))->GetCurSel();

	memset(&g_pMainFrame->m_szStationInfoPath, 0x00, 255);
	
	switch (nSelIndex)
	{
	case 0:
		{
			CString strIni = _T("\\StationFile\\Station.bin");
			strcpy_s(g_pMainFrame->m_szStationInfoPath, g_pMainFrame->m_strAppPath + strIni);
		}		
		break;
	default:
		{
			CString strIni;
			strIni.Format("\\StationFile\\Station_%d.bin", nSelIndex+1);
			strcpy_s(g_pMainFrame->m_szStationInfoPath, g_pMainFrame->m_strAppPath + strIni);
		}
	}

	if (PathFileExists(g_pMainFrame->m_szStationInfoPath))
	{
		g_pMainFrame->ReadStationFile();
	}
	else
	{
		g_pMainFrame->m_vtStationInfo.clear();
	}
	g_pMainFrame->OnRefreshSystemData();
}

void CPageRunData::OnBnClickedBtnArrive()
{
	// TODO: Add your control notification handler code here

	if(MAN_ATUO == m_nRunMode)
	{
		CString strTmp;
		strTmp = (m_vtRunStationInfo.at(m_nFrontStation)).szStationName;
		m_sFrontStation.SetCenterText(strTmp);
	}
	m_bArriveBC = TRUE;
	if (m_bEmcPulse)
	{
		((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(FALSE);
		SetTimer(IDT_TIME_ARRIVE, TIMEPULSE, NULL);
	}
}

void CPageRunData::OnBnClickedBtnLeave()
{
	// TODO: Add your control notification handler code here
	m_bLeaveBC = TRUE;
	if (m_bEmcPulse)
	{
		((CButton *)GetDlgItem(IDC_BTN_ARRIVE))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BTN_LEAVE))->EnableWindow(FALSE);
		SetTimer(IDT_TIME_LEAVE, TIMEPULSE, NULL);
	}
}

void CPageRunData::OnBnClickedBtnDataView()
{
	m_pDataAnalysisDlg->ShowWindow(SW_SHOW);
}

void CPageRunData::OnBnClickedButtonOpenDoor()
{
	//Add by Johnny in 2017-08-18 13:43:56
	if(MAN_ATUO == m_nRunMode)
	{
		g_bLastOpenLeft = 1;

		CString strDoorState;

		if(g_bLastOpenRight)
		{
			strDoorState = _T("���࿪��");
		}
		else
		{
			strDoorState = _T("��࿪��");
		}

		m_STATIC_Cur_Door_State.SetWindowText(strDoorState);
	}

	m_bOpenLeftDoor   = 1;
	m_bCloseLeftDoor  = 0;
	m_bClosedLeft     = 0;

	SetTimer(IDT_TIME_KILL_OPEN_DOOR, 3000, NULL);
}

void CPageRunData::OnBnClickedButtonCloseDoor()
{
	//Add by Johnny in 2017-08-18 13:43:56
	if(MAN_ATUO == m_nRunMode)
	{
		g_bLastOpenLeft = 0;
		CString strDoorState;

		if(g_bLastOpenRight)
		{
			strDoorState = _T("�Ҳ࿪��");
		}
		else
		{
			strDoorState = _T("�������");
		}

		m_STATIC_Cur_Door_State.SetWindowText(strDoorState);
		
	}

	m_bOpenLeftDoor   = 0;
	m_bCloseLeftDoor  = 1;
	m_bClosedLeft     = 1;
	SetTimer(IDT_TIME_KILL_CLOSE_DOOR, 3000, NULL);

}

void CPageRunData::OnBnClickedButtonOpenRightDoor()
{
	if(MAN_ATUO == m_nRunMode)
	{
		CString strDoorState;
		g_bLastOpenRight = 1;

		if(g_bLastOpenLeft)
		{
			strDoorState = _T("���࿪��");
		}
		else
		{
			strDoorState = _T("�Ҳ࿪��");
		}

		m_STATIC_Cur_Door_State.SetWindowText(strDoorState);
	}

	m_bOpenRightDoor  = 1;
	m_bCloseRightDoor = 0;
	m_bClosedRight	  = 0;
	SetTimer(IDT_TIME_KILL_OPEN_DOOR, 3000, NULL);
}

void CPageRunData::OnBnClickedButtonCloseRightDoor()
{
	if(MAN_ATUO == m_nRunMode)
	{
		CString strDoorState;
		g_bLastOpenRight = 0;

		if(g_bLastOpenLeft)
		{
			strDoorState = _T("��࿪��");
		}
		else
		{
			strDoorState = _T("�������");
		}

		m_STATIC_Cur_Door_State.SetWindowText(strDoorState);
	}

	m_bOpenRightDoor  = 0;
	m_bCloseRightDoor = 1;
	m_bClosedRight	  = 1;
	SetTimer(IDT_TIME_KILL_CLOSE_DOOR, 3000, NULL);

}


void CPageRunData::OnBnClickedButtonSetNextStation()
{
	// TODO: Add your control notification handler code here
	CString strNextStation;
	int nCursel = ((CComboBox *)GetDlgItem(IDC_COMBO_NEXT_STATION))->GetCurSel();
	((CComboBox *)GetDlgItem(IDC_COMBO_NEXT_STATION))->GetLBText(nCursel, strNextStation);
	
	std::vector<StationInfo>::iterator it = m_vtRunStationInfo.begin();
	for(int i=0; it!=m_vtRunStationInfo.end(); it++,i++)
	{
		StationInfo pStation = (StationInfo)*it;
		CString strTemp(pStation.szStationName);
		if(strNextStation.Compare(strTemp) == 0)
		{
			
			//m_nNextStation = i;
			//�޸�ΪHMI��һվ
			//Modify by Johnny in 2017-08-01 19:29:21
			//m_nHMI_Next = i;
			m_nHMI_Next = i + 1;
			
			//Del by Johnny for out Memory in 2017-07-25 16:28:26

			//SetTimer(IDT_TIME_NEXT_STATION, TIMEPULSE, NULL);
			break;
		}
	}
	
}

void CPageRunData::OnBnClickedButtonDatetimeValid()
{
	// TODO: Add your control notification handler code here
	m_bDateTimeValid = 1;
	SetTimer(IDT_TIME_DATETIME_VALID, 5000, NULL);
}

void CPageRunData::OnBnClickedButtonStartStopStation()
{
	// TODO: Add your control notification handler code here
	//m_nStartStation = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->GetCurSel();
	//
	//m_nEndStation   = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->GetCurSel();

	int start = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->GetCurSel();
	int end   = ((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->GetCurSel();

	if (start > end)
	{
		MessageBox(_T("����վ���Ҫ������ʼվ���"), _T("��ʾ"));

		//m_nStartStation = -1;
		//m_nEndStation =  -1;
		
		//�����������ѡ���ϴ���ȷѡ��
		((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_STATA))->SetCurSel(m_nLastStart);
		((CComboBox*)GetDlgItem(IDC_COMBO_SECTION_END))->SetCurSel(m_nLastEnd);

		return;
	}
	else
	{
		m_nLastStart = start;
		m_nLastEnd   = end;
	}
	//SetTimer(IDT_TIME_START_STOP_STATION, TIMEPULSE, NULL);
}


void CPageRunData::SetMode(void)
{
	int nAtc	= ((CButton*)GetDlgItem(IDC_CHECK_ATC_MODE))->GetCheck();
	int nTcms	= ((CButton*)GetDlgItem(IDC_CHECK_TCMS_MODE))->GetCheck();
	int nAto	= ((CButton*)GetDlgItem(IDC_CHECK_ATO_MODE))->GetCheck();
	int nPm		= ((CButton*)GetDlgItem(IDC_CHECK_PM_MODE))->GetCheck();

	//��¼���һ��ģʽ
	if(ERROR_AUTO != m_nRunMode)
	{
		m_nLast_Run_Mode = m_nRunMode;
	}	

	CStatic* pStcMode = (CStatic*)GetDlgItem(IDC_STATIC_MODE);
	CString strMode;
	if(nAtc == 1 && nTcms == 1)
	{
		strMode = _T("ģʽ����");
		m_nRunMode = ERROR_AUTO;
	}
	else
	{
		//Ĭ��Ϊ���Զ�ģʽ
		strMode = _T("���Զ�ģʽ");
		m_nRunMode = SEMI_AUTO;

		//PM�ֶ�ģʽ
		if(nPm == 1)
		{
			strMode = _T("�ֶ�ģʽ");
			m_nRunMode = MAN_ATUO;
		}
		else
		{
			//���Զ�ģʽ
			if(nAto == 1 && nTcms == 0)
			{
				strMode = _T("ȫ�Զ�ģʽ");
				m_nRunMode = FULL_ATUO;
			}

		}
	}
	if(strMode != _T("ģʽ����"))
	{
		pStcMode->SetWindowText(strMode);
	}
	
	EnableWnd_Btn(TRUE);
}

void CPageRunData::OnBnClickedCheckAtoMode()
{
	//��ֹ����
	StopRunStation();
	SetMode();
	//���ò���
	Reset_Param();
}

void CPageRunData::OnBnClickedCheckPmMode()
{
	//��ֹ����
	StopRunStation();
	SetMode();
	//���ò���
	Reset_Param();
}

void CPageRunData::OnBnClickedCheckAtcMode()
{
	//��ֹ����
	StopRunStation();
	SetMode();

	if(m_nRunMode == ERROR_AUTO)
	{
		m_nRunMode = m_nLast_Run_Mode;

		((CButton*)GetDlgItem(IDC_CHECK_ATC_MODE))->SetCheck(FALSE);

		MessageBox(_T("TCMS��ATC����ͬʱΪ1��"));

	}

	//���ò���
	Reset_Param();
}

void CPageRunData::OnBnClickedCheckTcmsMode()
{
	//��ֹ����
	StopRunStation();
	SetMode();

	if(m_nRunMode == ERROR_AUTO)
	{
		m_nRunMode = m_nLast_Run_Mode;

		((CButton*)GetDlgItem(IDC_CHECK_TCMS_MODE))->SetCheck(FALSE);

		MessageBox(_T("TCMS��ATC����ͬʱΪ1��"));

	}
	//���ò���
	Reset_Param();

}

int CPageRunData::Reset_Param(void)
{
	//�޸��ٶ�
	m_nCurrentSpeed = 0;
	//�޸Ľ�����
	m_pDistance.SetPos(0);
	//�޸ľ���
	m_nLastStationDist = 0;	
	//���� nStata
	nStata = 0;
	//��ť�ָ�

	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_START_RUN);
	pBtn->EnableWindow(TRUE);
	
	CString strDistance;
	strDistance.Format("%.3f km", m_nLastStationDist * 1.0 / 1000);
	SetDlgItemText(IDC_STATIC_STATION_DIST, strDistance);		// ��ʾ�г�����һվ�ľ���

	return 0;
}

void CPageRunData::OnBnClickedBtnStartRun()
{
	if(!m_bOpen_Port)
	{
		MessageBox(_T("���ȴ򿪴���"));
		return;
	}


	Set_Open_Close_Zero();
	//Add by Johnny in 2017-08-01 13:55:22
	nStata	   = 0;
	m_nSumDist = 0;
	//Add by Johnny in 2017-07-27 14:51:04����������������л�������
	m_nTimeCount = 0;
	EnableWnd_Btn(TRUE);
	//����������ť
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_START_RUN);
	pBtn->EnableWindow(FALSE);
	StartRunStation();
}

void CPageRunData::EnableWnd_Btn(bool bEnable)
{
	//�ϡ��¡�Ӧ
	//IDC_BTN_STATION_UP
	//IDC_BTN_STATION_NEXT
	//IDC_BTN_APPRY

	GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_APPRY)->EnableWindow(bEnable);

	//�����ء�������
	//IDC_BUTTON_OPEN_DOOR
	//IDC_BUTTON_CLOSE_DOOR
	//IDC_BTN_ARRIVE
	//IDC_BTN_LEAVE

	
	if(bEnable && m_nRunMode == FULL_ATUO)
	{
		GetDlgItem(IDC_BUTTON_OPEN_DOOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE_DOOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ARRIVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_LEAVE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_OPEN_RIGHT_DOOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE_RIGHT_DOOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPEN_DOOR)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_CLOSE_DOOR)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_ARRIVE)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_LEAVE)->EnableWindow(bEnable);

		GetDlgItem(IDC_BUTTON_OPEN_RIGHT_DOOR)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_CLOSE_RIGHT_DOOR)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_STATION_UP)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_STATION_NEXT)->EnableWindow(bEnable);
	}

	GetDlgItem(IDC_BTN_ADDVEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SUBVEL)->EnableWindow(TRUE);

}

void CPageRunData::Set_Open_Close_Zero(void)
{
	m_bOpenRightDoor  = 0;
	m_bOpenLeftDoor	  = 0;
	m_bClosedLeft	  = 0;
	m_bClosedRight	  = 0;
	m_bCloseLeftDoor  = 0;
	m_bCloseRightDoor = 0;
	Sleep(300);
}


int CPageRunData::Man_Auto_Set_Station(int Cur, int Nxt, int Start, int End)
{
	if(MAN_ATUO == m_nRunMode)
	{
		if(m_nFrontStation == Cur - 1)
		{

		}
		else
		{
			//m_nStartStation = Start - 1;
			//m_nEndStation   = End - 1;
			//m_nNextStation  = Nxt;

			m_nFrontStation = Cur - 1;
			CString strTmp;
			strTmp = (m_vtRunStationInfo.at(m_nFrontStation)).szStationName;
			m_sNextStation.SetCenterText(strTmp);
			
		}


	}
	return 0;
}

void CPageRunData::OnBnClickedCheckLeftDoor()
{
	if(m_BTN_Next_Open_Left.GetCheck() && m_BTN_Next_Open_Right.GetCheck())
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("���࿪��"));
	}
	else if(m_BTN_Next_Open_Left.GetCheck())
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("��࿪��"));
	}
	else if(m_BTN_Next_Open_Right.GetCheck())
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("�Ҳ࿪��"));
	}
	else
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("�޿��Ų�"));
	}
}

void CPageRunData::OnBnClickedCheckRightDoor()
{
	if(m_BTN_Next_Open_Left.GetCheck() && m_BTN_Next_Open_Right.GetCheck())
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("���࿪��"));
	}
	else if(m_BTN_Next_Open_Left.GetCheck())
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("��࿪��"));
	}
	else if(m_BTN_Next_Open_Right.GetCheck())
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("�Ҳ࿪��"));
	}
	else
	{
		m_STATIC_Next_Open_Side.SetWindowText(_T("�޿��Ų�"));
	}
}
