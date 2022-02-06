///////////////////////////////////////////////////////////////////////////////
//
//  File:           LastError.hpp
//
//  Facility:       Сообщения об оибках
//
//
//  Abstract:       Базовый класс, хранящий последнее сообщение об ошибке.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  24-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: LastError.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Common/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Common/include
* 
*/

#ifndef _LAST_ERROR_
#define _LAST_ERROR_    

#ifndef MAX_LAST_ERROR_LENGTH
#define MAX_LAST_ERROR_LENGTH     1024
#endif

class LastError
{
public:
    
    //
    // Конструктор
    //

    LastError ();

    //
    // Деструктор
    //

    virtual ~LastError ();


    //
    // Возвращает текст последней ошибки.
    //

    const wchar_t* GetLastErrorString ();


    //
    // Устанавливает текст последней ошибки
    //

    void SetLastErrorString (const wchar_t*);

protected:

    //
    // Текст последней ошибки.
    //

    wchar_t m_wcsLastError [MAX_LAST_ERROR_LENGTH];
};

#endif