///////////////////////////////////////////////////////////////////////////////
//
//  File:           InspectorDlg.h
//
//  Facility:       ������� ������� ���������
//
//
//  Abstract:       ������� ������ ����������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  23-11-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: InspectorDlg.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/Inspector/Inspector
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 6:14
 * Updated in $/PPCMercury.root/Inspector
 * ��������� Source Safe comments
* 
*/

#pragma once

#include "DataManager.hpp"

#include "afxwin.h"

// CInspectorDlg dialog
class CInspectorDlg : public CDialog
{
// Construction
public:
	CInspectorDlg(CWnd* pParent = NULL);	// standard constructor
	enum { IDD = IDD_INSPECTOR_DIALOG };
    enum CommandType 
    { 
        ctInvalid = 0,
        ctReload,
        ctSummary,
        ctEnergy,
        ctPower,
        ctScan,
        ctAbout,
        ctMaxCommand
    };

protected:

    //
    // ���������� ������ ������������
    //

    void ReloadCustomerList ();

    //
    // ���������� ��������� �������� �����������
    //
    
    void PocessCustomerChange ();

    //
    // ���������� ������
    //

    void UpdateReport (const wchar_t *wcsMessage);

protected:
	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV support
    afx_msg void OnCustomerChanged ();
    virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
    
protected:
	HICON m_hIcon;

	
    int m_nLastCommandIdx;
    CComboBox m_CustomersCombo;
    CEdit m_OutputBox;
    DataManager m_DataManager;
    CComboBox m_CommandsBox;
public:
    afx_msg void OnCommandChange();
};
