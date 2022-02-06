///////////////////////////////////////////////////////////////////////////////
//
//  File:           HandleTransport.hpp
//
//  Facility:       �������������� � �c����������
//
//
//  Abstract:       Windows handle ���������.
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
* $History: HandleTransport.hpp $
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

#ifndef _HANDLETRANSPORT_H_
#define _HANDLETRANSPORT_H_

#include <windows.h>
#include <string.h>
#include "ITransport.hpp"

#define MAX_TRANSPORT_NAME_LENGTH 16

namespace Transport
{
    class HandleTransport: public ITransport
    {
    public:

        //
        // �����������.
        //

        HandleTransport ();

        //
        // ����������.
        //

        virtual ~HandleTransport ();

        //
        // ������ nCount ���� � ����� pBuffer.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Read (unsigned char* pBuffer, size_t nCount);

        //
        // ���������� nCount ���� �� ������ pBuffer.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Write (const unsigned char* pBuffer, size_t nCount);

        //
        // ������������� ����������.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Init (const wchar_t *wcsTransportName);

        //
        // ��������������� ����������.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool ReInit ();

        //
        // ���������� true, ���� ��������� ������������������.
        //

        virtual bool IsInited ();

        //
        // ��������������� ����������. ��� ������� ������� �������������.
        //

        virtual void Reset ();
        
        //
        // ���������� ���������� ����������
        //

        HANDLE GetHahdle () const;

        //
        // ���������� ��� ����������
        //

        const wchar_t*  GetDeviceName () const;

    protected:

        //
        // ���������� ����������.
        //

        HANDLE m_hDevice;

        //
        // ��� ����������
        //

        wchar_t m_wcsDeviceName [512];
    };
}
#endif