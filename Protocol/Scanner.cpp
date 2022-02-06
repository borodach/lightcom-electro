///////////////////////////////////////////////////////////////////////////////
//
//  File:           Scanner.cpp
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
* $History: Scanner.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/Scanner/Protocol
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:15
 * Updated in $/LightCom/C++/Electro/Scanner/Protocol
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 7  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Protocol
 * 
 * *****************  Version 6  *****************
 * User: Sergey       Date: 30.01.07   Time: 20:51
 * Updated in $/PPCMercury.root/Protocol
 * Исправлено сообщение об ошибке чтения мощностей
 * 
 * *****************  Version 5  *****************
 * User: Sergey       Date: 28.01.07   Time: 23:11
 * Updated in $/PPCMercury.root/Protocol
 * Внесены изменения, в соотетствии с замечаниями ЭнергоСбыта
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 28.01.07   Time: 20:52
 * Updated in $/PPCMercury.root/Protocol
 * Отключен вывод сообщений об ошибках
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * Первая версия
* 
*/

#define SUPRESS_ERROR_REPORT

//
// Количество попыток чтения перед тем, как показать сообщение об ошибке.
//

#define MAX_RETRY_COUNT 32 

//
// Начальное значение даты
//

#define INITIAL_DATE_VALUE 100000

//
// Апапка с аудиофайлами
//

#define SOUND_FOLDER L"\\Sounds\\"

#include <windows.h>
#include "RegKeys.h"
#include "Scanner.hpp"
#include "Manager.hpp"

#include "Connect.hpp"
#include "Response.hpp"
#include "ReadCurrentTime.hpp"
#include "ReadLocation.hpp"
#include "ReadNetAddress.hpp"
#include "ReadSerialNumber.hpp"
#include "ReadTransformData.hpp"
#include "ReadVersion.hpp"
#include "ReadProductionType.hpp"
#include "ReadEnergy.hpp"
#include "ReadPower.hpp"
#include "ReadLastPowerAddress.hpp"
#include "ReadLogEntry.hpp"

#include "Disconnect.hpp"
#include "Utils.h"

//
// Количество месяцев, для которых нужно получить мощности
//

#define POWER_MOTNH_COUNT 2

namespace Mercury
{
    //
    // Статический экземпляр класса.
    //

    Scanner Scanner::m_Instance; 


    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Scanner
    //
    //  Description:
    //
    //  Конструктор.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Scanner::Scanner () 
    {
        m_bStopFlag = false;
        m_hWnd = NULL;

        m_nMonthCount = 3;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ~Scanner
    //
    //  Description:
    //
    //  Деструктор.
    //
    ///////////////////////////////////////////////////////////////////////////////

    Scanner::~Scanner ()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetStopRequest
    //
    //  Description:
    //
    //  Устнановка значения запроса на остановку
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Scanner::SetStopRequest (bool bRequest/* = true*/)
    {
        m_bStopFlag = bRequest;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       IsStopRequestSet
    //
    //  Description:
    //
    //  Устнановка значения запроса на остановку
    //
    ///////////////////////////////////////////////////////////////////////////////


    bool Scanner::IsStopRequestSet () const
    {
        return m_bStopFlag;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       SetWindow
    //
    //  Description:
    //
    //  Установка окна для получения сообщений.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Scanner::SetWindow (HWND hWnd) 
    {
        m_hWnd = hWnd;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       AddToLog
    //
    //  Description:
    //
    //  Добавляет запись в лог.
    //
    ///////////////////////////////////////////////////////////////////////////////

    void Scanner::AddToLog (const wchar_t *wcsMessage)
    {
        if (! IsWindow (m_hWnd)) return;
        SendMessage (m_hWnd, WM_USER + 1, 0, (LPARAM) wcsMessage);
    }

#define CHECK_ABORT_FLAG if (IsStopRequestSet ()) {AddToLog(L"\nПрервано пользователем"); DoNotify (etInterrupted); return true;}
#define EXIT_WITH_MESSAGE(a) {SetLastErrorString (Manager::m_Instance.GetLastErrorString ());AddToLog(a); DoNotify(etFatalError); return false;}

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       DoScan
    //
    //  Description:
    //
    //  Выполняет сканирование счетчика.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool Scanner::DoScan (MeteringData &result,
                          Transport::ITransport &transport,
                          bool bPowerScanEnabled /*= true*/,
                          const Connect::Password &password/* = Connect::m_Level1DefaultPwd*/,
                          Connect::AccessLevel accessLevel/* = Connect::alLevel1*/,
                          Frame::NetAddress netAddress/* = Frame::m_nBroadcastNetAddress*/)
    {
        result.Reset ();
        SetLastErrorString (L"OK");

        Frame::CrcType crcType = Frame::crcInvalid;
        
        for (int nRetry = 0; nRetry < 50; ++ nRetry)
        {
            CHECK_ABORT_FLAG
            crcType = Manager::m_Instance.DetectCrcType (transport, 
                                                         netAddress);
            if (Frame::crcInvalid != crcType)
            {
                break;
            }
        }
        

        if (Frame::crcInvalid == crcType)
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"Не удалось определить тип контрольной суммы, используемой счетчиком\n%s", 
                Manager::m_Instance.GetLastErrorString ());
            SetLastErrorString (wcsError);
            DoNotify (etConnectionFailed);

            return false;
        }

        DoNotify (etConnect);

        ProtocolType protocolType = 
            Manager::m_Instance.Connect (transport,
                                         crcType, 
                                         netAddress, 
                                         password, 
                                         accessLevel);

        if (ptInvalid == protocolType)
        {
            wchar_t wcsError [MAX_LAST_ERROR_LENGTH];
            wsprintf (wcsError, L"Не удалось установить соединение со счетчиком\n%s", 
                Manager::m_Instance.GetLastErrorString ());
            SetLastErrorString (wcsError);
            DoNotify (etConnectionFailed);

            return false;
        }

        AddToLog (L"\nКанал связи со счетчиком открыт");                
        CHECK_ABORT_FLAG;

        result.SetProtocolType (protocolType);
        bool bResult = false;
        unsigned char buffer [64];

        //
        // Получение сетевого адреса.
        //
        
        ZeroMemory (buffer, sizeof (buffer));
        ReadNetAddress readNetAddressRequest (crcType);
        readNetAddressRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            readNetAddressRequest, 
            buffer);
        if (bResult)
        {
            netAddress = (Frame::NetAddress) buffer [1];
            result.SetNetAddress (netAddress);
            AddToLog (L"\nПрочитан сетевой адрес");                
        }
        else
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось прочитать сетевой адрес")
        }
        
        CHECK_ABORT_FLAG

        //
        // Получение текущего времени.
        //

        ZeroMemory (buffer, sizeof (buffer));
        ReadCurrentTime readTimeRequest (crcType);
        readTimeRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            readTimeRequest, 
            buffer);
        if (bResult)
        {
            MeteringData::DateTime &dateTime = result.GetCounterTime ();
            dateTime.Reset ();
            dateTime.ReadFromV1Buffer (buffer);
            result.SetWinter (buffer [7] != 0);
            result.SetCounterTime (dateTime);

            AddToLog (L"\nПолучено текущее время");
            
            if (bEvaluation)
            {
                wchar_t buf [64];
                wsprintf (buf, L"%04u.%02u.%02u", dateTime.GetYear (), dateTime.GetMonth (), dateTime.GetDay ());
                if (wcscmp (buf, EVALUATION_MAX_DATE) > 0)
                {
                    EXIT_WITH_MESSAGE (L"\nПробный период закончился.");
                }
            } 

        }
        else
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось получить текущее время");
        }

        CHECK_ABORT_FLAG

        //
        // Получение названия места расположения счетчика.
        //

        ZeroMemory (buffer, sizeof (buffer));
        ReadLocation readLocationRequest (protocolType, crcType);
        readLocationRequest.SetAddress (netAddress);
        
        bResult = Manager::m_Instance.SendRequest (transport,
            readLocationRequest, 
            buffer);
        if (bResult)
        {
            char szBuffer [32];
            if (ptSET == protocolType)
            {
                buffer [16] = 0;
                sprintf (szBuffer, "%s", buffer);    
            }
            else
            {
                unsigned int nVal = IntFromBinary (buffer);
                sprintf (szBuffer, "%u", nVal);
            }

            result.SetLocation (szBuffer);
                         
            AddToLog (L"\nПолучено описание положения");
        }
        else
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось получить описание положения");
        }

        CHECK_ABORT_FLAG

        //
        // Получение коэффициентов трансформации.
        //

        ZeroMemory (buffer, sizeof (buffer));
        ReadTransformData readTransformDataRequest (protocolType, crcType);
        readTransformDataRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            readTransformDataRequest, 
            buffer);
        if (bResult)
        {
            unsigned short nKn = ShortFromBinary (buffer);
            unsigned short nKt = ShortFromBinary (buffer + 2);
            result.SetKn (nKn);
            result.SetKt (nKt);

            AddToLog (L"\nПолучены коэффициенты\nтрансформации");
        }
        else
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось получить\nкоэффициенты трансформации");
        }

        //
        // Получение серийного номера.
        //

        ZeroMemory (buffer, sizeof (buffer));
        ReadSerialNumber readSNRequest (crcType);
        readSNRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            readSNRequest, 
            buffer);
        if (bResult)
        {
            unsigned int nVal = 0;
            if (ptMercury == protocolType)
            {
                nVal = buffer [0];
                nVal = nVal * 100 + buffer [1];
                nVal = nVal * 100 + buffer [2];
                nVal = nVal * 100 + buffer [3];
            }
            else
            {
                nVal = IntFromBinary (buffer);
            }

            result.SetSerialNumber (nVal);

            MeteringData::DateTime &dateTime = result.GetProductionDate ();
            dateTime.Reset ();
            dateTime.buffer [3] = BCDFromInt (buffer [4]);
            dateTime.buffer [4] = BCDFromInt (buffer [5]);
            dateTime.buffer [5] = BCDFromInt (buffer [6]);

            AddToLog (L"\nПолучен серийный номер");
        }
        else
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось получить серийный номер");
        }

        CHECK_ABORT_FLAG

        //
        // Получение информации о версии ПО.
        //

        ZeroMemory (buffer, sizeof (buffer));
        ReadVersion readVersionRequest (crcType);
        readVersionRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            readVersionRequest, 
            buffer);
        if (bResult)
        {
            char szBuffer [32];
            sprintf (szBuffer, "%02u.%02u.%02u", 
                buffer [0],
                buffer [1],
                buffer [2]);    
            result.SetSoftwareVersion (szBuffer);

            AddToLog (L"\nПолучена версия ПО");
        }
        else
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось получить версию ПО");
        }

        CHECK_ABORT_FLAG

        //
        // Чтение варианта исполнения
        //

        ZeroMemory (buffer, sizeof (buffer));
        ReadProductionType readProductionTypeRequest (protocolType, crcType);
        readProductionTypeRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            readProductionTypeRequest, 
            buffer);
        if (bResult)
        {
            ProductionType productionType;
            memset (productionType.rawData, 0, sizeof (productionType.rawData));
            memcpy (productionType.rawData, 
                    buffer, 
                    readProductionTypeRequest.GetResponseSize ());
            result.SetProductionType (productionType);

            AddToLog (L"\nПолучен вариант исполнения");
        }
        else 
        {
            EXIT_WITH_MESSAGE (L"\nНе удалось получить вариант исполнения");
        }

        DoNotify (etBaseScanCompleted);
        CHECK_ABORT_FLAG

        //
        // Информация о накопленной энергии
        //
   
        for (size_t nIdx = 0; nIdx < TARIFF_COUNT; ++ nIdx)
        {
            AllEnergyInfo &ei = result.GetEnergy () [nIdx];
            bool bResult = ScanEnergy (ei,
                                       nIdx,
                                       transport,
                                       netAddress,
                                       crcType,
                                       protocolType);
            if (! bResult)
            {
                EXIT_WITH_MESSAGE (L"\nНе удалось получить сведения об энергопотреблении");
            }
            if (0 == nIdx)
            {
                AddToLog (L"\nПрочитано сумм. энергопотр.");
            }
            else
            {
                wchar_t buf [64];
                wsprintf (buf, L"\nПрочитано энергопотр. тарифа №%d", nIdx);
                AddToLog (buf);    
            }
            
            CHECK_ABORT_FLAG
        }

        DoNotify (etEnergyRead);

        CHECK_ABORT_FLAG

        //
        // Журнал событий
        //

        ReadLogEntry::LogEntryType logEntryType = ReadLogEntry::letOnOff;
        ReadLogEntry readLogEntryRequest (protocolType, crcType);
        readLogEntryRequest.SetAddress (netAddress);

        result.ResetEventLog ();
        MeteringData::EventLogEntries* pEventLog = result.GetEventLog ();        
        wchar_t buf [64];
        for (; logEntryType < ReadLogEntry::letMaxValue; 
             logEntryType = (ReadLogEntry::LogEntryType) (((int) logEntryType) + 1))
        {
            readLogEntryRequest.SetLogEntryType (logEntryType);
            for (unsigned char nRecord = 0; nRecord < LOG_ENTRY_COUNT; ++ nRecord)
            {
                CHECK_ABORT_FLAG
                MeteringData::EventLogEntry &eventLogEntry = pEventLog [logEntryType] [nRecord];
                readLogEntryRequest.SetEntryNumber (nRecord);
                ZeroMemory (buffer, sizeof (buffer));
                bResult = Manager::m_Instance.SendRequest (transport,
                                                           readLogEntryRequest, 
                                                           buffer);
                if (bResult)
                {
                    if (ptSET == protocolType)
                    {
                        eventLogEntry.FirstValue ().ReadFromV1Buffer (buffer);
                        eventLogEntry.SetValid (true);
                        if (14 == readLogEntryRequest.GetResponseSize ())
                        {
                            eventLogEntry.SecondValue ().ReadFromV1Buffer (buffer + 7);                            
                        }
                    }
                    else
                    {
                        eventLogEntry.FirstValue ().ReadFromBuffer (buffer);
                        eventLogEntry.SetValid (true);
                        if (12 == readLogEntryRequest.GetResponseSize ())
                        {
                            eventLogEntry.SecondValue ().ReadFromBuffer (buffer + 6);                            
                        }
                    }
                }
                else
                {
                    EXIT_WITH_MESSAGE (L"\nНе удалось прочитать журнал событий");
                }
            }

            wsprintf (buf, L"\nПрочитано:\n%s",  Mercury::ReadLogEntry::m_EvenLogTypeNames [logEntryType]);
            AddToLog (buf);
        }
        DoNotify (etLogRead);
        AddToLog (L"\nПрочитан журнал событий");

        //
        // Чтение мощности
        //

        int nLastAddress = 0;
        int nStartAddress = 0;
        unsigned nT = 0;
        int nLastHour;  
        int nLastMinute;
        int nLastDay;   
        int nLastMonth; 
        int nLastYear;  
        unsigned nMaxAddress = 0x10000 - 0x10;

        if (bPowerScanEnabled)
        {
            AddToLog (L"\nНачато чтение профиля средних мощностей");
            DoNotify (etPowerStarted);
            Sleep (300);
            ZeroMemory (buffer, sizeof (buffer));
            ReadLastPowerAddress  readPowerAddressRequest (crcType);
            readPowerAddressRequest.SetAddress (netAddress);
            bResult = Manager::m_Instance.SendRequest (transport,
                                                       readPowerAddressRequest, 
                                                       buffer);
            if (!bResult) 
            {
                AddToLog (L"\nНе удалось получить адрес последней записи мощностей\nЧтение мощностей отменено");
                DoNotify (etPowerError);
                bPowerScanEnabled = false;
            }
            else
            {
                nLastAddress = buffer [1] + ((unsigned) 256)* buffer [0];

                if (buffer [2] & 0x10) 
                {
                    bool b64K = Is64KMemory (result);                    

                    if (!b64K)
                    {
                        nLastAddress |= 0x10000;
                        nMaxAddress = 0x20000 - 0x10;

                        AddToLog (L"\nИспользуется 17 битная адресация");
                    }
                    else
                    {
                        AddToLog (L"\nИспользуется 16 битная адресация");
                    }
                }
                nT = buffer [8];
                
                nLastHour   = IntFromBCD (buffer [3]);
                nLastMinute = IntFromBCD (buffer [4]);
                nLastDay    = IntFromBCD (buffer [5]);
                nLastMonth  = IntFromBCD (buffer [6]);
                nLastYear  = 2000 + IntFromBCD (buffer [7]);

                AddToLog (L"\nПолучен адрес последней записи мощностей:");
                wsprintf (buf, L"\n%02d:%02d %02d.%02d.%02d %d мин. 0x%05X 0x%X", 
                    nLastHour, nLastMinute, nLastDay, nLastMonth, nLastYear,
                    nT, nLastAddress, int (buffer [2]));
                AddToLog (buf);

                if (0 == nT) bPowerScanEnabled = false;
            }
        }
        CHECK_ABORT_FLAG
        if (bPowerScanEnabled)
        {   
            unsigned nWriteDelay = 0;
            unsigned nReadDelay = 0;
            unsigned nErrorCount = 0;
            unsigned nOldDay = INITIAL_DATE_VALUE;
            unsigned nOldMonth = INITIAL_DATE_VALUE;
            unsigned nOldYear = INITIAL_DATE_VALUE;

            ZeroMemory (buffer, sizeof (buffer));
            ReadPower  readPowerRequest (crcType);
            readPowerRequest.SetAddress (netAddress);

            PowerStorage &powerStg = result.GetPowerStorage ();

            bool bFirstTime = true;
            for (unsigned int nAddress = nLastAddress; true; 
                 nAddress = (nAddress != 0 ? nAddress - 0x10 : nMaxAddress))
            {
                CHECK_ABORT_FLAG
           
                readPowerRequest.SetMemoryAddress (nAddress);
                ZeroMemory (buffer, sizeof (buffer));
                bool bResult = Manager::m_Instance.SendRequest (transport,
                    readPowerRequest, 
                    buffer, nWriteDelay, nReadDelay);
                if (!bResult)
                {
                    if (nErrorCount % MAX_RETRY_COUNT == MAX_RETRY_COUNT - 1) 
                    {
                        nErrorCount = 0;
#ifndef SUPRESS_ERROR_REPORT                        
                        wsprintf (buf, 
                            L"\nНе удалось прочитать запись по адресу 0x%05X/\n%s", 
                                  nAddress,
                                  Manager::m_Instance.GetLastErrorString ());
                        AddToLog (buf);
#endif
                        DoNotify (etPowerConnectionLost);
                        if (::MessageBox (m_hWnd, L"Счётчик не отвечает на запросы чтения мощности.\nВозможно, произошел обрыв связи.\nПовторить попытки чтения (ДА)\nили остановить сканирование (НЕТ)?", 
                                          L"Внимание!", MB_YESNO) == IDNO)
                        {
                            return true;
                        }

                    }

                    nAddress += 0x10;
                    ++nErrorCount;
                    if (nErrorCount % 2) 
                    {
                        nWriteDelay = 150;        
                        nReadDelay = 150;
#ifndef SUPRESS_ERROR_REPORT
                        wsprintf (buf, 
                            L"\nПопытка №%d чтения по адресу 0x%05X не удалась:\n%s", 
                                  nErrorCount / 2 + 1,  nAddress - 0x10,
                                  Manager::m_Instance.GetLastErrorString ());
                        AddToLog (buf);
#endif
                        Sleep (500);
                        continue;
                    }

                    Sleep (100);
#ifndef SUPRESS_ERROR_REPORT
                    AddToLog (L"\nРеининиализация транспорта ...");
#endif
                    if (! transport.ReInit ())
                    {
#ifndef SUPRESS_ERROR_REPORT
                        wsprintf (buf, 
                            L"\nРеинициализация не удалась:\n%s",
                            transport.GetLastErrorString ());

                        AddToLog (buf);
#endif
                        continue;
                    }
#ifndef SUPRESS_ERROR_REPORT
                    AddToLog (L"\nРеинициализация выполнена");
#endif
                    protocolType =  Manager::m_Instance.Connect (transport,
                                                                 crcType, 
                                                                 netAddress, 
                                                                 password, 
                                                                 accessLevel);

                    if (ptInvalid == protocolType)
                    {
#ifndef SUPRESS_ERROR_REPORT
                        wsprintf (buf, 
                            L"\nНе удалось установить связь со счетчиком:\n%s", 
                            Manager::m_Instance.GetLastErrorString ());
                        AddToLog (buf);
#endif
                        
                    }
                    else
                    {
#ifndef SUPRESS_ERROR_REPORT
                        AddToLog (L"\nCвязь со счетчиком установлена");
#endif
                         continue;
                    }
                }
                else
                {
                    PowerInfo powerInfo;                
                    powerInfo.ReadFromV2Buffer (buffer);
                    
                    if (2000 == powerInfo.nYear) 
                    {
                        bFirstTime = false;
                        continue;    
                    }
                    /*
                    wsprintf (buf, 
                              L"\nДанные по адресу 0x%05X успешно прочитаны", 
                              nAddress);
                    AddToLog (buf);
                    */

                    if ((nOldYear != INITIAL_DATE_VALUE) && 
                        nOldMonth != powerInfo.nMonth ||
                        nOldYear != powerInfo.nYear)
                    {
                        DoNotify (etPowerMonthRead);
                    }

                    if (nOldMonth != powerInfo.nMonth ||
                        nOldYear != powerInfo.nYear || 
                        nOldDay != powerInfo.nDay)
                    {
                        wsprintf (buf, 
                              L"\n0x%05X %02d:%02d %02d.%02d.%02d  P+ = %d", 
                              //L"\n%02d.%02d.%02d %02d:%02d        P+ = %d", 
                              nAddress, 
                              powerInfo.nHour, powerInfo.nMinute, 
                              powerInfo.nDay, powerInfo.nMonth, powerInfo.nYear,                               
                              (int) powerInfo.nPPlus/*, powerInfo.nT*/);
                        AddToLog (buf);
                        nOldMonth = powerInfo.nMonth;
                        nOldYear = powerInfo.nYear;
                        nOldDay = powerInfo.nDay;
                    }

                    if ((nLastYear * 12 + nLastMonth) - (powerInfo.nYear * 12 + powerInfo.nMonth) > m_nMonthCount ||
                        (!bFirstTime && nAddress == nLastAddress))
                    {
                        AddToLog (L"\nДанные о мощности получены, сканирование остановлено");
                        DoNotify (etPowerCompleted);
                        break;
                    }
                    else
                    {
                        powerStg.push_back (powerInfo);
                    }

                    bFirstTime = false;
          
                    nErrorCount = 0;
                    nWriteDelay = 0;
                    nReadDelay = 0;
          
                }            
            }
         }                                                         

        //
        // Закрываем соединение.
        //

        Response::ResponseCode responseCode = Response::rcInvalid;
        responseCode = Response::rcInvalid;
        Disconnect disconnectRequest (crcType);
        disconnectRequest.SetAddress (netAddress);
        bResult = Manager::m_Instance.SendRequest (transport,
            disconnectRequest, 
           (unsigned char*) &responseCode);
        AddToLog (L"\nКанал связи закрыт\nСканирование завершено");
        DoNotify (etCompleted);

        return true;

    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ScanPower
    //
    //  Description:
    //
    //  Чтение массива средних мощностей в заданном периоде адресов.
    //
    ///////////////////////////////////////////////////////////////////////////////
/*
    void Scanner::ScanPower (unsigned nStartAddress, 
                             unsigned nFinishAddress,
                             Transport::ITransport &transport,
                             ReadPower &readPowerRequest, 
                             MaxPowerStorage &powerStorage,
                             ProtocolType protocolType,
                             double counterConst,
                             unsigned nKn,
                             unsigned nKt)
    {
        unsigned char buffer [64];
        unsigned nWriteDelay = 0;
        unsigned nReadDelay = 0;
        unsigned nErrorCount = 0;
        //for (unsigned nAddress = nStartAddress; nAddress < nFinishAddress; nAddress += 0x10)
        for (unsigned int nAddress = nFinishAddress; nAddress < nStartAddress; nAddress -= 0x00)
        {
            if (IsStopRequestSet () == true) 
            {
                return;
            }
       
            readPowerRequest.SetMemoryAddress (nAddress);
            bool bResult = Manager::m_Instance.SendRequest (transport,
                readPowerRequest, 
                buffer, nWriteDelay, nReadDelay);
            if (!bResult)
            {
            }
            else
            {
                nErrorCount = 0;
                nWriteDelay = 0;
                nReadDelay = 0;
                powerStorage.AddPowerInfo (buffer, counterConst, nKn, nKt);
            }            
        }
    }
    */

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       ScanEnergy
    //
    //  Description:
    //
    //  Полученеие сведений об энергии заданного тарифа.
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool Scanner::ScanEnergy (AllEnergyInfo &result,
                              unsigned char nTariff,
                              Transport::ITransport &transport,
                              Mercury::Frame::NetAddress netAddress,
                              Frame::CrcType crcType,
                              ProtocolType protocolType,
                              size_t nWriteTimeout/* = 20*/,
                              size_t nReadTimeout/* = 20*/)
    {
        bool bResult = false;
        result.Reset ();
        unsigned char buffer [64];
        ZeroMemory (buffer, sizeof (buffer));
        ReadEnergy readEnergy (crcType);
        readEnergy.SetAddress (netAddress);
        readEnergy.SetTariff (nTariff);

        if (IsStopRequestSet ()) {return false;}

        //
        // Со сброса
        //

        readEnergy.SetEnergyType (etFromReset);
        bResult = Manager::m_Instance.SendRequest (transport,
            readEnergy, 
            buffer, nWriteTimeout, nReadTimeout);
        if (bResult)
        {
            result.SetValid (true);
            EnergyInfo &ei = result.GetEnergyFromReset ();
            ei.ReadFromBuffer (buffer, protocolType);
        }
        else
        {
            return false;
        }

        if (IsStopRequestSet ()) {return false;}

        //
        // Сегодня
        //

        readEnergy.SetEnergyType (etToday);
        bResult = Manager::m_Instance.SendRequest (transport,
            readEnergy, 
            buffer, nWriteTimeout, nReadTimeout);
        if (bResult)
        {
            result.SetValid (true);
            EnergyInfo &ei = result.GetEnergyToday ();
            ei.ReadFromBuffer (buffer, protocolType);
        }
        else
        {
            return false;
        }

        if (IsStopRequestSet ()) {return false;}

        //
        // Вчера
        //

        readEnergy.SetEnergyType (etYesterday);
        bResult = Manager::m_Instance.SendRequest (transport,
            readEnergy, 
            buffer, nWriteTimeout, nReadTimeout);
        if (bResult)
        {
            result.SetValid (true);
            EnergyInfo &ei = result.GetEnergyYesterday ();
            ei.ReadFromBuffer (buffer, protocolType);
        }
        else
        {
            return false;
        }
        if (IsStopRequestSet ()) {return false;}

        //
        // В этот год
        // 

        readEnergy.SetEnergyType (etThisYear);
        bResult = Manager::m_Instance.SendRequest (transport,
            readEnergy, 
            buffer, nWriteTimeout, nReadTimeout);
        if (bResult)
        {
            result.SetValid (true);
            EnergyInfo &ei = result.GetEnergyThisYear ();
            ei.ReadFromBuffer (buffer, protocolType);
        }
        else
        {
            return false;
        }
        if (IsStopRequestSet ()) {return false;}

        //
        // В прошлый год
        // 

        readEnergy.SetEnergyType (etLastYear);
        bResult = Manager::m_Instance.SendRequest (transport,
            readEnergy, 
            buffer, nWriteTimeout, nReadTimeout);
        if (bResult)
        {
            result.SetValid (true);
            EnergyInfo &ei = result.GetEnergyLastYear ();
            ei.ReadFromBuffer (buffer, protocolType);
        }
        else
        {
            return false;
        }
        if (IsStopRequestSet ()) {return false;}

        //
        // По месяцам
        //

        readEnergy.SetEnergyType (etMonth);
        EnergyInfo *monthlyInfo = result.GetEnergyMonth ();
        for (size_t nMonth = 1; nMonth < 13; ++ nMonth, ++ monthlyInfo)
        {
            if (IsStopRequestSet ()) {return false;}
            readEnergy.SetMonth (nMonth);
            bResult = Manager::m_Instance.SendRequest (transport,
                                                       readEnergy, 
                                                       buffer, nWriteTimeout, nReadTimeout);
            if (bResult)
            {
                result.SetValid (true);
                monthlyInfo->ReadFromBuffer (buffer, protocolType);
            }
            else
            {
                return false;
            }
        }

        return true;
    }    

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       Is64KMemory
    //
    //  Description:
    //
    //
    //  Возвращает true, если в счетчике 64K памяти
    //
    ///////////////////////////////////////////////////////////////////////////////

    bool Scanner::Is64KMemory (const MeteringData &data) const
    {
        //if (!data.IsProductionTypeValid ())
        {
            if (ptMercury == data.GetProtocolType ())
            {
                size_t nCount = m_Mercury128K.size ();
                for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
                {
                    if (data.GetSerialNumber () == m_Mercury128K [nIdx])
                    {
                        return false;
                    }
                }
            }
        }

        return true;
        //const ProductionType &pt = data.GetProductionType ();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Function:       DoNotify
    //
    //  Description:
    //
    //  Обработчик событий
    //
    ///////////////////////////////////////////////////////////////////////////////
    
    void Scanner::DoNotify (EventType et) const
    {
        struct NotifyInfo
        {
            EventType eventType;
            const wchar_t *wcsFileName;
        };
        NotifyInfo table [] = 
        {
            {etConnect,             L"ConnectionOK.wav"},           // Cоединение со счетчиком установлено
            {etConnectionFailed,    L"ConnectionFailed.wav"},       // Не удалось установить соединение со счетчиком
            {etFatalError,          L"FatalError.wav"},             // Сканирование аварийно завершено
            {etInterrupted,         L"Interrupted.wav"},            // Сканирование прервано пользователем
            {etBaseScanCompleted,   L"BaseScanCompleted.wav"},      // Прочитаны базовые сведения о счетчике
            {etCompleted,           L"ScanCompleted.wav"},          // Сканирование завершено
            {etEnergyRead,          L"EnergyScanCompleted.wav"},    // Регистры накопленной энергии прочитаны
            {etLogRead,             L"LogScanCompleted.wav"},       // Журнал событий прочитан
            {etPowerMonthRead,      L"PowerMonthCompleted.wav"},    // Прочитаны записи средних мощностей за месяц
            {etPowerError,          L"PowerScanError.wav"},         // Не удалось найти последнюю запись средних мощностей.
            {etPowerStarted,        L"PowerScanStarted.wav"},       // Начато чтение записей средних мощностей.
            {etPowerCompleted,      L"PowerScanCompleted.wav"},     // Записей средних мощностей прочитаны.
            {etPowerConnectionLost, L"PowerScanLostConnection.wav"},// Внимание. Потеряно соединение со счетчиком.
        };

        for (int i = 0; i < SIZE_OF (table); ++i)
        {
            if (et == table [i].eventType)
            {
                wchar_t wcsFilePath [1024];
                if (NULL != GetProgramFolder (wcsFilePath, 
                    SIZE_OF (wcsFilePath)))
                {
                    wcscat (wcsFilePath, SOUND_FOLDER);
                    wcscat (wcsFilePath, table [i].wcsFileName);
                    PlaySound (wcsFilePath, NULL, SND_FILENAME | SND_SYNC);
                }

                return;
            }
        }
    }
}