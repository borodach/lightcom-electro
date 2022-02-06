///////////////////////////////////////////////////////////////////////////////
//
//  File:           HardwareId.cpp
//
//  Facility:       Уникальный идентификатор устройства
//
//
//  Abstract:       Класс, хранящий уникальный идентификатор устройства.
//                  Основная задача - персистенция.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  27-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include "HardwareId.hpp"
#include "Utils.h"

//
// Ключ первой версии
//

const unsigned char HardwareId::m_Version1Key [128] = 
{
    0x6c, 0x14, 0x75, 0x92, 0x17, 0xa2, 0x31, 0xf3, 
    0xdc, 0xa6, 0x06, 0x44, 0x28, 0xfb, 0x7c, 0x1b, 
    0x2a, 0xbb, 0xda, 0x66, 0x58, 0x42, 0x3c, 0xc2, 
    0x87, 0xa2, 0x55, 0xdc, 0xda, 0x55, 0x7b, 0xe7, 
    0x9a, 0x6e, 0x32, 0x9d, 0x3e, 0x15, 0xd8, 0xe7, 
    0x9b, 0x9a, 0x8d, 0xa8, 0xa2, 0xdc, 0x20, 0xb7, 
    0x90, 0x58, 0x02, 0x92, 0xfd, 0x9d, 0x03, 0xca, 
    0xac, 0x78, 0xf6, 0xbb, 0x8c, 0x96, 0xe4, 0x8f, 
    0xca, 0xed, 0xfa, 0x1a, 0x5f, 0xac, 0xd4, 0x93, 
    0x09, 0xfd, 0x52, 0xb4, 0x05, 0x65, 0xa2, 0x44, 
    0x8b, 0x05, 0x98, 0xa5, 0x5b, 0x81, 0x13, 0x4f, 
    0x55, 0xe2, 0x81, 0xdb, 0x79, 0x2e, 0x38, 0xa1, 
    0x43, 0xff, 0xc5, 0x5e, 0xbb, 0xe9, 0xdf, 0x0a, 
    0x2e, 0xf6, 0x20, 0x46, 0xf4, 0xf1, 0x2c, 0x7f, 
    0x25, 0xe1, 0x78, 0x49, 0xb6, 0x62, 0x4a, 0xf6, 
    0xcb, 0x3d, 0x26, 0xf1, 0xc4, 0xef, 0x46, 0xed
};

const unsigned char HardwareId::m_MD5Key [MD5_HASH_LEN] = 
{
    0x92, 0x0e, 0xab, 0x3c, 0xfe, 0xd2, 0x1b, 0x45, 
    0x3d, 0xd7, 0x67, 0x81, 0x98, 0x4e, 0x47, 0x67
};

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       HardwareId
//
//  Description:
//
//  Конструктор
//
///////////////////////////////////////////////////////////////////////////////

HardwareId::HardwareId (): m_wcsHardwareId (NULL)
{
    Reset ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~HardwareId
//
//  Description:
//
//  Деструктор
//
///////////////////////////////////////////////////////////////////////////////

HardwareId::~HardwareId ()
{
    Reset ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Reset
//
//  Description:
//
//  Очищает объект.
//
///////////////////////////////////////////////////////////////////////////////

void HardwareId::Reset ()
{
    m_nVersion = m_nLatestVersion;
    SetHardwareId (NULL);
}

     //
    // Доступ к буферу с hardware id
    //

    wchar_t* Detatch ();

    void SetHardwareId (wchar_t* wcsHardwareId);
    void Attatch (const wchar_t* wcsHardwareId);

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetHardwareId
//
//  Description:
//
//  Доступ к буферу с hardware id
//
///////////////////////////////////////////////////////////////////////////////

wchar_t* HardwareId::GetHardwareId ()
{
    return m_wcsHardwareId;    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetHardwareId
//
//  Description:
//
//  Доступ к буферу с hardware id
//
///////////////////////////////////////////////////////////////////////////////

const wchar_t* HardwareId::GetHardwareId () const
{
    return m_wcsHardwareId;    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Detatch
//
//  Description:
//
//  Забирает у объекта буфер с hardware id
//
///////////////////////////////////////////////////////////////////////////////

wchar_t* HardwareId::Detatch ()
{
    wchar_t* wcsResult = m_wcsHardwareId;
    m_wcsHardwareId = NULL;

    return wcsResult;    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetHardwareId
//
//  Description:
//
//  Записывает в объект переданный hardware id
//
///////////////////////////////////////////////////////////////////////////////

void HardwareId::SetHardwareId (const wchar_t* wcsHardwareId)
{
    delete [] m_wcsHardwareId;
    m_wcsHardwareId = NULL;
    if (NULL == wcsHardwareId) return;

    size_t nLen = wcslen (wcsHardwareId);
    m_wcsHardwareId = new wchar_t [nLen + 1];    
    wcscpy (m_wcsHardwareId, wcsHardwareId);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Attatch
//
//  Description:
//
//  Присоединяет к объекту буфер с hardware id
//
///////////////////////////////////////////////////////////////////////////////

void HardwareId::Attatch (wchar_t* wcsHardwareId)
{
    SetHardwareId (NULL);
    m_wcsHardwareId = wcsHardwareId;    
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetVersion
//
//  Description:
//
//  Возвращает версию объекта
//
///////////////////////////////////////////////////////////////////////////////

int HardwareId::GetVersion () const
{
    return m_nVersion;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetVersion
//                   
//  Description:
//
//  Устанавливает версию объекта
//
///////////////////////////////////////////////////////////////////////////////

void HardwareId::SetVersion (int nVersion)
{
    m_nVersion = nVersion ;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Encode
//                   
//  Description:
//
//  Кодирует данные и записывает их в буфер. Функция возыращает динамический 
//  буфер
//
///////////////////////////////////////////////////////////////////////////////

unsigned char* HardwareId::Encode (const void* key/* = m_Version1Key*/, 
                                   size_t nKeySize/* sizeof (m_Version1Key)*/) const
{
    size_t nDataSize = sizeof (wchar_t) * wcslen (m_wcsHardwareId);
    size_t nResultSize = 2 * (nDataSize + sizeof (m_nVersion)) + 1;
    unsigned char* szEncodedHardwareId = new unsigned char [nResultSize];
    if (NULL == szEncodedHardwareId) return NULL;
   
    XorBuffer ((unsigned char*) m_wcsHardwareId, 
               (unsigned char*) m_wcsHardwareId,  
               nDataSize,
               key,
               nKeySize);

    BinaryToHexString (&m_nVersion, 
                       szEncodedHardwareId, sizeof (m_nVersion));
    BinaryToHexString (m_wcsHardwareId, 
                       szEncodedHardwareId + 2 * sizeof (m_nVersion), nDataSize);

    XorBuffer (m_wcsHardwareId, 
               m_wcsHardwareId,  
               nDataSize,
               key,
               nKeySize);

    return szEncodedHardwareId;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Decode
//                   
//  Description:
//
//  Декодирует данные из buffer
//
///////////////////////////////////////////////////////////////////////////////

bool HardwareId::Decode (const unsigned char *szEncodedHardwareId,
                         const void* key/* = m_Version1Key*/, 
                         size_t nKeySize/* sizeof (m_Version1Key)*/)
{
    Reset ();
    size_t nLength = strlen ((char*) szEncodedHardwareId);
    if (nLength < sizeof (m_nVersion) * 2 + 2)
    {
        return false;
    }

    void* binaryData = HexStringToBinary (szEncodedHardwareId, 
                                                   &m_nVersion, 
                                                   sizeof (m_nVersion) * 2);        
    if (NULL == binaryData)
    {
        Reset ();
        return false;
    }
    if (m_nVersion <= 0 || m_nVersion > m_nLatestVersion)
    {
        Reset ();
        return false;
    }

    size_t nHardwareIdLength = (nLength / 2 - sizeof (m_nVersion)) / 2;
    m_wcsHardwareId = new wchar_t [1 + nHardwareIdLength];
    if (NULL == m_wcsHardwareId)
    {
        Reset ();
        return false;
    }
    m_wcsHardwareId [nHardwareIdLength] = 0;

    binaryData = HexStringToBinary (szEncodedHardwareId + sizeof (m_nVersion) * 2, 
                                    m_wcsHardwareId, 
                                    nLength - sizeof (m_nVersion) * 2);

    XorBuffer (m_wcsHardwareId,
               m_wcsHardwareId, 
               nHardwareIdLength * 2,
               key,
               nKeySize);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Encode
//                   
//  Description:
//
//  Генерирует ключ для заданной hardware id. Функция возыращает динамический 
//  буфер.
//
///////////////////////////////////////////////////////////////////////////////

unsigned char* HardwareId::GenerateKey (const wchar_t *wcsCommentString,
                                        const void* key/* = m_MD5Key*/, 
                                        size_t nKeySize/* = sizeof (m_MD5Key)*/) const
{
    char hash [MD5_HASH_LEN];
    memset (hash, 0, sizeof (hash));
    const void* pData [] = {wcsCommentString, m_wcsHardwareId};
    size_t dataSize [] = {(wcslen (wcsCommentString) + 1) * sizeof (wchar_t), 
                          (wcslen (m_wcsHardwareId) + 1) * sizeof (wchar_t)};
    bool bResult = SignDataWithMD5 (pData , 
                                   dataSize, 
                                   SIZE_OF (pData),
                                   hash);
    if (! bResult) 
    {
        return NULL;
    }

    size_t nResultSize = 2 * (MD5_HASH_LEN + sizeof (m_nVersion)) + 1;
    unsigned char* szResult = new unsigned char [nResultSize];
    if (NULL == szResult) return NULL;
    
    XorBuffer (hash, 
               hash,  
               MD5_HASH_LEN,
               key,
               nKeySize);

    BinaryToHexString (&m_nVersion, 
                       szResult, sizeof (m_nVersion));
    BinaryToHexString (hash, 
                       szResult + 2 * sizeof (m_nVersion), 
                       MD5_HASH_LEN);

    return szResult;    
}