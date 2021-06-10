// CPageLocalConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libBhPanelTms.h"
#include "PageLocalConfig.h"
#include "PanelMainFram.h"


extern CPanelMainFram *g_pMainFrame;

// CPageLocalConfig �Ի���

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


// CPageLocalConfig ��Ϣ�������

BOOL CPageLocalConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_listLocalSet.SetExtendedStyle(m_listLocalSet.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listLocalSet.InsertColumn(0, _T("��������"), LVCFMT_CENTER, 180);
	m_listLocalSet.InsertColumn(1, _T("ѡ��"), LVCFMT_CENTER, 180);
	m_listLocalSet.InsertColumn(2, _T("˵��"), LVCFMT_LEFT, 390);

	int arrColumnCtrl[64] = {0};
	arrColumnCtrl[0] = CTableCtrl::eLast;
	arrColumnCtrl[1] = CTableCtrl::eCombo;
	arrColumnCtrl[2] = CTableCtrl::eLast;
	m_listLocalSet.SetColumnCtrl(arrColumnCtrl);

	//�б��г�ʼ������
	char szTemp[32]={0};
	int i=0;
	int nRow = 0;

	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("����"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("�ر�"));
	m_listLocalSet.InsertItem(nRow, "�����㲥����");
	m_listLocalSet.SetItemText(nRow, 2, _T("�����㲥��2����Զ�ͣ�������㲥λ"));

	nRow++;
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("����"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("�ر�"));
	m_listLocalSet.InsertItem(nRow, "����վ�����");
	m_listLocalSet.SetItemText(nRow, 2, _T("�Ƿ���ʾվ�����ҳ��"));	

	nRow++;
	for (i=3; i<=10; i++)
	{
		sprintf_s(szTemp, "%d", i);
		m_listLocalSet.SetComBoxTextCtrl(nRow, szTemp);
	}
	m_listLocalSet.InsertItem(nRow, "������(��ͷ 2)");
	m_listLocalSet.SetItemText(nRow, 2, _T("����������ʾ������(��:3����1�ڳ���2�ڳ�ͷ)"));

	nRow++;
	for (i=7; i<=31; i++)
	{
		sprintf_s(szTemp, "%d", i);
		m_listLocalSet.SetComBoxTextCtrl(nRow, szTemp);
	}
	m_listLocalSet.InsertItem(nRow, "������ʾ����");
	m_listLocalSet.SetItemText(nRow, 2, _T("����������ʾ������Ŀ����"));

	nRow++;
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("����"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("�ر�"));
	m_listLocalSet.InsertItem(nRow, "��������״̬");
	m_listLocalSet.SetItemText(nRow, 2, _T("�Ƿ���ʾ����״̬ҳ��"));	

	nRow++;
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("����"));
	m_listLocalSet.SetComBoxTextCtrl(nRow, _T("�ر�"));
	m_listLocalSet.InsertItem(nRow, "���������߿�");
	m_listLocalSet.SetItemText(nRow, 2, _T("�Ƿ���ʾ����ҳ��״̬�ı߿�"));	

	return TRUE;  // return TRUE unless you set the fo cus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ˴������Ϣ����������
}


static int GetItemTextState(CString szItemText)
{
	int nTempValue=0;
	if (strcmp(szItemText, _T("����"))==0)
	{
		nTempValue = 1;
	}
	else if (strcmp(szItemText, _T("�ر�"))==0)
	{
		nTempValue = 0;
	}
	return nTempValue;
}

void CPageLocalConfig::OnBnClickedBtnLocalSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	MessageBox(_T("����ɹ�"), _T("��ʾ"));
}

void CPageLocalConfig::OnRefreshLocalItem(LocalInfoSet stlocalInfo)
{
	CString TempItemText;
	TempItemText.Format("%s", stlocalInfo.bEnableEmcPulse?_T("����"):_T("�ر�"));
	m_listLocalSet.SetItemText(0, 1, TempItemText);

	TempItemText.Format("%s", stlocalInfo.bEnableStationManage?_T("����"):_T("�ر�"));
	m_listLocalSet.SetItemText(1, 1, TempItemText);

	TempItemText.Format("%d", stlocalInfo.nCarNum);
	m_listLocalSet.SetItemText(2, 1, TempItemText);

	TempItemText.Format("%d", stlocalInfo.nFeedBackNum);
	m_listLocalSet.SetItemText(3, 1, TempItemText);

	TempItemText.Format("%s", stlocalInfo.bEnableFeedBack?_T("����"):_T("�ر�"));
	m_listLocalSet.SetItemText(4, 1, TempItemText);

	TempItemText.Format("%s", stlocalInfo.bEnableFeedBackBorder?_T("����"):_T("�ر�"));
	m_listLocalSet.SetItemText(5, 1, TempItemText);
}


