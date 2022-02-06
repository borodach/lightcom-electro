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
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 21.01.07   Time: 15:54
 * Created in $/PPCMercury.root/ManagerPPC
 * ����� ������
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
    // ���������� ��������� ������� ����
    //

    afx_msg void OnSize(UINT nType, int cx, int cy);

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

    CEdit m_Mercury64;

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
