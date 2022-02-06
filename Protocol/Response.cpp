///////////////////////////////////////////////////////////////////////////////
//
//  File:           Response.cpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма стандартного ответа
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
* $History: Response.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * Первая версия
* 
*/

#include "Utils.h"
#include "Response.hpp"

namespace Mercury
{
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Response
    //
    //  Description:
    //
    //  Конструктор.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Response::Response (CrcType nType/* = crc*/) : Frame (1, nType)
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetResponseString
    //
    //  Description:
    //
    //  Возвращает строку, описывающую код ответа счетчика.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const wchar_t* Response::GetResponseString (const Response::ResponseCode nCode)
    {
        static const wchar_t *wcsDefaultResponse = L"Неизвестный код";
        struct ResponseInfo
        {
            ResponseCode    nResponseCode;
            const wchar_t*  wcsResponseString;
        };

        static ResponseInfo table [] = 
        {
            {rcInvalid,             L"Невалидный код ответа"},
            {rcOK,                  L"Все нормально"},
            {rcInvalidArg,          L"Недопустимая команда или параметр"},
            {rcInternalError,       L"Внутренняя ошибка счетчика"},
            {rcAccessDenied,        L"Не достаточен уровень доступа для удовлетворения запроса"},
            {rcTimerAdjustDenied,   L"Внутренние часы счетчика уже корректировались в течение текущих суток"},
            {rcNoConnected,         L"Не открыт канал связи"},
            {rcTryLater,            L"Повторить запрос в течении 0,5 с (СЭТ-4тм.02 c V15.XX.XX и выше)"}, 
            {rcResultNotReady,      L"Не готов результат измерения по запрашиваемому параметру (не закончилось время интегрирования после пуска измерителя) (СЭТ-4тм.02 c V23.XX.XX и выше)"}
        };

        size_t nCount = SIZE_OF (table);
        for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
        {
            if (nCode == table [nIdx].nResponseCode)
            {
                return table [nIdx].wcsResponseString;
            }
        }

        return wcsDefaultResponse;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetResponseCode
    //
    //  Description:
    //
    //  Возвращает код ответа.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Response::ResponseCode Response::GetResponseCode () const
    {
        return (ResponseCode) (0x0f & m_Body [1]);    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetResponseCode
    //
    //  Description:
    //
    //  Устанавливает код ответа.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Response::SetResponseCode (const Response::ResponseCode nCode)
    {
        m_Body [1] &= 0xf0;
        m_Body [1] |= nCode;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetTimeout
    //
    //  Description:
    //
    //  Возвращает таймаут ответа (в секундах).
    //
    ///////////////////////////////////////////////////////////////////////////////

    unsigned int Response::GetTimeout () const
    {
        return (m_Body [1] >> 4) & 0x0f;    
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetTimeout
    //
    //  Description:
    //
    //  Устанавливает таймаут ответа ((в секундах)).
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Response::SetTimeout (unsigned int nTimeout)
    {
        m_Body [1] &= 0x0f;
        m_Body [1] |= (nTimeout << 4) & 0xf0;
    }
}
