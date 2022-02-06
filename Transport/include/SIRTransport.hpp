///////////////////////////////////////////////////////////////////////////////
//
//  File:           SIRTransport.hpp
//
//  Facility:       Взаимодействие с уcтройствами
//
//
//  Abstract:       Serial IrDA транспорт.
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
* $History: SIRTransport.hpp $
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

#ifndef _SIRTRANSPORT_H_
#define _SIRTRANSPORT_H_

#include "HandleTransport.hpp"

namespace Transport
{
    class SIRTransport: public HandleTransport
    {
    public:

        //
        // Конструктор.
        //

        SIRTransport ();
        
        //
        // Инициализация транспорта.
        // Возвращает true, если операция прошла успешно.
        //

        virtual bool Init (const wchar_t *wcsTransportName);

        //
        // Деинициализация транспорта. Все занятые ресурсы освобождаются.
        //

        virtual void Reset ();
 
        //
        // Читает имя транспорта из реестра.
        //

        static wchar_t* ReadTransportNameFromRegostry (wchar_t * wcsBuffer, 
                                                       size_t nMaxCharCount);

        //
        // Таймауты для работы с портом.
        //

        const COMMTIMEOUTS & GetTimeount () const;
        COMMTIMEOUTS & GetTimeount ();
        void SetTimeouts (COMMTIMEOUTS timeouts);

        //
        // Установка таймауты по-умолчанию.
        //

        void ResetTimeouts ();

        //
        // Автоматически конфигурирует SIR транспорт, читая из реестра 
        // нужные сведения.
        //

        bool AutoConfig ();

    protected:

        //
        // Таймауты для работы с портом.
        //

        COMMTIMEOUTS  m_commTimeouts;        
    };
}

#endif _SIRTRANSPORT_H_