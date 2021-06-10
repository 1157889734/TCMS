// CPageLocalConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "PageLocalConfig.h"
#include "PanelMainFram.h"


extern CPanelMainFram *g_pMainFrame;

// CPageLocalConfig 对话框

IMPLEMENT_DYNAMIC(CPageLocalConfig, CDialog)

CPageLocalConfig::CPageLocalConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CPageLocalConfig::IDD, pParent)
{

}

CPageLocalConfig::~CPageLocalConfig()
{
}

void CPageLocalConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOCAL_SET, m_listLocalSet);
	DDX_Control(pDX, IDC_BTN_LOCAL_SET, m_btnLocalSet);
}


BEGIN_MESSAGE_MAP(CPageLocalConfig, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_LOCAL_SET, &CPageLocalConfig::OnBnClickedBtnLocalSet)
END_MESSAGE_MAP()


// CPageLocalConfig 消息处理程序

BOOL CPageLocalConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_listLocalSet.SetExtendedStyle(m_listLocalSet.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listLocalSet.InsertColumn(0, _T("本地配置"), LVCFMT_CENTER, 180);
	m_listLocalSet.InsertColumn(1, _T("选项"), LVCFMT_CENTER, 180);
	m_listLocalSet.InsertColumn(2, _T("说明"), LVCFMT_LEFT, 390);

	int arrColumnCtrl[64] = {0};
	arrColumnCtrl[0] = CTableCtrl::eLast;
	arrColumnCtrl[1] = CTableCtrl::eCombo;
	arrColumnCtrl[2] = CTableCtrl::eLast;
	m_listLocalSet.SetColumnCtrl(arrColumnCtrl);

	//列表行初始化配置
	char szTemp[32]={0};
	int i=0;
	int nRow = 0;

	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("启用"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("关闭"));
	m_listLocalSet.InsertItem(nRow, "紧急广播脉冲");
	m_listLocalSet.SetItemText(nRow, 2, _T("紧急广播后2秒后自动停发紧急广播位"));

	nRow++;
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("启用"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("关闭"));
	m_listLocalSet.InsertItem(nRow, "启动站点管理");
	m_listLocalSet.SetItemText(nRow, 2, _T("是否显示站点管理页面"));	

	nRow++;
	for (i=3; i<=10; i++)
	{
		sprintf_s(szTemp, "%d", i);
		m_listLocalSet.SetComBoxTextCtrl(nRow, szTemp);
	}
	m_listLocalSet.InsertItem(nRow, "车厢数(车头 2)");
	m_listLocalSet.SetItemText(nRow, 2, _T("反馈界面显示车厢数(如:3代表1节车厢2节车头)"));

	nRow++;
	for (i=7; i<=31; i++)
	{
		sprintf_s(szTemp, "%d", i);
		m_listLocalSet.SetComBoxTextCtrl(nRow, szTemp);
	}
	m_listLocalSet.InsertItem(nRow, "反馈显示行数");
	m_listLocalSet.SetItemText(nRow, 2, _T("反馈界面显示反馈项目条数"));

	nRow++;
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("启用"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("关闭"));
	m_listLocalSet.InsertItem(nRow, "启动反馈状态");
	m_listLocalSet.SetItemText(nRow, 2, _T("是否显示反馈状态页面"));	

	nRow++;
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("启用"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("关闭"));
	m_listLocalSet.InsertItem(nRow, "启动反馈边框");
	m_listLocalSet.SetItemText(nRow, 2, _T("是否显示反馈页面状态的边框"));	

	return TRUE;  // return TRUE unless you set the fo cus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPageLocalConfig::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_listLocalSet)
	{
		m_listLocalSet.MoveWindow(10, 10, cx-20, cy-50, TRUE);
	}
	if (m_btnLocalSet)
	{
		m_btnLocalSet.MoveWindow(cx-90, cy-35, 80, 30);
	}
	// TODO: 在此处添加消息处理程序代码
}


static int GetItemTextState(CString szItemText)
{
	int nTempValue=0;
	if (strcmp(szItemText, _T("启用"))==0)
	{
		nTempValue = 1;
	}
	else if (strcmp(szItemText, _T("关闭"))==0)
	{
		nTempValue = 0;
	}
	return nTempValue;
}

void CPageLocalConfig::OnBnClickedBtnLocalSet()
{
	// TODO: 在此添加控件通知处理程序代码
	LocalInfoSet stlocalInfo;
	memset(&stlocalInfo, 0x00, sizeof(LocalInfoSet));

	CString szItemText = m_listLocalSet.GetItemText(0, 1);
	stlocalInfo.bEnableEmcPulse = GetItemTextState(szItemText);

	szItemText = m_listLocalSet.GetItemText(1, 1);
	stlocalInfo.bEnableStationManage = GetItemTextState(szItemText);

	szItemText = m_listLocalSet.GetItemText(2, 1);
	stlocalInfo.nCarNum = atoi(szItemText);

	szItemText = m_listLocalSet.GetItemText(3, 1);
	stlocalInfo.nFeedBackNum = atoi(szItemText);	

	szItemText = m_listLocalSet.GetItemText(4, 1);
	stlocalInfo.bEnableFeedBack = GetItemTextState(szItemText);
	
	szItemText = m_listLocalSet.GetItemText(5, 1);
	stlocalInfo.bEnableFeedBackBorder = GetItemTextState(szItemText);

	stlocalInfo.bEnableLocalSetInfo = 1;

	g_pMainFrame->RefreshLocalSet(stlocalInfo);
	g_pMainFrame->WriteLocalInfo();

	MessageBox(_T("保存成功"), _T("提示"));
}

void CPageLocalConfig::OnRefreshLocalItem(LocalInfoSet stlocalInfo)
{
	CString TempItemText;
	TempItemText.Format("%s", stlocalInfo.bEnableEmcPulse?_T("启用"):_T("关闭"));
	m_listLocalSet.SetItemText(0, 1, TempItemText);

	TempItemText.Format("%s", stlocalInfo.bEnableStationManage?_T("启用"):_T("关闭"));
	m_listLocalSet.SetItemText(1, 1, TempItemText);

	TempItemText.Format("%d", stlocalInfo.nCarNum);
	m_listLocalSet.SetItemText(2, 1, TempItemText);

	TempItemText.Format("%d", stlocalInfo.nFeedBackNum);
	m_listLocalSet.SetItemText(3, 1, TempItemText);

	TempItemText.Format("%s", stlocalInfo.bEnableFeedBack?_T("启用"):_T("关闭"));
	m_listLocalSet.SetItemText(4, 1, TempItemText);

	TempItemText.Format("%s", stlocalInfo.bEnableFeedBackBorder?_T("启用"):_T("关闭"));
	m_listLocalSet.SetItemText(5, 1, TempItemText);
}


