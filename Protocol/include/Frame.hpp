///////////////////////////////////////////////////////////////////////////////
//
//  File:           Frame.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ������ ���������. ��������� ������� ����������� �����.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  19-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
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
    // ��� ���������. � ���������, ������ ��������� ���������� ����������.
    //

    enum ProtocolType
    {
        ptInvalid = 0x00,   // ���������� ��������
        ptSET   = 0x01,   // ��������, ��������� � ��������� prot_SET.doc
        ptMercury   = 0x02    // ��������, ��������� � ��������� protocol_merc.DOC
    };

    //
    // ����� ������ ���������. ��������� ������� ����������� �����.
    //

    class Frame
    {
    public:

        //
        // ��� ������ ��� �������� ���� ������.
        //

        typedef unsigned char * FrameBody;

        //
        // ��� ������ ��� �������� �������� ������.
        //

        typedef unsigned char NetAddress;

        //
        // ��� ���� ����������� �����.
        //

        enum CrcType
        {
            crcInvalid = 0xFF,  // ����������
            crc        = 0x00,  // ������������
            modbus     = 0x01,  // ������������
        };

        //
        // ��� �������� ����������� �����.
        //

        typedef unsigned short CRCValue;

        //
        // �����������.
        //  nSize - ������ ������ ��� ����� �������� ����c� � ����������� �����
        //  nType - ��� ����������� �����.
        //
        Frame (size_t nSize = 0, CrcType nType = crc);
        
        //
        // ����������
        //

        virtual ~Frame ();

        //
        // ����� ���� ������.
        //

        FrameBody GetFrameBoby () const;
        
        //
        // ������ ������ ������ � ������.
        //

        int GetFrameSize () const;

        //
        // ��� ����������� �����.
        //

        CrcType GetCRCType () const;

        //
        // ������� �����
        //

        NetAddress GetAddress () const;

        //
        // ������� �����
        //

        void SetAddress (NetAddress address);
          
        //
        //  ���������� ����������� �����, �������� � ���� ������.
        //

        CRCValue GetCrc () const;
        
        //
        //  ���������� ����������� ����� � ���� ������.
        //
        
        void SetCrc (CRCValue crcValue);

        //
        // ���������� ����������� ����� ��� ������. 
        // ���������� ����������� �����.
        //

        CRCValue CalculateCrc ();

        //
        // ���������� ����������� ����� ������ � ������ �� �� �����. 
        //

        void CalculateAndStoreCrc ();
         
        //
        //  ���������� ������� ����������� �����. 
        //      bytes - ������, ��� ������� ��������� ����������� �����.
        //      start - ������ ������� �����, � �������� ����������  �������
        //      count - ���������� ������, ����������� � ��������
        //  ���������� ����������� ����� 
        //

        static CRCValue CalculateCrc (unsigned char *bytes, 
                                      size_t nStart, 
                                      size_t nCount);
        
        //
        //  ���������� ����������� ����� ���������� MODBUS. 
        //      bytes - ������, ��� ������� ��������� ����������� �����.
        //      start - ������ ������� �����, � �������� ����������  �������
        //      count - ���������� ������, ����������� � ��������
        //  ���������� ����������� �����. 
        //

        static CRCValue CalculateModbus (unsigned char *bytes, 
                                         size_t nStart, 
                                         size_t nCount);

        public:

             /* ����� 0 ������������ ��� �����, �� ���� �������� ��� �������� ���� � 
             * ������-������ ��� ����� ������ � ������ �������������� ������ � 
             * ����� ���������. ����� ����, �� �������� ������ ��������� ��� 
             * �������� ������ ������, ���� ����������� ����� �������� �� �������.
             * ����� FFh �������������� ��� ����� �� ��������� ����� ������������� 
             * ����-����� ���-4��.02 ������ 14.��.�� � ���� (� �������� ��� 
             * ��������� ���������). ��� ��������� ���-4��.01 ����� ������ � ��� 
             * ��������� ���-4��.02 ������ ���� 14.��.�� ����� ������������� 
             * ��������������� ����� 00h. 
             * ����� FEh ������������ ��� ����� ��� ����������������� ��������� 
             * (��� ���-4��.02 � ������ 22.23.��).
             */

            //
            // ����������������� ������� �����.
            //

            static const NetAddress m_nBroadcastNetAddress = 0x00;

            //
            // ����� �������� ��-���������. 
            //

            static const NetAddress m_nDefaultNetAddress = 0xFF;

        protected:
            
            //
            // ��������� ��� ���������� ����������� ����� � ����� MODBUS.
            //

            static const unsigned short m_nInitCRC = 0xFFFF;

            //
            // ������� ��� ���������� ����������� ����� � ����� MODBUS.
            //

            static const unsigned char m_srCRCLo [];
            static const unsigned char m_srCRCHi []; 

            //
            // �����, �������� ���� ������.
            //

            FrameBody m_Body;

            //
            // ������ ���� ������ � ������
            //

            size_t m_nSize;

            //
            // ��� ����������� �����.
            //

            CrcType m_crcType;
    };
}

#endif //_MERCURY_FRAME_