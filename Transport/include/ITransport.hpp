///////////////////////////////////////////////////////////////////////////////
//
//  File:           ITransport.cpp
//
//  Facility:       Взаимодействие с уcтройствами
//                                                         
//
//  Abstract:       Интерфейс для I/O с устройством
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  22-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ITransport.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Transport/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Transport/include
* 
*/

#ifndef _ITRANSPORT_H_
#define _ITRANSPORT_H_

#include "LastError.hpp"

namespace Transport
{
    class ITransport: public LastError
    {
        public:

        //
        // Читает nCount байт в буфер pBuffer.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Read (unsigned char* pBuffer, size_t nCount) = 0;

        //
        // Записывает nCount байт из буфера pBuffer.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Write (const unsigned char* pBuffer, size_t nCount) = 0;

        //
        // Инициализация транспорта.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Init (const wchar_t *wcsTransportName) = 0;

        //
        // Реинициализация транспорта.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool ReInit () = 0;

        //
        // Возвращает true, если транспорт проинициализирован.
        //

        virtual bool IsInited () = 0;

        //
        // Деинициализация транспорта. Все занятые ресурсы освобождаются.
        //

        virtual void Reset () = 0;

        //
        // Деструктор.
        //

        virtual ~ITransport () {};
    };
}

#endif _ITRANSPORT_H_