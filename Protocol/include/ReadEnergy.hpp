///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadEnergy.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма запроса на чтение сведений о накопленной 
//                  энергии.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  29-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
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
    // Тип накопленной энергии
    //

    enum EnergyType : unsigned char
    {
        etInvalid   = 0xFF, // Невалидное значение
        etFromReset = 0x00, // Чтение накопленной энергии от сброса.
        etThisYear  = 0x01, // Чтение накопленной энергии за текущий год.
        etLastYear  = 0x02, // Чтение накопленной энергии за предыдущий год.
        etMonth     = 0x03, // Чтение накопленной энергии за месяц.
        etToday     = 0x04, // Чтение накопленной энергии за текущие сутки.
        etYesterday = 0x05  // Чтение накопленной энергии за предыдущие сутки.
    };
 
    class ReadEnergy: public Request
    {
    public:

        //                 
        // Конструктор
        //

        ReadEnergy (CrcType nType = crc) : Request (rcReadEnergy, 2, nType)
        {
            m_nResponseSize = 16;
        }

        //
        // Тип накопленной энергии
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
        // Номер месяца - нумерация с 1 (январь)
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
        // Номер тарифа
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