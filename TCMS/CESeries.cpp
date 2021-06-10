// CESeries.cpp: implementation of the CCESeries class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CESeries.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//������д�̷߳��͵���Ϣ����
const int CM_THREADCOMMWRITE = WM_USER+110;
const int CM_THREADCOMMGETNEWCHAR = WM_USER+111;
#define DATAHEAD 0x7E
#define DATATAIL 0x0D

//��CCESeries���캯��
CCESeries::CCESeries()
{
	idstr.Empty();
	m_hComm = INVALID_HANDLE_VALUE;
	m_hReadCloseEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hWriteCloseEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	::InitializeCriticalSection(&m_Csection);	
}

//��CCESeries��������
CCESeries::~CCESeries()
{
	ClosePort();
	CloseHandle(m_hReadCloseEvent);
	CloseHandle(m_hWriteCloseEvent);
	::DeleteCriticalSection(&m_Csection);
}

/*
*�������ܣ��򿪴���
*��ڲ�����pPortOwner	:ʹ�ô˴�����Ĵ�����
		   portNo		:���ں�
		   baud			:������
		   parity		:��żУ��
		   databits		:����λ
		   stopbits		:ֹͣλ
*���ڲ�����(��)
*����ֵ��TRUE:�ɹ��򿪴���;FALSE:�򿪴���ʧ��
*/
BOOL CCESeries::OpenPort(CWnd* pPortOwner,			/*ʹ�ô����࣬������*/
						 UINT portNo	,			/*���ں�*/
						 UINT baud		,			/*������*/
						 UINT parity	,			/*��żУ��*/
						 UINT databits	,			/*����λ*/
						 UINT stopbits			   /*ֹͣλ*/
						 )
{
	DCB commParam;
	TCHAR szPort[15];	
	
	// �Ѿ��򿪵Ļ���ֱ�ӷ���
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}
	ASSERT(pPortOwner != NULL);
	
	//���ô�����
// 	if (portNo >= 10)
// 	{
		wsprintf(szPort, _T("\\\\.\\COM%d"), portNo);
// 	}
// 	else
// 	{
// 		wsprintf(szPort, _T("COM%d"), portNo);
// 	}
	
	//�򿪴���
	m_hComm = CreateFile(
		szPort,
		GENERIC_READ | GENERIC_WRITE,	//�������д
		0,								//��ռ��ʽ������ģʽ��
		NULL,
		OPEN_EXISTING,					//�򿪶����Ǵ�����������ʽ��
		FILE_FLAG_OVERLAPPED,
		NULL 
		);
	idstr.Format(_T("ID:Com%d"),portNo);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// ��Ч���,���ء�		
		return FALSE;
	}
	
	// �õ��򿪴��ڵĵ�ǰ���Բ������޸ĺ����������ô��ڡ�
	// ���ô��ڵĳ�ʱ����Ϊ�������ء�
	
	if (!GetCommState(m_hComm,&commParam))
	{		
		//�رմ���
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	GetCommState(m_hComm,&commParam);
	commParam.BaudRate = baud;					// ���ò����� 
	commParam.fBinary = TRUE;					// ���ö�����ģʽ���˴���������TRUE
	commParam.fParity = FALSE;//TRUE;					// ֧����żУ�� 
	commParam.ByteSize = databits;				// ����λ,��Χ:4-8 
	commParam.Parity = parity;					// У��ģʽ
	commParam.StopBits = stopbits;				// ֹͣλ 
	
	commParam.fOutxCtsFlow = FALSE;				// No CTS output flow control 
	commParam.fOutxDsrFlow = FALSE;				// No DSR output flow control 
	commParam.fDtrControl = DTR_CONTROL_ENABLE; 
	// DTR flow control type 
	commParam.fDsrSensitivity = FALSE;			// DSR sensitivity 
	commParam.fTXContinueOnXoff = FALSE;		// XOFF continues Tx 
	commParam.fOutX = FALSE;					// No XON/XOFF out flow control 
	commParam.fInX = FALSE;						// No XON/XOFF in flow control 
	commParam.fErrorChar = FALSE;				// Disable error replacement 
	commParam.fNull = FALSE;					// Disable null stripping 

	commParam.fRtsControl = RTS_CONTROL_ENABLE; //RTS_CONTROL_ENABLE; 
	// 485�Ķ�
	// RTS flow control 
	commParam.fAbortOnError = FALSE;			// �����ڷ������󣬲�����ֹ���ڶ�д
	
	if (!SetCommState(m_hComm, &commParam))
	{
		//�رմ���
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	
    //���ô��ڶ�д��ʱʱ��
	COMMTIMEOUTS CommTimeOuts;
	//�õ���ʱ����
	GetCommTimeouts (m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;  
	CommTimeOuts.WriteTotalTimeoutConstant = 0;  
	
	if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
	{
		//�رմ���
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	
	m_pPortOwner = pPortOwner;	
	
	//ָ���˿ڼ����¼���, �����߳���ָ��
	SetCommMask (m_hComm, EV_RXCHAR);
	
	//�����豸������
	SetupComm(m_hComm,512,512);
	
	//��ʼ���������е���Ϣ
	PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);

	EscapeCommFunction(m_hComm,CLRDTR);
	EscapeCommFunction(m_hComm,CLRRTS);
	
	ResetEvent(m_hReadCloseEvent);
	ResetEvent(m_hWriteCloseEvent);
	//�����������߳�
	m_hReadThread = CreateThread(NULL,0,ReadThreadFunc,this,CREATE_SUSPENDED,&m_dwReadThreadID);
	//����д�����߳�
	m_hWriteThread = CreateThread(NULL,0,WriteThreadFunc,this,CREATE_SUSPENDED,&m_dwWriteThreadID);

	ResumeThread(m_hWriteThread);
	ResumeThread(m_hReadThread);

	return TRUE;
}

/*
*�������ܣ��رմ���
*��ڲ�����(��)
*���ڲ�����(��)
*����ֵ��  (��)
*/
void CCESeries::ClosePort()
{	
	//��ʾ���ڻ�û�д�
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	
	//�رն��߳�
	CloseReadThread();
	//�ر�д�߳�
	CloseWriteThread();
	
	//�رմ���
	if (CloseHandle (m_hComm))
	{
		m_hComm = INVALID_HANDLE_VALUE;
		return ;
	}
}

/*
*�������ܣ��򴮿ڷ�������
*��ڲ�����buf		: ��Ҫ������д������ݵĻ�����
		   bufLen	: ��Ҫ������д������ݵĻ���������
*���ڲ�����(��)
*����ֵ��TRUE:��ʾ�ɹ��ؽ�Ҫ���͵����ݴ��ݵ�д�߳���Ϣ���С�
		 FALSE:��ʾ��Ҫ���͵����ݴ��ݵ�д�߳���Ϣ����ʧ�ܡ�
		 ע�ӣ��˴���TRUE,��ֱ�Ӵ�������һ���ɹ�д�뵽�����ˡ�
*/
BOOL CCESeries::WritePort(const BYTE *buf,DWORD bufLen)
{
	//��Ҫ���͵����ݴ��ݵ�д�߳���Ϣ����
	if (PostThreadMessage(m_dwWriteThreadID, CM_THREADCOMMWRITE,
		WPARAM(bufLen), LPARAM(buf)))
	{
		return TRUE;
	}
	return FALSE;
}

/*
*�������ܣ����ô��ڶ�ȡ��д�볬ʱ
*��ڲ�����CommTimeOuts : ָ��COMMTIMEOUTS�ṹ
*���ڲ�����(��)
*����ֵ��TRUE:���óɹ�;FALSE:����ʧ��
*/
BOOL CCESeries::SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	return SetCommTimeouts(m_hComm,&CommTimeOuts);
}


//���ڶ��̺߳���
DWORD CCESeries::ReadThreadFunc(LPVOID lparam)
{
	CCESeries *ceSeries = (CCESeries*)lparam;
	
	DWORD	evtMask;
	BYTE * readBuf = NULL;//��ȡ���ֽ�
	DWORD actualReadLen=0;//ʵ�ʶ�ȡ���ֽ���
	DWORD willReadLen;
	DWORD dwCommModemStatus;
	
	DWORD dwReadErrors;
	COMSTAT	cmState;

	OVERLAPPED ovn;
	HANDLE hevent;
	hevent=CreateEvent(NULL,TRUE,FALSE,NULL);
	ZeroMemory(&ovn,sizeof(ovn));
	ovn.hEvent=hevent;
	
	// ��ջ��壬����鴮���Ƿ�򿪡�
	ASSERT(ceSeries->m_hComm !=INVALID_HANDLE_VALUE); 
	
	
	//��մ���
	PurgeComm(ceSeries->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );
	
	SetCommMask(ceSeries->m_hComm, EV_RXCHAR);
	
	//����յ����߳��˳��źţ����˳��߳�
	while (WaitForSingleObject(ceSeries->m_hReadCloseEvent,1) == WAIT_TIMEOUT)
	{   	
		SetCommMask (ceSeries->m_hComm, EV_RXCHAR);

		if (WaitCommEvent(ceSeries->m_hComm,&evtMask,0))
		{		
			ClearCommError(ceSeries->m_hComm,&dwReadErrors,&cmState);
			//��ʾ�����յ��ַ�		
			if (evtMask & EV_RXCHAR) 
			{
				willReadLen = cmState.cbInQue ;
				if (willReadLen <= 0)
				{
					continue;
				}
				readBuf=(BYTE*)malloc(willReadLen);
				::EnterCriticalSection(&ceSeries->m_Csection);
				BOOL bret=ReadFile(ceSeries->m_hComm, readBuf, willReadLen, &actualReadLen,&ovn);
				if(!bret)
				{
					if(GetLastError()==ERROR_IO_PENDING)
					{
						//////////////////////////////////////////////////////////////////
						DWORD des;
						des=WaitForSingleObject(ovn.hEvent,1000);
						if(des==WAIT_OBJECT_0)//
						{
							BOOL bResult = GetOverlappedResult(ceSeries->m_hComm, &ovn, 
																&actualReadLen, FALSE) ; 
							// if there was a problem ... 
							if (!bResult) 
								actualReadLen=0;
						}
					}
				}
				::LeaveCriticalSection(&ceSeries->m_Csection);
				
				//�����ȡ�����ݴ���0��
				if (actualReadLen>0)
				{
					COMM_DATAPACK *pptr=(COMM_DATAPACK*)malloc(sizeof(COMM_DATAPACK));
					pptr->nLen=actualReadLen;
					pptr->pPtr=readBuf;

					ceSeries->m_pPortOwner->PostMessage(WM_COMMDATAEVENT, 
														(WPARAM)ceSeries,(LPARAM)pptr);					
				}
				else
				{
					free(readBuf);
				}
			}
		}
		GetCommModemStatus(ceSeries->m_hComm, &dwCommModemStatus);
	}
	return 0;
}

int m_nLastSpeed;
int m_nCurrentSpeed;
float m_nSumDist;

int m_nLastSpeed_ye;
int m_nCurrentSpeed_ye;
float m_nSumDist_ye;

//����д�̺߳���
DWORD CCESeries::WriteThreadFunc(LPVOID lparam)
{
	CCESeries *ceSeries = (CCESeries*)lparam;
	MSG msg;
	DWORD dwWriteLen = 0;
	BYTE * buf = NULL;
				
	//����յ�д�߳��˳��źţ����˳��߳�
	while (WaitForSingleObject(ceSeries->m_hWriteCloseEvent,1) == WAIT_TIMEOUT)
	{
		//�����׽���߳���Ϣ
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.hwnd != 0 )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
			
			if (msg.message == CM_THREADCOMMWRITE)
			{
				//�򴮿�д
				buf = (BYTE*)msg.lParam;
				dwWriteLen = msg.wParam;

				//�򴮿�д
				::EnterCriticalSection(&ceSeries->m_Csection);
				BOOL bRst = WritePort(ceSeries->m_hComm, buf, dwWriteLen);
#if 0
				//Del by Johnny for test in 2017-08-01 11:10:19
				if(bRst)
				{
					BYTE OpenDoor;
					BYTE AllClr;
					AllClr = buf[66] << 4 >> 4;
					OpenDoor = buf[65] >> 4;

					if(OpenDoor & 0x01 || OpenDoor & 0x02 || OpenDoor & 0x04 || OpenDoor & 0x08 || (AllClr & 0x01 == 0) || (AllClr & 0x02 == 0) )
					{
						m_nSumDist = 0;
						m_nSumDist_ye = 0;
					}

					//[81] [82]
					m_nLastSpeed = m_nCurrentSpeed;
					m_nCurrentSpeed = ((int)(buf[82] << 8) + buf[81]) / 10;

					m_nLastSpeed_ye = m_nCurrentSpeed_ye;
					m_nCurrentSpeed_ye = ((int)(buf[82] << 8) + buf[81]);

					//[83]  life

					SHORT lifeSign = buf[83] + SHORT(buf[84] << 8);
					
					float s1;
					s1 = (m_nLastSpeed_ye + m_nCurrentSpeed_ye) * 1.0 / 2 * 300 /36000 ;
					m_nSumDist_ye += s1;			

					float s0;
					s0 = (m_nLastSpeed + m_nCurrentSpeed) * 1.0  / 3.6 / 2 * 0.3;
					m_nSumDist += s0;

					CString strSum;
					strSum.Format("%.3f km   %.3f km  LastSpeed = %d  CurSpeed = %d  Open = %d  AllClr = %d  life = %d\n", 
									m_nSumDist * 1.0 / 1000, m_nSumDist_ye / 1000, m_nLastSpeed, m_nCurrentSpeed,
									OpenDoor, AllClr, lifeSign);
					TRACE(strSum);
				}
#endif
				::LeaveCriticalSection(&ceSeries->m_Csection);

				//Modify by Johnny in 2017-07-17 18:50:37

#if 0
				//ɾ����̬������ڴ�
				free(buf);
#endif
				Sleep(1);
			}
		}
	}
	return 0;
}


//˽�÷����������򴮿�д���ݣ���д�̵߳���
BOOL CCESeries::WritePort(HANDLE hComm,const BYTE *buf,DWORD bufLen)
{
	DWORD dwNumBytesWritten;
	DWORD dwHaveNumWritten =0 ; //�Ѿ�д�����
	
	ASSERT(hComm != INVALID_HANDLE_VALUE);
	BOOL bResult=FALSE,bExitWhile=FALSE; 
	OVERLAPPED ovn;
	ZeroMemory(&ovn,sizeof(ovn));
	ovn.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	
	do
	{
		BOOL bret = WriteFile ( hComm,								//���ھ�� 
								buf+dwHaveNumWritten,				//��д���ݻ����� 
								bufLen - dwHaveNumWritten,          //��д���ݻ�������С
								&dwNumBytesWritten,					//����ִ�гɹ��󣬷���ʵ���򴮿�д�ĸ���	
								&ovn);
		if(!bret)
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				if(!GetOverlappedResult(hComm,&ovn,&dwNumBytesWritten,TRUE))
					break;
			}else  //�������������������Ƿ��ͳ����˴���ĺ���
				break;
		}
		dwHaveNumWritten = dwHaveNumWritten + dwNumBytesWritten;
		//д�����
		if (dwHaveNumWritten == bufLen)
		{
			bResult=TRUE;
			bExitWhile=TRUE;
		}
	}while (!bExitWhile);
	CloseHandle(ovn.hEvent);		
	return bResult;	
}



//�رն��߳�
void CCESeries::CloseReadThread()
{
	SetEvent(m_hReadCloseEvent);
	//���������¼���Ч��Ч
	SetCommMask(m_hComm, 0);
	//������н�Ҫ��������
    PurgeComm( m_hComm,  PURGE_RXCLEAR );
    //�ȴ�10�룬������߳�û���˳�����ǿ���˳�
    if (WaitForSingleObject(m_hReadThread,1000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hReadThread,10);
		DWORD dwExit;
		
		while(1)
		{
			GetExitCodeThread(m_hReadThread,&dwExit);
			if(dwExit!=STILL_ACTIVE) 
				break;
			Sleep(10);
		}
	}
	m_hReadThread = NULL;
}

//�ر�д�߳�
void CCESeries::CloseWriteThread()
{
	SetEvent(m_hWriteCloseEvent);
	
	//������н�Ҫд������
    PurgeComm( m_hComm,  PURGE_TXCLEAR );
	
    //�ȴ�10�룬������߳�û���˳�����ǿ���˳�
    if (WaitForSingleObject(m_hWriteThread,1000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hWriteThread,10);
		DWORD dwExit;
		
		while(1)
		{
			GetExitCodeThread(m_hWriteThread,&dwExit);
			if(dwExit!=STILL_ACTIVE) 
				break;
			Sleep(10);
		}
	}
	m_hWriteThread = NULL;
}



BOOL CCESeries::GetCommObjState()
{
	if(	m_hComm == INVALID_HANDLE_VALUE)
		return FALSE;
	else 
		return TRUE;
}

