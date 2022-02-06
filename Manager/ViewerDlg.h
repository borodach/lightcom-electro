///////////////////////////////////////////////////////////////////////////////
//
//  File:           ViewerDlg.cpp
//
//  Facility:       Desktop утилита для работы с результатами сканирования 
//                  счетчиков.
//
//
//  Abstract:       Диалог просмотра лога сканирования счетчика.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  11-09-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ViewerDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
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
    // Конструктор.
    //

    CViewerDlg (CWnd* pParent = NULL, 
                const wchar_t *wcsCaption = L"Журнал сканирования счетчика");

    //
    // Деструктор.
    //

    virtual ~CViewerDlg ();

    //
    // Установка текста лога.
    //

    void SetLogMsg (const CString &strMsg)
    {
        m_strLog = strMsg;
    }

    //
    // Идентификатор диалога.
    //

    enum { IDD = IDD_VIEWER_DLG};

    //
    // Обработчик изменения размера диалога.
    //

    afx_msg void OnSize (UINT nType, int cx, int cy);

    //
    // Инициализация диалога.
    //

    virtual BOOL OnInitDialog ();

    //
    // Обмен данными с диалогом.
    //

    virtual void DoDataExchange (CDataExchange* pDX);

    //
    // Карта сообщений.
    //

    DECLARE_MESSAGE_MAP()

protected:
    CString m_strCaption;
    CString m_strLog;
    CRichEditCtrl m_LogView;    
};
