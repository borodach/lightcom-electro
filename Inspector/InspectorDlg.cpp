///////////////////////////////////////////////////////////////////////////////
//
//  File:           InspectorDlg.cpp
//
//  Facility:       ЛайтКом Электро Инспектор
//
//
//  Abstract:       Главный диалог приложения
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
* $History: InspectorDlg.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/Inspector/Inspector
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:16
 * Updated in $/LightCom/C++/Electro/Inspector/Inspector
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/Inspector/Inspector
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 3.03.07    Time: 23:01
 * Updated in $/PPCMercury.root/Inspector
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 28.01.07   Time: 23:11
 * Updated in $/PPCMercury.root/Inspector
 * Внесены изменения, в соотетствии с замечаниями ЭнергоСбыта
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 6:14
 * Updated in $/PPCMercury.root/Inspector
 * Добавлены Source Safe comments
* 
*/

#include "HwIdProtection.h"
#include "DataManager.hpp"
#include "RegKeys.h"
#include <fstream>

#include "stdafx.h"
#include "Inspector.h"
#include "InspectorDlg.h"
#include "AboutDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CInspectorDlg dialog

CInspectorDlg::CInspectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInspectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInspectorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CUSTOMER, m_CustomersCombo);
    DDX_Control(pDX, IDC_REPORT, m_OutputBox);
    DDX_Control(pDX, IDC_COMMAND, m_CommandsBox);
}

BEGIN_MESSAGE_MAP(CInspectorDlg, CDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_CUSTOMER, &CInspectorDlg::OnCustomerChanged)
    ON_CBN_SELCHANGE(IDC_COMMAND, &CInspectorDlg::OnCommandChange)
END_MESSAGE_MAP()

bool bEvaluation = true;

// CInspectorDlg message handlers

BOOL CInspectorDlg::OnInitDialog()
{
    if (! Protect (ELECTRO_INSPECTOR_KEY,
                   L"\\Keys\\InspectorKey.txt", 
                   L"\\HardwareId", 
                   L"\\HwId.txt"))
    {
        //EndDialog (0);
        //return FALSE;
        bEvaluation = true;
    }
    else
    {
        bEvaluation = false;
    }
    

    CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    m_OutputBox.SetLimitText (64 * 1024);
    
    //
    // Инициализация списка команд
    //

    m_CommandsBox.SetItemData (m_CommandsBox.AddString (L"<ПЕРЕЧИТАТЬ>"), (DWORD_PTR) ctReload);
    m_CommandsBox.SetItemData (m_CommandsBox.AddString (L"<СКАНЕР>"), (DWORD_PTR) ctScan);
    m_CommandsBox.SetItemData (m_nLastCommandIdx = m_CommandsBox.AddString (L"ОБЩИЙ"), (DWORD_PTR) ctSummary);
    m_CommandsBox.SetItemData (m_CommandsBox.AddString (L"ЭНЕРГИЯ"), (DWORD_PTR) ctEnergy);
    m_CommandsBox.SetItemData (m_CommandsBox.AddString (L"МОЩНОСТЬ"), (DWORD_PTR) ctPower);
    m_CommandsBox.SetItemData (m_CommandsBox.AddString (L"О ПРОГРАММЕ"), (DWORD_PTR) ctAbout);
    m_CommandsBox.SetCurSel (m_nLastCommandIdx);

    m_CommandsBox.EnableWindow (FALSE);
    
    //ReloadCustomerList ();
    m_CustomersCombo.AddString (L"<ПЕРЕЧИТАТЬ>");
    if (m_DataManager.LoadCustomers ())
    {
        const CustomerList &customers =  m_DataManager.GetCustomers ();
        size_t nCount = customers.size ();
        for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
        {
            m_CustomersCombo.AddString (customers [nIdx].GetName ());
        }
    }
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInspectorDlg::OnSize(UINT nType, int cx, int cy)
{
    int nOffset = 1;
    RECT commandsRect;
    m_CommandsBox.GetWindowRect (&commandsRect);
    ScreenToClient (&commandsRect);
    commandsRect.top = nOffset;
    commandsRect.bottom = cy - nOffset;
    commandsRect.left = cx - (commandsRect.right - commandsRect.left);
    commandsRect.right = cx - nOffset;
	m_CommandsBox.MoveWindow (&commandsRect);

    RECT customersRect;
    m_CustomersCombo.GetWindowRect (&customersRect);
    ScreenToClient (&customersRect);
    customersRect.top = commandsRect.top;
    customersRect.bottom = commandsRect.bottom;
    customersRect.left = nOffset;
    customersRect.right = commandsRect.left - nOffset;
	m_CustomersCombo.MoveWindow (&customersRect);

    RECT outputRect;    
    m_OutputBox.GetWindowRect (&outputRect);
    ScreenToClient (&outputRect);

    //COMBOBOXINFO cbInfo;
    //cbInfo.cbSize = sizeof (cbInfo);
    //m_CustomersCombo.GetComboBoxInfo (&cbInfo);
    outputRect.top = 21 + nOffset;
    outputRect.left = customersRect.left;
    outputRect.right = commandsRect.right;
    outputRect.bottom = cy - nOffset;
	m_OutputBox.MoveWindow (&outputRect);
} 


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ReloadCustomerList
//
//  Description:
//
//  Заполнение списка потребителей
//
///////////////////////////////////////////////////////////////////////////////

void CInspectorDlg::ReloadCustomerList ()
{
    m_CustomersCombo.ResetContent ();
    m_CustomersCombo.AddString (L"<ПЕРЕЧИТАТЬ>");

    if (! m_DataManager.LoadCustomers ())
    {
        PocessCustomerChange ();
        return;
    }

    const CustomerList &customers =  m_DataManager.GetCustomers ();
    size_t nCount = customers.size ();
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        m_CustomersCombo.AddString (customers [nIdx].GetName ());
    }

    if (m_DataManager.GetCurrentCustomerIdx () != m_DataManager.m_nInvalidIdx)
    {
        m_CustomersCombo.SetCurSel (m_DataManager.GetCurrentCustomerIdx () + 1);
    }

    PocessCustomerChange ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCustomerChanged
//
//  Description:
//
//  Обработчик изменения текущего потребителя
//
///////////////////////////////////////////////////////////////////////////////

void CInspectorDlg::OnCustomerChanged ()
{
    UpdateWindow ();
    int nIndex = m_CustomersCombo.GetCurSel ();
    if (nIndex < 0) 
    {
        PocessCustomerChange ();
        return;
    }
    if (0 == nIndex)
    {
        ReloadCustomerList ();    
    }
    else
    {
        CString strText = "ПОЖАЛУЙСТА ПОДОЖДИТЕ.\nВЫПОЛНЯЕТСЯ ЗАГРУЗКА ДАННЫХ\nСЧЁТЧИКОВ - ЭТО МОЖЕТ ЗАНЯТЬ\nНЕСКОЛЬКО МИНУТ.";
        static wchar_t newLine [] = {13, 13, 10, 0};
        strText.Replace (L"\n", newLine);

        m_OutputBox.SetWindowText (strText);
        m_DataManager.SetCurrentCustomerIdx (nIndex - 1);
        PocessCustomerChange ();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       PocessCustomerChange
//
//  Description:
//
//  Обработчик изменения текущего потребителя
//
///////////////////////////////////////////////////////////////////////////////

void CInspectorDlg::PocessCustomerChange ()
{
    size_t nCustomerIdx = m_DataManager.GetCurrentCustomerIdx ();
    m_CommandsBox.EnableWindow (nCustomerIdx != m_DataManager.m_nInvalidIdx);
    if (nCustomerIdx != m_DataManager.m_nInvalidIdx)
    {
        m_CommandsBox.SetCurSel (m_nLastCommandIdx);
    }
    OnCommandChange ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCommandChange
//
//  Description:
//
//  Обработчик новой команды пользователя
//
///////////////////////////////////////////////////////////////////////////////

void CInspectorDlg::OnCommandChange ()
{
    int nCommandIdx = m_CommandsBox.GetCurSel ();
    if (nCommandIdx < 0)
    {
        UpdateReport (L"");
        return;
    }

    CommandType cmdType = (CommandType) m_CommandsBox.GetItemData (nCommandIdx);
    bool bChangeLastCommand = true;
    switch (cmdType)
    {
    case ctReload:
        m_DataManager.SetCurrentCustomerIdx (m_DataManager.GetCurrentCustomerIdx ());
        bChangeLastCommand = false;
        break;
    case ctSummary:
        UpdateReport (m_DataManager.SummaryReportText ());
        break;
    case ctEnergy:
        UpdateReport (m_DataManager.EnergyReportText ());
        break;
    case ctPower:
        UpdateReport (m_DataManager.PowerReportText ());
        break;
    case ctScan:
        {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            memset (&si, 0, sizeof (si));
            si.cb = sizeof (si);

            wchar_t wcsPath [1024];
            GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
            wcscat (wcsPath, L"\\Scanner.exe");

            if (CreateProcess (wcsPath,
                               NULL,
                               NULL, 
                               NULL, 
                               FALSE, 
                               0,
                               NULL,
                               NULL,
                               &si,
                               &pi))
            {
                CloseHandle (pi.hThread);
                CloseHandle (pi.hProcess);
            }
            else
            {
                DWORD dwError = GetLastError ();
                MessageBox (L"Не удалось запустить программу-сканер.", L"Ошибка", MB_OK | MB_ICONERROR);
            }
        }

        bChangeLastCommand = false;
        break;
    case ctAbout:
        CAboutDlg dlg (this);
        dlg.DoModal ();
        break;
    };

    if (bChangeLastCommand)
    {
        m_nLastCommandIdx = nCommandIdx;
    }
    else
    {
        m_CommandsBox.SetCurSel (m_nLastCommandIdx);
        OnCommandChange ();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       UpdateReport
//
//  Description:
//
//  Обновление отчета
//
///////////////////////////////////////////////////////////////////////////////

void CInspectorDlg::UpdateReport (const wchar_t *wcsMessage)
{
    if (! IsWindow (m_OutputBox.m_hWnd))
    {
        return;
    }

    //static wchar_t newLine [] = {13, 13, 10, 0};
    //CString strMessage (wcsMessage);
    //strMessage.Replace (L"\n", newLine);
    m_OutputBox.SetWindowText (wcsMessage);
    m_OutputBox.LineScroll (0);
}
