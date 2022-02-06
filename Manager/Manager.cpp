///////////////////////////////////////////////////////////////////////////////
//
//  File:           Manager.cpp
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
* $History: Manager.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/Manager
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
* 
*/

// Manager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Manager.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CManagerApp

BEGIN_MESSAGE_MAP(CManagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CManagerApp::OnAppAbout)
END_MESSAGE_MAP()

bool bEvaluation = true;

// CManagerApp construction

CManagerApp::CManagerApp()
{
}

CManagerApp::~CManagerApp()
{
}


// The one and only CManagerApp object

CManagerApp theApp;


// CManagerApp initialization

BOOL CManagerApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame (IDR_MAINFRAME,	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    pFrame->SetIcon (m_hIcon, TRUE);
    pFrame->SetIcon (m_hIcon, FALSE);

    AfxInitRichEdit2 ();

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CManagerApp message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


