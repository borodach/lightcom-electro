///////////////////////////////////////////////////////////////////////////////
//
//  File:           ScanOptionsDlg.cpp
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//                  
//
//
//  Abstract:       ������ ������� ������������ ��������.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  12-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ScanOptionsDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#pragma once
#include "afxcmn.h"

//
// CScanOptionsDlg dialog
//

class CScanOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC (CScanOptionsDlg)

public:

    //
    // �����������.
    //

	CScanOptionsDlg (const CString &strFolder, CWnd* pParent = NULL);

    //                                                                           
    //  ����������.
    //

	virtual ~CScanOptionsDlg();

    //
    // ������������� �������.
    //

	enum { IDD = IDD_SCAN_OPTIONS };

    //                                                                           
    // ����� ������� � ��������.
    //

    virtual void DoDataExchange (CDataExchange* pDX);

    //
    // ������������� �������.
    //

    virtual BOOL OnInitDialog();

    //
    // ���������� ����� �� ������ OK.
    //

    afx_msg void OnBnClickedOk();

    //                                                                           
    // ���������� ����� �� ������ ���������.
    //

    afx_msg void OnBnClickedApply();

    //afx_msg void OnBnClickedReload();

    //                                                                           
    // ���������� ��������� ������ �������� �������.
    //

    afx_msg void OnEnChangeMercury64();

    //                                                                           
    // ���������� ��������� ������� ������������.
    //

    afx_msg void OnEnChangeScanDepth();

    //                                                                           
    // ������ ���������� � ���������������� �����.
    //

    bool Save ();

    //                                                                           
    // ������ ���������� �� ���������������� ������.
    //

    bool Load ();

    //
    // ����� ���������.
    //

	DECLARE_MESSAGE_MAP()

protected:

    //
    // ���� ��� ������� ������������.
    //

    CSpinButtonCtrl m_ScanDepthSpin;

    //
    // RichEdit ��� ������ �������� �������.
    //

    CRichEditCtrl m_Mercury64;

    //
    // ������ "���������".
    //

    CButton m_ApplyButton;

    //
    // ������� ������������.
    //

    UINT m_nScatDepth;

    //
    // ����� � ����������������� �������.
    //

    CString m_strFolder;

    //
    // ������ �������� ������� ���������, ������� 64K ������.
    //

    CString m_strMercury64;    
};
