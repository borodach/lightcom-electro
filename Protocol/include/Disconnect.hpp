///////////////////////////////////////////////////////////////////////////////
//
//  File:           Disconnect.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма запроса на разрыв соединения
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
* $History: Disconnect.hpp $
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

#ifndef _MERCURY_DISCONNECT_
#define _MERCURY_DISCONNECT_

#include "Request.hpp"

namespace Mercury
{
    
    class Disconnect: public Request
    {
    public:

        //
        // Конструктор
        //

        Disconnect (CrcType nType = crc) : Request (rcDisconnect, 0, nType)
        {
            m_nResponseSize = 1;
        }
    };
}
#endif