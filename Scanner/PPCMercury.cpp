///////////////////////////////////////////////////////////////////////////////
//
//  File:           PPCMercury.cpp
//
//  Facility:       ЛайтКом Электро сканер
//
//
//  Abstract:       Главный модуль прложения
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
* $History: PPCMercury.cpp $
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
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/PPCMercury
 * Первая версия
* 
*/

#include "stdafx.h"
#include "PPCMercury.h"
#include "PPCMercuryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPPCMercuryApp

BEGIN_MESSAGE_MAP(CPPCMercuryApp, CWinApp)
END_MESSAGE_MAP()


// CPPCMercuryApp construction
CPPCMercuryApp::CPPCMercuryApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPPCMercuryApp object
CPPCMercuryApp theApp;

// CPPCMercuryApp initialization

BOOL CPPCMercuryApp::InitInstance()
{
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CPPCMercuryDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
