///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadPower.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма запроса на чтение средних мощностей
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  30-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ReadPower.hpp $
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

#ifndef _MERCURY_READ_POWER_
#define _MERCURY_READ_POWER_

#include "Request.hpp"

namespace Mercury
{
 
    class ReadPower: public Request
    {
    public:

        //                 
        // Конструктор
        //

        ReadPower (CrcType nType = crc) : Request (rcReadPhysMemory, 4, nType)
        {
            m_Body [2] = 0x03;
            m_Body [5] = 15;
            m_nResponseSize = 15;
            SetMemoryAddress (0x0000);
        }

        //
        // Адрес памяти
        //

        unsigned int GetMemoryAddress () const
        {
            unsigned short nResult = (unsigned short) 256 * m_Body [3] + m_Body [4];
            if (m_Body [2] & 0x80) 
            {
                nResult |= 0x10000;
            }

            return nResult;
        }

        void SetMemoryAddress (unsigned int nAddress)
        {
            m_Body [3] = (nAddress >> 8) & 0xFF;
            m_Body [4] = nAddress & 0xFF;
            if (nAddress & 0x10000) 
            {
                m_Body [2] = 0x83;    
            }
            else
            {
                m_Body [2] = 0x03;
            }

            CalculateAndStoreCrc ();
        }

    };
}
#endif