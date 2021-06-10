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

//定义向写线程发送的消息常量
const int CM_THREADCOMMWRITE = WM_USER+110;
const int CM_THREADCOMMGETNEWCHAR = WM_USER+111;
#define DATAHEAD 0x7E
#define DATATAIL 0x0D

//类CCESeries构造函数
CCESeries::CCESeries()
{
	idstr.Empty();
	m_hComm = INVALID_HANDLE_VALUE;
	m_hReadCloseEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hWriteCloseEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	::InitializeCriticalSection(&m_Csection);	
}

//类CCESeries析构函数
CCESeries::~CCESeries()
{
	ClosePort();
	CloseHandle(m_hReadCloseEvent);
	CloseHandle(m_hWriteCloseEvent);
	::DeleteCriticalSection(&m_Csection);
}

/*
*函数介绍：打开串口
*入口参数：pPortOwner	:使用此串口类的窗体句柄
		   portNo		:串口号
		   baud			:波特率
		   parity		:奇偶校验
		   databits		:数据位
		   stopbits		:停止位
*出口参数：(无)
*返回值：TRUE:成功打开串口;FALSE:打开串口失败
*/
BOOL CCESeries::OpenPort(CWnd* pPortOwner,			/*使用串口类，窗体句柄*/
						 UINT portNo	,			/*串口号*/
						 UINT baud		,			/*波特率*/
						 UINT parity	,			/*奇偶校验*/
						 UINT databits	,			/*数据位*/
						 UINT stopbits			   /*停止位*/
						 )
{
	DCB commParam;
	TCHAR szPort[15];	
	
	// 已经打开的话，直接返回
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}
	ASSERT(pPortOwner != NULL);
	
	//设置串口名
// 	if (portNo >= 10)
// 	{
		wsprintf(szPort, _T("\\\\.\\COM%d"), portNo);
// 	}
// 	else
// 	{
// 		wsprintf(szPort, _T("COM%d"), portNo);
// 	}
	
	//打开串口
	m_hComm = CreateFile(
		szPort,
		GENERIC_READ | GENERIC_WRITE,	//允许读和写
		0,								//独占方式（共享模式）
		NULL,
		OPEN_EXISTING,					//打开而不是创建（创建方式）
		FILE_FLAG_OVERLAPPED,
		NULL 
		);
	idstr.Format(_T("ID:Com%d"),portNo);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// 无效句柄,返回。		
		return FALSE;
	}
	
	// 得到打开串口的当前属性参数，修改后再重新设置串口。
	// 设置串口的超时特性为立即返回。
	
	if (!GetCommState(m_hComm,&commParam))
	{		
		//关闭串口
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	GetCommState(m_hComm,&commParam);
	commParam.BaudRate = baud;					// 设置波特率 
	commParam.fBinary = TRUE;					// 设置二进制模式，此处必须设置TRUE
	commParam.fParity = FALSE;//TRUE;					// 支持奇偶校验 
	commParam.ByteSize = databits;				// 数据位,范围:4-8 
	commParam.Parity = parity;					// 校验模式
	commParam.StopBits = stopbits;				// 停止位 
	
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
	// 485改动
	// RTS flow control 
	commParam.fAbortOnError = FALSE;			// 当串口发生错误，并不终止串口读写
	
	if (!SetCommState(m_hComm, &commParam))
	{
		//关闭串口
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	
    //设置串口读写超时时间
	COMMTIMEOUTS CommTimeOuts;
	//得到超时参数
	GetCommTimeouts (m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;  
	CommTimeOuts.WriteTotalTimeoutConstant = 0;  
	
	if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
	{
		//关闭串口
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	
	m_pPortOwner = pPortOwner;	
	
	//指定端口监测的事件集, 留到线程中指定
	SetCommMask (m_hComm, EV_RXCHAR);
	
	//分配设备缓冲区
	SetupComm(m_hComm,512,512);
	
	//初始化缓冲区中的信息
	PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);

	EscapeCommFunction(m_hComm,CLRDTR);
	EscapeCommFunction(m_hComm,CLRRTS);
	
	ResetEvent(m_hReadCloseEvent);
	ResetEvent(m_hWriteCloseEvent);
	//创建读串口线程
	m_hReadThread = CreateThread(NULL,0,ReadThreadFunc,this,CREATE_SUSPENDED,&m_dwReadThreadID);
	//创建写串口线程
	m_hWriteThread = CreateThread(NULL,0,WriteThreadFunc,this,CREATE_SUSPENDED,&m_dwWriteThreadID);

	ResumeThread(m_hWriteThread);
	ResumeThread(m_hReadThread);

	return TRUE;
}

/*
*函数介绍：关闭串口
*入口参数：(无)
*出口参数：(无)
*返回值：  (无)
*/
void CCESeries::ClosePort()
{	
	//表示串口还没有打开
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	
	//关闭读线程
	CloseReadThread();
	//关闭写线程
	CloseWriteThread();
	
	//关闭串口
	if (CloseHandle (m_hComm))
	{
		m_hComm = INVALID_HANDLE_VALUE;
		return ;
	}
}

/*
*函数介绍：向串口发送数据
*入口参数：buf		: 将要往串口写入的数据的缓冲区
		   bufLen	: 将要往串口写入的数据的缓冲区长度
*出口参数：(无)
*返回值：TRUE:表示成功地将要发送的数据传递到写线程消息队列。
		 FALSE:表示将要发送的数据传递到写线程消息队列失败。
		 注视：此处的TRUE,不直接代表数据一定成功写入到串口了。
*/
BOOL CCESeries::WritePort(const BYTE *buf,DWORD bufLen)
{
	//将要发送的数据传递到写线程消息队列
	if (PostThreadMessage(m_dwWriteThreadID, CM_THREADCOMMWRITE,
		WPARAM(bufLen), LPARAM(buf)))
	{
		return TRUE;
	}
	return FALSE;
}

/*
*函数介绍：设置串口读取、写入超时
*入口参数：CommTimeOuts : 指向COMMTIMEOUTS结构
*出口参数：(无)
*返回值：TRUE:设置成功;FALSE:设置失败
*/
BOOL CCESeries::SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	return SetCommTimeouts(m_hComm,&CommTimeOuts);
}


//串口读线程函数
DWORD CCESeries::ReadThreadFunc(LPVOID lparam)
{
	CCESeries *ceSeries = (CCESeries*)lparam;
	
	DWORD	evtMask;
	BYTE * readBuf = NULL;//读取的字节
	DWORD actualReadLen=0;//实际读取的字节数
	DWORD willReadLen;
	DWORD dwCommModemStatus;
	
	DWORD dwReadErrors;
	COMSTAT	cmState;

	OVERLAPPED ovn;
	HANDLE hevent;
	hevent=CreateEvent(NULL,TRUE,FALSE,NULL);
	ZeroMemory(&ovn,sizeof(ovn));
	ovn.hEvent=hevent;
	
	// 清空缓冲，并检查串口是否打开。
	ASSERT(ceSeries->m_hComm !=INVALID_HANDLE_VALUE); 
	
	
	//清空串口
	PurgeComm(ceSeries->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );
	
	SetCommMask(ceSeries->m_hComm, EV_RXCHAR);
	
	//如果收到读线程退出信号，则退出线程
	while (WaitForSingleObject(ceSeries->m_hReadCloseEvent,1) == WAIT_TIMEOUT)
	{   	
		SetCommMask (ceSeries->m_hComm, EV_RXCHAR);

		if (WaitCommEvent(ceSeries->m_hComm,&evtMask,0))
		{		
			ClearCommError(ceSeries->m_hComm,&dwReadErrors,&cmState);
			//表示串口收到字符		
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
				
				//如果读取的数据大于0，
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

//串口写线程函数
DWORD CCESeries::WriteThreadFunc(LPVOID lparam)
{
	CCESeries *ceSeries = (CCESeries*)lparam;
	MSG msg;
	DWORD dwWriteLen = 0;
	BYTE * buf = NULL;
				
	//如果收到写线程退出信号，则退出线程
	while (WaitForSingleObject(ceSeries->m_hWriteCloseEvent,1) == WAIT_TIMEOUT)
	{
		//如果捕捉到线程消息
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
				//向串口写
				buf = (BYTE*)msg.lParam;
				dwWriteLen = msg.wParam;

				//向串口写
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
				//删除动态分配的内存
				free(buf);
#endif
				Sleep(1);
			}
		}
	}
	return 0;
}


//私用方法，用于向串口写数据，被写线程调用
BOOL CCESeries::WritePort(HANDLE hComm,const BYTE *buf,DWORD bufLen)
{
	DWORD dwNumBytesWritten;
	DWORD dwHaveNumWritten =0 ; //已经写入多少
	
	ASSERT(hComm != INVALID_HANDLE_VALUE);
	BOOL bResult=FALSE,bExitWhile=FALSE; 
	OVERLAPPED ovn;
	ZeroMemory(&ovn,sizeof(ovn));
	ovn.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	
	do
	{
		BOOL bret = WriteFile ( hComm,								//串口句柄 
								buf+dwHaveNumWritten,				//被写数据缓冲区 
								bufLen - dwHaveNumWritten,          //被写数据缓冲区大小
								&dwNumBytesWritten,					//函数执行成功后，返回实际向串口写的个数	
								&ovn);
		if(!bret)
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				if(!GetOverlappedResult(hComm,&ovn,&dwNumBytesWritten,TRUE))
					break;
			}else  //这里上下两处跳出，是发送出现了错误的含义
				break;
		}
		dwHaveNumWritten = dwHaveNumWritten + dwNumBytesWritten;
		//写入完成
		if (dwHaveNumWritten == bufLen)
		{
			bResult=TRUE;
			bExitWhile=TRUE;
		}
	}while (!bExitWhile);
	CloseHandle(ovn.hEvent);		
	return bResult;	
}



//关闭读线程
void CCESeries::CloseReadThread()
{
	SetEvent(m_hReadCloseEvent);
	//设置所有事件无效无效
	SetCommMask(m_hComm, 0);
	//清空所有将要读的数据
    PurgeComm( m_hComm,  PURGE_RXCLEAR );
    //等待10秒，如果读线程没有退出，则强制退出
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

//关闭写线程
void CCESeries::CloseWriteThread()
{
	SetEvent(m_hWriteCloseEvent);
	
	//清空所有将要写的数据
    PurgeComm( m_hComm,  PURGE_TXCLEAR );
	
    //等待10秒，如果读线程没有退出，则强制退出
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

