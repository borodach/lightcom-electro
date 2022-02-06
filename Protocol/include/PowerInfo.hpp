///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerInfo.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Классы для подсчета максимальных мощностей по мечяцам.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  30-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: PowerInfo.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _POWER_INFO_
#define _POWER_INFO_

#ifndef IO_DELIM
#define IO_DELIM ' '
#endif


#include <iostream>
#include <vector>
#include <math.h>
#include "Utils.h"

//
// Информация о мощности
//

struct PowerInfo 
{
    //
    // Конструктор по-умолчанию
    //

    PowerInfo ()
    {
        Reset ();
    }

    //
    // Конструктор копирования
    //

    PowerInfo (const PowerInfo& other)
    {
        (*this) = other;
    }

    //
    // Оператор копирования
    //

    PowerInfo& operator = (const PowerInfo& other)
    {
        nPPlus  = other.nPPlus;
        nPMinus = other.nPMinus;
        nQPlus  = other.nQPlus;
        nQMinus = other.nQMinus;
        nState  = other.nState;
        nT      = other.nT;
        nMinute = other.nMinute; 
        nHour   = other.nHour;
        nDay    = other.nDay;
        nMonth  = other.nMonth;
        nYear   = other.nYear;

        return *this;
    }

    //
    // Оператор ==
    //

    bool operator == (const PowerInfo& other) const
    {
        return (nYear == other.nYear) &&
               (nMonth == other.nMonth) &&
               (nDay == other.nDay) &&
               (nHour == other.nHour) &&
               (nHour == other.nHour);
    }
                                                

    //
    // Оператор <
    //

    bool operator < (const PowerInfo& other) const
    {
        if (nYear != other.nYear) return nYear < other.nYear;
        if (nMonth != other.nMonth) return nMonth < other.nMonth;
        if (nDay != other.nDay) return nDay < other.nDay;
        if (nHour != other.nHour) return nHour < other.nHour;
        if (nMinute != other.nMinute) return nMinute < other.nMinute;

        return false;
    }

    //
    // Оператор сложения
    //

    const PowerInfo& operator += (const PowerInfo& other)
    {
        nPPlus  += other.nPPlus;
        nPMinus += other.nPMinus;
        nQPlus  += other.nQPlus;
        nQMinus += other.nQMinus;

        return *this;
    }


    //
    // Оператор умножения на число
    //

    const PowerInfo& operator *= (double k)
    {
        nPPlus  = (__int64) (nPPlus * k);
        nPMinus = (__int64) (nPMinus * k);
        nQPlus  = (__int64) (nQPlus * k);
        nQMinus = (__int64) (nQMinus * k);

        return *this;
    }

    //
    // Нормализует данные, переводя показатели в Вт*час (вар)
    //

    void Normalize (int kTn_kTt, double dCounterConst)
    {
        if (0 == nT || 0 == dCounterConst) return;
        double k = 1000 * kTn_kTt * 60.0 / (nT * 2 * dCounterConst);
        //double k = 10000 * kTn_kTt / (2 * dCounterConst);
        *this *= k;

        /*
        if (nMinute < 30)
        {                                                                                                               
            nMinute = 0;
        }
        else if (nMinute > 30)
        {
            nMinute = 30;
        }
        */
    }

    //
    // Читает данные из буфера с данными протокола #2
    //

    void ReadFromV2Buffer (const unsigned char *pData)
    {
        nPPlus  = pData [7] + 256 * (unsigned short) pData [8];
        nPMinus = pData [9] + 256 * (unsigned short) pData [10];
        nQPlus  = pData [11] + 256 * (unsigned short) pData [12];
        nQMinus = pData [13] + 256 * (unsigned short) pData [14];       
        nState = pData [0];
        nT = pData [6];

        nHour   = IntFromBCD (pData [1]);
        nMinute = IntFromBCD (pData [2]);
        nDay    = IntFromBCD (pData [3]);
        nMonth  = IntFromBCD (pData [4]);
        nYear  =  2000 + IntFromBCD (pData [5]);
    }

    //
    // Очистка объекта
    //

    void Reset ()
    {
        nPPlus  = 0;
        nPMinus = 0;
        nQPlus  = 0;
        nQMinus = 0;
        nState  = 0;
        nT      = 0;
        nMinute = 0; 
        nHour   = 0;
        nDay    = 0;
        nMonth  = 0;
        nYear   = 0;
    }

    //
    // Сохранение объекта в поток
    //

    bool SaveGuts (std::ostream &os) const
    {
        os.width (2);
        os.fill ('0');
        os << nHour   << IO_DELIM;
        os.width (2);
        os.fill ('0');
        os << nMinute << IO_DELIM;
        os.width (2);
        os.fill ('0');
        os << nDay    << IO_DELIM;
        os.width (2);
        os.fill ('0');
        os << nMonth  << IO_DELIM;
        os.width (4);
        os.fill ('0');
        os << nYear   << IO_DELIM;
        os.width (5);
        os.fill ('0');
        os << nPPlus  << IO_DELIM;
        os.width (5);
        os.fill ('0');
        os << nPMinus << IO_DELIM;
        os.width (5);
        os.fill ('0');
        os << nQPlus  << IO_DELIM;
        os.width (5);
        os.fill ('0');
        os << nQMinus << IO_DELIM;
        os.width (2);
        os.fill ('0');
        os << nT << IO_DELIM;
        os.width (3);
        os.fill ('0');
        os << nState;        

        return ! (! os);
    }

    //
    // Восстановление объекта из потока
    //

    bool RestoreGuts (std::istream &is)
    {
        is >> nHour
           >> nMinute   
           >> nDay   
           >> nMonth 
           >> nYear  
           >> nPPlus
           >> nPMinus
           >> nQPlus 
           >> nQMinus
           >> nT
           >> nState;

        return ! (! is);
    }

    //
    // Зафиксированные мощности
    //

    __int64 nPPlus;   // P+
    __int64 nPMinus;  // P-
    __int64 nQPlus;   // Q+
    __int64 nQMinus;  // Q-

    unsigned nState;      // Статус записи
    unsigned nT;          // Период интегригования

    //
    // Дата и время фиксации.
    //

    unsigned nMinute; 
    unsigned nHour;
    unsigned nDay;
    unsigned nMonth;
    unsigned nYear;  
};

typedef std::vector <PowerInfo> PowerStorage;

//
// Класс для подсчета максимальных мощностей
//
/*
class MaxPowerStorage
{
public:

    //
    // Хранилище максимальных максимальных можностей за месяцы.
    //

    typedef std::vector <PowerInfo> MaxPowerPerMonth;

    //
    // Конструктор
    //

    MaxPowerStorage ();

    //
    // Деструктор
    //

    virtual ~MaxPowerStorage ();

    //
    // P+
    //
    
    MaxPowerPerMonth& GetActiveDirectPower () {return m_ActiveDirect;}
    const MaxPowerPerMonth& GetActiveDirectPower () const {return m_ActiveDirect;}

    //
    // P-
    //

    MaxPowerPerMonth& GetActiveRevercePower () {return m_ActiveReverce;}
    const MaxPowerPerMonth& GetActiveRevercePower () const {return m_ActiveReverce;}

    //
    // Q+
    //

    MaxPowerPerMonth& GetReactiveDirectPower () {return m_ReactiveDirect;}
    const MaxPowerPerMonth& GetReactiveDirectPower () const {return m_ReactiveDirect;}

    //
    // Q-
    //

    MaxPowerPerMonth& GetReactiveRevercePower () {return m_ReactiveReverce;}
    const MaxPowerPerMonth& GetReactiveRevercePower () const {return m_ReactiveReverce;}

    //
    // Очистка объекта
    //

    void Reset ();

    //
    // Добавление сведений о мощности
    //

    void AddPowerInfo (unsigned char *pData, 
                       double counterConst,
                       unsigned nKn = 1,
                       unsigned nKt = 1);
    
protected:

    //
    // Добавление сведений о мощности в заданное хранилище
    //

    void AddPowerInfo (const PowerInfo &powerInfo, 
                       MaxPowerPerMonth &powerStorage);
protected:
    MaxPowerPerMonth m_ActiveDirect;        // P+
    MaxPowerPerMonth m_ActiveReverce;       // P-
    MaxPowerPerMonth m_ReactiveDirect;      // Q+
    MaxPowerPerMonth m_ReactiveReverce;     // Q-
};
*/
#endif