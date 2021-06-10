#pragma once
#include "afxwin.h"
#include "MyCenterStatic.h"
#include "afxcmn.h"
#include "DataAnalysisDlg.h"

//----------------------------------
#include "PageStationInfo.h"

// CPageRunData �Ի���

class CPageRunData : public CDialog
{
	DECLARE_DYNAMIC(CPageRunData)

public:
	CPageRunData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageRunData();

// �Ի�������
	enum { IDD = IDD_DIALOG_PAGE_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CMyCenterStatic m_sFrontStation;
	CMyCenterStatic m_sNextStation;
	CMyCenterStatic m_sDirect;
	CProgressCtrl   m_pDistance;
	CListCtrl       m_ListBc;
	CDataAnalysisDlg* m_pDataAnalysisDlg;

	std::vector<StationInfo> m_vtRunStationInfo;

	RunStationEnum m_EnumCurrentState;
	RunStationEnum m_EnumOldState;

	void InsertComBoxStation(BOOL bUpstream);	
	void OnRunStation();
	void ReadRunLocalInfo();
	int  NextOrUpStation(int nNext);
	void CalcRunSpeed(int nSpeed);
	void CalDisForAccel();
	void RunEnableWindow(BOOL bEnable);
public:
	void StartRunStation();
	void StopRunStation();
	void PauseRunStation(BOOL bPause);
	RunStationEnum GetCurrentStationInfo(RunStationInfo *pStationInfo);
	void OnEnablePulse(BOOL bpulse);
	void RefreshRunDataPage();
	void InitDataAnalysisDlg();
	void InitNextStationCombox();
private:
	int				m_nFrontStation;	 //��ǰվ
	int				m_nNextStation;		 //��һվ
	BOOL			m_bIsUpDirect;		 //������	
	int             m_nStartStation;	 //��ʼվ
	int             m_nEndStation;		 //�յ�վ
	BOOL			m_bPause;			 //1ͣ�� 0����
	int				m_nEvenVel; 		 //�г�ƽ���ٶ�  km/h
	int				m_nDisForAccl;		 //���پ���
	int				m_nTimeCount; 		 //ʱ���������ÿ20ms��1
	int				m_nZoneDist; 		 //վ����룬�̶�
	int				m_nCurZoneDist; 	 //����һվ�ľ���
	int				m_nTimeCountWA; 	 //��ʼ����ʱ��ʱ�����
	int				m_nDisForArri; 		 //��վǰ20s����
	int				m_nDisForLeave;		 //��ʼ��վ20s����
	int				m_nPerDist; 		 //����ʱÿ200ms�ľ���
	int				m_nCurrentSpeed;	 //��ǰ�ٶ�	km / h
	int				m_nLastStationDist;	 //�г�����һվ�ľ���

//	BOOL			m_bAnounceFlag;		 //�����㲥
	bool			m_bPlayBC;		 	 //���Ž����㲥
	int				m_nAnounceIndex;     //�����㲥���к�
	bool			m_bArriveBC;		 //��վ�㲥
	bool			m_bLeaveBC;			 //��վ�㲥

	BOOL			m_bEmcPulse;		 //�����㲥�����Ƿ�����
	BYTE			m_bJumpUp;			 //����Խվ
	BYTE			m_bJumpDown;		 //����Խվ

	BYTE			m_bDateTimeValid;
	BYTE			m_bClosedLeft;		 //���Źغ�
	BYTE			m_bClosedRight;		 //���Źغ�

	BYTE			m_bCloseLeftDoor;	 //������
	BYTE			m_bCloseRightDoor;	 //������
	BYTE			m_bOpenLeftDoor;	 //������
	BYTE			m_bOpenRightDoor;	 //������

	BYTE			m_bATC_Open_A;   //ATC������
	BYTE			m_bATC_Open_B;	 //ATC������
	BYTE			m_bATC_Close_A;	 //ATC������
	BYTE			m_bATC_Close_B;  //ATC������
public:

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMClickListBc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnAppry();
	afx_msg void OnBnClickedBtnAddvel();
	afx_msg void OnBnClickedBtnSubvel();	
	afx_msg void OnBnClickedBtnStationUp();
	afx_msg void OnBnClickedBtnStationNext();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedRadioUp();
	afx_msg void OnBnClickedBtnStartEmc();
	afx_msg void OnBnClickedBtnStopEmc();
	afx_msg void OnBnClickedRadioHmi();
	afx_msg void OnBnClickedRadioAtc();
	afx_msg void OnCbnSelchangeComboLineid();
	afx_msg void OnBnClickedBtnArrive();
	afx_msg void OnBnClickedBtnLeave();
	afx_msg void OnBnClickedBtnDataView();
	afx_msg void OnBnClickedButtonOpenDoor();
	afx_msg void OnBnClickedButtonCloseDoor();
	afx_msg void OnBnClickedButtonSetNextStation();
	afx_msg void OnBnClickedButtonDatetimeValid();
	afx_msg void OnBnClickedButtonStartStopStation();

	afx_msg void OnBnClickedCheckAtoMode();
	afx_msg void OnBnClickedCheckPmMode();
	afx_msg void OnBnClickedCheckAtcMode();
	afx_msg void OnBnClickedCheckTcmsMode();
	afx_msg void OnBnClickedBtnStartRun();

	int  Reset_Param(void);	
	void SetMode(void);
	void EnableWnd_Btn(bool bEnable);
	void Set_Open_Close_Zero(void);	
	
	int m_nRunMode;
	int m_nLastSpeed;
	int m_nLastStart;
	int m_nLastEnd;
	int	m_nHMI_Start;
	int	m_nHMI_End;
	int	m_nHMI_Next;
	int m_nLast_Run_Mode;

	float m_nSumDist;
	float m_nSumDist1;
	BOOL m_bOpen_Port;
	afx_msg void OnBnClickedButtonOpenRightDoor();
	afx_msg void OnBnClickedButtonCloseRightDoor();
	int Man_Auto_Set_Station(int Cur, int Nxt, int Start, int End);
	CButton m_BTN_Next_Open_Left;
	CButton m_BTN_Next_Open_Right;
	CStatic m_STATIC_Next_Open_Side;
	afx_msg void OnBnClickedCheckLeftDoor();
	afx_msg void OnBnClickedCheckRightDoor();
	CStatic m_STATIC_Cur_Door_State;
	int m_nLocationInfo;
};
