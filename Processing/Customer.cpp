///////////////////////////////////////////////////////////////////////////////
//
//  File:           Customer.cpp
//
//  Facility:       ��������� ���������
//
//
//  Abstract:       �����, ����������� ����������� ��������������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  12-05-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Customer.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/ManagerPPC/Processing
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/Processing
 * 
 * *****************  Version 5  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Processing
 * ��������� ���������� ������ �� ����
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/Processing
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing
 * ��������� Source Safe comments
* 
*/

#include "Customer.hpp"
#include "Utils.h"
#include <string>
#include "RegKeys.h"

//
// ����� Customer::Counter
//

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Counter
//
//  Description:
//
//  ����������� ��-���������
//
///////////////////////////////////////////////////////////////////////////////

Customer::Counter::Counter (): m_nSerialNumber (0), 
    m_nProtocolType (Mercury::ptMercury), 
    m_nKn (0), m_nKt (0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Counter
//
//  Description:
//
//  ����������� �����������
//
///////////////////////////////////////////////////////////////////////////////

Customer::Counter::Counter (const Counter& other)
{
    *this = other;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Reset
//
//  Description:
//
//  ������� �������
//
///////////////////////////////////////////////////////////////////////////////

void Customer::Counter::Reset ()
{
    m_nSerialNumber = 0;
    m_nKn = 0;
    m_nKt = 0;
    m_nProtocolType = Mercury::ptMercury;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator =
//
//  Description:
//
//  �������� �����������
//
///////////////////////////////////////////////////////////////////////////////

const Customer::Counter& Customer::Counter::operator = (const Counter& other)
{
    m_nSerialNumber = other.m_nSerialNumber;
    m_nKn = other.m_nKn;
    m_nKt = other.m_nKt;
    m_nProtocolType = other.m_nProtocolType;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator ==
//
//  Description:
//
//  �������� ==
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::Counter::operator == (const Counter& other) const
{
    return (m_nSerialNumber == other.m_nSerialNumber) && 
           (m_nProtocolType == other.m_nProtocolType) &&
           (m_nKn == other.m_nKn) && (m_nKt == other.m_nKt);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator <
//
//  Description:
//
//  �������� <
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::Counter::operator < (const Counter& other) const
{
    if (m_nSerialNumber != other.m_nSerialNumber) 
    {                      
        return m_nSerialNumber < other.m_nSerialNumber;
    }

    if (m_nProtocolType != other.m_nProtocolType) 
    {                      
        return m_nProtocolType < other.m_nProtocolType;
    }

    if (m_nKn != other.m_nKn) 
    {                      
        return m_nKn < other.m_nKn;
    }

    if (m_nKn != other.m_nKn) 
    {                      
        return m_nKn < other.m_nKn;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RestoreGuts
//
//  Description:
//
//  ������ ������� �� ������
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::Counter::RestoreGuts (std::istream &is, int /*nVersion*/)
{
    Reset ();
    int nVal;
    is >> nVal
       >> m_nSerialNumber
       >> m_nKn
       >> m_nKt;
    m_nProtocolType = (Mercury::ProtocolType) nVal;

    return ! (!is);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveGuts
//
//  Description:
//
//  ������ ������� � �����
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::Counter::SaveGuts (std::ostream &os) const
{
    int nVal = (int) m_nProtocolType;
    os << nVal << ' '
       << m_nSerialNumber << ' '
       << m_nKn << ' '
       << m_nKt;    

    return ! (!os);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetFileName
//
//  Description:
//
//  ���������� ��� ����� (��� ����������) � ����������� ������������� 
//  ��������.
//
///////////////////////////////////////////////////////////////////////////////

CString& Customer::Counter::GetFileName (CString &strBuffer) const
{
    strBuffer.Format (L"%08u_%d", 
                      m_nSerialNumber, 
                      m_nProtocolType);
    return strBuffer;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetLogFileName
//
//  Description:
//
//  ���������� ��� ��� �����.
//
///////////////////////////////////////////////////////////////////////////////

CString& Customer::Counter::GetLogFileName (CString &strBuffer) const
{
    GetFileName (strBuffer);
    strBuffer += L".txt.log";

    return strBuffer;
}      

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetTypeString
//
//  Description:
//
//  ���������� ������ � ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

const wchar_t* Customer::Counter::GetTypeString () const
{
    wchar_t *wcsType = L"�����������";
    switch (m_nProtocolType)
    {
    case Mercury::ptSET:
        wcsType = L"���";
        break;
    case Mercury::ptMercury:
        wcsType = L"��������";
        break;
    };

    return wcsType;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetShortDisplayName
//
//  Description:
//
//  ���������� �������� �������� � �������� �������.
//
///////////////////////////////////////////////////////////////////////////////

CString& Customer::Counter::GetShortDisplayName (CString &strBuffer) const
{
    CString strBaseName;
    GetFileName (strBaseName);
    strBuffer.Format (L"%s (%s)", (LPCTSTR) strBaseName, GetTypeString ());

    return strBuffer;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GetFullDisplayName
//
//  Description:
//
//  ���������� �������� �������� � ������� �������.
//
///////////////////////////////////////////////////////////////////////////////

CString& Customer::Counter::GetFullDisplayName (CString &strBuffer) const
{
    CString strBaseName;
    GetFileName (strBaseName);
    strBuffer.Format (L"%s %08u (���=%d, ���=%d)", 
                      GetTypeString (), 
                      m_nSerialNumber,
                      m_nKn, m_nKt);

    return strBuffer;
}

//
// ����� Customer
//

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Customer
//
//  Description:
//
//  ����������� ��-���������
//
///////////////////////////////////////////////////////////////////////////////

Customer::Customer (): m_fPowerLimit (0.0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Customer
//
//  Description:
//
//  ����������� �����������
//
///////////////////////////////////////////////////////////////////////////////

Customer::Customer (const Customer &other)
{
    *this = other;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~Customer
//
//  Description:
//
//  ����������
//
///////////////////////////////////////////////////////////////////////////////

Customer::~Customer ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Reset
//
//  Description:
//
//  ������� �������
//
///////////////////////////////////////////////////////////////////////////////

void Customer::Reset ()
{
    m_strName = L"";
    m_fPowerLimit = 0.0;
    m_Counters.clear ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator =
//
//  Description:
//
//  �������� ������������
//
///////////////////////////////////////////////////////////////////////////////

const Customer& Customer::operator = (const Customer &other)
{
    m_strName = other.m_strName;
    m_fPowerLimit = other.m_fPowerLimit;
    m_Counters.clear ();
    size_t nCount = other.m_Counters.size ();
    m_Counters.reserve (nCount);
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        m_Counters.push_back (other.m_Counters [nIdx]);    
    }

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator ==
//
//  Description:
//
//  �������� ==
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::operator == (const Customer &other) const
{
    return (m_strName == other.m_strName) && 
           (m_fPowerLimit == other.m_fPowerLimit);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator <
//
//  Description:
//
//  �������� <
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::operator < (const Customer &other) const
{
    int nResult = m_strName.CompareNoCase (other.m_strName);
    if (0 != nResult)
    {
        return nResult < 0;
    }
    
    return m_fPowerLimit < other.m_fPowerLimit;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RestoreGuts
//
//  Description:
//
//  �������������� ������� �� ������
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::RestoreGuts (std::istream &is)
{
    Reset ();
    int nVersion = 0;
    is >> nVersion;
    if (nVersion <= 0 || nVersion > m_nLatestVersion)
    {
        return false;
    }

    std::string strBuffer;
    std::getline (is, strBuffer);
    std::getline (is, strBuffer);
    
    MultiByte2WideChar (strBuffer.c_str (), m_strName, 1251);

    is >> m_fPowerLimit;
    size_t nCount = 0;
    is >> nCount;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        Counter counter;
        if ( ! counter.RestoreGuts (is, nVersion))
        {
            Reset ();

            return false;
        }

        if (bEvaluation && nIdx >= EVALUATION_MAX_COUNTERS)
        {
            continue;    
        }

        m_Counters.push_back (counter);
    }

    return ! (! is);    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveGuts
//
//  Description:
//
//  ���������� ������� � �����
//
///////////////////////////////////////////////////////////////////////////////

bool Customer::SaveGuts (std::ostream &os) const
{
    int nVersion = 0;
    os << m_nLatestVersion << std::endl;    

    std::string strBuffer;
    WideChar2MultiByte (m_strName, strBuffer, 1251);
    os << strBuffer.c_str () << '\n';

    os << m_fPowerLimit << '\n';
    size_t nCount = m_Counters.size ();
    os << nCount << '\n';
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        const Counter &counter = m_Counters [nIdx];
        if ( ! counter.SaveGuts (os))
        {
            return false;
        }

        os << '\n';
    }

    return ! (! os);    
}