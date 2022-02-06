///////////////////////////////////////////////////////////////////////////////
//
//  File:           MeteringData.cpp
//
//  Facility:       Хранение результатов сканирования счетчика
//
//
//  Abstract:       Класс для хранения результатов сканирования счетчика.
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  26-04-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MeteringData.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/Scanner/Common
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:24
 * Created in $/LightCom/C++/Electro/Scanner/Common
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:16
 * Created in $/LightCom/C++/Electro/ManagerPPC/Common
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/Common
 * Первая версия
* 
*/

#include <windows.h>
#include "MeteringData.hpp"
#include <fstream>
#include <strstream>
#include "RegKeys.h"

//
// Ключ по-умолчанию для формирования электронной подписи.
//

const unsigned char MeteringData::m_DefaultKey [16] = 
{
    0x8c, 0xce, 0x99, 0x55, 
    0xcc, 0xdb, 0x76, 0x5d, 
    0xe6, 0xe7, 0x85, 0xe6, 
    0x33, 0x1a, 0x21, 0xec
};

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       MeteringData
//
//  Description:
//
//  Конструктор по-умолчанию
//
///////////////////////////////////////////////////////////////////////////////

MeteringData::MeteringData ()
{
    Reset ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       MeteringData
//
//  Description:
//
//  Конструктор копирования
//
///////////////////////////////////////////////////////////////////////////////

MeteringData::MeteringData (const MeteringData &other)
{
    *this = other;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~MeteringData
//
//  Description:
//
//  Деструктор
//
///////////////////////////////////////////////////////////////////////////////

MeteringData::~MeteringData ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Reset
//
//  Description:
//
//  Очистка всех данных
//
///////////////////////////////////////////////////////////////////////////////

void MeteringData::Reset ()
{
    m_nVersion = m_nLatestVersion;
    m_ProtocolType = Mercury::ptInvalid;
    ResetNetAddress ();
    ResetCounterTime ();
    ResetLocation ();
    ResetKn ();
    ResetKt ();
    ResetSerialNumber ();
    ResetSoftwareVersion ();   
    ResetProductionType ();   
    ResetEnergy (); 
    m_PowerStorage.clear ();
    ResetEventLog ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator =
//
//  Description:
//
//  Оператор копирования
//
///////////////////////////////////////////////////////////////////////////////

const MeteringData& MeteringData::operator = (const MeteringData &other)
{
    std::strstream stream;    
    other.SaveGuts (stream);
    stream.seekg (0, std::ios::beg);
    RestoreGuts (stream);

    return *this;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveGuts
//
//  Description:
//
//  сохранёние объекта в поток
//
///////////////////////////////////////////////////////////////////////////////

bool MeteringData::SaveGuts (std::ostream &os) const
{
    //
    // Реальная версия объекта
    //

    os << m_nVersion << std::endl;

    //
    // Тип протокола
    //

    os << (int) m_ProtocolType << std::endl;

    //
    // Сетевой адрес счетчика
    //

    os << BOOL_TO_INT (m_bNetAddressValid) 
       << IO_DELIM 
       << (int) m_NetAddress 
       << std::endl;

    //
    // Текущее время счетчика
    //

    os << BOOL_TO_INT (m_bCounterTimeValid) 
       << IO_DELIM;
    if ( ! m_CounterTime.SaveGuts (os)) return false;
    os   << IO_DELIM 
       << BOOL_TO_INT (m_bIsWinter)
       << std::endl;

    //
    // Описание места расположения счетчика
    //


    os << BOOL_TO_INT (m_bLocationValid);

    size_t nCount = sizeof (m_szLocation);
    os << IO_DELIM << nCount;
    for (size_t nIdx = 0; nIdx < nCount; ++nIdx)
    {
        os << IO_DELIM 
           << (unsigned)  m_szLocation [nIdx];
    }
    os << std::endl;

    //
    // Коэффициенты трансформации по напряжению и по току
    //

    os << BOOL_TO_INT (m_bKnValid) 
       << IO_DELIM 
       << m_nKn
       << IO_DELIM 
       << BOOL_TO_INT (m_bKtValid)
       << IO_DELIM 
       << m_nKt
       << std::endl;
    
    //
    // Серийный номер счетчика
    //

    os << BOOL_TO_INT (m_bSerialNumberValid) 
       << IO_DELIM 
       << m_nSerialNumber
       << IO_DELIM;
    if ( ! m_ProductionDate.SaveGuts (os)) return false;
    os << std::endl;

    //
    // Версия програмного обеспечения счетчика
    //

    os << BOOL_TO_INT (m_bSoftwareVersionValid) 
       << IO_DELIM 
       << m_szSoftwareVersion
       << std::endl;

    //
    // Вариант исполнения счетчика
    //

    os << BOOL_TO_INT (m_bProductionTypeValid) << std::endl; 
    if ( ! m_ProductionType.SaveGuts (os))
    {
        return false;
    }

    os << std::endl;

    //
    // Массив сведений о накопленной энергии.
    // Нулевой индекс - суммарные сведения по всем тарифам. Первый инлекс - 
    // сведения по первому тарифу, второй - по второму ...
    //

    nCount = SIZE_OF (m_Energy);
    os << nCount << std::endl;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        if (! m_Energy [nIdx].SaveGuts (os))
        {
            return false;
        }
        os << std::endl;
    }

    //
    // Журнал событий. Индекс в массиве m_EventLog четко соответствует значению 
    // Mercury::ReadLogEntry::LogEntryType
    //

    nCount = SIZE_OF (m_EventLog);
    os << nCount << std::endl;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        size_t nCount1 = SIZE_OF (m_EventLog [0]);
        os << nCount1 << std::endl;
        for (size_t nIdx1 = 0; nIdx1 < nCount1; ++ nIdx1)
        {
            if ( ! m_EventLog [nIdx] [nIdx1].SaveGuts (os))
            {
                return false;
            }
            os << std::endl;
        }
    }

    //
    // Профили средних мощностей.
    //

    nCount = m_PowerStorage.size ();
    os << nCount << std::endl;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        if (! m_PowerStorage [nIdx].SaveGuts (os))
        {
            return false;
        }
        os << std::endl;
    }

    return ! (! os);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveGuts
//
//  Description:
//
//  сохранёние объекта в подписанный файл
//
///////////////////////////////////////////////////////////////////////////////

bool MeteringData::SaveGuts (const wchar_t *wcsFileName, 
                             const void *key/* = m_DefaultKey*/,
                             size_t keySize/* = sizeof (m_DefaultKey)*/) const
{
    bool bResult;
    std::ofstream os (wcsFileName, std::ios::trunc);
    if (! os.is_open ()) 
    {
        return false;
    }
    bResult = SaveGuts (os);
    os.close ();
    if (! bResult) 
    {
        DeleteFile (wcsFileName);

        return false;
    }

    bResult = SignFile (wcsFileName, key, keySize);
    if (! bResult) 
    {
        DeleteFile (wcsFileName);

        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RestoreGuts
//
//  Description:
//
//  Восстановление объекта из потока
//
///////////////////////////////////////////////////////////////////////////////

bool MeteringData::RestoreGuts (std::istream &is)
{
    int nVal = 0;
    Reset ();

    //
    // Реальная версия объекта
    //

    is >> m_nVersion;
    if (m_nVersion > m_nLatestVersion)
    {
        Reset ();
        return false;
    }

    //
    // Тип протокола
    //

    is >> nVal;
    m_ProtocolType = (Mercury::ProtocolType) nVal;

    //
    // Сетевой адрес счетчика
    //

    is >> nVal;
    m_bNetAddressValid = INT_TO_BOOL (nVal);
    is >> nVal;
    m_NetAddress = (Mercury::Frame::NetAddress) nVal;

    //
    // Текущее время счетчика
    //

    is >> nVal;
    m_bCounterTimeValid = INT_TO_BOOL (nVal);
    m_CounterTime.RestoreGuts (is);
    is >> nVal;
    m_bIsWinter = INT_TO_BOOL (nVal);


    if (bEvaluation)
    {
        wchar_t buf [64];
        wsprintf (buf, L"%04u.%02u.%02u", m_CounterTime.GetYear (), m_CounterTime.GetMonth (), m_CounterTime.GetDay ());
        if (wcscmp (buf, EVALUATION_MAX_DATE) > 0)
        {
            Reset ();
            return false;
        }
    }


    //
    // Описание места расположения счетчика
    //

    is >> nVal;
    m_bLocationValid = INT_TO_BOOL (nVal);
    size_t nCount;
    is >> nCount;
    if (nCount != sizeof (m_szLocation)) {Reset (); return false;}
    for (size_t nIdx = 0; nIdx < nCount; ++nIdx)
    {
        unsigned nByte;
        is >> nByte;
        m_szLocation [nIdx] = nByte;
    }
    
    //
    // Коэффициенты трансформации по напряжению и по току
    //

    is >> nVal;
    m_bKnValid = INT_TO_BOOL (nVal);
    is >> m_nKn;

    is >> nVal;
    m_bKtValid = INT_TO_BOOL (nVal);
    is >> m_nKt;
    
    //
    // Серийный номер счетчика
    //

    is >> nVal;
    m_bSerialNumberValid = INT_TO_BOOL (nVal);
    is >> m_nSerialNumber;
    if ( !m_ProductionDate.RestoreGuts (is)) {Reset (); return false;}

    //
    // Версия програмного обеспечения счетчика
    //

    is >> nVal;
    m_bSoftwareVersionValid = INT_TO_BOOL (nVal);
    is >> m_szSoftwareVersion;

    is >> nVal;
    m_bProductionTypeValid = INT_TO_BOOL (nVal);
    if (! m_ProductionType.RestoreGuts (is))
    {
        Reset ();
        return false;
    }

    //
    // Массив сведений о накопленной энергии.
    // Нулевой индекс - суммарные сведения по всем тарифам. Первый инлекс - 
    // сведения по первому тарифу, второй - по второму ...
    //

    nCount = SIZE_OF (m_Energy);
    is >> nVal;
    if (nVal != nCount)
    {
        Reset ();
        return false;
    }

    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        if (! m_Energy [nIdx].RestoreGuts (is))
        {
            return false;
        }
    }

    //
    // Журнал событий. Индекс в массиве m_EventLog четко соответствует значению 
    // Mercury::ReadLogEntry::LogEntryType
    //

    nCount = SIZE_OF (m_EventLog);

    is >> nVal;
    if (nVal != nCount)
    {
        Reset ();
        return false;
    }

    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        size_t nCount1 = SIZE_OF (m_EventLog [0]);
        is >> nVal;
        if (nVal != nCount1)
        {
            Reset ();
            return false;
        }

        for (size_t nIdx1 = 0; nIdx1 < nCount1; ++ nIdx1)
        {
            if ( ! m_EventLog [nIdx] [nIdx1].RestoreGuts (is))
            {
                return false;
            }
        }
    }

    //
    // Профили средних мощностей.
    //

    is >> nCount;
    PowerInfo dummy;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        m_PowerStorage.push_back (dummy);
        if (! m_PowerStorage [nIdx].RestoreGuts (is))
        {
            return false;
        }
    }

    return ! (! is);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RestoreGuts
//
//  Description:
//
//  Восстановление объекта из подписанного файла
//
///////////////////////////////////////////////////////////////////////////////

bool MeteringData::RestoreGuts (const wchar_t *wcsFileName, 
                                const void *key/* = m_DefaultKey*/,
                                size_t keySize/* = sizeof (m_DefaultKey)*/)
{
    bool bResult;
    bResult = VerifySignedFile (wcsFileName, key, keySize);
    if (! bResult)
    {
        Reset ();

        return false;
    }

    std::ifstream is (wcsFileName);
    if (! is.is_open ()) 
    {
        Reset ();

        return false;
    }

    bResult = RestoreGuts (is);

    return bResult;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       operator << 
//                  operator >>       
//
//  Description:
//
//  Операторы потокового I/O
//
///////////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& os, const MeteringData &val)
{
    val.SaveGuts (os);
    return os;
}

std::istream& operator >> (std::istream& is, MeteringData &val)
{
    val.RestoreGuts (is);
    return is;
}

/*
std::ostream& operator << (std::ostream& os, const MeteringData::DateTime &val)
{
    val.SaveGuts (os);
    return os;
}

std::istream& operator >> (std::istream& is, MeteringData::DateTime &val)
{
    val.RestoreGuts (is);
    return is;
}
*/