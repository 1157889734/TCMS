// TCMSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCMS.h"
#include "TCMSDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SEND_DATA_TIME 1001



CTCMSDlg* g_TCMSDlgApp = NULL;

//BHPANEL_TMS_API	void BH_Set_Open_Port(BOOL bOpen);


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTCMSDlg 对话框

#define SIZE_TMS_PACKET		sizeof(tms_packet_t)


CTCMSDlg::CTCMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCMSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wSignal = 0;
	m_wVCM_Life=0;

	//Modify By Johnny in 2017-07-18 09:46:24
	//m_nSendTime = 500;
	m_nSendTime = 300;
	m_pRecvBuf = (BYTE *)malloc(BUF_LEN);
	memset(m_pRecvBuf, 0, BUF_LEN);
	m_iRecvBufLen = 0;
}

void CTCMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTCMSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_COMMDATAEVENT, OnCommPort_Data)
	ON_BN_CLICKED(IDOK, &CTCMSDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTCMSDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTCMSDlg 消息处理程序

BOOL CTCMSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	g_TCMSDlgApp = this;

	m_TCMSCtrlDlg.Create(IDD_DIALOG_CTRL, this);

	CString strAppPath;
	::GetModuleFileName(NULL, strAppPath.GetBuffer(256), 256);
	strAppPath.ReleaseBuffer();
	int nPos = strAppPath.ReverseFind('\\');
	strAppPath = strAppPath.Left(nPos);
	CString strConfigPath;
	strConfigPath.Format(_T("%s\\ConfigTms.ini"), strAppPath);

	char szValue[512];
	GetPrivateProfileString("SysCfg", "SendTime", "", szValue, 64, strConfigPath);
	m_nSendTime = atoi(szValue);
	
#define PANEL_WIDTH			807
#define PANLE_HEIDHT		685
#define PANEL_TMS_HEIGHT	490

	GetPrivateProfileString("SysCfg", "TitleName", "", szValue, 128, strConfigPath);
	SetWindowText(szValue);
	::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, 0, 0, PANEL_WIDTH, PANLE_HEIDHT, SWP_HIDEWINDOW);	
	GetDlgItem(IDC_STATIC_PANEL_TMS)->MoveWindow(5, 5, PANEL_WIDTH-15, PANEL_TMS_HEIGHT, TRUE);

	m_TCMSCtrlDlg.MoveWindow(0, PANEL_TMS_HEIGHT+5, PANEL_WIDTH, PANLE_HEIDHT-(PANEL_TMS_HEIGHT+5), TRUE);

	m_TCMSCtrlDlg.ShowWindow(SW_SHOW);

	HWND hwnd;
	BH_InitTMS(GetDlgItem(IDC_STATIC_PANEL_TMS)->m_hWnd, &hwnd);

	m_DoorFaultDlg.Create(IDD_DIALOG_TCMS_DORR, (CWnd*)CWnd::FromHandle(hwnd));
	BH_InsertPageTab(&m_DoorFaultDlg, "门 故 障 模 拟");

  	BH_RegCallBackSendDataView(OnRegDataViewSendCallbackFunction, this);

	BH_OnEnableMainFrame();

	OnRefreshFeedbackPanel();
	
	//Com
	m_pEseries = new CCESeries();
	//COM_InitComList();

#if 0
	tms_recv_packet_t stRecvpacket;
	memset(&stRecvpacket, 0x00, sizeof(tms_recv_packet_t));
	stRecvpacket.err_t.car_err2.a1_box_led_err= 1;

	OnRefreshState(stRecvpacket);
#endif
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCMSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCMSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTCMSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTCMSDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);

	if (nIDEvent == SEND_DATA_TIME)
	{
		OnSendUrumqiData();
	}

	
}

CTCMSDlg* CTCMSDlg::GetTCMSDlgApp()
{
	return (CTCMSDlg*)g_TCMSDlgApp;
}


tms_packet_t tmsPacket;
BYTE pbuf[SIZE_TMS_PACKET];

void CTCMSDlg::OnSendUrumqiData()
{
	
	RunStationInfo stRunStationInfo;
	ZeroMemory(&stRunStationInfo, sizeof(stRunStationInfo));

 	RunStationEnum  RunState = BH_GetTMSRunState(&stRunStationInfo);

	//Modify by Johnny in 2017-07-17 19:03:31
	//tms_packet_t tmsPacket;
	memset(&tmsPacket, 0x00, sizeof(tms_packet_t));

	tmsPacket.car_id_t.train_id_2	 =	0x02;
	tmsPacket.car_id_t.train_id_1	 =	0x01;
	tmsPacket.car_id_t.line_id_h	 =	0x01;
	tmsPacket.car_id_t.line_id_l	 =	0x01;

 	WORD wSignal = m_wSignal++;
// 	if (m_wSignal>=255)
// 	{
// 		m_wSignal = 0;
// 	}
// 	tmsPacket.life_t.life_h = wSignal>>8;
// 	tmsPacket.life_t.life_l = wSignal;
// 
// 	tmsPacket.tcms_ver_t.tcms_h = 0x01;
// 	tmsPacket.tcms_ver_t.tcms_l = 0x02;

	CTime pt = CTime::GetCurrentTime();
	tmsPacket.time_t.year   = pt.GetYear()%100;
	tmsPacket.time_t.month  = pt.GetMonth();
	tmsPacket.time_t.date	= pt.GetDay();
	tmsPacket.time_t.hour	= pt.GetHour();
	tmsPacket.time_t.minute = pt.GetMinute();
	tmsPacket.time_t.second = pt.GetSecond();
	tmsPacket.time_t.DateTimeValid_t.Date_Time_Valid = stRunStationInfo.bDateTimeValid;
	
	tmsPacket.atc_t.fault_valid_t.CBTC_MODE = stRunStationInfo.bCBTC;
	

	//atc故障
	tmsPacket.atc_t.fault_valid_t.ATC1_Serious_fault	  = 1;	//严重故障

	//atc快速
	tmsPacket.atc_t2.door_info_t.ATC1_Drive_br_setp_valid = 0; //牵引/制动参考值有效
	
#if 0
	//2秒脉冲
	if(RunState == Enum_ComStation)
	{
		tmsPacket.atc_t2.door_info_t.ATC1_Open_doors_side_A	 = stRunStationInfo.bOpenLeft ?	1 : 0;
		tmsPacket.atc_t2.door_info_t.ATC1_Open_doors_side_B	 = stRunStationInfo.bOpenRight? 1 : 0;
	}
	if (RunState==Enum_PreLeaveStation)
	{
		tmsPacket.atc_t2.door_info_t.ATC1_Close_doors_side_A = stRunStationInfo.bOpenLeft ? 1 : 0;
		tmsPacket.atc_t2.door_info_t.ATC1_Close_doors_side_B = stRunStationInfo.bOpenRight? 1 : 0;

		//tmsPacket.tcms_t.pa1_info.leave_broadcast=1;%
	}
#endif

	tmsPacket.atc_t2.door_info_t.ATC1_Open_doors_side_A	 = stRunStationInfo.ATC_Open_A;
	tmsPacket.atc_t2.door_info_t.ATC1_Open_doors_side_B	 = stRunStationInfo.ATC_Open_B;

	tmsPacket.atc_t2.door_info_t.ATC1_Close_doors_side_A = stRunStationInfo.ATC_Close_A;
	tmsPacket.atc_t2.door_info_t.ATC1_Close_doors_side_B = stRunStationInfo.ATC_Close_B;

	/*tmsPacket.atc_t2.mode_info_t.ATC1_ATO_active = stRunStationInfo.bRunMode;*/

	tmsPacket.atc_t2.mode_info_t.ATC1_ATO_active = stRunStationInfo.bAtoMode;

//	TRACE(">>>>>> %d %d \n", stRunStationInfo.bOpenLeft, stRunStationInfo.bOpenRight);
	//atc慢速
	tmsPacket.atc_t3.ATC1_Time_Mon  = pt.GetMonth();
	tmsPacket.atc_t3.ATC1_Time_year = pt.GetYear()%100;
	tmsPacket.atc_t3.ATC1_Time_Hour = pt.GetHour();
	tmsPacket.atc_t3.ATC1_Time_Day  = pt.GetDay();
	tmsPacket.atc_t3.ATC1_Time_Sec  = pt.GetSecond();
	tmsPacket.atc_t3.ATC1_Time_Min  = pt.GetMinute();

	//Modify by Johnny in 2017-08-02 13:40:29
	//tmsPacket.atc_t3.open_info_t.ATC1_Open_B_side_next	=	stRunStationInfo.bOpenRight ? 1 : 0;
	//tmsPacket.atc_t3.open_info_t.ATC1_Open_A_side_next  =	stRunStationInfo.bOpenLeft  ? 1 : 0;
	tmsPacket.atc_t3.open_info_t.ATC1_Open_A_side_next  =	stRunStationInfo.bNextOpenSide_A  ? 1 : 0;
	tmsPacket.atc_t3.open_info_t.ATC1_Open_B_side_next	=	stRunStationInfo.bNextOpenSide_B  ? 1 : 0;

	
	//ATC速度
	tmsPacket.atc_t3.ATC1_Train_number_l		=   0x01;
	tmsPacket.atc_t3.ATC1_Current_velocity_l	=	stRunStationInfo.bRunSpeed;
	tmsPacket.atc_t3.ATC1_Current_velocity_h	=	stRunStationInfo.bRunSpeed >> 8;

	//Modify by Johnny in 2017-08-01 19:50:34
#if 0
	tmsPacket.atc_t3.ATC1_End_station_ID_l		=	stRunStationInfo.bEndStation;
	tmsPacket.atc_t3.ATC1_End_station_ID_h		=	stRunStationInfo.bEndStation >> 8;
	tmsPacket.atc_t3.ATC1_Current_station_ID_l	=	stRunStationInfo.bCurrentStation;
	tmsPacket.atc_t3.ATC1_Current_station_ID_h	=	stRunStationInfo.bCurrentStation >> 8;
 	tmsPacket.atc_t3.ATC1_Next_station_ID_l		=	stRunStationInfo.bNextStation;
 	tmsPacket.atc_t3.ATC1_Next_station_ID_h		=	stRunStationInfo.bNextStation >> 8;
#endif

	if(stRunStationInfo.bUpOrDownStream)
	{
		tmsPacket.atc_t3.ATC1_End_station_ID_l		=	stRunStationInfo.ATC_End_Info.nUpCode;
		tmsPacket.atc_t3.ATC1_End_station_ID_h		=	stRunStationInfo.ATC_End_Info.nUpCode >> 8;
		tmsPacket.atc_t3.ATC1_Current_station_ID_l	=	stRunStationInfo.ATC_Cur_Info.nUpCode;
		tmsPacket.atc_t3.ATC1_Current_station_ID_h	=	stRunStationInfo.ATC_Cur_Info.nUpCode >> 8;
		tmsPacket.atc_t3.ATC1_Next_station_ID_l		=	stRunStationInfo.ATC_Nxt_Info.nUpCode;
		tmsPacket.atc_t3.ATC1_Next_station_ID_h		=	stRunStationInfo.ATC_Nxt_Info.nUpCode >> 8;
	}
	else
	{
		tmsPacket.atc_t3.ATC1_End_station_ID_l		=	stRunStationInfo.ATC_End_Info.nDownCode;
		tmsPacket.atc_t3.ATC1_End_station_ID_h		=	stRunStationInfo.ATC_End_Info.nDownCode >> 8;
		tmsPacket.atc_t3.ATC1_Current_station_ID_l	=	stRunStationInfo.ATC_Cur_Info.nDownCode;
		tmsPacket.atc_t3.ATC1_Current_station_ID_h	=	stRunStationInfo.ATC_Cur_Info.nDownCode >> 8;
		tmsPacket.atc_t3.ATC1_Next_station_ID_l		=	stRunStationInfo.ATC_Nxt_Info.nDownCode;
		tmsPacket.atc_t3.ATC1_Next_station_ID_h		=	stRunStationInfo.ATC_Nxt_Info.nDownCode >> 8;
	}


#if 0
	//预到站
	if(RunState == Enum_PreComStation)
	{
		tmsPacket.atc_t3.ATC1_Location_info_l=1;
	}
	//到站
	else if(RunState == Enum_ComStation)
	{
		tmsPacket.atc_t3.ATC1_Location_info_l=2;
	}
	//预离站
	else if(RunState == Enum_PreLeaveStation)
	{
		tmsPacket.atc_t3.ATC1_Location_info_l=4;
	}
	//离站
	else if(RunState == Enum_LeaveStation)
	{
		tmsPacket.atc_t3.ATC1_Location_info_l=8;
	}
	//
	else
	{
		tmsPacket.atc_t3.ATC1_Location_info_l=0;
	}
#endif

	//Add by Johnny in 2018-01-02 09_07
	tmsPacket.atc_t3.ATC1_Location_info_l= stRunStationInfo.nLocationInfo;

	//VCM->PIS_PA

	//TC1、TC2
	tmsPacket.vcm_t.cab_info_t.Cab_manned_Tc1 = stRunStationInfo.bMainHMI	? 1 : 0;
	tmsPacket.vcm_t.cab_info_t.Cab_manned_Tc2 = stRunStationInfo.bMainHMI ? 0 : 1;
	tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed	=	1;
	tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed	=	1;
	//2秒脉冲
	if (stRunStationInfo.bRunMode == 1)
	{
		//Del by Johnny in 2017-07-28 10:36:15
		//if(RunState == Enum_ComStation)
		//{
			//tmsPacket.vcm_t.cab_info_t.open_Left_door = stRunStationInfo.bOpenLeft	? 1 : 0;
			//tmsPacket.vcm_t.cab_info_t.open_right_door = stRunStationInfo.bOpenRight? 1 : 0;
		
		//}

		if (RunState == Enum_OpenDoor)
		{	
			tmsPacket.vcm_t.cab_info_t.open_Left_door  = stRunStationInfo.bOpenLeft	? 1 : 0;
			tmsPacket.vcm_t.cab_info_t.open_right_door = stRunStationInfo.bOpenRight? 1 : 0;

			//Modfiy by Johnny in 2017-08-01 15:34:28
			//if (stRunStationInfo.bOpenLeft == 1)
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed = 0;
			//}
			//else
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed = 1;
			//}

			//if (stRunStationInfo.bOpenRight == 1)
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed = 0;
			//}
			//else
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed = 1;
			//}

			tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed = stRunStationInfo.bClosedLeft;

			tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed = stRunStationInfo.bClosedRight;
			
			if(stRunStationInfo.bClosedLeft != stRunStationInfo.bClosedRight)
			{
				int x = 3;
			}


		}

		if ( RunState == Enum_CloseDoor )
		{
			//Modify by Johnny in 2017-07-31 19:21:07
			//tmsPacket.vcm_t.cab_info_t.close_Left_door  = stRunStationInfo.bOpenLeft ? 1 : 0;
			//tmsPacket.vcm_t.cab_info_t.close_right_door = stRunStationInfo.bOpenRight? 1 : 0;

			tmsPacket.vcm_t.cab_info_t.close_Left_door  = stRunStationInfo.bClosedLeft ? 1 : 0;
			tmsPacket.vcm_t.cab_info_t.close_right_door = stRunStationInfo.bClosedRight? 1 : 0;

			//if (stRunStationInfo.bOpenLeft == 1)
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed = 0;
			//}
			//else
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed = 1;
			//}

			//if (stRunStationInfo.bOpenRight == 1)
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed= 0;
			//}
			//else
			//{
			//	tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed= 1;
			//}

			tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed = stRunStationInfo.bClosedLeft;
			tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed = stRunStationInfo.bClosedRight;

			if(stRunStationInfo.bClosedLeft != stRunStationInfo.bClosedRight)
			{
				int x = 3;
			}
		}

		if (RunState==Enum_PreLeaveStation)
		{
			tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed	= 1;
			tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed	= 1;

// 			if (tmsPacket.vcm_t.cab_info_t.close_Left_door == 1)
// 			{
// 				tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed=1;
// 			}
// 			else
// 			{
// 				tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed=0;
// 			}
// 
// 			if (tmsPacket.vcm_t.cab_info_t.close_Left_door == 1)
// 			{
// 				tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed=1;
// 			}
// 			else
// 			{
// 				tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed=0;
// 			}
		}

	}
	else //非关门阶段
	{
		tmsPacket.vcm_t.cab_info_t.close_Left_door				=	stRunStationInfo.bCloseLeft  ? 1 : 0;
		tmsPacket.vcm_t.cab_info_t.close_right_door				=	stRunStationInfo.bCloseRight ? 1 : 0;
		tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed	=	stRunStationInfo.bClosedLeft ? 1 : 0;
		tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed	=	stRunStationInfo.bClosedRight? 1 : 0;
		tmsPacket.vcm_t.cab_info_t.open_Left_door  = stRunStationInfo.bOpenLeft					 ? 1 : 0;
		tmsPacket.vcm_t.cab_info_t.open_right_door = stRunStationInfo.bOpenRight			 	 ? 1 : 0;
	}
	
	//ATO/ATP   PM 模式设置 
	tmsPacket.vcm_t.cbk_info_t.ATO_ATP_mode_active	= stRunStationInfo.bAtoMode;
	tmsPacket.vcm_t.cbk_info_t.PM_mode_active		= stRunStationInfo.bPmMode;

	tmsPacket.vcm_t.Emergency_info_t.Emergency_announcement_touch = stRunStationInfo.bPlayEmc;
	tmsPacket.vcm_t.Emergency_announcement_ID_h		= stRunStationInfo.dwEmcIndexId >> 8;
	tmsPacket.vcm_t.Emergency_announcement_ID_l		= stRunStationInfo.dwEmcIndexId;

	//ATC、TCMS 广播模式
	tmsPacket.vcm_t.Emergency_info_t.Announcement_Mode_ATC	= stRunStationInfo.bAtcMode;
	tmsPacket.vcm_t.Emergency_info_t.Announcement_Mode_TCMS = stRunStationInfo.bTcmsMode;

	//Modify by Johnny in 2017-08-02 17:32:29
	//HMI start、end、next,目前HMI、start、end 与ATC一致
	//tmsPacket.vcm_t.HMI_start_ID_h = stRunStationInfo.bStartStation >> 8;
	//tmsPacket.vcm_t.HMI_start_ID_l = stRunStationInfo.bStartStation;

	//tmsPacket.vcm_t.HMI_end_ID_h = stRunStationInfo.bEndStation >> 8;
	//tmsPacket.vcm_t.HMI_end_ID_l = stRunStationInfo.bEndStation;

	tmsPacket.vcm_t.HMI_start_ID_h = stRunStationInfo.nHMI_Start >> 8;
	tmsPacket.vcm_t.HMI_start_ID_l = stRunStationInfo.nHMI_Start;

	tmsPacket.vcm_t.HMI_end_ID_h = stRunStationInfo.nHMI_End >> 8;
	tmsPacket.vcm_t.HMI_end_ID_l = stRunStationInfo.nHMI_End;

	//Modify by Johnny in 2017-07-27 14:55:17
	tmsPacket.vcm_t.HMI_next_ID_h = stRunStationInfo.nHMI_Next >> 8;
	tmsPacket.vcm_t.HMI_next_ID_l = stRunStationInfo.nHMI_Next;
	
	tmsPacket.vcm_t.Emergency_info_t.Jump_station_down	 = stRunStationInfo.bJumpDown;
	tmsPacket.vcm_t.Emergency_info_t.Jump_station_up	 = stRunStationInfo.bJumpUp;

	tmsPacket.vcm_t.Emergency_info_t.Route_map_open		 = stRunStationInfo.bLineMap?1:0;
	tmsPacket.vcm_t.Emergency_info_t.Route_map_close	 = stRunStationInfo.bLineMap?0:1;
	
	tmsPacket.vcm_t.Ref_Velocity_h = stRunStationInfo.bRunSpeed >> 8;
	tmsPacket.vcm_t.Ref_Velocity_l = stRunStationInfo.bRunSpeed;

	WORD wVCM_Life = m_wVCM_Life++;
	if(wVCM_Life>=65536)
	{
		wVCM_Life=0;
	}
	////VCM
	////测试丢包与否
	//if(stRunStationInfo.bRunSpeed == 0)
	//{
	//	wVCM_Life = 0;
	//	m_wVCM_Life = 0;
	//}

	tmsPacket.vcm_t.VCM_Life_l = wVCM_Life;
	tmsPacket.vcm_t.VCM_Life_h = wVCM_Life>>8;

	tmsPacket.atc_t3.ATC1_Heartbeat_l = wVCM_Life;
	tmsPacket.atc_t3.ATC1_Heartbeat_h = wVCM_Life>>8;

	tmsPacket.vcm_t.HMI_Line_ID_h=0;
	tmsPacket.vcm_t.HMI_Line_ID_l=2;

// 	tmsPacket.vcm_t.Car1_Temp_l = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_A1);
// 	tmsPacket.vcm_t.Car1_Temp_h = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_A1)>>8;
// 	tmsPacket.vcm_t.Car2_Temp_l = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_B1);
// 	tmsPacket.vcm_t.Car2_Temp_h = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_B1)>>8;
// 	tmsPacket.vcm_t.Car3_Temp_l = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_C1);
// 	tmsPacket.vcm_t.Car3_Temp_h = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_C1)>>8;
// 	tmsPacket.vcm_t.Car4_Temp_l = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_C2);
// 	tmsPacket.vcm_t.Car4_Temp_h = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_C2)>>8;
// 	tmsPacket.vcm_t.Car5_Temp_l = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_B2);
// 	tmsPacket.vcm_t.Car5_Temp_h = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_B2)>>8;
// 	tmsPacket.vcm_t.Car6_Temp_l = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_A2);
// 	tmsPacket.vcm_t.Car6_Temp_h = m_TempFaultDlg.GetDlgItemInt(IDC_EDIT_A2)>>8;

	//HMI 手动控制
	if (stRunStationInfo.bRunMode==0)
	{
// 		tmsPacket.vcm_t.cbk_info_t.Cbk_Left_all_doors_closed=1;
// 		tmsPacket.vcm_t.cbk_info_t.Cbk_Right_all_doors_closed=1;
		if (stRunStationInfo.bArriveBC==1)
		{
			tmsPacket.vcm_t.cbk_info_t.Manual_station_ann_arrive=1;
			TRACE("手动到站广播\n");
		}
		if (stRunStationInfo.bLeaveBC==1)
		{
			tmsPacket.vcm_t.cbk_info_t.Manual_station_ann_leave=1;
			TRACE("手动离站广播\n");
		}
// 		if(1==stRunStationInfo.bHMI_Jump_station_up)
// 		{
// 			tmsPacket.vcm_t.Emergency_info_t.Jump_station_up=1;
// 			TRACE("向上越一站");
// 		}
// 		if(1==stRunStationInfo.bHMI_Jump_station_down)
// 		{
// 			tmsPacket.vcm_t.Emergency_info_t.Jump_station_down=1;
// 			TRACE("向下越一站");
// 		}
// 		if(1==stRunStationInfo.bHMI_Route_map_open)
// 		{
// 			tmsPacket.vcm_t.Emergency_info_t.Route_map_open=1;
// 			TRACE("线路地图开");
// 		}
// 		else
// 		{
// 			tmsPacket.vcm_t.Emergency_info_t.Route_map_open=0;
// 			TRACE("线路地图关");
// 		}
// 		if (stRunStationInfo.bHMI_OpenDoor)
// 		{
// 			tmsPacket.tcms_t.cab_info.left_door_open = stRunStationInfo.bOpenLeft;
// 			tmsPacket.tcms_t.cab_info.right_door_open = stRunStationInfo.bOpenRight;
// 
// 			tmsPacket.tcms_t.pa2_info.left_door_close = stRunStationInfo.bOpenLeft;
// 			tmsPacket.tcms_t.pa2_info.right_door_close = stRunStationInfo.bOpenRight;
// 
// 			tmsPacket.tcms_t.cab_info.left_alldoor_closed=stRunStationInfo.bOpenLeft?0:1;
// 			tmsPacket.tcms_t.cab_info.right_alldoor_closed =stRunStationInfo.bOpenRight?0:1;
// 
// 			TRACE("手动开门 \n");
// 		}
// 		if (stRunStationInfo.bHMI_CloseDoor)
// 		{
// 			tmsPacket.tcms_t.cab_info.left_door_open = 0;
// 			tmsPacket.tcms_t.cab_info.right_door_open = 0;
// 
// 			tmsPacket.tcms_t.pa2_info.left_door_close = 1;
// 			tmsPacket.tcms_t.pa2_info.right_door_close = 1;
// 
// 			tmsPacket.tcms_t.cab_info.left_alldoor_closed=1;
// 			tmsPacket.tcms_t.cab_info.right_alldoor_closed =1;
// 			TRACE("手动关门 \n");
// 		}
 	}	

	//TRACE(">>>>>> stRunStationInfo.bCurrentStation %d %d\n", stRunStationInfo.bCurrentStation, stRunStationInfo.bNextStation);
	//TRACE("next = %d , from = %d \n", stRunStationInfo.dwZoneDist, stRunStationInfo.dwAllZoneDist-stRunStationInfo.dwZoneDist);

	//Modify By Johnny in 2017-07-17 19:02:05

	//VCM->PIS_TV
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_1_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 0);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_2_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 1);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_3_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 2);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_4_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 3);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_5_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 4);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_6_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 5);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_7_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 6);
	tmsPacket.vcm_t2.door_emergency1_t.Door_emergency_release_8_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 7);
	tmsPacket.vcm_t2.fire_alarm1_t.Fire_alarm_1_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 8);
	tmsPacket.vcm_t2.fire_alarm1_t.Fire_alarm_2_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 9);
	tmsPacket.vcm_t2.fire_alarm1_t.Fire_alarm_3_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 10);
	tmsPacket.vcm_t2.fire_alarm1_t.Fire_alarm_4_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 11);
	tmsPacket.vcm_t2.fire_alarm1_t.Fire_alarm_5_1 = m_DoorFaultDlg.GetDlgBtnCheck(0, 12);

	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_1_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 0);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_2_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 1);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_3_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 2);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_4_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 3);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_5_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 4);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_6_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 5);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_7_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 6);
	tmsPacket.vcm_t2.door_emergency2_t.Door_emergency_release_8_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 7);
	tmsPacket.vcm_t2.fire_alarm2_t.Fire_alarm_1_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 8);
	tmsPacket.vcm_t2.fire_alarm2_t.Fire_alarm_2_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 9);
	tmsPacket.vcm_t2.fire_alarm2_t.Fire_alarm_3_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 10);
	tmsPacket.vcm_t2.fire_alarm2_t.Fire_alarm_4_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 11);
	tmsPacket.vcm_t2.fire_alarm2_t.Fire_alarm_5_2 = m_DoorFaultDlg.GetDlgBtnCheck(1, 12);

	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_1_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 0);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_2_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 1);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_3_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 2);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_4_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 3);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_5_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 4);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_6_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 5);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_7_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 6);
	tmsPacket.vcm_t2.door_emergency3_t.Door_emergency_release_8_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 7);
	tmsPacket.vcm_t2.fire_alarm3_t.Fire_alarm_1_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 8);
	tmsPacket.vcm_t2.fire_alarm3_t.Fire_alarm_2_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 9);
	tmsPacket.vcm_t2.fire_alarm3_t.Fire_alarm_3_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 10);
	tmsPacket.vcm_t2.fire_alarm3_t.Fire_alarm_4_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 11);
	tmsPacket.vcm_t2.fire_alarm3_t.Fire_alarm_5_3 = m_DoorFaultDlg.GetDlgBtnCheck(2, 12);

	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_1_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 0);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_2_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 1);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_3_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 2);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_4_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 3);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_5_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 4);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_6_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 5);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_7_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 6);
	tmsPacket.vcm_t2.door_emergency4_t.Door_emergency_release_8_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 7);
	tmsPacket.vcm_t2.fire_alarm4_t.Fire_alarm_1_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 8);
	tmsPacket.vcm_t2.fire_alarm4_t.Fire_alarm_2_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 9);
	tmsPacket.vcm_t2.fire_alarm4_t.Fire_alarm_3_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 10);
	tmsPacket.vcm_t2.fire_alarm4_t.Fire_alarm_4_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 11);
	tmsPacket.vcm_t2.fire_alarm4_t.Fire_alarm_5_4 = m_DoorFaultDlg.GetDlgBtnCheck(3, 12);

	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_1_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 0);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_2_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 1);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_3_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 2);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_4_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 3);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_5_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 4);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_6_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 5);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_7_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 6);
	tmsPacket.vcm_t2.door_emergency5_t.Door_emergency_release_8_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 7);
	tmsPacket.vcm_t2.fire_alarm5_t.Fire_alarm_1_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 8);
	tmsPacket.vcm_t2.fire_alarm5_t.Fire_alarm_2_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 9);
	tmsPacket.vcm_t2.fire_alarm5_t.Fire_alarm_3_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 10);
	tmsPacket.vcm_t2.fire_alarm5_t.Fire_alarm_4_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 11);
	tmsPacket.vcm_t2.fire_alarm5_t.Fire_alarm_5_5 = m_DoorFaultDlg.GetDlgBtnCheck(4, 12);

	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_1_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 0);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_2_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 1);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_3_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 2);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_4_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 3);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_5_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 4);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_6_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 5);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_7_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 6);
	tmsPacket.vcm_t2.door_emergency6_t.Door_emergency_release_8_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 7);
	tmsPacket.vcm_t2.fire_alarm6_t.Fire_alarm_1_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 8);
	tmsPacket.vcm_t2.fire_alarm6_t.Fire_alarm_2_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 9);
	tmsPacket.vcm_t2.fire_alarm6_t.Fire_alarm_3_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 10);
	tmsPacket.vcm_t2.fire_alarm6_t.Fire_alarm_4_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 11);
	tmsPacket.vcm_t2.fire_alarm6_t.Fire_alarm_5_6 = m_DoorFaultDlg.GetDlgBtnCheck(5, 12);


	//包组合
	tmsPacket.fram_begin = 0xFE;
	tmsPacket.checksum = GetCheckSum((BYTE *)&tmsPacket+1, sizeof(tms_packet_t)-3);
	tmsPacket.fram_end = 0xFF;

	int nSendSize = sizeof(tms_packet_t);

	//BYTE *pbuf = new BYTE[SIZE_TMS_PACKET];
	
	memcpy(pbuf, &tmsPacket, SIZE_TMS_PACKET);

  	BH_UpdateDataView(pbuf, SIZE_TMS_PACKET);

	CString szbuf;
	for (int i=0; i< nSendSize;i++)
	{
		CString szbufTemp;
		szbufTemp.Format("%02X ", pbuf[i]);
		szbuf += szbufTemp;
	}

	//////////////////////////////////////////////////////////////////////////
	
	//正常写入不释放？pbuf？ Modify By Johnny in 2017-07-17 18:23:25
	//将其改为全局变量！
 	if ( !m_pEseries->WritePort(pbuf, nSendSize) )
 	{
		TRACE(_T("发送失败"));
  		//delete pbuf;
  		//pbuf = NULL;
 	}

	//delete pbuf;
	//pbuf = NULL;

	m_TCMSCtrlDlg.SetDlgItemText(IDC_EDIT_SEND_DATE, szbuf);
}

DWORD dwStartTime=0;


//???????????????????????????????????????????????????????垃圾代码

void __stdcall CTCMSDlg::RegEseriesDataFunction(long lComId, BYTE *pRecvbuf, int nleng, void *pCountxt)
{
	DWORD dwEndTime = GetTickCount();
	if (dwEndTime-dwStartTime >= 1000)
	{
		dwStartTime = dwEndTime;
	}
	else
	{
		return;
	}

	CString szbuf;
	for (int n=0; n<nleng; n++)
	{
		CString szbufTemp;
		szbufTemp.Format("%02X ", pRecvbuf[n]);
		szbuf += szbufTemp;
	}
	
	CTCMSDlg::GetTCMSDlgApp()->m_TCMSCtrlDlg.SetDlgItemText(IDC_EDIT_RECV_DATE, szbuf);	
}
//---------------------------------------------------------------------------------



BOOL CTCMSDlg::OnStartTcms(int nComNo)
{
	if (m_pEseries->OpenPort(this, nComNo, 19200))
	{
		//Add by Johnny in 2017-08-01 11:32:06
		BH_Set_Open_Close_Zero();
		//串口打开标识
		BH_Set_Open_Port(TRUE);

		SetTimer(SEND_DATA_TIME, m_nSendTime, NULL);
	}
	else
	{
		MessageBox(_T("打开串口失败, 请检查..."), _T("提示"));
		return 0;
	}
	return 1;
}

void CTCMSDlg::OnPause(BOOL bPause)
{
	if (bPause)
	{
		KillTimer(SEND_DATA_TIME);
	}
	else
	{
		SetTimer(SEND_DATA_TIME, m_nSendTime, NULL);
	}
}

void CTCMSDlg::OnStopTcms()
{	
	KillTimer(SEND_DATA_TIME);

	BH_Set_Open_Close_Zero();

	BH_Set_Open_Port(FALSE);

	BH_StopRunTMS();

	if (m_pEseries->GetCommObjState())
	{
		m_pEseries->ClosePort();
	}
}

void CTCMSDlg::OnDestroy()
{
	CDialog::OnDestroy();

	BH_StopRunTMS();

	BH_UnInitTMS();

	if (m_pEseries)
	{
		delete m_pEseries;
		m_pEseries = NULL;
	}
}

void CTCMSDlg::OnRegDataViewSendCallbackFunction(DWORD dwItem, void *pView, void *pVoid)
{
	OnDataViewDisPlay(dwItem, pView, pVoid);
}



LRESULT CTCMSDlg::OnCommPort_Data(WPARAM wParam,LPARAM lParam) 
{
	COMM_DATAPACK *pptr= (COMM_DATAPACK*)lParam;
	int nlen = sizeof(tms_recv_packet_t);
	int nRecCnt = 0;
	if (pptr)
	{
		BYTE *tempBuf = (BYTE *)malloc(BUF_LEN);
		while(nRecCnt+BUF_LEN < pptr->nLen)
		{
			memset(tempBuf, 0, BUF_LEN);
			memcpy(m_pRecvBuf+m_iRecvBufLen, pptr->pPtr+nRecCnt, BUF_LEN-m_iRecvBufLen);
			m_iRecvBufLen = BUF_LEN;
			memcpy(tempBuf, m_pRecvBuf, m_iRecvBufLen);
			int nProcessCnt = ProcessComData(tempBuf, m_iRecvBufLen);
			memset(m_pRecvBuf, 0 , m_iRecvBufLen);
			memcpy(m_pRecvBuf, tempBuf+nProcessCnt, m_iRecvBufLen-nProcessCnt);
			m_iRecvBufLen -= nProcessCnt;
			nRecCnt+=BUF_LEN;
		}

		if (pptr->nLen+m_iRecvBufLen<BUF_LEN)
		{
			memcpy(m_pRecvBuf+m_iRecvBufLen, pptr->pPtr+nRecCnt, pptr->nLen);
			m_iRecvBufLen+=pptr->nLen;
			if (m_iRecvBufLen >= nlen)	//缓冲区中数据大于或者等于一包数据的大小  然后开始处理
			{
				memset(tempBuf, 0, BUF_LEN);
				memcpy(tempBuf, m_pRecvBuf, m_iRecvBufLen);
				int nProcessCnt = ProcessComData(tempBuf, m_iRecvBufLen);
				memset(m_pRecvBuf, 0 , BUF_LEN);
				memcpy(m_pRecvBuf, tempBuf+nProcessCnt, m_iRecvBufLen-nProcessCnt);
				m_iRecvBufLen -= nProcessCnt;
			}
		}
		//Modify By Johnny in 2017-07-17 19:41:01
		free(tempBuf);
	}

	//
	
	free(pptr->pPtr);
	free(pptr);

	return S_OK;

	/*
	if (pptr)
	{
		int m_nRecCnt = 0;
		char m_bRecBuffer[512]={0};
		for (int i = 0; i < pptr->nLen; i++)
		{
			if (0 == m_nRecCnt)
			{
				if (0xFE == *(pptr->pPtr + i))
				{
					if (m_nRecCnt >= 512)
					{
						return 0;
					}
					m_bRecBuffer[++m_nRecCnt] = *(pptr->pPtr + i);

				} 
				else
				{
					continue;
				}
			}
			else
			{
				if (m_nRecCnt >= 512 )
				{
					memset(m_bRecBuffer, 0, sizeof(m_bRecBuffer));
					m_nRecCnt = 0;
					return 0;
				}
				m_bRecBuffer[++m_nRecCnt] = *(pptr->pPtr + i);
				if (0xFF == *(pptr->pPtr + i) && nleng==m_nRecCnt)//结束符？
				{
					BYTE *tempBuf = (BYTE *)malloc(m_nRecCnt);
					memset(tempBuf, 0, m_nRecCnt);
					memcpy(tempBuf, m_bRecBuffer,m_nRecCnt); 

					//发送消息
					//SendMessage(WM_PROC_COM_DATA, (WPARAM)tempBuf, (LPARAM)m_nRecCntComm);//可以直接用函数

					CString strtemp;
					for (int i=0; i<m_nRecCnt; i++)
					{
						CString buf;
						buf.Format("%02X ", tempBuf[i]);
						strtemp += buf;
					}
					
					m_TCMSCtrlDlg.GetDlgItem(IDC_EDIT_RECV_DATE)->SetWindowText(strtemp);
					tms_recv_packet_t stRecvpacket;
					memset(&stRecvpacket, 0x00, nleng);
					if (sizeof(tms_recv_packet_t) == m_nRecCnt)
					{
						memcpy(&stRecvpacket, tempBuf, sizeof(tms_recv_packet_t));
						OnRefreshState(stRecvpacket);
					}

					memset(m_bRecBuffer, 0, sizeof(m_bRecBuffer));
					m_nRecCnt = 0;
					if(tempBuf != NULL)
					{
						free(tempBuf);
						tempBuf = NULL;
					}
				}
			}
		}

		free(pptr->pPtr);
		free(pptr);
	}
	return S_OK;
	*/
}





int CTCMSDlg::ProcessComData(const BYTE *pData, int nLen)
{
	tms_recv_packet_t stRecvpacket;
	int nlen = sizeof(tms_recv_packet_t);
	BYTE *tempBuf = (BYTE *)malloc(nlen);
	BYTE bAdd = 0;
	int nProcessCnt = 0;
	for (int i = 0; i < nLen; i++)
	{
		if (i+nlen-1<nLen)
		{
			if( 0xFE == *(pData + i) && 0xFF == *(pData + i + 82))
			{
				memset(tempBuf, 0, nlen);
				memcpy(tempBuf, pData + i,nlen); 

				bAdd = GetCheckSum(&(tempBuf[1]), nlen-3);
				CString strtemp;
				for (int j=0; j<nlen; j++)
				{
					CString buf;
					buf.Format("%02X ", tempBuf[j]);
					strtemp += buf;
				}
				if (bAdd == tempBuf[nlen-2])
				{
					m_TCMSCtrlDlg.GetDlgItem(IDC_EDIT_RECV_DATE)->SetWindowText(strtemp);
					
					//内存放在外面申请
					//tms_recv_packet_t stRecvpacket;
					memset(&stRecvpacket, 0x00, nlen);

					memcpy(&stRecvpacket, tempBuf, sizeof(tms_recv_packet_t));
					OnRefreshState(stRecvpacket);
					i += 83;
				}
			}
			nProcessCnt = i;
		}
		else
		{
			nProcessCnt = i;
			continue;
		}
		
	}
	if(tempBuf != NULL)
	{
		free(tempBuf);
		tempBuf = NULL;
	}
	return nProcessCnt;
}
void CTCMSDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CTCMSDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
