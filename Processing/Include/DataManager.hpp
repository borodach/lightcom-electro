///////////////////////////////////////////////////////////////////////////////
//
//  File:           DataManager.hpp
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
//  Creation Date:  24-05-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: DataManager.hpp $
 * 
 * *****************  Version 6  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * Добавлены Source Safe comments
* 
*/

#include "Customer.hpp"
#include "CounterData.hpp"
#include "PowerUsageSummarizer.hpp"
#include <afxwin.h>

#ifndef _DATA_MANAGER_HPP_
#define _DATA_MANAGER_HPP_

#define APP_RESULTS_FOLDER  L"\\Results"
#define APP_RESULTS_EXT     L"txt"


class DataManager
{
public:

    //
    // Отчет о потреблении мощности
    //

    struct MonthlyPowerReport
    {
    public:

        //
        // Записывает в strReport отчет о потреблении мощности.
        //

        void Dump (CString &strReportText, 
                   const PowerUsageSummarizer::SummaryStorage &stg,
                   const CounterDataList &counterList,
                   __int64 powerLimit) const;

    public:
        __int64 powerLimit;      // Максимально допустимая мощность
        __int64 minPower;        // Минимальная мощность
        size_t  minPowerIdx;     // Индекс записи средних мощностей, соответствующий
                                 // минимальной мощности
        __int64 maxPower;        // Максимальная мощность
        size_t  maxPowerIdx;     // Индекс записи средних мощностей, соответствующий
                                 // максимальной мощности
        __int64 avgPower;        // Средняя мощность
        __int64 countPower;      // Количество записей о мощности
        __int64 minPowerExces;   // Минимальное превышение мощности
        size_t  minPowerExcesIdx;// Индекс записи средних мощностей, соответствующий
                                 // минимальному превышению мощности
        __int64 maxPowerExces;   // Максимальное превышение мощности
        size_t maxPowerExcesIdx; // Индекс записи средних мощностей, соответствующий
                                 // максимальному превышению мощности
        __int64 avgPowerExces;   // Среднее превышение мощности
        std::vector<size_t> excesIndexes; // Индексы превышений мощности

        size_t nFromHour;        // Начало интервала проверки
        size_t nFromMinute;
        size_t nToHour;          // Конец интервала проверки
        size_t nToMinute;                 
        CString strDescription;  // Описание интервала
    };

    //
    // Конструктор
    //

    DataManager (): m_strResultsFolder (APP_RESULTS_FOLDER)
    {

        Reset ();
    }

    //
    // Деструктор
    //

    virtual ~DataManager ()
    {
        Reset ();
    }

    //
    // Очистка объекта
    //

    void Reset ();

    //
    // Загрузка списка потребителей из файла strFileName
    //

    bool LoadCustomers (const wchar_t* wcsFileName = NULL);

    //
    //  Загружает результаты сканирования всех счетчиков заданного потребителя
    //

    void LoadAllCounterData (Customer &customer, const wchar_t *wcsPath);

    //
    // Сохранение списка потребителей в файла strFileName
    //

    bool SaveCustomers (const wchar_t* wcsFileName = NULL) const;

    //
    // Список потребиелей
    //

    CustomerList& GetCustomers () {return m_Customers;}
    const CustomerList& GetCustomers () const {return m_Customers;}

    //
    // Текущий потребитель
    //

    size_t GetCurrentCustomerIdx () const {return m_nCurrentCustomerIdx;}
    void JustChangeCurrentCustomerIdx (size_t nIdx) {m_nCurrentCustomerIdx = nIdx;}
    void SetCurrentCustomerIdx (size_t nIdx,
                                const wchar_t* wcsResultsFolder = NULL);
    //
    // Добавление потребителя
    //

    void InsertCustomer (const Customer &customer, size_t nCustomerIdx);

    //
    // Удаление потребителя
    //

    void DeleteCustomer (size_t nCustomerIdx);
    
    //
    // Данные о счетчиках
    //

    CounterDataList& GetCounterData () {return m_CounterData;}
    const CounterDataList& GetCounterData () const {return m_CounterData;}


    //
    // Общий отчет
    //

    const CString& SummaryReportText () const {return m_strSummaryReport;}

    //
    // Отчет об энергопотреблении
    //

    const CString& EnergyReportText () const {return m_strEnergyReport;}

    //
    // Отчет о потреблении мощности
    //

    const CString& PowerReportText () const {return m_strPowerReport;}

    //
    //  Строит все отчеты
    //

    virtual void GenerateAllReports ();


protected:

    //
    // Общий отчет
    //

    virtual void GenerateSummaryReport (CString &strReportText) const;

    //
    // Отчет об энергопотреблении
    //

    virtual void GenerateEnergyReport (CString &strReportText) const;

    //
    // Отчет о потреблении мощности
    //

    virtual void GeneratePowerReport (CString &strReportText);

    //
    // Папка с результатами сканирования
    //

    const CString& GetResultsFolder () const {return m_strResultsFolder;}
    CString& GetResultsFolder () {return m_strResultsFolder;}
    void SetResultsFolder (const CString& strFolder) {m_strResultsFolder = strFolder;}
    
    //
    // Обработка мощностей за один месяц
    //

    static bool ProcessMonth (const PowerUsageSummarizer::SummaryStorage &stg, 
                              size_t &nIdx, 
                              MeteringData::DateTime &month,
                              size_t nReportCount,
                              MonthlyPowerReport reports []);
public:

    //
    // Невалидный индекс
    //

    static const size_t m_nInvalidIdx = ~0;

protected:
    
    //
    // Список потребителей
    //

    CustomerList m_Customers;

    //
    // Индекс текущего потребителя
    //

    size_t m_nCurrentCustomerIdx;

    //
    // Спискок данных о счетчиках
    //

    CounterDataList m_CounterData;

    //
    // Общий отчет
    //

    CString m_strSummaryReport;

    //
    // Отчет об энергопотреблении
    //

    CString m_strEnergyReport;

    //
    // Отчет о потреблении мощности
    //

    CString m_strPowerReport;

    //
    // Папка с результатами сканирования
    //

    CString m_strResultsFolder;
};
#endif // _DATA_MANAGER_HPP_
