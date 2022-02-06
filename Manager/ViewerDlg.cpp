///////////////////////////////////////////////////////////////////////////////
//
//  File:           ViewerDlg.cpp
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//
//
//  Abstract:       ������ ��������� ���� ������������ ��������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  11-09-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ViewerDlg.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#include "stdafx.h"
#include "Manager.h"
#include "ViewerDlg.h"

//
// CViewerDlg dialog
//

IMPLEMENT_DYNAMIC(CViewerDlg, CDialog)

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CViewerDlg
//
//  Description:
//
//  �����������.
//
///////////////////////////////////////////////////////////////////////////////

CViewerDlg::CViewerDlg (CWnd* pParent /*=NULL*/,
                        const wchar_t *wcsCaption/* = L"������ ������������ ��������"*/)
    : CDialog(CViewerDlg::IDD, pParent),
    m_strCaption (wcsCaption)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CViewerDlg
//
//  Description:
//
//  ����������.
//
///////////////////////////////////////////////////////////////////////////////

CViewerDlg::~CViewerDlg ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DoDataExchange
//
//  Description:
//
//  ����� ������� � ��������.
//
///////////////////////////////////////////////////////////////////////////////

void CViewerDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
    DDX_Control (pDX, IDC_LOG_VIEW, m_LogView);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnInitDialog
//
//  Description:
//
//  ������������� �������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CViewerDlg::OnInitDialog ()
{
    CDialog::OnInitDialog ();

    SetWindowText (m_strCaption);
    //static wchar_t newLine [] = {13, 13, 10, 0};
    //m_strLog.Replace (L"\n", newLine);
    m_LogView.LimitText (1024 * 1024 * 8);
    m_LogView.SetTextMode (TM_PLAINTEXT);
    m_LogView.SetWindowText (m_strLog);
    m_LogView.SetFocus ();
    m_LogView.SetSel (0, 0);
    m_strLog = L"";

    HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon (hIcon, TRUE);
    SetIcon (hIcon, FALSE);
    RECT rect;
    GetClientRect (&rect);
    OnSize (SIZE_RESTORED, rect.right - rect.left, rect.bottom - rect.top);
 
    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSize
//
//  Description:
//
//  ���������� ��������� ������� �������.
//
///////////////////////////////////////////////////////////////////////////////

void CViewerDlg::OnSize (UINT nType, int cx, int cy)
{
    //CDialog::OnSize (nType, cx, cy);
    int d = 2;
    int width (cx);
    int height (cy);
    if (cx > 2 * d && cy > 2 * d)
    {
        width -= 2 * d;
        height -= 2 * d;
    }

    if (IsWindow (m_LogView.m_hWnd))
    {
        m_LogView.MoveWindow (d, d, width, height);
    }
}
