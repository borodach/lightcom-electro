///////////////////////////////////////////////////////////////////////////////
//
//  File:           SIRTransport.hpp
//
//  Facility:       �������������� � �c����������
//
//
//  Abstract:       Serial IrDA ���������.
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
* $History: SIRTransport.hpp $
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

#ifndef _SIRTRANSPORT_H_
#define _SIRTRANSPORT_H_

#include "HandleTransport.hpp"

namespace Transport
{
    class SIRTransport: public HandleTransport
    {
    public:

        //
        // �����������.
        //

        SIRTransport ();
        
        //
        // ������������� ����������.
        // ���������� true, ���� �������� ������ �������.
        //

        virtual bool Init (const wchar_t *wcsTransportName);

        //
        // ��������������� ����������. ��� ������� ������� �������������.
        //

        virtual void Reset ();
 
        //
        // ������ ��� ���������� �� �������.
        //

        static wchar_t* ReadTransportNameFromRegostry (wchar_t * wcsBuffer, 
                                                       size_t nMaxCharCount);

        //
        // �������� ��� ������ � ������.
        //

        const COMMTIMEOUTS & GetTimeount () const;
        COMMTIMEOUTS & GetTimeount ();
        void SetTimeouts (COMMTIMEOUTS timeouts);

        //
        // ��������� �������� ��-���������.
        //

        void ResetTimeouts ();

        //
        // ������������� ������������� SIR ���������, ����� �� ������� 
        // ������ ��������.
        //

        bool AutoConfig ();

    protected:

        //
        // �������� ��� ������ � ������.
        //

        COMMTIMEOUTS  m_commTimeouts;        
    };
}

#endif _SIRTRANSPORT_H_