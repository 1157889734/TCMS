// libBhPanelTms.h : libBhPanelTms DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ClibBhPanelTmsApp
// �йش���ʵ�ֵ���Ϣ������� libBhPanelTms.cpp
//

class ClibBhPanelTmsApp : public CWinApp
{
public:
	ClibBhPanelTmsApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
