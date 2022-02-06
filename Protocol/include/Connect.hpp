///////////////////////////////////////////////////////////////////////////////
//
//  File:           Connect.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма запроса на установление соединения
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
* $History: Connect.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol/include
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol/include
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Protocol/include
* 
*/

#ifndef _MERCURY_CONNECT_
#define _MERCURY_CONNECT_    

#include "Request.hpp"

namespace Mercury
{
    
    class Connect: public Request
    {
    public:

        //
        // Тип данных - уровень доступа.
        //

        enum AccessLevel : unsigned char
        {
            alInvalid = 0xFF,
            alLevel1  = 0x01,   // Режим пользователя
            alLevel2  = 0x02    // Режим хозяина
        };

        //
        // Тип данных - пароль.
        //

        typedef unsigned char Password [6];

        //
        // Пароль по-умолчанию для потребительского режима.
        //

        static const Password m_Level1DefaultPwd;

        //
        // Пароль по-умолчанию для режима "хозяина".
        //

        static const Password m_Level2DefaultPwd;

        //
        // Конструктор
        //

        Connect (ProtocolType protocolType = ptSET,
                 CrcType nType = crc, 
                 const Password &pwd = m_Level1DefaultPwd,
                 Connect::AccessLevel nLevel = Connect::alLevel1);


        //
        // Возвращает пароль.
        //

        const Password&  GetPassword () const;
       
        //
        // Устанавливает пароль.
        //

        void SetPassword (const Password& pwd);

        //
        // Возвращает уровень доступа.
        //

        AccessLevel GetAccessLevel () const;
       
        //
        // Устанавливает уровень доступа.
        //

        void SetAccessLevel (AccessLevel nLevel);

        //
        // Возвращает тип протокола.
        //

        ProtocolType GetProtocolType () const;

    protected:
        
        //
        // Тип протокла
        //

        ProtocolType m_nProtocolType;
    };
}
#endif