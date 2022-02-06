//////////////////////////////////////////////////////////////////////////////
//                                                                           
//  File:           PPCChecker.hpp                                          
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
* $History: PPCChecker.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/HardwareID/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/HardwareID/include
* 
*/

#ifndef _POCKETPC_CHECKER_
#define _POCKETPC_CHECKER_

#include <windows.h>

//
// PPCChecker class declaration.
//

class PPCChecker
{
    
public:

    //
    // Getting preset ID and platform ID of the device.
    //

    static bool GetDeviceID (wchar_t *wcsPresetId, 
                             size_t nPresetIdLength,
                             wchar_t *wcsPlatformId,
                             size_t nPlatformIdLength);


    //
    // Reads device ID from registry.
    //

    static bool GetRegistryDeviceID (wchar_t *wcsDeviceId, 
                                     size_t &nBufferLength);

    //
    // Getting OEM info for the device. 
    //

    static bool GetOEMInfo (wchar_t *wcsOEMInfo, 
                            size_t nBufferLength);

    //
    // Getting platform id for the device. 
    //

    static bool GetPlatformId (wchar_t *wcsPlatformId, 
                               size_t nBufferLength);

    //
    // Возвращает общую информацию о системе
    //

    static void GetSysInfo (LPSYSTEM_INFO lpSystemInfo)
    {
        GetSystemInfo (lpSystemInfo);

        return;
    }

    //
    // Возвращает идентификатор устройства. Метод сам выделяет память и поэтому
    // буфер, который возвращает этот метод нужно освободить с помощью оператора
    // delete []
    //

    static wchar_t* GetHardwareId (OUT size_t &nBufferLength);
};

#endif //_POCKETPC_CHECKER_