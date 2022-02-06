///////////////////////////////////////////////////////////////////////////////
//
//  File:           EnergyReport.cpp
//
//  Facility:       Инспекция счетчиков
//
//
//  Abstract:       Файл содержит класс, предназначенный для упрощения 
//                  построения отчета по энергопотреблению
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  10-06-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: EnergyReport.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/Processing
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing
 * Добавлены Source Safe comments
* 
*/

#include "EnergyReport.hpp"

//
// Названия месяцев
//

extern const wchar_t* monthNames [];

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Init
//
//  Description:
//
//  Заполнение объекта данными
//
///////////////////////////////////////////////////////////////////////////////

void EnergyReport::Init (const AllEnergyInfo &energyInfo, 
                         MeteringData::DateTime scanDate,
                         int nKn, 
                         int nKt)
{
    unsigned nMonth = scanDate.GetMonth ();
    unsigned nYear = scanDate.GetYear ();
    int k = nKn * nKt;

    Reset ();

    m_ThisYear = energyInfo.GetEnergyThisYear ();
    m_LastYear = energyInfo.GetEnergyLastYear ();
    m_FromReset = energyInfo.GetEnergyFromReset ();

    m_LastYear *= k;
    m_ThisYear *= k;
    m_FromReset *= k;

    const EnergyInfo *pEnergyMonth = energyInfo.GetEnergyMonth ();

    size_t nCount = 12;
    MeteringData::DateTime dateVal;
    
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const EnergyInfo &eInfo = pEnergyMonth [nIdx];
        if (eInfo.IsEmpty ()) 
        {
            continue;
        }

        dateVal = scanDate;
        dateVal.SetMonth ((unsigned int) (nIdx + 1));
        if (nIdx + 1 > nMonth)
        {
            dateVal = scanDate;
            dateVal.SetYear (nYear - 1);
        }

        ReportEntry entry (dateVal, eInfo);
        entry.second *= k;

        m_Storage.push_back (entry);
    }

    std::sort (m_Storage.begin (), m_Storage.end ());
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DumpMonth
//
//  Description:
//
//  Записывает в строку strReport отчет за месяц nMonth.
//  nMonth - смещение относительно последнего месяца. Должно быть 
//  неположительным
//
///////////////////////////////////////////////////////////////////////////////

void EnergyReport::DumpMonth (CString &strReport, int nMonth) const
{
    wchar_t buffer [1024];
    int nActiveYear = (int) GetThisYear ();
    int nActiveMonth = ((int) GetThisMonth ()) + nMonth;

    if (nActiveMonth <= 0)
    {
        nActiveMonth += 12;
        -- nActiveYear;
    }
  
    if (nActiveMonth < 1) nActiveMonth = 1;
    if (nActiveMonth > 11) nActiveMonth = 11;

    swprintf (buffer, L"\n%s %04u года:",
              monthNames [nActiveMonth - 1], nActiveYear);
    strReport += buffer;
    size_t nCount = m_Storage.size ();
    if ((int) nCount > - nMonth)
    {
        
        const ReportEntry &entry = m_Storage [nCount + nMonth - 1];
        if (entry.first.GetYear () == nActiveYear && 
            entry.first.GetMonth () == nActiveMonth)
        {
            const EnergyInfo &lastMonth = entry.second;
            double dAValue = lastMonth.nActiveDirect / 1000.0;
            double dRValue = lastMonth.nReactiveDirect / 1000.0;
            swprintf (buffer, L"\n\tA = %3g кВт*ч\n\tR = %3g кВар*ч",
                dAValue, dRValue);
            strReport += buffer;
        }
        else
        {
            strReport += "\n\tнет данных";
        }
    }
    else
    {
        strReport += "\n\tнет данных";
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Dump
//
//  Description:
//
//  Запись информации об объекте в строку
//
///////////////////////////////////////////////////////////////////////////////

void EnergyReport::Dump (CString &strReport) const
{
    if (IsEmpty ())
    {
        strReport += L"\nНет данных";
        return;
    }

    wchar_t buffer [1024];

    DumpMonth (strReport, 0);
    DumpMonth (strReport, -1);
    DumpMonth (strReport, -2);

    size_t nYear = GetThisYear ();

    if (m_LastYear.bValid)
    {
        double dAValue = m_ThisYear.nActiveDirect / 1000.0;
        double dRValue = m_ThisYear.nReactiveDirect / 1000.0;
        swprintf (buffer, L"\nПоследний год (%04u):\n\tA = %3g кВт*ч\n\tR = %3g кВар*ч",
            nYear, dAValue, dRValue);
    }
    else
    {
        swprintf (buffer, L"\nПоследний год (%04u):\n\tнет сведений",
            nYear);
    }
    strReport += buffer;

    if (m_LastYear.bValid)
    {
        double dAValue = m_LastYear.nActiveDirect / 1000.0;
        double dRValue = m_LastYear.nReactiveDirect / 1000.0;
        swprintf (buffer, L"\nПредпоследний год (%04u):\n\tA = %3g кВт*ч\n\tR = %3g кВар*ч",
            nYear - 1, dAValue, dRValue);
    }
    else
    {
        swprintf (buffer, L"\nПредпоследний год (%04u):\n\tнет сведений",
            nYear - 1);
    }
    strReport += buffer;

    if (m_FromReset.bValid)
    {
        double dAValue = m_FromReset.nActiveDirect / 1000.0;
        double dRValue = m_FromReset.nReactiveDirect / 1000.0;
        swprintf (buffer, L"\nСо сброса:\n\tA = %3g кВт*ч\n\tR = %3g кВар*ч",
            dAValue, dRValue);
    }
    else
    {
        swprintf (buffer, L"\nСо сброса:\n\tнет сведений");
    }
    strReport += buffer;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator +=
//
//  Description:
//
//  Формирование суммарного отчета об энергопотреблении
//
///////////////////////////////////////////////////////////////////////////////

const EnergyReport& EnergyReport::operator += (const EnergyReport& other)
{
    size_t nOtherCount = other.m_Storage.size ();
    for (size_t nOtherIdx = 0; nOtherIdx < nOtherCount; ++ nOtherIdx)
    {
        const ReportEntry &otherEntry = other.m_Storage [nOtherIdx];
        bool bFound = false;

        size_t nThisCount = m_Storage.size ();
        for (size_t nThisIdx = 0; nThisIdx < nThisCount; ++ nThisIdx)
        {
            ReportEntry &thisEntry = m_Storage [nThisIdx];
            if (otherEntry.first.GetYear () == thisEntry.first.GetYear () &&
                otherEntry.first.GetMonth () == thisEntry.first.GetMonth ())
            {
                thisEntry.second += otherEntry.second;
                bFound = true;
                break;
            }
        }

        if (! bFound)
        {
            m_Storage.push_back (otherEntry);
        }
    }

    std::sort (m_Storage.begin (), m_Storage.end ());

    if (other.m_ThisYear.bValid)
    {
        if (! m_ThisYear.bValid)
        {
            m_ThisYear = other.m_ThisYear;
            m_LastYear = other.m_LastYear;
        }
        else
        {
            if (GetThisYear () < other.GetThisYear ())
            {
                m_ThisYear = other.m_ThisYear;
                m_LastYear = other.m_LastYear;
            }
            else if (GetThisYear () == other.GetThisYear ())
            {
                m_ThisYear += other.m_ThisYear;
                m_LastYear += other.m_LastYear;
            }
        }
    }

    if (other.m_FromReset.bValid)
    {
        if (! m_FromReset.bValid)
        {
            m_FromReset = other.m_FromReset;
        }
        else
        {
            m_FromReset += other.m_FromReset;
        }
    }



    return *this;
}

