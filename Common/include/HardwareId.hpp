///////////////////////////////////////////////////////////////////////////////
//
//  File:           HardwareId.hpp
//
//  Facility:       ���������� ������������� ����������
//
//
//  Abstract:       �����, �������� ���������� ������������� ����������.
//                  �������� ������ - ������������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  27-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: HardwareId.hpp $
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

#ifndef _HARDWARE_ID_
#define _HARDWARE_ID_    

#include "Utils.h"

class HardwareId
{
public:
    
    //
    // �����������
    //

    HardwareId ();

    //
    // ����������
    //

    virtual ~HardwareId ();

    //
    // ������� ������.
    //

    void Reset ();

    //
    // ������ � ������ � hardware id
    //

    wchar_t* GetHardwareId ();
    const wchar_t* GetHardwareId () const;
    wchar_t* Detatch ();

    void SetHardwareId (const wchar_t* wcsHardwareId);
    void Attatch (wchar_t* wcsHardwareId);

    //
    // ������ � ������ �������
    //

    int GetVersion () const;
    void SetVersion (int nVersion);

    //
    // �������� ������ � ���������� �� � �����. ������� ���������� ������������ 
    // �����.
    //

    unsigned char* Encode (const void* key = m_Version1Key, 
                           size_t nKeySize = sizeof (m_Version1Key)) const;

    //
    // ���������� ������ �� buffer.
    //

    bool Decode (const unsigned char *szEncodedHardwareId,
                 const void* key = m_Version1Key, 
                 size_t nKeySize = sizeof (m_Version1Key));

    //
    // ���������� ���� ��� �������� hardware id. ������� ���������� ������������ 
    // �����.
    //

    unsigned char* GenerateKey (const wchar_t *wcsCommentString,
                                const void* key = m_MD5Key, 
                                size_t nKeySize = sizeof (m_MD5Key)) const;
public:

    //
    // ��������� ������ �������
    //

    static const int m_nLatestVersion = 1;    

protected:

    //
    // ���� ������ ������
    //

    static const unsigned char m_Version1Key [128];

    //
    // ���� ������ ������
    //

    static const unsigned char m_MD5Key [MD5_HASH_LEN];

    //
    // ������ �������
    //

    int m_nVersion;

    //
    // Hardware id buffer
    //

    wchar_t *m_wcsHardwareId;
};

#endif