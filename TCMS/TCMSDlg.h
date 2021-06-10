// TCMSDlg.h : ͷ�ļ�
//

#pragma once

#include "TCMSCtrlDlg.h"
#include "DoorFaultDlg.h"
#include "CESeries.h"


#define BUF_LEN 1024	//��������С

// CTCMSDlg �Ի���
class CTCMSDlg : public CDialog
{
// ����
public:
	CTCMSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCMS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

protected:
	CTCMSCtrlDlg m_TCMSCtrlDlg;	
	CDoorFaultDlg m_DoorFaultDlg;

	WORD m_wSignal;
	WORD m_wVCM_Life;
	int  m_nSendTime;

public:
	static CTCMSDlg* GetTCMSDlgApp();
	static void __stdcall RegEseriesDataFunction(long lComId, BYTE *pRecvbuf, int nleng, void *pCountxt);
	static void __stdcall OnRegDataViewSendCallbackFunction(DWORD dwItem, void *pView, void *pVoid);

	void OnSendUrumqiData();
	BOOL OnStartTcms(int nComNo);
	void OnPause(BOOL bPause);
	void OnStopTcms();

	CCESeries *m_pEseries;
	afx_msg LRESULT OnCommPort_Data(WPARAM wParam,LPARAM lParam);
	int ProcessComData(const BYTE *pData, int nLen);

	BYTE *m_pRecvBuf;
	int m_iRecvBufLen;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
