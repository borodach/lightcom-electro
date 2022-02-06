///////////////////////////////////////////////////////////////////////////////
//
//  File:           CustomerPropertiesDlg.cpp
//
//  Facility:       Desktop утилита для работы с результатами сканирования 
//                  счетчиков.
//                  
//
//
//  Abstract:       Диалог свойств потребителя.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  11-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CustomerPropertiesDlg.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
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
//  Конструктор.
//
///////////////////////////////////////////////////////////////////////////////

CCustomerPropertiesDlg::CCustomerPropertiesDlg (CWnd* pParent /*=NULL*/,
                                               const wchar_t *wcsCaption/* = L"Свойства потребителя"*/
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
//  Деструктор.
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
//  Обмен данными с диалогом.
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
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnInitDialog
//
//  Description:
//
//  Инициализация диалога.
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
//  Проверяет, валидны ли введенные пользователем данные о потребтеле.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CCustomerPropertiesDlg::CheckData ()
{
    UpdateData ();
    if (m_CustomerName.IsEmpty ())
    {
        MessageBox (L"Имя потребителя не должно быть пустым.");
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
//  Обработчик клика на кнопку OK.
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
