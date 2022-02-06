///////////////////////////////////////////////////////////////////////////////
//
//  File:           Connect.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ������ ������� �� ������������ ����������
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
* $History: Connect.hpp $
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

#ifndef _MERCURY_CONNECT_
#define _MERCURY_CONNECT_    

#include "Request.hpp"

namespace Mercury
{
    
    class Connect: public Request
    {
    public:

        //
        // ��� ������ - ������� �������.
        //

        enum AccessLevel : unsigned char
        {
            alInvalid = 0xFF,
            alLevel1  = 0x01,   // ����� ������������
            alLevel2  = 0x02    // ����� �������
        };

        //
        // ��� ������ - ������.
        //

        typedef unsigned char Password [6];

        //
        // ������ ��-��������� ��� ���������������� ������.
        //

        static const Password m_Level1DefaultPwd;

        //
        // ������ ��-��������� ��� ������ "�������".
        //

        static const Password m_Level2DefaultPwd;

        //
        // �����������
        //

        Connect (ProtocolType protocolType = ptSET,
                 CrcType nType = crc, 
                 const Password &pwd = m_Level1DefaultPwd,
                 Connect::AccessLevel nLevel = Connect::alLevel1);


        //
        // ���������� ������.
        //

        const Password&  GetPassword () const;
       
        //
        // ������������� ������.
        //

        void SetPassword (const Password& pwd);

        //
        // ���������� ������� �������.
        //

        AccessLevel GetAccessLevel () const;
       
        //
        // ������������� ������� �������.
        //

        void SetAccessLevel (AccessLevel nLevel);

        //
        // ���������� ��� ���������.
        //

        ProtocolType GetProtocolType () const;

    protected:
        
        //
        // ��� ��������
        //

        ProtocolType m_nProtocolType;
    };
}
#endif