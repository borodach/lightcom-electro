///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerUsage.hpp
//
//  Facility:       ��������� ���������
//
//
//  Abstract:       ����� ��� �������� �������� � ����������� ��������, ������ 
//                  ����������� ����������
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
* $History: PowerUsage.hpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * ��������� Source Safe comments
* 
*/

#ifndef _POWER_USAGE_HPP_
#define _POWER_USAGE_HPP_

#include "CounterData.hpp"
#include "PowerInfo.hpp"


class PowerUsage
{
public:
    typedef std::pair <PowerInfo*, unsigned> UsageEntry;
    typedef std::vector <UsageEntry> UsageStorage;
public:

    //
    // �����������
    //

    PowerUsage ()
    {
    }

    //
    // ����������
    //

    virtual ~PowerUsage ()
    {
    }

    //
    // ��������� ��������� ���������. �������, ��� ����� ������� ����� ������
    // �����.
    //

    bool operator == (const PowerUsage& other) const 
    {
        return true;
    }

    bool operator < (const PowerUsage& other) const 
    {
        return false;
    }

    //
    // ������� �������
    //

    void Reset ()
    {
        m_UsageStorage.clear ();
    }

    //
    // ��������� ����������� ��������
    //

    UsageStorage& GetStorage () {return m_UsageStorage;}
    const UsageStorage& GetStorage () const {return m_UsageStorage;}

    //
    // ���������� �������� � ����������� ��������
    //

    void Add (const UsageEntry& entry);

    //
    // ������������ ���������� ������ � ������������� ��������
    //

    void CreateSummaryUsage (PowerInfo &summaryUsage) const;

protected:

    //
    // ��������� ����������� ��������
    //

    UsageStorage m_UsageStorage;
};
#endif //_POWER_USAGE_HPP_
