///////////////////////////////////////////////////////////////////////////////
//
//  File:           EnergyReport.hpp
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
* $History: EnergyReport.hpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * Добавлены Source Safe comments
* 
*/

#ifndef _ENERGY_REPORT_HPP_
#define _ENERGY_REPORT_HPP_

#include "afxwin.h"
#include "MeteringData.hpp"
#include "EnergyInfo.hpp"
#include <vector>
#include <algorithm>

class EnergyReport
{
public:

    //
    // Отчет за один месяц
    //

    typedef std::pair <MeteringData::DateTime, EnergyInfo> ReportEntry;

    //
    // Хранилище помесячных отчетов
    //

    typedef std::vector <ReportEntry> ReportStorage;    

    //
    // Конструктор по-умолчанию
    //

    EnergyReport ()
    {
    }

    //
    // Конструктор
    //

    EnergyReport (const AllEnergyInfo &energyInfo, 
                  MeteringData::DateTime scanDate,
                  int nKn, 
                  int nKt)
    {
        Init (energyInfo, scanDate, nKn, nKt);
    }

    //
    // Деструктор
    //

    virtual ~EnergyReport ()
    {
    }

    //
    // Очистка данных, хранимых в объекте
    //

    void Reset () 
    {
        m_Storage.clear (); 
        m_ThisYear.Reset ();
        m_LastYear.Reset ();
        m_FromReset.Reset ();
    }

    //
    // Возвращает номер года в который были получены данные об энергопотреблении
    //

    size_t GetThisYear () const
    {
        MeteringData::DateTime scanDate;
        if  (!GetScanDate (scanDate))
        {
            return 0;
        }
        return scanDate.GetYear ();
    }

    //
    // Возвращает номер месяца в который были получены данные об энергопотреблении
    //

    size_t GetThisMonth () const
    {
        MeteringData::DateTime scanDate;
        if  (!GetScanDate (scanDate))
        {
            return 0;
        }
        return scanDate.GetMonth ();
    }

    //
    // Возвращает дату/время когда были получены данные об энергопотреблении
    //

    bool GetScanDate (MeteringData::DateTime &scanDate) const
    {
        size_t nCount = m_Storage.size ();
        if (nCount < 1) return false;

        scanDate = m_Storage [nCount - 1].first;

        return true;
    }

    //
    // Проверяет, есть ли данные об энергопотреблении
    //

    bool IsEmpty () const
    {
        return m_Storage.size () == 0;
    }

    //
    // Хранилище сведений о помесячном энергопотреблении
    //

    const ReportStorage& GetStorage () const {return m_Storage;}
    ReportStorage& GetStorage () {return m_Storage;}

    //
    // Энергопотребление в текущем году
    //

    const EnergyInfo& ThisYear () const {return m_ThisYear;}
    EnergyInfo& ThisYear () {return m_ThisYear;}

    //
    // Энергопотребление в прошлом году
    //

    const EnergyInfo& LastYear () const {return m_LastYear;}
    EnergyInfo& LastYear () {return m_LastYear;}

    //
    // Энергопотребление со сброса
    //

    const EnergyInfo& FromReset () const {return m_FromReset;}
    EnergyInfo& FromReset () {return m_FromReset;}

    //
    // Заполнение объекта данными
    //

    void Init (const AllEnergyInfo &energyInfo, 
               MeteringData::DateTime scanDate,
               int nKn, 
               int nKt);

    //
    // Формирование суммарного отчета об энергопотреблении
    //

    const EnergyReport& operator += (const EnergyReport& other);


    //
    // Запись информации об объекте в строку
    //

    void Dump (CString &strReport) const;

    //
    // Записывает в строку strReport отчет за месяц nMonth.
    // nMonth - смещение относительно последнего месяца. Должно быть 
    // неположительным
    //
    
    void DumpMonth (CString &strReport, int nMonth) const;

protected:

    //
    // Хранилище сведений о помесячном энергопотреблении
    //

    ReportStorage m_Storage;

    EnergyInfo m_ThisYear;      // Энергопотребление в текущем году
    EnergyInfo m_LastYear;      // Энергопотребление в прошлом году
    EnergyInfo m_FromReset;     // Энергопотребление со сброса
};

#endif //_ENERGY_REPORT_HPP_