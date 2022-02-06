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
* $History: LastError.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Common
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Common
 * Первая версия
* 
*/

#include "Utils.h"
#include "LastError.hpp"
#include <string.h>
    
///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Manager
//
//  Description:
//
//  Конструктор
//
///////////////////////////////////////////////////////////////////////////////

LastError::LastError ()
{
    SetLastErrorString (NULL);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       LastError
//
//  Description:
//
//  Деструктор
//
///////////////////////////////////////////////////////////////////////////////

LastError::~LastError ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetLastErrorString
//
//  Description:
//
//  Устанавливает описание последней ошибки.
//
///////////////////////////////////////////////////////////////////////////////

void LastError::SetLastErrorString (const wchar_t* wcsError)
{
    if (NULL != wcsError)
    {
        wcsncpy (m_wcsLastError, wcsError, MAX_LAST_ERROR_LENGTH - 1);
    }
    else
    {
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_wcsLastError); ++ nIdx)
        {
            m_wcsLastError [nIdx] =  0;    
        }    
    }
} 

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetLastErrorString
//
//  Description:
//
//  Возвращает описание последней ошибки.
//
///////////////////////////////////////////////////////////////////////////////

const wchar_t* LastError::GetLastErrorString ()
{
    return m_wcsLastError;
}