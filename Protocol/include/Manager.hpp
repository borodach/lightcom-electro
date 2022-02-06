///////////////////////////////////////////////////////////////////////////////
//
//  File:           Manager.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       �����, ����������� ��������������� ������� ���������
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
* $History: Manager.hpp $
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

#ifndef _MERCURY_MANAGER_
#define _MERCURY_MANAGER_    

#include "LastError.hpp"
#include "Frame.hpp"
#include "Request.hpp"
#include "Connect.hpp"
#include "ITransport.hpp"

#ifndef MAX_LAST_ERROR_LENGTH
#define MAX_LAST_ERROR_LENGTH     512
#endif

namespace Mercury
{
    class Manager: public LastError
    {
    public:
        
        //
        // �����������
        //

        Manager ();

        //
        // ����������
        //

        virtual ~Manager ();

        //
        // �������� ��� ����������� �����.
        //

        Frame::CrcType DetectCrcType (Transport::ITransport &transport, 
                                      Frame::NetAddress nAddress = Mercury::Frame::m_nBroadcastNetAddress,
                                      size_t nWriteTimeout = 150,
                                      size_t nReadTimeout = 150);

        //
        // ��������� ����� ����� �� ���������.
        //

        ProtocolType Connect (Transport::ITransport &transport, 
                              Frame::CrcType crcType = Frame::crcInvalid,
                              Frame::NetAddress nAddress = Frame::m_nBroadcastNetAddress,
                              const Connect::Password &password = Mercury::Connect::m_Level1DefaultPwd,
                              Connect::AccessLevel accessLevel = Mercury::Connect::alLevel1,
                              size_t nWriteTimeout = 150,
                              size_t nReadTimeout = 150);

        //
        // ���������� ����� � �������.
        //

        bool WriteFrame (Transport::ITransport &transport, Frame &frame);

        //
        // ������ ����� �� ��������.
        //

        bool ReadFrame (Transport::ITransport &transport, Frame &frame);

        //
        // ��������� ������ � ���������� ����� � ����� pResponse.
        // ����� ������ ����� ������ �� ����� frame.GetResponseSize () ����.
        //

        bool SendRequest (Transport::ITransport &transport, 
                          Request &frame, 
                          unsigned char *pResponse,
                          size_t nWriteTimeout = 150,
                          size_t nReadTimeout = 150,
                          size_t nRetryCount = 3);

    public:

        //
        // ����������� ��������� ������.
        //

        static Manager m_Instance;

    };
}
#endif