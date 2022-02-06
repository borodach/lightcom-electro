///////////////////////////////////////////////////////////////////////////////
//
//  File:           CounterData.cpp
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
//  Creation Date:  27-09-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CounterData.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/Processing
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing
 * Добавлены Source Safe comments
* 
*/

#include "CounterData.hpp"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CounterData
//
//  Description:
//
//  Конструктор по умолчанию
//
///////////////////////////////////////////////////////////////////////////////

CounterData::CounterData ()
{
    Reset ();
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CounterData
//
//  Description:
//
//  Конструктор копирования
//
///////////////////////////////////////////////////////////////////////////////

CounterData::CounterData (const CounterData &other)
{
    *this = other;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CounterData
//
//  Description:
//
//  Деструктор
//
///////////////////////////////////////////////////////////////////////////////

CounterData::~CounterData ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Reset
//
//  Description:
//
//  Очистка объекта
//
///////////////////////////////////////////////////////////////////////////////

void CounterData::Reset ()
{
    m_State = CounterData::dsNotInited;
    m_Data.Reset ();
    m_Counter.Reset ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator =
//
//  Description:
//
//  Оператор копирования
//
///////////////////////////////////////////////////////////////////////////////

const CounterData& CounterData::operator = (const CounterData &other)
{
    m_State = other.m_State;
    m_Counter = other.m_Counter;
    m_Data = other.m_Data;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator ==
//
//  Description:
//
//  Оператор сравнения
//
///////////////////////////////////////////////////////////////////////////////

bool CounterData::operator == (const CounterData &other) const
{
    return m_Counter == other.m_Counter;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetStateText
//
//  Description:
//
//  Возвращает текстовое описание статуса
//
///////////////////////////////////////////////////////////////////////////////

const wchar_t * CounterData::GetStateText (DataState state)
{
    static const wchar_t* stateText [] =
    {
        L"нет данных",
        L"данные валидны",
        L"нет данных",
        L"нет файла",
        L"файл другого счетчика",
        L"ошибка ввода/вывода",
        L"нарушена целостность",
        L"неожиданная ошибка",
    };

    if (state < 0 || state >= dsMaxValue)
    {
        state = dsUnexpectedError;
    }

    return stateText [state];
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       LoadMeteringDataFromFile
//
//  Description:
//
//  Загружает результаты сканирования счетчика из файла, выполняя проверки
//
///////////////////////////////////////////////////////////////////////////////

CounterData::DataState CounterData::LoadMeteringDataFromFile (const wchar_t *wcsFileName)
{
    m_Data.Reset ();
    if (! VerifySignedFile (wcsFileName, 
                            MeteringData::m_DefaultKey, 
                            sizeof (MeteringData::m_DefaultKey)))
    {
        SetState (CounterData::dsVerifyFailed);
    }

    std::ifstream is (wcsFileName);
    if (! is.is_open ())
    {
        SetState (CounterData::dsNoFile);
        return GetState ();
    }

    bool bValid = m_Data.RestoreGuts (is);
    if (bValid)
    {
        PowerStorage& powerStorage =  m_Data.GetPowerStorage ();
        size_t nPowerCount = powerStorage.size ();
        if (nPowerCount > 0)
        {
            std::sort (powerStorage.begin (), powerStorage.end ());
        }

        if (m_Counter.m_nSerialNumber != m_Data.GetSerialNumber ())
        {
            m_Data.Reset ();
            SetState (CounterData::dsWrongFile);
        }
        else
        {
            if (GetState () != CounterData::dsVerifyFailed)
            {
                SetState (CounterData::dsOK);
            }
        }
    }        
    else
    {
        m_Data.Reset ();
        SetState (CounterData::dsIOError);
    }

    return GetState ();
}