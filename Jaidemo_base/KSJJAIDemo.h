
// KSJJAIDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKSJJAIDemoApp: 
// �йش����ʵ�֣������ KSJJAIDemo.cpp
//

class CKSJJAIDemoApp : public CWinApp
{
public:
	CKSJJAIDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKSJJAIDemoApp theApp;