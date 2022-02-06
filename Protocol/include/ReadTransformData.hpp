///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadTransformData.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма запроса на чтение коэффициентов трансформации
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
* $History: ReadTransformData.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _MERCURY_READ_TRANSFORM_DATA_
#define _MERCURY_READ_TRANSFORM_DATA_

#include "Request.hpp"

namespace Mercury
{
    
    class ReadTransformData: public Request
    {
    public:

        //
        // Конструктор
        //

        ReadTransformData (ProtocolType protocolType = ptSET,
                           CrcType nType = crc) : 
        Request (rcReadParams, 1, nType)
        {
            m_Body [2] = 0x02;
            if (ptSET == protocolType)
            {
                m_nResponseSize = 10;
            }
            else
            {
                m_nResponseSize = 4;
            }
        }
    };
}
#endif