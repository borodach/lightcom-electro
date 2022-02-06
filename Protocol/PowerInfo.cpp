///////////////////////////////////////////////////////////////////////////////
//
//  File:           PowerInfo.�pp
//
//  Facility:       �������� ��������
//
//
//  Abstract:       ������ ��� �������� ������������ ��������� �� �������.
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  30-04-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: PowerInfo.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/Protocol
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Protocol
 * ������ ������
* 
*/

#include "PowerInfo.hpp"

/*
///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   MaxPowerStorage
//                                                                           
//  Description:                                                             
//                                                                           
//  �����������
//                                                                           
///////////////////////////////////////////////////////////////////////////////

MaxPowerStorage::MaxPowerStorage ()
{
    Reset ();
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   ~MaxPowerStorage                                                  
//                                                                           
//  Description:                                                             
//                                                                           
//  ����������
//                                                                           
///////////////////////////////////////////////////////////////////////////////

MaxPowerStorage::~MaxPowerStorage ()
{
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   Reset
//                                                                           
//  Description:                                                             
//                                                                           
//  ������� �������
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void MaxPowerStorage::Reset ()
{
    m_ActiveDirect.clear ();
    m_ActiveReverce.clear ();
    m_ReactiveDirect.clear ();
    m_ReactiveReverce.clear ();
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   AddPowerInfo
//                                                                           
//  Description:                                                             
//                                                                           
//  ���������� �������� � ��������
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void MaxPowerStorage::AddPowerInfo (unsigned char *pData, 
                                    double counterConst,
                                    unsigned nKn = 1,
                                    unsigned nKt = 1)
{
    if (0.0 == counterConst) return;
    unsigned short nActiveDirect = pData [7] + 256 * (unsigned short) pData [8];
    unsigned short nActiveReverce = pData [9] + 256 * (unsigned short) pData [10];
    unsigned short nReactiveDirect = pData [11] + 256 * (unsigned short) pData [12];
    unsigned short nReactiveReverce = pData [13] + 256 * (unsigned short) pData [14];
    if (0xFFFF == nActiveReverce) nActiveReverce = 0;
    if (0xFFFF == nReactiveReverce) nReactiveReverce = 0;

    if (0 == nActiveDirect && 0 == nActiveReverce &&
        0 == nReactiveDirect && 0 == nReactiveReverce)
    {
        return;
    }

    unsigned char nT = pData [6];
    PowerInfo powerInfo;
    powerInfo.nHour   = IntFromBCD (pData [1]);
    powerInfo.nMinute = IntFromBCD (pData [2]);
    powerInfo.nDay    = IntFromBCD (pData [3]);
    powerInfo.nMonth  = IntFromBCD (pData [4]);
    powerInfo.nYear  =  2000 + IntFromBCD (pData [5]);
    double k = (nKn * nKt * 60.0) / (2 * counterConst * nT);

    powerInfo.fPowerValue =  k * nActiveDirect;
    AddPowerInfo (powerInfo, m_ActiveDirect);

    powerInfo.fPowerValue =  k * nActiveReverce;
    AddPowerInfo (powerInfo, m_ActiveReverce);

    powerInfo.fPowerValue =  k * nReactiveDirect;
    AddPowerInfo (powerInfo, m_ReactiveDirect);

    powerInfo.fPowerValue =  k * nReactiveReverce;
    AddPowerInfo (powerInfo, m_ReactiveReverce);    
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   AddPowerInfo
//                                                                           
//  Description:                                                             
//                                                                           
//  ���������� �������� � �������� � �������� ���������
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void MaxPowerStorage::AddPowerInfo (const PowerInfo &powerInfo, 
                                    MaxPowerPerMonth &powerStorage)
{
    size_t nCount = powerStorage.size ();
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {                 
        PowerInfo &storedItem = powerStorage [nIdx];
        if (storedItem == powerInfo)
        { 
            if (storedItem.fPowerValue < powerInfo.fPowerValue)
            {
                storedItem = powerInfo;
            }

            return;
        }
    }
    powerStorage.push_back (powerInfo);
}                                      
*/