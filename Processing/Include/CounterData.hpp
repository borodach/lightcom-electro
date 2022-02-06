///////////////////////////////////////////////////////////////////////////////
//
//  File:           CounterData.hpp
//
//  Facility:       ��������� ���������
//
//
//  Abstract:       �����, �������� �������� � ��������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  21-05-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CounterData.hpp $
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * ��������� Source Safe comments
* 
*/

#ifndef _COUNTER_DATA_HPP_
#define _COUNTER_DATA_HPP_

#include "Customer.hpp"
#include "MeteringData.hpp"
#include <vector>
#include <fstream>

class CounterData
{
public:

    enum DataState
    {
        dsInvalid = 0,      // ��������� ��������.
        dsOK,               // ������ �������.
        dsNotInited,        // ������ �� �������������������.
        dsNoFile,           // ��� ����� � �������.
        dsWrongFile,        // ���� ����, �� � ��� ������ ��� ������� ��������.
        dsIOError,          // ������ �����/������.
        dsVerifyFailed,     // �������� ��������� ������� ���������� �����������.
        dsUnexpectedError,  // ����������� ������
        dsMaxValue
    };

    //
    // ����������� �� ���������
    //

    CounterData ();

    //
    // ����������� �����������
    //

    CounterData (const CounterData &other);

    //
    // ����������
    //

    virtual ~CounterData ();

    //
    // ������� �������
    //

    void Reset ();
    //
    // �������� �����������
    //

    const CounterData& operator = (const CounterData &other);

    //
    // �������� ���������
    //

    bool operator == (const CounterData &other) const;
        
    //
    // ��������� ������.
    //

    DataState GetState () const {return m_State;}
    void SetState (DataState state) {m_State = state;}
    bool IsValid () const {return dsOK == m_State || dsVerifyFailed == m_State;}
    const wchar_t* GetStateText () const
    {
        return GetStateText (GetState ());
    }

    //
    // ��������� ���������� ������������ �������� �� �����, �������� ��������
    //

    static const wchar_t * GetStateText (DataState state);

    //
    //  ��������� ���������� ������������ �������� �� �����, �������� ��������
    //

    CounterData::DataState LoadMeteringDataFromFile (const wchar_t *wcsFileName);

    //
    // �������� ��������
    //

    const Customer::Counter& GetCounter () const {return m_Counter;}
    Customer::Counter& GetCounter () {return m_Counter;}
    void SetCounter (const Customer::Counter& counter) {m_Counter = counter;}

    //
    // ��������� ������������ ��������
    //

    const MeteringData& GetData () const {return m_Data;}
    MeteringData& GetData () {return m_Data;}
    void SetData (const MeteringData& data) {m_Data = data;}

protected:

    //
    // ��������� ������.
    //

    DataState m_State;

    //
    // �������� ��������
    //

    Customer::Counter m_Counter;

    //
    // ��������� ������������ ��������
    //

    MeteringData m_Data;    
};

//
// ������ ������ � ���������
//

typedef std::vector <CounterData> CounterDataList;

#endif //_COUNTER_DATA_HPP_