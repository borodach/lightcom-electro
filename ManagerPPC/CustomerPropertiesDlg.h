///////////////////////////////////////////////////////////////////////////////
//
//  File:           CustomerPropertiesDlg.h
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//                  
//
//
//  Abstract:       ������ ������� �����������.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  11-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CustomerPropertiesDlg.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/ManagerPPC
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 21.01.07   Time: 15:54
 * Updated in $/PPCMercury.root/ManagerPPC
 * ����� ������
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 21.01.07   Time: 13:41
 * Created in $/PPCMercury.root/ManagerPPC
 * ������ ������
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#pragma once
#include "afxwin.h"
#include "Customer.hpp"
#include "afxcmn.h"

//
// CCustomerPropertiesDlg dialog
//

class CCustomerPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomerPropertiesDlg)

public:

    //
    // �����������.
    //

	CCustomerPropertiesDlg (CWnd* pParent = NULL, 
                            const wchar_t *wcsCaption = L"�������� �����������");   

    //
    // ����������.
    //

	virtual ~CCustomerPropertiesDlg ();

    //
    // ID �������.
    //

	enum { IDD = IDD_CUSTOMER_PROP };

    //
    // ������ ������� � �������������� ������� ������ Customer
    //

    const Customer& GetCustomer () const {return m_Customer;}
    Customer& GetCustomer () {return m_Customer;}
    void SetCustomer (const Customer& customer) 
    {
        m_Customer = customer;
    }
    
    //
    // ���������, ������� �� ��������� ������������� ������ � ����������.
    //

    BOOL CheckData ();

    //
    // ������������� �������.
    //

    virtual BOOL OnInitDialog ();

    //
    // ����� ������� � ��������.
    //

	virtual void DoDataExchange (CDataExchange* pDX);

    //
    // ���������� ����� �� ������ OK.
    //

    afx_msg void OnBnClickedOk ();

    //
    // ���������� ��������� ������� ����.
    //

    afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

protected:

    //
    // �������� �����������.
    //

    CString m_CustomerName;

    //
    // ������������� �����������.
    //

    Customer m_Customer;

    //
    // ���� ��� ����� ������ ��������.
    //

    CSpinButtonCtrl m_PowerLimitSpin;

    //
    // ����� �������� � ���.
    //

    UINT m_PowerLimit;

    //
    // ��������� �������.
    //

    CString m_strCaption;    
};
