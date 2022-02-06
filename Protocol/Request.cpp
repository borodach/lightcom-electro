///////////////////////////////////////////////////////////////////////////////
//
//  File:           Request.cpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс базового фрейма запроса
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  24-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Request.cpp $
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

#include "Request.hpp"

namespace Mercury
{                            

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Request
    //
    //  Description:
    //
    //  Конструктор.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Request::Request (RequestCode nRequestType, size_t nSize/* = 0*/, CrcType nType/* = crc*/) : 
        Frame (nSize + 1, nType), m_nResponseSize (1)
    {
        SetRequestCode (nRequestType);
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetRequestCode
    //
    //  Description:
    //
    //  Возвращает код запроса.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Request::RequestCode Request::GetRequestCode () const
    {
        return (RequestCode) m_Body [1];    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetResponseSize
    //
    //  Description:
    //
    //  Размер ожидаемого ответа (не включает сетевой адрес и контрольную 
    //  сумму).
    //
    ///////////////////////////////////////////////////////////////////////////////

    size_t Request::GetResponseSize () const
    {
        return m_nResponseSize;
    }

    //
    // Устанавливает код запроса.
    //

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetRequestCode
    //
    //  Description:
    //
    //  Устанавливает код запроса.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Request::SetRequestCode (Request::RequestCode nCode)
    {
        m_Body [1] = nCode;
        CalculateAndStoreCrc ();
    } 
}
