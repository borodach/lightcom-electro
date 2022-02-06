///////////////////////////////////////////////////////////////////////////////
//
//  File:           DataManagerXML.hpp
//
//  Facility:       Desktop утилита для работы с результатами сканирования 
//                  счетчиков.
//                  
//
//
//  Abstract:       Класс для обработки результатов сканирования.
//
//
//  Environment:    MSVC 2005
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  11-Sep-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: DataManagerXML.hpp $
 * 
 * *****************  Version 23  *****************
 * User: Sergey       Date: 19.01.07   Time: 21:56
 * Updated in $/PPCMercury.root/Manager
 * Исправлены мелкие ошибки
 * 
 * *****************  Version 22  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
* 
*/

#ifndef _MATA_MANAGER_XML_HPP_
#define _MATA_MANAGER_XML_HPP_

#define DEFAULT_CUSTOMERS_FILE L"\\Data\\Customers.txt"
#define DEFAULT_RESULTS_FOLDER L"\\Data\\Results\\"
#define DEFAULT_BASE_FOLDER L"\\Data\\"
#define DEFAULT_HTML_FOLDER L"\\html\\"
#define DEFAULT_XML_FOLDER L"\\html\\xml\\"
#define DEFAULT_XSL_FOLDER L"\\html\\xsl\\"

#include "DataManager.hpp"
#include "MSXMLUtils.hpp"
#include "EnergyReport.hpp"
#include "PowerUsageSummarizer.hpp"

class DataManagerXML : public DataManager
{
public:

    //
    // Конструктор
    //

    DataManagerXML ();

    //
    // Деструктор
    //

    virtual ~DataManagerXML (void);

    //
    // Строит все отчеты
    //

    virtual void GenerateAllReports ();

    //
    // Строит сумманый отчет 
    //

    MSXML::IXMLDOMElementPtr GenerateSummaryReport (MSXML::IXMLDOMDocumentPtr &xmlDocument);


    //
    // Строит отчет по потребителю. Возвращает элемент с отчетом. Элемент _не_ 
    // добавляется в документ xmlDocument
    //
    
    MSXML::IXMLDOMElementPtr GenerateCustomerReport (MSXML::IXMLDOMDocumentPtr &xmlDocument, 
                                                     size_t nCustomerIdx);

    //
    // Строит отчет по счетчику
    //

    MSXML::IXMLDOMElementPtr GenerateCounterReport (MSXML::IXMLDOMDocumentPtr &xmlDocument, 
                                                    size_t nCounterIdx);

    
    //
    // Создает XML элемент c отчетом по потреблению мощности счетчиком
    //

    MSXML::IXMLDOMElementPtr GenerateCounterPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                         size_t nCounterIdx);

    //
    // Создает XML элемент c отчетом по потреблению энергии счетчиком
    //

    MSXML::IXMLDOMElementPtr GenerateCounterEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                          size_t nCounterIdx);

    //
    // Создает XML элемент c журналом событий счетчика
    //

    MSXML::IXMLDOMElementPtr GenerateCounterEventLog (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                      size_t nCounterIdx);

    //
    // Создает XML элемент c отчетом по энегропотреблению потребителя
    //

    MSXML::IXMLDOMElementPtr GenerateCustomerEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument);


    //
    // Создает XML элемент отчета суммарного потребления мощности
    //

    MSXML::IXMLDOMElementPtr GenerateCustomerSummaryPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                 PowerUsageSummarizer &summaryReport);

    //
    // Создает XML элемент отчета суммарного потребления мощности по месяцам
    //

    MSXML::IXMLDOMElementPtr GenerateCustomerMonthlyPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                 const PowerUsageSummarizer::SummaryStorage &storage);

    //
    // Добавляет к ptrParent элемент wcsName свременой меткой pTime.
    // По-умолчанию используется текущее системное время.
    //

    static MSXML::IXMLDOMElementPtr CreateTimeStamp (MSXML::IXMLDOMElementPtr ptrParent,
                                                     const wchar_t *wcsName = L"time_stamp",
                                                     PSYSTEMTIME pTime = NULL);
    
    //
    // Создает элемент, описывающий потребление энергии
    //

    static MSXML::IXMLDOMElementPtr CreateEnergyInfoElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                             const EnergyInfo &energyInfo,                                                             
                                                             const wchar_t *wcsName);

    //
    // Создает элемент, описывающий потребление мощности
    //

    static MSXML::IXMLDOMElementPtr CreatePowerInfoElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                            const PowerInfo &powerInfo,                                                            
                                                            const wchar_t *wcsName);

    //
    // Создает элемент, описывающий суммарное потребление мощности
    //

    static MSXML::IXMLDOMElementPtr CreateSummaryPowerEntryElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const PowerUsageSummarizer::SummaryEntry &entry,                                                                    
                                                                    unsigned nIdx,
                                                                    const wchar_t *wcsName);

    //
    // Создает XML элемент описания результатов сканирования счетчика
    //

    static MSXML::IXMLDOMElementPtr CreateCounterDataElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                              const CounterData &counterData);

    //
    // Создает XML элемент счетчика
    //

    MSXML::IXMLDOMElementPtr CreateCounterElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                   unsigned nCounterIdx,
                                                   const Customer::Counter &counter);

    //
    // Создает XML элемент c результатами сканирования счетчика
    //

    static MSXML::IXMLDOMElementPtr CreateMeteringDataElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const MeteringData &meteringData);


    //
    // Создает XML элемент c отчетом по потреблению мощности счетчиком
    //

    static MSXML::IXMLDOMElementPtr CreateCounterPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                             const MeteringData &meteringData);

    //
    // Создает XML элемент c отчетом по потреблению энергии счетчиком
    //

    static MSXML::IXMLDOMElementPtr CreateCounterEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const MeteringData &meteringData);

    //
    // Создает XML элемент c журналом событий счетчика
    //

    static MSXML::IXMLDOMElementPtr CreateCounterEventLog (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                           const MeteringData &meteringData);

    //
    //  Создает XML элемент потребителя
    //

    static MSXML::IXMLDOMElementPtr CreateCustomerElement (MSXML::IXMLDOMDocumentPtr xmlDocument, 
                                                           size_t nCustomerIdx,
                                                           const Customer &customer);
    
    //
    // Создает XML элемент описания суммарного энергопотребления
    //
    
    static MSXML::IXMLDOMElementPtr CreateEnergyReportElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const EnergyReport &rpt,
                                                               const wchar_t *wcsName);

    //
    // Создает XML элемент описания суммарного потребления мощности
    //

    static MSXML::IXMLDOMElementPtr CreateSummaryPowerElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const PowerUsageSummarizer::SummaryStorage &storage,
                                                               const wchar_t *wcsName);

    //
    // Создает XML элемент описания суммарного потребления мощности за месяц
    //

    static MSXML::IXMLDOMElementPtr CreateMonthlyPowerElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const MonthlyPowerReport &rpt,
                                                               const PowerUsageSummarizer::SummaryStorage &storage,
                                                               unsigned nMonth, unsigned nYear,
                                                               const wchar_t *wcsName);

protected:

};

#endif _MATA_MANAGER_XML_HPP_