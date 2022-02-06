///////////////////////////////////////////////////////////////////////////////
//
//  File:           Customer.hpp
//
//  Facility:       Инспекция счетчиков
//
//
//  Abstract:       Класс, описывающий потребителя электроэнергии
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  10-05-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Customer.hpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * Добавлены Source Safe comments
* 
*/

#ifndef _CUSTOMER_
#define _CUSTOMER_

#include <vector>
#include <iostream>
#include <afxwin.h>

#include "Frame.hpp"

class Customer
{
public:

    //
    // Описание счетчика
    //

    struct Counter
    {
        //
        // Конструктор по-умолчанию
        //

        Counter ();

        //
        // Конструктор копирования
        //

        Counter (const Counter& other);

        //
        // Очистка объекта
        //

        void Reset ();

        //
        // Оператор копирования
        //

        const Counter& operator = (const Counter& other);

        //
        // Оператор ==
        //

        bool operator == (const Counter& other) const;

        //
        // Оператор <
        //

        bool operator < (const Counter& other) const;

        //
        // Чтение объекта из потока
        //

        bool RestoreGuts (std::istream &is, int nVersion);

        //
        // Запись объекта в поток
        //

        bool SaveGuts (std::ostream &os) const;

        //
        // Возвращает имя файла (без расширения) с резуьтатами сканировнания 
        // счетчика.
        //

        CString& GetFileName (CString &strBuffer) const;        

        //
        // Возвращает имя лог файла.
        //

        CString& GetLogFileName (CString &strBuffer) const;

        //
        // Возвращает строку с типом счетчика.
        //

        const wchar_t* GetTypeString () const;
        
        //
        // Возвращает описание счетчика в коротком формате.
        //

        CString& GetShortDisplayName (CString &strBuffer) const;
        
        //
        // Возвращает описание счетчика в длинном формате.
        //

        CString& GetFullDisplayName (CString &strBuffer) const;
        
        //
        // Серийный номер
        //

        unsigned m_nSerialNumber;

        //
        // Коэффициент трансформации по напряжению
        //

        int m_nKn;

        //
        // Коэффициент трансформации по току
        //

        int m_nKt;

        //
        // Тип протокола счетчика
        //

        Mercury::ProtocolType m_nProtocolType;
    };

    //
    // Список описаний счетчиков
    //

    typedef std::vector <Counter> CounterList; 

public:

    //
    // Конструктор по-умолчанию
    //

    Customer ();

    //
    // Конструктор копирования
    //

    Customer (const Customer &other);

    //
    // Деструктор
    //

    virtual ~Customer ();

    //
    // Очистка объекта
    //

    void Reset ();

    //
    // Оператор присваивания
    //

    const Customer & operator = (const Customer &other);
    //
    // Оператор ==
    //

    bool operator == (const Customer &other) const;

    //
    // Оператор <
    //

    bool operator < (const Customer &other) const;

    //
    // Восстановление объекта из потока
    //

    bool RestoreGuts (std::istream &is);

    //
    // Сохранение объекта в поток
    //

    bool SaveGuts (std::ostream &os) const;

    //
    // Название потребителя
    //

    const CString& GetName() const {return m_strName;}
    CString& GetName() {return m_strName;}
    void SetName (const CString &strName) {m_strName = strName;}
    CString& GetDisplayName (CString &strDisplayName) const
    {
        strDisplayName.Format (L"%s (%u кВт)", 
                               (LPCWSTR) GetName (), 
                               (unsigned) GetPowerLimit ());
        return strDisplayName;
    }


    //
    // Лимит мощности
    //

    double GetPowerLimit () const {return m_fPowerLimit;}
    void SetPowerLimit (double fPowerLimit) {m_fPowerLimit = fPowerLimit;}

    //
    // Счетчики потребителя
    //

    const CounterList& Counters () const {return m_Counters;}
    CounterList& Counters () {return m_Counters;}

public:

    //
    // Номер последней версии формата хранения сведений о 
    //

    static const int m_nLatestVersion = 1;
protected:
    CString     m_strName;      // Название потребителя
    double      m_fPowerLimit;  // Лимит мощности
    CounterList m_Counters;     // Счетчики потребителя
};

//
// Список потребителей
//

typedef std::vector <Customer> CustomerList;

#endif
