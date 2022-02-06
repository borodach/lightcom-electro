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
* $History: ViewerDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#pragma once
#include "afxcmn.h"

//
// CViewerDlg dialog
//

class CViewerDlg : public CDialog
{
    DECLARE_DYNAMIC(CViewerDlg)
public:

    //
    // �����������.
    //

    CViewerDlg (CWnd* pParent = NULL, 
                const wchar_t *wcsCaption = L"������ ������������ ��������");

    //
    // ����������.
    //

    virtual ~CViewerDlg ();

    //
    // ��������� ������ ����.
    //

    void SetLogMsg (const CString &strMsg)
    {
        m_strLog = strMsg;
    }

    //
    // ������������� �������.
    //

    enum { IDD = IDD_VIEWER_DLG};

    //
    // ���������� ��������� ������� �������.
    //

    afx_msg void OnSize (UINT nType, int cx, int cy);

    //
    // ������������� �������.
    //

    virtual BOOL OnInitDialog ();

    //
    // ����� ������� � ��������.
    //

    virtual void DoDataExchange (CDataExchange* pDX);

    //
    // ����� ���������.
    //

    DECLARE_MESSAGE_MAP()

protected:
    CString m_strCaption;
    CString m_strLog;
    CRichEditCtrl m_LogView;    
};
