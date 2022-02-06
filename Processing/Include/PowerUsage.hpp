///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerUsage.hpp
//
//  Facility:       Инспекция счётчиков
//
//
//  Abstract:       Класс для хранения сведений о потреблении мощности, снятых 
//                  несколькими счетчиками
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
* $History: PowerUsage.hpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * Добавлены Source Safe comments
* 
*/

#ifndef _POWER_USAGE_HPP_
#define _POWER_USAGE_HPP_

#include "CounterData.hpp"
#include "PowerInfo.hpp"


class PowerUsage
{
public:
    typedef std::pair <PowerInfo*, unsigned> UsageEntry;
    typedef std::vector <UsageEntry> UsageStorage;
public:

    //
    // Конструктор
    //

    PowerUsage ()
    {
    }

    //
    // Деструктор
    //

    virtual ~PowerUsage ()
    {
    }

    //
    // Фиктивные операторы сравнения. Считаем, что любые объекты этого класса
    // равны.
    //

    bool operator == (const PowerUsage& other) const 
    {
        return true;
    }

    bool operator < (const PowerUsage& other) const 
    {
        return false;
    }

    //
    // Очистка объекта
    //

    void Reset ()
    {
        m_UsageStorage.clear ();
    }

    //
    // Хранилище потребления мощности
    //

    UsageStorage& GetStorage () {return m_UsageStorage;}
    const UsageStorage& GetStorage () const {return m_UsageStorage;}

    //
    // Добавление сведений о потреблении мощности
    //

    void Add (const UsageEntry& entry);

    //
    // Формирование суммарного отчета о использовании мощности
    //

    void CreateSummaryUsage (PowerInfo &summaryUsage) const;

protected:

    //
    // Хранилище потребления мощности
    //

    UsageStorage m_UsageStorage;
};
#endif //_POWER_USAGE_HPP_
