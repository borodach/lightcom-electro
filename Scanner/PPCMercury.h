///////////////////////////////////////////////////////////////////////////////
//
//  File:           PPCMercury.h
//
//  Facility:       ДайтКом Электро Сканер
//
//
//  Abstract:       Главный модуль
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
* $History: PPCMercury.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:16
 * Created in $/LightCom/C++/Electro/Scanner/Scanner
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/PPCMercury
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/PPCMercury
* 
*/

// PPCMercury.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CPPCMercuryApp:
// See PPCMercury.cpp for the implementation of this class
//

class CPPCMercuryApp : public CWinApp
{
public:
	CPPCMercuryApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPPCMercuryApp theApp;
