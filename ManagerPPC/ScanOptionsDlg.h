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
* $History: ScanOptionsDlg.h $
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
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
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
    // Конструктор.
    //

	CScanOptionsDlg (const CString &strFolder, CWnd* pParent = NULL);

    //                                                                           
    //  Деструктор.
    //

	virtual ~CScanOptionsDlg();

    //
    // Идентификатор диалога.
    //

	enum { IDD = IDD_SCAN_OPTIONS };

    //                                                                           
    // Обмен данными с диалогом.
    //

    virtual void DoDataExchange (CDataExchange* pDX);

    //
    // Инициализация диалога.
    //

    virtual BOOL OnInitDialog();

    //
    // Обработчик клика на кнопку OK.
    //

    afx_msg void OnBnClickedOk();

    //                                                                           
    // Обработчик клика на кнопку Применить.
    //

    afx_msg void OnBnClickedApply();

    //afx_msg void OnBnClickedReload();

    //                                                                           
    // Обработчик изменения списка серийных номеров.
    //

    afx_msg void OnEnChangeMercury64();

    //                                                                           
    // Обработчик изменения глубины сканирования.
    //

    afx_msg void OnEnChangeScanDepth();

    //
    // Обработчик изменения размера окна
    //

    afx_msg void OnSize(UINT nType, int cx, int cy);

    //                                                                           
    // Запись параметров в конфигурационные файлы.
    //

    bool Save ();

    //                                                                           
    // Чтение параметров из конфигурационных файлов.
    //

    bool Load ();

    //
    // Карта сообщений.
    //

	DECLARE_MESSAGE_MAP()

protected:

    //
    // Спин для глубины сканирования.
    //

    CSpinButtonCtrl m_ScanDepthSpin;

    //
    // RichEdit для списка серийных номеров.
    //

    CEdit m_Mercury64;

    //
    // Конпка "Применить".
    //

    CButton m_ApplyButton;

    //
    // Глубина сканирования.
    //

    UINT m_nScatDepth;

    //
    // Папка с конфигурационными файлами.
    //

    CString m_strFolder;

    //
    // Список серийных номеров счетчиков, имеющих 64K памяти.
    //

    CString m_strMercury64;
};
