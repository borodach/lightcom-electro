///////////////////////////////////////////////////////////////////////////////
//
//  File:           CustomerPropertiesDlg.cpp
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
* $History: CustomerPropertiesDlg.cpp $
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
 * User: Sergey       Date: 21.01.07   Time: 13:39
 * Created in $/PPCMercury.root/ManagerPPC
 * ��������� ������� ��� �������� � �����������
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#include "stdafx.h"
#include "Manager.h"
#include "CustomerPropertiesDlg.h"

// CCustomerPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CCustomerPropertiesDlg, CDialog)

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CCustomerPropertiesDlg
//
//  Description:
//
//  �����������.
//
///////////////////////////////////////////////////////////////////////////////

CCustomerPropertiesDlg::CCustomerPropertiesDlg (CWnd* pParent /*=NULL*/,
                                               const wchar_t *wcsCaption/* = L"�������� �����������"*/
                                               )
	: CDialog(CCustomerPropertiesDlg::IDD, pParent)
    , m_PowerLimit(0), m_strCaption (wcsCaption)
{
    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CCustomerPropertiesDlg
//
//  Description:
//
//  ����������.
//
///////////////////////////////////////////////////////////////////////////////

CCustomerPropertiesDlg::~CCustomerPropertiesDlg ()
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

void CCustomerPropertiesDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text (pDX, IDC_CUSTOMER_NAME, m_CustomerName);
    DDX_Control(pDX, IDC_POWER_LIMIT_SPIN, m_PowerLimitSpin);
    DDX_Text(pDX, IDC_POWER_LIMIT, m_PowerLimit);
}


BEGIN_MESSAGE_MAP(CCustomerPropertiesDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CCustomerPropertiesDlg::OnBnClickedOk)
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

BOOL CCustomerPropertiesDlg::OnInitDialog ()
{
    CDialog::OnInitDialog();
    SetWindowText (m_strCaption);
    SetIcon (AfxGetApp ()->LoadIcon (IDI_ICON1), TRUE);

    m_PowerLimitSpin.SetRange32 (0, 0x7ffffff);
    m_PowerLimitSpin.SetBuddy (GetDlgItem (IDC_POWER_LIMIT));

    m_CustomerName = m_Customer.GetName ();
    m_PowerLimit = (UINT) m_Customer.GetPowerLimit ();

    UpdateData (FALSE);

    return TRUE;  
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CheckData
//
//  Description:
//
//  ���������, ������� �� ��������� ������������� ������ � ����������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CCustomerPropertiesDlg::CheckData ()
{
    UpdateData ();
    if (m_CustomerName.IsEmpty ())
    {
        MessageBox (L"��� ����������� �� ������ ���� ������.");
        GetDlgItem (IDC_CUSTOMER_NAME)->SetFocus ();

        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnBnClickedOk
//
//  Description:
//
//  ���������� ����� �� ������ OK.
//
///////////////////////////////////////////////////////////////////////////////

void CCustomerPropertiesDlg::OnBnClickedOk ()
{
    if (CheckData ())
    {
        m_Customer.SetName (m_CustomerName);
        m_Customer.SetPowerLimit (m_PowerLimit);

        OnOK ();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSize
//
//  Description:
//
//  ���������� ��������� ������� ����.
//
///////////////////////////////////////////////////////////////////////////////

void CCustomerPropertiesDlg::OnSize (UINT nType, int cx, int cy)
{
    CDialog::OnSize (nType, cx, cy);
    CWnd *pWnd = GetDlgItem (IDC_CUSTOMER_NAME);
    if (NULL == pWnd)
    {
        return;
    }

    int nOffset = 4;

    RECT rect;
    pWnd->GetWindowRect (&rect);
    ScreenToClient (&rect);
    rect.right = cx - nOffset;
    pWnd->MoveWindow (&rect);

}
