///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadLogEntry.hpp
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
* $History: ReadLogEntry.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol/include
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _MERCURY_READ_LOG_ENTRY_
#define _MERCURY_READ_LOG_ENTRY_

#include "Request.hpp"                   

namespace Mercury
{
    class ReadLogEntry: public Request
    {
    public:

        //
        // Тип элемента лога
        //

        enum LogEntryType : unsigned char
        {
            letOnOff = 0,       // Включение/выключение прибора
            letTimeChange,      // Коррекция часов
            letPhase1,          // Включение/выключение фазы 1
            letPhase2,          // Включение/выключение фазы 2
            letPhase3,          // Включение/выключение фазы 3
            letTariffTable,     // Коррекция тарифного расписания
            letHolydayTable,    // Коррекция расписания праздничных дней
            letEnergy,          // Сброс регистров накопленной энергии
            letPower,           // Инициализация массива средних мощностей
            letProtection,      // Открытие/закрытие крышки (электронная пломба)
            letMaxValue         // Максимальное значение
        };

        //
        // Описание запроса на чтение элемента лога
        //
        
        struct RequestInfo
        {
            LogEntryType  entryType;          // Тип элемента лога
            unsigned char protocol1Command;   // Код параметра для протокола 1
            unsigned char protocol2Command;   // Код параметра для протокола 2
            unsigned char responseLength;     // Количество объектов DataTime в ответе.
        };

        //
        // Название типов элемента лога
        //

        static const wchar_t* m_EvenLogTypeNames [letMaxValue + 1];

        //
        // Конструктор
        //

        ReadLogEntry (ProtocolType protocolType,
                      CrcType nType = crc) : 
            Request (rcReadEventLog, ptSET == protocolType? 1 : 2, nType),
            m_nProtocolType (protocolType)
        {
            SetEntryNumber (0);
            SetType (letOnOff);
        }

        //
        // Возвращает тип элемента лога
        //

        LogEntryType GetLogEntryType () const
        {
            const RequestInfo &ri = GetRequestInfo ();

            return ri.entryType;
        }
        
        //
        // Устанавливает тип элемента лога
        //

        void SetLogEntryType (LogEntryType nType)
        {
            const RequestInfo &ri = GetRequestInfo (nType);
            if (ptSET == m_nProtocolType)
            {
                SetType (ri.protocol1Command);
                m_nResponseSize = ri.responseLength * 7;
            }
            else
            {
                SetType (ri.protocol2Command);
                m_nResponseSize = ri.responseLength * 6;
            }            
        }
        
        //
        // Возвращает номер записи в логе
        //

        unsigned char GetEntryNumber () const
        {
            unsigned char nResult = 0;
            if (ptSET == m_nProtocolType)
            {
                nResult = m_Body [2] & 0x0F;
            }
            else
            {
                nResult = m_Body [3];
            }

            return nResult;
        }

        //
        // Устанавивает номер записи в логе
        //

        void SetEntryNumber (unsigned char nEntryNumber)
        {
            if (ptSET == m_nProtocolType)
            {
                m_Body [2] &= 0xF0;
                m_Body [2] |= 0x0F & nEntryNumber;
            }
            else
            {
                m_Body [3] = nEntryNumber;
            }
            CalculateAndStoreCrc ();
        }

        //
        // Возвращает тип протокола
        //

        ProtocolType GetProtocolType () const {return m_nProtocolType;}

        //
        // Возвращает описание элемента лога по его типу.
        //

        static const RequestInfo& GetRequestInfo (LogEntryType entryType);

        //
        // Возвращает описание элемента лога по его нативному типу.
        //

        const RequestInfo& GetRequestInfo () const;

protected:       

        //
        // Описания типов элементов лога
        //

        static const RequestInfo m_RequestInfoTable [letMaxValue];
       
        //
        // Возвращает тип элемента лога
        //

        unsigned char GetType () const
        {
            unsigned char nResult = 0;
            if (ptSET == m_nProtocolType)
            {
                nResult = (m_Body [2] >> 4) & 0x0F;
            }
            else
            {
                nResult = m_Body [2];
            }

            return nResult;   
        }

        //
        // Устанавливает тип элемента лога
        //

        void SetType (unsigned char nType)
        {
            if (ptSET == m_nProtocolType)
            {
                m_Body [2] &= 0x0F;
                m_Body [2] |= 0xF0 & (nType << 4);
            }
            else
            {
                m_Body [2] = nType;
            }
            CalculateAndStoreCrc ();
        }

        //
        // Тип протокола
        //

        ProtocolType m_nProtocolType;
    };
}
#endif