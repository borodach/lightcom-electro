///////////////////////////////////////////////////////////////////////////////
//
//  File:           ScanOptionsDlg.cpp
//
//  Facility:       Desktop утилита для работы с результатами сканирования 
//                  счетчиков.
//                  
//
//
//  Abstract:       Диалог свойств сканирования счетчика.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  12-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: ScanOptionsDlg.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:16
 * Updated in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 21.01.07   Time: 15:54
 * Created in $/PPCMercury.root/ManagerPPC
 * Альфа версия
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
* 
*/

#include "stdafx.h"
#include "Manager.h"
#include "ScanOptionsDlg.h"
#include "Utils.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <strstream>

#define MERCURY128_FILE L"Mercury128K.txt"
#define SCAN_DEPTH_FILE L"ScanSettings.txt"
#define DEFAULT_SCAN_DEPTH 3

// CScanOptionsDlg dialog

IMPLEMENT_DYNAMIC(CScanOptionsDlg, CDialog)

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   CScanOptionsDlg
//                                                                             
//  Description:                                                             
//                                                                           
//  Конструктор.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

CScanOptionsDlg::CScanOptionsDlg (const CString &strFolder, 
                                  CWnd* pParent /*=NULL*/)
	: CDialog(CScanOptionsDlg::IDD, pParent),
    m_strFolder (strFolder)
    , m_strMercury64(_T(""))
{
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   CScanOptionsDlg
//                                                                             
//  Description:                                                             
//                                                                           
//  Деструктор.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

CScanOptionsDlg::~CScanOptionsDlg ()
{
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   DoDataExchange
//                                                                             
//  Description:                                                             
//                                                                           
//  Обмен данными с диалогом.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void CScanOptionsDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SCAN_DEPTH, m_nScatDepth);
    DDX_Control(pDX, IDC_SCAN_DEPTH_SPIN, m_ScanDepthSpin);
    DDX_Control(pDX, IDC_MERCURY64, m_Mercury64);
    DDX_Text(pDX, IDC_MERCURY64, m_strMercury64);
    DDX_Control(pDX, IDAPPLY, m_ApplyButton);
} 

BEGIN_MESSAGE_MAP(CScanOptionsDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CScanOptionsDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDAPPLY, &CScanOptionsDlg::OnBnClickedApply)
    ON_WM_SIZE()
    //ON_BN_CLICKED(IDRELOAD, &CScanOptionsDlg::OnBnClickedReload)
    ON_EN_CHANGE(IDC_MERCURY64, &CScanOptionsDlg::OnEnChangeMercury64)
    ON_EN_CHANGE(IDC_SCAN_DEPTH, &CScanOptionsDlg::OnEnChangeScanDepth)
    ON_WM_SIZE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   OnBnClickedOk
//                                                                             
//  Description:                                                             
//                                                                           
//  Обработчик клика на кнопку OK.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void CScanOptionsDlg::OnBnClickedOk ()
{                    
    UpdateData ();
    if (m_nScatDepth < 1)
    {
        MessageBox (L"Глубина сканирования должна быть больше 0.", NULL, MB_OK | MB_ICONERROR);
        GetDlgItem (IDC_SCAN_DEPTH)->SetFocus ();

        return;
    }

    if (! Save ())
    {
        MessageBox (L"Не удалось сохранить параметры сканирования.", NULL, MB_OK | MB_ICONERROR);
        return;
    }

    OnOK ();
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   OnBnClickedApply
//                                                                             
//  Description:                                                             
//                                                                           
//  Обработчик клика на кнопку Применить.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void CScanOptionsDlg::OnBnClickedApply ()
{
    UpdateData ();
    if (m_nScatDepth < 1)
    {
        MessageBox (L"Глубина сканирования должна быть больше 0.", NULL, MB_OK | MB_ICONERROR);
        GetDlgItem (IDC_SCAN_DEPTH)->SetFocus ();

        return;
    }

    if (! Save ())
    {
        MessageBox (L"Не удалось сохранить параметры сканирования.", NULL, MB_OK | MB_ICONERROR);
        return;
    }
    Load ();
    UpdateData (FALSE);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   OnInitDialog
//                                                                             
//  Description:                                                             
//                                                                           
//  Инициализация диалога.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

BOOL CScanOptionsDlg::OnInitDialog ()
{
    CDialog::OnInitDialog();
    m_ScanDepthSpin.SetRange32 (1, 0x7ffffff);
    m_ScanDepthSpin.SetBuddy (GetDlgItem (IDC_SCAN_DEPTH));
    Load ();
    UpdateData (FALSE);
    m_Mercury64.SetSel (-1, -1);
    m_Mercury64.SetFocus ();
    //m_Mercury64.SetEventMask (ENM_CHANGE);

    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/*
void CScanOptionsDlg::OnBnClickedReload ()
{
    Load ();
    UpdateData (FALSE);
}
*/

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   OnEnChangeMercury64
//                                                                             
//  Description:                                                             
//                                                                           
//  Обработчик изменения списка серийных номеров.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void CScanOptionsDlg::OnEnChangeMercury64 ()
{
    m_ApplyButton.EnableWindow (TRUE);    
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   OnEnChangeScanDepth
//                                                                             
//  Description:                                                             
//                                                                           
//  Обработчик изменения глубины сканирования
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void CScanOptionsDlg::OnEnChangeScanDepth ()
{
    m_ApplyButton.EnableWindow (TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   Load
//                                                                             
//  Description:                                                             
//                                                                           
//  Чтение параметров их крнфигурационных файлов.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

bool CScanOptionsDlg::Load ()
{
    m_nScatDepth = DEFAULT_SCAN_DEPTH;
    CString strFileName;
    strFileName = m_strFolder + SCAN_DEPTH_FILE;
    std::wifstream is (strFileName);
    unsigned nVal = 0;
    if (is.is_open ())
    {
        is >> nVal;
        if (nVal > 0)
        {
            m_nScatDepth = nVal;
        }
        is.close ();
    }

    m_strMercury64 = L""; 
    strFileName = m_strFolder + MERCURY128_FILE;
    std::wifstream mercury128KStream (strFileName);
    std::vector <unsigned> serialNumbers;
    if (mercury128KStream.is_open ())
    {
        while (! mercury128KStream.eof ())
        {
            unsigned nVal = 0;
            mercury128KStream >> nVal;
            if (! ! mercury128KStream)
            {
                serialNumbers.push_back (nVal);
            }
            else
            {
                mercury128KStream.clear ();
                mercury128KStream.get ();
            }            
        }
        mercury128KStream.close ();

        std::sort (serialNumbers.begin (), serialNumbers.end ());
        bool bFirst = true;
        size_t nCount = serialNumbers.size ();
        unsigned nVal = 0;
        for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
        {
            if (nIdx > 0 && nVal == serialNumbers [nIdx])
            {
                continue;
            }

            nVal = serialNumbers [nIdx];
            CString strBuffer;
            strBuffer.Format (L"%08u", nVal);
            if (! bFirst)
            {
                m_strMercury64 += L" "; 
            }
            else
            {
                bFirst = false;
            }
            m_strMercury64 += strBuffer;                 
        }

    }

    m_ApplyButton.EnableWindow (FALSE); 

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   Save
//                                                                             
//  Description:                                                             
//                                                                           
//  Запись параметров в конфигурационные файлы.
//                                                                           
///////////////////////////////////////////////////////////////////////////////

bool CScanOptionsDlg::Save ()
{
    CString strFileName;
    strFileName = m_strFolder + SCAN_DEPTH_FILE;
    std::wofstream os (strFileName);
    unsigned nVal = 0;
    if (! os.is_open ())
    {
        return false;
    }

    os << m_nScatDepth;
    os.close ();

    strFileName = m_strFolder + MERCURY128_FILE;
    std::wofstream mercury128KStream (strFileName);
    if (! mercury128KStream.is_open ())
    {
        return false;    
    }

    std::string strBuffer;
    WideChar2MultiByte (m_strMercury64, strBuffer);
    std::istrstream is (strBuffer.c_str (), (std::streamsize) strBuffer.length ());
    while (! is.eof ())
    {
        unsigned nVal = 0;
        is >> nVal;
        if (! ! is)
        {
            CStringA strBuffer;
            strBuffer.Format ("%08u\n", nVal);
            mercury128KStream << strBuffer;
        }
        else
        {
            is.clear ();
            is.get ();
        }            
    }

    mercury128KStream.close ();

    m_ApplyButton.EnableWindow (FALSE); 

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  Function:   OnSize
//                                                                             
//  Description:                                                             
//                                                                           
//  Обработчик изменения размера окна
//                                                                           
///////////////////////////////////////////////////////////////////////////////

void  CScanOptionsDlg::OnSize (UINT nType, int cx, int cy)
{
    int nOffset = 4;

    CWnd *pWnd = GetDlgItem (IDOK);
    if (NULL == pWnd)
    {
        return;
    }

    RECT rect;
    int nLastPosX;
    int nLastPosY;
    int nHeight; 
    int nWidth; 
    
    //
    // OK button
    //

    pWnd->GetWindowRect (&rect);
    ScreenToClient (&rect);
    nHeight = rect.bottom - rect.top; 
    nWidth = rect.right - rect.left; 
    
    nLastPosY = rect.top = cy - nHeight - nOffset;
    rect.left = nOffset;
    nLastPosX = rect.right = rect.left + nWidth;
    rect.bottom = rect.top + nHeight;

    pWnd->MoveWindow (&rect);

    //
    // Save button
    //

    pWnd = GetDlgItem (IDAPPLY);
    if (NULL == pWnd)
    {
        return;
    }

    pWnd->GetWindowRect (&rect);
    ScreenToClient (&rect);
    nWidth = rect.right - rect.left; 

    rect.top = nLastPosY;
    rect.left = nLastPosX + nOffset;
    nLastPosX = rect.right = rect.left + nWidth;
    rect.bottom = rect.top + nHeight;

    pWnd->MoveWindow (&rect);

    //
    // Cancel button
    //

    pWnd = GetDlgItem (IDCANCEL);
    if (NULL == pWnd)
    {
        return;
    }

    pWnd->GetWindowRect (&rect);
    ScreenToClient (&rect);
    nWidth = rect.right - rect.left; 

    rect.top = nLastPosY;
    rect.left = nLastPosX + nOffset;
    nLastPosX = rect.right = rect.left + nWidth;
    rect.bottom = rect.top + nHeight;

    pWnd->MoveWindow (&rect);

    //
    // Serial numbers
    //
    
    pWnd = GetDlgItem (IDC_MERCURY64);
    if (NULL == pWnd)
    {
        return;
    }

    pWnd->GetWindowRect (&rect);
    ScreenToClient (&rect);
    nWidth = rect.right - rect.left; 

    rect.bottom = nLastPosY - nOffset;
    rect.left = nOffset;
    rect.right = cx - nOffset;
    
    pWnd->MoveWindow (&rect);    
}