///////////////////////////////////////////////////////////////////////////////
//
//  File:           ProductionType.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс для хранения сведений о варианте исполнения счётчика.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  29-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ProductionType.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Common/include
 * 
 * *****************  Version 5  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Common/include
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Common/include
* 
*/

#ifndef _PRODUCTION_TYPE_
#define _PRODUCTION_TYPE_

#ifndef IO_DELIM
#define IO_DELIM ' '
#endif

//
// Массив классов точности (%).
//

const double PrecisionClasses [16] = {0.2, 0.5, 1.0, 2.0};

//
// Массив номинальных напряжений (В).
//

const double NominalVoltage [] = {57.7, 220, 0.0, 0.0};

//
// Массив номинальных токов (А).
//

const double NominalCurrent [] = {5.0, 1.0, 10.0, 0};

//
// Массив постоянных счётчика (имп/кВт*ч) для первого протокола.
//

const double CounterConstonts1 [] = 
{
    5000.0, 25000.0, 1250.0, 6250.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0
};

//
// Массив постоянных счётчика (имп/кВт*ч) для второго протокола.
//

const double CounterConstonts2 [16] = 
{
    5000.0, 25000.0, 1250.0, 500.0,
    1000.0, 250.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0
};

//
// Вариант исполненя счетчика.
//

struct ProductionType
{
    
    //
    // сохранёние объекта в поток
    //

    bool SaveGuts (std::ostream &os) const
    {
        os << sizeof (rawData);
        for (size_t nIdx = 0; nIdx < sizeof (rawData); ++nIdx)
        {
            os << IO_DELIM << (unsigned) rawData [nIdx];
        }

        return ! (! os);
    }

    //
    // Восстановление объекта их потока
    //

    bool RestoreGuts (std::istream &is)
    {
        size_t nSize = 0;
        is >> nSize;
        if (nSize > sizeof (rawData)) return false;
        for (size_t nIdx = 0; nIdx < nSize; ++nIdx)
        {
            unsigned nVal;
            is >> nVal;
            rawData [nIdx] = nVal;
        }

        return ! (! is);
    }

    //
    // Класс точности по активной энергии (%)
    //

    double ActivePrecision () const 
    {
        int nPrecisionIdx = (rawData [0] >> 6) & 0x03;
        return PrecisionClasses [nPrecisionIdx];
    }

    //
    // Класс точности по реактивной энергии (%)
    //

    double ReactivePrecision () const 
    {
        int nPrecisionIdx = (rawData [0] >> 4) & 0x03;
        return PrecisionClasses [nPrecisionIdx];
    }

    //
    // Номинальное напряжение (В)
    //

    double Voltage () const 
    {
        int nVoltageIdx = (rawData [0] >> 2) & 0x03;
        return NominalVoltage [nVoltageIdx];
    }

    //
    // Номинальный ток (А)
    //

    double Current () const 
    {
        int nCurrentIdx = (rawData [0]) & 0x03;
        return NominalCurrent [nCurrentIdx];
    }

    //
    // Число напрвлений
    //

    unsigned DirectionCount () const
    {
        return (rawData [1] & 0x80) ? 1 : 2;
    }

    //
    // Температурный диапазон (градусы Цельсия)
    //

    unsigned TemperatureRange () const
    {
        return (rawData [1] & 0x40) ? 40 : 20;
    }

    //
    // Число фаз счетчика
    //

    unsigned PhaseCount () const
    {
        return (rawData [1] & 0x10) ? 1 : 3;
    }

    //
    // Постоянная счетчика для первого протокола (имп/кВт*ч)
    //

    double CounterConst1 () const
    {
        unsigned nCounterIdx = rawData [1] & 0x0F;
        return CounterConstonts1 [nCounterIdx];
    }

    //
    // Постоянная счетчика для второго протокола (имп/кВт*ч)
    //

    double CounterConst2 () const
    {
        unsigned nCounterIdx = rawData [1] & 0x0F;
        return CounterConstonts2 [nCounterIdx];
    }

    //
    // Возвращает размер памяти
    //

    size_t GetMemorySize () const
    {
        return (rawData [3] & 0x80) ? (128 * 1024) : (64 * 1024);
    }

    //
    // Данные, полученные от счетчика. На данный момент используется только 
    // первые два байта.
    //

    unsigned char rawData [6];
};

#endif
