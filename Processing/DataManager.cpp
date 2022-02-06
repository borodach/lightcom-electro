///////////////////////////////////////////////////////////////////////////////
//
//  File:           DataManager.cpp
//
//  Facility:       Инспекция счетчиков
//
//
//  Abstract:       Класс, упарвляющий данными обо всех потребителях
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  05-06-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: DataManager.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/ManagerPPC/Processing
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/Processing
 * 
 * *****************  Version 10  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Processing
 * Подмешаны результаты работы не дома
 * 
 * *****************  Version 9  *****************
 * User: Sergey       Date: 19.01.07   Time: 21:55
 * Updated in $/PPCMercury.root/Processing
 * Исправлены мелкие ошибки
 * 
 * *****************  Version 8  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing
 * Добавлены Source Safe comments
* 
*/

#include "afxwin.h"
#include <algorithm>
#include "DataManager.hpp"
#include "EnergyReport.hpp"
#include "RegKeys.h"

const wchar_t* monthNames [] =
{
    L"Январь", L"Февраль", L"Март",     L"Апрель",  L"Май",    L"Июнь", 
    L"Июль",   L"Август",  L"Сентябрь", L"Октябрь", L"Ноябрь", L"Декабрь"
};
const wchar_t* monthNamesForDate [] =
{
    L"января", L"февраля", L"марта",     L"апреля",  L"мая",    L"июня", 
    L"июля",   L"августа",  L"сентября", L"октября", L"ноября", L"декабря"
};

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DataManager
//
//  Description:
//
//  Очистка объекта
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::Reset ()
{
    m_CounterData.clear ();
    m_Customers.clear ();
    m_nCurrentCustomerIdx = m_nInvalidIdx;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       LoadCustomers
//
//  Description:
//
//  Загрузка списка потребителей из файла strFileName
//
///////////////////////////////////////////////////////////////////////////////

bool DataManager::LoadCustomers (const wchar_t* wcsFileName/* = NULL*/)
{
    wchar_t wcsPath [1024];
    if (NULL == wcsFileName)
    {
        GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
        wcscat (wcsPath, L"\\Customers.txt");
    }
    else
    {
        wcscpy (wcsPath, wcsFileName);
    }
    
    
    CString strCurrentCustomerName;
    if (m_nCurrentCustomerIdx != m_nInvalidIdx && 
        m_nCurrentCustomerIdx < m_Customers.size ())
    {
        strCurrentCustomerName = m_Customers [m_nCurrentCustomerIdx].GetName ();
    }

    Reset ();

    std::ifstream is (wcsPath);
    if (! is.is_open ())
    {
        return false;
    }

    size_t nCount = 0;
    size_t nNewCurrentIdx = m_nInvalidIdx;
    is >> nCount;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        Customer customer;
        if (! customer.RestoreGuts (is))
        {
            Reset ();
            return false;
        }

        if (bEvaluation && nIdx >= EVALUATION_MAX_CUSTOMERS)
        {
            continue;    
        }

        m_Customers.push_back (customer);
    }

    std::sort (m_Customers.begin (), m_Customers.end ());
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        if (m_Customers [nIdx].GetName () == strCurrentCustomerName)
        {
            nNewCurrentIdx = nIdx;
        }
    }

    SetCurrentCustomerIdx (nNewCurrentIdx);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveCustomers
//
//  Description:
//
//  Сохранение списка потребителей в файла strFileName
//
///////////////////////////////////////////////////////////////////////////////    

bool DataManager::SaveCustomers (const wchar_t* wcsFileName/* = NULL*/) const
{
    wchar_t wcsPath [1024];
    if (NULL == wcsFileName)
    {
        GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
        wcscat (wcsPath, L"\\Customers.txt");
    }
    else
    {
        wcscpy (wcsPath, wcsFileName);
    }
   
    std::ofstream os (wcsPath);
    if (! os.is_open ())
    {
        return false;
    }

    size_t nCount = m_Customers.size ();
    size_t nNewCurrentIdx = 0;
    os << nCount << '\n';
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const Customer &customer = m_Customers [nIdx];
        if (! customer.SaveGuts (os))
        {
            return false;
        }
        os << '\n';
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetCurrentCustomerIdx
//
//  Description:
//
//  Текущий потребитель
//
///////////////////////////////////////////////////////////////////////////////    

void DataManager::SetCurrentCustomerIdx (size_t nNewIdx, 
                                         const wchar_t* wcsResultsFolder/* = NULL*/)
{
    CWaitCursor wait; 
    wchar_t wcsPath [1024];

    if (NULL == wcsResultsFolder)
    {
        GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
        wcscat (wcsPath, m_strResultsFolder);
    }
    else
    {
        wcscpy (wcsPath, wcsResultsFolder);
    }

    m_CounterData.clear ();
    if (m_nInvalidIdx == nNewIdx || nNewIdx >= m_Customers.size ())
    {
        m_nCurrentCustomerIdx = m_nInvalidIdx;
        return;
    }

    m_nCurrentCustomerIdx = nNewIdx;
    Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    LoadAllCounterData (customer, wcsPath);    

    GenerateAllReports ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       LoadAllCounterData
//
//  Description:
//
//  Загружает результаты сканирования всех счетчиков заданного потребителя
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::LoadAllCounterData (Customer &customer,
                                      const wchar_t *wcsPath)
{
    Customer::CounterList &counters = customer.Counters ();
    size_t nCount = counters.size ();
    m_CounterData.resize (nCount);
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        CounterData &counterData = m_CounterData [nIdx];
        Customer::Counter &counter =  counters [nIdx];
        counterData.SetCounter (counter);
        counterData.SetState (CounterData::dsNotInited);

        CString strFileName;
        counter.GetFileName (strFileName);
        strFileName += L"."APP_RESULTS_EXT;
        CString strFullFilePath (wcsPath);
        int nLength = strFullFilePath.GetLength ();
        if (nLength > 0)
        {
            if (strFullFilePath [nLength - 1] != L'\\')
            {
                strFullFilePath += L"\\";
            }
        }

        strFullFilePath += strFileName;
        counterData.LoadMeteringDataFromFile (strFullFilePath);

    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       InsertCustomer
//
//  Description:
//
//  Добавление потребителя
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::InsertCustomer (const Customer &customer, size_t nCustomerIdx)
{
    size_t nCount = m_Customers.size ();
    if (nCustomerIdx >= nCount)
    {
        m_Customers.push_back (customer);

        return;
    }

    m_Customers.insert (m_Customers.begin () + nCustomerIdx, customer);

    size_t nCurrentCustomerIdx = GetCurrentCustomerIdx ();
    if (m_nInvalidIdx != nCurrentCustomerIdx)
    {
        if (nCustomerIdx <= nCurrentCustomerIdx)
        {
            ++nCurrentCustomerIdx;
            JustChangeCurrentCustomerIdx (nCurrentCustomerIdx);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DeleteCustomer
//
//  Description:
//
//  Удаление потребителя
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::DeleteCustomer (size_t nCustomerIdx)
{
    size_t nCount = m_Customers.size ();
    if (nCustomerIdx >= nCount)
    {
        return;
    }

    m_Customers.erase (m_Customers.begin () + nCustomerIdx);

    size_t nCurrentCustomerIdx = GetCurrentCustomerIdx ();
    if (m_nInvalidIdx != nCurrentCustomerIdx)
    {
        if (nCustomerIdx == nCurrentCustomerIdx)
        {
            SetCurrentCustomerIdx (m_nInvalidIdx);
        }
        else
        {
            if (nCustomerIdx < nCurrentCustomerIdx)
            {
                --nCurrentCustomerIdx;
                JustChangeCurrentCustomerIdx (nCurrentCustomerIdx);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateAllReports
//
//  Description:
//
//  Строит все отчеты
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::GenerateAllReports ()
{
    GenerateSummaryReport (m_strSummaryReport);
    GenerateEnergyReport (m_strEnergyReport);
    GeneratePowerReport (m_strPowerReport);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateSummaryReport
//
//  Description:
//
//  Общий отчет
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::GenerateSummaryReport (CString &strReportText) const
{
    strReportText.Preallocate (32000);
    if (m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        strReportText = L"Для построения общего отчета нужно выбрать потребителя.";
        return;
    }

    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    wchar_t buffer [1024];
    SYSTEMTIME now;  
    GetLocalTime (&now);
    
    strReportText = L"Общий отчет.";
    wsprintf (buffer, L"\nВремя создания: %02u.%02u.%04u %02u:%02u:%02u",
              now.wDay, now.wMonth, now.wYear, 
              now.wHour, now.wMinute, now.wSecond);
    strReportText += L"\nПотребитель: ";
    strReportText += customer.GetName ();

    double dLimit = customer.GetPowerLimit ();
    if (dLimit < 1000)
    {
        swprintf (buffer, L"\nЛимит мощности: %g кВт.", dLimit);
    }
    else 
    {
        swprintf (buffer, L"\nЛимит мощности: %g МВт.", dLimit / 1000);
    }

    strReportText += buffer;

    size_t nCount = m_CounterData.size ();
    wsprintf (buffer, L"\nКоличество счетчиков: %u", nCount);
    strReportText += buffer;
    
    size_t nScannedCount = 0;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        if (m_CounterData [nIdx].IsValid ())
        {
            ++ nScannedCount;
        }
    }
    wsprintf (buffer, L"\nИз них просканировано: %u", nScannedCount);
    strReportText += buffer;
    //strReportText += L"\n\nСчётчики:";

    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const CounterData &data = m_CounterData [nIdx];
        wsprintf (buffer, L"\n\nСчётчик №%u\n", nIdx + 1);
        strReportText += buffer;

        wsprintf (buffer, L"\nСерийный номер: %08u", 
            (int) data.GetCounter ().m_nSerialNumber);
        strReportText += buffer;

        if (Mercury::ptSET == data.GetCounter ().m_nProtocolType)
        {
            strReportText += L"\nТип: СЭТ";
        }
        else if (Mercury::ptMercury == data.GetCounter ().m_nProtocolType)
        {
            strReportText += L"\nТип: Меркурий";
        }
        else
        {
            strReportText += L"\nТип: Неизвестен";
        }

        int nKn = (int) data.GetCounter ().m_nKn;
        int nKt = (int) data.GetCounter ().m_nKt;

        wsprintf (buffer, L"\nКтн.: %d", nKn);
        strReportText += buffer;
        wsprintf (buffer, L"\nКтт.: %d", nKt);
        strReportText += buffer;

        strReportText += L"\nСостояние: ";
        strReportText += data.GetStateText ();
        if (! data.IsValid ())
        {
            strReportText += L"\nРезультатов сканирования нет";
            continue;
        }

        const MeteringData &scanResult = data.GetData ();
        {
            const MeteringData::DateTime &dt = scanResult.GetProductionDate ();
            wsprintf (buffer, L"\nИзготовлен: %02u.%02u.%04u", 
                dt.GetDay (), dt.GetMonth (), dt.GetYear ());
            strReportText += buffer;
        }

        const ProductionType& pt = scanResult.GetProductionType ();
        double dCounterConst = pt.CounterConst1 ();
        if (scanResult.GetProtocolType () == Mercury::ptMercury)  
        {
            dCounterConst = pt.CounterConst2 ();
        }

        swprintf (buffer, L"\nA: %.0f (имп/кВт*ч)", dCounterConst);
        strReportText += buffer;

       
        const MeteringData::DateTime &dt = scanResult.GetCounterTime ();
        wsprintf (buffer, L"\nПросканирован: %02u:%02u:%02u %02u.%02u.%04u (%s)", 
                  dt.GetHour (), dt.GetMinute (), dt.GetSecond (),
                  dt.GetDay (), dt.GetMonth (), dt.GetYear (),
                  scanResult.IsWinter () ? L"зима" : L"лето");
        strReportText += buffer;
       

        EnergyReport rpt (data.GetData ().GetEnergy () [0], dt, nKn, nKt);
        wsprintf (buffer, L"\nЭнергопотребление: %s",
            rpt.IsEmpty () ? L"не прочитано" : L"прочитано");
        strReportText += buffer;

        const PowerStorage &ps = scanResult.GetPowerStorage ();
        size_t nPowerDataCount = ps.size ();

        if (0 == nPowerDataCount)
        {
            strReportText += L"\nМощность: не прочитана";
        }
        else
        {
            wsprintf (buffer, L"\nМощность: с %02u.%02u.%04u по %02u.%02u.%04u",
                ps [0].nDay, ps [0].nMonth, ps [0].nYear,
                ps [nPowerDataCount - 1].nDay, ps [nPowerDataCount - 1].nMonth, ps [nPowerDataCount - 1].nYear);
            strReportText += buffer;
            
        }
        
        
    }

    static wchar_t newLine [] = {13, 13, 10, 0};
    strReportText.Replace (L"\n", newLine);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateEnegyReport
//
//  Description:
//
//  Отчет об энергопотреблении
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::GenerateEnergyReport (CString &strReportText) const
{
    strReportText.Preallocate (32000);
    if (m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        strReportText = L"Для построения отчета по энергопотреблению нужно выбрать потребителя.";
        return;
    }

    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    wchar_t buffer [1024];
    SYSTEMTIME now;  
    GetLocalTime (&now);
    
    strReportText = L"Отчет по энергопотреблению.";
    wsprintf (buffer, L"\nВремя создания: %02u.%02u.%04u %02u:%02u:%02u",
              now.wDay, now.wMonth, now.wYear, 
              now.wHour, now.wMinute, now.wSecond);
    strReportText += L"\nПотребитель: ";
    strReportText += customer.GetName ();

    EnergyReport summaryReport;

    size_t nCount = m_CounterData.size ();
    wsprintf (buffer, L"\nКоличество счетчиков: %u", nCount);
    strReportText += buffer;    

    int nWithoutEnergyData = 0;
    int nNotScanned = 0;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const CounterData &data = m_CounterData [nIdx];
        if (! data.IsValid ())
        {
            ++ nNotScanned;
            continue;
        }
        const MeteringData &scanResult = data.GetData ();
        const MeteringData::DateTime &dt = scanResult.GetCounterTime ();
        EnergyReport rpt (scanResult.GetEnergy () [0], dt, 1, 1);
        if (rpt.IsEmpty ())
        {
            ++ nWithoutEnergyData;
            continue;
        }
    }
    if (0 == (nNotScanned + nWithoutEnergyData))
    {
        strReportText += L"\nДля всех счетчиков есть данные";
    }
    else
    {
        strReportText += L"\nИз них";
        wsprintf (buffer, L"\n    не просканировано: %d\n    без данных: %d", 
                  nNotScanned, 
                  nWithoutEnergyData);
        strReportText += buffer;
    }


    //strReportText += L"\n\nСчётчики:";
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const CounterData &data = m_CounterData [nIdx];
        wsprintf (buffer, L"\n\nСчётчик №%u\n\nСН: %08u", nIdx + 1, 
            (int) data.GetCounter ().m_nSerialNumber);
        strReportText += buffer;
        if (Mercury::ptSET == data.GetCounter ().m_nProtocolType)
        {
            strReportText += L", СЭТ";
        }
        else if (Mercury::ptMercury == data.GetCounter ().m_nProtocolType)
        {
            strReportText += L", Меркурий";
        }
        else
        {
            strReportText += L", Неизвестен";
        }

        int nKn = data.GetCounter ().m_nKn;
        int nKt = data.GetCounter ().m_nKt;
        wsprintf (buffer, L"\nКтн = %d Ктт = %d", 
            nKn, nKt);
        strReportText += buffer;
        if (! data.IsValid ())
        {
            strReportText += L"\nРезультатов сканирования нет";
            continue;
        }
        
        const MeteringData &scanResult = data.GetData ();
        const MeteringData::DateTime &dt = scanResult.GetCounterTime ();
        wsprintf (buffer, L"\nПросканирован: %02u.%02u.%04u (%s)", 
                  dt.GetDay (), dt.GetMonth (), dt.GetYear (),
                  scanResult.IsWinter () ? L"зима" : L"лето");
        strReportText += buffer;

        //22.07.2006
        //EnergyReport rpt (scanResult.GetEnergy () [0], dt, nKn, nKt);
        EnergyReport rpt (scanResult.GetEnergy () [0], dt, 1, 1);

        if (rpt.IsEmpty ())
        {
            strReportText += L"\nНет данных по энергии";
            continue;
        }

        const ProductionType& pt = scanResult.GetProductionType ();
        double dCounterConst = pt.CounterConst1 ();
        if (scanResult.GetProtocolType () == Mercury::ptMercury)  
        {
            dCounterConst = pt.CounterConst2 ();
        }
        rpt.Dump (strReportText);        

        summaryReport += rpt;
    }


    strReportText += L"\n\nЭнергопотребление всех счетчиков\n";
    summaryReport.Dump (strReportText);        

    static wchar_t newLine [] = {13, 13, 10, 0};
    strReportText.Replace (L"\n", newLine);

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GeneratePowerReport
//
//  Description:
//
//  Отчет о потреблении мощности
//
///////////////////////////////////////////////////////////////////////////////

#define POWER2STR(a,b)\
    if ((a) < 1000000) swprintf (buffer, L"%g кВт.", (a) / 1000.0);\
else swprintf (buffer, L"%g МВт.", (a) / 1000000.0);\
b = buffer;

void DataManager::GeneratePowerReport (CString &strReportText)
{
    strReportText.Preallocate (32000);
    CString strTmp;
    if (m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        strReportText = L"Для построения отчета по потреблению мощности нужно выбрать потребителя.";
        return;
    }

    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    wchar_t buffer [1024];
    SYSTEMTIME now;  
    GetLocalTime (&now);
    
    strReportText = L"Отчет по потреблению мощности.";
    wsprintf (buffer, L"\nВремя создания: %02u.%02u.%04u %02u:%02u:%02u",
              now.wDay, now.wMonth, now.wYear, 
              now.wHour, now.wMinute, now.wSecond);
    strReportText += L"\nПотребитель: ";
    strReportText += customer.GetName ();

    __int64 powerLimit = (__int64) (1000.0 * customer.GetPowerLimit ());
    POWER2STR (powerLimit, strTmp);
    strReportText += "\nЛимит мощности: ";
    strReportText += strTmp;

    PowerUsageSummarizer summaryReport;

    size_t nCount = m_CounterData.size ();
    wsprintf (buffer, L"\nКоличество счетчиков: %u", nCount);
    strReportText += buffer;    

    int nWithoutData = 0;
    int nNotScanned = 0;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        CounterData &data = m_CounterData [nIdx];
        if (! data.IsValid ())
        {
            ++ nNotScanned;
            continue;
        }
        MeteringData &scanResult = data.GetData ();
        const MeteringData::DateTime &dt = scanResult.GetCounterTime ();
        PowerStorage &ps = scanResult.GetPowerStorage ();
        size_t nPowerDataCount = ps.size ();
        if (0 == nPowerDataCount)
        {
            ++ nWithoutData;
            continue;
        }

        int nKn = data.GetCounter ().m_nKn;
        int nKt = data.GetCounter ().m_nKt;        
        const ProductionType& pt = scanResult.GetProductionType ();
        double dCounterConst = pt.CounterConst1 ();
        if (scanResult.GetProtocolType () == Mercury::ptMercury)  
        {
            dCounterConst = pt.CounterConst2 ();
        }

        int k = nKn * nKt;
        for (size_t nPowerIdx = 0; nPowerIdx < nPowerDataCount; ++ nPowerIdx)
        {
            PowerInfo &powerInfo = ps [nPowerIdx];
            summaryReport.Add (powerInfo, (unsigned int) nIdx, k, dCounterConst);
        }
        
    }
    if (0 == (nNotScanned + nWithoutData))
    {
        strReportText += L"\nДля всех счетчиков есть данные";
    }
    else
    {
        strReportText += L"\nИз них";
        wsprintf (buffer, L"\n    не просканировано: %d\n    без данных: %d", 
                  nNotScanned, 
                  nWithoutData);
        strReportText += buffer;
    }    

    summaryReport.Sort ();
    const PowerUsageSummarizer::SummaryStorage &stg = summaryReport.GetStorage ();
    nCount = stg.size ();

    size_t nIdx = 0;
    MeteringData::DateTime month;

    MonthlyPowerReport reports [2];

    reports [0].strDescription = L"Утро (с 8:30 до 10:30)";
    reports [0].powerLimit = powerLimit;
    reports [0].nFromHour = 9;  //8:30
    reports [0].nFromMinute = 00;
    reports [0].nToHour = 10;
    reports [0].nToMinute = 30;

    reports [1].strDescription = L"Вечер (с 18:00 до 21:00)";
    reports [1].powerLimit = powerLimit;
    reports [1].nFromHour = 18;
    reports [1].nFromMinute = 30;   //18:00
    reports [1].nToHour = 21;
    reports [1].nToMinute = 00;
    /*
    reports [2].powerLimit = powerLimit;
    reports [2].nFromHour = 0;
    reports [2].nFromMinute = 0;
    reports [2].nToHour = 23;
    reports [2].nToMinute = 30;
    */

    size_t nReportCount = SIZE_OF (reports);

    if (nCount)
    {
        bool bResult = true;
        do
        {
            bResult  = ProcessMonth (stg, 
                                     nIdx, 
                                     month,
                                     nReportCount,
                                     reports);

            int nMonth = ((int) month.GetMonth ()) - 1;
            if (nMonth < 0) nMonth = 0;
            if (nMonth > 11) nMonth = 11;

            swprintf (buffer, 
                L"\n\n%s %04u года",
                      monthNames [nMonth], month.GetYear ());
            strReportText += buffer;
            for (size_t i = 0; i < nReportCount; ++ i)
            {
                reports [i].Dump (strReportText, stg, m_CounterData, powerLimit);
            }            
        }
        while (bResult);
    }
    else
    {
        strReportText += L"\nНет данных о потреблении мощности";
    }
    static wchar_t newLine [] = {13, 13, 10, 0};
    strReportText.Replace (L"\n", newLine);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ProcessMonth
//
//  Description:
//
//  Обработка мощностей за один месяц
//
///////////////////////////////////////////////////////////////////////////////

bool DataManager::ProcessMonth (const PowerUsageSummarizer::SummaryStorage &stg, 
                                size_t &nIdx, 
                                MeteringData::DateTime &month,
                                size_t nReportCount,
                                MonthlyPowerReport reports [])
{

    for (size_t i = 0; i < nReportCount; ++ i)
    {
        MonthlyPowerReport &rpt = reports [i];
        rpt.minPower = rpt.minPowerExces = _I64_MAX;
        rpt.maxPower = rpt.avgPower = rpt.countPower = 0;
        rpt.maxPowerExces = rpt.avgPowerExces = 0;
        rpt.excesIndexes.clear ();

        rpt.minPowerIdx = rpt.minPowerExcesIdx = size_t (-1);
        rpt.maxPowerIdx = rpt.maxPowerExcesIdx = size_t (-1);
    }

    size_t nCount = stg.size ();
    month = stg [nIdx].first;
    PowerInfo pi;

    for (; nIdx < nCount; ++ nIdx)
    {
        const PowerUsageSummarizer::SummaryEntry &powerEntry = stg [nIdx];
        if (powerEntry.first.GetYear () != month.GetYear () ||
            powerEntry.first.GetMonth () != month.GetMonth ())
        {

            for (size_t i = 0; i < nReportCount; ++ i)
            {
                MonthlyPowerReport &rpt = reports [i];
                if (0 != rpt.countPower) rpt.avgPower /= rpt.countPower;
                if (rpt.excesIndexes.size () > 0) 
                {
                    rpt.avgPowerExces /= rpt.excesIndexes.size ();
                }
                else
                {
                    rpt.maxPowerExces = 0;
                    rpt.minPowerExces = 0;
                    rpt.avgPowerExces = 0;
                    rpt.minPowerExcesIdx = size_t (-1);
                    rpt.maxPowerExcesIdx = size_t (-1);
                }
            }

            return true;            
        }

        size_t nMinute = powerEntry.first.GetHour () * 60 +  powerEntry.first.GetMinute ();
        powerEntry.second.CreateSummaryUsage (pi);
        for (size_t i = 0; i < nReportCount; ++ i)
        {
            MonthlyPowerReport &rpt = reports [i];
            if (! ((nMinute >= rpt.nFromHour * 60 + rpt.nFromMinute) && 
                (nMinute <= rpt.nToHour * 60 + rpt.nToMinute)))
            {
                continue;
            }

            ++rpt.countPower;
            rpt.avgPower += pi.nPPlus;
            if (pi.nPPlus > rpt.maxPower) 
            {
                rpt.maxPower = pi.nPPlus;
                rpt.maxPowerIdx = nIdx;

            }
            if (pi.nPPlus < rpt.minPower) 
            {
                rpt.minPower = pi.nPPlus;
                rpt.minPowerIdx = nIdx;
            }

            if (pi.nPPlus > rpt.powerLimit)
            {
                rpt.excesIndexes.push_back (nIdx);    
                rpt.avgPowerExces += pi.nPPlus;
                if (pi.nPPlus > rpt.maxPowerExces) 
                {
                    rpt.maxPowerExces = pi.nPPlus;
                    rpt.maxPowerExcesIdx = nIdx;
                }
                if (pi.nPPlus < rpt.minPowerExces) 
                {
                    rpt.minPowerExces = pi.nPPlus;
                    rpt.minPowerExcesIdx = nIdx;
                }
            } 
        }
    }

    for (size_t i = 0; i < nReportCount; ++ i)
    {
        MonthlyPowerReport &rpt = reports [i];
        if (0 != rpt.countPower) rpt.avgPower /= rpt.countPower;
        if (rpt.excesIndexes.size () > 0) 
        {
            rpt.avgPowerExces /= rpt.excesIndexes.size ();
        }
        else
        {
            rpt.maxPowerExces = 0;
            rpt.minPowerExces = 0;
            rpt.avgPowerExces = 0;

            rpt.minPowerExcesIdx = size_t (-1);
            rpt.maxPowerExcesIdx = size_t (-1);
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Dump
//
//  Description:
//
//  Записывает в strReportText отчет о потреблении мощности.
//
///////////////////////////////////////////////////////////////////////////////

void DataManager::MonthlyPowerReport::Dump (CString &strReportText, 
                                            const PowerUsageSummarizer::SummaryStorage &stg,
                                            const CounterDataList &counterList,
                                            __int64 powerLimit) const
{
    wchar_t buffer [1024];
    CString strMax;
    POWER2STR (maxPower, strMax);

    wsprintf (buffer, L"\n\n****************************\n%s",
        strDescription
        /*nFromHour, 
        nFromMinute, 
        nToHour, 
        nToMinute*/);
    strReportText += buffer;

    wsprintf (buffer, 
        L"\nФактический максимум: %s", (LPCTSTR) strMax);
    strReportText += buffer;
    if (maxPower <= powerLimit)
    {
        strReportText += L"\nПревышения нагрузки нет.";
    }
    else
    {
        CString strMaxExces;
        POWER2STR (maxPower - powerLimit, strMaxExces);

        strReportText += L"\nПревышение нагрузки на ";
        strReportText += strMaxExces;
    }

    if (maxPowerIdx != size_t (-1))
    {
        const PowerUsageSummarizer::SummaryEntry &powerEntry = stg [maxPowerIdx];

        int nMonth = ((int) powerEntry.first.GetMonth ()) - 1;
        if (nMonth < 0) nMonth = 0;
        if (nMonth > 11) nMonth = 11;

        wsprintf (buffer, L"\nЗафиксирован: %u %s в %02u:%02u",
            powerEntry.first.GetDay (),
            monthNamesForDate [nMonth],
            powerEntry.first.GetHour (), 
            powerEntry.first.GetMinute ());
        strReportText += buffer;
    }
    strReportText += L"\n****************************";

    if (maxPowerIdx != size_t (-1))
    {
        const PowerUsageSummarizer::SummaryEntry &powerEntry = stg [maxPowerIdx];
        const PowerUsage::UsageStorage& usageStorage = powerEntry.second.GetStorage ();
        size_t nUsageCount = usageStorage.size ();
        for (size_t i = 0; i < nUsageCount; ++i)
        {
            CString strVal;
            const PowerUsage::UsageEntry &entry = usageStorage [i];
            POWER2STR (entry.first->nPPlus, strVal);

            const CounterData &data = counterList [entry.second];
            wsprintf (buffer, L"\n\nСчётчик №%u", entry.second + 1);
            strReportText += buffer;
                                
            wsprintf (buffer, L"\nСерийный номер: %08u", 
                (int) data.GetCounter ().m_nSerialNumber);
            strReportText += buffer;

            if (Mercury::ptSET == data.GetCounter ().m_nProtocolType)
            {
                strReportText += L"\nТип: СЭТ";
            }
            else if (Mercury::ptMercury == data.GetCounter ().m_nProtocolType)
            {
                strReportText += L"\nТип: Меркурий";
            }
            else
            {
                strReportText += L"\nТип: Неизвестен";
            }

            int nKn = (int) data.GetCounter ().m_nKn;
            int nKt = (int) data.GetCounter ().m_nKt;
            wsprintf (buffer, L"\nРасчетный коэффиц.: %d", nKn * nKt);
            strReportText += buffer;
            wsprintf (buffer, L"\nФактич. нагрузка: %s", (LPCTSTR) strVal);
            strReportText += buffer;           

        }

    }
}


/*
void DataManager::MonthlyPowerReport::Dump (CString &strReportText, 
                                            const PowerUsageSummarizer::SummaryStorage &stg) const
{
    wchar_t buffer [1024];
    wsprintf (buffer, L"\n\nИнтервал с %02u:%02u по %02u:%02u", nFromHour, nFromMinute, nToHour, nToMinute);
    strReportText += buffer;

    //strReportText += L"\n\nПревышения лимита мощности.";
    if (excesIndexes.size () > 0)
    {
        CString strMin, strMax, strAvg;
        POWER2STR (minPowerExces, strMin);
        POWER2STR (maxPowerExces, strMax);
        POWER2STR (avgPowerExces, strAvg);

        CString strMinD, strMaxD, strAvgD;
        POWER2STR (minPowerExces - powerLimit, strMinD);
        POWER2STR (maxPowerExces - powerLimit, strMaxD);
        POWER2STR (avgPowerExces - powerLimit, strAvgD);

        swprintf (buffer, 
            L"\nОбнаружено превышений: %u\nМин.: %s (%s)\nМакс.: %s (%s)\nСр.: %s (%s)",
            excesIndexes.size (), 
            (LPCTSTR) strMin, (LPCTSTR) strMinD, 
            (LPCTSTR) strMax, (LPCTSTR) strMaxD, 
            (LPCTSTR) strAvg, (LPCTSTR) strAvgD);
        strReportText += buffer;

        //
        // Вывод списка превышений
        //

        //strReportText += L"\nПревышения";
        size_t nExcesCount = excesIndexes.size ();
        PowerInfo pi;
        CString strVal, strValD;


        __int64 nMaxPowerExces = 0;
        __int64 nMinPowerExces = _UI64_MAX;
        __int64 nAvgPowerExces = 0;
        size_t nDayExcesCount = 0;
        MeteringData::DateTime nMaxExcesTime;
        const PowerUsage::UsageStorage *pUusageStg = NULL;

        size_t nOldDay = 0;
        for (size_t nExces = 0; nExces < nExcesCount; ++ nExces)
        {
            const PowerUsageSummarizer::SummaryEntry &powerEntry = stg [excesIndexes [nExces]];
            powerEntry.second.CreateSummaryUsage (pi);    

            size_t nCurrentDay = powerEntry.first.GetDay ();
            if ((0 == nExces) || nCurrentDay == nOldDay)
            {
                //
                // День не изменился - считаем статистику
                //

                nOldDay = nCurrentDay;
                ++ nDayExcesCount;

                nAvgPowerExces += pi.nPPlus;
                if (pi.nPPlus > nMaxPowerExces) 
                {
                    nMaxPowerExces = pi.nPPlus;
                    nMaxExcesTime = powerEntry.first;
                    pUusageStg = &(powerEntry.second.GetStorage ());
                }
                if (pi.nPPlus < nMinPowerExces) nMinPowerExces = pi.nPPlus;

                if (nExces != nExcesCount - 1) continue;
            }

            //
            // Новый день, готовим данные для отчета
            //

            if (nDayExcesCount)
            {
                nAvgPowerExces /= nDayExcesCount;
            }
            else
            {
                nMaxPowerExces = 0;
                nMinPowerExces = 0;
                nAvgPowerExces = 0;
            }

            POWER2STR (nMinPowerExces, strMin);
            POWER2STR (nMaxPowerExces, strMax);
            POWER2STR (nAvgPowerExces, strAvg);

            POWER2STR (nMinPowerExces - powerLimit, strMinD);
            POWER2STR (nMaxPowerExces - powerLimit, strMaxD);
            POWER2STR (nAvgPowerExces - powerLimit, strAvgD);

            swprintf (buffer, 
                L"\n\n%02u. Кол-во: %u\nМин.: %s (%s)\nМакс.: %s (%s)\nСр.: %s (%s)",
                nOldDay,
                nDayExcesCount, 
                (LPCTSTR) strMin, (LPCTSTR) strMinD, 
                (LPCTSTR) strMax, (LPCTSTR) strMaxD, 
                (LPCTSTR) strAvg, (LPCTSTR) strAvgD);
            strReportText += buffer;

            wsprintf (buffer, 
                L"\nВремя макс. превышения: %02u:%02u",
                      nMaxExcesTime.GetHour (), nMaxExcesTime.GetMinute ());
            strReportText += buffer;
            if (pUusageStg != NULL)
            {
                size_t nUsageCount = pUusageStg->size ();
                //strReportText += L"\nПоказания счётчиков:";
                for (size_t i = 0; i < nUsageCount; ++i)
                {
                    const PowerUsage::UsageEntry &entry = (*pUusageStg) [i];
                    POWER2STR (entry.first->nPPlus, strVal);
                    wsprintf (buffer, 
                        L"\n№%u: %s ",
                        entry.second + 1, (LPCTSTR) strVal);                        
                    strReportText += buffer;
                }
            }

            nOldDay = nCurrentDay;
            nMaxPowerExces = pi.nPPlus;
            nMinPowerExces = pi.nPPlus;
            nAvgPowerExces = pi.nPPlus;
            nDayExcesCount = 1;
            nMaxExcesTime = powerEntry.first;
            pUusageStg = &(powerEntry.second.GetStorage ());
        }
    }
    else
    {
        strReportText += L"\nПревышений не обнаружено";   
    }

    strReportText += L"\n\nПотребление мощности.";
    CString strMin, strMax, strAvg;
    POWER2STR (minPower, strMin);
    POWER2STR (maxPower, strMax);
    POWER2STR (avgPower, strAvg);

    swprintf (buffer, 
        L"\nКоличество: %u\nМин.: %s\nМакс.: %s\nСр.: %s",
        (int) countPower, (LPCTSTR) strMin, (LPCTSTR) strMax, (LPCTSTR) strAvg);
            strReportText += buffer;
}*/