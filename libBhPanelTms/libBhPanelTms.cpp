// libBhPanelTms.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "PanelMainFram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// ClibBhPanelTmsApp

BEGIN_MESSAGE_MAP(ClibBhPanelTmsApp, CWinApp)
END_MESSAGE_MAP()


// ClibBhPanelTmsApp 构造

ClibBhPanelTmsApp::ClibBhPanelTmsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 ClibBhPanelTmsApp 对象

ClibBhPanelTmsApp theApp;


// ClibBhPanelTmsApp 初始化

BOOL ClibBhPanelTmsApp::InitInstance()
{
	CWinApp::InitInstance();
	

	return TRUE;
}

CPanelMainFram *g_pMainFrame=NULL;
HWND g_MainFramePranthWnd   =NULL;

void BH_InitTMS(HWND hWnd, HWND *pWnd)
{
	g_pMainFrame = new CPanelMainFram;
	if (g_pMainFrame)
	{
		g_pMainFrame->Create(IDD_DIALOG_MIANFRAME, (CWnd*)CWnd::FromHandle(hWnd));

		*pWnd = g_pMainFrame->m_tMainTab;	
		g_MainFramePranthWnd = hWnd;
	}
}

void BH_UnInitTMS()
{
	if (g_pMainFrame)
	{
		g_pMainFrame->DestroyWindow();
		delete g_pMainFrame;
		g_pMainFrame = NULL;
	}
}

RunStationEnum BH_GetTMSRunState(RunStationInfo *pRunInfo)
{
	return g_pMainFrame->m_pageRunDlg.GetCurrentStationInfo(pRunInfo);
}

void BH_RegCallBackSendDataView(OnRegDataViewSendCallback DataViewCallback, void *pCountxt)
{
	g_pMainFrame->m_pageRunDlg.m_pDataAnalysisDlg->m_pDataViewDlg->SetDataAnalysisCallback(DataViewCallback, pCountxt);
}

void BH_SetElementHighlight(void *pViewDlg, int n)
{
	((CDlgDataView*)pViewDlg)->SetElementHighlight(n);
}

DWORD BH_GetViewDataByte(void *pViewDlg, int n)
{
	return (((CDlgDataView*)pViewDlg)->GetDataByte(n));
}

void BH_SetDataTextToStatic(char szbuf[1024])
{
	g_pMainFrame->m_pageRunDlg.m_pDataAnalysisDlg->m_stcSendViewInfo.SetWindowText(szbuf);
}

void BH_UpdateDataView(const BYTE *pData, UINT uLen)
{
	g_pMainFrame->m_pageRunDlg.m_pDataAnalysisDlg->m_pDataViewDlg->UpdateDataView(pData, uLen);
}

void BH_InsertPageTab(CDialog *pDlg, char szItemTitle[32])
{
	g_pMainFrame->InsertPageTab(pDlg, szItemTitle);
}

void BH_OnEnableMainFrame()
{
	g_pMainFrame->OnEnableMainFrame();
	CRect rt;
	((CWnd*)CWnd::FromHandle(g_MainFramePranthWnd))->GetClientRect(rt);
	//g_pMainFrame->MoveWindow(0, 0, 800, 700, TRUE);

	//rt.right -= 50;
	g_pMainFrame->MoveWindow(rt, TRUE);
	g_pMainFrame->ShowWindow(SW_SHOW);
}

void BH_StartRunTMS()
{
	g_pMainFrame->m_pageRunDlg.StartRunStation();
	g_pMainFrame->m_PageStationDlg.OnRunTmsEnableWindowBtn(FALSE);
}

void BH_StopRunTMS()
{
	g_pMainFrame->m_pageRunDlg.StopRunStation();
	g_pMainFrame->m_pageRunDlg.EnableWnd_Btn(TRUE);
	g_pMainFrame->m_PageStationDlg.OnRunTmsEnableWindowBtn(TRUE);
}

void BH_PauseRunTMS(BOOL bPause)
{
	g_pMainFrame->m_pageRunDlg.PauseRunStation(bPause);
}

void BH_Set_Open_Close_Zero()
{
	g_pMainFrame->m_pageRunDlg.Set_Open_Close_Zero();
}

void BH_Set_Open_Port(BOOL bOpen)
{
	g_pMainFrame->m_pageRunDlg.m_bOpen_Port = bOpen;
}

void BH_SetFeedbackLine(int nCar, int nFeedbackLine)
{
	g_pMainFrame->m_PageFeedBackDlg.SetFeedbackPanel(nCar, nFeedbackLine);
}

void BH_SetCarVerticalState(int nCar, int nFeedback, En_Feedback_State nType)
{
	g_pMainFrame->m_PageFeedBackDlg.SetCarVerticalState(nCar, nFeedback, nType);
}

void BH_SetDrawLineText(int nCar, int nFeedbackLine, char szItemText[32])
{
	g_pMainFrame->m_PageFeedBackDlg.SetDrawLineText(nCar, nFeedbackLine, szItemText);
}

void BH_Man_Auto_Set_Station(int CurSta, int NxtSta, int Start, int End)
{
	g_pMainFrame->m_pageRunDlg.Man_Auto_Set_Station(CurSta, NxtSta, Start, End);
}
