///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadLogEntry.hpp
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
* $History: ReadLogEntry.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol/include
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _MERCURY_READ_LOG_ENTRY_
#define _MERCURY_READ_LOG_ENTRY_

#include "Request.hpp"                   

namespace Mercury
{
    class ReadLogEntry: public Request
    {
    public:

        //
        // ��� �������� ����
        //

        enum LogEntryType : unsigned char
        {
            letOnOff = 0,       // ���������/���������� �������
            letTimeChange,      // ��������� �����
            letPhase1,          // ���������/���������� ���� 1
            letPhase2,          // ���������/���������� ���� 2
            letPhase3,          // ���������/���������� ���� 3
            letTariffTable,     // ��������� ��������� ����������
            letHolydayTable,    // ��������� ���������� ����������� ����
            letEnergy,          // ����� ��������� ����������� �������
            letPower,           // ������������� ������� ������� ���������
            letProtection,      // ��������/�������� ������ (����������� ������)
            letMaxValue         // ������������ ��������
        };

        //
        // �������� ������� �� ������ �������� ����
        //
        
        struct RequestInfo
        {
            LogEntryType  entryType;          // ��� �������� ����
            unsigned char protocol1Command;   // ��� ��������� ��� ��������� 1
            unsigned char protocol2Command;   // ��� ��������� ��� ��������� 2
            unsigned char responseLength;     // ���������� �������� DataTime � ������.
        };

        //
        // �������� ����� �������� ����
        //

        static const wchar_t* m_EvenLogTypeNames [letMaxValue + 1];

        //
        // �����������
        //

        ReadLogEntry (ProtocolType protocolType,
                      CrcType nType = crc) : 
            Request (rcReadEventLog, ptSET == protocolType? 1 : 2, nType),
            m_nProtocolType (protocolType)
        {
            SetEntryNumber (0);
            SetType (letOnOff);
        }

        //
        // ���������� ��� �������� ����
        //

        LogEntryType GetLogEntryType () const
        {
            const RequestInfo &ri = GetRequestInfo ();

            return ri.entryType;
        }
        
        //
        // ������������� ��� �������� ����
        //

        void SetLogEntryType (LogEntryType nType)
        {
            const RequestInfo &ri = GetRequestInfo (nType);
            if (ptSET == m_nProtocolType)
            {
                SetType (ri.protocol1Command);
                m_nResponseSize = ri.responseLength * 7;
            }
            else
            {
                SetType (ri.protocol2Command);
                m_nResponseSize = ri.responseLength * 6;
            }            
        }
        
        //
        // ���������� ����� ������ � ����
        //

        unsigned char GetEntryNumber () const
        {
            unsigned char nResult = 0;
            if (ptSET == m_nProtocolType)
            {
                nResult = m_Body [2] & 0x0F;
            }
            else
            {
                nResult = m_Body [3];
            }

            return nResult;
        }

        //
        // ������������ ����� ������ � ����
        //

        void SetEntryNumber (unsigned char nEntryNumber)
        {
            if (ptSET == m_nProtocolType)
            {
                m_Body [2] &= 0xF0;
                m_Body [2] |= 0x0F & nEntryNumber;
            }
            else
            {
                m_Body [3] = nEntryNumber;
            }
            CalculateAndStoreCrc ();
        }

        //
        // ���������� ��� ���������
        //

        ProtocolType GetProtocolType () const {return m_nProtocolType;}

        //
        // ���������� �������� �������� ���� �� ��� ����.
        //

        static const RequestInfo& GetRequestInfo (LogEntryType entryType);

        //
        // ���������� �������� �������� ���� �� ��� ��������� ����.
        //

        const RequestInfo& GetRequestInfo () const;

protected:       

        //
        // �������� ����� ��������� ����
        //

        static const RequestInfo m_RequestInfoTable [letMaxValue];
       
        //
        // ���������� ��� �������� ����
        //

        unsigned char GetType () const
        {
            unsigned char nResult = 0;
            if (ptSET == m_nProtocolType)
            {
                nResult = (m_Body [2] >> 4) & 0x0F;
            }
            else
            {
                nResult = m_Body [2];
            }

            return nResult;   
        }

        //
        // ������������� ��� �������� ����
        //

        void SetType (unsigned char nType)
        {
            if (ptSET == m_nProtocolType)
            {
                m_Body [2] &= 0x0F;
                m_Body [2] |= 0xF0 & (nType << 4);
            }
            else
            {
                m_Body [2] = nType;
            }
            CalculateAndStoreCrc ();
        }

        //
        // ��� ���������
        //

        ProtocolType m_nProtocolType;
    };
}
#endif