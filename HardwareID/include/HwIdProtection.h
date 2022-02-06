//////////////////////////////////////////////////////////////////////////////
//                                                                           
//  File:           HwIdProtection.h
//                                                                          
//  Facility:       �������� ���������� � ����������
//                                                                          
//  Abstract:       �������, ����������� ������ ���� ������ ����������.
//                                                                          
//  Environment:    MSVC++ 8.0                            
//                                                                          
//  Author:         ������ �.�.                                              
//                                                                          
//  Creation Date:  27/04/2006                                              
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
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
// ��������� ������ ���� ������ ����������: 
//  - ������������� ����������� ��������������
//  - ������ ��� � ���� wcsHwIdFolderName + wcsHwIdFile
//  - �������� ���������� ����� � ������ wcsKeyFile.
//
//  ���� ���������� ��������� ������, �� ������� ���������� false.
//

bool Protect (wchar_t *wcsComment,
              wchar_t *wcsKeyFile,
              wchar_t *wcsHwIdFolderName = 0, 
              wchar_t *wcsHwIdFile = 0);
#endif