//////////////////////////////////////////////////////////////////////////////
//                                                                           
//  File:           PPCChecker.cpp                                          
//                                                                          
//  Facility:       Получение сведенй об оборудовании PPC.                  
//                                                                          
//  Abstract:       Получение сведений об аппаратуре, уникально             
//                  идентифицирующей                                        
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
* $History: PPCChecker.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/HardwareID
 * 
 * *****************  Version 5  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/HardwareID
 * Первая версия
* 
*/

#include "Utils.h"
#include "PPCChecker.hpp"
#include <winioctl.h>
#include <projects.h>

#define MANUFACTUREID_INVALID   0x01
#define SERIALNUM_INVALID       0x02

#define IOCTL_HAL_GET_DEVICEID CTL_CODE (FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define BUFFER_SIZE 256

//
// EXTERNAL FUNCTIONS
//

extern "C"__declspec( dllimport ) BOOL KernelIoControl (DWORD dwIoControlCode, 
                                                        LPVOID lpInBuf, 
                                                        DWORD nInBufSize, 
                                                        LPVOID lpOutBuf, 
                                                        DWORD nOutBufSize, 
                                                        LPDWORD lpBytesReturned);

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   GetDeviceID                                                  
//                                                                           
//  Description:                                                             
//                                                                           
//  Getting the preset ID and platform ID of the device.                     
//                                                                           
///////////////////////////////////////////////////////////////////////////////

bool PPCChecker::GetDeviceID (wchar_t *wcsPresetId, 
                              size_t nPresetIdLength,
                              wchar_t *wcsPlatformId,
                              size_t nPlatformIdLength)
{
    *wcsPresetId = 0;
    *wcsPlatformId = 0;

    DWORD dwBuffSize = 0;
    DWORD dwError = 0;

    DWORD dwReturned = 0;
    typedef struct _DEVICE_ID {
        DWORD dwSize;
        DWORD dwPresetIDOffset;
        DWORD dwPresetIDBytes;
        DWORD dwPlatformIDOffset;
        DWORD dwPlatformIDBytes;
    } DEVICE_ID, *PDEVICE_ID;
   
    BYTE buffer [BUFFER_SIZE];
    memset (buffer, 0, sizeof (buffer));
    PDEVICE_ID pDdeviceId = (PDEVICE_ID) buffer; 
    pDdeviceId->dwSize = sizeof (buffer);

    //
    // Request device ID using szBuffer
    //

    if (! KernelIoControl (IOCTL_HAL_GET_DEVICEID, 
                           0, 
                           0, 
                           buffer, 
                           sizeof (buffer), 
                           &dwReturned))
    {
        return false;
    }

    unsigned i;
    wchar_t buf [4];
    int nSize = nPresetIdLength - 1;
    for (i = 0; i < pDdeviceId->dwPresetIDBytes; ++ i)
    {
        wsprintf (buf, L"%02X", unsigned (buffer [pDdeviceId->dwPresetIDOffset + i]));
        wcsncat (wcsPresetId, buf, nSize);
        nSize -= wcslen (buf);
    }
    
    nSize = nPlatformIdLength - 1;
    for (i = 0; i < pDdeviceId->dwPlatformIDBytes; ++ i)
    {
        wsprintf (buf, L"%02X", unsigned (buffer [pDdeviceId->dwPlatformIDOffset + i]));
        wcsncat (wcsPlatformId, buf, nSize);
        nSize -= wcslen (buf);
    }        

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Function:   GetRegistryDeviceID                                          //
//                                                                           //
//  Description:                                                             //
//                                                                           //
//  Reads the device ID from the registry.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool PPCChecker::GetRegistryDeviceID (wchar_t *wcsDeviceId, 
                                      size_t &nBufferLength)
{
    *wcsDeviceId = 0;

    DWORD dwError = 0;
    HKEY hKey = 0;
    
    if (ERROR_SUCCESS != RegOpenKeyEx (HKEY_LOCAL_MACHINE, 
                                       L"\\Ident", 
                                       0, 
                                       0,
                                       &hKey) != ERROR_SUCCESS)
    {
        return false;
    }

    DWORD dwType = REG_SZ;
    DWORD dwSize = nBufferLength;
    if (RegQueryValueEx (hKey, 
                         L"Name", 
                         NULL, 
                         &dwType, 
                         (PBYTE) wcsDeviceId, 
                         &dwSize) != ERROR_SUCCESS)
    {
        nBufferLength = dwSize;
        RegCloseKey (hKey);

        return false;
    }
    nBufferLength = dwSize;
    RegCloseKey (hKey);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                         
//  Function:   GetOEMInfo                                                 
//                                                                         
//  Description:                                                           
//                                                                         
//  Getting OEM info for the device. 
//                                                                         
///////////////////////////////////////////////////////////////////////////////

bool PPCChecker::GetOEMInfo (wchar_t *wcsOEMInfo, 
                             size_t nBufferLength)
{
    return  FALSE != SystemParametersInfo (SPI_GETOEMINFO, 
                                           nBufferLength, 
                                           wcsOEMInfo, 0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                         
//  Function:   GetPlatformId                                                 
//                                                                         
//  Description:                                                           
//                                                                         
//  Getting platform id for the device. 
//                                                                         
///////////////////////////////////////////////////////////////////////////////

bool PPCChecker::GetPlatformId (wchar_t *wcsPlatformId, 
                                size_t nBufferLength)
{
    return  FALSE != SystemParametersInfo (SPI_GETPLATFORMTYPE, 
                                           nBufferLength, 
                                           wcsPlatformId, 0);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                         
//  Function:   GetHardwareId
//                                                                         
//  Description:                                                           
//                                                                         
// Возвращает идентификатор устройства. Метод сам выделяет память и поэтому
// буфер, который возвращает этот метод нужно освободить с помощью оператора
// delete []
//                                                                         
///////////////////////////////////////////////////////////////////////////////

wchar_t* PPCChecker::GetHardwareId (OUT size_t &nBufferLength)
{
    nBufferLength = 0;
    wchar_t wcsPresetId [1024];
    wchar_t wcsPlatformId [1024];
    bool bDeviceIdValid = GetDeviceID (wcsPresetId, 
                                       SIZE_OF (wcsPresetId),
                                       wcsPlatformId,
                                       SIZE_OF (wcsPlatformId));
    if (! bDeviceIdValid)
    {
        return NULL;
    }

    /*
    wchar_t wcsRegistryDeviceId [1024];
    size_t nRegistryDeviceIdSize = sizeof (wcsRegistryDeviceId) / sizeof (wcsRegistryDeviceId [0]);
    bool bRegistryDeviceId = GetRegistryDeviceID (wcsRegistryDeviceId, 
                                                  nRegistryDeviceIdSize);
    */
                                                                                                                
    wchar_t wcsOEMInfo [1024];
    wcsOEMInfo [0] = 0; 
    bool bOEMInfo = GetOEMInfo (wcsOEMInfo, 
                                SIZE_OF (wcsOEMInfo));

    wchar_t wcsOEMPlatformId [1024];
    wcsOEMPlatformId [0] = 0;
    bool bOEMPlatformId = GetPlatformId (wcsOEMPlatformId, 
                                         SIZE_OF (wcsOEMPlatformId));
    
    SYSTEM_INFO systemInfo;
    GetSysInfo (&systemInfo);   

    nBufferLength = wcslen (wcsPresetId) + 1 + wcslen (wcsPlatformId) + 1 +
                    wcslen (wcsOEMInfo) + 1 + wcslen  (wcsOEMPlatformId) + 
                    4 + 1 + //dwNumberOfProcessors;
                    2 + 1 + //wProcessorArchitecture
                    2 + 1 + //wProcessorLevel
                    2 + 1;  //wProcessorRevision
    wchar_t *wcsResult = new wchar_t [nBufferLength + 1];
    if (NULL == wcsResult) return NULL;
    wsprintf (wcsResult, 
              L"%s-%s-%s-%s-%04X-%02X-%02X-%02X", 
              wcsPresetId, 
              wcsPlatformId,
              wcsOEMInfo,
              wcsOEMPlatformId,
              systemInfo.dwNumberOfProcessors,
              systemInfo.wProcessorArchitecture,
              systemInfo.wProcessorLevel,
              systemInfo.wProcessorRevision);
    size_t n = wcslen (wcsResult);
    return wcsResult;
}

