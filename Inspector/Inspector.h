///////////////////////////////////////////////////////////////////////////////
//
//  File:           Inspector.h
//
//  Facility:       ЛайтКом Электро Инспектор
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
* $History: Inspector.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/Inspector/Inspector
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 6:14
 * Updated in $/PPCMercury.root/Inspector
 * Добавлены Source Safe comments
* 
*/

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CInspectorApp:
// See Inspector.cpp for the implementation of this class
//

class CInspectorApp : public CWinApp
{
public:
	CInspectorApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CInspectorApp theApp;
