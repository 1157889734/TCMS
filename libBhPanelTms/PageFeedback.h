#pragma once


// CPageFeedback 对话框

#define LINE_COLUMN_MAX 10
#define LINE_ROW_MAX	31

class CPageFeedback : public CDialog
{
	DECLARE_DYNAMIC(CPageFeedback)

public:
	CPageFeedback(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageFeedback();

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGE_FEEDBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	int nDrawLineRow;//行
	int nDrawLineColumn;//列
	int nTopStart;	//顶部开始位置
	int nLeftStart;	//左边开始位置

	BOOL m_bEableBorder;

	BOOL m_bExitThread;

	CRect m_RtFill[LINE_COLUMN_MAX][LINE_ROW_MAX];

	En_Feedback_State m_EnItemState[LINE_COLUMN_MAX][LINE_ROW_MAX];
	char m_szItemText[LINE_COLUMN_MAX][LINE_ROW_MAX][32];

	BOOL m_bRedrawWindow;

	void InitDrawVerticalText();
	void SetFeedbackEableBorder(BOOL bEable);
	void SetFeedbackPanel(int nCar, int nFeedback);
	void SetCarVerticalState(int nCar, int nFeedback, En_Feedback_State nType);
	void SetDrawLineText(int nCar, int nFeedbackLine, char szItemText[32]);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
