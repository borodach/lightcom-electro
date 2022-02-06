///////////////////////////////////////////////////////////////////////////////
//
//  File:           Customer.hpp
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
//  Creation Date:  10-05-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: Customer.hpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Processing/Include
 * ��������� Source Safe comments
* 
*/

#ifndef _CUSTOMER_
#define _CUSTOMER_

#include <vector>
#include <iostream>
#include <afxwin.h>

#include "Frame.hpp"

class Customer
{
public:

    //
    // �������� ��������
    //

    struct Counter
    {
        //
        // ����������� ��-���������
        //

        Counter ();

        //
        // ����������� �����������
        //

        Counter (const Counter& other);

        //
        // ������� �������
        //

        void Reset ();

        //
        // �������� �����������
        //

        const Counter& operator = (const Counter& other);

        //
        // �������� ==
        //

        bool operator == (const Counter& other) const;

        //
        // �������� <
        //

        bool operator < (const Counter& other) const;

        //
        // ������ ������� �� ������
        //

        bool RestoreGuts (std::istream &is, int nVersion);

        //
        // ������ ������� � �����
        //

        bool SaveGuts (std::ostream &os) const;

        //
        // ���������� ��� ����� (��� ����������) � ����������� ������������� 
        // ��������.
        //

        CString& GetFileName (CString &strBuffer) const;        

        //
        // ���������� ��� ��� �����.
        //

        CString& GetLogFileName (CString &strBuffer) const;

        //
        // ���������� ������ � ����� ��������.
        //

        const wchar_t* GetTypeString () const;
        
        //
        // ���������� �������� �������� � �������� �������.
        //

        CString& GetShortDisplayName (CString &strBuffer) const;
        
        //
        // ���������� �������� �������� � ������� �������.
        //

        CString& GetFullDisplayName (CString &strBuffer) const;
        
        //
        // �������� �����
        //

        unsigned m_nSerialNumber;

        //
        // ����������� ������������� �� ����������
        //

        int m_nKn;

        //
        // ����������� ������������� �� ����
        //

        int m_nKt;

        //
        // ��� ��������� ��������
        //

        Mercury::ProtocolType m_nProtocolType;
    };

    //
    // ������ �������� ���������
    //

    typedef std::vector <Counter> CounterList; 

public:

    //
    // ����������� ��-���������
    //

    Customer ();

    //
    // ����������� �����������
    //

    Customer (const Customer &other);

    //
    // ����������
    //

    virtual ~Customer ();

    //
    // ������� �������
    //

    void Reset ();

    //
    // �������� ������������
    //

    const Customer & operator = (const Customer &other);
    //
    // �������� ==
    //

    bool operator == (const Customer &other) const;

    //
    // �������� <
    //

    bool operator < (const Customer &other) const;

    //
    // �������������� ������� �� ������
    //

    bool RestoreGuts (std::istream &is);

    //
    // ���������� ������� � �����
    //

    bool SaveGuts (std::ostream &os) const;

    //
    // �������� �����������
    //

    const CString& GetName() const {return m_strName;}
    CString& GetName() {return m_strName;}
    void SetName (const CString &strName) {m_strName = strName;}
    CString& GetDisplayName (CString &strDisplayName) const
    {
        strDisplayName.Format (L"%s (%u ���)", 
                               (LPCWSTR) GetName (), 
                               (unsigned) GetPowerLimit ());
        return strDisplayName;
    }


    //
    // ����� ��������
    //

    double GetPowerLimit () const {return m_fPowerLimit;}
    void SetPowerLimit (double fPowerLimit) {m_fPowerLimit = fPowerLimit;}

    //
    // �������� �����������
    //

    const CounterList& Counters () const {return m_Counters;}
    CounterList& Counters () {return m_Counters;}

public:

    //
    // ����� ��������� ������ ������� �������� �������� � 
    //

    static const int m_nLatestVersion = 1;
protected:
    CString     m_strName;      // �������� �����������
    double      m_fPowerLimit;  // ����� ��������
    CounterList m_Counters;     // �������� �����������
};

//
// ������ ������������
//

typedef std::vector <Customer> CustomerList;

#endif
