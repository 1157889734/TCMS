#pragma once
#include "afxcmn.h"
#include "ctrl/TableCtrl.h"
#include "afxwin.h"

// CPageLocalConfig 对话框

class CPageLocalConfig : public CDialog
{
	DECLARE_DYNAMIC(CPageLocalConfig)

public:
	CPageLocalConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageLocalConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGE_LOCAL_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	/*CListCtrl*/CTableCtrl m_listLocalSet;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedBtnLocalSet();
public:
	CButton m_btnLocalSet;

	void OnRefreshLocalItem(LocalInfoSet stlocalInfo);
};
