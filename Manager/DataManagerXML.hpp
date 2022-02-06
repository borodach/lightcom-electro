///////////////////////////////////////////////////////////////////////////////
//
//  File:           DataManagerXML.hpp
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//                  
//
//
//  Abstract:       ����� ��� ��������� ����������� ������������.
//
//
//  Environment:    MSVC 2005
//
//  Author:         ������ �.�.
//
//  Creation Date:  11-Sep-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: DataManagerXML.hpp $
 * 
 * *****************  Version 23  *****************
 * User: Sergey       Date: 19.01.07   Time: 21:56
 * Updated in $/PPCMercury.root/Manager
 * ���������� ������ ������
 * 
 * *****************  Version 22  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
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
    // �����������
    //

    DataManagerXML ();

    //
    // ����������
    //

    virtual ~DataManagerXML (void);

    //
    // ������ ��� ������
    //

    virtual void GenerateAllReports ();

    //
    // ������ �������� ����� 
    //

    MSXML::IXMLDOMElementPtr GenerateSummaryReport (MSXML::IXMLDOMDocumentPtr &xmlDocument);


    //
    // ������ ����� �� �����������. ���������� ������� � �������. ������� _��_ 
    // ����������� � �������� xmlDocument
    //
    
    MSXML::IXMLDOMElementPtr GenerateCustomerReport (MSXML::IXMLDOMDocumentPtr &xmlDocument, 
                                                     size_t nCustomerIdx);

    //
    // ������ ����� �� ��������
    //

    MSXML::IXMLDOMElementPtr GenerateCounterReport (MSXML::IXMLDOMDocumentPtr &xmlDocument, 
                                                    size_t nCounterIdx);

    
    //
    // ������� XML ������� c ������� �� ����������� �������� ���������
    //

    MSXML::IXMLDOMElementPtr GenerateCounterPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                         size_t nCounterIdx);

    //
    // ������� XML ������� c ������� �� ����������� ������� ���������
    //

    MSXML::IXMLDOMElementPtr GenerateCounterEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                          size_t nCounterIdx);

    //
    // ������� XML ������� c �������� ������� ��������
    //

    MSXML::IXMLDOMElementPtr GenerateCounterEventLog (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                      size_t nCounterIdx);

    //
    // ������� XML ������� c ������� �� ����������������� �����������
    //

    MSXML::IXMLDOMElementPtr GenerateCustomerEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument);


    //
    // ������� XML ������� ������ ���������� ����������� ��������
    //

    MSXML::IXMLDOMElementPtr GenerateCustomerSummaryPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                 PowerUsageSummarizer &summaryReport);

    //
    // ������� XML ������� ������ ���������� ����������� �������� �� �������
    //

    MSXML::IXMLDOMElementPtr GenerateCustomerMonthlyPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                 const PowerUsageSummarizer::SummaryStorage &storage);

    //
    // ��������� � ptrParent ������� wcsName ��������� ������ pTime.
    // ��-��������� ������������ ������� ��������� �����.
    //

    static MSXML::IXMLDOMElementPtr CreateTimeStamp (MSXML::IXMLDOMElementPtr ptrParent,
                                                     const wchar_t *wcsName = L"time_stamp",
                                                     PSYSTEMTIME pTime = NULL);
    
    //
    // ������� �������, ����������� ����������� �������
    //

    static MSXML::IXMLDOMElementPtr CreateEnergyInfoElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                             const EnergyInfo &energyInfo,                                                             
                                                             const wchar_t *wcsName);

    //
    // ������� �������, ����������� ����������� ��������
    //

    static MSXML::IXMLDOMElementPtr CreatePowerInfoElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                            const PowerInfo &powerInfo,                                                            
                                                            const wchar_t *wcsName);

    //
    // ������� �������, ����������� ��������� ����������� ��������
    //

    static MSXML::IXMLDOMElementPtr CreateSummaryPowerEntryElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const PowerUsageSummarizer::SummaryEntry &entry,                                                                    
                                                                    unsigned nIdx,
                                                                    const wchar_t *wcsName);

    //
    // ������� XML ������� �������� ����������� ������������ ��������
    //

    static MSXML::IXMLDOMElementPtr CreateCounterDataElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                              const CounterData &counterData);

    //
    // ������� XML ������� ��������
    //

    MSXML::IXMLDOMElementPtr CreateCounterElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                   unsigned nCounterIdx,
                                                   const Customer::Counter &counter);

    //
    // ������� XML ������� c ������������ ������������ ��������
    //

    static MSXML::IXMLDOMElementPtr CreateMeteringDataElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const MeteringData &meteringData);


    //
    // ������� XML ������� c ������� �� ����������� �������� ���������
    //

    static MSXML::IXMLDOMElementPtr CreateCounterPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                             const MeteringData &meteringData);

    //
    // ������� XML ������� c ������� �� ����������� ������� ���������
    //

    static MSXML::IXMLDOMElementPtr CreateCounterEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const MeteringData &meteringData);

    //
    // ������� XML ������� c �������� ������� ��������
    //

    static MSXML::IXMLDOMElementPtr CreateCounterEventLog (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                           const MeteringData &meteringData);

    //
    //  ������� XML ������� �����������
    //

    static MSXML::IXMLDOMElementPtr CreateCustomerElement (MSXML::IXMLDOMDocumentPtr xmlDocument, 
                                                           size_t nCustomerIdx,
                                                           const Customer &customer);
    
    //
    // ������� XML ������� �������� ���������� �����������������
    //
    
    static MSXML::IXMLDOMElementPtr CreateEnergyReportElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const EnergyReport &rpt,
                                                               const wchar_t *wcsName);

    //
    // ������� XML ������� �������� ���������� ����������� ��������
    //

    static MSXML::IXMLDOMElementPtr CreateSummaryPowerElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const PowerUsageSummarizer::SummaryStorage &storage,
                                                               const wchar_t *wcsName);

    //
    // ������� XML ������� �������� ���������� ����������� �������� �� �����
    //

    static MSXML::IXMLDOMElementPtr CreateMonthlyPowerElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                               const MonthlyPowerReport &rpt,
                                                               const PowerUsageSummarizer::SummaryStorage &storage,
                                                               unsigned nMonth, unsigned nYear,
                                                               const wchar_t *wcsName);

protected:

};

#endif _MATA_MANAGER_XML_HPP_