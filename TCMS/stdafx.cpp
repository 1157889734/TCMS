// stdafx.cpp : 只包括标准包含文件的源文件
// TCMS.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


BYTE GetCheckSum( BYTE *pData,int iLen )
{
	BYTE bAdd  = 0;//累加和
	for (int i = 0; i < iLen; i++)//累加和
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
