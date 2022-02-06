///////////////////////////////////////////////////////////////////////////////
//
//  File:           EnergyInfo.hpp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ����� ��� �������� �������� � ����������� �������.
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
* $History: EnergyInfo.hpp $
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

#ifndef _ENERGY_INFO_
#define _ENERGY_INFO_

#ifndef IO_DELIM
#define IO_DELIM ' '
#endif

#ifndef BOOL_TO_INT
#define BOOL_TO_INT(a) (a ? 1 : 0)
#endif

#ifndef INT_TO_BOOL
#define INT_TO_BOOL(a) (a != 0)
#endif

#include "Utils.h"
#include "Frame.hpp"

//
// ���������� � ����������� �������
//

struct EnergyInfo 
{
    //
    // �����������
    //
    EnergyInfo ()
    {
        Reset ();
    }

    //
    // ��������� ��������� ���������. �������, ��� ����� ������� ����� ������
    // �����.
    //

    bool operator == (const EnergyInfo& other) const 
    {
        return true;
    }

    bool operator < (const EnergyInfo& other) const 
    {
        return false;
    }

    //
    // ��������� ���� ��������� �� �������� �����������
    //

    const EnergyInfo& operator *= (int k)
    {
        nActiveDirect    *= k;
        nActiveReverce   *= k;
        nReactiveDirect  *= k;
        nReactiveReverce *= k;

        return *this;
    }

    //
    // ���������� ���������
    //

    const EnergyInfo& operator += (const EnergyInfo& other)
    {
        nActiveDirect    += other.nActiveDirect;
        nActiveReverce   += other.nActiveReverce;
        nReactiveDirect  += other.nReactiveDirect;
        nReactiveReverce += other.nReactiveReverce;

        return *this;
    }

    //
    // ������� �������
    //

    void Reset ()
    {
        nActiveDirect = 0;
        nActiveReverce = 0;
        nReactiveDirect = 0;
        nReactiveReverce = 0;
        bValid = 0;
    }

    //
    // ��������� ������� � �����
    //

    bool SaveGuts (std::ostream &os) const
    {
        os << BOOL_TO_INT (bValid)
           << IO_DELIM << nActiveDirect   
           << IO_DELIM << nActiveReverce
           << IO_DELIM << nReactiveDirect
           << IO_DELIM << nReactiveReverce;

        return ! (! os);
    }

    //
    // �������������� ������� �� ������
    //

    bool RestoreGuts (std::istream &is)
    {
        int nVal = 0;
        is >> nVal;
        bValid = INT_TO_BOOL (nVal);
        __int64 nVal64;
        is >> nVal64;
        nActiveDirect = (int) nVal64;
        is >> nVal64;
        nActiveReverce = (int) nVal64;
        is >> nVal64;
        nReactiveDirect = (int) nVal64;
        is >> nVal64;
        nReactiveReverce = (int) nVal64;

        return ! (! is);
    }

    //
    // ������ ����, ��� ����������� ������� ����� ����.
    //

    bool IsEmpty () const 
    {
        if (! bValid) return true;
        return (0 == nActiveDirect) && (0 == nActiveReverce) &&
               (0 == nReactiveDirect) && 
               (0 == nReactiveReverce || 0xFFFFFFFF == nReactiveReverce);
    }

    //
    // �������������� ������ ������� �� ��������� ������
    //

    void ReadFromBuffer (unsigned char* buffer, 
                         Mercury::ProtocolType protocolType)
    {
        if (Mercury::ptSET == protocolType)
        {
            nActiveDirect    = IntFromBinary (buffer);
            nActiveReverce   = IntFromBinary (buffer + sizeof (int));
            nReactiveDirect  = IntFromBinary (buffer + 2 * sizeof (int));
            nReactiveReverce = IntFromBinary (buffer + 3 * sizeof (int));
        }
        else
        {
            nActiveDirect    = *(unsigned short*) (buffer);
            nActiveDirect    <<= 16;
            nActiveDirect    += *(unsigned short*) (buffer + 2);            

            nActiveReverce    = *(unsigned short*) (buffer + 4);
            nActiveReverce    <<= 16;
            nActiveReverce    += *(unsigned short*) (buffer + 6);

            if (0xFFFFFFFF == nActiveReverce) nActiveReverce = 0;

            nReactiveDirect    = *(unsigned short*) (buffer + 8);
            nReactiveDirect    <<= 16;
            nReactiveDirect    += *(unsigned short*) (buffer + 10);

            nReactiveReverce    = *(unsigned short*) (buffer + 12);
            nReactiveReverce    <<= 16;
            nReactiveReverce    += *(unsigned short*) (buffer + 14);
        }

        bValid = true;
    }

    //
    // ������������ ������� �� ����� ������������ ���������� (nCounterValue)
    // � ���*� (����*� )
    //

    static double CalculateEnergyValue (int nCounterValue, 
                                        double counterConst,
                                        int nKn,
                                        int nKt)
    {
        if (0 == counterConst) return 0.0;
        return nKn * nKt * (nCounterValue / (2 * counterConst));
    }

    int nActiveDirect;     // �������� ������
    int nActiveReverce;    // �������� ��������
    int nReactiveDirect;   // ���������� ������
    int nReactiveReverce;  // ���������� ��������
    bool bValid;           // ������� �� ������
};

class AllEnergyInfo
{
public:
    //
    // �����������.
    //

    AllEnergyInfo ()
    {
        Reset ();
    }

    //
    // ������� �������.
    //

    void Reset ()
    {
        SetValid (false);
        m_EnergyFromReset.Reset ();
        m_EnergyThisYear.Reset ();
        m_EnergyLastYear.Reset ();
        m_EnergyToday.Reset ();
        m_EnergyYesterday.Reset ();
        for (size_t nIdx = 0; nIdx < SIZE_OF (m_EnergyMonth); ++ nIdx)
        {
            m_EnergyMonth [nIdx].Reset ();
        }
    }

    //
    // ��������� ������� � �����
    //

    bool SaveGuts (std::ostream &os) const
    {
        os << BOOL_TO_INT (m_bValid) << std::endl;

        if (! m_EnergyFromReset.SaveGuts (os)) return false;
        os << std::endl;
        if (! m_EnergyThisYear.SaveGuts (os)) return false;
        os << std::endl;
        if (! m_EnergyLastYear.SaveGuts (os)) return false;
        os << std::endl;
        if (! m_EnergyToday.SaveGuts (os)) return false;
        os << std::endl;
        if (! m_EnergyYesterday.SaveGuts (os)) return false;
        os << std::endl;
        
        size_t nCount = SIZE_OF (m_EnergyMonth);
        os << nCount << std::endl;
        for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
        {
            if (! m_EnergyMonth [nIdx].SaveGuts (os)) return false;
            os << std::endl;
        }       

        return ! (! os);
    }

    //
    // �������������� ������� �� ������
    //

    bool RestoreGuts (std::istream &is)
    {
        int nVal = 0;
        is >> nVal;
        m_bValid = INT_TO_BOOL (nVal);

        if (! m_EnergyFromReset.RestoreGuts (is)) {Reset (); return false;}
        if (! m_EnergyThisYear.RestoreGuts (is)) {Reset (); return false;}
        if (! m_EnergyLastYear.RestoreGuts (is)) {Reset (); return false;}
        if (! m_EnergyToday.RestoreGuts (is)) {Reset (); return false;}
        if (! m_EnergyYesterday.RestoreGuts (is)) {Reset (); return false;}
        
        size_t nCount = SIZE_OF (m_EnergyMonth);
        is >> nVal;
        if (nCount != nVal)
        {
            Reset (); 
            return false;    
        }

        for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
        {
            if (! m_EnergyMonth [nIdx].RestoreGuts (is)) {Reset (); return false;}
        }

        return ! (! is);
    }


    //
    // ������ ���������� ������.
    //

    bool IsValid () const {return m_bValid;}
    void SetValid (bool bValid = true) {m_bValid = bValid;}

    //
    // ������ ����, ��� ����������� ������� ����� ����.
    //

    bool IsEmpty () const 
    {
        if (! IsValid ()) return true;
        if ((! m_EnergyFromReset.IsEmpty ()) ||
            (! m_EnergyThisYear.IsEmpty ()) ||
            (! m_EnergyLastYear.IsEmpty ()) ||
            (! m_EnergyToday.IsEmpty ()) ||
            (! m_EnergyYesterday.IsEmpty ()))
        {
            return false;
        }

        for (size_t nIdx = 0; nIdx < SIZE_OF (m_EnergyMonth); ++ nIdx)
        {
            if (! m_EnergyMonth [nIdx].IsEmpty ()) return false;
        }

        return true;
    }

    //
    // �������� � ����������� ������� � ������� ������.
    //

    const EnergyInfo& GetEnergyFromReset () const {return m_EnergyFromReset;}
    EnergyInfo& GetEnergyFromReset () {return m_EnergyFromReset;}

    //
    // �������� � ����������� ������� �� ������� ���.
    //

    const EnergyInfo& GetEnergyThisYear () const {return m_EnergyThisYear;}
    EnergyInfo& GetEnergyThisYear () {return m_EnergyThisYear;}

    //
    // �������� � ����������� ������� �� ������� ���.
    //

    const EnergyInfo& GetEnergyLastYear () const {return m_EnergyLastYear;}
    EnergyInfo& GetEnergyLastYear () {return m_EnergyLastYear;}

    //
    // �������� � ����������� ������� �� ������� �����.
    //

    const EnergyInfo& GetEnergyToday () const {return m_EnergyToday;}
    EnergyInfo& GetEnergyToday () {return m_EnergyToday;}

    //
    // �������� � ����������� ������� �� ������� �����.
    //

    const EnergyInfo& GetEnergyYesterday () const {return m_EnergyYesterday;}
    EnergyInfo& GetEnergyYesterday () {return m_EnergyYesterday;}

    //
    // �������� � ����������� ������� �� ������ ����� � ������� ����.
    // ������ �������� �� ���, ������� � �������� ������. ������� ����� - ������.
    // ������������ ����� - �������.
    //

    const EnergyInfo * GetEnergyMonth () const {return m_EnergyMonth;}
    EnergyInfo*  GetEnergyMonth () {return m_EnergyMonth;}   

    //
    // ������� ����, ��� ������ ������� �������.
    //

    bool m_bValid;
    
protected:

    //
    // �������� � ����������� �������.
    //

    EnergyInfo m_EnergyFromReset;
    EnergyInfo m_EnergyThisYear;
    EnergyInfo m_EnergyLastYear;
    EnergyInfo m_EnergyToday;
    EnergyInfo m_EnergyYesterday;
    EnergyInfo m_EnergyMonth [12];
};
#endif
