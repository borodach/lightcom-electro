///////////////////////////////////////////////////////////////////////////////
//
//  File:           ProductionType.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ��� �������� �������� � �������� ���������� ��������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  29-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ProductionType.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Common/include
 * 
 * *****************  Version 5  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Common/include
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/Common/include
* 
*/

#ifndef _PRODUCTION_TYPE_
#define _PRODUCTION_TYPE_

#ifndef IO_DELIM
#define IO_DELIM ' '
#endif

//
// ������ ������� �������� (%).
//

const double PrecisionClasses [16] = {0.2, 0.5, 1.0, 2.0};

//
// ������ ����������� ���������� (�).
//

const double NominalVoltage [] = {57.7, 220, 0.0, 0.0};

//
// ������ ����������� ����� (�).
//

const double NominalCurrent [] = {5.0, 1.0, 10.0, 0};

//
// ������ ���������� �������� (���/���*�) ��� ������� ���������.
//

const double CounterConstonts1 [] = 
{
    5000.0, 25000.0, 1250.0, 6250.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0
};

//
// ������ ���������� �������� (���/���*�) ��� ������� ���������.
//

const double CounterConstonts2 [16] = 
{
    5000.0, 25000.0, 1250.0, 500.0,
    1000.0, 250.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0
};

//
// ������� ��������� ��������.
//

struct ProductionType
{
    
    //
    // ��������� ������� � �����
    //

    bool SaveGuts (std::ostream &os) const
    {
        os << sizeof (rawData);
        for (size_t nIdx = 0; nIdx < sizeof (rawData); ++nIdx)
        {
            os << IO_DELIM << (unsigned) rawData [nIdx];
        }

        return ! (! os);
    }

    //
    // �������������� ������� �� ������
    //

    bool RestoreGuts (std::istream &is)
    {
        size_t nSize = 0;
        is >> nSize;
        if (nSize > sizeof (rawData)) return false;
        for (size_t nIdx = 0; nIdx < nSize; ++nIdx)
        {
            unsigned nVal;
            is >> nVal;
            rawData [nIdx] = nVal;
        }

        return ! (! is);
    }

    //
    // ����� �������� �� �������� ������� (%)
    //

    double ActivePrecision () const 
    {
        int nPrecisionIdx = (rawData [0] >> 6) & 0x03;
        return PrecisionClasses [nPrecisionIdx];
    }

    //
    // ����� �������� �� ���������� ������� (%)
    //

    double ReactivePrecision () const 
    {
        int nPrecisionIdx = (rawData [0] >> 4) & 0x03;
        return PrecisionClasses [nPrecisionIdx];
    }

    //
    // ����������� ���������� (�)
    //

    double Voltage () const 
    {
        int nVoltageIdx = (rawData [0] >> 2) & 0x03;
        return NominalVoltage [nVoltageIdx];
    }

    //
    // ����������� ��� (�)
    //

    double Current () const 
    {
        int nCurrentIdx = (rawData [0]) & 0x03;
        return NominalCurrent [nCurrentIdx];
    }

    //
    // ����� ����������
    //

    unsigned DirectionCount () const
    {
        return (rawData [1] & 0x80) ? 1 : 2;
    }

    //
    // ������������� �������� (������� �������)
    //

    unsigned TemperatureRange () const
    {
        return (rawData [1] & 0x40) ? 40 : 20;
    }

    //
    // ����� ��� ��������
    //

    unsigned PhaseCount () const
    {
        return (rawData [1] & 0x10) ? 1 : 3;
    }

    //
    // ���������� �������� ��� ������� ��������� (���/���*�)
    //

    double CounterConst1 () const
    {
        unsigned nCounterIdx = rawData [1] & 0x0F;
        return CounterConstonts1 [nCounterIdx];
    }

    //
    // ���������� �������� ��� ������� ��������� (���/���*�)
    //

    double CounterConst2 () const
    {
        unsigned nCounterIdx = rawData [1] & 0x0F;
        return CounterConstonts2 [nCounterIdx];
    }

    //
    // ���������� ������ ������
    //

    size_t GetMemorySize () const
    {
        return (rawData [3] & 0x80) ? (128 * 1024) : (64 * 1024);
    }

    //
    // ������, ���������� �� ��������. �� ������ ������ ������������ ������ 
    // ������ ��� �����.
    //

    unsigned char rawData [6];
};

#endif
