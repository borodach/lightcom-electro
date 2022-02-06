///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerUsageSummarizer.hpp
//
//  Facility:       Инспекция счётчиков
//
//
//  Abstract:       Файл содержит класс для вычисления сведений о суммарном
//                  потреблении мощности несколькоими счетчиками
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  11-06-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: PowerUsageSummarizer.hpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * Добавлены Source Safe comments
* 
*/

#ifndef _POWER_USAGE_CALCULATOR_HPP_
#define _POWER_USAGE_CALCULATOR_HPP_

#include <vector>
#include <algorithm>
#include "PowerUsage.hpp"

class PowerUsageSummarizer
{
public:
    
    //
    // Сведения об потреблении мощности за почаса
    //

    typedef std::pair <MeteringData::DateTime, PowerUsage> SummaryEntry;

    //
    // Сведения об потреблении мощности за весь отчетный период
    //

    typedef std::vector <SummaryEntry> SummaryStorage;

public:

    //
    // Конструктор
    //

    PowerUsageSummarizer ()
    {
    }

    //
    // Деструктор
    //

    virtual ~PowerUsageSummarizer ()
    {
    }

    //
    // Очистка объекта
    //

    void Reset ()
    {
        m_Storage.clear ();
    }

    //
    // Сведения об энергопотреблении за весь отчетный период
    //

    const SummaryStorage & GetStorage () const {return m_Storage;}
    SummaryStorage & GetStorage () {return m_Storage;}

    //
    // Сотритровка хранилища
    //

    void Sort ()
    {
        std::sort (m_Storage.begin (), m_Storage.end ());
    }

    //
    // Добавление сведений о потреблении мощности
    //

    void Add (PowerInfo &powerInfo, 
              unsigned nCounterIdx,
              int kTn_kTt, 
              double dCounterConst);
protected:

    //
    // Сведения об энергопотреблении за весь отчетный период
    //

    SummaryStorage m_Storage;
};

#endif _POWER_USAGE_CALCULATOR_HPP_