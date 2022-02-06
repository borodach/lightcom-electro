///////////////////////////////////////////////////////////////////////////////
//
//  File:           ITransport.cpp
//
//  Facility:       �������������� � �c����������
//                                                         
//
//  Abstract:       ��������� ��� I/O � �����������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  22-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ITransport.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Transport/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Transport/include
* 
*/

#ifndef _ITRANSPORT_H_
#define _ITRANSPORT_H_

#include "LastError.hpp"

namespace Transport
{
    class ITransport: public LastError
    {
        public:

        //
        // ������ nCount ���� � ����� pBuffer.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Read (unsigned char* pBuffer, size_t nCount) = 0;

        //
        // ���������� nCount ���� �� ������ pBuffer.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Write (const unsigned char* pBuffer, size_t nCount) = 0;

        //
        // ������������� ����������.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Init (const wchar_t *wcsTransportName) = 0;

        //
        // ��������������� ����������.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool ReInit () = 0;

        //
        // ���������� true, ���� ��������� ������������������.
        //

        virtual bool IsInited () = 0;

        //
        // ��������������� ����������. ��� ������� ������� �������������.
        //

        virtual void Reset () = 0;

        //
        // ����������.
        //

        virtual ~ITransport () {};
    };
}

#endif _ITRANSPORT_H_