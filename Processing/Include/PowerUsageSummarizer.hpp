///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerUsageSummarizer.hpp
//
//  Facility:       ��������� ���������
//
//
//  Abstract:       ���� �������� ����� ��� ���������� �������� � ���������
//                  ����������� �������� ������������ ����������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  11-06-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: PowerUsageSummarizer.hpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * ��������� Source Safe comments
* 
*/

#ifndef _POWER_USAGE_CALCULATOR_HPP_
#define _POWER_USAGE_CALCULATOR_HPP_

#include <vector>
#include <algorithm>
#include "PowerUsage.hpp"

class PowerUsageSummarizer
{
public:
    
    //
    // �������� �� ����������� �������� �� ������
    //

    typedef std::pair <MeteringData::DateTime, PowerUsage> SummaryEntry;

    //
    // �������� �� ����������� �������� �� ���� �������� ������
    //

    typedef std::vector <SummaryEntry> SummaryStorage;

public:

    //
    // �����������
    //

    PowerUsageSummarizer ()
    {
    }

    //
    // ����������
    //

    virtual ~PowerUsageSummarizer ()
    {
    }

    //
    // ������� �������
    //

    void Reset ()
    {
        m_Storage.clear ();
    }

    //
    // �������� �� ����������������� �� ���� �������� ������
    //

    const SummaryStorage & GetStorage () const {return m_Storage;}
    SummaryStorage & GetStorage () {return m_Storage;}

    //
    // ����������� ���������
    //

    void Sort ()
    {
        std::sort (m_Storage.begin (), m_Storage.end ());
    }

    //
    // ���������� �������� � ����������� ��������
    //

    void Add (PowerInfo &powerInfo, 
              unsigned nCounterIdx,
              int kTn_kTt, 
              double dCounterConst);
protected:

    //
    // �������� �� ����������������� �� ���� �������� ������
    //

    SummaryStorage m_Storage;
};

#endif _POWER_USAGE_CALCULATOR_HPP_