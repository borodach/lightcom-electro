///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadLogEntry.cpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма запроса на чтение записи из журнала событий.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  24-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ReadLogEntry.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Protocol
 * 
 * *****************  Version 7  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol
 * 
 * *****************  Version 6  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Protocol
 * Добавлены Source Safe comments
* 
*/

#include "Utils.h"
#include "ReadLogEntry.hpp"                   

namespace Mercury
{
    //
    // Описания типов элементов лога
    //

    const ReadLogEntry::RequestInfo ReadLogEntry::m_RequestInfoTable [ReadLogEntry::letMaxValue] = 
    {
        {ReadLogEntry::letOnOff,        0x01, 0x01, 2},
        {ReadLogEntry::letTimeChange,   0x02, 0x02, 2},
        {ReadLogEntry::letPhase1,       0x07, 0x03, 2},
        {ReadLogEntry::letPhase2,       0x08, 0x04, 2},
        {ReadLogEntry::letPhase3,       0x09, 0x05, 2},
        {ReadLogEntry::letTariffTable,  0x04, 0x07, 1},
        {ReadLogEntry::letHolydayTable, 0x03, 0x08, 1},
        {ReadLogEntry::letEnergy,       0x05, 0x09, 1},
        {ReadLogEntry::letPower,        0x06, 0x0A, 1},
        {ReadLogEntry::letProtection,   0x0A, 0x12, 2}
    };


    //
    // Название типов элемента лога
    //

    const wchar_t* ReadLogEntry::m_EvenLogTypeNames [ReadLogEntry::letMaxValue + 1] =
    {
        L"Включение/выключение прибора",
        L"Коррекция часов",
        L"Включение/выключение фазы 1",
        L"Включение/выключение фазы 2",
        L"Включение/выключение фазы 3",                  
        L"Коррекция тарифного расписания",               
        L"Коррекция расписания праздничных дней",
        L"Сброс регистров накопленной энергии",
        L"Инициализация массива средних мощностей",
        L"Открытие/закрытие крышки (электронная пломба)",
        L"Невалидное значение"
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetRequestInfo
    //
    //  Description:
    //
    //  Возвращает описание элемента лога по его типу.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const ReadLogEntry::RequestInfo& ReadLogEntry::GetRequestInfo (ReadLogEntry::LogEntryType entryType)
    {
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_RequestInfoTable); ++ nIdx)
        {
            if (m_RequestInfoTable [nIdx].entryType == entryType)
            {
                return m_RequestInfoTable [nIdx];
            }
        }

        return m_RequestInfoTable [0];
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetRequestInfo
    //
    //  Description:
    //
    //  Возвращает описание элемента лога по его нативному типу.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const ReadLogEntry::RequestInfo& ReadLogEntry::GetRequestInfo () const
    {
        unsigned char nType = GetType ();
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_RequestInfoTable); ++ nIdx)
        {
            unsigned char nNativeCode;
            if (ptSET == m_nProtocolType)
            {
                nNativeCode = m_RequestInfoTable [nIdx].protocol1Command;
            }
            else
            {
                nNativeCode = m_RequestInfoTable [nIdx].protocol2Command;
            }

            if (nNativeCode == nType)
            {
                return m_RequestInfoTable [nIdx];
            }
        }

        return m_RequestInfoTable [0];
    }
}