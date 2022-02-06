///////////////////////////////////////////////////////////////////////////////
//
//  File:           Response.cpp
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
* $History: Response.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * ������ ������
* 
*/

#include "Utils.h"
#include "Response.hpp"

namespace Mercury
{
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Response
    //
    //  Description:
    //
    //  �����������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Response::Response (CrcType nType/* = crc*/) : Frame (1, nType)
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetResponseString
    //
    //  Description:
    //
    //  ���������� ������, ����������� ��� ������ ��������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const wchar_t* Response::GetResponseString (const Response::ResponseCode nCode)
    {
        static const wchar_t *wcsDefaultResponse = L"����������� ���";
        struct ResponseInfo
        {
            ResponseCode    nResponseCode;
            const wchar_t*  wcsResponseString;
        };

        static ResponseInfo table [] = 
        {
            {rcInvalid,             L"���������� ��� ������"},
            {rcOK,                  L"��� ���������"},
            {rcInvalidArg,          L"������������ ������� ��� ��������"},
            {rcInternalError,       L"���������� ������ ��������"},
            {rcAccessDenied,        L"�� ���������� ������� ������� ��� �������������� �������"},
            {rcTimerAdjustDenied,   L"���������� ���� �������� ��� ���������������� � ������� ������� �����"},
            {rcNoConnected,         L"�� ������ ����� �����"},
            {rcTryLater,            L"��������� ������ � ������� 0,5 � (���-4��.02 c V15.XX.XX � ����)"}, 
            {rcResultNotReady,      L"�� ����� ��������� ��������� �� �������������� ��������� (�� ����������� ����� �������������� ����� ����� ����������) (���-4��.02 c V23.XX.XX � ����)"}
        };

        size_t nCount = SIZE_OF (table);
        for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
        {
            if (nCode == table [nIdx].nResponseCode)
            {
                return table [nIdx].wcsResponseString;
            }
        }

        return wcsDefaultResponse;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetResponseCode
    //
    //  Description:
    //
    //  ���������� ��� ������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Response::ResponseCode Response::GetResponseCode () const
    {
        return (ResponseCode) (0x0f & m_Body [1]);    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetResponseCode
    //
    //  Description:
    //
    //  ������������� ��� ������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Response::SetResponseCode (const Response::ResponseCode nCode)
    {
        m_Body [1] &= 0xf0;
        m_Body [1] |= nCode;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetTimeout
    //
    //  Description:
    //
    //  ���������� ������� ������ (� ��������).
    //
    ///////////////////////////////////////////////////////////////////////////////

    unsigned int Response::GetTimeout () const
    {
        return (m_Body [1] >> 4) & 0x0f;    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetTimeout
    //
    //  Description:
    //
    //  ������������� ������� ������ ((� ��������)).
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Response::SetTimeout (unsigned int nTimeout)
    {
        m_Body [1] &= 0x0f;
        m_Body [1] |= (nTimeout << 4) & 0xf0;
    }
}
