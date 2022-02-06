///////////////////////////////////////////////////////////////////////////////
//
//  File:           LastError.hpp
//
//  Facility:       ��������� �� ������
//
//
//  Abstract:       ������� �����, �������� ��������� ��������� �� ������.
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
* $History: LastError.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Common
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Common
 * ������ ������
* 
*/

#include "Utils.h"
#include "LastError.hpp"
#include <string.h>
    
///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Manager
//
//  Description:
//
//  �����������
//
///////////////////////////////////////////////////////////////////////////////

LastError::LastError ()
{
    SetLastErrorString (NULL);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       LastError
//
//  Description:
//
//  ����������
//
///////////////////////////////////////////////////////////////////////////////

LastError::~LastError ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetLastErrorString
//
//  Description:
//
//  ������������� �������� ��������� ������.
//
///////////////////////////////////////////////////////////////////////////////

void LastError::SetLastErrorString (const wchar_t* wcsError)
{
    if (NULL != wcsError)
    {
        wcsncpy (m_wcsLastError, wcsError, MAX_LAST_ERROR_LENGTH - 1);
    }
    else
    {
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_wcsLastError); ++ nIdx)
        {
            m_wcsLastError [nIdx] =  0;    
        }    
    }
} 

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetLastErrorString
//
//  Description:
//
//  ���������� �������� ��������� ������.
//
///////////////////////////////////////////////////////////////////////////////

const wchar_t* LastError::GetLastErrorString ()
{
    return m_wcsLastError;
}