
// VideoPlay.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoPlayApp:
// �йش����ʵ�֣������ VideoPlay.cpp
//

class CVideoPlayApp : public CWinApp
{
public:
	CVideoPlayApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoPlayApp theApp;