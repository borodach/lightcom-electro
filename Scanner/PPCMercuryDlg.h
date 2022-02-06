///////////////////////////////////////////////////////////////////////////////
//
//  File:           PPCMercuryDlg.h
//
//  Facility:       ЛайтКом Электро Сканер
//
//
//  Abstract:       Главный диалог
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  23-11-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: PPCMercuryDlg.h $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:16
 * Updated in $/LightCom/C++/Electro/Scanner/Scanner
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 5.03.07    Time: 6:16
 * Created in $/LightCom/C++/Electro/Scanner/Scanner
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/PPCMercury/PPCMercury
 * 
 * *****************  Version 8  *****************
 * User: Sergey       Date: 3.03.07    Time: 23:07
 * Updated in $/PPCMercury.root/PPCMercury
 * 
 * *****************  Version 7  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:07
 * Updated in $/PPCMercury.root/PPCMercury
* 
*/

#pragma once

#define APP_KEY_NAME        L"Лайтком"
#define APP_VERSION         L"1.1.1"
#define APP_VENDOR          L"Лайтком Software"
#define APP_RESULTS_FOLDER  L"\\Results"
#define APP_RESULTS_EXT     L"txt"

#include "SIRTransport.hpp"
#include "MeteringData.hpp"
#include "Frame.hpp"
#include "afxwin.h"

// CPPCMercuryDlg dialog
class CPPCMercuryDlg : public CDialog
{
// Construction
public:
	CPPCMercuryDlg(CWnd* pParent = NULL);	// standard constructor
    ~CPPCMercuryDlg ()
    {
        Stop (TRUE);
    }

    void Scan ();
    void ClearLog ();
    void AddToLog (const wchar_t *wcsMessage);
    void FlushLog ();

    //
    // Публикует в реестре информацию о приложении
    //

    void PublishAppInfo ();

// Dialog Data
	enum { IDD = IDD_PPCMERCURY_DIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    
    void ShowScanResults ();
    void ShowEnergyInfo (const EnergyInfo &ei, 
                         const wchar_t *wcsName,
                         Mercury::ProtocolType protocolType,
                         double counterConst,
                         unsigned short nKn,
                         unsigned short nKt);    

    void ShowAllEnergyInfo (const AllEnergyInfo &allEnergyInfo, 
                            const wchar_t *wcsName,
                            Mercury::ProtocolType protocolType,
                            double counterConst,
                            unsigned short nKn,
                            unsigned short nKt);

    /*
    void ShowPowerInfo (const PowerStorage &powerInfo, 
                        const wchar_t *wcsName,
                        const wchar_t *wcsUnit);
    */

    void Start ();
    void Stop (BOOL bDestruction = FALSE);

    //
    // Сохранение лога в файл
    //

    void SaveLog (wchar_t *wcsFileName);

// Implementation
protected:
	HICON m_hIcon;

    Transport::SIRTransport transport;
    MeteringData scanResult;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
public:
    CEdit m_OutputBox;
public:
    CButton m_ScanButton;
public:
    afx_msg void OnBnClickedCopy ();
    afx_msg LRESULT UserMessage (WPARAM wParam, LPARAM lParam);

public:
    BOOL m_bPowerScanEnabled;
    HANDLE m_hThread;
    CString m_strLog;
    CString m_strTitle;

public:
    CButton m_CopyButton;
public:
    CButton m_PowerButton;
public:
    afx_msg void OnAbout();
public:
    CButton m_AboutBtn;
};
