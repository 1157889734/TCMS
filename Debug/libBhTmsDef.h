#pragma once

#ifdef DVSSDK_EXPORTS
#define BHPANEL_TMS_API extern "C" __declspec(dllexport)
#else
#define BHPANEL_TMS_API extern "C" __declspec(dllimport)
#endif


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
	//Modify by Johnny in 2017-07-27 10:53:57
	WORD	bRunSpeed;			//��ǰ�����ٶ�
	BYTE	bPlayEmc;			//���Ž����㲥
	int		dwEmcIndexId;		//�����㲥ID��
	int		dwZoneDist;			//����վ�ľ���
	bool	bArriveBC;			// ��վ�㲥�ź�
	bool	bLeaveBC;			// ��վ�㲥�ź�
	int		dwLastDist;			// �г�����һվ�ľ���
	BYTE	bJumpUp;			//����Խվ
	BYTE	bJumpDown;			//����Խվ
	BYTE	bCBTC;				//CBTCģʽ����
	BYTE	bDateTimeValid;		//ʱ����Ч
	BYTE	bLineMap;			//��·��ͼ
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
// 	Enum_Alarm
	Enum_Empty,
	Enum_Normal,//����
	Enum_FAULT,//����
	Enum_PECU,//ͨѶ����
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