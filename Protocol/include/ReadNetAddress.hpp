///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadNetAddress.hpp
//
//  Facility:       ???????? ????????
//
//
//  Abstract:       ????? ?????? ??????? ?? ?????? ???????? ??????
//
//  Environment:    VC++ 8.0
//
//  Author:         ?????? ?.?.
//
//  Creation Date:  24-04-2006
//
//  Copyright (C) OOO "???????", 2006. ??? ????? ????????.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ReadNetAddress.hpp $
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

#ifndef _MERCURY_READ_ADDRESS_
#define _MERCURY_READ_ADDRESS_

#include "Request.hpp"

namespace Mercury
{
    
    class ReadNetAddress: public Request
    {
    public:

        //                 
        // ???????????
        //

        ReadNetAddress (CrcType nType = crc) : Request (rcReadParams, 1, nType)
        {
            m_Body [2] = 0x05;
            m_nResponseSize = 2;
        }
    };
}
#endif