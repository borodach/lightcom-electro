///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadEnergy.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ������ ������� �� ������ �������� � ����������� 
//                  �������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  29-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ReadEnergy.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _MERCURY_READ_ENERGY_
#define _MERCURY_READ_ENERGY_

#include "Request.hpp"

namespace Mercury
{
    //
    // ��� ����������� �������
    //

    enum EnergyType : unsigned char
    {
        etInvalid   = 0xFF, // ���������� ��������
        etFromReset = 0x00, // ������ ����������� ������� �� ������.
        etThisYear  = 0x01, // ������ ����������� ������� �� ������� ���.
        etLastYear  = 0x02, // ������ ����������� ������� �� ���������� ���.
        etMonth     = 0x03, // ������ ����������� ������� �� �����.
        etToday     = 0x04, // ������ ����������� ������� �� ������� �����.
        etYesterday = 0x05  // ������ ����������� ������� �� ���������� �����.
    };
 
    class ReadEnergy: public Request
    {
    public:

        //                 
        // �����������
        //

        ReadEnergy (CrcType nType = crc) : Request (rcReadEnergy, 2, nType)
        {
            m_nResponseSize = 16;
        }

        //
        // ��� ����������� �������
        //

        EnergyType GetEnergyType () const
        {
            EnergyType nResult = EnergyType ((m_Body [2] >> 4) & 0x0F);
            return nResult;
        }

        void SetEnergyType (EnergyType energyType)
        {
            m_Body [2] &= 0x0F;
            m_Body [2] |= (energyType << 4) & 0xF0;
            CalculateAndStoreCrc ();
        }

        //
        // ����� ������ - ��������� � 1 (������)
        //

        unsigned char GetMonth () const
        {
            unsigned char nResult = m_Body [2] & 0x0F;
            return nResult;
        }

        void SetMonth (unsigned char nMonth)
        {
            m_Body [2] &= 0xF0;
            m_Body [2] |= nMonth & 0x0F;
            CalculateAndStoreCrc ();
        }

        //
        // ����� ������
        //

        unsigned char GetTariff () const
        {
            return m_Body [3];
        }

        void SetTariff (unsigned char nTariff)
        {
            m_Body [3] = nTariff;
            CalculateAndStoreCrc ();
        }


    };
}
#endif