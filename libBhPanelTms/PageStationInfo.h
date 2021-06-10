#pragma once
#include "afxcmn.h"

// CPageStationInfo 对话框

#define STATION_ITEM_MAX 50

class CPageStationInfo : public CDialog
{
	DECLARE_DYNAMIC(CPageStationInfo)

public:
	CPageStationInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageStationInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGE_STATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listStation;
	CRect     m_StationRt;
	static std::vector<StationInfo> m_vtManageStationInfo;

	BYTE *m_pItembuf[STATION_ITEM_MAX];	
	void SetListItemtext(StationInfo station);
	int  GetStationOnlyID();

	void OnRefreshStationData();

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedBtnSAdd();
	afx_msg void OnBnClickedBtnSSave();
	afx_msg void OnNMClickListStation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSModfiy();
	afx_msg void OnBnClickedBtnSDel();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	void OnRunTmsEnableWindowBtn(BOOL bEnable);
	afx_msg void OnBnClickedRadioHand();
	afx_msg void OnBnClickedRadioAuto();

};
