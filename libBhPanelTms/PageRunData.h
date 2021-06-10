#pragma once
#include "afxwin.h"
#include "MyCenterStatic.h"
#include "afxcmn.h"
#include "DataAnalysisDlg.h"

//----------------------------------
#include "PageStationInfo.h"

// CPageRunData 对话框

class CPageRunData : public CDialog
{
	DECLARE_DYNAMIC(CPageRunData)

public:
	CPageRunData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageRunData();

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGE_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	int				m_nFrontStation;	 //当前站
	int				m_nNextStation;		 //下一站
	BOOL			m_bIsUpDirect;		 //上下行	
	int             m_nStartStation;	 //起始站
	int             m_nEndStation;		 //终点站
	BOOL			m_bPause;			 //1停车 0正常
	int				m_nEvenVel; 		 //列车平稳速度  km/h
	int				m_nDisForAccl;		 //加速距离
	int				m_nTimeCount; 		 //时间计数，隔每20ms加1
	int				m_nZoneDist; 		 //站间距离，固定
	int				m_nCurZoneDist; 	 //到下一站的距离
	int				m_nTimeCountWA; 	 //开始减速时的时间计数
	int				m_nDisForArri; 		 //到站前20s距离
	int				m_nDisForLeave;		 //开始离站20s距离
	int				m_nPerDist; 		 //均速时每200ms的距离
	int				m_nCurrentSpeed;	 //当前速度	km / h
	int				m_nLastStationDist;	 //列车距上一站的距离

//	BOOL			m_bAnounceFlag;		 //紧急广播
	bool			m_bPlayBC;		 	 //播放紧急广播
	int				m_nAnounceIndex;     //紧急广播序列号
	bool			m_bArriveBC;		 //到站广播
	bool			m_bLeaveBC;			 //离站广播

	BOOL			m_bEmcPulse;		 //紧急广播脉冲是否启动
	BYTE			m_bJumpUp;			 //向上越站
	BYTE			m_bJumpDown;		 //向下越站

	BYTE			m_bDateTimeValid;
	BYTE			m_bClosedLeft;		 //左门关好
	BYTE			m_bClosedRight;		 //右门关好

	BYTE			m_bCloseLeftDoor;	 //关左门
	BYTE			m_bCloseRightDoor;	 //关右门
	BYTE			m_bOpenLeftDoor;	 //开左门
	BYTE			m_bOpenRightDoor;	 //开右门

	BYTE			m_bATC_Open_A;   //ATC开左门
	BYTE			m_bATC_Open_B;	 //ATC开右门
	BYTE			m_bATC_Close_A;	 //ATC关左门
	BYTE			m_bATC_Close_B;  //ATC关右门
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
