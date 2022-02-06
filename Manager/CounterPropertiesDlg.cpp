///////////////////////////////////////////////////////////////////////////////
//
//  File:           CounterPropertiesDlg.cpp
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//                  
//
//
//  Abstract:       ������ ������� ��������.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  11-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: CounterPropertiesDlg.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#include "stdafx.h"
#include "Manager.h"
#include "CounterPropertiesDlg.h"

//
// CCounterPropertiesDlg dialog
//

IMPLEMENT_DYNAMIC(CCounterPropertiesDlg, CDialog)

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CCounterPropertiesDlg
//
//  Description:
//
//  �����������.
//
///////////////////////////////////////////////////////////////////////////////

CCounterPropertiesDlg::CCounterPropertiesDlg (CWnd* pParent /*=NULL*/, 
                                              const wchar_t *wcsCaption/* = L"�������� ��������"*/)
	: CDialog(CCounterPropertiesDlg::IDD, pParent)
    , m_nSerialNumber(0)
    , m_nKtn(0)
    , m_nKtt(0),
    m_strCaption (wcsCaption)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CCounterPropertiesDlg
//
//  Description:
//
//  ����������.
//
///////////////////////////////////////////////////////////////////////////////

CCounterPropertiesDlg::~CCounterPropertiesDlg ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DoDataExchange
//
//  Description:
//
//  ����� ������� � ��������.
//
///////////////////////////////////////////////////////////////////////////////

void CCounterPropertiesDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_KTN_SPIN, m_KtnSpin);
    DDX_Control(pDX, IDC_KTT_SPIN, m_KttSpin);
    DDX_Text(pDX, IDC_COUNTER_SERIAL, m_nSerialNumber);
    DDX_Text(pDX, IDC_KTN, m_nKtn);
    DDX_Text(pDX, IDC_KTT, m_nKtt);
    DDX_Control(pDX, IDC_TYPE, m_CounterType);
}


BEGIN_MESSAGE_MAP(CCounterPropertiesDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CCounterPropertiesDlg::OnBnClickedOk)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnInitDialog
//
//  Description:
//
//  ������������� �������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CCounterPropertiesDlg::OnInitDialog ()
{
    CDialog::OnInitDialog();
    SetWindowText (m_strCaption);
    SetIcon (AfxGetApp ()->LoadIcon (IDI_ICON6), TRUE);

    m_KtnSpin.SetRange32 (1, 0x7ffffff);
    m_KtnSpin.SetBuddy (GetDlgItem (IDC_KTN));
    m_KttSpin.SetRange32 (1, 0x7ffffff);
    m_KttSpin.SetBuddy (GetDlgItem (IDC_KTT));    

    m_nSerialNumber = m_Counter.m_nSerialNumber;
    m_nKtn = m_Counter.m_nKn;
    m_nKtt = m_Counter.m_nKt;

    m_CounterType.AddString (L"��������");
    m_CounterType.SetCurSel (0);

    UpdateData (FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CheckData
//
//  Description:
//
//  ���������, ������� �� ��������� ������������� ������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CCounterPropertiesDlg::CheckData ()
{
    UpdateData ();
    if (m_nKtn < 1)
    {
        MessageBox (L"��� ������ ���� ������ ����.");
        GetDlgItem (IDC_KTN)->SetFocus ();

        return FALSE;
    }
    if (m_nKtt < 1)
    {
        MessageBox (L"��� ������ ���� ������ ����.");
        GetDlgItem (IDC_KTT)->SetFocus ();

        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnBnClickedOk
//
//  Description:
//
//  ���������� ����� �� ������ OK.
//
///////////////////////////////////////////////////////////////////////////////

void CCounterPropertiesDlg::OnBnClickedOk ()
{
    if (CheckData ())
    {
        m_Counter.m_nSerialNumber = m_nSerialNumber;
        m_Counter.m_nKn = m_nKtn;
        m_Counter.m_nKt = m_nKtt;
        //����� ����� �������� ��� ��������� ���� ��������.!!!

        OnOK ();
    }
}
