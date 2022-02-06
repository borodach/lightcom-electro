///////////////////////////////////////////////////////////////////////////////
//
//  File:           Manager.cpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс, выполняющий высокоуровневые запросы протокола
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
* $History: Manager.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * Первая версия
* 
*/

#include <windows.h>
#include <string.h>
#include "Manager.hpp"
#include "TestConnection.hpp"
#include "Response.hpp"

namespace Mercury
{
    //
    //##Статический экземпляр класса
    //

    Manager Manager::m_Instance;
    
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Manager
    //
    //  Description:
    //
    //  Конструктор
    //
    ///////////////////////////////////////////////////////////////////////////////

    Manager::Manager ()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ~Manager
    //
    //  Description:
    //
    //  Деструктор
    //
    ///////////////////////////////////////////////////////////////////////////////

    Manager::~Manager ()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       DetectCrcType
    //
    //  Description:
    //
    //  Выясняет тип контрольной суммы.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Frame::CrcType Manager::DetectCrcType (Transport::ITransport &transport, 
                                           Frame::NetAddress nAddress/* = Frame::m_nBroadcastNetAddress*/,
                                           size_t nWriteTimeout/* = 150*/,
                                           size_t nReadTimeout/* = 150*/)
    {
        SetLastErrorString (L"OK");
        Mercury::TestConnection testCrcConnection (Frame::crc);
        testCrcConnection.SetAddress (nAddress);

        Response::ResponseCode responseCode = Response::rcInvalid;
        if (SendRequest (transport, 
                         testCrcConnection, 
                         (unsigned char*) &responseCode, 
                         nWriteTimeout, 
                         nReadTimeout))
        {
            if (Response::rcOK == responseCode)
            {
                return Frame::crc;
            }
        }
            
        responseCode = Response::rcInvalid;
        Mercury::TestConnection testModbusConnection (Mercury::Frame::modbus);
        testModbusConnection.SetAddress (nAddress);
        if (SendRequest (transport, 
                         testModbusConnection, 
                         (unsigned char*) &responseCode,
                         nWriteTimeout, 
                         nReadTimeout))
        {
            if (Response::rcOK == responseCode)
            {
                return Frame::modbus;
            }
        }

        wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
        wsprintf (wcsError, 
                  L"Не удалось определить тип контрольной суммы:\n%s", 
                  GetLastErrorString ());
        SetLastErrorString (wcsError);

        return Frame::crcInvalid;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       WriteFrame
    //
    //  Description:
    //
    //  Передает фрейм в заданный транспорт.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool Manager::WriteFrame (Transport::ITransport &transport, Frame &frame)
    {
        SetLastErrorString (L"OK");
        frame.CalculateAndStoreCrc ();
        bool bResult = transport.Write (frame.GetFrameBoby (), frame.GetFrameSize ());
        if (! bResult)
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, 
                L"Не удалось записать фрейм:\n%s", 
                transport.GetLastErrorString ());
            SetLastErrorString (wcsError);
        }

        return bResult;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ReadFrame
    //
    //  Description:
    //
    //  Читает фрейм из заданного транспорта.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool Manager::ReadFrame (Transport::ITransport &transport, Frame &frame)
    {
        SetLastErrorString (L"OK");

        unsigned char *buffer = frame.GetFrameBoby ();
        size_t nSize = frame.GetFrameSize ();

        Response errorResponse (frame.GetCRCType ());
        unsigned char *errorBuffer = errorResponse.GetFrameBoby ();
        size_t nErrorSize = errorResponse.GetFrameSize ();

        bool bResult = transport.Read (buffer, nErrorSize);
        if (! bResult)
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, 
                L"Не удалось прочитать первую часть фрейма:\n%s", 
                transport.GetLastErrorString ());
            SetLastErrorString (wcsError);

            return bResult;
        }
        if (nErrorSize != nSize)
        {
            bResult = transport.Read (buffer + nErrorSize,  nSize - nErrorSize);
            if (! bResult)
            {
                for (size_t nIdx = 0; nIdx < nErrorSize; ++ nIdx)
                {
                    errorBuffer [nIdx] = buffer [nIdx];
                }
                if (errorResponse.CalculateCrc () == errorResponse.GetCrc ())
                {
                    wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
                    
                    wsprintf (wcsError, 
                        L"Счетчик прислал сообщение об ошибке:\n%s (код 0x%02X)", 
                        Response::GetResponseString (errorResponse.GetResponseCode ()),
                        (int) errorResponse.GetResponseCode ());

                    SetLastErrorString (wcsError);
                }
                else
                {
                    wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
                    wsprintf (wcsError, 
                        L"Не удалось прочитать остаток фрейма:\n%s", 
                        transport.GetLastErrorString ());
                    SetLastErrorString (wcsError);
                }

                return bResult;
            }
        }
        
        Frame::CRCValue nRealCrc = frame.CalculateCrc ();
        if (nRealCrc != frame.GetCrc ())
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, 
                L"Получен фрейм с непрвильной контрольной суммой (конторольная сумма фрейма 0x%02X, реальная 0x%02X",
                (int) frame.GetCrc (),
                (int) nRealCrc);
            SetLastErrorString (wcsError);

            return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SendRequest
    //
    //  Description:
    //
    //  Выполняет запрос и записывает ответ в буфер pResponse.
    //  Буфер должен иметь размер не менее frame.GetResponseSize () байт.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool Manager::SendRequest (Transport::ITransport &transport, 
                               Request &frame, 
                               unsigned char *pResponse,
                               size_t nWriteTimeout/* = 150*/,
                               size_t nReadTimeout/* = 150*/,
                               size_t nRetryCount/* = 3*/)
    {
        for (size_t nRetry = 0; nRetry < nRetryCount; ++ nRetry)
        {
            SetLastErrorString (L"OK");

            bool bResult;
            bResult = WriteFrame (transport, frame);
            if (! bResult) 
            {
               continue;
            }
            Sleep (nWriteTimeout);

            size_t nResponseSize = frame.GetResponseSize ();
            Mercury::Frame responseFrame (nResponseSize, 
                                          frame.GetCRCType ());

            bResult = ReadFrame (transport, responseFrame);
            Sleep (nReadTimeout);
            if (! bResult) 
            {
                continue;
            }

            unsigned char *pResponseFrameBody = responseFrame.GetFrameBoby ();
            for (size_t nIdx = 0; nIdx < nResponseSize; ++ nIdx)
            {
                pResponse [nIdx] = pResponseFrameBody [nIdx + 1];
            }

            return true;
        }

        return false;        
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Connect
    //
    //  Description:
    //
    //  Открывает канал связи со счетчиком.
    //
    ///////////////////////////////////////////////////////////////////////////////

    ProtocolType Manager::Connect (Transport::ITransport &transport, 
                                   Frame::CrcType crcType/* = Frame::crcInvalid*/,
                                   Frame::NetAddress nAddress/* = Frame::m_nBroadcastNetAddress*/,
                                   const Connect::Password &password/* = Mercury::Connect::m_Level1DefaultPwd*/,
                                   Connect::AccessLevel accessLevel/* = Mercury::Connect::alLevel1*/,
                                   size_t nWriteTimeout/* = 150*/,
                                   size_t nReadTimeout/* = 150*/)
    {
        SetLastErrorString (L"OK");
        if (Frame::crcInvalid == crcType)
        {
            crcType = DetectCrcType (transport, 
                                     nAddress, 
                                     nWriteTimeout, 
                                     nReadTimeout);
            if (Frame::crcInvalid == crcType)
            {
                return ptInvalid;
            }
        }

        Mercury::Connect connectType1 (ptSET, crcType, password, accessLevel);
        connectType1.SetAddress (nAddress);
        Response::ResponseCode responseCode = Response::rcInvalid;
        if (SendRequest (transport, 
                         connectType1, 
                         (unsigned char*) &responseCode, 
                         nWriteTimeout, 
                         nReadTimeout))
        {
            if (Response::rcOK == responseCode)
            {
                return ptSET;
            }
        }

        Mercury::Connect connectType2 (ptMercury, crcType, password, accessLevel);
        connectType2.SetAddress (nAddress);
        responseCode = Response::rcInvalid;
        if (SendRequest (transport, 
                         connectType2, 
                         (unsigned char*) &responseCode, 
                         nWriteTimeout, 
                         nReadTimeout))
        {
            if (Response::rcOK == responseCode)
            {
                return ptMercury;
            }
        }

        wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
        wsprintf (wcsError, 
            L"Не удалось установить соединение со счетчиком:\n%s", 
            GetLastErrorString ());
        SetLastErrorString (wcsError);

        return ptInvalid;
    }
}