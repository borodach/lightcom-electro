///////////////////////////////////////////////////////////////////////////////
//
//  File:           Manager.h
//
//  Facility:       ЛайтКом Электро Менеджер
//
//
//  Abstract:       Главный модуль приложения 
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  23-11-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Manager.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
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