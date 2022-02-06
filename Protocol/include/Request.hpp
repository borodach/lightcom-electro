///////////////////////////////////////////////////////////////////////////////
//
//  File:           Request.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� �������� ������ �������
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
* $History: Request.hpp $
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

#ifndef _MERCURY_REQUEST_
#define _MERCURY_REQUEST_

#include "Frame.hpp"

namespace Mercury
{
    
    class Request: public Frame
    {
    public:

        //
        // ��� ������ - ��� �������.
        //

        enum RequestCode : unsigned char
        {
            rcInvalid           = 0xFF, // ���������� ��� �������.
            rcTestConnection    = 0x00, // ������ �� �������� ������ �����
            rcConnect           = 0x01, // ������ �� ������������ ����������
            rcDisconnect        = 0x02, // ������ �� ������ ����������
            rcReadEventLog      = 0x04, // ������ �� ������ ������� �������
            rcReadEnergy        = 0x05, // ������ �� ������ �������� � ����������� �������
            rcReadPhysMemory    = 0x06, // ������ �� ������ ���������� ������
            rcReadParams        = 0x08  // ������ �� ������ ���������� � ������
        };


        //
        // �����������
        //

        Request (RequestCode nRequestType, size_t nSize = 0, CrcType nType = crc);

        //
        // ���������� ��� �������.
        //

        RequestCode GetRequestCode () const;

        //
        // ������ ���������� ������ (�� �������� ������� ����� � ����������� 
        // �����).
        //

        virtual size_t GetResponseSize () const;

    protected:

        //
        // ������������� ��� �������.
        //

        void SetRequestCode (RequestCode nCode);

        //
        // ������ ���������� ������ (�� �������� ������� ����� � ����������� 
        // �����).
        //

        size_t m_nResponseSize;
    };
}
#endif