///////////////////////////////////////////////////////////////////////////////
//
//  File:           DataManagerXML.сpp
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
* $History: DataManagerXML.cpp $
 * 
 * *****************  Version 27  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Manager
 * Подмешаны результаты работы не дома
 * 
 * *****************  Version 26  *****************
 * User: Sergey       Date: 19.01.07   Time: 21:56
 * Updated in $/PPCMercury.root/Manager
 * Исправлены мелкие ошибки
 * 
 * *****************  Version 25  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
* 
*/

#include "DataManagerXML.hpp"
#include "ReadLogEntry.hpp"
#include "EnergyReport.hpp"

//#define TEST_ENABLED

#ifdef TEST_ENABLED
#   define TEST_START {
#   define TEST_END }
#else
#   define TEST_START
#   define TEST_END
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DataManagerXML
//
//  Description:
//
//  Конструктор
//
///////////////////////////////////////////////////////////////////////////////

DataManagerXML::DataManagerXML ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~DataManagerXML
//
//  Description:
//
//  Деструктор
//
///////////////////////////////////////////////////////////////////////////////

DataManagerXML::~DataManagerXML ()
{
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

void DataManagerXML::GenerateAllReports ()
{

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateSummaryReport
//
//  Description:
//
//  Строит сумманый отчет. Возвращает элемент с отчетом. Элемент _не_ 
//  добавляется в документ xmlDocument
//
///////////////////////////////////////////////////////////////////////////////

 MSXML::IXMLDOMElementPtr DataManagerXML::GenerateSummaryReport (MSXML::IXMLDOMDocumentPtr &xmlDocument)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }    

    //
    // Элемент - контейнер отчета
    //

    size_t nCustomerCount = m_Customers.size ();
    CString strElemName = L"summary_report";
    
    tagAttribute customersAttributes [] = 
    {
        {_bstr_t (L"customer_count"), _variant_t ((unsigned) nCustomerCount)},
    };
    size_t nCustomersAttributeCount = SIZE_OF (customersAttributes);

    MSXML::IXMLDOMElementPtr ptrCustomers = 
        CreateXMLElement (xmlDocument,
                          strElemName,
                          nCustomersAttributeCount,
                          customersAttributes);
    if (NULL == ptrCustomers)
    {
        return NULL;
    }

    CreateTimeStamp (ptrCustomers);


    //
    // Элементы - потребители
    //

    for (size_t nCustomerIdx = 0; nCustomerIdx < nCustomerCount; ++ nCustomerIdx)
    {
        const Customer &customer = m_Customers [nCustomerIdx]; 

        MSXML::IXMLDOMElementPtr ptrCustomer = GenerateCustomerReport (xmlDocument, 
                                                                       nCustomerIdx);
        if (NULL == ptrCustomer)
        {
            return NULL;
        }
        HRESULT hr = ptrCustomers->appendChild (ptrCustomer);
        if (FAILED (hr))
        {
            return NULL;
        }
    }

    return ptrCustomers;    
}

 ///////////////////////////////////////////////////////////////////////////////
 //
 //  Function:       GenerateCustomerReport
 //
 //  Description:
 //
 //  Строит отчет по потребителю. Возвращает элемент с отчетом. Элемент _не_ 
 //  добавляется в документ xmlDocument
 //
 ///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCustomerReport (MSXML::IXMLDOMDocumentPtr &xmlDocument, 
                                                                 size_t nCustomerIdx)
{
    if (nCustomerIdx >= m_Customers.size ())
    {
        return NULL;
    }
    const Customer &customer = m_Customers [nCustomerIdx]; 

    MSXML::IXMLDOMElementPtr ptrCustomer = CreateCustomerElement (xmlDocument, 
                                                                  nCustomerIdx, 
                                                                  customer);
    if (NULL == ptrCustomer)
    {
        return NULL;
    }

    CreateTimeStamp (ptrCustomer);
    
    //
    // Элементы - счетчики
    //

    const Customer::CounterList &counters = customer.Counters (); 
    size_t nCounterCount = counters.size ();

    for (size_t nCounterIdx = 0; nCounterIdx < nCounterCount; ++ nCounterIdx)
    {
        const Customer::Counter &counter = counters [nCounterIdx];

        MSXML::IXMLDOMElementPtr ptrCounter = 
            CreateCounterElement (xmlDocument, 
                                  (unsigned) nCounterIdx, 
                                  counter);
        if (NULL == ptrCounter)
        {
            return NULL;
        }
        HRESULT hr = ptrCustomer->appendChild (ptrCounter);
        if (FAILED (hr))
        {
            return NULL;
        }
    }

    return ptrCustomer;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCounterReport
//
//  Description:
//
//  Строит отчет по счетчику. Возвращает элемент с отчетом. Элемент _не_ 
//  добавляется в документ xmlDocument
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCounterReport (MSXML::IXMLDOMDocumentPtr &xmlDocument, 
                                                                size_t nCounterIdx)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    const Customer::CounterList &counters = customer.Counters ();
    size_t nCounterCount = counters.size ();
    if (nCounterIdx >= nCounterCount)
    {
        return NULL;
    }
    const CounterData &counterData = m_CounterData [nCounterIdx];
    const Customer::Counter &counter = counterData.GetCounter ();


    //
    // Элемент - контейнер отчета
    //

    CString strElemName = L"counter_report";
    CString strBuffer;
    
    tagAttribute counterAttributes [] = 
    {
        {_bstr_t (L"serial_number"), _variant_t (counter.m_nSerialNumber)},
        {_bstr_t (L"type"),          _variant_t (counter.m_nProtocolType)},
        {_bstr_t (L"kTn"),           _variant_t (counter.m_nKn)},
        {_bstr_t (L"kTt"),           _variant_t (counter.m_nKt)},
        {_bstr_t (L"idx"),           _variant_t ((unsigned) nCounterIdx)},
    };
    size_t nCounterAttributeCount = SIZE_OF (counterAttributes);

    MSXML::IXMLDOMElementPtr ptrReport = 
        CreateXMLElement (xmlDocument,
                          strElemName,
                          nCounterAttributeCount,
                          counterAttributes);
    if (NULL == ptrReport)
    {
        return NULL;
    }
    CreateTimeStamp (ptrReport);

    //
    // Элемент - потребитель
    //

    MSXML::IXMLDOMElementPtr ptrCustomer = CreateCustomerElement (xmlDocument, 
                                                                  m_nCurrentCustomerIdx, 
                                                                  customer);
    if (NULL == ptrCustomer)
    {
        return NULL;
    }
    HRESULT hr = ptrReport->appendChild (ptrCustomer);
    if (FAILED (hr))
    {
        return NULL;
    }

    //
    // Элемент - счетчик
    //

    MSXML::IXMLDOMElementPtr ptrCounter = CreateCounterElement (xmlDocument, 
                                                                (unsigned) nCounterIdx, 
                                                                counter);
    if (NULL == ptrCounter)
    {
        return NULL;
    }
    hr = ptrReport->appendChild (ptrCounter);
    if (FAILED (hr))
    {
        return NULL;
    }
    
    //
    // Элемент - данные счетчика
    //

    MSXML::IXMLDOMElementPtr ptrCounterData = CreateCounterDataElement (xmlDocument, 
                                                                        counterData);
    if (NULL == ptrCounterData)
    {
        return NULL;
    }
    hr = ptrCounter->appendChild (ptrCounterData);
    if (FAILED (hr))
    {
        return NULL;
    }

    //
    // Элемент - результат сканирования счетчика
    //

    const MeteringData &meterinngData = counterData.GetData ();
    MSXML::IXMLDOMElementPtr ptrMeteringData = CreateMeteringDataElement (xmlDocument,
                                                                          meterinngData);
    if (NULL == ptrMeteringData)
    {
        return NULL;
    }
    hr = ptrCounterData->appendChild (ptrMeteringData);
    if (FAILED (hr))
    {
        return NULL;
    }

    return ptrReport;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCounterPowerReport
//
//  Description:
//
//  Создает XML элемент c отчетом по потреблению мощности счетчиком
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCounterPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                     size_t nCounterIdx)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    const Customer::CounterList &counters = customer.Counters ();
    size_t nCounterCount = counters.size ();
    if (nCounterIdx >= nCounterCount)
    {
        return NULL;
    }
    const CounterData &counterData = m_CounterData [nCounterIdx];
    const Customer::Counter &counter = counterData.GetCounter ();

    MSXML::IXMLDOMElementPtr ptrResult = CreateCounterPowerReport (xmlDocument, counterData.GetData ());
    if (ptrResult != NULL) CreateTimeStamp (ptrResult);

    return ptrResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCounterEnergyReport
//
//  Description:
//
//  Создает XML элемент c отчетом по потреблению энергии счетчиком
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCounterEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                      size_t nCounterIdx)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    const Customer::CounterList &counters = customer.Counters ();
    size_t nCounterCount = counters.size ();
    if (nCounterIdx >= nCounterCount)
    {
        return NULL;
    }
    const CounterData &counterData = m_CounterData [nCounterIdx];
    const Customer::Counter &counter = counterData.GetCounter ();

    MSXML::IXMLDOMElementPtr ptrResult = CreateCounterEnergyReport (xmlDocument, counterData.GetData ());
    if (ptrResult != NULL) CreateTimeStamp (ptrResult);

    return ptrResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCounterEventLog
//
//  Description:
//
//  Создает XML элемент c журналом событий счетчика
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCounterEventLog (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                  size_t nCounterIdx)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    const Customer::CounterList &counters = customer.Counters ();
    size_t nCounterCount = counters.size ();
    if (nCounterIdx >= nCounterCount)
    {
        return NULL;
    }
    const CounterData &counterData = m_CounterData [nCounterIdx];
    const Customer::Counter &counter = counterData.GetCounter ();

    MSXML::IXMLDOMElementPtr ptrResult = CreateCounterEventLog (xmlDocument, counterData.GetData ());
    if (ptrResult != NULL) CreateTimeStamp (ptrResult);

    return ptrResult;
}  

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateCounterDataElement
//
//  Description:
//
//  Создает XML элемент описания результатов сканирования счетчика
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateCounterDataElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                   const CounterData &counterData)
{
    tagAttribute counterDataAttributes [] = 
    {
        {_bstr_t (L"state_code"), _variant_t (counterData.GetState ())},
        {_bstr_t (L"state_text"), _variant_t (counterData.GetStateText ())},
        {_bstr_t (L"valid"),      _variant_t (counterData.IsValid ())},
        {_bstr_t (L"valid"),      _variant_t (counterData.IsValid ())},

    };
    size_t nCounterDataAttributeCount = SIZE_OF (counterDataAttributes);

    CString strCounterDataTag;
    strCounterDataTag.Format (L"counter_data");
    _bstr_t bstrCounterDataTag (strCounterDataTag);

    MSXML::IXMLDOMElementPtr ptrCounterData = 
        CreateXMLElement (xmlDocument,
                          bstrCounterDataTag, 
                          nCounterDataAttributeCount,
                          counterDataAttributes);   
    return ptrCounterData;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateCustomerElement
//
//  Description:
//
//  Создает XML элемент потребителя.
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateCustomerElement (MSXML::IXMLDOMDocumentPtr xmlDocument, 
                                                                size_t nCustomerIdx,
                                                                const Customer &customer)
{
    const Customer::CounterList &counters = customer.Counters ();

    size_t nCounterCount = counters.size ();
    tagAttribute customerAttributes [] = 
    {
        {_bstr_t (L"name"), _variant_t (customer.GetName ())},
        {_bstr_t (L"limit"), _variant_t (customer.GetPowerLimit ())},
        {_bstr_t (L"idx"), _variant_t ((unsigned) nCustomerIdx)},
        {_bstr_t (L"counter_count"), _variant_t ((unsigned) nCounterCount)},
    };
    size_t nCustomerAttributeCount = SIZE_OF (customerAttributes);

    CString strCustomerTag;
    strCustomerTag.Format (L"customer");
    _bstr_t bstrCustomerTag (strCustomerTag);

    MSXML::IXMLDOMElementPtr ptrCustomer = 
        CreateXMLElement (xmlDocument,
                          bstrCustomerTag,
                          nCustomerAttributeCount,
                          customerAttributes);   
    return ptrCustomer;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateCounterElement
//
//  Description:
//
//  Создает XML элемент счетчика.
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateCounterElement (MSXML::IXMLDOMDocumentPtr xmlDocument, 
                                                               size_t nCounterIdx,
                                                               const Customer::Counter &counter)
{
    tagAttribute counterAttributes [] = 
    {
        {_bstr_t (L"serial_number"), _variant_t (counter.m_nSerialNumber)},
        {_bstr_t (L"type"),          _variant_t (counter.m_nProtocolType)},
        {_bstr_t (L"kTn"),           _variant_t (counter.m_nKn)},
        {_bstr_t (L"kTt"),           _variant_t (counter.m_nKt)},
        {_bstr_t (L"idx"),           _variant_t ((unsigned) nCounterIdx)},
    };
    size_t nCounterAttributeCount = SIZE_OF (counterAttributes);

    CString strCounterTag;
    strCounterTag.Format (L"counter");
    _bstr_t bstrCounterTag (strCounterTag);

    MSXML::IXMLDOMElementPtr ptrCounter = 
        CreateXMLElement (xmlDocument,
                          bstrCounterTag, 
                          nCounterAttributeCount,
                          counterAttributes);
    if (ptrCounter == NULL)
    {
        return NULL;
    }

    const CounterDataList &counterDataList = GetCounterData ();
    size_t nCounterCount = counterDataList.size ();
    if (nCounterIdx < nCounterCount)
    {
        const CounterData &counterData = counterDataList [nCounterIdx];
        const MeteringData &data = counterData.GetData ();

        const PowerStorage &ps= data.GetPowerStorage ();
        if (FAILED (ptrCounter->setAttribute (L"power", ps.size () > 0))) return NULL;
        
        const MeteringData::DateTime &dt = data.GetCounterTime ();
        EnergyReport rpt (data.GetEnergy () [0], dt, 1, 1);
        if (FAILED (ptrCounter->setAttribute (L"energy", !rpt.IsEmpty ()))) return NULL;
        
        if (FAILED (ptrCounter->setAttribute (L"state", counterData.GetState ()))) return NULL;
        if (FAILED (ptrCounter->setAttribute (L"state_text", counterData.GetStateText ()))) return NULL;
        
    }

    return ptrCounter;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateMeteringDataElement
//
//  Description:
//
//  Создает XML элемент c результатами сканирования счетчика
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateMeteringDataElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const MeteringData &meteringData)
{
    /*
    <metering_data version=1 protocol_type=2>

        <net_address valid=true>
            1
        </net_address>
        <counter_time valid=true day=1 month=1 year=2000 hour=5 minute=10 winter=0/>
        <location valid=true>
            "Положение счетчика"
        </location>
        <kTn valid=true>
            1
        </location>
        <kTt valid=true>
            2
        </location>
        <serial_number valid=true>
            12345
        </location>
        <production_time valid=true day=1 month=1 year=2000 hour=5 minute=10/>
        <production_type/>
        <firmware valid=true>
            "Firmware version"
        </firmware>
        <energy tariff_count=5>
            <tariff index=0>
                <from_reset/>
                <this_year/>
                <last_year/>
                <today/>
                <yesterday/>
                <months entry_count=12>
                    <motnth/>
                </months>
            </tariff>
        </energy>
        <log entry_count=10>
            <log_entry index=0 type=0 name="Название">
            </tariff>
        </log>
        <power entry_count=10>
            <power_entry index=0>
            </power_entry>
        </power>
        
    </metering_data>
    */

    //
    // metering_data element
    //

    TEST_START
    tagAttribute dataTagAttr [] = 
    {
        {_bstr_t (L"version"),       _variant_t (meteringData.GetVersion ())},
        {_bstr_t (L"protocol_type"), _variant_t (meteringData.GetProtocolType ())},
    };
    size_t nDataTagAttrCount = SIZE_OF (dataTagAttr);
    _bstr_t bstrDataTag (L"metering_data");
    MSXML::IXMLDOMElementPtr ptrMeteringData;
    TEST_END
    
    //
    // net_address element
    //
    
    TEST_START
    tagAttribute netAddressTagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.IsNetAddressValid ())},
        {_bstr_t (L"value"), _variant_t ((long) meteringData.GetNetAddress ())},
    };
    size_t nNetAddressAttrCount = SIZE_OF (netAddressTagAttr);
    _bstr_t bstrNetAddressTag (L"net_address");
    MSXML::IXMLDOMElementPtr ptrNetAddress;
    TEST_END

    //
    // counter_time element
    //
    
    TEST_START
    const MeteringData::DateTime &counterDateTime = meteringData.GetCounterTime ();
    tagAttribute counterTimeTagAttr [] = 
    {
        {_bstr_t (L"valid"),   _variant_t (meteringData.IsCounterTimeValid ())},
        {_bstr_t (L"day"),     _variant_t (counterDateTime.GetDay ())},
        {_bstr_t (L"month"),   _variant_t (counterDateTime.GetMonth ())},
        {_bstr_t (L"year"),    _variant_t (counterDateTime.GetYear ())},
        {_bstr_t (L"hour"),    _variant_t (counterDateTime.GetHour ())},
        {_bstr_t (L"minute"),  _variant_t (counterDateTime.GetMinute ())},
        {_bstr_t (L"winter"),  _variant_t (meteringData.IsWinter ())},
    };
    size_t nCounterTimeAttrCount = SIZE_OF (counterTimeTagAttr);
    _bstr_t bstrCounterTimeTag (L"counter_time");
    MSXML::IXMLDOMElementPtr ptrCounterTime;
    TEST_END

    //
    // location element
    //
    
    TEST_START
    tagAttribute locationTagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.IsLocationValid ())},
        {_bstr_t (L"value"), _variant_t (meteringData.GetLocation ())},
    };
    size_t nLocationAttrCount = SIZE_OF (locationTagAttr);
    _bstr_t bstrLocationTag (L"location");
    MSXML::IXMLDOMElementPtr ptrLocation;
    TEST_END

    //
    // kTn element
    //
    
    TEST_START
    tagAttribute kTnTagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.IsKnValid ())},
        {_bstr_t (L"value"), _variant_t (meteringData.GetKn ())},
    };
    size_t nKTnAttrCount = SIZE_OF (kTnTagAttr);
    _bstr_t bstrKTnTag (L"kTn");
    MSXML::IXMLDOMElementPtr ptrKTn;
    TEST_END

    //
    // kTt element
    //

    TEST_START
    tagAttribute kTtTagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.IsKtValid ())},
        {_bstr_t (L"value"), _variant_t (meteringData.GetKt ())},
    };
    size_t nKTtAttrCount = SIZE_OF (kTtTagAttr);
    _bstr_t bstrKTtTag (L"kTt");
    MSXML::IXMLDOMElementPtr ptrKTt;
    TEST_END
    
    //
    // serial_number element
    //

    TEST_START
    tagAttribute serialNumberTagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.IsSerialNumberValid ())},
        {_bstr_t (L"value"), _variant_t (meteringData.GetSerialNumber ())},
    };
    size_t nSerialNumberAttrCount = SIZE_OF (serialNumberTagAttr);
    _bstr_t bstrSerialNumberTag (L"serial_number");
    MSXML::IXMLDOMElementPtr ptrSerialNumber;
    TEST_END
 
    //
    // production_time element
    //

    TEST_START
    const MeteringData::DateTime &productionTime = meteringData.GetProductionDate ();
    tagAttribute productionTimeTagAttr [] = 
    {
        {_bstr_t (L"valid"),   _variant_t (meteringData.IsSerialNumberValid ())},
        {_bstr_t (L"day"),     _variant_t (productionTime.GetDay ())},
        {_bstr_t (L"month"),   _variant_t (productionTime.GetMonth ())},
        {_bstr_t (L"year"),    _variant_t (productionTime.GetYear ())},
        {_bstr_t (L"hour"),    _variant_t (productionTime.GetHour ())},
        {_bstr_t (L"minute"),  _variant_t (productionTime.GetMinute ())},
    };
    size_t nProductionTimeAttrCount = SIZE_OF (productionTimeTagAttr);
    _bstr_t bstrProductionTimeTag (L"production_time");
    MSXML::IXMLDOMElementPtr ptrProductionTime;
    TEST_END

    //
    // production_type element
    //
   
    TEST_START
    const ProductionType &productionType = meteringData.GetProductionType ();
    double dCounterConst = productionType.CounterConst1 ();
    if (meteringData.GetProtocolType () == Mercury::ptMercury)  
    {
        dCounterConst = productionType.CounterConst2 ();
    }

    tagAttribute productionTypeTagAttr [] = 
    {
        {_bstr_t (L"valid"),               _variant_t (meteringData.IsProductionTypeValid ())},
        {_bstr_t (L"active_precision"),    _variant_t (productionType.ActivePrecision ())},
        {_bstr_t (L"reactive_precision"),  _variant_t (productionType.ReactivePrecision ())},
        {_bstr_t (L"voltage"),             _variant_t (productionType.Voltage ())},
        {_bstr_t (L"current"),             _variant_t (productionType.Current ())},
        {_bstr_t (L"direction_count"),     _variant_t (productionType.DirectionCount ())},
        {_bstr_t (L"temperature_range"),   _variant_t (productionType.TemperatureRange ())},
        {_bstr_t (L"phase_count"),         _variant_t (productionType.PhaseCount ())},
        {_bstr_t (L"counter_const"),       _variant_t (dCounterConst)},
    };
    size_t nProductionTypeAttrCount = SIZE_OF (productionTypeTagAttr);
    _bstr_t bstrProductionTypeTag (L"production_type");
    MSXML::IXMLDOMElementPtr ptrProductionType;
    TEST_END

    //
    // firmware element
    //

    TEST_START
    tagAttribute firmwareTagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.IsSoftwareVersionValid ())},
        {_bstr_t (L"value"), _variant_t (meteringData.GetSoftwareVersion ())},
    };
    size_t nFirmwareAttrCount = SIZE_OF (firmwareTagAttr);
    _bstr_t bstrFirmwareTag (L"firmware");
    MSXML::IXMLDOMElementPtr ptrFirmware;
    TEST_END

    //
    // energy element
    //

    TEST_START

    EnergyReport tmpReport (meteringData.GetEnergy () [0], counterDateTime, 1, 1);
    bool bEnergyEmpty = tmpReport.IsEmpty ();
    tagAttribute energyTagAttr [] = 
    {
        {_bstr_t (L"tariff_count"), _variant_t (TARIFF_COUNT)},
        {_bstr_t (L"empty"),        _variant_t (bEnergyEmpty)},

    };
    size_t nEnergyAttrCount = SIZE_OF (energyTagAttr);
    _bstr_t bstrEnergyTag (L"energy");
    MSXML::IXMLDOMElementPtr ptrEnergy;
    TEST_END

    //
    // log element
    //

    TEST_START
    tagAttribute logTagAttr [] = 
    {
        {_bstr_t (L"entry_count"), _variant_t ((long) Mercury::ReadLogEntry::letMaxValue)}
    };
    size_t nLogAttrCount = SIZE_OF (logTagAttr);
    _bstr_t bstrLogTag (L"log");
    MSXML::IXMLDOMElementPtr ptrLog;
    TEST_END

    //
    // power element
    //

    TEST_START
    const PowerStorage& powerStorage =  meteringData.GetPowerStorage ();
    size_t nPowerCount = powerStorage.size ();

    MeteringData::DateTime powerFrom;
    MeteringData::DateTime powerTo;
    if (nPowerCount > 0)
    {
        const PowerInfo &first = powerStorage [0];
        powerFrom.SetYear (first.nYear);
        powerFrom.SetMonth (first.nMonth);
        powerFrom.SetDay (first.nDay);
        powerFrom.SetHour (first.nHour);
        powerFrom.SetMinute (first.nMinute);

        const PowerInfo &second = powerStorage [nPowerCount - 1];
        powerTo.SetYear (second.nYear);
        powerTo.SetMonth (second.nMonth);
        powerTo.SetDay (second.nDay);
        powerTo.SetHour (second.nHour);
        powerTo.SetMinute (second.nMinute);
    }
 
    tagAttribute powerTagAttr [] = 
    {
        {_bstr_t (L"entry_count"), _variant_t ((long) nPowerCount)},

        {_bstr_t (L"from_day"),     _variant_t (powerFrom.GetDay ())},
        {_bstr_t (L"from_month"),   _variant_t (powerFrom.GetMonth ())},
        {_bstr_t (L"from_year"),    _variant_t (powerFrom.GetYear ())},
        {_bstr_t (L"from_hour"),    _variant_t (powerFrom.GetHour ())},
        {_bstr_t (L"from_minute"),  _variant_t (powerFrom.GetMinute ())},

        {_bstr_t (L"to_day"),     _variant_t (powerTo.GetDay ())},
        {_bstr_t (L"to_month"),   _variant_t (powerTo.GetMonth ())},
        {_bstr_t (L"to_year"),    _variant_t (powerTo.GetYear ())},
        {_bstr_t (L"to_hour"),    _variant_t (powerTo.GetHour ())},
        {_bstr_t (L"to_minute"),  _variant_t (powerTo.GetMinute ())},

        //{_bstr_t (L"valid"),       _variant_t (counterData.IsValid ())},
    };
    size_t nPowerAttrCount = SIZE_OF (powerTagAttr);
    _bstr_t bstrPowerTag (L"power");
    MSXML::IXMLDOMElementPtr ptrPower;
    TEST_END

/*
    tagAttribute <>TagAttr [] = 
    {
        {_bstr_t (L"valid"), _variant_t (meteringData.Is<>Valid ()))}
    };
    size_t <>AttrCount = sizeof (<>TagAttr) / sizeof (<>TagAttr [0]);
    _bstr_t bstr<>Tag (L"<>");
    _variant_t <>Value = meteringData.Get<> ();
    IXMLDOMElementPtr ptr<>;
 */

#ifndef TEST_ENABLED   
    MSXML::IXMLDOMElementPtr ptrNULL;
    tagEntity <_bstr_t, tagAttribute*> xmlElements [] =
    {
    //  Name                   Parent element   New element        Attribute count           Attributes
        {bstrDataTag,           ptrNULL,         ptrMeteringData,   nDataTagAttrCount,        dataTagAttr},
        {bstrNetAddressTag,     ptrMeteringData, ptrNetAddress,     nNetAddressAttrCount,     netAddressTagAttr},
        {bstrCounterTimeTag,    ptrMeteringData, ptrCounterTime,    nCounterTimeAttrCount,    counterTimeTagAttr},
        {bstrLocationTag,       ptrMeteringData, ptrLocation,       nLocationAttrCount,       locationTagAttr},
        {bstrKTnTag,            ptrMeteringData, ptrKTn,            nKTnAttrCount,            kTnTagAttr},
        {bstrKTtTag,            ptrMeteringData, ptrKTt,            nKTtAttrCount,            kTtTagAttr},
        {bstrSerialNumberTag,   ptrMeteringData, ptrSerialNumber,   nSerialNumberAttrCount,   serialNumberTagAttr},
        {bstrProductionTimeTag, ptrMeteringData, ptrProductionTime, nProductionTimeAttrCount, productionTimeTagAttr},
        {bstrProductionTypeTag, ptrMeteringData, ptrProductionType, nProductionTypeAttrCount, productionTypeTagAttr},
        {bstrFirmwareTag,       ptrMeteringData, ptrFirmware,       nFirmwareAttrCount,       firmwareTagAttr},
        {bstrEnergyTag,         ptrMeteringData, ptrEnergy,         nEnergyAttrCount,         energyTagAttr},
        {bstrLogTag,            ptrMeteringData, ptrLog,            nLogAttrCount,            logTagAttr},
        {bstrPowerTag,          ptrMeteringData, ptrPower,          nPowerAttrCount,          powerTagAttr},
    };

    //
    // Создаем основные элементы
    //

    for (size_t nElemIdx = 0; nElemIdx < SIZE_OF (xmlElements); ++ nElemIdx)
    {
        if (! xmlElements [nElemIdx].CreateXMLElement (xmlDocument))
        {
            return NULL;
        }
    }
#endif //TEST_ENABLED
   
    return ptrMeteringData;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateEnergyInfoElement
//
//  Description:
//
//  Создает элемент, описывающий потребление энергии
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateEnergyInfoElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                  const EnergyInfo &energyInfo,
                                                                  const wchar_t *wcsName)
{
    tagAttribute attributes [] = 
    {
        {_bstr_t (L"valid"), _variant_t (energyInfo.bValid)},
        {_bstr_t (L"empty"), _variant_t (energyInfo.IsEmpty ())},
        {_bstr_t (L"a_d"),   _variant_t (energyInfo.nActiveDirect)},
        {_bstr_t (L"a_r"),   _variant_t (energyInfo.nActiveReverce)},
        {_bstr_t (L"r_d"),   _variant_t (energyInfo.nReactiveDirect)},
        {_bstr_t (L"r_r"),   _variant_t (energyInfo.nReactiveReverce)},
    };

    unsigned nCount = SIZE_OF (attributes);
    MSXML::IXMLDOMElementPtr ptrResult = CreateXMLElement (xmlDocument, 
                                                           wcsName, 
                                                           nCount, 
                                                           attributes);
    return ptrResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreatePowerInfoElement
//
//  Description:
//
//  Создает элемент, описывающий потребление мощности
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreatePowerInfoElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                 const PowerInfo &powerInfo,                                                                 
                                                                 const wchar_t *wcsName)
{

    tagAttribute attributes [] = 
    {
        {_bstr_t (L"h"),       _variant_t (powerInfo.nHour)},
        {_bstr_t (L"m"),       _variant_t (powerInfo.nMinute)},
        {_bstr_t (L"d"),       _variant_t (powerInfo.nDay)},
        {_bstr_t (L"mm"),      _variant_t (powerInfo.nMonth)},
        {_bstr_t (L"y"),       _variant_t (powerInfo.nYear)},
        {_bstr_t (L"p_p"),     _variant_t (powerInfo.nPPlus)},
        {_bstr_t (L"p_m"),     _variant_t (powerInfo.nPMinus)},
        {_bstr_t (L"q_p"),     _variant_t (powerInfo.nQPlus)},
        {_bstr_t (L"q_m"),     _variant_t (powerInfo.nQMinus)},
        {_bstr_t (L"st"),      _variant_t (powerInfo.nState)},
        {_bstr_t (L"t"),       _variant_t (powerInfo.nT)},
    };

    unsigned nCount = SIZE_OF (attributes);
    MSXML::IXMLDOMElementPtr ptrResult = CreateXMLElement (xmlDocument, 
                                                           wcsName, 
                                                           nCount, 
                                                           attributes);
    return ptrResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateSummaryEntryElement
//
//  Description:
//
//  Создает элемент, описывающий суммарное потребление мощности
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateSummaryPowerEntryElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                         const PowerUsageSummarizer::SummaryEntry &entry,                                                                    
                                                                         unsigned nIdx,
                                                                         const wchar_t *wcsName)
{
    const MeteringData::DateTime &dt = entry.first;
    const PowerUsage::UsageStorage &usageStorage = entry.second.GetStorage ();
    unsigned nCounterCount = (unsigned) usageStorage.size ();

    PowerInfo summaryUsage;
    entry.second.CreateSummaryUsage (summaryUsage);

    tagAttribute summaryEntryAttributes [] = 
    {
        {_bstr_t (L"count"),       _variant_t (nCounterCount)},
        {_bstr_t (L"idx"),         _variant_t (nIdx)},
        {_bstr_t (L"m"),           _variant_t (dt.GetMinute ())},
        {_bstr_t (L"h"),           _variant_t (dt.GetHour ())},
        {_bstr_t (L"d"),           _variant_t (dt.GetDay ())},
        {_bstr_t (L"mm"),          _variant_t (dt.GetMonth ())},
        {_bstr_t (L"y"),           _variant_t (dt.GetYear ())},
        {_bstr_t (L"p_p"),         _variant_t (summaryUsage.nPPlus)},
        {_bstr_t (L"p_m"),         _variant_t (summaryUsage.nPMinus)},
        {_bstr_t (L"q_p"),         _variant_t (summaryUsage.nQPlus)},
        {_bstr_t (L"q_m"),         _variant_t (summaryUsage.nQMinus)},
    };

    unsigned nSummaryEntryAttrCount = SIZE_OF (summaryEntryAttributes);
    
    MSXML::IXMLDOMElementPtr ptrSummaryEntry = 
        CreateXMLElement (xmlDocument, 
                          wcsName, 
                          nSummaryEntryAttrCount,
                          summaryEntryAttributes);
    if (ptrSummaryEntry == NULL) return NULL;
    
    for (unsigned nCounterIdx = 0; nCounterIdx < nCounterCount; ++ nCounterIdx)
    {
        const PowerUsage::UsageEntry &entry = usageStorage [nCounterIdx];
        if (NULL == entry.first) continue;

        PowerInfo &powerInfo = *entry.first;
        tagAttribute attributes [] = 
        {
            {_bstr_t (L"cnt"),     _variant_t (entry.second)},
            {_bstr_t (L"p_p"),     _variant_t (powerInfo.nPPlus)},
            {_bstr_t (L"p_m"),     _variant_t (powerInfo.nPMinus)},
            {_bstr_t (L"q_p"),     _variant_t (powerInfo.nQPlus)},
            {_bstr_t (L"q_m"),     _variant_t (powerInfo.nQMinus)},
            {_bstr_t (L"st"),      _variant_t (powerInfo.nState)},
            {_bstr_t (L"t"),       _variant_t (powerInfo.nT)},
        };

        unsigned nCount = SIZE_OF (attributes);
        MSXML::IXMLDOMElementPtr ptrUsageEntry = CreateXMLElement (xmlDocument, 
                                                                   L"pi", 
                                                                   nCount, 
                                                                   attributes);
        if (ptrUsageEntry == NULL) return NULL;         
        if (FAILED (ptrSummaryEntry->appendChild (ptrUsageEntry))) return NULL;
    }

    return ptrSummaryEntry;
}



///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateEnergyReportElement
//
//  Description:
//
//  Создает XML элемент описания суммарного энергопотребления
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateEnergyReportElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const EnergyReport &rpt,
                                                                    const wchar_t *wcsName)
{
    MeteringData::DateTime firstDate;
    MeteringData::DateTime lastDate;
    rpt.GetScanDate (lastDate);
    const EnergyReport::ReportStorage &rs = rpt.GetStorage ();
    unsigned nCount = (unsigned) rs.size ();
    if (nCount > 0)
    {
        firstDate = rs [0].first;
    }

    tagAttribute attributes [] = 
    {
        {_bstr_t (L"empty"),       _variant_t (rpt.IsEmpty ())},
        {_bstr_t (L"first_month"), _variant_t (firstDate.GetMonth ())},
        {_bstr_t (L"first_year"),  _variant_t (firstDate.GetYear ())},
        {_bstr_t (L"last_month"),  _variant_t (lastDate.GetMonth ())},
        {_bstr_t (L"last_year"),   _variant_t (lastDate.GetYear ())},
    };

    unsigned nAttrCount = SIZE_OF (attributes);
    MSXML::IXMLDOMElementPtr ptrReport = CreateXMLElement (xmlDocument, 
                                                           wcsName, 
                                                           nAttrCount, 
                                                           attributes);
    if (ptrReport == NULL) return NULL;
    
    {
        MSXML::IXMLDOMElementPtr ptrThisYear = CreateEnergyInfoElement (xmlDocument,
                                                                        rpt.ThisYear (), 
                                                                        L"this_year");
        if (ptrThisYear == NULL) return NULL;
        if (FAILED (ptrReport->appendChild (ptrThisYear))) return NULL;
    }

    {
        MSXML::IXMLDOMElementPtr ptrLastYear = CreateEnergyInfoElement (xmlDocument,
                                                                        rpt.LastYear (),
                                                                        L"last_year");
        if (ptrLastYear == NULL) return NULL;
        if (FAILED (ptrReport->appendChild (ptrLastYear))) return NULL;
    }

    {
        MSXML::IXMLDOMElementPtr ptrFromReset = CreateEnergyInfoElement (xmlDocument,
                                                                         rpt.FromReset (), 
                                                                         L"from_reset");
        if (ptrFromReset == NULL) return NULL;
        if (FAILED (ptrReport->appendChild (ptrFromReset))) return NULL;
    }

    
    tagAttribute storageAttributes [] = 
    {
        {_bstr_t (L"entry_count"), _variant_t (nCount)},
    };

    unsigned nStorageCount = SIZE_OF (storageAttributes);
    MSXML::IXMLDOMElementPtr ptrStorage = CreateXMLElement (xmlDocument, 
                                                            L"months", 
                                                            nStorageCount, 
                                                            storageAttributes);
    if (ptrStorage == NULL) return NULL;
    if (FAILED (ptrReport->appendChild (ptrStorage))) return NULL;

    for (unsigned nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const EnergyReport::ReportEntry &entry = rs [nIdx];
        MSXML::IXMLDOMElementPtr ptrMonth = CreateEnergyInfoElement (xmlDocument,
                                                                     entry.second, 
                                                                     L"month");
        if (ptrMonth == NULL) return NULL;
        
        if (FAILED (ptrMonth->setAttribute (L"index", nIdx))) return NULL;
        if (FAILED (ptrMonth->setAttribute (L"month", entry.first.GetMonth ()))) return NULL;
        if (FAILED (ptrMonth->setAttribute (L"year", entry.first.GetYear ()))) return NULL;

        if (FAILED (ptrStorage->appendChild (ptrMonth))) return NULL;

    }

    return ptrReport;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreatePowerSummaryElement
//
//  Description:
//
//  Создает XML элемент описания суммарного потребления мощности
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateSummaryPowerElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const PowerUsageSummarizer::SummaryStorage &storage,
                                                                    const wchar_t *wcsName)
{
    MeteringData::DateTime firstDate;
    MeteringData::DateTime lastDate;
    
    unsigned nCount = (unsigned) storage.size ();
    if (nCount > 0)
    {
        firstDate = storage [0].first;
        lastDate = storage [nCount - 1].first;
    }

    tagAttribute attributes [] = 
    {
        {_bstr_t (L"count"),       _variant_t (nCount)},
        {_bstr_t (L"first_month"), _variant_t (firstDate.GetMonth ())},
        {_bstr_t (L"first_year"),  _variant_t (firstDate.GetYear ())},
        {_bstr_t (L"last_month"),  _variant_t (lastDate.GetMonth ())},
        {_bstr_t (L"last_year"),   _variant_t (lastDate.GetYear ())},
    };

    unsigned nAttrCount = SIZE_OF (attributes);
    MSXML::IXMLDOMElementPtr ptrReport = 
        CreateXMLElement (xmlDocument, 
                          wcsName, 
                          nAttrCount, 
                          attributes);
    if (ptrReport == NULL) return NULL;

    for (unsigned nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const PowerUsageSummarizer::SummaryEntry &entry = storage [nIdx];
        MSXML::IXMLDOMElementPtr ptrSummaryEntry = CreateSummaryPowerEntryElement (xmlDocument, 
                                                                                   entry, 
                                                                                   nIdx, 
                                                                                   L"se");
        if (ptrSummaryEntry == NULL) return NULL;
        if (FAILED (ptrReport->appendChild (ptrSummaryEntry))) return NULL;
    }

    return ptrReport;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateCounterPowerReport
//
//  Description:
//
//  Создает XML элемент c отчетом по потреблению мощности счетчиком
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateCounterPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                   const MeteringData &meteringData)
{
    const ProductionType &productionType = meteringData.GetProductionType ();
    double dCounterConst = productionType.CounterConst1 ();
    if (meteringData.GetProtocolType () == Mercury::ptMercury)  
    {
        dCounterConst = productionType.CounterConst2 ();
    }

    //
    // power element
    //

    const PowerStorage& powerStorage =  meteringData.GetPowerStorage ();
    size_t nPowerCount = powerStorage.size ();

    MeteringData::DateTime powerFrom;
    MeteringData::DateTime powerTo;
    if (nPowerCount > 0)
    {
        const PowerInfo &first = powerStorage [0];
        powerFrom.SetYear (first.nYear);
        powerFrom.SetMonth (first.nMonth);
        powerFrom.SetDay (first.nDay);
        powerFrom.SetHour (first.nHour);
        powerFrom.SetMinute (first.nMinute);

        const PowerInfo &second = powerStorage [nPowerCount - 1];
        powerTo.SetYear (second.nYear);
        powerTo.SetMonth (second.nMonth);
        powerTo.SetDay (second.nDay);
        powerTo.SetHour (second.nHour);
        powerTo.SetMinute (second.nMinute);
    }
 
    tagAttribute powerTagAttr [] = 
    {
        {_bstr_t (L"entry_count"), _variant_t ((long) nPowerCount)},

        {_bstr_t (L"from_day"),     _variant_t (powerFrom.GetDay ())},
        {_bstr_t (L"from_month"),   _variant_t (powerFrom.GetMonth ())},
        {_bstr_t (L"from_year"),    _variant_t (powerFrom.GetYear ())},
        {_bstr_t (L"from_hour"),    _variant_t (powerFrom.GetHour ())},
        {_bstr_t (L"from_minute"),  _variant_t (powerFrom.GetMinute ())},

        {_bstr_t (L"to_day"),     _variant_t (powerTo.GetDay ())},
        {_bstr_t (L"to_month"),   _variant_t (powerTo.GetMonth ())},
        {_bstr_t (L"to_year"),    _variant_t (powerTo.GetYear ())},
        {_bstr_t (L"to_hour"),    _variant_t (powerTo.GetHour ())},
        {_bstr_t (L"to_minute"),  _variant_t (powerTo.GetMinute ())},

        //{_bstr_t (L"valid"),       _variant_t (counterData.IsValid ())},
    };
    size_t nPowerAttrCount = SIZE_OF (powerTagAttr);
    _bstr_t bstrPowerTag (L"power");
    MSXML::IXMLDOMElementPtr ptrPower = CreateXMLElement (xmlDocument, 
                                                          bstrPowerTag, 
                                                          nPowerAttrCount, 
                                                          powerTagAttr);
    if (ptrPower == NULL) return NULL;

    //
    // Мощность
    //

    for (size_t nIdx = 0; nIdx < nPowerCount; ++ nIdx)
    {
        PowerInfo powerInfo = powerStorage [nIdx];
        int k = meteringData.GetKn () * meteringData.GetKt ();
        powerInfo.Normalize (k, dCounterConst);

        MSXML::IXMLDOMElementPtr ptrPowerInfo = 
            CreatePowerInfoElement (xmlDocument,
                                    powerInfo,
                                    L"pe");
        if (ptrPowerInfo == NULL)
        {
            return NULL;
        }

        HRESULT hr = ptrPower->appendChild (ptrPowerInfo);
        if (FAILED (hr))
        {
            return NULL;
        }
    }

    return ptrPower;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateCounterEnergyReport
//
//  Description:
//
//  Создает XML элемент c отчетом по потреблению энергии счетчиком
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateCounterEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const MeteringData &meteringData)
{
    //
    // energy element
    //

    const MeteringData::DateTime &counterDateTime = meteringData.GetCounterTime ();
    EnergyReport tmpReport (meteringData.GetEnergy () [0], counterDateTime, 1, 1);
    bool bEnergyEmpty = tmpReport.IsEmpty ();
    tagAttribute energyTagAttr [] = 
    {
        {_bstr_t (L"tariff_count"), _variant_t (TARIFF_COUNT)},
        {_bstr_t (L"empty"),        _variant_t (bEnergyEmpty)},

    };
    size_t nEnergyAttrCount = SIZE_OF (energyTagAttr);
    _bstr_t bstrEnergyTag (L"energy");
    MSXML::IXMLDOMElementPtr ptrEnergy = CreateXMLElement (xmlDocument, 
                                                           bstrEnergyTag, 
                                                           nEnergyAttrCount, 
                                                           energyTagAttr);
    if (ptrEnergy == NULL) return NULL;
    
    //
    // energy report
    //

    if (! bEnergyEmpty)
    {
        const AllEnergyInfo  *pAllEnergyInfo = meteringData.GetEnergy ();
        for (size_t nTariff = 0; nTariff < TARIFF_COUNT; ++ nTariff)
        {
            const AllEnergyInfo &tariff = pAllEnergyInfo [nTariff];

            //
            // tariff element
            //

            TEST_START
            tagAttribute tariffTagAttr [] = 
            {
                {_bstr_t (L"idx"), _variant_t ((unsigned) nTariff)},
                {_bstr_t (L"valid"), _variant_t (tariff.IsValid ())},
                {_bstr_t (L"empty"), _variant_t (tariff.IsEmpty ())},
            };

            size_t nTariffAttrCount = SIZE_OF (tariffTagAttr);
            _bstr_t bstrTariffTag (L"tariff");
            MSXML::IXMLDOMElementPtr ptrTariff;
            tagEntity <_bstr_t, tagAttribute*> xmlTariffElement =
            {
                bstrTariffTag,
                ptrEnergy, 
                ptrTariff,
                nTariffAttrCount,
                tariffTagAttr
            };

            if (! xmlTariffElement.CreateXMLElement (xmlDocument))
            {
                return NULL;
            }
            if (! tariff.IsValid () || (tariff.IsEmpty ()))
            {
                continue;
            }

            TEST_END

            struct tagEnergyInfo
            {
                const wchar_t    *wcsNаme;
                const EnergyInfo &enargyInfo;
            };

            tagEnergyInfo allEnergyElement [] =
            {
                {L"from_reset", tariff.GetEnergyFromReset ()},
                {L"this_year",  tariff.GetEnergyThisYear ()},
                {L"last_year",  tariff.GetEnergyLastYear ()},
                {L"today",      tariff.GetEnergyToday ()},
                {L"yesterday",  tariff.GetEnergyYesterday ()},
            };

            for (size_t nIdx = 0; nIdx < SIZE_OF (allEnergyElement); ++ nIdx)
            {
                MSXML::IXMLDOMElementPtr ptrEnergyInfo = 
                    CreateEnergyInfoElement (xmlDocument,
                                             allEnergyElement [nIdx].enargyInfo,
                                             allEnergyElement [nIdx].wcsNаme);
                if (ptrEnergyInfo == NULL)
                {
                    return NULL;
                }
                HRESULT hr = ptrTariff->appendChild (ptrEnergyInfo);
                if (FAILED (hr))
                {
                    return NULL;
                }
            }

            //
            // Сведения по месяцам
            //

            //
            // months element
            //

            TEST_START
            tagAttribute monthsTagAttr [] = 
            {
                {_bstr_t (L"entry_count"), _variant_t (12)},
            };
            size_t nMonthsAttrCount = SIZE_OF (monthsTagAttr);
            _bstr_t bstrMonthsTag (L"months");
            MSXML::IXMLDOMElementPtr ptrMonths;
            tagEntity <_bstr_t, tagAttribute*> xmlMonthsElement =
            {
                bstrMonthsTag,
                ptrTariff, 
                ptrMonths,
                nMonthsAttrCount,
                monthsTagAttr
            };

            if (! xmlMonthsElement.CreateXMLElement (xmlDocument))
            {
                return NULL;
            }
            TEST_END

            int nYear = counterDateTime.GetYear ();
            int nMonth = counterDateTime.GetMonth () - 1;
            const EnergyInfo *pEnergyInfo = tariff.GetEnergyMonth ();

            for (int i = 0; i < 2; ++ i)
            {
                for (int nIdx = 0; nIdx < 12; ++ nIdx)
                {
                    if (0 == i && nIdx <= nMonth) continue;
                    if (0 != i && nIdx > nMonth) continue;
                    if (pEnergyInfo [nIdx].IsEmpty ()) continue;
                    if (! pEnergyInfo [nIdx].bValid) continue;
                    _variant_t value ((unsigned) nIdx);
                    MSXML::IXMLDOMElementPtr ptrEnergyInfo = 
                        CreateEnergyInfoElement (xmlDocument,
                                                 pEnergyInfo [nIdx],                                                 
                                                 L"ee");
                    if (ptrEnergyInfo == NULL) return NULL;
                    if (nIdx <= nMonth) value = nYear;
                    else value = nYear - 1;
                    
                    HRESULT hr = ptrEnergyInfo->setAttribute (L"year", value);
                    if (FAILED (hr)) return NULL;
                    
                    value = (unsigned) nIdx;
                    hr = ptrEnergyInfo->setAttribute (L"month", value);
                    if (FAILED (hr)) return NULL;
                    
                    hr = ptrMonths->appendChild (ptrEnergyInfo);
                    if (FAILED (hr)) return NULL;
                }
            }
        }
    }   

    return ptrEnergy;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateCounterEventLog
//
//  Description:
//
//  Создает XML элемент c журналом событий счетчика
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateCounterEventLog (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                const MeteringData &meteringData)
{
    tagAttribute logTagAttr [] = 
    {
        {_bstr_t (L"entry_count"), _variant_t ((long) Mercury::ReadLogEntry::letMaxValue)}
    };
    size_t nLogAttrCount = SIZE_OF (logTagAttr);
    _bstr_t bstrLogTag (L"log");
    MSXML::IXMLDOMElementPtr ptrLog = CreateXMLElement (xmlDocument, 
                                                        bstrLogTag, 
                                                        nLogAttrCount, 
                                                        logTagAttr);
    if (ptrLog == NULL) return NULL;

    //
    // Журнал событий
    //

    const MeteringData::EventLogEntries *pEventLog = meteringData.GetEventLog ();
    for (size_t nIdx = 0; nIdx < (size_t) (Mercury::ReadLogEntry::letMaxValue); ++ nIdx)
    {
        const MeteringData::EventLogEntries &logEntries = pEventLog [nIdx];
        const Mercury::ReadLogEntry::RequestInfo &requestInfo = 
            Mercury::ReadLogEntry::GetRequestInfo ((Mercury::ReadLogEntry::LogEntryType) nIdx);

        TEST_START
        tagAttribute logEntriesTagAttr [] = 
        {
            {_bstr_t (L"type"),        _variant_t ((unsigned) nIdx)},
            {_bstr_t (L"type_name"),   _variant_t (Mercury::ReadLogEntry::m_EvenLogTypeNames [nIdx])},
            {_bstr_t (L"value_count"), _variant_t (requestInfo.responseLength)},
            {_bstr_t (L"entry_count"), _variant_t ((unsigned) LOG_ENTRY_COUNT)},
        };

        size_t nLogEntriesAttrCount = SIZE_OF (logEntriesTagAttr);
        _bstr_t bstrLogEntriesTag (L"log_entries");
        MSXML::IXMLDOMElementPtr ptrLogEntries;
        tagEntity <_bstr_t, tagAttribute*> xmlLogEntriesElement =
        {
            bstrLogEntriesTag,
            ptrLog, 
            ptrLogEntries,
            nLogEntriesAttrCount,
            logEntriesTagAttr
        };

        if (! xmlLogEntriesElement.CreateXMLElement (xmlDocument))
        {
            return NULL;
        }
        TEST_END

        for (size_t nEntryIdx = 0; nEntryIdx < LOG_ENTRY_COUNT; ++ nEntryIdx)
        {
            const MeteringData::EventLogEntry &logEntry = logEntries [nEntryIdx];
            if (! logEntry.IsValid ()) continue;
            const MeteringData::DateTime &firstValue = logEntry.FirstValue ();
            if (firstValue.GetYear () == 2000 &&
                firstValue.GetMonth () == 0 &&
                firstValue.GetDay () == 0) 
            {
                continue;
            }

            TEST_START            
            tagAttribute logEntryTagAttr [] = 
            {   
                {_bstr_t (L"valid"), _variant_t (logEntry.IsValid ())},
                {_bstr_t (L"idx"), _variant_t ((unsigned) nEntryIdx)},
            };

            size_t nLogEntryAttrCount = SIZE_OF (logEntryTagAttr);
            _bstr_t bstrLogEntryTag (L"le");
            MSXML::IXMLDOMElementPtr ptrLogEntry;
            tagEntity <_bstr_t, tagAttribute*> xmlLogEntryElement =
            {
                bstrLogEntryTag,
                ptrLogEntries, 
                ptrLogEntry,
                nLogEntryAttrCount,
                logEntryTagAttr
            };

            if (! xmlLogEntryElement.CreateXMLElement (xmlDocument))
            {
                return NULL;
            }
            TEST_END

            TEST_START
            tagAttribute logEntryValue1TagAttr [] = 
            {   
                {_bstr_t (L"d"),  _variant_t (firstValue.GetDay ())},
                {_bstr_t (L"mm"), _variant_t (firstValue.GetMonth ())},
                {_bstr_t (L"y"),  _variant_t (firstValue.GetYear ())},
                {_bstr_t (L"h"),  _variant_t (firstValue.GetHour ())},
                {_bstr_t (L"m"),  _variant_t (firstValue.GetMinute ())},
            };

            size_t nLogEntryValue1AttrCount = SIZE_OF (logEntryValue1TagAttr);
            _bstr_t bstrLogEntryValue1Tag (L"first");
            MSXML::IXMLDOMElementPtr ptrLogEntryValue1;
            tagEntity <_bstr_t, tagAttribute*> xmlLogEntryValue1Element =
            {
                bstrLogEntryValue1Tag,
                ptrLogEntry, 
                ptrLogEntryValue1,
                nLogEntryValue1AttrCount,
                logEntryValue1TagAttr
            };

            if (! xmlLogEntryValue1Element.CreateXMLElement (xmlDocument))
            {
                return NULL;
            }
            TEST_END

            TEST_START
            if (requestInfo.responseLength > 1)
            {
                const MeteringData::DateTime &secondValue = logEntry.SecondValue ();
                tagAttribute logEntryValue2TagAttr [] = 
                {   
                    {_bstr_t (L"d"),  _variant_t (secondValue.GetDay ())},
                    {_bstr_t (L"mm"), _variant_t (secondValue.GetMonth ())},
                    {_bstr_t (L"y"),  _variant_t (secondValue.GetYear ())},
                    {_bstr_t (L"h"),  _variant_t (secondValue.GetHour ())},
                    {_bstr_t (L"m"),  _variant_t (secondValue.GetMinute ())},
                };

                size_t nLogEntryValue2AttrCount = SIZE_OF (logEntryValue2TagAttr);
                _bstr_t bstrLogEntryValue2Tag (L"second");
                MSXML::IXMLDOMElementPtr ptrLogEntryValue2;
                tagEntity <_bstr_t, tagAttribute*> xmlLogEntryValue2Element =
                {
                    bstrLogEntryValue2Tag,
                    ptrLogEntry, 
                    ptrLogEntryValue2,
                    nLogEntryValue2AttrCount,
                    logEntryValue2TagAttr
                };

                if (! xmlLogEntryValue2Element.CreateXMLElement (xmlDocument))
                {
                    return NULL;
                }
            }
            TEST_END
        }
    }

    return ptrLog;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateTimeStamp
//
//  Description:
//
//  Добавляет к ptrParent элемент wcsName свременой меткой pTime.
//  По-умолчанию используется текущее системное время.
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateTimeStamp (MSXML::IXMLDOMElementPtr ptrParent,
                                                          const wchar_t *wcsName/* = L"time_stamp"*/,
                                                          PSYSTEMTIME pTime/* = NULL*/)
{
    if (ptrParent == NULL) return NULL;
    MSXML::IXMLDOMDocument *pxmlDocument = NULL; 
    HRESULT hr = ptrParent->get_ownerDocument (&pxmlDocument);
    if (FAILED (hr)) return NULL;
    if (NULL == pxmlDocument) return NULL;

    SYSTEMTIME now;    
    if (NULL == pTime)
    {   
        GetLocalTime (&now);
        pTime = &now;
    }

    tagAttribute timestampAttributes [] = 
    {
        {_bstr_t (L"day"),            _variant_t ((unsigned) now.wDay)},
        {_bstr_t (L"month"),          _variant_t ((unsigned) now.wMonth)},
        {_bstr_t (L"year"),           _variant_t ((unsigned) now.wYear)},
        {_bstr_t (L"hour"),           _variant_t ((unsigned) now.wHour)},
        {_bstr_t (L"minute"),         _variant_t ((unsigned) now.wMinute)},
        {_bstr_t (L"second"),         _variant_t ((unsigned) now.wSecond)},

    };
    size_t nTimestampAttributeCount = SIZE_OF (timestampAttributes);

    MSXML::IXMLDOMElementPtr ptrResult = 
        CreateXMLElement (pxmlDocument,
                          wcsName,
                          nTimestampAttributeCount,
                          timestampAttributes);
    if (NULL == ptrResult)
    {
        return NULL;
    }

    if (FAILED (ptrParent->appendChild (ptrResult))) return NULL;

    return ptrResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateMonthlyPowerElement
//
//  Description:
//
//  Создает XML элемент описания суммарного потребления мощности за месяц
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::CreateMonthlyPowerElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                    const MonthlyPowerReport &rpt,
                                                                    const PowerUsageSummarizer::SummaryStorage &storage,
                                                                    unsigned nMonth, unsigned nYear,
                                                                    const wchar_t *wcsName)
{
    unsigned nExcessCount = (unsigned) rpt.excesIndexes.size ();
    tagAttribute monthlyPowerTagAttr [] = 
    {
        {_bstr_t (L"excess_count"),   _variant_t ((unsigned) nExcessCount)},
        {_bstr_t (L"limit"),          _variant_t (rpt.powerLimit)},
        {_bstr_t (L"month"),          _variant_t (nMonth)},
        {_bstr_t (L"year"),           _variant_t (nYear)},
        {_bstr_t (L"from_hour"),      _variant_t ((unsigned) rpt.nFromHour)},
        {_bstr_t (L"from_minute"),    _variant_t ((unsigned) rpt.nFromMinute)},
        {_bstr_t (L"to_hour"),        _variant_t ((unsigned) rpt.nToHour)},
        {_bstr_t (L"to_minute"),      _variant_t ((unsigned) rpt.nToMinute)},
        {_bstr_t (L"desc"),           _variant_t (rpt.strDescription)},
        {_bstr_t (L"min_power"),      _variant_t (rpt.minPower)},
        {_bstr_t (L"min_power_idx"),  _variant_t ((unsigned) rpt.minPowerIdx)},
        {_bstr_t (L"max_power"),      _variant_t (rpt.maxPower)},
        {_bstr_t (L"max_power_idx"),  _variant_t ((unsigned) rpt.maxPowerIdx)},
        {_bstr_t (L"avg_power"),      _variant_t (rpt.avgPower)},
        {_bstr_t (L"entry_count"),    _variant_t (rpt.countPower)},
        {_bstr_t (L"min_excess"),     _variant_t (rpt.minPowerExces)},
        {_bstr_t (L"min_excess_idx"), _variant_t ((unsigned) rpt.minPowerExcesIdx)},
        {_bstr_t (L"max_excess"),     _variant_t (rpt.maxPowerExces)},
        {_bstr_t (L"max_excess_idx"), _variant_t ((unsigned) rpt.maxPowerExcesIdx)},
        {_bstr_t (L"avg_excess"),     _variant_t (rpt.avgPowerExces)},
    };
    size_t nMonthlyPowerAttrCount = SIZE_OF (monthlyPowerTagAttr);
    _bstr_t bstrMonthlyPowerTag (L"pm");
    MSXML::IXMLDOMElementPtr ptrMonthlyPower = CreateXMLElement (xmlDocument, 
                                                                 bstrMonthlyPowerTag, 
                                                                 nMonthlyPowerAttrCount, 
                                                                 monthlyPowerTagAttr);
    if (ptrMonthlyPower == NULL) return NULL;

    size_t nStorageSize = storage.size ();
    if (rpt.minPowerIdx < nStorageSize)
    {
        MSXML::IXMLDOMElementPtr ptrEntry = 
            CreateSummaryPowerEntryElement (xmlDocument, storage [rpt.minPowerIdx], (unsigned) rpt.minPowerIdx, L"min_pwr");
        if (ptrEntry == NULL) return NULL;
        if (FAILED (ptrMonthlyPower->appendChild (ptrEntry))) return NULL;
    }
    if (rpt.maxPowerIdx < nStorageSize)
    {
        MSXML::IXMLDOMElementPtr ptrEntry = 
            CreateSummaryPowerEntryElement (xmlDocument, storage [rpt.maxPowerIdx], (unsigned) rpt.maxPowerIdx, L"max_pwr");
        if (ptrEntry == NULL) return NULL;
        if (FAILED (ptrMonthlyPower->appendChild (ptrEntry))) return NULL;
    }
    if (rpt.maxPowerExcesIdx < nStorageSize)
    {
        MSXML::IXMLDOMElementPtr ptrEntry = 
            CreateSummaryPowerEntryElement (xmlDocument, storage [rpt.maxPowerExcesIdx], (unsigned) rpt.maxPowerExcesIdx, L"max_pwr_exc");
        if (ptrEntry == NULL) return NULL;
        if (FAILED (ptrMonthlyPower->appendChild (ptrEntry))) return NULL;
    }
    if (rpt.minPowerExcesIdx < nStorageSize)
    {
        MSXML::IXMLDOMElementPtr ptrEntry = 
            CreateSummaryPowerEntryElement (xmlDocument, storage [rpt.minPowerExcesIdx], (unsigned) rpt.minPowerExcesIdx, L"min_pwr_exc");
        if (ptrEntry == NULL) return NULL;
        if (FAILED (ptrMonthlyPower->appendChild (ptrEntry))) return NULL;
    }

    for (unsigned nIdx = 0; nIdx < nExcessCount; ++ nIdx)
    {
        unsigned nStorageIdx = (unsigned) rpt.excesIndexes [nIdx];
        MSXML::IXMLDOMElementPtr ptrEntry = 
            CreateSummaryPowerEntryElement (xmlDocument, storage [nStorageIdx], (unsigned) nStorageIdx, L"pwr_exc");
        if (ptrEntry == NULL) return NULL;
        if (FAILED (ptrMonthlyPower->appendChild (ptrEntry))) return NULL;
    }

    return ptrMonthlyPower;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCustomerEnergyReport
//
//  Description:
//
//  Создает XML элемент c отчетом по энегропотреблению потребителя
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCustomerEnergyReport (MSXML::IXMLDOMDocumentPtr xmlDocument)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    const CounterDataList &counterData = GetCounterData ();
    size_t nCounterCount = counterData.size ();

    EnergyReport summaryReport;
    for (size_t nCounterIdx = 0; nCounterIdx < nCounterCount; ++ nCounterIdx)
    {
        const CounterData &data = counterData [nCounterIdx];
        if (! data.IsValid ())
        {
            continue;
        }

        const MeteringData &meteringData = data.GetData ();
        EnergyReport rpt (meteringData.GetEnergy () [0], meteringData.GetCounterTime (), 1, 1);
        summaryReport += rpt;
    }

    MSXML::IXMLDOMElementPtr ptrResult = CreateEnergyReportElement (xmlDocument, summaryReport, L"energy_summary");
    if (ptrResult != NULL) CreateTimeStamp (ptrResult);

    return ptrResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCustomerSummaryPowerReport
//
//  Description:
//
//  Создает XML элемент отчета суммарного потребления мощности
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCustomerSummaryPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                             PowerUsageSummarizer &summaryReport)
{
    summaryReport.Reset ();
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    CounterDataList &counterData = GetCounterData ();
    size_t nCounterCount = counterData.size ();
        
    for (size_t nCounterIdx = 0; nCounterIdx < nCounterCount; ++ nCounterIdx)
    {
        CounterData &data = counterData [nCounterIdx];
        if (! data.IsValid ())
        {
            continue;
        }

        MeteringData &meteringData = data.GetData ();
        MeteringData::DateTime &dt = meteringData.GetCounterTime ();
        PowerStorage &ps = meteringData.GetPowerStorage ();
        size_t nPowerDataCount = ps.size ();
        if (0 == nPowerDataCount)
        {
            continue;
        }

        int nKn = data.GetCounter ().m_nKn;
        int nKt = data.GetCounter ().m_nKt;        
        const ProductionType& pt = meteringData.GetProductionType ();
        double dCounterConst = pt.CounterConst1 ();
        if (meteringData.GetProtocolType () == Mercury::ptMercury)  
        {
            dCounterConst = pt.CounterConst2 ();
        }

        int k = nKn * nKt;
        for (size_t nPowerIdx = 0; nPowerIdx < nPowerDataCount; ++ nPowerIdx)
        {
            PowerInfo& powerInfo = ps [nPowerIdx];
            summaryReport.Add (powerInfo, (unsigned int) nCounterIdx, k, dCounterConst);
        }
    }

    summaryReport.Sort ();
    const PowerUsageSummarizer::SummaryStorage &stg = summaryReport.GetStorage ();

    MSXML::IXMLDOMElementPtr ptrResult = CreateSummaryPowerElement (xmlDocument, stg, L"power_summary");
    if (ptrResult != NULL) CreateTimeStamp (ptrResult);

    return ptrResult;

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateCustomerMonthlyPowerReport
//
//  Description:
//
//  Создает XML элемент отчета суммарного потребления мощности по месяцам
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMElementPtr DataManagerXML::GenerateCustomerMonthlyPowerReport (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                                                             const PowerUsageSummarizer::SummaryStorage &storage)
{
    if (NULL == xmlDocument)
    {
        return NULL;
    }

    size_t nCustomerCount = m_Customers.size ();
    if (m_nCurrentCustomerIdx >= nCustomerCount ||
        m_nInvalidIdx == m_nCurrentCustomerIdx)
    {
        return NULL;
    }
    const Customer &customer = m_Customers [m_nCurrentCustomerIdx];
    __int64 powerLimit = (__int64) (1000.0 * customer.GetPowerLimit ());
    
    MSXML::IXMLDOMElementPtr ptrResult = CreateXMLElement (xmlDocument, 
                                                           L"power_usage", 
                                                           0, 
                                                           (tagAttribute*) NULL);
    if (ptrResult == NULL) return NULL;
    CreateTimeStamp (ptrResult);


    size_t nIdx = 0;
    MeteringData::DateTime month;

    MonthlyPowerReport reports [2];

    reports [0].strDescription = L"утро (с 8:30 до 10:30)";
    reports [0].powerLimit = powerLimit;
    reports [0].nFromHour = 9;  //8:30
    reports [0].nFromMinute = 00;
    reports [0].nToHour = 10;
    reports [0].nToMinute = 30;

    reports [1].strDescription = L"вечер (с 18:00 до 21:00)";
    reports [1].powerLimit = powerLimit;
    reports [1].nFromHour = 18;
    reports [1].nFromMinute = 30;   //18:00
    reports [1].nToHour = 21;
    reports [1].nToMinute = 00;


    size_t nReportCount = SIZE_OF (reports);

    size_t nCount = storage.size ();
    if (nCount)
    {
        bool bResult = true;
        do
        {
            bResult  = ProcessMonth (storage, 
                                     nIdx, 
                                     month,
                                     nReportCount,
                                     reports);

            
            for (size_t i = 0; i < nReportCount; ++ i)
            {
                const MonthlyPowerReport &rpt = reports [i];

                MSXML::IXMLDOMElementPtr ptrMonth = 
                    CreateMonthlyPowerElement (xmlDocument, 
                                               rpt, 
                                               storage, 
                                               month.GetMonth (), 
                                               month.GetYear (), 
                                               L"month" );

                if (ptrMonth == NULL) return NULL;
                if (FAILED (ptrResult->appendChild (ptrMonth))) return NULL;                
            }            
        }
        while (bResult);
    }
    
    return ptrResult;    
}