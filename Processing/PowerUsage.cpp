///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerUsage.cpp
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
* $History: PowerUsage.cpp $
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

#include "PowerUsage.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Add
//
//  Description:
//
//  Добавление сведений о потреблении мощности
//
///////////////////////////////////////////////////////////////////////////////

void PowerUsage::Add (const UsageEntry& entry)
{
    size_t nCount = m_UsageStorage.size ();
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        UsageEntry &usageEntry = m_UsageStorage [nIdx];
        if (entry.second == usageEntry.second)
        {
            if (entry.first->nPPlus > usageEntry.first->nPPlus)
            {
                usageEntry = entry;
            }

            return;
        }
    }
    m_UsageStorage.push_back (entry);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateSummaryUsage
//
//  Description:
//
//  Формирование суммарного отчета о использовании мощности
//
///////////////////////////////////////////////////////////////////////////////

void PowerUsage::CreateSummaryUsage (PowerInfo &summaryUsage) const
{
    summaryUsage.Reset ();
    size_t nCount = m_UsageStorage.size ();
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const UsageEntry &usageEntry = m_UsageStorage [nIdx];
        summaryUsage += *(usageEntry.first);
    }
}
