///////////////////////////////////////////////////////////////////////////////
//
//  File:           Frame.hpp
//
//  Facility:       Протокол Меркурий
//
//
//  Abstract:       Класс фрейма протокола. Реализует подсчет контрольной суммы.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  19-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Frame.hpp $
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

#ifndef _MERCURY_FRAME_
#define _MERCURY_FRAME_

namespace Mercury
{
    //
    // Тип протокола. К сожалению, точные ноазвания протоколов неизвестны.
    //

    enum ProtocolType
    {
        ptInvalid = 0x00,   // Невалидное значение
        ptSET   = 0x01,   // Протокол, описанный в документе prot_SET.doc
        ptMercury   = 0x02    // Протокол, описанный в документе protocol_merc.DOC
    };

    //
    // Класс фрейма протокола. Реализует подсчет контрольной суммы.
    //

    class Frame
    {
    public:

        //
        // Тип данных для хранения тела фрейма.
        //

        typedef unsigned char * FrameBody;

        //
        // Тип данных для хранения сетевого адреса.
        //

        typedef unsigned char NetAddress;

        //
        // Тип вида контрольной суммы.
        //

        enum CrcType
        {
            crcInvalid = 0xFF,  // Невалидная
            crc        = 0x00,  // Однобайтовая
            modbus     = 0x01,  // Двухбайтовая
        };

        //
        // Тип значения контрольной суммы.
        //

        typedef unsigned short CRCValue;

        //
        // Конструктор.
        //  nSize - Размер фрейма без полей сетевого адреcа и контрольной суммы
        //  nType - Тип контрольной суммы.
        //
        Frame (size_t nSize = 0, CrcType nType = crc);
        
        //
        // Деструктор
        //

        virtual ~Frame ();

        //
        // Байты тела фрейма.
        //

        FrameBody GetFrameBoby () const;
        
        //
        // Полный размер фрейма в байтах.
        //

        int GetFrameSize () const;

        //
        // Тип контрольной суммы.
        //

        CrcType GetCRCType () const;

        //
        // Сетевой адрес
        //

        NetAddress GetAddress () const;

        //
        // Сетевой адрес
        //

        void SetAddress (NetAddress address);
          
        //
        //  Возвращает контрольную сумму, хранимую в теле фрейма.
        //

        CRCValue GetCrc () const;
        
        //
        //  Записывает контрольную сумму в тело фрейма.
        //
        
        void SetCrc (CRCValue crcValue);

        //
        // Вычисление контрольной суммы для фрейма. 
        // Возвращает контрольную сумму.
        //

        CRCValue CalculateCrc ();

        //
        // Вычисление контрольной суммы фрейма и запись ее во фрейм. 
        //

        void CalculateAndStoreCrc ();
         
        //
        //  Вычисление простой контрольной суммы. 
        //      bytes - Данные, для которых считается контрольная сумма.
        //      start - Индекс первого байта, с которого начинается  подсчет
        //      count - Количество байтов, участвующих в подсчете
        //  Возвращает контрольную сумму 
        //

        static CRCValue CalculateCrc (unsigned char *bytes, 
                                      size_t nStart, 
                                      size_t nCount);
        
        //
        //  Вычисление контрольной суммы аналогично MODBUS. 
        //      bytes - Данные, для которых считается контрольная сумма.
        //      start - Индекс первого байта, с которого начинается  подсчет
        //      count - Количество байтов, участвующих в подсчете
        //  Возвращает контрольную сумму. 
        //

        static CRCValue CalculateModbus (unsigned char *bytes, 
                                         size_t nStart, 
                                         size_t nCount);

        public:

             /* Адрес 0 используется как общий, на него отвечают все счетчики сети и 
             * исполь-зовать его можно только в случае индивидуальной работы с 
             * одним счетчиком. Кроме того, по нулевому адресу запрещены все 
             * операции записи данных, если собственный адрес счетчика не нулевой.
             * Адрес FFh зарезервирован как адрес по умолчанию после инициализации 
             * счет-чиков СЭТ-4ТМ.02 версий 14.ХХ.ХХ и выше (в основном это 
             * заводская процедура). Для счетчиков СЭТ-4ТМ.01 любых версий и для 
             * счетчиков СЭТ-4ТМ.02 версий ниже 14.ХХ.ХХ после инициализации 
             * устанавливается адрес 00h. 
             * Адрес FEh используется как адрес для широковещательных сообщений 
             * (для СЭТ-4ТМ.02 с версии 22.23.ХХ).
             */

            //
            // Широковещательный сетевой адрес.
            //

            static const NetAddress m_nBroadcastNetAddress = 0x00;

            //
            // Адрес счетчика по-умолчанию. 
            //

            static const NetAddress m_nDefaultNetAddress = 0xFF;

        protected:
            
            //
            // Константа для вычисления контрольной суммы в стиле MODBUS.
            //

            static const unsigned short m_nInitCRC = 0xFFFF;

            //
            // Массивы для вычисления контрольной суммы в стиле MODBUS.
            //

            static const unsigned char m_srCRCLo [];
            static const unsigned char m_srCRCHi []; 

            //
            // Буфер, хранящий тело фрейма.
            //

            FrameBody m_Body;

            //
            // Размер тела фрейма в байтах
            //

            size_t m_nSize;

            //
            // Тип контрольной суммы.
            //

            CrcType m_crcType;
    };
}

#endif //_MERCURY_FRAME_