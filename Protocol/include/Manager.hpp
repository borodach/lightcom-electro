///////////////////////////////////////////////////////////////////////////////
//
//  File:           Manager.hpp
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
* $History: Manager.hpp $
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

#ifndef _MERCURY_MANAGER_
#define _MERCURY_MANAGER_    

#include "LastError.hpp"
#include "Frame.hpp"
#include "Request.hpp"
#include "Connect.hpp"
#include "ITransport.hpp"

#ifndef MAX_LAST_ERROR_LENGTH
#define MAX_LAST_ERROR_LENGTH     512
#endif

namespace Mercury
{
    class Manager: public LastError
    {
    public:
        
        //
        // Конструктор
        //

        Manager ();

        //
        // Деструктор
        //

        virtual ~Manager ();

        //
        // Выясняет тип контрольной суммы.
        //

        Frame::CrcType DetectCrcType (Transport::ITransport &transport, 
                                      Frame::NetAddress nAddress = Mercury::Frame::m_nBroadcastNetAddress,
                                      size_t nWriteTimeout = 150,
                                      size_t nReadTimeout = 150);

        //
        // Открывает канал связи со счетчиком.
        //

        ProtocolType Connect (Transport::ITransport &transport, 
                              Frame::CrcType crcType = Frame::crcInvalid,
                              Frame::NetAddress nAddress = Frame::m_nBroadcastNetAddress,
                              const Connect::Password &password = Mercury::Connect::m_Level1DefaultPwd,
                              Connect::AccessLevel accessLevel = Mercury::Connect::alLevel1,
                              size_t nWriteTimeout = 150,
                              size_t nReadTimeout = 150);

        //
        // Отправляет фрейм в счетчик.
        //

        bool WriteFrame (Transport::ITransport &transport, Frame &frame);

        //
        // Читает фрейм из счетчика.
        //

        bool ReadFrame (Transport::ITransport &transport, Frame &frame);

        //
        // Выполняет запрос и записывает ответ в буфер pResponse.
        // Буфер должен иметь размер не менее frame.GetResponseSize () байт.
        //

        bool SendRequest (Transport::ITransport &transport, 
                          Request &frame, 
                          unsigned char *pResponse,
                          size_t nWriteTimeout = 150,
                          size_t nReadTimeout = 150,
                          size_t nRetryCount = 3);

    public:

        //
        // Статический экземпляр класса.
        //

        static Manager m_Instance;

    };
}
#endif