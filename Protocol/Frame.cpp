///////////////////////////////////////////////////////////////////////////////
//
//  File:           Frame.cpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма протокола. Реализует подсчет контрольной суммы.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  22-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Frame.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * Первая версия
* 
*/

#include "Frame.hpp"

namespace Mercury
{

    //
    // Массивы для вычисления контрольной суммы в стиле MODBUS.
    //

    const unsigned char Frame::m_srCRCHi [] =
    {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
        0x00, 0xC1, 0x81, 0x40
    };

    const unsigned char Frame::m_srCRCLo [] =
    {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 
        0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 
        0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 
        0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 
        0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 
        0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 
        0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
        0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 
        0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
        0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 
        0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
        0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 
        0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 
        0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 
        0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 
        0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 
        0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 
        0x41, 0x81, 0x80, 0x40
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Frame
    //
    //  Description:
    //
    //  Конструктор.
    //  nSize - Размер фрейма без полей сетевого адреcа и контрольной суммы
    //  nType - Тип контрольной суммы.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::Frame (size_t nSize/* = 0*/, 
                  Frame::CrcType nType/* = crc*/) : 
        m_crcType (nType), m_nSize (modbus == nType ? nSize + 3 : nSize + 2)
    {
        m_Body = new unsigned char [m_nSize];
        for (size_t nIdx = 0; nIdx < m_nSize; ++ nIdx)
        {
            m_Body [nIdx] = 0;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ~Frame
    //
    //  Description:
    //
    //  Деструктор
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::~Frame ()
    {
        delete [] m_Body;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetFrameBoby
    //
    //  Description:
    //
    //  Возвращает байты тела фрейма.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::FrameBody Frame::GetFrameBoby () const
    {
        return m_Body;    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetFrameSize
    //
    //  Description:
    //
    //  Возвращает полный размер фрейма в байтах.
    //
    ///////////////////////////////////////////////////////////////////////////////

    int Frame::GetFrameSize () const
    {
        return m_nSize;
    }        

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetCRCType
    //
    //  Description:
    //
    //  Возвращает тип контрольной суммы.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::CrcType Frame::GetCRCType () const
    {
        return m_crcType;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetAddress
    //
    //  Description:
    //
    //  Возвращает сетевой адрес.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::NetAddress Frame::GetAddress () const
    {
        if (m_nSize < 1) return m_nBroadcastNetAddress;
        return m_Body [0];
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetAddress
    //
    //  Description:
    //
    //  Устанавливает сетевой адрес.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Frame::SetAddress (Frame::NetAddress address)
    {
        if (m_nSize < 1) return;
        m_Body [0] = address;
        CalculateAndStoreCrc ();
    }
      
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       CalculateCrc
    //
    //  Description:
    //
    //  Вычисление контрольной суммы для фрейма. 
    //  Возвращает контрольную сумму.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::CRCValue Frame::CalculateCrc ()
    {
        if (crc == m_crcType)
        {
            return CalculateCrc (m_Body, 0, m_nSize - 1);
        }
        else
        {
            return CalculateModbus (m_Body, 0, m_nSize - 2);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       CalculateAndStoreCrc
    //
    //  Description:
    //
    //  Вычисление контрольной суммы фрейма и запись ее во фрейм. 
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Frame::CalculateAndStoreCrc ()
    {
        SetCrc (CalculateCrc ());
    }
         
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetCrc
    //
    //  Description:
    //
    //  Возвращает контрольную сумму, хранимую в теле фрейма.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::CRCValue Frame::GetCrc () const
    {
        if (crc == m_crcType)
        {
            if (m_nSize < 1) return 0;
            return (Frame::CRCValue) m_Body [m_nSize - 1];
        }
        else
        {
            if (m_nSize < 2) return 0;
            Frame::CRCValue nResult = m_Body [m_nSize - 2] + 256 * m_Body [m_nSize - 1];
            return nResult;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetCrc
    //
    //  Description:
    //
    //  Записывает контрольную сумму в тело фрейма.
    //
    ///////////////////////////////////////////////////////////////////////////////
    void Frame::SetCrc (Frame::CRCValue crcValue)
    {
        if (crc == m_crcType)
        {
            if (m_nSize < 1) return;
            m_Body [m_nSize - 1] = (unsigned char) (crcValue % 256);
        }
        else
        {
            if (m_nSize < 2) return;
            m_Body [m_nSize - 2] = (unsigned char) (crcValue % 256);
            m_Body [m_nSize - 1] = (unsigned char) ((crcValue / 256) % 256);
        }
    }
        
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       CalculateCrc
    //
    //  Description:
    //
    //  Вычисление простой контрольной суммы. 
    //      bytes - Данные, для которых считается контрольная сумма.
    //      start - Индекс первого байта, с которого начинается  подсчет
    //      count - Количество байтов, участвующих в подсчете
    //  Возвращает контрольную сумму 
    //
    ///////////////////////////////////////////////////////////////////////////////    

    Frame::CRCValue Frame::CalculateCrc (unsigned char *bytes, 
                           size_t nStart, 
                           size_t nCount)
    {
        unsigned char nResult = (unsigned char) 0xFF;
        for (size_t nIdx = 0; nIdx < nCount; ++nIdx)
        {
            nResult ^= bytes [nIdx + nStart];
        }

        return nResult;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       CalculateModbus
    //
    //  Description:
    //
    //  Вычисление контрольной суммы аналогично MODBUS. 
    //      bytes - Данные, для которых считается контрольная сумма.
    //      start - Индекс первого байта, с которого начинается  подсчет
    //      count - Количество байтов, участвующих в подсчете
    //  Возвращает контрольную сумму. 
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::CRCValue Frame::CalculateModbus (unsigned char *bytes, 
                                            size_t nStart, 
                                            size_t nCount)
    {
        unsigned short nResult = m_nInitCRC;
        for (size_t nIdx = 0; nIdx < nCount; ++nIdx)
        {
            unsigned char lo = (unsigned char) (nResult % 256);
            unsigned char hi = (unsigned char) (nResult / 256);
            int i = lo ^ bytes [nIdx + nStart];
            lo = (unsigned char) (hi ^ (int) m_srCRCHi [i]);
            hi = m_srCRCLo [i];

            nResult = (unsigned short) (lo + (unsigned short) (hi) * 256);
        }

        return nResult;
    }
}
