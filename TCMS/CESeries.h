// CESeries.h: interface for the CCESeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CESERIES_H__4040241A_FA58_4655_88BA_8D8DF018446D__INCLUDED_)
#define AFX_CESERIES_H__4040241A_FA58_4655_88BA_8D8DF018446D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//CE����ͨѶ��
class CCESeries
{

public:
	CCESeries();
	virtual ~CCESeries();

public:
	BOOL GetCommObjState();

	//�򿪴���
	BOOL OpenPort(CWnd* pPortOwner,			/*ʹ�ô����࣬������*/
				  UINT portNo	= 2,		/*���ں�*/
				  UINT baud		= 9600,		/*������*/
				  UINT parity	= NOPARITY, /*��żУ��*/
				  UINT databits	= 8,		/*����λ*/
				  UINT stopbits	= 0         /*ֹͣλ*/
				  );
	//�رմ���
	void ClosePort();

	//���ô��ڶ�ȡ��д�볬ʱ
	BOOL SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts);

	//�򴮿�д������,���ⲿ����
	BOOL WritePort(const BYTE *buf,DWORD bufLen);

	CRITICAL_SECTION m_Csection; 
private:
    //���ڶ��̺߳���
    static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);

	//����д�̺߳���
    static DWORD WINAPI WriteThreadFunc(LPVOID lparam);

	//ʵ���򴮿�д������
	static BOOL WritePort(HANDLE hComm,const BYTE *buf,DWORD bufLen);

	//�رն��߳�
	void CloseReadThread();
	//�ر�д�߳�
	void CloseWriteThread();
private:
	CWnd* m_pPortOwner;

    //�Ѵ򿪵Ĵ��ھ��
	HANDLE	m_hComm;

	CString idstr;
	//��д�߳̾��
	HANDLE m_hReadThread;
	HANDLE m_hWriteThread;
	
	//��д�߳�ID��ʶ
	DWORD m_dwReadThreadID;
	DWORD m_dwWriteThreadID;
	
	//���߳��˳��¼�
	HANDLE m_hReadCloseEvent;
	//д�߳��˳��¼�
	HANDLE m_hWriteCloseEvent;
};


#endif // !defined(AFX_CESERIES_H__4040241A_FA58_4655_88BA_8D8DF018446D__INCLUDED_)
