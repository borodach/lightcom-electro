///////////////////////////////////////////////////////////////////////////////
//
//  File:           HandleTransport.hpp
//
//  Facility:       Взаимодействие с уcтройствами
//
//
//  Abstract:       Windows handle транспорт.
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
* $History: HandleTransport.hpp $
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

#ifndef _HANDLETRANSPORT_H_
#define _HANDLETRANSPORT_H_

#include <windows.h>
#include <string.h>
#include "ITransport.hpp"

#define MAX_TRANSPORT_NAME_LENGTH 16

namespace Transport
{
    class HandleTransport: public ITransport
    {
    public:

        //
        // Конструктор.
        //

        HandleTransport ();

        //
        // Деструктор.
        //

        virtual ~HandleTransport ();

        //
        // Читает nCount байт в буфер pBuffer.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Read (unsigned char* pBuffer, size_t nCount);

        //
        // Записывает nCount байт из буфера pBuffer.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Write (const unsigned char* pBuffer, size_t nCount);

        //
        // Инициализация транспорта.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Init (const wchar_t *wcsTransportName);

        //
        // Реинициализация транспорта.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool ReInit ();

        //
        // Возвращает true, если транспорт проинициализирован.
        //

        virtual bool IsInited ();

        //
        // Деинициализация транспорта. Все занятые ресурсы освобождаются.
        //

        virtual void Reset ();
        
        //
        // Возвращает дескриптор устройства
        //

        HANDLE GetHahdle () const;

        //
        // Возвращает имя устройства
        //

        const wchar_t*  GetDeviceName () const;

    protected:

        //
        // Дескриптор устройства.
        //

        HANDLE m_hDevice;

        //
        // Имя устройства
        //

        wchar_t m_wcsDeviceName [512];
    };
}
#endif