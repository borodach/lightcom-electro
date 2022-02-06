///////////////////////////////////////////////////////////////////////////////
//
//  File:           SIRTransport.cpp
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
* $History: SIRTransport.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Transport
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Transport
 * Первая версия
* 
*/

#include "Utils.h"
#include "SIRTransport.hpp"

namespace Transport
{
    //////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SIRTransport
    //
    //  Description:
    //
    //  Конструктор.
    //
    ///////////////////////////////////////////////////////////////////////////////

    SIRTransport::SIRTransport ()
    {
        ResetTimeouts ();
    } 
    
    //////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetTimeouts
    //
    //  Description:
    //
    //  Возвращает таймауты.
    //
    ///////////////////////////////////////////////////////////////////////////////

    const COMMTIMEOUTS & SIRTransport::GetTimeount () const
    {
        return m_commTimeouts;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       GetTimeouts
    //
    //  Description:
    //
    //  Возвращает таймауты.
    //
    ///////////////////////////////////////////////////////////////////////////////

    COMMTIMEOUTS & SIRTransport::GetTimeount ()
    {
        return m_commTimeouts;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetTimeouts
    //
    //  Description:
    //
    //  Установка таймаутов.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void SIRTransport::SetTimeouts (COMMTIMEOUTS timeouts)
    {
        m_commTimeouts.ReadTotalTimeoutConstant    = timeouts.ReadTotalTimeoutConstant;        
        m_commTimeouts.ReadIntervalTimeout         = timeouts.ReadIntervalTimeout;
        m_commTimeouts.ReadTotalTimeoutMultiplier  = timeouts.ReadTotalTimeoutMultiplier;
        m_commTimeouts.WriteTotalTimeoutConstant   = timeouts.WriteTotalTimeoutConstant;        
        m_commTimeouts.WriteTotalTimeoutMultiplier = timeouts.WriteTotalTimeoutMultiplier;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ResetTimeouts
    //
    //  Description:
    //
    //  Установка таймауты по-умолчанию.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void SIRTransport::ResetTimeouts ()
    {
        m_commTimeouts.ReadTotalTimeoutConstant = 100;        
        m_commTimeouts.ReadIntervalTimeout = 20;
        m_commTimeouts.ReadTotalTimeoutMultiplier = 20;
        m_commTimeouts.WriteTotalTimeoutConstant = 100;        
        m_commTimeouts.WriteTotalTimeoutMultiplier = 20;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Init
    //
    //  Description:
    //
    //  Инициализация транспорта.
    //  Возвращает true, если операция прошла успешно.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool SIRTransport::Init (const wchar_t *wcsTransportName)
    {
        bool bResult = HandleTransport::Init (wcsTransportName);
        if (! bResult) return bResult;

        bResult = (FALSE != EscapeCommFunction (m_hDevice, SETIR));
        if (! bResult) 
        {
            DWORD dwError = GetLastError ();
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"Не удалось перевести порт в режим IrDA (код ошибки %d)", 
                dwError);
            SetLastErrorString (wcsError);
            Reset ();

            return false;
        }

        bResult = (FALSE != SetCommTimeouts (m_hDevice, &m_commTimeouts));
        if (! bResult) 
        {
            DWORD dwError = GetLastError ();
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"Не удалось установить таймауты порта (код ошибки %d)", 
                dwError);
            SetLastErrorString (wcsError);
            Reset ();

            return false;
        }

        //
        // Отправляем на счетчик инициализирующую последовательность, чтобы 
        // переключить его на интерфейс IrDA.
        //

        unsigned char initSeq [80];
        int i;
        for (i = 0; i < 40; ++ i) initSeq [i] = 0xAA;
        for (i = 40; i < 80; ++ i) initSeq [i] = 0x66;

        bResult = Write (initSeq, sizeof (initSeq));

        if (! bResult) 
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"Не удалось передать в счетчик последовательность инициализации IR порта:\n%s", 
                GetLastErrorString ());
            SetLastErrorString (wcsError);

            Reset ();

            return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Reset
    //
    //  Description:
    //
    //  Деинициализация транспорта. Все занятые ресурсы освобождаются.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void SIRTransport::Reset ()
    {
        if (IsInited ())
        {
            EscapeCommFunction (m_hDevice, CLRIR);
        }

        HandleTransport::Reset ();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ReadTransportNameFromRegostry
    //
    //  Description:
    //
    //  Читает имя транспорта из реестра.
    // 
    //
    ///////////////////////////////////////////////////////////////////////////////

    wchar_t* SIRTransport::ReadTransportNameFromRegostry (wchar_t * wcsBuffer, 
                                                          size_t nMaxCharCount)
    {
        if (nMaxCharCount < 4) return false;
        wcsBuffer [nMaxCharCount - 1] = 0;
        wcsncpy (wcsBuffer, L"", nMaxCharCount - 1);

        char readBuffer [512];

        HKEY hKey;
        if (ERROR_SUCCESS != RegOpenKeyEx (HKEY_LOCAL_MACHINE, 
                                           L"Comm\\IrDA\\Linkage", 
                                           0, 
                                           KEY_READ, 
                                           &hKey))
        {
            return NULL;
        }

        DWORD dataType;
        DWORD bufferSize = sizeof (readBuffer);

        if (ERROR_SUCCESS != RegQueryValueEx (hKey, 
                                              L"Bind",
                                              NULL,
                                              &dataType,
                                              (LPBYTE) readBuffer,
                                              &bufferSize))
        {
            RegCloseKey (hKey);
            return NULL;
        }

        RegCloseKey (hKey);

        if (0 == bufferSize)
        {
            return NULL;
        }
        if (bufferSize >= sizeof (readBuffer) - 2)
        {
            return NULL;
        }
        readBuffer [bufferSize] = 0;
        readBuffer [bufferSize + 1] = 0;

        wchar_t keyPath [128];
        wsprintf (keyPath, L"Comm\\%s\\Parms", readBuffer);

        if (ERROR_SUCCESS != RegOpenKeyEx (HKEY_LOCAL_MACHINE, 
                                           keyPath, 
                                           0, 
                                           KEY_READ, 
                                           &hKey))
        {
            return NULL;
        }

        if (ERROR_SUCCESS != RegQueryValueEx (hKey, 
                                              L"Port",
                                              NULL,
                                              &dataType,
                                              (LPBYTE) readBuffer,
                                              &bufferSize))
        {
            RegCloseKey (hKey);
            return NULL;
        }
        RegCloseKey (hKey);

        if (bufferSize != sizeof (DWORD)) return false;

        DWORD nPort = * (DWORD*) readBuffer;
        wsprintf (wcsBuffer, L"COM%d", nPort);

        return wcsBuffer;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       AutoConfig
    //
    //  Description:
    //
    //  Автоматически конфигурирует SIR транспорт, читая из реестра 
    //  нужные сведения.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool SIRTransport::AutoConfig ()
    {
        SetLastErrorString (L"OK");
        wchar_t wcsPortName [MAX_TRANSPORT_NAME_LENGTH];
        wchar_t *wcsName = ReadTransportNameFromRegostry (wcsPortName, 
                                                          SIZE_OF (wcsPortName));
        if (NULL == wcsName) 
        {
            SetLastErrorString (L"Не удалось прочитать из реестра имя IR порта");
            return false;
        }
        wcscat (wcsName, L":");

        return Init (wcsName);
    }

}