///////////////////////////////////////////////////////////////////////////////
//
//  File:           DataManager.hpp
//
//  Facility:       ��������� ���������
//
//
//  Abstract:       �����, ����������� ������� ��� ���� ������������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  24-05-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: DataManager.hpp $
 * 
 * *****************  Version 6  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * ��������� Source Safe comments
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
    // ����� � ����������� ��������
    //

    struct MonthlyPowerReport
    {
    public:

        //
        // ���������� � strReport ����� � ����������� ��������.
        //

        void Dump (CString &strReportText, 
                   const PowerUsageSummarizer::SummaryStorage &stg,
                   const CounterDataList &counterList,
                   __int64 powerLimit) const;

    public:
        __int64 powerLimit;      // ����������� ���������� ��������
        __int64 minPower;        // ����������� ��������
        size_t  minPowerIdx;     // ������ ������ ������� ���������, ���������������
                                 // ����������� ��������
        __int64 maxPower;        // ������������ ��������
        size_t  maxPowerIdx;     // ������ ������ ������� ���������, ���������������
                                 // ������������ ��������
        __int64 avgPower;        // ������� ��������
        __int64 countPower;      // ���������� ������� � ��������
        __int64 minPowerExces;   // ����������� ���������� ��������
        size_t  minPowerExcesIdx;// ������ ������ ������� ���������, ���������������
                                 // ������������ ���������� ��������
        __int64 maxPowerExces;   // ������������ ���������� ��������
        size_t maxPowerExcesIdx; // ������ ������ ������� ���������, ���������������
                                 // ������������� ���������� ��������
        __int64 avgPowerExces;   // ������� ���������� ��������
        std::vector<size_t> excesIndexes; // ������� ���������� ��������

        size_t nFromHour;        // ������ ��������� ��������
        size_t nFromMinute;
        size_t nToHour;          // ����� ��������� ��������
        size_t nToMinute;                 
        CString strDescription;  // �������� ���������
    };

    //
    // �����������
    //

    DataManager (): m_strResultsFolder (APP_RESULTS_FOLDER)
    {

        Reset ();
    }

    //
    // ����������
    //

    virtual ~DataManager ()
    {
        Reset ();
    }

    //
    // ������� �������
    //

    void Reset ();

    //
    // �������� ������ ������������ �� ����� strFileName
    //

    bool LoadCustomers (const wchar_t* wcsFileName = NULL);

    //
    //  ��������� ���������� ������������ ���� ��������� ��������� �����������
    //

    void LoadAllCounterData (Customer &customer, const wchar_t *wcsPath);

    //
    // ���������� ������ ������������ � ����� strFileName
    //

    bool SaveCustomers (const wchar_t* wcsFileName = NULL) const;

    //
    // ������ �����������
    //

    CustomerList& GetCustomers () {return m_Customers;}
    const CustomerList& GetCustomers () const {return m_Customers;}

    //
    // ������� �����������
    //

    size_t GetCurrentCustomerIdx () const {return m_nCurrentCustomerIdx;}
    void JustChangeCurrentCustomerIdx (size_t nIdx) {m_nCurrentCustomerIdx = nIdx;}
    void SetCurrentCustomerIdx (size_t nIdx,
                                const wchar_t* wcsResultsFolder = NULL);
    //
    // ���������� �����������
    //

    void InsertCustomer (const Customer &customer, size_t nCustomerIdx);

    //
    // �������� �����������
    //

    void DeleteCustomer (size_t nCustomerIdx);
    
    //
    // ������ � ���������
    //

    CounterDataList& GetCounterData () {return m_CounterData;}
    const CounterDataList& GetCounterData () const {return m_CounterData;}


    //
    // ����� �����
    //

    const CString& SummaryReportText () const {return m_strSummaryReport;}

    //
    // ����� �� �����������������
    //

    const CString& EnergyReportText () const {return m_strEnergyReport;}

    //
    // ����� � ����������� ��������
    //

    const CString& PowerReportText () const {return m_strPowerReport;}

    //
    //  ������ ��� ������
    //

    virtual void GenerateAllReports ();


protected:

    //
    // ����� �����
    //

    virtual void GenerateSummaryReport (CString &strReportText) const;

    //
    // ����� �� �����������������
    //

    virtual void GenerateEnergyReport (CString &strReportText) const;

    //
    // ����� � ����������� ��������
    //

    virtual void GeneratePowerReport (CString &strReportText);

    //
    // ����� � ������������ ������������
    //

    const CString& GetResultsFolder () const {return m_strResultsFolder;}
    CString& GetResultsFolder () {return m_strResultsFolder;}
    void SetResultsFolder (const CString& strFolder) {m_strResultsFolder = strFolder;}
    
    //
    // ��������� ��������� �� ���� �����
    //

    static bool ProcessMonth (const PowerUsageSummarizer::SummaryStorage &stg, 
                              size_t &nIdx, 
                              MeteringData::DateTime &month,
                              size_t nReportCount,
                              MonthlyPowerReport reports []);
public:

    //
    // ���������� ������
    //

    static const size_t m_nInvalidIdx = ~0;

protected:
    
    //
    // ������ ������������
    //

    CustomerList m_Customers;

    //
    // ������ �������� �����������
    //

    size_t m_nCurrentCustomerIdx;

    //
    // ������� ������ � ���������
    //

    CounterDataList m_CounterData;

    //
    // ����� �����
    //

    CString m_strSummaryReport;

    //
    // ����� �� �����������������
    //

    CString m_strEnergyReport;

    //
    // ����� � ����������� ��������
    //

    CString m_strPowerReport;

    //
    // ����� � ������������ ������������
    //

    CString m_strResultsFolder;
};
#endif // _DATA_MANAGER_HPP_
