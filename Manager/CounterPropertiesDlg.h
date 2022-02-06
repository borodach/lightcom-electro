///////////////////////////////////////////////////////////////////////////////
//
//  File:           CounterPropertiesDlg.h
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//                  
//
//
//  Abstract:       ������ ������� ��������.
//
//
//  Environment:    MSVC 2005
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CounterPropertiesDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#pragma once
#include "afxcmn.h"
#include "Customer.hpp"
#include "afxwin.h"

//
// CCounterPropertiesDlg dialog
//

class CCounterPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CCounterPropertiesDlg)
public:

    //
    // �����������.
    //
    
    CCounterPropertiesDlg(CWnd* pParent = NULL,
                          const wchar_t *wcsCaption = L"�������� ��������");

    //
    // ����������.
    //

	virtual ~CCounterPropertiesDlg();

    //
    // ������������� �������.
    //

	enum { IDD = IDD_COUNTER_PROP };

    //
    // ������ ������� � �������������� ��������.
    //

    const Customer::Counter& GetCounter () const {return m_Counter;}
    Customer::Counter& GetCounter () {return m_Counter;}
    void SetCounter (const Customer::Counter& �ounter) 
    {
        m_Counter = �ounter;
    }

    //
    // ���������, ������� �� ��������� ������������� ������.
    //

    BOOL CheckData ();

    //
    // ����� ������� � ��������.
    //

	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV support

    //
    // ������������� �������.
    //

    virtual BOOL OnInitDialog ();

    //
    // ���������� ����� �� ������ OK.
    //

    afx_msg void OnBnClickedOk ();

    //
    // ����� ���������.
    //

	DECLARE_MESSAGE_MAP()
    
protected:
    
    //
    // ���� ��� ���.
    //

    CSpinButtonCtrl m_KtnSpin;

    //
    // ���� ��� ���.
    //

    CSpinButtonCtrl m_KttSpin;

    //
    // �������� �����.
    //

    UINT m_nSerialNumber;

    //
    // ����������� ������������� ����������.
    //

    UINT m_nKtn;

    //
    // ����������� ������������� ����.
    //

    UINT m_nKtt;

    //
    // ��� �������.
    //

    CComboBox m_CounterType;

    //
    // ������������� �������.
    //

    Customer::Counter m_Counter;    

    //
    // ��������� �������.
    //

    CString m_strCaption;
};
