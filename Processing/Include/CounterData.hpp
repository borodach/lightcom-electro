///////////////////////////////////////////////////////////////////////////////
//
//  File:           CounterData.hpp
//
//  Facility:       Инспекция счетчиков
//
//
//  Abstract:       Класс, хранящий сведения о счетчике
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  21-05-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CounterData.hpp $
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * Добавлены Source Safe comments
* 
*/

#ifndef _COUNTER_DATA_HPP_
#define _COUNTER_DATA_HPP_

#include "Customer.hpp"
#include "MeteringData.hpp"
#include <vector>
#include <fstream>

class CounterData
{
public:

    enum DataState
    {
        dsInvalid = 0,      // Неваидное значение.
        dsOK,               // Данные валидны.
        dsNotInited,        // Данные не проинициализированы.
        dsNoFile,           // Нет файла с данными.
        dsWrongFile,        // Файл есть, но в нем данные для другого счетчика.
        dsIOError,          // Ошибка ввода/вывода.
        dsVerifyFailed,     // Проверка сигнатуры выявила нарушениие целостности.
        dsUnexpectedError,  // Неожиданная ошибка
        dsMaxValue
    };

    //
    // Конструктор по умолчанию
    //

    CounterData ();

    //
    // Конструктор копирования
    //

    CounterData (const CounterData &other);

    //
    // Деструктор
    //

    virtual ~CounterData ();

    //
    // Очистка объекта
    //

    void Reset ();
    //
    // Оператор копирования
    //

    const CounterData& operator = (const CounterData &other);

    //
    // Оператор сравнения
    //

    bool operator == (const CounterData &other) const;
        
    //
    // Состояние данных.
    //

    DataState GetState () const {return m_State;}
    void SetState (DataState state) {m_State = state;}
    bool IsValid () const {return dsOK == m_State || dsVerifyFailed == m_State;}
    const wchar_t* GetStateText () const
    {
        return GetStateText (GetState ());
    }

    //
    // Загружает результаты сканирования счетчика из файла, выполняя проверки
    //

    static const wchar_t * GetStateText (DataState state);

    //
    //  Загружает результаты сканирования счетчика из файла, выполняя проверки
    //

    CounterData::DataState LoadMeteringDataFromFile (const wchar_t *wcsFileName);

    //
    // Описание счетчика
    //

    const Customer::Counter& GetCounter () const {return m_Counter;}
    Customer::Counter& GetCounter () {return m_Counter;}
    void SetCounter (const Customer::Counter& counter) {m_Counter = counter;}

    //
    // Результат сканирования счетчика
    //

    const MeteringData& GetData () const {return m_Data;}
    MeteringData& GetData () {return m_Data;}
    void SetData (const MeteringData& data) {m_Data = data;}

protected:

    //
    // Состояние данных.
    //

    DataState m_State;

    //
    // Описание счетчика
    //

    Customer::Counter m_Counter;

    //
    // Результат сканирования счетчика
    //

    MeteringData m_Data;    
};

//
// Список данных о счетчиках
//

typedef std::vector <CounterData> CounterDataList;

#endif //_COUNTER_DATA_HPP_