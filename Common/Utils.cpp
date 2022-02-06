///////////////////////////////////////////////////////////////////////////////
//
//  File:           Utils.hpp
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
* $History: Utils.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Common
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:16
 * Created in $/LightCom/C++/Electro/ManagerPPC/Common
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 19.01.07   Time: 21:55
 * Updated in $/PPCMercury.root/Common
 * Исправлены мелкие ошибки
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Common
 * Первая версия
* 
*/

#include <windows.h>
#include <wincrypt.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include <Utils.h>

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShortFromBinary
//
//  Description:
//
//  Читает двухбайтовое целое число из бинарного бцфера.
//
///////////////////////////////////////////////////////////////////////////////

unsigned short ShortFromBinary (void *pData)
{
    unsigned char *chData = (unsigned char *) pData;
    unsigned short nResult = chData [0] * 256 + chData [1];
    return nResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       IntFromBinary
//
//  Description:
//
//  Читает четырехбайтовое целое число из бинарного бцфера.
//
///////////////////////////////////////////////////////////////////////////////

unsigned int IntFromBinary (void *pData)
{
    unsigned char *chData = (unsigned char *) pData;
    unsigned int nResult = chData [0] * 256ul * 256ul * 256ul + 
                           chData [1] * 256ul * 256ul + 
                           chData [2] * 256ul + 
                           chData [3];
    return nResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       IntFromBCD
//
//  Description:
//
//  Преобразует двоично-десятичное число в двоичный вид
//
///////////////////////////////////////////////////////////////////////////////

unsigned char IntFromBCD (unsigned char data)
{
    unsigned char nResult = (data & 0x0F) + 10 * ((data >> 4) & 0x0F);
    return nResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       BCDFromInt
//
//  Description:
//
//  Преобразует двоичное число в двоично-десятичный вид
//
///////////////////////////////////////////////////////////////////////////////

unsigned char BCDFromInt (unsigned char data)
{
    unsigned char nResult = ((data % 10) & 0x0F) +  (((data / 10) << 4) & 0xF0);
    return nResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       XorBuffer
//
//  Description:
//
//  Выполняет операцию dstData = srcData xor key
//
///////////////////////////////////////////////////////////////////////////////

void XorBuffer (const void *srcData, 
                void *dstData, 
                size_t nDataSize,
                const void *key,
                size_t nKeySize)
{
    const unsigned char *chSrcData = (const unsigned char *) srcData;
    unsigned char *chDstData = (unsigned char *) dstData;
    const unsigned char *chKey = (const unsigned char *) key;
    if (0 == nKeySize)
    {
        memcpy (chDstData, chSrcData, nDataSize);
        return;
    }

    for (size_t nIdx = 0; nIdx < nDataSize; ++ nIdx)
    {
        unsigned char nMask = chKey [nIdx % nKeySize];
        chDstData [nIdx] = chSrcData [nIdx] ^ nMask;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       BinaryToHexString
//
//  Description:
//
//  Кодирует бинарный буфер в строку dstData, преобразуя каждый байт в два 
//  символа - представление байта в шестнадцатиричном виде.
//
///////////////////////////////////////////////////////////////////////////////

void* BinaryToHexString (const void *srcData, 
                         void *dstData, 
                         size_t nSrcDataSize)
{
    const unsigned char *chSrcData = (const unsigned char *) srcData;
    unsigned char *chDstData = (unsigned char *) dstData;

    static const unsigned char translationTable [] = "0123456789ABCDEF";
    size_t nDstIdx = 0;
    for (size_t nIdx = 0; nIdx < nSrcDataSize; ++ nIdx)
    {
        unsigned int nVal = chSrcData [nIdx];
        chDstData [nDstIdx ++] = translationTable [ nVal >> 4];
        chDstData [nDstIdx ++] = translationTable [ nVal & 0x0F];
    }
    chDstData [nDstIdx] = 0;

    return dstData;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       HexStringToBinary
//
//  Description:
//
//  Декодирует hex-encoded стоку в бинарный буфер.
//
///////////////////////////////////////////////////////////////////////////////

void* HexStringToBinary (const void *srcData, 
                         void *dstData, 
                         size_t nSrcDataSize)
{
    const unsigned char *chSrcData = (const unsigned char *) srcData;
    unsigned char *chDstData = (unsigned char *) dstData;

    if (nSrcDataSize % 2) return NULL;
    size_t nDstIdx = 0;
    for (size_t nIdx = 0; nIdx < nSrcDataSize; nIdx += 2)
    {
        unsigned char nHi = chSrcData [nIdx];
        if (nHi >= '0' && nHi <= '9') nHi -= '0';
        else if (nHi >= 'A' && nHi <= 'F') nHi = nHi - 'A' + 10;
        else if (nHi >= 'a' && nHi <= 'f') nHi = nHi - 'a' + 10;
        else return NULL;

        unsigned char nLo = chSrcData [nIdx + 1];
        if (nLo >= '0' && nLo <= '9') nLo -= '0';
        else if (nLo >= 'A' && nLo <= 'F') nLo = nLo - 'A' + 10;
        else if (nLo >= 'a' && nLo <= 'f') nLo = nLo - 'a' + 10;
        else return NULL;

        chDstData [nDstIdx ++] = nLo | (nHi << 4);
    }

    return dstData;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetProgramFolder
//
//  Description:
//
//  Возвращает строку с полным путем ло папки в котрой находится исполняемый
//  файл данного приложения. В конце нет бэкслэша.
//
///////////////////////////////////////////////////////////////////////////////

wchar_t* GetProgramFolder (wchar_t *wcsProgramFolder, size_t nCount)
{
    wcsProgramFolder [0] = 0;
    GetModuleFileNameW (NULL, wcsProgramFolder, (DWORD) nCount);
    int nLen = (int) wcslen (wcsProgramFolder);
    for (int nIdx = nLen - 1; nIdx >= 0; -- nIdx)
    {
        if ('\\' == wcsProgramFolder [nIdx])
        {
            wcsProgramFolder [nIdx] = 0;
            break;
        }
    }
    
    return wcsProgramFolder;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SignDataWithMD5
//
//  Description:
//
//  Вычисляет MD5 хэш для последовательностей байтов из pData (в dataSize
//  хранятся длины этих последовательностей). Количество последовательностей
//  равно nCount.
//  Хэш записывается в буфер pHashValue. Размер хэша всегда равен MD5_HASH_LEN 
//  или 16 байтам.
//
///////////////////////////////////////////////////////////////////////////////

bool SignDataWithMD5 (const void *pData [], 
                      size_t dataSize [],
                      size_t nCount,
                      void *pHashValue)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;

    if ( !CryptAcquireContext (&hProv,
                               NULL,
                               NULL,
                               PROV_RSA_FULL,
                               CRYPT_VERIFYCONTEXT))
    {
        return false;
    }
    
    if ( !CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash))
    {
        CryptReleaseContext (hProv, 0);

        return false;
    }

    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        if ( ! CryptHashData (hHash, 
                              (const BYTE*) pData [nIdx], 
                              (DWORD) dataSize [nIdx], 
                              0))
        {
            CryptDestroyHash (hHash);
            CryptReleaseContext (hProv, 0);  

            return false;
        }
    }
    
    DWORD dwHashSize = MD5_HASH_LEN;
    if ( !CryptGetHashParam (hHash, HP_HASHVAL, (BYTE*) pHashValue, &dwHashSize, 0))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  
        
        return false;
    }

    CryptDestroyHash (hHash);
    CryptReleaseContext (hProv, 0);  

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SignDataWithMD5
//
//  Description:
//
//  Вычисляет MD5 хэш для буфера pData.  nDataSize - количество байт в буфере.
//
//  Хэш записывается в буфер pHashValue. Размер хэша всегда равен MD5_HASH_LEN 
//  или 16 байтам.
//
///////////////////////////////////////////////////////////////////////////////

bool SignDataWithMD5 (const void *pData, 
                      size_t nDataSize,                      
                      void *pHashValue)
{
    return SignDataWithMD5 (&pData, &nDataSize, 1, pHashValue);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SignFile
//
//  Description:
//
//  Записывает в конец текстового файла строку с электронной подписью.
//
///////////////////////////////////////////////////////////////////////////////

bool SignFile (const char *szFileName,
               const void *key,
               size_t nKeySize)
{
    FILE *pFile = fopen (szFileName, "r+t");
    if (NULL == pFile) return false;
    bool bResult = SignFile (pFile,
                             key,
                             nKeySize);
    if (0 != fclose (pFile)) bResult = false;

    return bResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SignFile
//
//  Description:
//
//  Записывает в конец текстового файла строку с электронной подписью.
//
///////////////////////////////////////////////////////////////////////////////

bool SignFile (const wchar_t *wcsFileName,
               const void *key,
               size_t nKeySize)
{
    FILE *pFile = _wfopen (wcsFileName, L"r+t");
    if (NULL == pFile) return false;
    bool bResult = SignFile (pFile,
                             key,
                             nKeySize);
    if (0 != fclose (pFile)) bResult = false;

    return bResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SignFile
//
//  Description:
//
//  Записывает в конец текстового файла строку с электронной подписью.
//
///////////////////////////////////////////////////////////////////////////////

bool SignFile (FILE *pFile,
               const void *key,
               size_t nKeySize)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    if ( !CryptAcquireContext (&hProv,
                               NULL,
                               NULL,
                               PROV_RSA_FULL,
                               CRYPT_VERIFYCONTEXT))
    {
        return false;
    }
    
    if ( !CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash))
    {
        CryptReleaseContext (hProv, 0);
        return false;
    }


    char buffer [1024];
    
    if ( 0 != fseek (pFile, 0, SEEK_SET))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  

        return false;
    }

    while (! feof (pFile))
    {
        buffer [0] = 0;
        fgets (buffer, sizeof (buffer), pFile);
        int nLen = (int) strlen (buffer);
        if (nLen > 0 && '\n' == buffer [nLen - 1])
        {
            buffer [nLen - 1] = 0;
        }
        else
        {
            int i = 0;
        }

        if ( ! CryptHashData (hHash, 
                              (const BYTE*) buffer, 
                              (DWORD) strlen (buffer), 
                              0))
        {
            CryptDestroyHash (hHash);
            CryptReleaseContext (hProv, 0);  

            return false;
        }
    }

    if ( ! CryptHashData (hHash, 
                          (const BYTE*) key, 
                          (DWORD) nKeySize, 
                          0))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  

        return false;
    }

    DWORD dwHashSize = MD5_HASH_LEN;
    BYTE hashValue [MD5_HASH_LEN];
    char szSignature [2 * MD5_HASH_LEN + 1];
    if ( !CryptGetHashParam (hHash, HP_HASHVAL, hashValue, &dwHashSize, 0))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  
        
        return false;
    }
    CryptDestroyHash (hHash);
    CryptReleaseContext (hProv, 0);

    if ( ! BinaryToHexString (hashValue, szSignature, sizeof (hashValue)))
    {
        return false;
    }

    if (EOF == fputs (szSignature, pFile))
    {
        return false;
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       VerifySignedFile
//
//  Description:
//
//  Записывает в конец текстового файла строку с электронной подписью.
//
///////////////////////////////////////////////////////////////////////////////

bool VerifySignedFile (const char *szFileName,
                       const void *key,
                       size_t nKeySize)
{
    FILE *pFile = fopen (szFileName, "rt");
    if (NULL == pFile) return false;
    bool bResult = VerifySignedFile (pFile,
                                     key,
                                     nKeySize);
    if (0 != fclose (pFile)) bResult = false;

    return bResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       VerifySignedFile
//
//  Description:
//
//  Записывает в конец текстового файла строку с электронной подписью.
//
///////////////////////////////////////////////////////////////////////////////

bool VerifySignedFile (const wchar_t *wcsFileName,
                       const void *key,
                       size_t nKeySize)
{
    FILE *pFile = _wfopen (wcsFileName, L"r+t");
    if (NULL == pFile) return false;
    bool bResult = VerifySignedFile (pFile,
                                     key,
                                     nKeySize);
    if (0 != fclose (pFile)) bResult = false;

    return bResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       VerifySignedFile
//
//  Description:
//
//  Проверяет целостность подписанного файла.
//
///////////////////////////////////////////////////////////////////////////////

bool VerifySignedFile (FILE *pFile,
                       const void *key,
                       size_t nKeySize)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;

    if ( !CryptAcquireContext (&hProv,
                               NULL,
                               NULL,
                               PROV_RSA_FULL,
                               CRYPT_VERIFYCONTEXT))
    {
        return false;
    }
    
    if ( !CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash))
    {
        CryptReleaseContext (hProv, 0);
        return false;
    }

    char buffer [1024];
    if ( 0 != fseek (pFile, 0, SEEK_SET) || 
        feof (pFile))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  

        return false;
    }
    while (! feof (pFile))
    {
        fgets (buffer, sizeof (buffer), pFile);
        int nLen = (int) strlen (buffer);
        if (nLen > 0 && '\n' == buffer [nLen - 1])
        {
            buffer [nLen - 1] = 0;
        }

        if (feof (pFile))
        {
            break;
        }

        if ( ! CryptHashData (hHash, 
                              (const BYTE*) buffer, 
                              (DWORD) strlen (buffer), 
                              0))
        {
            CryptDestroyHash (hHash);
            CryptReleaseContext (hProv, 0);  

            return false;
        }
    }
    if ( ! CryptHashData (hHash, 
                          (const BYTE*) key, 
                          (DWORD) nKeySize, 
                          0))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  

        return false;
    }

    DWORD dwHashSize = MD5_HASH_LEN;
    BYTE hashValue [MD5_HASH_LEN];
    char szSignature [2 * MD5_HASH_LEN + 1];
    if ( !CryptGetHashParam (hHash, HP_HASHVAL, hashValue, &dwHashSize, 0))
    {
        CryptDestroyHash (hHash);
        CryptReleaseContext (hProv, 0);  
        
        return false;
    }
    CryptDestroyHash (hHash);
    CryptReleaseContext (hProv, 0);

    if ( ! BinaryToHexString (hashValue, szSignature, sizeof (hashValue)))
    {
        return false;
    }
   
    return 0 == strcmp (szSignature, buffer);
}