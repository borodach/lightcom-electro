///////////////////////////////////////////////////////////////////////////////
//
//  File:           Scanner.hpp
//
//  Facility:       Чтение информации о счетчике
//
//
//  Abstract:       Класс, управляющий сканированием счетчика
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
* $History: Scanner.hpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:16
 * Updated in $/LightCom/C++/Electro/Scanner/Protocol/include
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

#ifndef _SCANNER_
#define _SCANNER_

#include "LastError.hpp"
#include "ITransport.hpp"
#include "Frame.hpp"
#include "Connect.hpp"
#include "ReadPower.hpp"
#include "MeteringData.hpp"
#include <vector>

namespace Mercury
{
    class Scanner : public LastError
    {
    public:

        //
        // Тип события
        //

        enum EventType
        {
            
            etConnect,          // Cоединение со счетчиком установлено
            etConnectionFailed, // Не удалось установить соединение со счетчиком
            etFatalError,       // Сканирование аварийно завершено
            etInterrupted,      // Сканирование прервано пользователем
            etBaseScanCompleted,// Прочитаны базовые сведения о счетчике
            etCompleted,        // Сканирование завершено
            etEnergyRead,       // Регистры накопленной энергии прочитаны
            etLogRead,          // Журнал событий прочитан
            etPowerMonthRead,   // Прочитаны записи средних мощностей за месяц
            etPowerError,       // Не удалось найти последнюю запись средних мощностей.
            etPowerStarted,     // Начато чтение записей средних мощностей.
            etPowerCompleted,   // Записей средних мощностей прочитаны.
            etPowerConnectionLost, // Внимание. Потеряно соединение со счетчиком.
        };

        //
        // Обработчик событий
        //

        void DoNotify (EventType et) const;
        
        //
        // Хранилище серийных номеров
        //

        typedef std::vector <unsigned> SerialNumberStorage;

        //
        // Конструктор
        //

        Scanner ();

        //
        // Деструктор
        //

        virtual ~Scanner ();

        //
        // Выполняет сканирование счетчика.
        //

        bool DoScan (MeteringData &result,
                     Transport::ITransport &transport,
                     bool bPowerScanEnabled = true,
                     const Mercury::Connect::Password &password = Mercury::Connect::m_Level1DefaultPwd,
                     Mercury::Connect::AccessLevel accessLevel = Mercury::Connect::alLevel1,
                     Mercury::Frame::NetAddress netAddress = Mercury::Frame::m_nBroadcastNetAddress);                         

        //
        // Устнановка значения запроса на остановку
        //

        void SetStopRequest (bool bRequest = true);

        //
        // Чтение запроса на остановку
        //

        bool IsStopRequestSet () const;

        //
        // Установка окна для получения сообщений.
        //

        void SetWindow (HWND hWnd);

        //
        // Количество месяцев для чтения профиля мощностей
        //

        unsigned GetMonthCount () const {return m_nMonthCount;}
        void SetMonthCount (unsigned nCount) {m_nMonthCount = nCount;}

        //
        // Серийные номера счетчиков "Меркурий", не поддерживающих 17-битную адресацию.
        //

        SerialNumberStorage& GetMercury128K () {return m_Mercury128K;}
        const SerialNumberStorage& GetMercury128K () const {return m_Mercury128K;}

    protected:

        //
        // Полученеие сведений об энергии заданного тарифа.
        //

        bool ScanEnergy (AllEnergyInfo &result,
                         unsigned char nTariff,
                         Transport::ITransport &transport,
                         Frame::NetAddress netAddress,
                         Frame::CrcType crcType,
                         ProtocolType protocolType,
                         size_t nWriteTimeout = 50,
                         size_t nReadTimeout = 50);

        //
        // Чтение массива средних мощностей в заданном bynthdfkt адресов.
        //
        /*
        bool ScanPower (unsigned nStartAddress, 
                        unsigned nFinishAddress,
                        Transport::ITransport &transport,
                        Mercury::ReadPower &readPowerRequest, 
                        MaxPowerStorage &powerStorage,
                        ProtocolType protocolType,
                        double counterConst,
                        unsigned nKn,
                        unsigned nKt);
        */

        //
        // Добавляет запись в лог.
        //

        virtual void AddToLog (const wchar_t *wcsMessage);

        //
        // Возвращает true, если в счетчике 64K памяти
        //

        bool Is64KMemory (const MeteringData &data) const;

    public:

        //
        // Статический экземпляр класса.
        //

        static Scanner m_Instance; 

    protected:

        //
        // Признак запроса на остановку сканирования.
        //

        bool m_bStopFlag;

        //
        // Окно для получения сообщений.
        //

        HWND m_hWnd;

        //
        // Количество месяцев для чтения профиля мощностей
        //

        unsigned m_nMonthCount;

        //
        // Серийные номера счетчиков "Меркурий", поддерживающих 17-битную адресацию.
        //

        SerialNumberStorage m_Mercury128K;
    };      
}
#endif
