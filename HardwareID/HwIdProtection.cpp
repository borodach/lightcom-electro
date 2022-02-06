//////////////////////////////////////////////////////////////////////////////
//                                                                           
//  File:           HwIdProtection.cpp
//                                                                          
//  Facility:       Привязка приложения к аппаратуре
//                                                                          
//  Abstract:       Функция, выполняющая полный цикл защиты приложения.
//                                                                          
//  Environment:    MSVC++ 8.0                            
//                                                                          
//  Author:         Зайцев С.А.                                              
//                                                                          
//  Creation Date:  27/04/2006                                              
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//                                                                          
//////////////////////////////////////////////////////////////////////////////

/*
* $History: HwIdProtection.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/HardwareID
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/HardwareID
 * Первая версия
* 
*/

#include "PPCChecker.hpp"
#include "Utils.h"
#include "HardwareId.hpp"

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   Protect                                                  
//                                                                           
//  Description:                                                             
//                                                                           
//  Выполняет полный цикл защиты приложения: 
//   - генерирование аппаратного идентификатора
//   - запись его в файл wcsHwIdFolderName + wcsHwIdFile
//   - проверка валидности файла с ключем wcsKeyFile.
//
//   Если приложение запускать нельзя, то функция возвращвет false.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

bool Protect (wchar_t *wcsComment,
              wchar_t *wcsKeyFile,
              wchar_t *wcsHwIdFolderName/* = NULL*/, 
              wchar_t *wcsHwIdFile/* = NULL*/)
{
    
    //
    // Создание HardwareId
    //

    size_t nHardwareIdSize = 0;
    wchar_t *pHardwareId = PPCChecker::GetHardwareId (nHardwareIdSize);
    if (NULL == pHardwareId)
    {
        return false;
    }

    HardwareId hwId;
    hwId.Attatch (pHardwareId);
    pHardwareId = NULL;
    
    //
    // Запись HardwareId в файл
    //

    if (NULL != wcsHwIdFolderName && 
        NULL != wcsHwIdFile)
    {
        unsigned char *pBuffer = hwId.Encode ();
        if (pBuffer)
        {
            wchar_t wcsPath [1024];
            GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
            wcscat (wcsPath, wcsHwIdFolderName);
            CreateDirectory (wcsPath, NULL);
            wcscat (wcsPath, wcsHwIdFile);
            FILE *pFile = _wfopen (wcsPath, L"w");
            if (NULL != pFile)
            {
                fputs ((char*) pBuffer, pFile);
                fclose (pFile);
            }

            delete [] pBuffer;
            pBuffer = NULL;
        }
    }

    unsigned char *szKey = hwId.GenerateKey (wcsComment);
    if (NULL == szKey)
    {
        return false;
    }
    
    wchar_t wcsPath [1024];
    GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
    wcscat (wcsPath, wcsKeyFile);
    
    /*
    FILE *pFile = _wfopen (wcsPath, L"w");
    if (NULL == pFile)
    {
        delete [] szKey;

        return false;    
    }

    fputs ((char*) szKey, pFile);
    fclose (pFile);
    return false;
    */
    
    FILE *pFile = _wfopen (wcsPath, L"r");
    if (NULL == pFile)
    {
        delete [] szKey;

        return false;    
    }

    size_t nLen = strlen ((char*) szKey);
    char *szBuf = new char [nLen + 1];
    if (NULL == szBuf)
    {
        delete [] szKey;
        return false;
    }
    szBuf [0] = 0;
    szBuf [nLen] = 0;

    fgets (szBuf, nLen + 1, pFile);
    fclose (pFile);

    int nCmpResult = strcmp ((char*) szKey, szBuf);

    delete [] szKey;
    delete [] szBuf;

    return nCmpResult == 0;
}    
