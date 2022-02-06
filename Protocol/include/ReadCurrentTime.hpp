///////////////////////////////////////////////////////////////////////////////
//
//  File:           ReadCurrentTime.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ������ ������� �� ������ �������� ������� ��������.
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
* $History: ReadCurrentTime.hpp $
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

#ifndef _MERCURY_READ_CURRENT_TIME_
#define _MERCURY_READ_CURRENT_TIME_

#include "Request.hpp"

namespace Mercury
{
    class ReadCurrentTime: public Request
    {
    public:

        //
        // �����������
        //

        ReadCurrentTime (CrcType nType = crc) : Request (rcReadEventLog, 1, nType)
        {
            m_Body [2] = 0x00;
            m_nResponseSize = 8;
        }
    };
}
#endif