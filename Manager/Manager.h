///////////////////////////////////////////////////////////////////////////////
//
//  File:           Manager.h
//
//  Facility:       ������� ������� ��������
//
//
//  Abstract:       ������� ������ ���������� 
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  23-11-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Manager.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CManagerApp:
// See Manager.cpp for the implementation of this class
//

class CManagerApp : public CWinApp
{
public:
	CManagerApp();
    ~CManagerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
    HICON m_hIcon;
};

extern CManagerApp theApp;