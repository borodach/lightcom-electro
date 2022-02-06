///////////////////////////////////////////////////////////////////////////////
//
//  File:           PPCChecker.cpp
//
//  Facility:       ������� ������� ������
//
//
//  Abstract:       ������� ������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  23-11-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
//////////////////////////////////////////////////////////////////////////////

/*
* $History: PPCMercuryDlg.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Sergey       Date: 11.04.07   Time: 22:34
 * Updated in $/LightCom/C++/Electro/Scanner/Scanner
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 10.03.07   Time: 9:15
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
 * *****************  Version 20  *****************
 * User: Sergey       Date: 3.03.07    Time: 23:07
 * Updated in $/PPCMercury.root/PPCMercury
 * 
 * *****************  Version 19  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/PPCMercury
 * 
 * *****************  Version 18  *****************
 * User: Sergey       Date: 23.11.06   Time: 1:15
 * Updated in $/PPCMercury.root/PPCMercury
 * ������ ������
* 
*/

#include "SIRTransport.hpp"
#include "Scanner.hpp"
#include "REadLogEntry.hpp"
#include "HwIdProtection.h"
#include "RegKeys.h"
#include <fstream>

#include "stdafx.h"
#include "PPCMercury.h"
#include "PPCMercuryDlg.h"
#include "AboutDlg.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
// ������������� �������, ������������ "���������" ����������
//

#define WAKEUP_TIMER_ID 1979

//
// �������� (��.) ������������ �������, ������������ "���������" ����������
//

#define WAKEUP_TIMER_INTERVAL 1000


#define MAX_DISPLAY_CHAR_COUNT 30000

//
// ������� �����������
//

bool bEvaluation = true;

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       WakeUpTimerProc
//
//  Description:
//
//  ���������� ������� �������. ��������� "���������" ����������.
//
///////////////////////////////////////////////////////////////////////////////

void CALLBACK WakeUpTimerProc (HWND hwnd, 
                               UINT uMsg, 
                               UINT idEvent, 
                               DWORD dwTime)
{
    SystemIdleTimerReset ();
}
// CPPCMercuryDlg dialog

CPPCMercuryDlg::CPPCMercuryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPPCMercuryDlg::IDD, pParent)
    , m_bPowerScanEnabled(FALSE), m_hThread (NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPPCMercuryDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_OUTPUT, m_OutputBox);
    DDX_Control(pDX, IDC_SCAN, m_ScanButton);
    DDX_Check(pDX, IDC_READ_POWER, m_bPowerScanEnabled);
    DDX_Control(pDX, IDC_COPY, m_CopyButton);
    DDX_Control(pDX, IDC_READ_POWER, m_PowerButton);
    DDX_Control(pDX, IDC_ABOUT_BTN, m_AboutBtn);
}

BEGIN_MESSAGE_MAP(CPPCMercuryDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_SCAN, &CPPCMercuryDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_COPY, &CPPCMercuryDlg::OnBnClickedCopy)
    ON_MESSAGE (WM_USER + 1, &CPPCMercuryDlg::UserMessage)
     
    
    ON_BN_CLICKED(IDC_ABOUT_BTN, &CPPCMercuryDlg::OnAbout)
END_MESSAGE_MAP()
       

// CPPCMercuryDlg message handlers

BOOL CPPCMercuryDlg::OnInitDialog()
{
    HANDLE hMutex = CreateMutex (NULL, TRUE, L"{BC63992B-A3BE-40af-A690-0E5CB8D2DDBD}");
    DWORD dwError = GetLastError ();
    if (NULL == hMutex  ||  ERROR_ALREADY_EXISTS == dwError)
    {   
        EndDialog (0);
        return FALSE;
    }

    if (! Protect (ELECTRO_SCANNER_KEY,
                   L"\\Keys\\BaseKey.txt", 
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

    PublishAppInfo ();

    ::SetTimer (m_hWnd, WAKEUP_TIMER_ID, WAKEUP_TIMER_INTERVAL, WakeUpTimerProc);
    wchar_t wcsFolderPath [1024];
    wchar_t wcsFilePath [1024];
    if (NULL != GetProgramFolder (wcsFolderPath, 
                                  SIZE_OF (wcsFolderPath)))
    {

        wcscpy (wcsFilePath, wcsFolderPath);
        wcscat (wcsFilePath, L"\\ScanSettings.txt");
        std::wifstream is (wcsFilePath);
        unsigned nVal = 0;
        if (is.is_open ())
        {
            is >> nVal;
            if (nVal > 0)
            {
                Mercury::Scanner::m_Instance.SetMonthCount (nVal);
            }
        }

        Mercury::Scanner::SerialNumberStorage &stg = Mercury::Scanner::m_Instance.GetMercury128K ();
        wcscpy (wcsFilePath, wcsFolderPath);
        wcscat (wcsFilePath, L"\\Mercury128K.txt");
        std::wifstream mercury128KStream (wcsFilePath);
        if (mercury128KStream.is_open ())
        {
            while (! mercury128KStream.eof ())
            {
                unsigned nVal = 0;
                mercury128KStream >> nVal;
                stg.push_back (nVal);
            }
        }
    }

    /*
    if ( ! scanResult.RestoreGuts (L"\\PPCMercury\\Results\\00337546.txt"))
    {
        MessageBox (L"File is corrupted");
    }
    scanResult.SaveGuts (L"test.txt");
    */
    
    GetWindowText (m_strTitle);
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_bPowerScanEnabled = TRUE;
    UpdateData (FALSE);

    CString strText (L"������� ������� ������ 1.11\n� �������, 2006. ��� ����� ���������������.");
    AddToLog (strText);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CPPCMercuryDlg::OnSize(UINT nType, int cx, int cy)
{
    int nOffset = 1;

    RECT startRect;
    m_ScanButton.GetWindowRect (&startRect);
    ScreenToClient (&startRect);
    int nHeight = startRect.bottom - startRect.top;
    startRect.bottom = cy - nOffset;
    startRect.top = startRect.bottom - nHeight;
    int nWidth = startRect.right - startRect.left;
    startRect.left = nOffset;
    startRect.right = startRect.left + nWidth;
    m_ScanButton.MoveWindow (&startRect);

    RECT aboutRect;
    m_AboutBtn.GetWindowRect (&aboutRect);
    ScreenToClient (&aboutRect);
    nWidth = aboutRect.right - aboutRect.left;
    aboutRect.top = startRect.top;
    aboutRect.bottom = startRect.bottom;
    aboutRect.right = cx - nOffset;
    aboutRect.left = aboutRect.right - nWidth;
    m_AboutBtn.MoveWindow (&aboutRect);
    
    RECT powerRect;
    m_PowerButton.GetWindowRect (&powerRect);
    ScreenToClient (&powerRect);
    RECT copyRect;
    m_CopyButton.GetWindowRect (&copyRect);
    ScreenToClient (&copyRect);

    int nPowerWidth = powerRect.right - powerRect.left;
    int nCopyWidth = copyRect.right - copyRect.left;
    int nSpace = ((aboutRect.left - startRect.right) - nPowerWidth - nCopyWidth) / 3;


    copyRect.top = powerRect.top = startRect.top;
    copyRect.bottom = powerRect.bottom = startRect.bottom;
    powerRect.left = startRect.right + nSpace;
    powerRect.right = powerRect.left + nPowerWidth;
    m_PowerButton.MoveWindow (&powerRect);

    copyRect.left = powerRect.right + nSpace;
    copyRect.right = copyRect.left + nCopyWidth;
    m_CopyButton.MoveWindow (&copyRect);


    RECT outputRect;
    outputRect.left = nOffset;
    outputRect.top = nOffset;
    outputRect.right = cx - nOffset;
    outputRect.bottom = startRect.top - nOffset;
    m_OutputBox.MoveWindow (&outputRect);  
}


void CPPCMercuryDlg::OnBnClickedButton1()
{
    if (NULL == m_hThread)
    {
        Start ();
    }
    else
    {
        Stop ();
    }
}

void CPPCMercuryDlg::Scan ()
{
    //m_ScanButton.EnableWindow (FALSE);
    ClearLog ();
    FlushLog ();

    SYSTEMTIME now;
    GetLocalTime (&now);
    wchar_t buf [64];

    wsprintf (buf, L"������������ ������ %02d:%02d:%02d %02d:%02d:%04d", 
        now.wHour, now.wMinute, now.wSecond,
        now.wDay, now.wMonth, now.wYear);

    AddToLog (buf);
    FlushLog ();
    _try
    {
        if (! transport.AutoConfig ())
        {
            AddToLog (L"\n�� ������� ������� ������������ ����\n");
            AddToLog (transport.GetLastErrorString ());
            FlushLog ();
            //m_ScanButton.EnableWindow (TRUE);

            MessageBeep (MB_ICONHAND);
            return;
        }

        AddToLog (L"\n������������ ���� ������� ������\n� ������������������");
        FlushLog ();
        Sleep (300);
        SetWindowText (L"��������...");
        if (! Mercury::Scanner::m_Instance.DoScan (scanResult, transport, m_bPowerScanEnabled != FALSE /*, Mercury::Connect::m_Level2DefaultPwd, Mercury::Connect::alLevel2*/))
        {
            AddToLog (L"\n�� ������� �������� ������������\n");
            AddToLog (Mercury::Scanner::m_Instance.GetLastErrorString ());            
            MessageBox (L"�� ������� �������� ��� ������ �� ��������. ���������� �� ���� ��������� � ����.",
                        L"��������!", MB_OK | MB_ICONWARNING);
        }
        else
        {
            SYSTEMTIME now;
            GetLocalTime (&now);
            wchar_t buf [64];
            wsprintf (buf, L"\n������������ ��������� %02d:%02d:%02d %02d:%02d:%04d", 
                now.wHour, now.wMinute, now.wSecond,
                now.wDay, now.wMonth, now.wYear);
            AddToLog (buf);


            bool bResultsSaved = false;
            wchar_t wcsPath [1024];
            if (NULL !=  GetProgramFolder (wcsPath, 
                                           SIZE_OF (wcsPath)))
            {
                wcscat (wcsPath, APP_RESULTS_FOLDER);
                CreateDirectory (wcsPath, NULL);
                wchar_t wcsFileName [1024];
                wsprintf (wcsFileName, L"%s\\%08u_%d.%s", 
                          wcsPath, 
                          scanResult.GetSerialNumber (), 
                          (int) scanResult.GetProtocolType (),
                          APP_RESULTS_EXT);

                bResultsSaved = scanResult.SaveGuts (wcsFileName);

                wsprintf (wcsFileName, L"%s\\%08u_%d.%s.log", 
                          wcsPath, 
                          scanResult.GetSerialNumber (), 
                          (int) scanResult.GetProtocolType (),
                          APP_RESULTS_EXT);
                SaveLog (wcsFileName);                

                wchar_t buf [64];
                if (bResultsSaved)
                {
                    wsprintf (buf, L"\n���������� ��������� � ���� %s", wcsFileName); 
                }
                else
                {
                    wsprintf (buf, L"\n�� ������� ��������� ���������� � ���� %s", wcsFileName); 
                }
                AddToLog (buf);
                FlushLog ();
            }
            else
            {
                AddToLog (L"\n�� ������� ��������� ���������");
                FlushLog ();
            }

            ShowScanResults ();            
            if (bResultsSaved)
            {
                MessageBox (L"�� �������� �������� ��� ����� � ������� ��������� � ����.",
                        L"�����", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox (L"�� �������� �������� ��� �����, �� �� �� ������� ��������� � ����.",
                        L"��������!", MB_OK | MB_ICONWARNING);
            }

        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        AddToLog (L"\n����������� �������������� ����������.\n����������, �������� �� ���� ������������ ������������ �����������.");    
    }

    SetWindowText (m_strTitle);
    //m_ScanButton.EnableWindow (TRUE);
    transport.Reset ();
    AddToLog (L"\n������������ ���� ������");    
    FlushLog ();   

    return;
}

void CPPCMercuryDlg::ShowScanResults ()
{  
    wchar_t buf [256];
    AddToLog (L"\n===��������� ������������ ��������===");
    Mercury::ProtocolType protocolType = scanResult.GetProtocolType ();
    
    //
    // ��� ���������
    //

    wsprintf (buf, L"\n��� ���������: %u", (int) scanResult.GetProtocolType ());
    AddToLog (buf);

    if (scanResult.IsNetAddressValid ())
    {
        wsprintf (buf, L"\n������� �����: 0x%02X", (int) scanResult.GetNetAddress ());
    }
    else
    {
        wcscpy (buf, L"\n������� �����: <�� �������>");
    }
    AddToLog (buf);

    //
    // ������� ����� ��������
    //

    if (scanResult.IsCounterTimeValid ())
    {
        MeteringData::DateTime &dt = scanResult.GetCounterTime ();
        wsprintf (buf, L"\n������� �����: %02u:%02u:%02u %02u.%02u.%04u (%s)", 
            dt.GetHour (), dt.GetMinute (), dt.GetSecond (),
            dt.GetDay (), dt.GetMonth (), dt.GetYear (),
            scanResult.IsWinter () ? L"����" : L"����");
    }
    else
    {
        wcscpy (buf, L"\n������ �����: <�� ��������>");
    }
    AddToLog (buf);

    //
    // �������� ����� � ���� ������������
    //

    if (scanResult.IsSerialNumberValid ())
    {
        MeteringData::DateTime &dt = scanResult.GetProductionDate ();
        wsprintf (buf, L"\n�������� �����: %08u\n���� ������������: %02u.%02u.%04u", 
            (int) scanResult.GetSerialNumber (),
            dt.GetDay (), dt.GetMonth (), dt.GetYear ());

    }
    else
    {
        wcscpy (buf, L"\n�������� �����: <�� �������>\n���� ������������: <�� ��������>");
    }
    AddToLog (buf);


    //
    // ������ ��
    //

    if (scanResult.IsSoftwareVersionValid ())
    {

        wsprintf (buf, L"\n������ ��: %S", scanResult.GetSoftwareVersion ());
    }
    else
    {
        wcscpy (buf, L"\n������ ��: <�� ��������>");
    }
    AddToLog (buf);

    //
    // �������� ������������ ��������
    //

    if (scanResult.IsLocationValid ())
    {
        wsprintf (buf, L"\n����������: %S", scanResult.GetLocation ());
    }
    else
    {
        wcscpy (buf, L"\n����������: <�� ��������>");
    }
    AddToLog (buf);


    unsigned short nKn = 0;
    unsigned short nKt = 0;

    //
    // ����������� ������������� �� ����������
    //

    if (scanResult.IsKnValid ())
    {
        nKn = scanResult.GetKn ();
        wsprintf (buf, L"\nK�: %u", (int) scanResult.GetKn ());
    }
    else
    {
        wcscpy (buf, L"\n��: <�� �������>");
    }
    AddToLog (buf);

    //
    // ����������� ������������� �� ����
    //

    if (scanResult.IsKtValid ())
    {
        nKt = scanResult.GetKt ();
        wsprintf (buf, L"\n��: %u", (int) scanResult.GetKt ());
    }
    else
    {
        wcscpy (buf, L"\n��: <�� �������>");
    }
    AddToLog (buf);

    //
    // ������� ����������
    //

    double counterConst = 0.0;
    if (scanResult.IsProductionTypeValid ())
    {
        const ProductionType& pt = scanResult.GetProductionType ();

        counterConst = pt.CounterConst1 ();
        if (scanResult.GetProtocolType () == Mercury::ptMercury)  
        {
            counterConst = pt.CounterConst2 ();
        }
                                     
        swprintf (buf, L"\n�.�. �� �������� �������: %.1f%%", pt.ActivePrecision ());
        AddToLog (buf);
        swprintf (buf, L"\n�.�. �� ���������� �������: %.1f%%", pt.ReactivePrecision ());
        AddToLog (buf);
        swprintf (buf, L"\n����������� ����������: %.1f �", pt.Voltage ());
        AddToLog (buf);
        swprintf (buf, L"\n����������� ���: %.1f �", pt.Current ());
        AddToLog (buf);
        swprintf (buf, L"\n����� �����������: %d", pt.DirectionCount ());
        AddToLog (buf);
        swprintf (buf, L"\n����� ���: %d", pt.PhaseCount ());
        AddToLog (buf);
        swprintf (buf, L"\n������������� ��������: -%d �", pt.TemperatureRange ());
        AddToLog (buf);
        swprintf (buf, L"\n���������� �������� (A): %.0f (���/���*�)", counterConst);
        AddToLog (buf);
    }
    else
    {
        AddToLog (L"\n������� ���������� �� �������");
    }

    AddToLog (L"\n�������� � ����������� �������");
    ShowAllEnergyInfo (scanResult.GetEnergy () [0],
        L"�������� �� ������� �� ���� �������", 
        protocolType, 
        counterConst, 
        nKn, 
        nKt);

    for (size_t nIdx = 1; nIdx < TARIFF_COUNT; ++ nIdx)
    {
        wsprintf (buf, L"�������� �� ������� �� ������ #%d", nIdx);
        ShowAllEnergyInfo (scanResult.GetEnergy () [nIdx],
                           buf, 
                           protocolType, 
                           counterConst, 
                           nKn, 
                           nKt);
    }

    //
    // ������ �������
    //

    AddToLog (L"\n������ �������");

    Mercury::ReadLogEntry::LogEntryType logEntryType = Mercury::ReadLogEntry::letOnOff;
    MeteringData::EventLogEntries* pEventLog = scanResult.GetEventLog ();        
    for (; logEntryType < Mercury::ReadLogEntry::letMaxValue; 
         logEntryType = (Mercury::ReadLogEntry::LogEntryType) (((int) logEntryType) + 1))
    {
        wsprintf (buf, L"\n%s", Mercury::ReadLogEntry::m_EvenLogTypeNames [logEntryType]);
        AddToLog (buf);

        for (unsigned char nRecord = 0; nRecord < LOG_ENTRY_COUNT; ++ nRecord)
        {
            MeteringData::EventLogEntry &eve = pEventLog [logEntryType] [nRecord];
            if (! eve.IsValid ()) continue;

            MeteringData::DateTime &fd = eve.FirstValue ();
            MeteringData::DateTime &sd = eve.SecondValue ();
            if (fd.IsEmpty () && sd.IsEmpty ())
            {
                continue;
            }

            wsprintf (buf, L"\n%d) %02d:%02d:%02d %02d.%02d.%02d - %02d:%02d:%02d %02d.%02d.%02d", 
                (int) (nRecord + 1),
                fd.GetHour (), fd.GetMinute (), fd.GetSecond (), fd.GetDay (), fd.GetMonth (), fd.GetYear () % 100,
                sd.GetHour (), sd.GetMinute (), sd.GetSecond (), sd.GetDay (), sd.GetMonth (), sd.GetYear () % 100);
            AddToLog (buf);
        }
    }
    /*
    AddToLog (L"\n������� ������� ���������");
    
    MaxPowerStorage &powerStorage = scanResult.GetPowerStorage ();
    ShowPowerInfo (powerStorage.GetActiveDirectPower (), L"P+", L"���");
    ShowPowerInfo (powerStorage.GetActiveRevercePower (), L"P-", L"���");
    ShowPowerInfo (powerStorage.GetActiveDirectPower (), L"Q+", L"����");
    ShowPowerInfo (powerStorage.GetActiveRevercePower (), L"Q-", L"����");
    */
    AddToLog (L"\n======");
}

void CPPCMercuryDlg::ShowAllEnergyInfo (const AllEnergyInfo &allEnergyInfo, 
                                        const wchar_t *wcsName,
                                        Mercury::ProtocolType protocolType,
                                        double counterConst,
                                        unsigned short nKn,
                                        unsigned short nKt)
{
    AddToLog (L"\n");
    AddToLog (wcsName);
    AddToLog (L":");
    if (! allEnergyInfo.IsValid ())
    {
        AddToLog (L" ��� ��������");
        return;
    }
    if (allEnergyInfo.IsEmpty ())
    {
        AddToLog (L" ����������� �������");
        return;
    }

    ShowEnergyInfo (allEnergyInfo.GetEnergyFromReset (), L"�� ������", protocolType, counterConst, nKn, nKt);
    ShowEnergyInfo (allEnergyInfo.GetEnergyToday (),     L"�������", protocolType, counterConst, nKn, nKt);
    ShowEnergyInfo (allEnergyInfo.GetEnergyYesterday (), L"�����", protocolType, counterConst, nKn, nKt);
    ShowEnergyInfo (allEnergyInfo.GetEnergyThisYear (),  L"� ���� ����", protocolType, counterConst, nKn, nKt);
    ShowEnergyInfo (allEnergyInfo.GetEnergyLastYear (),  L"� ������� ����", protocolType, counterConst, nKn, nKt);

    static wchar_t * monthNames [] = 
    {
        L"������",
        L"�������",
        L"����",
        L"������",
        L"���",
        L"����",
        L"����",
        L"������",
        L"��������",
        L"�������",
        L"������",
        L"�������"
    };

    const EnergyInfo *pEnergyInfo = allEnergyInfo.GetEnergyMonth ();
    for (size_t nIdx = 0; nIdx < 12; ++ nIdx)
    {
        ShowEnergyInfo (pEnergyInfo [nIdx],  monthNames [nIdx], protocolType, counterConst, nKn, nKt);
    }  
}
/*
void CPPCMercuryDlg::ShowPowerInfo (const MaxPowerStorage::MaxPowerPerMonth &maxPower, 
                                    const wchar_t *wcsName,
                                    const wchar_t *wcsUnit)
{
    //std::sort (maxPower.begin (), maxPower.end ());
    AddToLog (L"\n");
    AddToLog (wcsName);
    AddToLog (L":");

    wchar_t buf [256];
    size_t nCount = maxPower.size ();
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        swprintf (buf, L"\n%02d.%02d.%04d %02d:%02d %.4f %s", 
                  maxPower [nIdx].nDay, maxPower [nIdx].nMonth, maxPower [nIdx].nYear,
                  maxPower [nIdx].nHour, maxPower [nIdx].nHour,
                  maxPower [nIdx].fPowerValue, wcsUnit);    
        AddToLog (buf);
    }
}*/

void CPPCMercuryDlg::ShowEnergyInfo (const EnergyInfo &ei, 
                                     const wchar_t *wcsName,
                                     Mercury::ProtocolType protocolType,
                                     double counterConst,
                                     unsigned short nKn,
                                     unsigned short nKt)
{
    if (ei.IsEmpty ())
    {
        //AddToLog (L" ����������� �������");
        return;
    }

    
    if (! ei.bValid)
    {
        AddToLog (L"\n");
        AddToLog (wcsName);
        AddToLog (L":");
        AddToLog (L" ��� ��������");
        return;
    }

    AddToLog (L"\n");
    AddToLog (wcsName);
    AddToLog (L":");

    wchar_t buf [256];
    AddToLog (L"\n    A+ = ");
    if (Mercury::ptSET == protocolType)
    {
        swprintf (buf, L"%.3f ���*�",  ei.CalculateEnergyValue (ei.nActiveDirect, 
                                                                counterConst, 
                                                                nKn, 
                                                                nKt));
    }
    else
    {
        swprintf (buf, L"%.3f ���*�",  ei.nActiveDirect / 1000.0);
    }
    AddToLog (buf);

    AddToLog (L"\n    A- = ");
    if (Mercury::ptSET == protocolType)
    {
        swprintf (buf, L"%.3f ���*�",  ei.CalculateEnergyValue (ei.nActiveReverce, 
                                                                counterConst, 
                                                                nKn, 
                                                                nKt));
    }
    else
    {
        swprintf (buf, L"%.3f ���*�",  ei.nActiveReverce / 1000.0);
    }
    AddToLog (buf);

    
    AddToLog (L"\n    R+ = ");
    if (Mercury::ptSET == protocolType)
    {
        swprintf (buf, L"%.3f ���*�",  ei.CalculateEnergyValue (ei.nReactiveDirect, 
                                                                counterConst, 
                                                                nKn, 
                                                                nKt));
    }
    else
    {
        swprintf (buf, L"%.3f ���*�",  ei.nReactiveDirect / 1000.0);
    }
    AddToLog (buf);

    AddToLog (L"\n    R- = ");
    if (Mercury::ptSET == protocolType)
    {
        swprintf (buf, L"%.3f ���*�",  ei.CalculateEnergyValue (ei.nReactiveDirect, 
                                                                counterConst, 
                                                                nKn, 
                                                                nKt));
    }
    else
    {
        swprintf (buf, L"%.3f ���*�",  ei.nReactiveDirect / 1000.0);
    }
    AddToLog (buf);
}


void CPPCMercuryDlg::ClearLog ()
{
    m_strLog = L"";
}

void CPPCMercuryDlg::AddToLog (const wchar_t *wcsMessage)
{
    if (! IsWindow (m_OutputBox.m_hWnd))
    {
        return;
    }

    static wchar_t newLine [] = {13, 13, 10, 0};
    CString strMessage (wcsMessage);
    strMessage.Replace (L"\n", newLine);
    m_strLog += strMessage;
}

void CPPCMercuryDlg::FlushLog ()
{
    if (IsWindow (m_OutputBox.m_hWnd))
    {
        int nOffset = 0;
        int nLength = m_strLog.GetLength ();
        if (nLength > MAX_DISPLAY_CHAR_COUNT)
        {
            nOffset = nLength - MAX_DISPLAY_CHAR_COUNT;
            
        }

        m_OutputBox.SetWindowText (((LPCTSTR) m_strLog) + nOffset);
        m_OutputBox.LineScroll (m_OutputBox.GetLineCount ());
    }    
}


void CPPCMercuryDlg::OnBnClickedCopy ()
{
    m_OutputBox.SetSel (0, -1);    
    m_OutputBox.Copy ();
    m_OutputBox.SetSel (0, 0);    

}

DWORD ThreadFunction (LPVOID pValue) 
{
    CPPCMercuryDlg *pDlg = (CPPCMercuryDlg *) pValue;
    pDlg->Scan ();

    pDlg->m_hThread = NULL;
    Mercury::Scanner::m_Instance.SetStopRequest (false);
    if (IsWindow (pDlg->m_ScanButton.m_hWnd))
    {
        pDlg->m_ScanButton.SetWindowText (L"�����");
    }

    return 0;
}

void CPPCMercuryDlg::Start ()
{
    Mercury::Scanner::m_Instance.SetWindow (m_hWnd);
    Mercury::Scanner::m_Instance.SetStopRequest (false);    
    Stop ();
    m_ScanButton.SetWindowText (L"����");
    DWORD dwId = 0;  
    UpdateData (TRUE);
    m_hThread = CreateThread (NULL, 0, ThreadFunction, this, 0, &dwId);
    if (NULL != m_hThread)
    {
        m_ScanButton.SetWindowText (L"����");
    }
    else
    {
        m_ScanButton.SetWindowText (L"�����");
        AddToLog (L"\n�� ������� ��������� ����� ������������");
    }
}

void CPPCMercuryDlg::Stop (BOOL bDestruction/* = FALSE*/)
{
    if (NULL == m_hThread) 
    {
        if (IsWindow (m_ScanButton.m_hWnd))
        {
            m_ScanButton.SetWindowText (L"�����");
        }
        return;
    }

    Mercury::Scanner::m_Instance.SetStopRequest (true);    
    CloseHandle (m_hThread);
    m_hThread = NULL;
    /*
    WaitForSingleObject (m_hThread, 30000);
    DWORD dwExitCode = 0;
    BOOL bResult = GetExitCodeThread (m_hThread, &dwExitCode);
    if (! bResult || STILL_ACTIVE == dwExitCode)
    {
        TerminateThread (m_hThread, 0);    
        m_hThread = NULL;
    }*/
}

LRESULT CPPCMercuryDlg::UserMessage (WPARAM wParam, LPARAM lParam)
{
    AddToLog ((const wchar_t*) (lParam));
    FlushLog ();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       PublishAppInfo
//
//  Description:
//
//  ��������� � ������� ���������� � ����������
//
///////////////////////////////////////////////////////////////////////////////

void CPPCMercuryDlg::PublishAppInfo ()
{
    #define APP_VERSION   L"1.11"
    #define APP_PUBLISHER L"������� Software"

    DWORD dwResult;
    HKEY hSoftwareKey = NULL;
    dwResult = RegOpenKeyEx (HKEY_LOCAL_MACHINE, 
                             L"Software", 
                             0, 
                             0, 
                             &hSoftwareKey);
    if (ERROR_SUCCESS != dwResult)
    {
        return;
    }
    
    DWORD dwDisposition;
    HKEY hAppKey = NULL;
    dwResult = RegCreateKeyEx (hSoftwareKey, 
                               APP_KEY_NAME, 
                               0, 
                               NULL, 
                               REG_OPTION_NON_VOLATILE, 
                               0,
                               NULL,
                               &hAppKey,
                               &dwDisposition);
    RegCloseKey (hSoftwareKey);

    if (ERROR_SUCCESS != dwResult)
    {
        return;
    }

    wchar_t wcsPath [1024];
    if (NULL != GetProgramFolder (wcsPath, 
                                  SIZE_OF (wcsPath)))
    {

        RegSetValueEx (hAppKey, 
                       L"InstallLocation", 
                       0, 
                       REG_SZ, 
                       (const BYTE*) wcsPath, 
                       (wcslen (wcsPath) + 1) *  sizeof (wcsPath [0]));
    }

    RegSetValueEx (hAppKey, 
                   L"Version", 
                   0, 
                   REG_SZ, 
                   (const BYTE*) APP_VERSION, 
                   (wcslen (APP_VERSION) + 1) * sizeof (APP_VERSION [0]));

    RegSetValueEx (hAppKey, 
                   L"Vendor", 
                   0, 
                   REG_SZ, 
                   (const BYTE*) APP_VENDOR, 
                   (wcslen (APP_VENDOR) + 1) * sizeof (APP_VENDOR [0]));

    RegCloseKey (hAppKey);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnAbout
//
//  Description:
//
//  ������� ������ �� ���������� � ���������
//
///////////////////////////////////////////////////////////////////////////////

void CPPCMercuryDlg::OnAbout ()
{
    CAboutDlg dlg (this);
    dlg.DoModal ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveLog
//
//  Description:
//
//  ���������� ���� � ����
//
///////////////////////////////////////////////////////////////////////////////

void CPPCMercuryDlg::SaveLog (wchar_t *wcsFileName)
{
    FILE *pFile = _wfopen (wcsFileName, L"w");
    if (NULL != pFile)
    {
        CString strLog;
        m_OutputBox.GetWindowText (strLog);
        static wchar_t newLine [] = {13, 13, 10, 0};
        strLog.Replace (newLine, L"\n");
        fputws (strLog, pFile);
        fclose (pFile);
    }
}