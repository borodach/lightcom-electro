///////////////////////////////////////////////////////////////////////////////
//
//  File:           CounterPropertiesDlg.h
//
//  Facility:       Desktop утилита для работы с результатами сканирования 
//                  счетчиков.
//                  
//
//
//  Abstract:       Диалог свойств счетчика.
//
//
//  Environment:    MSVC 2005
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CounterPropertiesDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
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
    // Конструктор.
    //
    
    CCounterPropertiesDlg(CWnd* pParent = NULL,
                          const wchar_t *wcsCaption = L"Свойства счётчика");

    //
    // Деструктор.
    //

	virtual ~CCounterPropertiesDlg();

    //
    // Идентификатор диалога.
    //

	enum { IDD = IDD_COUNTER_PROP };

    //
    // Методы доступа к редактируемому счетчику.
    //

    const Customer::Counter& GetCounter () const {return m_Counter;}
    Customer::Counter& GetCounter () {return m_Counter;}
    void SetCounter (const Customer::Counter& сounter) 
    {
        m_Counter = сounter;
    }

    //
    // Проверяет, валидны ли введенные пользователем данные.
    //

    BOOL CheckData ();

    //
    // Обмен данными с диалогом.
    //

	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV support

    //
    // Инициализация диалога.
    //

    virtual BOOL OnInitDialog ();

    //
    // Обработчик клика на кнопку OK.
    //

    afx_msg void OnBnClickedOk ();

    //
    // Карта сообщений.
    //

	DECLARE_MESSAGE_MAP()
    
protected:
    
    //
    // Спин для КТН.
    //

    CSpinButtonCtrl m_KtnSpin;

    //
    // Спин для КТТ.
    //

    CSpinButtonCtrl m_KttSpin;

    //
    // Серийный номер.
    //

    UINT m_nSerialNumber;

    //
    // Коэффициент трансформации напряжения.
    //

    UINT m_nKtn;

    //
    // Коэффициент трансформации тока.
    //

    UINT m_nKtt;

    //
    // Тип сетчика.
    //

    CComboBox m_CounterType;

    //
    // Редактируемый счетчик.
    //

    Customer::Counter m_Counter;    

    //
    // Заголовок диалога.
    //

    CString m_strCaption;
};
