///////////////////////////////////////////////////////////////////////////////
//
//  File:           ManagerPPC.h
//
//  Facility:       ЛайтКом Энерго Менеджер
//                  
//
//
//  Abstract:       Класс приложения
//
//
//  Environment:    MSVC 2005
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  22-Jan-2007
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ManagerPPC.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/ManagerPPC
*/

#ifndef _MANAGERPPC_H_
#define _MANAGERPPC_H_

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resourceppc.h"

class CManagerPPCApp : public CWinApp
{
public:

    //
    // Конструктор
    //

	CManagerPPCApp ();

    //
    // Инициализация экземпляра приложения
    //

    virtual BOOL InitInstance ();

protected:
	
    //
    // Обработчик команды "О программе"
    //

    afx_msg void OnAppAbout();

    //
    // Карта сообщений
    //

	DECLARE_MESSAGE_MAP()
};

//
// Экземпляр приложения
//

extern CManagerPPCApp theApp;

#endif //_MANAGERPPC_H_