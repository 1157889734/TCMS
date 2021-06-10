// PageFeedback.cpp : 实现文件
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "PageFeedback.h"


#define FIGURE_NORMAL_CLOLOR   RGB(0,225,0)
#define FIGURE_FAULT_CLOLOR    RGB(255,0,0)
#define FIGURE_PECU_CLOLOR    RGB(255,255,0)
#define FIGURE_CALL_CLOLOR    RGB(0,255,255)
#define FIGURE_ACTIVE_CLOLOR    RGB(128,128,128)

// CPageFeedback 对话框

IMPLEMENT_DYNAMIC(CPageFeedback, CDialog)

CPageFeedback::CPageFeedback(CWnd* pParent /*=NULL*/)
	: CDialog(CPageFeedback::IDD, pParent)
{
	nDrawLineRow=LINE_ROW_MAX;
	nDrawLineColumn=LINE_COLUMN_MAX;
	nTopStart=40;
	nLeftStart=20;
	m_bRedrawWindow = FALSE;
	m_bExitThread = TRUE;
	m_bEableBorder = FALSE;

	for (int i=0; i<LINE_COLUMN_MAX; i++)
	{
		for (int j=0;j<LINE_ROW_MAX; j++)
		{
			m_EnItemState[i][j]=Enum_Normal;
		}		
	}	
	InitDrawVerticalText();	
}

CPageFeedback::~CPageFeedback()
{
	m_bExitThread = FALSE;
}

void CPageFeedback::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageFeedback, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPageFeedback 消息处理程序

void CPageFeedback::InitDrawVerticalText()
{
	for (int i=0; i<LINE_COLUMN_MAX; i++)
	{
		for (int j=0;j<LINE_ROW_MAX; j++)
		{
			memset(&m_szItemText[i][j],0x00, 32);
			m_RtFill[i][j].top=0;
			m_RtFill[i][j].bottom=0;
			m_RtFill[i][j].left=0;
			m_RtFill[i][j].right=0;
		}		
	}

	strcpy_s(m_szItemText[0][0],"司机室 1");
	strcpy_s(m_szItemText[nDrawLineColumn-1][0],"司机室 2");
	for (int i=1; i<nDrawLineColumn-1; i++)
	{
		char szTembuf[32]={0};
		sprintf_s(szTembuf, "客室   %d", i);
		strcpy_s(m_szItemText[i][0],szTembuf);
	}

	//	SetCarVerticalStata(0,5,Enum_Normal);

	for (int n=1; n<nDrawLineColumn-1; n++)
	{
		for (int i=1; i<nDrawLineRow; i++)
		{
			char szTembuf[32]={0};
			if (i<=5)
			{
				sprintf_s(szTembuf, "报警器   %d", i);
			}
			else if(i<=10)
			{
				sprintf_s(szTembuf, "报警状态 %d", i-5);
			}
			else
			{
				sprintf_s(szTembuf, "动态地图 %d", i-10);
			}			
			strcpy_s(m_szItemText[n][i],szTembuf);
		}
	}

	strcpy_s(m_szItemText[0][1],"TMS模块");
	strcpy_s(m_szItemText[0][2],"MVB模块");
	strcpy_s(m_szItemText[0][3],"音频处理器");
	strcpy_s(m_szItemText[0][4],"广播控制盒");
	strcpy_s(m_szItemText[0][5],"录音模块");
	strcpy_s(m_szItemText[0][6],"数字报站器");

	strcpy_s(m_szItemText[nDrawLineColumn-1][1],"TMS模块");
	strcpy_s(m_szItemText[nDrawLineColumn-1][2],"MVB模块");
	strcpy_s(m_szItemText[nDrawLineColumn-1][3],"音频处理器");
	strcpy_s(m_szItemText[nDrawLineColumn-1][4],"广播控制盒");
	strcpy_s(m_szItemText[nDrawLineColumn-1][5],"录音模块");
	strcpy_s(m_szItemText[nDrawLineColumn-1][6],"数字报站器");
}

void CPageFeedback::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename


	CFont* def_font = dc.SelectObject(&font);


	CRect rt;
	GetClientRect(rt);

	int nW = rt.Width()-20;
	int nH = rt.Height()-15;

	//
	CPen myPen1(PS_SOLID, 1, RGB(134,200,232));
	dc.SelectObject(&myPen1);
	
	float nLatticeH = (float)(nH-nTopStart)/nDrawLineRow;
	float nLatticeW = (float)(nW-nLeftStart)/nDrawLineColumn;

	if (m_bEableBorder)
	{
		int nY=0;
		for (int nRow=0; nRow<=nDrawLineRow; nRow++)
		{
			nY = nTopStart+(nLatticeH*nRow);
			dc.MoveTo(nLeftStart, nY);
			dc.LineTo(nW, nY);
		}

		for (int nVertical=0; nVertical<=nDrawLineColumn; nVertical++)
		{		
			dc.MoveTo(nLeftStart+nVertical*nLatticeW, nTopStart);
			dc.LineTo(nLeftStart+nVertical*nLatticeW, nY);	
		}
	}
	//	
	dc.SetBkMode(TRANSPARENT);
	//dc.SetTextColor(RGB(63,63,63));
	//dc.SetBkColor(RGB(0,255,0));
	
	int nCountLattice=0;
	int nSpliet = 2;
 	for (int n=0; n<nDrawLineColumn;n++)
 	{		
		for (int j=0; j<nDrawLineRow; j++)
		{			
			m_RtFill[n][j].left=(nLeftStart+n*nLatticeW)+nSpliet;
			m_RtFill[n][j].top=(nTopStart+j*nLatticeH)+nSpliet;
			m_RtFill[n][j].bottom=(m_RtFill[n][j].top+nLatticeH)-nSpliet;
			m_RtFill[n][j].right=(m_RtFill[n][j].left+nLatticeW)-nSpliet;

			if (strcmp(m_szItemText[n][j],"")!=0)
			{
				CBrush brush1;
				if (m_EnItemState[n][j]==Enum_Normal)
				{
					brush1.CreateSolidBrush(FIGURE_NORMAL_CLOLOR);
					dc.FillRect(m_RtFill[n][j], &brush1);
				}
				else if (m_EnItemState[n][j]==Enum_FAULT)
				{
					brush1.CreateSolidBrush(FIGURE_FAULT_CLOLOR);
					dc.FillRect(m_RtFill[n][j], &brush1);
				}
				else if (m_EnItemState[n][j]==Enum_PECU)
				{
					brush1.CreateSolidBrush(FIGURE_PECU_CLOLOR);
					dc.FillRect(m_RtFill[n][j], &brush1);
				}
				else if (m_EnItemState[n][j]==Enum_CALL)
				{
					brush1.CreateSolidBrush(FIGURE_CALL_CLOLOR);
					dc.FillRect(m_RtFill[n][j], &brush1);
				}
				else if (m_EnItemState[n][j]==Enum_ACTIVE)
				{
					brush1.CreateSolidBrush(FIGURE_ACTIVE_CLOLOR);
					dc.FillRect(m_RtFill[n][j], &brush1);
				}
			}			

			if (j==0)
			{
				dc.SetTextColor(RGB(128,0,0));
			}
			else
			{
				dc.SetTextColor(RGB(36,36,36));
			}
 			dc.DrawText(m_szItemText[n][j], m_RtFill[n][j], DT_CENTER|DT_SINGLELINE|DT_VCENTER);			
			nCountLattice++;
		}
 	}

	CRect rtNormal(20,13, 70, 34);
	CRect rtFault(80,13, 130, 34);
	CRect rtPecu(140,13, 190, 34);
	CRect rtCall(200,13, 250, 34);
	CRect rtActive(260,13, 310,34);

	
	//dc.SetBkMode(OPAQUE);
	//dc.SetBkColor(RGB(0,255,0));

	CBrush brushNormal;	
	brushNormal.CreateSolidBrush(FIGURE_NORMAL_CLOLOR);
	dc.FillRect(rtNormal, &brushNormal);
	dc.DrawText(_T("正常"), rtNormal, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	CBrush brushFault;
	brushFault.CreateSolidBrush(FIGURE_FAULT_CLOLOR);
	dc.FillRect(rtFault, &brushFault);
	dc.DrawText(_T("故障"), rtFault, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	CBrush brushPecu;
	brushPecu.CreateSolidBrush(FIGURE_PECU_CLOLOR);
	dc.FillRect(rtPecu, &brushPecu);
	dc.DrawText(_T("通讯"), rtPecu, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	CBrush brushCall;
	brushCall.CreateSolidBrush(FIGURE_CALL_CLOLOR);
	dc.FillRect(rtCall, &brushCall);
	dc.DrawText(_T("呼叫"), rtCall, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	CBrush brushActive;
	brushActive.CreateSolidBrush(FIGURE_ACTIVE_CLOLOR);
	dc.FillRect(rtActive, &brushActive);
	dc.DrawText(_T("激活"), rtActive, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	dc.SelectObject(def_font);
	font.DeleteObject(); 

	//TRACE("nW=%d, 20+nVertical*nLatticeW=%d \n", nW, nLeftStart+(7*nLatticeW));
}

void CPageFeedback::SetFeedbackPanel(int nCar, int nFeedback)
{
	if ( nFeedback>LINE_ROW_MAX || nFeedback<7 || nCar<3 || nCar>LINE_COLUMN_MAX )
	{
		return;
	}

	nDrawLineRow = nFeedback;
	nDrawLineColumn = nCar;

	InitDrawVerticalText();
}


void CPageFeedback::SetFeedbackEableBorder(BOOL bEable)
{
	m_bEableBorder = bEable;
}

UINT StartRefresh(LPVOID pParam)
{
	CPageFeedback *pPageBackdlg = (CPageFeedback*)pParam;
	if (pPageBackdlg)
	{
		while(pPageBackdlg->m_bExitThread)
		{
			if (pPageBackdlg->m_bRedrawWindow)
			{
				CRect rt;
				pPageBackdlg->GetClientRect(rt);

				rt.top = pPageBackdlg->nTopStart;
				rt.left = pPageBackdlg->nLeftStart;
				rt.bottom = rt.top+rt.Height()-15;
				rt.right = rt.left+rt.Width()-20;

				pPageBackdlg->RedrawWindow(rt);
				pPageBackdlg->m_bRedrawWindow = FALSE;
			}
			
			Sleep(1300);
		}	
	}
	return 1;
}

void CPageFeedback::SetCarVerticalState(int nCar, int nFeedback, En_Feedback_State nType)
{
	if (m_EnItemState[nCar][nFeedback] != nType && strcmp(m_szItemText[nCar][nFeedback],"")!=0)
	{
		m_EnItemState[nCar][nFeedback] = nType;
		m_bRedrawWindow = TRUE;
		RedrawWindow(m_RtFill[nCar][nFeedback]); 		
	}
// 	static bool b=false;
// 	if (!b)
// 	{
// 		b=true;
// 		AfxBeginThread(StartRefresh, this);
// 	}	
}

void CPageFeedback::SetDrawLineText(int nCar, int nFeedbackLine, char szItemText[32])
{
	strcpy_s(m_szItemText[nCar][nFeedbackLine], szItemText);
}

void CPageFeedback::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	/*
	CRect rt;
	GetClientRect(rt);

	rt.top = nTopStart;
	rt.left = nLeftStart;
	rt.bottom = rt.top+rt.Height()-15;
	rt.right = rt.left+rt.Width()-20;

	RedrawWindow(rt);
	KillTimer(1);
*/
	CDialog::OnTimer(nIDEvent);
}
