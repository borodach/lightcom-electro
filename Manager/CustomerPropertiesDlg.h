///////////////////////////////////////////////////////////////////////////////
//
//  File:           CustomerPropertiesDlg.h
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
* $History: CustomerPropertiesDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
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
    // Конструктор.
    //

	CCustomerPropertiesDlg (CWnd* pParent = NULL, 
                            const wchar_t *wcsCaption = L"Свойства потребителя");   

    //
    // Деструктор.
    //

	virtual ~CCustomerPropertiesDlg ();

    //
    // ID диалога.
    //

	enum { IDD = IDD_CUSTOMER_PROP };

    //
    // Методы доступа к редактируемому объекту класса Customer
    //

    const Customer& GetCustomer () const {return m_Customer;}
    Customer& GetCustomer () {return m_Customer;}
    void SetCustomer (const Customer& customer) 
    {
        m_Customer = customer;
    }
    
    //
    // Проверяет, валидны ли введенные пользователем данные о потребтеле.
    //

    BOOL CheckData ();

    //
    // Инициализация диалога.
    //

    virtual BOOL OnInitDialog ();

    //
    // Обмен данными с диалогом.
    //

	virtual void DoDataExchange (CDataExchange* pDX);

    //
    // Обработчик клика на кнопку OK.
    //

    afx_msg void OnBnClickedOk ();

	DECLARE_MESSAGE_MAP()

protected:

    //
    // Название потребителя.
    //

    CString m_CustomerName;

    //
    // Редактируемый потребитель.
    //

    Customer m_Customer;

    //
    // Спин для ввода лимита нагрузки.
    //

    CSpinButtonCtrl m_PowerLimitSpin;

    //
    // Лимит нагрузки в кВт.
    //

    UINT m_PowerLimit;

    //
    // Заголовок диалога.
    //

    CString m_strCaption;    
};
