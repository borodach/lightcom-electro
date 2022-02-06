///////////////////////////////////////////////////////////////////////////////
//
//  File:           EnergyReport.hpp
//
//  Facility:       ��������� ���������
//
//
//  Abstract:       ���� �������� �����, ��������������� ��� ��������� 
//                  ���������� ������ �� �����������������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  10-06-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: EnergyReport.hpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * ��������� Source Safe comments
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
    // ����� �� ���� �����
    //

    typedef std::pair <MeteringData::DateTime, EnergyInfo> ReportEntry;

    //
    // ��������� ���������� �������
    //

    typedef std::vector <ReportEntry> ReportStorage;    

    //
    // ����������� ��-���������
    //

    EnergyReport ()
    {
    }

    //
    // �����������
    //

    EnergyReport (const AllEnergyInfo &energyInfo, 
                  MeteringData::DateTime scanDate,
                  int nKn, 
                  int nKt)
    {
        Init (energyInfo, scanDate, nKn, nKt);
    }

    //
    // ����������
    //

    virtual ~EnergyReport ()
    {
    }

    //
    // ������� ������, �������� � �������
    //

    void Reset () 
    {
        m_Storage.clear (); 
        m_ThisYear.Reset ();
        m_LastYear.Reset ();
        m_FromReset.Reset ();
    }

    //
    // ���������� ����� ���� � ������� ���� �������� ������ �� �����������������
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
    // ���������� ����� ������ � ������� ���� �������� ������ �� �����������������
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
    // ���������� ����/����� ����� ���� �������� ������ �� �����������������
    //

    bool GetScanDate (MeteringData::DateTime &scanDate) const
    {
        size_t nCount = m_Storage.size ();
        if (nCount < 1) return false;

        scanDate = m_Storage [nCount - 1].first;

        return true;
    }

    //
    // ���������, ���� �� ������ �� �����������������
    //

    bool IsEmpty () const
    {
        return m_Storage.size () == 0;
    }

    //
    // ��������� �������� � ���������� �����������������
    //

    const ReportStorage& GetStorage () const {return m_Storage;}
    ReportStorage& GetStorage () {return m_Storage;}

    //
    // ����������������� � ������� ����
    //

    const EnergyInfo& ThisYear () const {return m_ThisYear;}
    EnergyInfo& ThisYear () {return m_ThisYear;}

    //
    // ����������������� � ������� ����
    //

    const EnergyInfo& LastYear () const {return m_LastYear;}
    EnergyInfo& LastYear () {return m_LastYear;}

    //
    // ����������������� �� ������
    //

    const EnergyInfo& FromReset () const {return m_FromReset;}
    EnergyInfo& FromReset () {return m_FromReset;}

    //
    // ���������� ������� �������
    //

    void Init (const AllEnergyInfo &energyInfo, 
               MeteringData::DateTime scanDate,
               int nKn, 
               int nKt);

    //
    // ������������ ���������� ������ �� �����������������
    //

    const EnergyReport& operator += (const EnergyReport& other);


    //
    // ������ ���������� �� ������� � ������
    //

    void Dump (CString &strReport) const;

    //
    // ���������� � ������ strReport ����� �� ����� nMonth.
    // nMonth - �������� ������������ ���������� ������. ������ ���� 
    // ���������������
    //
    
    void DumpMonth (CString &strReport, int nMonth) const;

protected:

    //
    // ��������� �������� � ���������� �����������������
    //

    ReportStorage m_Storage;

    EnergyInfo m_ThisYear;      // ����������������� � ������� ����
    EnergyInfo m_LastYear;      // ����������������� � ������� ����
    EnergyInfo m_FromReset;     // ����������������� �� ������
};

#endif //_ENERGY_REPORT_HPP_