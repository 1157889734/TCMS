// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// TCMS.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


BYTE GetCheckSum( BYTE *pData,int iLen )
{
	BYTE bAdd  = 0;//�ۼӺ�
	for (int i = 0; i < iLen; i++)//�ۼӺ�
	{
		bAdd += pData[i];
	}
	return bAdd;

	WORD wSum = 0x00;
	for (int i=0;i<iLen;i++)
	{
		wSum += *(pData+i);
	}
	wSum = 0x00FF & wSum;
	return (BYTE)wSum;
}
