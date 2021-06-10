#pragma once

#ifdef DVSSDK_EXPORTS
#define BHPANEL_TMS_API extern "C" __declspec(dllexport)
#else
#define BHPANEL_TMS_API extern "C" __declspec(dllimport)
#endif


typedef struct _RunStationInfo_
{
	BYTE	bLineId;			//线路号
	BYTE	bUpOrDownStream;	//上下行   1.up  0.down
	BYTE	bRunMode;			//运行模式 1.ATC 0.HMI
	BYTE	bMainHMI;			//TC 主备  1.TC1 0.TC2
	BYTE	bOpenLeft;			//开左门
	BYTE	bOpenRight;			//开右门
	BYTE	bCloseLeft;			//关左门
	BYTE	bCloseRight;		//关右门
	int		bStartStation;		//起始站
	int		bEndStation;		//终点站
	int		bCurrentStation;	//当前站
	int		bNextStation;		//下一站
	//Modify by Johnny in 2017-07-27 10:53:57
	WORD	bRunSpeed;			//当前运行速度
	BYTE	bPlayEmc;			//播放紧急广播
	int		dwEmcIndexId;		//紧急广播ID号
	int		dwZoneDist;			//到下站的距离
	bool	bArriveBC;			// 到站广播信号
	bool	bLeaveBC;			// 离站广播信号
	int		dwLastDist;			// 列车距上一站的距离
	BYTE	bJumpUp;			//向上越站
	BYTE	bJumpDown;			//向下越站
	BYTE	bCBTC;				//CBTC模式激活
	BYTE	bDateTimeValid;		//时间生效
	BYTE	bLineMap;			//线路地图
	BYTE	bClosedLeft;		//左门关门锁好
	BYTE	bClosedRight;		//右门关门锁好
	//ATC
	BYTE	bAtcMode;			//广播ATC
	//TCMS
	BYTE	bTcmsMode;			//广播TCMS
	//ATO
	BYTE	bAtoMode;			//ATO模式
	//PM
	BYTE	bPmMode;			//PM模式
	//HMI
	int		nHMI_Start;
	int		nHMI_End;
	int		nHMI_Next;
}RunStationInfo;

typedef struct _StationInfo_ 
{
	int  nStationId;
	char szStationName[32];
	int  nUpCode;
	int  nDownCode;
	int  nDistance;
}StationInfo;

typedef enum RunStationEnum
{
	Enum_CloseDoor,			//关门
	Enum_NextStation,		//给下一站
	Enum_PreLeaveStation,	//预离站
	Enum_LeaveStation,		//离站
	Enum_UniformRun,		//匀速运行
	Enum_PreComStation,		//预到站
	Enum_ComStation,		//到站
	Enum_OpenDoor,			//开门
};

typedef enum En_Feedback_State
{
// 	Enum_Empty,
// 	Enum_Normal,
// 	Enum_Alarm
	Enum_Empty,
	Enum_Normal,//正常
	Enum_FAULT,//故障
	Enum_PECU,//通讯错误
	Enum_CALL,//呼叫
	Enum_ACTIVE//激活
};

typedef void (__stdcall *OnRegDataViewSendCallback)(DWORD dwItem, void *pView, void *pVoid);

//初始化TMS 
/*hWnd  pWnd Tab */
BHPANEL_TMS_API void BH_InitTMS(HWND hWnd, HWND *pWnd);
BHPANEL_TMS_API void BH_UnInitTMS();

//注册数据解析回调
BHPANEL_TMS_API void BH_RegCallBackSendDataView(OnRegDataViewSendCallback DataViewCallback, void *pCountxt);

//数据解析设置
BHPANEL_TMS_API void BH_SetElementHighlight(void *pViewDlg, int n);

//获取数据解析数据
BHPANEL_TMS_API DWORD BH_GetViewDataByte(void* pViewDlg, int n);

//发送显示分析数据
BHPANEL_TMS_API void BH_SetDataTextToStatic(char szbuf[1024]);

//更新数据分析
BHPANEL_TMS_API void BH_UpdateDataView(const BYTE *pData, UINT uLen);

//外部插入页
BHPANEL_TMS_API void BH_InsertPageTab(CDialog *pDlg, char szItemTitle[32]);

//启用页面显示
BHPANEL_TMS_API void BH_OnEnableMainFrame();

//启用TMS
BHPANEL_TMS_API void BH_StartRunTMS();

//停止TMS运行
BHPANEL_TMS_API void BH_StopRunTMS();

//暂停 TMS 运行 TRUE暂停  FALSE继续
BHPANEL_TMS_API void BH_PauseRunTMS(BOOL bPause);

//获取当前状态 站点信息
BHPANEL_TMS_API RunStationEnum BH_GetTMSRunState(RunStationInfo *pRunInfo);

//最大值支持8节车厢+2车头 30行反馈显示+1行车标题
BHPANEL_TMS_API void BH_SetFeedbackLine(int nCar, int nFeedbackLine);

//设置文本显示状态
BHPANEL_TMS_API void BH_SetCarVerticalState(int nCar, int nFeedback, En_Feedback_State nType);

//设置文本输出
BHPANEL_TMS_API void BH_SetDrawLineText(int nCar, int nFeedbackLine, char szItemText[32]);

//设置开关门参数为0
BHPANEL_TMS_API	void BH_Set_Open_Close_Zero();

BHPANEL_TMS_API	void BH_Set_Open_Port(BOOL bOpen);