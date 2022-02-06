///////////////////////////////////////////////////////////////////////////////
//
//  File:           Utils.h
//
//  Facility:       ��������������� �������
//
//
//  Abstract:       ��������������� �������, � �������� ����������� 
//                  �������������� ������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  26-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Utils.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Common/include
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Common/include
* 
*/

#ifndef _UTILS_
#define _UTILS_

#define SIZE_OF(a) (sizeof (a) / sizeof (a [0]))

//
// ����� MD5 ����.
//

#ifndef MD5_HASH_LEN
#define MD5_HASH_LEN  16
#endif

#include <stdio.h>
#include <windows.h>

//
//  ������ ������������ ����� ����� �� ��������� ������.
//

unsigned short ShortFromBinary (void *pData);

//
//  ������ ��������������� ����� ����� �� ��������� ������.
//

unsigned int IntFromBinary (void *pData);

//
//  ����������� �������-���������� ����� � �������� ���
//

unsigned char IntFromBCD (unsigned char data);

//
//  ����������� �������� ����� ��� �������-���������� ���
//

unsigned char BCDFromInt (unsigned char data);

//
//  ��������� �������� dstData = srcData xor key
//

void XorBuffer (const void *srcData, 
                void *dstData, 
                size_t nDataSize,
                const void *key,
                size_t keySize);

//
//  �������� �������� ����� � ������ dstData, ���������� ������ ���� � ��� 
//  ������� - ������������� ����� � ����������������� ����.
//

void* BinaryToHexString (const void *srcData, 
                         void *dstData, 
                         size_t nSrcDataSize);
//
//  ���������� hex-encoded ����� � �������� �����.
//

void* HexStringToBinary (const void *srcData, 
                         void *dstData, 
                         size_t nSrcDataSize);

//
//  ���������� ������ � ������ ����� �� ����� � ������ ��������� �����������
//  ���� ������� ����������. � ����� ��� ��������.
//

wchar_t* GetProgramFolder (wchar_t *wcsProgramFolder, size_t nCount);

//
// ���������� ������ � ������ ����� �� ����� � ������ ��������� �����������
// ���� ������� ����������. � ����� ��� ��������.
//

template <class T>
const T& GetProgramFolder (T& strProgramFolder)
{
    wchar_t wcsProgramFolder [1024] = L"";
    GetModuleFileName (NULL, wcsProgramFolder, 
                       SIZE_OF (wcsProgramFolder));
    int nLen = (int) wcslen (wcsProgramFolder);
    for (int nIdx = nLen - 1; nIdx >= 0; -- nIdx)
    {
        if ('\\' == wcsProgramFolder [nIdx])
        {
            wcsProgramFolder [nIdx] = 0;
            break;
        }
    }
    strProgramFolder = wcsProgramFolder;
    
    return strProgramFolder;
}


//
//  ��������� MD5 ��� ��� ������������������� ������ �� pData (� dataSize
//  �������� ����� ���� �������������������). ���������� �������������������
//  ����� nCount.
//  ��� ������������ � ����� pHashValue. ������ ���� ������ ����� MD5_HASH_LEN 
//  ��� 16 ������.
//

bool SignDataWithMD5 (const void *pData [], 
                      size_t dataSize [],
                      size_t nCount,
                      void *pHashValue);

//
//  ��������� MD5 ��� ��� ������ pData.  nDataSize - ���������� ���� � ������.
//
//  ��� ������������ � ����� pHashValue. ������ ���� ������ ����� MD5_HASH_LEN 
//  ��� 16 ������.
//


bool SignDataWithMD5 (const void *pData, 
                      size_t nDataSize,                      
                      void *pHashValue);

//
//  ���������� � ����� ���������� ����� ������ � ����������� ��������.
//

bool SignFile (const char *szFileName,
               const void *key,
               size_t nKeySize);
bool SignFile (const wchar_t *wcsFileName,
               const void *key,
               size_t nKeySize);
bool SignFile (FILE *pFile,
               const void *key,
               size_t nKeySize);

//
//  ��������� ����������� ������������ �����.
//

bool VerifySignedFile (const char *szFileName,
                       const void *key,
                       size_t nKeySize);
bool VerifySignedFile (const wchar_t *wcsFileName,
                       const void *key,
                       size_t nKeySize);
bool VerifySignedFile (FILE *pFile,
                       const void *key,
                       size_t nKeySize);

//
// ����������� UNICODE ������ � multibyte ������.
//

template <class T>
bool WideChar2MultiByte (const wchar_t* wcsString, T& strOutput, DWORD dwCodePage = CP_ACP)
{
    strOutput = "";
    if (wcsString [0] == 0)
    {
        return true;
    }

    int nSize = WideCharToMultiByte (dwCodePage, 0, wcsString, -1, NULL, 0, NULL, NULL);
    if (nSize <= 0)
    {
        return false;
    }

    char *buffer = new char [nSize];
    if (NULL == buffer) 
    {
        return false;
    }

    int nResult = WideCharToMultiByte (dwCodePage, 0, wcsString, -1, buffer, nSize, NULL, NULL);
    if (nResult <= 0)
    {
        delete [] buffer;
        return false;
    }

    strOutput = buffer;
    delete [] buffer;

    return true;
}

//
// ����������� UNICODE ������ � multibyte ������.
//

template <class T>
bool MultiByte2WideChar (const char* szString, T& strOutput, DWORD dwCodePage = CP_ACP)
{
    strOutput = L"";
    if (szString [0] == 0)
    {
        return true;
    }

    int nSize = MultiByteToWideChar (dwCodePage, 0, szString, -1, NULL, 0);
    if (nSize <= 0)
    {
        return false;
    }

    wchar_t *buffer = new wchar_t [nSize];
    if (NULL == buffer) 
    {
        return false;
    }

    int nResult = MultiByteToWideChar (dwCodePage, 0, szString, -1, buffer, nSize);
    if (nResult <= 0)
    {
        delete [] buffer;
        return false;
    }

    strOutput = buffer;
    delete [] buffer;

    return true;
}

#endif