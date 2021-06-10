#pragma once
#include "afxcmn.h"
#include "PageRunData.h"
#include "PageStationInfo.h"
#include "PageFeedback.h"
#include "PageLocalConfig.h"

// CPanelMainFram �Ի���

#define TAB_PAGE_MAX 6

class CPanelMainFram : public CDialog
{
	DECLARE_DYNAMIC(CPanelMainFram)

public:
	CPanelMainFram(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPanelMainFram();

// �Ի�������
	enum { IDD = IDD_DIALOG_MIANFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tMainTab;
	CPageRunData m_pageRunDlg;
	CPageFeedback m_PageFeedBackDlg;
	CPageStationInfo m_PageStationDlg;
	CPageLocalConfig m_PageLocalConfig;

	CDialog  *m_pTabArryDlg[TAB_PAGE_MAX];

	int  m_nTabPageIndex;
	void DoTab(int nTab);
public:
	virtual BOOL OnInitDialog();

public:
	void OnInitFrame();
	CString m_strAppPath;
	char m_szStationInfoPath[255];	//�Զ�վ����
	char m_szHandStationInfoPath[255];	//�ֶ�վ����
	std::vector<StationInfo> m_vtStationInfo;
	std::vector<StationInfo> m_vtHandStationInfo;
	void WriteStationFile(std::vector<StationInfo> vtInfo, BOOL bAuto=TRUE);	//�Զ�ģʽ�µ�վ����
	void ReadStationFile();

	void WriteLocalInfo();
	void ReadLoaclInfo();

	char m_szLocalInfoPath[255];
	LocalInfoSet m_LocalInfo;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void MoveTabPage();
	void InsertPageTab(CDialog *pDlg, char szItemTitle[32]);

	void RefreshLocalSet(LocalInfoSet stlocalInfo, BOOL bReplace=TRUE);

	void DeleteTabItem(CDialog  *pDlg);
	void InsertTabItem(int nItemIndex, LPCTSTR lpszItem, CDialog  *pDlg);
	BOOL IsTabItemWindowVisible(CDialog  *pDlg);

	void OnEnableMainFrame();
	void OnRefreshSystemData();
};
