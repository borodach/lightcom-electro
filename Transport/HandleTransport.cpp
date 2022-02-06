////////////////////////////////////////////////////////////////////////////////
//
//  File:           HandleTransport.cpp
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
* $History: HandleTransport.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Transport
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Transport
 * ������ ������
* 
*/

#include "HandleTransport.hpp"
namespace Transport
{
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       HandleTransport
    //
    //  Description:
    //
    //  �����������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    HandleTransport::HandleTransport (): m_hDevice (INVALID_HANDLE_VALUE)
    {
        m_wcsDeviceName [0] = 0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ~HandleTransport
    //
    //  Description:
    //
    //  ����������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    HandleTransport::~HandleTransport ()
    {
        Reset ();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetHahdle
    //
    //  Description:
    //
    //  ���������� ���������� ����������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    HANDLE HandleTransport::GetHahdle () const
    {
        return m_hDevice;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetDeviceName
    //
    //  Description:
    //
    //  ���������� ��� ����������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const wchar_t* HandleTransport::GetDeviceName () const 
    {
        return m_wcsDeviceName;
    }

#define INIT_PHASE wcsncpy (m_wcsLastError,  L"OK", MAX_LAST_ERROR_LENGTH - 1);\
    if (! IsInited ())\
    {\
        SetLastErrorString (L"��������� �� ������������������");\
        return false;\
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Read
    //
    //  Description:
    //
    //  ������ nCount ���� � ����� pBuffer.
    //  ���������� true, ���� �������� ������ �������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool HandleTransport::Read (unsigned char* pBuffer, size_t nCount)
    {
        INIT_PHASE;
        DWORD dwCount = nCount;
        BOOL bResult = ReadFile (m_hDevice, pBuffer, nCount, &dwCount, NULL);
        if (! bResult)
        {
            DWORD dwError = GetLastError ();
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"��� ������ �������� ������ � ����� %d", dwError);
            SetLastErrorString (wcsError);

            return false;        
        }
        if (nCount != dwCount)
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"���������� ����������� ������ �� ������������� ���������� (��������� %d �� %d)", 
                dwCount,
                nCount);
            SetLastErrorString (wcsError);

            return false;        
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Write
    //
    //  Description:
    //
    // ���������� nCount ���� �� ������ pBuffer.
    // ���������� true, ���� �������� ������ �������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool HandleTransport::Write (const unsigned char* pBuffer, size_t nCount)
    {
        INIT_PHASE;
        DWORD dwCount = 0;
        BOOL bResult = WriteFile (m_hDevice, pBuffer, nCount, &dwCount, NULL);

        if (! bResult)
        {
            DWORD dwError = GetLastError ();
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"��� ������ �������� ������ � ����� %d", dwError);
            SetLastErrorString (wcsError);

            return false;        
        }
        if (nCount != dwCount)
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"���������� ���������� ������ �� ������������� ���������� (�������� %d �� %d)", 
                dwCount,
                nCount);
            SetLastErrorString (wcsError);

            return false;        
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Init
    //
    //  Description:
    //
    //  ������������� ����������.
    //  ���������� true, ���� �������� ������ �������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool HandleTransport::Init (const wchar_t *wcsTransportName)
    {
        SetLastErrorString (L"OK");
        if (IsInited ()) 
        {
            Reset ();
        }

        wcscpy (m_wcsDeviceName, wcsTransportName);
        m_hDevice = CreateFileW (wcsTransportName, 
            GENERIC_READ | GENERIC_WRITE, 
            0,
            NULL, 
            OPEN_EXISTING, 
            0, 
            0);
        if (INVALID_HANDLE_VALUE == m_hDevice)
        {
            DWORD dwError = GetLastError ();
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"�� ������� ������� ���������� (��� ������ %d)", 
                dwError);
            SetLastErrorString (wcsError);
            Reset ();

            return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ReInit
    //
    //  Description:
    //
    //  ��������������� ����������.
    //  ���������� true, ���� �������� ������ �������.
    //
    /////////////////////////////////////////////////////////////////////////////// 

    bool HandleTransport::ReInit ()
    {
        return Init (GetDeviceName ());
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       IsInited
    //
    //  Description:
    //
    //  ���������� true, ���� ��������� ������������������.
    //
    /////////////////////////////////////////////////////////////////////////////// 

    bool HandleTransport::IsInited ()
    {
        return INVALID_HANDLE_VALUE != m_hDevice;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Reset
    //
    //  Description:
    //
    //  ��������������� ����������. ��� ������� ������� �������������.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void HandleTransport::Reset ()
    {
        if (! IsInited ()) return;
        CloseHandle (m_hDevice);
        m_hDevice = INVALID_HANDLE_VALUE;
    }
}

