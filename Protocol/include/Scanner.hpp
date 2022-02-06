///////////////////////////////////////////////////////////////////////////////
//
//  File:           Scanner.hpp
//
//  Facility:       ������ ���������� � ��������
//
//
//  Abstract:       �����, ����������� ������������� ��������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  24-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
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
        // ��� �������
        //

        enum EventType
        {
            
            etConnect,          // C��������� �� ��������� �����������
            etConnectionFailed, // �� ������� ���������� ���������� �� ���������
            etFatalError,       // ������������ �������� ���������
            etInterrupted,      // ������������ �������� �������������
            etBaseScanCompleted,// ��������� ������� �������� � ��������
            etCompleted,        // ������������ ���������
            etEnergyRead,       // �������� ����������� ������� ���������
            etLogRead,          // ������ ������� ��������
            etPowerMonthRead,   // ��������� ������ ������� ��������� �� �����
            etPowerError,       // �� ������� ����� ��������� ������ ������� ���������.
            etPowerStarted,     // ������ ������ ������� ������� ���������.
            etPowerCompleted,   // ������� ������� ��������� ���������.
            etPowerConnectionLost, // ��������. �������� ���������� �� ���������.
        };

        //
        // ���������� �������
        //

        void DoNotify (EventType et) const;
        
        //
        // ��������� �������� �������
        //

        typedef std::vector <unsigned> SerialNumberStorage;

        //
        // �����������
        //

        Scanner ();

        //
        // ����������
        //

        virtual ~Scanner ();

        //
        // ��������� ������������ ��������.
        //

        bool DoScan (MeteringData &result,
                     Transport::ITransport &transport,
                     bool bPowerScanEnabled = true,
                     const Mercury::Connect::Password &password = Mercury::Connect::m_Level1DefaultPwd,
                     Mercury::Connect::AccessLevel accessLevel = Mercury::Connect::alLevel1,
                     Mercury::Frame::NetAddress netAddress = Mercury::Frame::m_nBroadcastNetAddress);                         

        //
        // ���������� �������� ������� �� ���������
        //

        void SetStopRequest (bool bRequest = true);

        //
        // ������ ������� �� ���������
        //

        bool IsStopRequestSet () const;

        //
        // ��������� ���� ��� ��������� ���������.
        //

        void SetWindow (HWND hWnd);

        //
        // ���������� ������� ��� ������ ������� ���������
        //

        unsigned GetMonthCount () const {return m_nMonthCount;}
        void SetMonthCount (unsigned nCount) {m_nMonthCount = nCount;}

        //
        // �������� ������ ��������� "��������", �� �������������� 17-������ ���������.
        //

        SerialNumberStorage& GetMercury128K () {return m_Mercury128K;}
        const SerialNumberStorage& GetMercury128K () const {return m_Mercury128K;}

    protected:

        //
        // ���������� �������� �� ������� ��������� ������.
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
        // ������ ������� ������� ��������� � �������� bynthdfkt �������.
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
        // ��������� ������ � ���.
        //

        virtual void AddToLog (const wchar_t *wcsMessage);

        //
        // ���������� true, ���� � �������� 64K ������
        //

        bool Is64KMemory (const MeteringData &data) const;

    public:

        //
        // ����������� ��������� ������.
        //

        static Scanner m_Instance; 

    protected:

        //
        // ������� ������� �� ��������� ������������.
        //

        bool m_bStopFlag;

        //
        // ���� ��� ��������� ���������.
        //

        HWND m_hWnd;

        //
        // ���������� ������� ��� ������ ������� ���������
        //

        unsigned m_nMonthCount;

        //
        // �������� ������ ��������� "��������", �������������� 17-������ ���������.
        //

        SerialNumberStorage m_Mercury128K;
    };      
}
#endif
