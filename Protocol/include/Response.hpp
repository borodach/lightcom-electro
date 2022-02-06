///////////////////////////////////////////////////////////////////////////////
//
//  File:           Response.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ������ ������������ ������
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
* $History: Response.hpp $
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

#ifndef _MERCURY_RESPONSE_
#define _MERCURY_RESPONSE_

#include "Frame.hpp"

namespace Mercury
{
    
    class Response: public Frame
    {
    public:

        //
        // ���� ������ ��������.
        //

        enum ResponseCode : unsigned char
        {
            rcInvalid           = 0xFF, // ���������� ��� ������.
            rcOK                = 0x00, // ��� ���������.
            rcInvalidArg        = 0x01, // ������������ ������� ��� ��������.
            rcInternalError     = 0x02, // ���������� ������ ��������.
            rcAccessDenied      = 0x03, // �� ���������� ������� ������� ��� �������������� �������.
            rcTimerAdjustDenied = 0x04, // ���������� ���� �������� ��� ���������������� � ������� ������� �����.
            rcNoConnected       = 0x05, // �� ������ ����� �����
            rcTryLater          = 0x06, // ��������� ������ � ������� 0,5 � (���-4��.02 c V15.XX.XX � ����) 
            rcResultNotReady    = 0x07  // �� ����� ��������� ��������� �� �������������� ��������� (�� ����������� ����� �������������� ����� ����� ����������) (���-4��.02 c V23.XX.XX � ����)
        };

        //
        // �����������
        //

        Response (CrcType nType = crc);

        //
        // ���������� ������, ����������� ��� ������ ��������.
        //

        static const wchar_t* GetResponseString (const ResponseCode nCode);

        //
        // ���������� ��� ������.
        //

        ResponseCode GetResponseCode () const;

        //
        // ������������� ��� ������.
        //

        void SetResponseCode (const ResponseCode nCode);

        //
        // ���������� ������� ������ (� ��������).
        //

        unsigned int GetTimeout () const;

        //
        // ������������� ������� ������ ((� ��������)).
        //

        void SetTimeout (unsigned int nTimeout);
    };
}
#endif