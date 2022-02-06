///////////////////////////////////////////////////////////////////////////////
//
//  File:           Connect.cpp
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
* $History: Connect.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * ������ ������
* 
*/

#include "Connect.hpp"

//#define OLD_FORMAT

namespace Mercury
{
    //
    // ������ ��-��������� ��� ���������������� ������.
    //

    const Connect::Password Connect::m_Level1DefaultPwd = {1, 1, 1, 1, 1, 1};

    //
    // ������ ��-��������� ��� ������ "�������".
    //

    const Connect::Password Connect::m_Level2DefaultPwd = {2, 2, 2, 2, 2, 2};

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Connect
    //
    //  Description:
    //
    //  ����������� 
    //
    ///////////////////////////////////////////////////////////////////////////////

    Connect::Connect (ProtocolType protocolType/* = ptSET*/,
                      CrcType nType/* = crc*/, 
                      const Password &pwd/* = m_Level1DefaultPwd*/,
                      Connect::AccessLevel nLevel/* = Connect::alLevel1*/) : 
        Request (rcConnect, 
                (ptMercury == protocolType) ? sizeof (Connect::Password) + 1 : sizeof (Connect::Password), 
                nType),
        m_nProtocolType (protocolType)

    {
       SetAccessLevel (nLevel);
       SetPassword (pwd);
       m_nResponseSize = 1;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetPassword
    //
    //  Description:
    //
    //  ���������� ������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const Connect::Password& Connect::GetPassword () const
    {
        size_t nBase = 2;
        if (ptMercury == m_nProtocolType) 
        {
            nBase = 3;    
        }

        return (const Password &) *(m_Body + nBase);    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetPassword
    //
    //  Description:
    //
    //  ������������� ������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Connect::SetPassword (const Password& pwd)
    {
        size_t nBase = 2;
        if (ptMercury == m_nProtocolType) 
        {
            nBase = 3;    
        }

        for (size_t nIdx = 0; nIdx < sizeof (Password); ++ nIdx)
        {
            m_Body [nBase + nIdx] = pwd [nIdx];
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetAccessLevel
    //
    //  Description:
    //
    //  ���������� ������� �������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Connect::AccessLevel Connect::GetAccessLevel () const
    {
        if (ptMercury == m_nProtocolType)
        {
            return (AccessLevel) (m_Body [2]);
        }

        return alInvalid;            
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetAccessLevel
    //
    //  Description:
    //
    //  ������������� ������� �������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Connect::SetAccessLevel (Connect::AccessLevel nLevel)
    {
        if (ptMercury == m_nProtocolType)
        {
            m_Body [2] = nLevel;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetProtocolType
    //
    //  Description:
    //
    //  ���������� ��� ���������
    //
    ///////////////////////////////////////////////////////////////////////////////

    ProtocolType Connect::GetProtocolType () const
    {
        return m_nProtocolType;
    }


}
