///////////////////////////////////////////////////////////////////////////////
//
//  File:           ManagerPPC.h
//
//  Facility:       ЛайтКом Энерго Менеджер
//                  
//
//
//  Abstract:       Класс приложения
//
//
//  Environment:    MSVC 2005
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  22-Jan-2007
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ManagerPPC.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 6  *****************
 * User: Sergey       Date: 3.03.07    Time: 23:04
 * Updated in $/PPCMercury.root/ManagerPPC
 * 
 * *****************  Version 5  *****************
 * User: Sergey       Date: 30.01.07   Time: 21:17
 * Updated in $/PPCMercury.root/ManagerPPC
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 28.01.07   Time: 23:11
 * Updated in $/PPCMercury.root/ManagerPPC
 * Внесены изменения, в соотетствии с замечаниями ЭнергоСбыта
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/ManagerPPC
*/

#include "stdafx.h"
#include "ManagerPPC.h"
#include "MainFrm.h"
#include "RegKeys.h"
#include "HwIdProtection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
// Карта сообщений класса CManagerPPCApp
//

BEGIN_MESSAGE_MAP(CManagerPPCApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CManagerPPCApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CManagerPPCApp
//
//  Description:
//
//  Конструктор.
//
///////////////////////////////////////////////////////////////////////////////

CManagerPPCApp::CManagerPPCApp (): CWinApp ()
{
}

//
// Экземпляр приложения.
//

CManagerPPCApp theApp;

//
// Признак пробной версии
//

bool bEvaluation = true;

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       InitInstance
//
//  Description:
//
//  Инициализация экземпляра приложения.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CManagerPPCApp::InitInstance ()
{
    if (! Protect (ELECTRO_MANAGER_KEY,
        L"\\Keys\\ManagerKey.txt", 
        L"\\HardwareId", 
        L"\\HwId.txt"))
    {
        //EndDialog (0);
        //return FALSE;
        bEvaluation = true;
    }
    else
    {
        bEvaluation = false;
    }

    //
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    //

    SHInitExtraControls();

	AfxEnableControlContainer();

    //
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
    //
		
    CMainFrame* pFrame = new CMainFrame;
    if (!pFrame)
        return FALSE;
    m_pMainWnd = pFrame;

    pFrame->LoadFrame (IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
    HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    pFrame->SetIcon (m_hIcon, TRUE);
    pFrame->SetIcon (m_hIcon, FALSE);

    //    
    // The one and only window has been initialized, so show and update it
    //

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


//
// CAboutDlg dialog used for App About
//

class CAboutDlg : public CDialog
{
    DECLARE_DYNAMIC(CAboutDlg)

public:
    
    //
    // Standard constructor
    //

    CAboutDlg (CWnd* pParent = NULL);

    //
    // Деструктор
    //

    virtual ~CAboutDlg ();

    //
    // Инициализация диалога
    //

    virtual BOOL OnInitDialog();

    //
    // Идентификатор диалога
    //

    enum { IDD = IDD_ABOUT };

protected:
    
    //
    // DDX/DDV support
    //

    virtual void DoDataExchange(CDataExchange* pDX);

    //
    // Обработчик клика на кнопке закрытия диалога.
    //

    afx_msg void OnCloseClicked ();

    //
    //  Обработчик изменения размера окна.
    //

    afx_msg void OnSize(UINT nType, int cx, int cy);

    //
    // Карта сообщений
    //

    DECLARE_MESSAGE_MAP()
protected:
    CEdit m_Copyright;  // Editbox с сведениями о владельцах программы.
    CStatic m_Logo;     // Логотип программы.
    CButton m_OKBtn;    // Кнопка закрытия диалога
};

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CAboutDlg
//
//  Description:
//
//  Конструктор.
//
///////////////////////////////////////////////////////////////////////////////

CAboutDlg::CAboutDlg (CWnd* pParent /*=NULL*/) : CDialog(CAboutDlg::IDD, pParent)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CAboutDlg
//
//  Description:
//
//  Деструктор.
//
///////////////////////////////////////////////////////////////////////////////

CAboutDlg::~CAboutDlg ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnInitDialog
//
//  Description:
//
//  Инициализация диалога.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CAboutDlg::OnInitDialog ()
{
    CDialog::OnInitDialog();
    UpdateData (FALSE);
    CString strText (L"ЛайтКом Электро Менеджер 1.0\n© ЛайтКом, 2007. Все права зарезервированы.\nтел: 383-214-46-83\nmail: lightscom@yandex.ru\nВнимание! Данная программа защищена законами об авторских правах. Незаконное воспроизведение или распространение данной программы или любой её части влечет гражданскую и уголовную ответственность.");
    static wchar_t newLine [] = {13, 13, 10, 0};
    strText.Replace (L"\n", newLine);
    m_Copyright.SetWindowText (strText);

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DoDataExchange
//
//  Description:
//
//  DDX/DDV support
//
///////////////////////////////////////////////////////////////////////////////

void CAboutDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ABOUT, m_Copyright);
    DDX_Control(pDX, IDC_LOGO_BOX, m_Logo);    
    DDX_Control(pDX, IDC_BUTTON1, m_OKBtn);
}

//
// Карта сообщений
//

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnCloseClicked)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCloseClicked
//
//  Description:
//
//  Обработчик клика на кнопке закрытия диалога.
//
///////////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnCloseClicked ()
{
    EndDialog (1);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSize
//
//  Description:
//
//  Обработчик изменения размера окна.
//
///////////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
    int nOffset = 1;

    RECT logoRect;
    m_Logo.GetWindowRect (&logoRect);
    ScreenToClient (&logoRect);
    int nWidth = (logoRect.right - logoRect.left);
    int nHeight = (logoRect.bottom - logoRect.top);

    logoRect.bottom = cy - nOffset;
    logoRect.top = logoRect.bottom - nHeight;
    logoRect.left = nOffset;
    logoRect.right = logoRect.left + nWidth;
    m_Logo.MoveWindow (&logoRect);

    RECT copyrightRect;
    m_Copyright.GetWindowRect (&copyrightRect);
    ScreenToClient (&copyrightRect);
    copyrightRect.top = nOffset;
    copyrightRect.bottom = logoRect.top - nOffset;
    copyrightRect.left = nOffset;
    copyrightRect.right = cx - nOffset;
    m_Copyright.MoveWindow (&copyrightRect);

    RECT okRect;
    m_OKBtn.GetWindowRect (&okRect);
    ScreenToClient (&okRect);
    nWidth = (okRect.right - okRect.left);
    nHeight = (okRect.bottom - okRect.top);

    okRect.bottom = cy - nOffset;
    okRect.top = okRect.bottom - nHeight;

    okRect.right = cx - nOffset;
    okRect.left = okRect.right - nWidth;

    m_OKBtn.MoveWindow (&okRect);
} 

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnAppAbout
//
//  Description:
//
//  App command to run the dialog.
//
///////////////////////////////////////////////////////////////////////////////

// 
void CManagerPPCApp::OnAppAbout ()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal ();
}
