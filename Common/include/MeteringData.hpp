///////////////////////////////////////////////////////////////////////////////
//
//  File:           MeteringData.hpp
//
//  Facility:       Хранение результатов сканирования счетчика
//
//
//  Abstract:       Класс для хранения результатов сканирования счетчика.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  26-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MeteringData.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Common/include
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Common/include
* 
*/

#ifndef _METERING_DATA_
#define _METERING_DATA_

#include <time.h>
#include <string.h>
#include <iostream>
#include "Frame.hpp"
#include "ProductionType.hpp"
#include "EnergyInfo.hpp"
#include "ReadLogEntry.hpp"
#include "PowerInfo.hpp"

#define TARIFF_COUNT    5
#define LOG_ENTRY_COUNT 10

#ifndef IO_DELIM
#define IO_DELIM ' '
#endif

#ifndef BOOL_TO_INT
#define BOOL_TO_INT(a) (a ? 1 : 0)
#endif

#ifndef INT_TO_BOOL
#define INT_TO_BOOL(a) (a != 0)
#endif

class MeteringData
{
public:

    //
    // Тип данных "дата/время".
    //

    struct DateTime
    {
        //
        // Очистка объекта.
        //

        void Reset ()
        {
            memset (buffer, 0, sizeof (buffer));
        }

        //
        // Копирование объекта.
        //

        const DateTime& operator = (const DateTime& other)
        {
            memcpy (buffer, other.buffer, sizeof (buffer));
            return *this;
        }

        //
        // сохранёние объекта в поток
        //

        bool SaveGuts (std::ostream &os) const
        {
            os << sizeof (buffer);
            for (size_t nIdx = 0; nIdx < sizeof (buffer); ++nIdx)
            {
                os << IO_DELIM << (unsigned) buffer [nIdx];
            }

            return ! (! os);
        }

        //
        // Восстановление объекта их потока
        //

        bool RestoreGuts (std::istream &is)
        {
            Reset ();
            size_t nSize = 0;
            is >> nSize;
            if (nSize > sizeof (buffer)) return false;
            for (size_t nIdx = 0; nIdx < nSize; ++nIdx)
            {
                unsigned nVal;
                is >> nVal;
                buffer [nIdx] = nVal;
            }

            return ! (! is);
        }

        //
        // Оператор ==
        //

        bool operator == (const DateTime& other) const
        {
            return memcmp (buffer, other.buffer, sizeof (buffer)) == 0;
        }

        //
        // Оператор <
        //

        bool operator < (const DateTime& other) const
        {
            unsigned nVal0 = GetYear ();
            unsigned nVal1 = other.GetYear ();
            if (nVal0 != nVal1) return nVal0 < nVal1;

            nVal0 = GetMonth ();
            nVal1 = other.GetMonth ();
            if (nVal0 != nVal1) return nVal0 < nVal1;

            nVal0 = GetDay ();
            nVal1 = other.GetDay ();
            if (nVal0 != nVal1) return nVal0 < nVal1;

            nVal0 = GetHour ();
            nVal1 = other.GetHour ();
            if (nVal0 != nVal1) return nVal0 < nVal1;

            nVal0 = GetMinute ();
            nVal1 = other.GetMinute ();
            if (nVal0 != nVal1) return nVal0 < nVal1;

            nVal0 = GetSecond ();
            nVal1 = other.GetSecond ();
//            if (nVal0 != nVal1) 
            return nVal0 < nVal1;
        }

        //
        // Возвращает количество секунд.
        //

        unsigned GetSecond () const {return IntFromBCD (buffer [0]);}

        //
        // Возвращает количество минут.
        //

        unsigned GetMinute () const {return IntFromBCD (buffer [1]);}

        //
        // Возвращает количество часов.
        //

        unsigned GetHour () const {return IntFromBCD (buffer [2]);}

        //
        // Возвращает день месяца.
        //

        unsigned GetDay () const {return IntFromBCD (buffer [3]);}

        //
        // Возвращает номер месяца.
        //

        unsigned GetMonth () const {return IntFromBCD (buffer [4]);}

        //
        // Возвращает год.
        //

        unsigned GetYear () const {return 2000 + IntFromBCD (buffer [5]);}

        //
        // Устанавливает минуту.
        //

        void SetMinute (unsigned nMinute) {buffer [1] = BCDFromInt (nMinute);}

        //
        // Устанавливает час.
        //

        void SetHour (unsigned nHour) {buffer [2] = BCDFromInt (nHour);}

        //
        // Устанавливает день.
        //

        void SetDay (unsigned nDay) {buffer [3] = BCDFromInt (nDay);}

        //
        // Устанавливает номер месяца.
        //

        void SetMonth (unsigned nMonth) {buffer [4] = BCDFromInt (nMonth);}

        //
        // Устаневливает номер года.
        //

        void SetYear (unsigned nYear) {buffer [5] = BCDFromInt (nYear - 2000);}

        //
        // Возвращает true если дата/время нулевое
        //

        bool IsEmpty () const
        {
            return (GetYear () == 2000) && (GetMonth () == 0) && (GetDay () == 0) &&
                   (GetHour () == 0) && (GetMinute () == 0) && (GetSecond () == 0);
        }

        //
        // Чтение даны времени из семибайтного буфера
        //

        void ReadFromV1Buffer (unsigned char *pBuffer)
        {
            buffer [0] = pBuffer [0];
            buffer [1] = pBuffer [1];
            buffer [2] = pBuffer [2];
            buffer [3] = pBuffer [4];
            buffer [4] = pBuffer [5];
            buffer [5] = pBuffer [6];
        }

        //
        // Чтение даны времени из шестибайтного буфера
        //

        void ReadFromBuffer (unsigned char *pBuffer)
        {
            memcpy (buffer, pBuffer, sizeof (buffer));
        }        

        //
        // Буфер с упакованной датой/временем.
        //

        unsigned char buffer [6];
    };

    //
    // Класс - элемент журнала событий
    //

    class EventLogEntry
    {
    public:

        //
        // Конструктор
        //

        EventLogEntry () 
        { 
            Reset ();
        }

        //
        // Копирование объекта.
        //

        const EventLogEntry& operator = (const EventLogEntry& other)
        {
            m_FirstValue  = other.m_FirstValue;
            m_SecondValue = other.m_SecondValue;
            m_bValid = other.m_bValid;
            
            return *this;
        }

        //
        // Очистка объкта
        //

        void Reset ()
        {
            SetValid (false);
            m_FirstValue.Reset ();
            m_SecondValue.Reset ();
        }


    //
    // сохранёние объекта в поток
    //

    bool SaveGuts (std::ostream &os) const
    {
        os << BOOL_TO_INT (m_bValid);
        os << IO_DELIM;
        if (! m_FirstValue.SaveGuts (os)) return false;
        os << IO_DELIM;
        if (! m_SecondValue.SaveGuts (os)) return false;        

        return ! (! os);
    }

    //
    // Восстановление объекта из потока
    //

    bool RestoreGuts (std::istream &is)
    {
        int nVal = 0;
        is >> nVal;
        m_bValid = INT_TO_BOOL (nVal);
        if ( !m_FirstValue.RestoreGuts (is)) return false;
        if ( !m_SecondValue.RestoreGuts (is)) return false;

        return ! (! is);
    }


        //
        // Признак валидности
        //

        bool IsValid () const {return m_bValid;}
        void SetValid (bool bValid = true) {m_bValid = bValid;}

        //
        // Первое значение
        //

        DateTime& FirstValue () {return m_FirstValue;}
        const DateTime& FirstValue () const {return m_FirstValue;}

        //
        // Второе значение
        //

        DateTime& SecondValue () {return m_SecondValue;}
        const DateTime& SecondValue () const {return m_SecondValue;}
        
    protected:
        DateTime m_FirstValue;  // Первое значение
        DateTime m_SecondValue; // Второе значение
        bool m_bValid;          // Признак валидности
    };

    //
    // Массив записей журнала событий (просто дата время)
    //

    typedef EventLogEntry EventLogEntries [LOG_ENTRY_COUNT];

    //
    // Конструктор по-умолчанию
    //

    MeteringData ();


    //
    // Конструктор копирования
    //

    MeteringData (const MeteringData &other);

    //
    // Деструктор
    //

    virtual ~MeteringData ();

    //
    // Очистка всех данных
    //

    void Reset ();

    //
    // Оператор копирования
    //

    const MeteringData& operator = (const MeteringData &other);


    //
    // сохранёние объекта в поток
    //

    bool SaveGuts (std::ostream &os) const;

    //
    // сохранёние объекта в подписанный файл
    //

    bool SaveGuts (const wchar_t *wcsFileName, 
                   const void *key = m_DefaultKey,
                   size_t keySize = sizeof (m_DefaultKey)) const;

    //
    // Восстановление объекта из потока
    //

    bool RestoreGuts (std::istream &is);

    //
    // Восстановление объекта из подписанного файла
    //

    bool RestoreGuts (const wchar_t *wcsFileName, 
                      const void *key = m_DefaultKey,
                      size_t keySize = sizeof (m_DefaultKey));

    //
    // Реальная версия объекта
    //

    int GetVersion () const {return m_nVersion;}
    void SetVersion (int nVersion) {m_nVersion = nVersion;}

    //
    // Тип протокола
    //

    Mercury::ProtocolType GetProtocolType () const {return m_ProtocolType;}
    void SetProtocolType (Mercury::ProtocolType nProtocolType) 
    {
        m_ProtocolType = nProtocolType;
    }

    //
    // Сетевой адрес счетчика
    //


    Mercury::Frame::NetAddress GetNetAddress () const {return m_NetAddress;}
    void SetNetAddress (Mercury::Frame::NetAddress nAddress) 
    {
        m_bNetAddressValid = true;
        m_NetAddress = nAddress;
    }
    bool IsNetAddressValid () const {return m_bNetAddressValid;}
    void ResetNetAddress () 
    {
        m_bNetAddressValid = false;
        m_NetAddress = (Mercury::Frame::NetAddress) 0;
    }

    //
    // Текущее время счетчика
    //

    const DateTime& GetCounterTime () const {return m_CounterTime;}
    DateTime& GetCounterTime () {return m_CounterTime;}
    void SetCounterTime (const DateTime & counterTime) 
    {
        m_bCounterTimeValid = true;
        m_CounterTime = counterTime;
    }

    bool IsWinter () const 
    {
        return m_bIsWinter;
    }

    void SetWinter (bool bIsWinter)
    {
        m_bIsWinter = bIsWinter;
    }
    bool IsCounterTimeValid () const {return m_bCounterTimeValid;}
    void ResetCounterTime () 
    {
        m_bCounterTimeValid = false;
        m_CounterTime.Reset ();
        m_bIsWinter = false;
    }

    //
    // Описание места расположения счетчика
    //

    const char* GetLocation () const {return (const char*) m_szLocation;}
    void SetLocation (const char *szLocation) 
    {
        m_bLocationValid = true;
        size_t nMaxSize = SIZE_OF (m_szLocation) - 1;
        strncpy (m_szLocation, 
                 szLocation, 
                 nMaxSize);
        m_szLocation [nMaxSize] = 0;

    }
    bool IsLocationValid () const {return m_bLocationValid;}
    void ResetLocation () 
    {
        m_bLocationValid = false;
        m_szLocation [0] = 0;
    }

    //
    // Коэффициент трансформации по напряжению
    //

    unsigned short GetKn () const {return m_nKn;}
    void SetKn (unsigned short nKn) 
    {
        m_bKnValid = true;
        m_nKn = nKn;
    }
    bool IsKnValid () const {return m_bKnValid;}
    void ResetKn () 
    {
        m_bKnValid = false;
        m_nKn = 0;
    }

    //
    // Коэффициент трансформации по току
    //

    unsigned short GetKt () const {return m_nKt;}
    void SetKt (unsigned short nKt) 
    {
        m_bKtValid = true;
        m_nKt = nKt;
    }
    bool IsKtValid () const {return m_bKtValid;}
    void ResetKt () 
    {
        m_bKtValid = false;
        m_nKt = 0;
    }

    //
    // Серийный номер счетчика
    //

    unsigned int GetSerialNumber () const {return m_nSerialNumber;}
    void SetSerialNumber (unsigned int nSerialNumber) 
    {
        m_bSerialNumberValid = true;
        m_nSerialNumber = nSerialNumber;
    }
    bool IsSerialNumberValid () const {return m_bSerialNumberValid;}
    void ResetSerialNumber () 
    {
        m_bSerialNumberValid = false;
        m_nSerialNumber = 0;
    }

    //
    // Дата изготовления счетчика
    //

    const DateTime& GetProductionDate () const {return m_ProductionDate;}
    DateTime& GetProductionDate () {return m_ProductionDate;}
    void SetProductionDate (const DateTime &productionDate) 
    {
        m_ProductionDate = productionDate;
    }

    //
    // Версия програмного обеспечения счетчика
    //

    const char* GetSoftwareVersion () const {return m_szSoftwareVersion;}
    void SetSoftwareVersion (const char *szSoftwareVersion) 
    {
        m_bSoftwareVersionValid = true;
        size_t nMaxSize = SIZE_OF (m_szSoftwareVersion) - 1;
        strncpy (m_szSoftwareVersion, 
                 szSoftwareVersion, 
                 nMaxSize);
        m_szSoftwareVersion [nMaxSize] = 0;

    }
    bool IsSoftwareVersionValid () const {return m_bSoftwareVersionValid;}
    void ResetSoftwareVersion () 
    {
        m_bSoftwareVersionValid = false;
        m_szSoftwareVersion [0] = 0;
    }


    //
    // Вариант исполнения счетчика
    //

    const ProductionType& GetProductionType () const {return m_ProductionType;}
    ProductionType& GetProductionType () {return m_ProductionType;}

    void SetProductionType (const ProductionType &productionType) 
    {
        m_bProductionTypeValid = true;
        memcpy (m_ProductionType.rawData, 
                productionType.rawData,
                sizeof (m_ProductionType.rawData));
    }

    bool IsProductionTypeValid () const {return m_bProductionTypeValid;}
    void ResetProductionType () 
    {
        m_bProductionTypeValid = false;
        memset (m_ProductionType.rawData, 0, sizeof (m_ProductionType.rawData));
    }
    
    //
    // Сведения о накопленной энергии.
    //

    const AllEnergyInfo * GetEnergy () const {return m_Energy;}
    AllEnergyInfo*  GetEnergy() {return m_Energy;}   
    void ResetEnergy ()
    {
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_Energy); ++ nIdx)
        {
            m_Energy [nIdx].Reset ();
        }
    }

    //
    // Хранилище максимальных мощностей.
    //

    PowerStorage & GetPowerStorage () {return m_PowerStorage;}
    const PowerStorage& GetPowerStorage () const {return m_PowerStorage;}    

    //
    // Журнал событий
    //
    
    EventLogEntries * GetEventLog () {return m_EventLog;}
    const EventLogEntries * GetEventLog () const {return m_EventLog;}
    void ResetEventLog ()
    {
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_EventLog); ++ nIdx)
        {
            for (size_t nIdx1 = 0; nIdx1 < SIZE_OF (m_EventLog [0]); ++ nIdx1)
            {
                m_EventLog [nIdx] [nIdx1].Reset ();
            }
        }
    }

public:

    //
    // Последняя версия формата хранения
    //
    
    static const int           m_nLatestVersion = 1;

    //
    // Ключ по-умолчанию для формирования электронной подписи.
    //

    static const unsigned char m_DefaultKey [16];

protected:

    //
    // Реальная версия объекта
    //

    int                        m_nVersion;

    //
    // Тип протокола
    //

    Mercury::ProtocolType      m_ProtocolType;

    //
    // Сетевой адрес счетчика
    //

    bool                       m_bNetAddressValid;
    Mercury::Frame::NetAddress m_NetAddress;

    //
    // Текущее время счетчика
    //

    bool                       m_bCounterTimeValid;
    DateTime                   m_CounterTime;
    bool                       m_bIsWinter;


    //
    // Описание места расположения счетчика
    //

    bool                       m_bLocationValid;
    char                       m_szLocation [17];

    //
    // Коэффициент трансформации по напряжению
    //

    bool                       m_bKnValid;
    unsigned short             m_nKn;

    //
    // Коэффициент трансформации по току
    //

    bool                       m_bKtValid;
    unsigned short             m_nKt;

    //
    // Серийный номер счетчика
    //
     
    bool                       m_bSerialNumberValid;
    unsigned int               m_nSerialNumber;

    //
    // Дата изготовления счетчика
    //

    DateTime                   m_ProductionDate;

    //
    // Версия програмного обеспечения счетчика
    //

    bool                       m_bSoftwareVersionValid;
    char                       m_szSoftwareVersion [9];

    //
    // Вариант исполнения счетчика
    //

    bool                       m_bProductionTypeValid;
    ProductionType             m_ProductionType;

    //
    // Массив сведений о накопленной энергии.
    // Нулевой индекс - суммарные сведения по всем тарифам. Первый индекс - 
    // сведения по первому тарифу, второй - по второму ...
    //

    AllEnergyInfo m_Energy [TARIFF_COUNT];

    //
    // Хранилище максимальных мощностей.
    //

    PowerStorage m_PowerStorage;

    //
    // Журнал событий. Индекс в массиве m_EventLog четко соответствует значению 
    // Mercury::ReadLogEntry::LogEntryType
    //

    EventLogEntries m_EventLog [Mercury::ReadLogEntry::letMaxValue];
};

//
// Операторы потокового I/O
//

std::ostream& operator << (std::ostream& os, const MeteringData &val);
std::istream& operator >> (std::istream& is, MeteringData &val);

/*
std::ostream& operator << (std::ostream& os, const MeteringData::DateTime &val);
std::istream& operator >> (std::istream& is, MeteringData::DateTime &val);
*/

#endif