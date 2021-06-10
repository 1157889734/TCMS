// CESeries.h: interface for the CCESeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CESERIES_H__4040241A_FA58_4655_88BA_8D8DF018446D__INCLUDED_)
#define AFX_CESERIES_H__4040241A_FA58_4655_88BA_8D8DF018446D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//CE串口通讯类
class CCESeries
{

public:
	CCESeries();
	virtual ~CCESeries();

public:
	BOOL GetCommObjState();

	//打开串口
	BOOL OpenPort(CWnd* pPortOwner,			/*使用串口类，窗体句柄*/
				  UINT portNo	= 2,		/*串口号*/
				  UINT baud		= 9600,		/*波特率*/
				  UINT parity	= NOPARITY, /*奇偶校验*/
				  UINT databits	= 8,		/*数据位*/
				  UINT stopbits	= 0         /*停止位*/
				  );
	//关闭串口
	void ClosePort();

	//设置串口读取、写入超时
	BOOL SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts);

	//向串口写入数据,供外部调用
	BOOL WritePort(const BYTE *buf,DWORD bufLen);

	CRITICAL_SECTION m_Csection; 
private:
    //串口读线程函数
    static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);

	//串口写线程函数
    static DWORD WINAPI WriteThreadFunc(LPVOID lparam);

	//实际向串口写入数据
	static BOOL WritePort(HANDLE hComm,const BYTE *buf,DWORD bufLen);

	//关闭读线程
	void CloseReadThread();
	//关闭写线程
	void CloseWriteThread();
private:
	CWnd* m_pPortOwner;

    //已打开的串口句柄
	HANDLE	m_hComm;

	CString idstr;
	//读写线程句柄
	HANDLE m_hReadThread;
	HANDLE m_hWriteThread;
	
	//读写线程ID标识
	DWORD m_dwReadThreadID;
	DWORD m_dwWriteThreadID;
	
	//读线程退出事件
	HANDLE m_hReadCloseEvent;
	//写线程退出事件
	HANDLE m_hWriteCloseEvent;
};


#endif // !defined(AFX_CESERIES_H__4040241A_FA58_4655_88BA_8D8DF018446D__INCLUDED_)
