///////////////////////////////////////////////////////////////////////////////
//
//  File:           Utils.h
//
//  Facility:       Вспомогательные функции
//
//
//  Abstract:       Вспомогательные функции, в основном выполняющие 
//                  преобразование данных.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  26-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
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
// Длина MD5 хэша.
//

#ifndef MD5_HASH_LEN
#define MD5_HASH_LEN  16
#endif

#include <stdio.h>
#include <windows.h>

//
//  Читает двухбайтовое целое число из бинарного бцфера.
//

unsigned short ShortFromBinary (void *pData);

//
//  Читает четырехбайтовое целое число из бинарного бцфера.
//

unsigned int IntFromBinary (void *pData);

//
//  Преобразует двоично-десятичное число в двоичный вид
//

unsigned char IntFromBCD (unsigned char data);

//
//  Преобразует двоичное число вид двоично-десятичный вид
//

unsigned char BCDFromInt (unsigned char data);

//
//  Выполняет операцию dstData = srcData xor key
//

void XorBuffer (const void *srcData, 
                void *dstData, 
                size_t nDataSize,
                const void *key,
                size_t keySize);

//
//  Кодирует бинарный буфер в строку dstData, преобразуя каждый байт в два 
//  символа - представление байта в шестнадцатиричном виде.
//

void* BinaryToHexString (const void *srcData, 
                         void *dstData, 
                         size_t nSrcDataSize);
//
//  Декодирует hex-encoded стоку в бинарный буфер.
//

void* HexStringToBinary (const void *srcData, 
                         void *dstData, 
                         size_t nSrcDataSize);

//
//  Возвращает строку с полным путем ло папки в котрой находится исполняемый
//  файл данного приложения. В конце нет бэкслэша.
//

wchar_t* GetProgramFolder (wchar_t *wcsProgramFolder, size_t nCount);

//
// Возвращает строку с полным путем ло папки в котрой находится исполняемый
// файл данного приложения. В конце нет бэкслэша.
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
//  Вычисляет MD5 хэш для последовательностей байтов из pData (в dataSize
//  хранятся длины этих последовательностей). Количество последовательностей
//  равно nCount.
//  Хэш записывается в буфер pHashValue. Размер хэша всегда равен MD5_HASH_LEN 
//  или 16 байтам.
//

bool SignDataWithMD5 (const void *pData [], 
                      size_t dataSize [],
                      size_t nCount,
                      void *pHashValue);

//
//  Вычисляет MD5 хэш для буфера pData.  nDataSize - количество байт в буфере.
//
//  Хэш записывается в буфер pHashValue. Размер хэша всегда равен MD5_HASH_LEN 
//  или 16 байтам.
//


bool SignDataWithMD5 (const void *pData, 
                      size_t nDataSize,                      
                      void *pHashValue);

//
//  Записывает в конец текстового файла строку с электронной подписью.
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
//  Проверяет целостность подписанного файла.
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
// Преобразует UNICODE строку в multibyte строку.
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
// Преобразует UNICODE строку в multibyte строку.
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