// libBhPanelTms.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "PanelMainFram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// ClibBhPanelTmsApp

BEGIN_MESSAGE_MAP(ClibBhPanelTmsApp, CWinApp)
END_MESSAGE_MAP()


// ClibBhPanelTmsApp ����

ClibBhPanelTmsApp::ClibBhPanelTmsApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� ClibBhPanelTmsApp ����

ClibBhPanelTmsApp theApp;


// ClibBhPanelTmsApp ��ʼ��

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
