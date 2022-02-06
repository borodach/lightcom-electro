///////////////////////////////////////////////////////////////////////////////
//
//  File:           Response.hpp
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
* $History: Response.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _MERCURY_RESPONSE_
#define _MERCURY_RESPONSE_

#include "Frame.hpp"

namespace Mercury
{
    
    class Response: public Frame
    {
    public:

        //
        // Коды ответа счетчика.
        //

        enum ResponseCode : unsigned char
        {
            rcInvalid           = 0xFF, // Невалидный код ответа.
            rcOK                = 0x00, // Все нормально.
            rcInvalidArg        = 0x01, // Недопустимая команда или параметр.
            rcInternalError     = 0x02, // Внутренняя ошибка счетчика.
            rcAccessDenied      = 0x03, // Не достаточен уровень доступа для удовлетворения запроса.
            rcTimerAdjustDenied = 0x04, // Внутренние часы счетчика уже корректировались в течение текущих суток.
            rcNoConnected       = 0x05, // Не открыт канал связи
            rcTryLater          = 0x06, // Повторить запрос в течении 0,5 с (СЭТ-4тм.02 c V15.XX.XX и выше) 
            rcResultNotReady    = 0x07  // Не готов результат измерения по запрашиваемому параметру (не закончилось время интегрирования после пуска измерителя) (СЭТ-4тм.02 c V23.XX.XX и выше)
        };

        //
        // Конструктор
        //

        Response (CrcType nType = crc);

        //
        // Возвращает строку, описывающую код ответа счетчика.
        //

        static const wchar_t* GetResponseString (const ResponseCode nCode);

        //
        // Возвращает код ответа.
        //

        ResponseCode GetResponseCode () const;

        //
        // Устанавливает код ответа.
        //

        void SetResponseCode (const ResponseCode nCode);

        //
        // Возвращает таймаут ответа (в секундах).
        //

        unsigned int GetTimeout () const;

        //
        // Устанавливает таймаут ответа ((в секундах)).
        //

        void SetTimeout (unsigned int nTimeout);
    };
}
#endif