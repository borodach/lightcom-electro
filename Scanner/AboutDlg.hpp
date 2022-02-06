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
* $History: AboutDlg.hpp $
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
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/PPCMercury
 * Первая версия
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/PPCMercury
* 
*/

#pragma once
#include "afxwin.h"
#include "resourceppc.h"


// CAboutDlg dialog

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);

// Dialog Data
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
public:
    CEdit m_Copyright;
public:
    CStatic m_Logo;
public:
    CButton m_OKBtn;
};