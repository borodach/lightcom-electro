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
* $History: LastError.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Common/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Common/include
* 
*/

#ifndef _LAST_ERROR_
#define _LAST_ERROR_    

#ifndef MAX_LAST_ERROR_LENGTH
#define MAX_LAST_ERROR_LENGTH     1024
#endif

class LastError
{
public:
    
    //
    // �����������
    //

    LastError ();

    //
    // ����������
    //

    virtual ~LastError ();


    //
    // ���������� ����� ��������� ������.
    //

    const wchar_t* GetLastErrorString ();


    //
    // ������������� ����� ��������� ������
    //

    void SetLastErrorString (const wchar_t*);

protected:

    //
    // ����� ��������� ������.
    //

    wchar_t m_wcsLastError [MAX_LAST_ERROR_LENGTH];
};

#endif