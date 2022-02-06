///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadLogEntry.cpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ������ ������� �� ������ ������ �� ������� �������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  24-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ReadLogEntry.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Protocol
 * 
 * *****************  Version 7  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol
 * 
 * *****************  Version 6  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Protocol
 * ��������� Source Safe comments
* 
*/

#include "Utils.h"
#include "ReadLogEntry.hpp"                   

namespace Mercury
{
    //
    // �������� ����� ��������� ����
    //

    const ReadLogEntry::RequestInfo ReadLogEntry::m_RequestInfoTable [ReadLogEntry::letMaxValue] = 
    {
        {ReadLogEntry::letOnOff,        0x01, 0x01, 2},
        {ReadLogEntry::letTimeChange,   0x02, 0x02, 2},
        {ReadLogEntry::letPhase1,       0x07, 0x03, 2},
        {ReadLogEntry::letPhase2,       0x08, 0x04, 2},
        {ReadLogEntry::letPhase3,       0x09, 0x05, 2},
        {ReadLogEntry::letTariffTable,  0x04, 0x07, 1},
        {ReadLogEntry::letHolydayTable, 0x03, 0x08, 1},
        {ReadLogEntry::letEnergy,       0x05, 0x09, 1},
        {ReadLogEntry::letPower,        0x06, 0x0A, 1},
        {ReadLogEntry::letProtection,   0x0A, 0x12, 2}
    };


    //
    // �������� ����� �������� ����
    //

    const wchar_t* ReadLogEntry::m_EvenLogTypeNames [ReadLogEntry::letMaxValue + 1] =
    {
        L"���������/���������� �������",
        L"��������� �����",
        L"���������/���������� ���� 1",
        L"���������/���������� ���� 2",
        L"���������/���������� ���� 3",                  
        L"��������� ��������� ����������",               
        L"��������� ���������� ����������� ����",
        L"����� ��������� ����������� �������",
        L"������������� ������� ������� ���������",
        L"��������/�������� ������ (����������� ������)",
        L"���������� ��������"
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetRequestInfo
    //
    //  Description:
    //
    //  ���������� �������� �������� ���� �� ��� ����.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const ReadLogEntry::RequestInfo& ReadLogEntry::GetRequestInfo (ReadLogEntry::LogEntryType entryType)
    {
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_RequestInfoTable); ++ nIdx)
        {
            if (m_RequestInfoTable [nIdx].entryType == entryType)
            {
                return m_RequestInfoTable [nIdx];
            }
        }

        return m_RequestInfoTable [0];
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetRequestInfo
    //
    //  Description:
    //
    //  ���������� �������� �������� ���� �� ��� ��������� ����.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const ReadLogEntry::RequestInfo& ReadLogEntry::GetRequestInfo () const
    {
        unsigned char nType = GetType ();
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_RequestInfoTable); ++ nIdx)
        {
            unsigned char nNativeCode;
            if (ptSET == m_nProtocolType)
            {
                nNativeCode = m_RequestInfoTable [nIdx].protocol1Command;
            }
            else
            {
                nNativeCode = m_RequestInfoTable [nIdx].protocol2Command;
            }

            if (nNativeCode == nType)
            {
                return m_RequestInfoTable [nIdx];
            }
        }

        return m_RequestInfoTable [0];
    }
}