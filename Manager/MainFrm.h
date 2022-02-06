///////////////////////////////////////////////////////////////////////////////
//
//  File:           MainFrm.h
//
//  Facility:       Desktop утилита для работы с результатами сканирования 
//                  счетчиков.
//                  
//
//
//  Abstract:       Главное окно приложения.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  11-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MainFrm.h $
 * 
 * *****************  Version 7  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * Добавлены Source Safe comments
* 
*/

#pragma once
#include "stdafx.h"
#include "afxhtml.h"
#include "afxcview.h"
#include "DataManagerXML.hpp"


enum TreeItemType
{
    titInvalid = -1,
    titCustomers,
    titCustomer,
    titCounter
};

class CMainFrame : public CFrameWnd
{
public:

    //
    // Конструктор.
    //

	CMainFrame();

    //
    // Деструктор.
    //

    virtual ~CMainFrame();

    //
    // Возвращает true, если список потребителей был изменен.
    //

    bool IsModified () const;

    //
    // Изменячет признак того, что список потребителей был изменен.
    //

    void SetModified (bool bModified = true);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    //
    // Обработчик нотификации "перед созданием окна".
    //

	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

    //
    // Диспетчеризация команд.
    //

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

    //
    //  Обновляет дерево потребителей.
    //

    void RefreshCustomersTree ();

    //
    // Загрузка списка потребителей.
    //

    bool LoadCustomers (wchar_t *wcsFileName = NULL);

    //
    // сохранёние списка потребителей.
    //

    bool SaveCustomers (wchar_t *wcsFileName = NULL);

    //
    // Чтение информации о заданном tree item. Выясняется тип item, а также 
    // индексы счетчик аи потребителя.
    //

    bool ReadTreeItemInfo (IN HTREEITEM hItem, 
                           OUT TreeItemType &itemType, 
                           OUT size_t &nCustomerIdx, 
                           OUT size_t &nCounterIdx);

    //
    // Добавить нового потребителя.
    //

    void AddCustomer ();

    //
    // Удалить потребителя.
    //

    void DeleteCustomer (HTREEITEM hItem, size_t nCustomerIdx);

    //
    // Редактировать потребителя.
    //

    void EditCustomer (HTREEITEM hItem, size_t nCustomerIdx);


    //
    // Добавить новый счетчик.
    //

    void AddCounter (HTREEITEM hItem, size_t nCustomerIdx);

    //
    // Удалить счетчик.
    //

    void DeleteCounter (HTREEITEM hItem, size_t nCustomerIdx, size_t nCounterIdx);

    //
    // Редактировать счетчик.
    //

    void EditCounter (HTREEITEM hItem, size_t nCustomerIdx, size_t nCounterIdx);

    //
    // Удаление всех потребителей.
    //

    void ClearCustomers ();

    //
    // Исправляет ссылки из элементов дерева - соседей hItem.
    // Для всех соседей, чья ссылка >= nBaseIdx, к ссылке добвляется nDelta. 
    //

    void FixReferences (HTREEITEM hItem, size_t nBaseIdx, int nDelta);
        
    //
    // Обработчик выделения узла "Все потребители".
    //

    void OnTreeCustomersSelected ();

    //
    // Обработчик выделения потребителя.
    //

    void OnTreeCustomerSelected (size_t nCustomerIdx);

    //
    // Обработчик выделения счетчика.
    //

    void OnTreeCounterSelected (size_t nCustomerIdx, size_t nCounterIdx);

    //
    // Добавить нового потребителя в дерево.
    //
    
    HTREEITEM InsertCustomerIntoTree (size_t nCustomerIdx);

    //
    // Добавить новый счетчик в дерево.
    //

    HTREEITEM InsertCounterIntoTree (HTREEITEM hCustomer, 
                                     const Customer::Counter &counter, 
                                     size_t nCounterIdx);

    //
    // Обновление HTML view в правой части окна.
    //

    void UpdateHtmlView (bool bReload, bool bRenderXML);

    //
    // Генерирует XML файлы с отчетами
    //

    void GenerateXMLFiles ();

    //
    // Удаляет все .XML отчеты
    //

    void ClearFiles ();

protected:  // control bar embedded members
	CStatusBar   m_wndStatusBar;
	CToolBar     m_wndToolBar;
	CReBar       m_wndReBar;
	CDialogBar   m_wndDlgBar;
    CSplitterWnd m_wndSplitter;

    CHtmlView    *m_pHtmlView;
    CTreeView    *m_pTreeView;

    CMenu         m_TreeDefaultMenu;
    CMenu         m_TreeCustomerMenu;
    CMenu         m_TreeCounterMenu;

    DataManagerXML m_DataMgr;

    bool m_bIsModified;

// Generated message map functions
protected:

    //
    // Обработчик создания окна.
    //

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    //
    // Обработчик получения фокуса.
    //

	afx_msg void OnSetFocus(CWnd *pOldWnd);

    //
    // Обработчик клавиатурных команд от TreeView
    //
    
    afx_msg void OnTreeKeyDown (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result);

    //
    // Обработчик изменения выделения в TreeView.
    //

    afx_msg void OnTreeSelChanged (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result);

    //
    // Обработчик правого клика на TreeView.
    //

    afx_msg void OnTreeRightClick (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result);

    //
    // Обработчик команды "перечитать дерево поребителей".
    //

    afx_msg void OnRefreshTree ();

    //
    // Обработчик команды "сохранить дерево поребителей".
    //

    afx_msg void OnSaveTree ();

    //
    // В зависимости от контекста управляет доступностью menu items.
    //

    afx_msg void OnUpdateMenuItem (CCmdUI *pCmdUI);

    //
    // Обработчик закрытитя окна приложения.
    //

    afx_msg void OnClose();

    //
    // Обработчик добавления потребителя.
    //

    afx_msg void OnAddCustomer ();

    //
    // Обработчик добавления счетчика.
    //

    afx_msg void OnAddCounter ();

    //
    // Обработчик удаления всех потребителей.
    //

    afx_msg void OnClearCustomers ();

    //
    // Обработчик удаления счетчика.
    //

    afx_msg void OnDeleteCounter ();

    //
    // Обработчик удаления потребителя.
    //

    afx_msg void OnDeleteCustomer ();

    //
    // Обработчик редактирования счетчика.
    //

    afx_msg void OnEditCounter ();

    //
    // Обработчик редактирования потребителя.
    //

    afx_msg void OnEditCustomer ();

    //
    // Обрабочик отображения лога сканирования счетчика.
    //

    afx_msg void OnViewCounterLog ();

    //
    // Обрабочик отображения диалога настроек параметров сканирования.
    //

    afx_msg void OnScanOptions ();

    DECLARE_MESSAGE_MAP()    
    DECLARE_DYNAMIC(CMainFrame)
};


