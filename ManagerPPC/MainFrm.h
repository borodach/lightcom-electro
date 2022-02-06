///////////////////////////////////////////////////////////////////////////////
//
//  File:           MainFrm.h
//
//  Facility:       ЛайтКом Энерго Менеджер
//                  
//
//
//  Abstract:       Класс главного окна приложения
//
//
//  Environment:    MSVC 2005
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  22-Jan-2007
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MainFrm.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 4  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/ManagerPPC
*/

#ifndef _MAINFRM_H_
#define _MAINFRM_H_

#include "TreeViewPPC.h"
#include "DataManager.hpp"

//
// Тип элемента дерева
//

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
    // Конструктор
    //

	CMainFrame();
	DECLARE_DYNCREATE (CMainFrame)

    //
    // Деструктор
    //
	
    virtual ~CMainFrame();

    //
    // Возвращает true, если список потребителей был изменен.
    //

    bool IsModified () const;

    //
    // Изменяет признак того, что список потребителей был изменен.
    //

    void SetModified (bool bModified = true);

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
    // Сохранение списка потребителей.
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

protected:
	CCommandBar   m_wndCommandBar;      // Окно для размещения меню.
    CStatusBar    m_wndStatusBar;       // Строка состояния. 
    CTreeView    *m_pTreeView;          // Указатель на "вид" приложения.
    CTreeViewPPC  m_wndView;            // "Вид" приложения
    CMenu         m_TreeDefaultMenu;    // Контекстное меню по-умолчанию.
    CMenu         m_TreeCustomerMenu;   // Контекстное меню для элемента "Потребитель".
    CMenu         m_TreeCounterMenu;    // Контекстное меню для элемента "Счётчик".
    DataManager   m_DataMgr;            // Класс управления данными.
    bool          m_bIsModified;        // Признак того, что дерево потребителей изменилось.
protected:

    //
    // Обработчик отображения контекстного меню
    //

    afx_msg LRESULT OnMenu (WPARAM wParam, LPARAM lParam);
    
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
	
    //
    // Карта сообщений
    //

	DECLARE_MESSAGE_MAP()
};

#endif //_MAINFRM_H_


