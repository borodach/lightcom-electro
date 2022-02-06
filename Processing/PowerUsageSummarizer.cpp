///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerUsageSummarizer.cpp
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
* $History: PowerUsageSummarizer.cpp $
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

#include "PowerUsageSummarizer.hpp"


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Add
//
//  Description:
//
//
// Добавление сведений о потреблении мощности
//
///////////////////////////////////////////////////////////////////////////////

void PowerUsageSummarizer::Add (PowerInfo &powerInfo, 
                                unsigned nCounterIdx,
                                int kTn_kTt, 
                                double dCounterConst)
{
    PowerUsage::UsageEntry usageEntry;
    usageEntry.second = nCounterIdx;
    usageEntry.first = &powerInfo;
    usageEntry.first->Normalize (kTn_kTt, dCounterConst);

    unsigned nMinute = powerInfo.nMinute;
    if (nMinute < 30)
    {
        nMinute = 0;
    }
    else if (nMinute > 30)
    {
        nMinute = 30;
    }
    MeteringData::DateTime dt;
    dt.SetYear (powerInfo.nYear);
    dt.SetMonth (powerInfo.nMonth);
    dt.SetDay (powerInfo.nDay);
    dt.SetHour (powerInfo.nHour);
    dt.SetMinute (nMinute);

    size_t nCount = m_Storage.size ();
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        SummaryEntry &summaryEntry = m_Storage [nIdx];
        if (summaryEntry.first == dt)
        {
            summaryEntry.second.Add (usageEntry);
            return;
        }
    }

    SummaryEntry summaryEntry;
    summaryEntry.first = dt;
    summaryEntry.second.Add (usageEntry);
    m_Storage.push_back (summaryEntry);

    return;
}