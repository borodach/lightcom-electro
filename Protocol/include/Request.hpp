///////////////////////////////////////////////////////////////////////////////
//
//  File:           Request.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс базового фрейма запроса
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
* $History: Request.hpp $
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

#ifndef _MERCURY_REQUEST_
#define _MERCURY_REQUEST_

#include "Frame.hpp"

namespace Mercury
{
    
    class Request: public Frame
    {
    public:

        //
        // Тип данных - код запроса.
        //

        enum RequestCode : unsigned char
        {
            rcInvalid           = 0xFF, // Невалидный код запроса.
            rcTestConnection    = 0x00, // Запрос на проверку канала связи
            rcConnect           = 0x01, // Запрос на установление соединения
            rcDisconnect        = 0x02, // Запрос на разрыв соединения
            rcReadEventLog      = 0x04, // Запрос на чтение журнала событий
            rcReadEnergy        = 0x05, // Запрос на чтение сведений о накопленной энергии
            rcReadPhysMemory    = 0x06, // Запрос на чтение физической памяти
            rcReadParams        = 0x08  // Запрос на чтение параметров и данных
        };


        //
        // Конструктор
        //

        Request (RequestCode nRequestType, size_t nSize = 0, CrcType nType = crc);

        //
        // Возвращает код запроса.
        //

        RequestCode GetRequestCode () const;

        //
        // Размер ожидаемого ответа (не включает сетевой адрес и контрольную 
        // сумму).
        //

        virtual size_t GetResponseSize () const;

    protected:

        //
        // Устанавливает код запроса.
        //

        void SetRequestCode (RequestCode nCode);

        //
        // Размер ожидаемого ответа (не включает сетевой адрес и контрольную 
        // сумму).
        //

        size_t m_nResponseSize;
    };
}
#endif