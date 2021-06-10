#pragma once

#ifdef TMS_PANEL_EXPORTS
#define TMS_PANEL_API extern "C" __declspec(dllexport)
#else
// #define TMS_PANEL_API extern "C" __declspec(dllimport)
#define BHPANEL_TMS_API extern "C" __declspec(dllimport)
#endif

/*
typedef struct _RunStationInfo_
{
	BYTE bLineId;						//��·��
	BYTE bUpOrDownStream;				//������   1.up  0.down
	BYTE bRunMode;						//����ģʽ 1.ATC 0.HMI
	BYTE bMainHMI;						//TC ����  1.TC1 0.TC2
	BYTE bOpenLeft;						//������
	BYTE bOpenRight;					//������
	BYTE bStartStation;					//��ʼվ
	BYTE bEndStation;					//�յ�վ
	BYTE bCurrentStation;				//��ǰվ
	BYTE bNextStation;					//��һվ
	BYTE bRunSpeed;						//��ǰ�����ٶ�
	BYTE bEnableEmc;					//�����㲥����
	DWORD dwEmcIndexId;					//�����㲥ID��
	DWORD dwZoneDist;					//����վ�ľ���
	DWORD dwAllZoneDist;				//��һվ�ܾ���
	BYTE  bHMI_arrive_Station;			//�ֶ���վ�㲥
	BYTE  bHMI_leave_Station;			//�ֶ���վ�㲥
	BYTE  bHMI_OpenDoor;				//�ֶ�����
	BYTE  bHMI_CloseDoor;				//�ֶ�����
}RunStationInfo;*/

typedef struct _StationInfo_ 
{
	int  nStationId;
	char szStationName[32];
	int  nUpCode;
	int  nDownCode;
	int  nDistance;
}StationInfo;

typedef struct _RunStationInfo_
{
	BYTE	bLineId;			//��·��
	BYTE	bUpOrDownStream;	//������   1.up  0.down
	BYTE	bRunMode;			//����ģʽ 1.ATC 0.HMI
	BYTE	bMainHMI;			//TC ����  1.TC1 0.TC2
	BYTE	bOpenLeft;			//������
	BYTE	bOpenRight;			//������
	BYTE	bCloseLeft;			//������
	BYTE	bCloseRight;		//������
	int		bStartStation;		//��ʼվ
	int		bEndStation;		//�յ�վ
	int		bCurrentStation;	//��ǰվ
	int		bNextStation;		//��һվ
	WORD	bRunSpeed;			//��ǰ�����ٶ�
	BYTE	bPlayEmc;			//���Ž����㲥
	BYTE	bStopEmc;			//ֹͣ�����㲥
	int		dwEmcIndexId;		//�����㲥ID��
	int		dwZoneDist;			//����վ�ľ���
	bool	bArriveBC;			//��վ�㲥�ź�
	bool	bLeaveBC;			//��վ�㲥�ź�
	int		dwLastDist;			//�г�����һվ�ľ���
	BYTE	bJumpUp;			//����Խվ
	BYTE	bJumpDown;			//����Խվ
	BYTE	bCBTC;				//CBTCģʽ����
	BYTE	bDateTimeValid;		//ʱ����Ч
	BYTE    bLineMap;			//��·��ͼ
	BYTE	bClosedLeft;		//���Ź�������
	BYTE	bClosedRight;		//���Ź�������
	//ATC
	BYTE	bAtcMode;			//�㲥ATC
	//TCMS
	BYTE	bTcmsMode;			//�㲥TCMS
	//ATO
	BYTE	bAtoMode;			//ATOģʽ
	//PM
	BYTE	bPmMode;			//PMģʽ
	int		nHMI_Start;
	int		nHMI_End;
	int		nHMI_Next;

	StationInfo ATC_Cur_Info;
	StationInfo ATC_Nxt_Info;
	StationInfo ATC_Start_Info;
	StationInfo ATC_End_Info;

	StationInfo HMI_Cur_Info;
	StationInfo HMI_Nxt_Info;
	StationInfo HMI_Start_Info;
	StationInfo HMI_End_Info;

	BYTE	bNextOpenSide_A;
	BYTE	bNextOpenSide_B;

	int nLocationInfo;

	BYTE    ATC_Open_A;
	BYTE    ATC_Open_B;
	BYTE    ATC_Close_A;
	BYTE    ATC_Close_B;



}RunStationInfo;



typedef enum RunStationEnum
{
	Enum_CloseDoor,			//����
	Enum_NextStation,		//����һվ
	Enum_PreLeaveStation,	//Ԥ��վ
	Enum_LeaveStation,		//��վ
	Enum_UniformRun,		//��������
	Enum_PreComStation,		//Ԥ��վ
	Enum_ComStation,		//��վ
	Enum_OpenDoor,			//����
};

typedef enum En_Feedback_State
{
// 	Enum_Empty,
// 	Enum_Normal,
// 	Enum_Alarm,
// 	Enum_PECU,
// 	Enum_FAULT
	Enum_Empty,
	Enum_Normal,//����
	Enum_FAULT,//����
	Enum_PECU,//ͨѶ
	Enum_CALL,//����
	Enum_ACTIVE//����
};

typedef void (__stdcall *OnRegDataViewSendCallback)(DWORD dwItem, void *pView, void *pVoid);




//��ʼ��TMS 
/*hWnd  pWnd Tab */
BHPANEL_TMS_API void BH_InitTMS(HWND hWnd, HWND *pWnd);
BHPANEL_TMS_API void BH_UnInitTMS();

//ע�����ݽ����ص�
BHPANEL_TMS_API void BH_RegCallBackSendDataView(OnRegDataViewSendCallback DataViewCallback, void *pCountxt);

//���ݽ�������
BHPANEL_TMS_API void BH_SetElementHighlight(void *pViewDlg, int n);

//��ȡ���ݽ�������
BHPANEL_TMS_API DWORD BH_GetViewDataByte(void* pViewDlg, int n);

//������ʾ��������
BHPANEL_TMS_API void BH_SetDataTextToStatic(char szbuf[1024]);

//�������ݷ���
BHPANEL_TMS_API void BH_UpdateDataView(const BYTE *pData, UINT uLen);

//�ⲿ����ҳ
BHPANEL_TMS_API void BH_InsertPageTab(CDialog *pDlg, char szItemTitle[32]);

//����ҳ����ʾ
BHPANEL_TMS_API void BH_OnEnableMainFrame();

//����TMS
BHPANEL_TMS_API void BH_StartRunTMS();

//ֹͣTMS����
BHPANEL_TMS_API void BH_StopRunTMS();

//��ͣ TMS ���� TRUE��ͣ  FALSE����
BHPANEL_TMS_API void BH_PauseRunTMS(BOOL bPause);

//��ȡ��ǰ״̬ վ����Ϣ
BHPANEL_TMS_API RunStationEnum BH_GetTMSRunState(RunStationInfo *pRunInfo);

//���ֵ֧��8�ڳ���+2��ͷ 30�з�����ʾ+1�г�����
BHPANEL_TMS_API void BH_SetFeedbackLine(int nCar, int nFeedbackLine);

//�����ı���ʾ״̬
BHPANEL_TMS_API void BH_SetCarVerticalState(int nCar, int nFeedback, En_Feedback_State nType);

//�����ı����
BHPANEL_TMS_API void BH_SetDrawLineText(int nCar, int nFeedbackLine, char szItemText[32]);

//���ÿ����Ų���Ϊ0
BHPANEL_TMS_API	void BH_Set_Open_Close_Zero();

BHPANEL_TMS_API	void BH_Set_Open_Port(BOOL bOpen);

BHPANEL_TMS_API void BH_Man_Auto_Set_Station(int CurSta, int NxtSta, int Start, int End);



//��ʼ��TMS 
/*hWnd  pWnd Tab */
/*
TMS_PANEL_API void TMS_InitTMS(HWND hWnd, HWND *pWnd);
TMS_PANEL_API void TMS_UnInitTMS();

//ע�����ݽ����ص�
TMS_PANEL_API void TMS_RegCallBackSendDataView(OnRegDataViewSendCallback DataViewCallback, void *pCountxt);

//���ݽ�������
TMS_PANEL_API void TMS_SetElementHighlight(void *pViewDlg, int n);

//��ȡ���ݽ�������
TMS_PANEL_API DWORD TMS_GetViewDataByte(void *pViewDlg, int n);

//������ʾ��������
TMS_PANEL_API void TMS_SetDataTextToStatic(char szbuf[1024]);

//�ⲿ����ҳ
TMS_PANEL_API void TMS_InsertPageTab(CDialog *pDlg, char szItemTitle[32]);

//����ҳ����ʾ
TMS_PANEL_API void TMS_OnEnableMainFrame();

//����TMS
TMS_PANEL_API void TMS_StartRunTMS();

//ֹͣTMS����
TMS_PANEL_API void TMS_StopRunTMS();

//��ͣ TMS ���� TRUE��ͣ  FALSE����
TMS_PANEL_API void TMS_PauseRunTMS(BOOL bPause);

//�������ݷ���
TMS_PANEL_API void TMS_UpdateDataView(const BYTE *pData, UINT uLen);

//��ȡ��ǰ״̬ վ����Ϣ
TMS_PANEL_API RunStationEnum TMS_GetTMSRunState(RunStationInfo *pRunInfo);

//���ֵ֧��8�ڳ���+2��ͷ 30�з�����ʾ+1�г�����
TMS_PANEL_API void TMS_SetFeedbackLine(int nCar, int nFeedbackLine);

//�����ı���ʾ״̬
TMS_PANEL_API void TMS_SetCarVerticalState(int nCar, int nFeedback, En_Feedback_State nType);

//�����ı����
TMS_PANEL_API void TMS_SetDrawLineText(int nCar, int nFeedbackLine, char szItemText[32]);
*/