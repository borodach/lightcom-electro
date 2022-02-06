//////////////////////////////////////////////////////////////////////////////
//                                                                           
//  File:           HwIdProtection.h
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
* $History: HwIdProtection.h $
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

#ifndef _HWID_PROTECTION_
#define _HWID_PROTECTION_

//
// Выполняет полный цикл защиты приложения: 
//  - генерирование аппаратного идентификатора
//  - запись его в файл wcsHwIdFolderName + wcsHwIdFile
//  - проверка валидности файла с ключем wcsKeyFile.
//
//  Если приложение запускать нельзя, то функция возвращвет false.
//

bool Protect (wchar_t *wcsComment,
              wchar_t *wcsKeyFile,
              wchar_t *wcsHwIdFolderName = 0, 
              wchar_t *wcsHwIdFile = 0);
#endif