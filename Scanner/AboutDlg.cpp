///////////////////////////////////////////////////////////////////////////////
//
//  File:           AboutDlg.hpp
//
//  Facility:       ЛайтКом Электро Сканер
//
//
//  Abstract:       About dialog
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
* $History: AboutDlg.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:15
 * Updated in $/LightCom/C++/Electro/Scanner/Scanner
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:16
 * Created in $/LightCom/C++/Electro/Scanner/Scanner
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/PPCMercury
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 3.03.07    Time: 23:07
 * Updated in $/PPCMercury.root/PPCMercury
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/PPCMercury
 * Первая версия
* 
*/

#include "stdafx.h"
#include "AboutDlg.hpp"


// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
}

CAboutDlg::~CAboutDlg()
{
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    UpdateData (FALSE);
    CString strText (L"ЛайтКом Электро Сканер 1.1.1\n© ЛайтКом, 2006. Все права зарезервированы.\nтел: 383-214-46-83\nmail: lightscom@yandex.ru\nВнимание! Данная программа защищена законами об авторских правах. Незаконное воспроизведение или распространение данной программы или любой её части влечет гражданскую и уголовную ответственность.");
    static wchar_t newLine [] = {13, 13, 10, 0};
    strText.Replace (L"\n", newLine);
    m_Copyright.SetWindowText (strText);

    return FALSE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ABOUT, m_Copyright);
    DDX_Control(pDX, IDC_LOGO_BOX, m_Logo);
    DDX_Control(pDX, IDC_BUTTON1, m_OKBtn);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAboutDlg message handlers

void CAboutDlg::OnBnClickedButton1()
{
    EndDialog (1);
}

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
