///////////////////////////////////////////////////////////////////////////////
//
//  File:           MainFrm.cpp
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
* $History: MainFrm.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 9  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/ManagerPPC
 * Подмешаны результаты работы не дома
 * 
 * *****************  Version 8  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/ManagerPPC
*/

#include "stdafx.h"
#include "ManagerPPC.h"
#include "MainFrm.h"

#include "CustomerPropertiesDlg.h"
#include "CounterPropertiesDlg.h"
#include "ScanOptionsDlg.h"
//#include "ViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef INVALID_FILE_ATTRIBUTES
#   define INVALID_FILE_ATTRIBUTES ((DWORD)-1) 
#endif

//
// Название root tree item
//

#define CUSTOMERS_TREE_ITEM_NAME L"Потребители"
#define CUSTOMERS_TREE_ITEM_NAME_MODIFIED L"*Потребители"

//
// Текст для modified pane в status bar
//

#define CUSTOMERS_NOT_MODIFIED L"Не изменен"
#define CUSTOMERS_MODIFIED L"Изменен"
#define CUSTOMERS_MODIFIED_IDX 1

//
// Индекс подменю "Правка"
//

#define EDIT_MENU_ITEM_IDX 2

// Индексы иконок для отображении в дереве. Индексы соответствуют очередности загрузки
// иконок в CImageList контрола CTreeCtrl

#define CUSTOMER_ICON 0
#define MERCURY_DEVICE_ICON 1
#define MERCURY_DEVICE_WITHDATA_ICON 2
#define MERCURY_DEVICE_WITHOUTDATA_ICON 3
#define ALL_CUSTOMER_ICON 4

// Определение типов счетчиков

#define MERCURY_TYPE_STRING L"Меркурий"
#define SET_TYPE_STRING L"СЭТ"
// CMainFrame


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

//
// Карта сообщений.
//

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_NOTIFY_RANGE(TVN_KEYDOWN, 0, 0xffffffff, OnTreeKeyDown)
    ON_NOTIFY_RANGE(TVN_SELCHANGED, 0, 0xffffffff, OnTreeSelChanged)
    ON_MESSAGE (WM_CONTEXTMENU, OnMenu)
    ON_COMMAND(ID_REFRESH_TREE, &CMainFrame::OnRefreshTree)
    ON_COMMAND(IDOK, &CMainFrame::OnClose)
    ON_COMMAND(ID_SAVE_CUSTOMERS, &CMainFrame::OnSaveTree)    
    ON_UPDATE_COMMAND_UI(ID_ADD_COUNTER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COUNTER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_DELETE_COUNTER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_COUNTER_LOG, &CMainFrame::OnUpdateMenuItem)    
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUSTOMER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_DELETE_CUSTOMER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_CLEAR_CUSTOMERS, &CMainFrame::OnUpdateMenuItem)
    ON_WM_CLOSE()
    ON_COMMAND(ID_ADD_CUSTOMER, &CMainFrame::OnAddCustomer)
    ON_COMMAND(ID_ADD_COUNTER, &CMainFrame::OnAddCounter)
    ON_COMMAND(ID_CLEAR_CUSTOMERS, &CMainFrame::OnClearCustomers)
    ON_COMMAND(ID_DELETE_COUNTER, &CMainFrame::OnDeleteCounter)
    ON_COMMAND(ID_DELETE_CUSTOMER, &CMainFrame::OnDeleteCustomer)
    ON_COMMAND(ID_EDIT_COUNTER, &CMainFrame::OnEditCounter)
    ON_COMMAND(ID_EDIT_CUSTOMER, &CMainFrame::OnEditCustomer)
    ON_COMMAND(ID_COUNTER_LOG, &CMainFrame::OnViewCounterLog)
    ON_COMMAND(ID_SCAN_OPTIONS, &CMainFrame::OnScanOptions)
END_MESSAGE_MAP()

const DWORD dwAdornmentFlags = 0; // exit button

//
// Индикаторы строки состояния.
//

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_SEPARATOR 	
};

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CMainFrame
//
//  Description:
//
//  Конструктор.
//
///////////////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame (): m_bIsModified (false)
{
	
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CMainFrame
//
//  Description:
//
//  Деструктор.
//
///////////////////////////////////////////////////////////////////////////////

CMainFrame::~CMainFrame()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCmdMsg
//
//  Description:
//
//  Диспетчеризация команд.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if (m_wndView.OnCmdMsg (nID, nCode, pExtra, pHandlerInfo))
        return TRUE;
        
    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       IsModified
//
//  Description:
//
//  Возвращает true, если список потребителей был изменен.
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::IsModified () const
{
    return m_bIsModified;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
    {
        TRACE0("Failed to create view window\n");
        return -1;
    }
    m_pTreeView = & m_wndView;

    if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAIN_MENU, 0))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	} 
    m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);

    //
    // Status bar
    //

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    m_wndStatusBar.SetPaneInfo (CUSTOMERS_MODIFIED_IDX, indicators [CUSTOMERS_MODIFIED_IDX], SBPS_NOBORDERS, 100);        
    m_wndStatusBar.SetPaneText (CUSTOMERS_MODIFIED_IDX, CUSTOMERS_NOT_MODIFIED);

    //
    // Dialog bar
    //
    /*
    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY);
    */
    
    
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    LONG nStyle = GetWindowLong (treeCtrl.m_hWnd, GWL_STYLE);
    SetWindowLong (treeCtrl.m_hWnd, GWL_STYLE, nStyle | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT);
    CImageList* pImageList = treeCtrl.GetImageList (TVSIL_NORMAL);

    if (NULL == pImageList)
    {
        pImageList = new CImageList ();
        pImageList->Create (16, 16, ILC_MASK, 5, 1);

        pImageList->SetBkColor ((COLORREF) GetSysColor (COLOR_WINDOW));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON1));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON2));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON3));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON4));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON5));

        treeCtrl.SetImageList (pImageList, TVSIL_NORMAL);
    }
    //
    // Загружаем контекстные меню
    //

    m_TreeDefaultMenu.LoadMenu (IDR_DEFAULT_MENU);
    m_TreeCustomerMenu.LoadMenu (IDR_CUSTOMER_MENU);
    m_TreeCounterMenu.LoadMenu (IDR_COUNTER_MENU);

    LoadCustomers ();

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetModified
//
//  Description:
//
//  Изменячет признак того, что список потребителей был изменен.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::SetModified (bool bModified/* = true*/)
{
    m_bIsModified = bModified;

    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hRoot = treeCtrl.GetRootItem ();
    if (NULL == hRoot)
    {
        return;
    }
    if (m_bIsModified)
    {
        treeCtrl.SetItemText (hRoot, CUSTOMERS_TREE_ITEM_NAME_MODIFIED);
        treeCtrl.SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD );
        m_wndStatusBar.SetPaneText (CUSTOMERS_MODIFIED_IDX, CUSTOMERS_MODIFIED);        
    }
    else
    {
        treeCtrl.SetItemText (hRoot, CUSTOMERS_TREE_ITEM_NAME);
        treeCtrl.SetItemState (hRoot, 0, TVIS_BOLD);
        m_wndStatusBar.SetPaneText (CUSTOMERS_MODIFIED_IDX, CUSTOMERS_NOT_MODIFIED);        
    }
}

// CMainFrame message handlers


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSetFocus
//
//  Description:
//
//  Обработчик получения фокуса.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSetFocus (CWnd* /*pOldWnd*/)
{
   m_wndView.SetFocus ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RefreshCustomersTree
//
//  Description:
//
//  Обновляет дерево потребителей.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::RefreshCustomersTree ()
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    treeCtrl.DeleteAllItems ();

    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = NULL;
    tvInsert.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvInsert.item.pszText = _T("Потребители");
    tvInsert.item.state = TVIS_BOLD | TVIS_EXPANDED;
    tvInsert.item.stateMask = TVIS_EXPANDED; //TVIS_BOLD
    tvInsert.item.iSelectedImage = tvInsert.item.iImage = ALL_CUSTOMER_ICON;
    HTREEITEM hAllCustomers = treeCtrl.InsertItem (&tvInsert);
    treeCtrl.SetItemData (hAllCustomers, (DWORD_PTR) ((0 & 0xffff) | (titCustomers << 16)));

    CustomerList &customers = m_DataMgr.GetCustomers ();
    size_t nCount = customers.size ();

    HTREEITEM hLastCustomer = NULL;
    for (size_t nIdx = 0; nIdx < nCount; ++ nIdx)
    {
        Customer &customer = customers [nIdx];
        hLastCustomer = InsertCustomerIntoTree (nIdx);

        Customer::CounterList &counters = customer.Counters ();
        size_t nCounterCount = counters.size ();
        HTREEITEM hLastCounter = NULL;
        for (size_t nCounter = 0; nCounter < nCounterCount; ++ nCounter)
        {
            Customer::Counter &counter = counters [nCounter];
            hLastCounter = InsertCounterIntoTree (hLastCustomer, counter, nCounter);
        }
    }

    treeCtrl.SetFocus ();
    treeCtrl.Expand (hAllCustomers, TVE_COLLAPSE);    
    treeCtrl.SelectItem (hAllCustomers);    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       LoadCustomers
//
//  Description:
//
//  Загрузка списка потребителей
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::LoadCustomers (wchar_t *wcsFileName/* = NULL*/)
{
    m_wndStatusBar.SetPaneText (0, L"Загрузка из файла...");

    wchar_t wcsPath [1024];
    wchar_t *pPath = NULL;
    if (NULL != wcsFileName)
    {
        wcscpy (wcsPath, wcsFileName);
        pPath = wcsPath;
    }

    SetModified (false);
    bool bResult = m_DataMgr.LoadCustomers (pPath);
    RefreshCustomersTree ();

    if (! bResult)
    {
        m_wndStatusBar.SetPaneText (0, L"Не удалось загрузить.");
    }
    else
    {
        m_wndStatusBar.SetPaneText (0, L"Успешно загружен.");
    }

    return bResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveCustomers
//
//  Description:
//
//  сохранёние списка потребителей.
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::SaveCustomers (wchar_t *wcsFileName/* = NULL*/)
{
    m_wndStatusBar.SetPaneText (0, L"Сохранение в файл...");

    wchar_t *pPath = NULL;
    if (NULL != wcsFileName)
    {
        pPath = wcsFileName;
    }

    if (m_DataMgr.SaveCustomers (pPath))
    {
        SetModified (false);
        m_wndStatusBar.SetPaneText (0, L"Успешно сохранён.");

        return true;
    }
    else
    {
        m_wndStatusBar.SetPaneText (0, L"Не удалось сохранить.");

        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ReadTreeItemInfo
//
//  Description:
//
//  Чтение информации о заданном tree item. Выясняется тип item, а также 
//  индексы счетчик и потребителя.
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::ReadTreeItemInfo (IN HTREEITEM hItem, 
                                   OUT TreeItemType &itemType, 
                                   OUT size_t &nCustomerIdx, 
                                   OUT size_t &nCounterIdx)
{
    wchar_t buffer [256] = L"";
    itemType = titInvalid;
    nCustomerIdx = ~0;
    nCounterIdx = ~0;

    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    size_t nIdx = (size_t) treeCtrl.GetItemData (hItem);

    itemType = (TreeItemType) (nIdx >> 16);
    nIdx &= 0xffff;
    if (titCustomer == itemType)
    {
        nCustomerIdx = nIdx;
        return true;
    }
    if (titCounter == itemType)
    {
        HTREEITEM hCustomerItem = treeCtrl.GetParentItem (hItem);
        if (NULL == hCustomerItem)
        {
            return false;
        }

        nCounterIdx = nIdx;
        nCustomerIdx = (size_t) treeCtrl.GetItemData (hCustomerItem) & 0xffff;

        return true;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeKeyDown
//
//  Description:
//
//  Обработчик клавиатурных команд от TreeView.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeKeyDown (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result)
{
    NMTVKEYDOWN *pNotify = (NMTVKEYDOWN*) pNotifyStruct;
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();

    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    if (NULL != hItem)
    {
        if (! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
        {
            itemType = titInvalid;
        }
    }

    switch (pNotify->wVKey)
    {
    case VK_INSERT:
        if (titCustomer == itemType || titCounter == itemType)
        {
            AddCounter (hItem, nCustomerIdx);
        }
        else
        {
            AddCustomer ();
        }

        break;
    case VK_DELETE:
        if (titCustomer == itemType)
        {
            if (IDYES != MessageBox (L"Удалить потребителя?", NULL, 
                MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
            {
                return;
            }

            DeleteCustomer (hItem, nCustomerIdx);
        }
        else if (titCounter == itemType)
        {
            if (IDYES != MessageBox (L"Удалить счетчик?", NULL, 
                MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
            {
                return;
            }

            DeleteCounter (hItem, nCustomerIdx, nCounterIdx);
        }
        break;

    case VK_F2:
        if (titCustomer == itemType)
        {
            EditCustomer (hItem, nCustomerIdx);
        }
        else if (titCounter == itemType)
        {
            EditCounter (hItem, nCustomerIdx, nCounterIdx);
        }
        break;

    case VK_F5:
        OnRefreshTree ();
        break;

    case VK_RETURN:
        treeCtrl.Expand (hItem, TVE_TOGGLE);
        break;
    }

    *result = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeSelChanged
//
//  Description:
//
//  Обработчик изменения выделения в TreeView.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeSelChanged (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result)
{
    /*
    NM_TREEVIEW *pNotify = (NM_TREEVIEW*) pNotifyStruct;
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = pNotify->itemNew.hItem;*/   
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnMenu
//
//  Description:
//
//  Обработчик отображения контекстного меню
//
///////////////////////////////////////////////////////////////////////////////

afx_msg LRESULT CMainFrame::OnMenu (WPARAM wParam, LPARAM lParam)
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    if ((HWND)(wParam) != m_pTreeView->m_hWnd)
    {
        return 0;
    }

       
    /*
    const MSG *pMsg = GetCurrentMessage ();
    if (NULL == pMsg)
    {
        return;
    }
    const POINT *pCursorPos = &pMsg->pt;
    */   

    POINT cursorPos;
    cursorPos.x = GET_X_LPARAM (lParam);
    cursorPos.y = GET_Y_LPARAM (lParam);
    treeCtrl.ScreenToClient (&cursorPos);
    POINT *pCursorPos = &cursorPos;
    //ISG_RIGHTTAP
    

    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    
    HTREEITEM hItem = treeCtrl.HitTest (*pCursorPos);
    if (NULL != hItem)
    {
        if (! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
        {
            itemType = titInvalid;
        }
        treeCtrl.SelectItem (hItem);
    }

    DWORD dwFlag = TPM_LEFTALIGN;// | TPM_RIGHTBUTTON;
    
    switch (itemType)
    {
        //case titCustomers:
        //    break;
    case titCustomer:
        m_TreeCustomerMenu.GetSubMenu (0)->TrackPopupMenu (dwFlag, pCursorPos->x, pCursorPos->y, this);
        break;
    case titCounter:
        m_TreeCounterMenu.GetSubMenu (0)->TrackPopupMenu (dwFlag, pCursorPos->x, pCursorPos->y, this);
        break;
    default:
        m_TreeDefaultMenu.GetSubMenu (0)->TrackPopupMenu (dwFlag, pCursorPos->x, pCursorPos->y, this);
        break;
    };

    MSG msg;
    PeekMessage (&msg, m_hWnd, WM_CONTEXTMENU, WM_CONTEXTMENU, PM_REMOVE);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnRefreshTree
//
//  Description:
//
//  Обработчик команды "перечитать дерево поребителей".
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnRefreshTree ()
{
    if (IsModified ())
    {
        if (IDYES != MessageBox (L"В списке потребителей есть несохранённые изменения.\nПеречитывание этого списка из файла приведет к потере этих изменений.\nПеречитать список из файла?", NULL, 
            MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
        {
            return;
        }
    }

    if (! LoadCustomers ())
    {
        MessageBox (L"Не удалось перечитать список потребителей.", NULL, MB_OK | MB_ICONERROR);
    } 
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSaveTree
//
//  Description:
//
//  Обработчик команды "сохранить дерево поребителей".
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSaveTree ()
{
    if (! SaveCustomers ())
    {
        MessageBox (L"Не удалось сохранить список потребителей.", NULL, MB_OK | MB_ICONERROR);
    }    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnUpdateMenuItem
//
//  Description:
//
//  В зависимости от контекста управляет доступностью menu items.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnUpdateMenuItem (CCmdUI *pCmdUI)
{
    if (ID_CLEAR_CUSTOMERS == pCmdUI->m_nID)
    {
        pCmdUI->Enable (m_DataMgr.GetCustomers ().size () > 0);
        return;
    }

    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (ID_COUNTER_LOG == pCmdUI->m_nID)
    {

        if (itemType != titCounter)
        {
            pCmdUI->Enable (FALSE);
            return;
        }


        CustomerList &customers = m_DataMgr.GetCustomers ();
        if (nCustomerIdx >= customers.size ())
        {
            pCmdUI->Enable (FALSE);
            return;
        }

        Customer::CounterList &counters = customers [nCustomerIdx].Counters ();
        if (nCounterIdx >= counters.size ())
        {
            pCmdUI->Enable (FALSE);
            return;
        }

        CString strFileName;
        GetProgramFolder (strFileName);
        strFileName += APP_RESULTS_FOLDER;
        CString strBuffer;
        strFileName += counters [nCounterIdx].GetLogFileName (strBuffer);
        //strFileName += L".txt";        

        pCmdUI->Enable (INVALID_FILE_ATTRIBUTES != GetFileAttributes (strFileName));

        return;
    }


    struct MenuItemAccessibility
    {
        DWORD dwMenuItemId;
        TreeItemType itemType;
    };

    static const MenuItemAccessibility itemInfo [] =
    {
        {ID_ADD_COUNTER,     titCustomer},
        {ID_EDIT_COUNTER,    titCounter},
        {ID_DELETE_COUNTER,  titCounter},
        //{ID_COUNTER_LOG,     titCounter},

        {ID_ADD_CUSTOMER,    titInvalid},
        //{ID_ADD_CUSTOMER,    titCustomer},
        {ID_EDIT_CUSTOMER,   titCustomer},
        {ID_DELETE_CUSTOMER, titCustomer},
    };

    for (size_t nIdx = 0; nIdx < sizeof (itemInfo) / sizeof (itemInfo [0]); ++ nIdx)
    {
        if (itemInfo [nIdx].dwMenuItemId != pCmdUI->m_nID ||
            itemInfo [nIdx].itemType != itemType) 
        {
            continue;
        }
        pCmdUI->Enable (TRUE);

        return;

    }
    pCmdUI->Enable (FALSE);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnClose
//
//  Description:
//
//  Обработчик закрытитя окна приложения.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnClose ()
{
    if (! IsModified ())
    {
        CFrameWnd::OnClose ();
        return;
    }

    DWORD dwResult = MessageBox (L"Сохранить изменения в списке потребителей?", NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
    if (IDNO == dwResult) 
    {
        CFrameWnd::OnClose ();
        return;
    }
    if (IDYES == dwResult) 
    {
        if (SaveCustomers ())
        {
            CFrameWnd::OnClose ();
            return;
        }
        else
        {
            MessageBox (L"Не удалось сохранить список потребителей.", NULL, MB_OK | MB_ICONERROR);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnAddCustomer
//
//  Description:
//
//  Обработчик добавления потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnAddCustomer ()
{
    AddCustomer ();   
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnAddCounter
//
//  Description:
//
//  Обработчик добавления счетчика.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnAddCounter ()
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (titCustomer == itemType)
    {
        AddCounter (hItem, nCustomerIdx); 
    }

    treeCtrl.Expand (hItem, TVE_EXPAND);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnClearCustomers
//
//  Description:
//
//  Обработчик удаления всех потребителей.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnClearCustomers ()
{
    if (IDYES != MessageBox (L"Удалить всех потребителей?", NULL, 
        MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
    {
        return;
    }

    ClearCustomers ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnDeleteCounter
//
//  Description:
//
//  Обработчик удаления счетчика.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnDeleteCounter ()
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (titCounter == itemType)
    {
        if (IDYES != MessageBox (L"Удалить счетчик?", NULL, 
            MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
        {
            return;
        }

        DeleteCounter (hItem, nCustomerIdx, nCounterIdx);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnDeleteCustomer
//
//  Description:
//
//  Обработчик удаления потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnDeleteCustomer ()
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (titCustomer == itemType)
    {
        if (IDYES != MessageBox (L"Удалить потребителя?", NULL, 
            MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
        {
            return;
        }

        DeleteCustomer (hItem, nCustomerIdx);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnEditCounter
//
//  Description:
//
//  Обработчик редактирования счетчика.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnEditCounter ()
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (titCounter == itemType)
    {
        EditCounter (hItem, nCustomerIdx, nCounterIdx);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnEditCustomer
//
//  Description:
//
//  Обработчик редактирования потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnEditCustomer ()
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (titCustomer == itemType)
    {
        EditCustomer (hItem, nCustomerIdx);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeCustomersSelected
//
//  Description:
//
//  Обработчик выделения узла "Все потребители".
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeCustomersSelected ()
{
    /*
    UpdateHtmlView (true, true);    

    CString strFolderName;
    GetProgramFolder (strFolderName);

    CString strXMLFile (strFolderName);
    strXMLFile += DEFAULT_XML_FOLDER;
    strXMLFile += "summary.xml";

    m_DataMgr.SetCurrentCustomerIdx (m_DataMgr.m_nInvalidIdx, 
        (LPCWSTR) strFolderName);
    CWaitCursor wait;
    MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
    if (NULL != xmlDocument)
    {
        MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateSummaryReport (xmlDocument); 
        if (NULL != ptrElement)
        {
            xmlDocument->documentElement->appendChild (ptrElement);
            //m_pHtmlView->Stop ();
            _variant_t fileName (strXMLFile);
            xmlDocument->save (fileName);    
        }
        else
        {
            MessageBox (L"Eror2");
        }
    }
    else
    {
        MessageBox (L"Error1");
    }
    m_pHtmlView->Navigate2 (strXMLFile, 
        navNoReadFromCache | navNoWriteToCache | navNoHistory);
    //MessageBox (L"Stub CMainFrame::OnTreeCustomersSelected");
    */
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeCustomerSelected
//
//  Description:
//
//  Обработчик выделения потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeCustomerSelected (size_t nCustomerIdx)
{
    m_DataMgr.JustChangeCurrentCustomerIdx (nCustomerIdx);   
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeCounterSelected
//
//  Description:
//
//  Обработчик выделения счетчика.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeCounterSelected (size_t nCustomerIdx, size_t nCounterIdx)
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       AddCustomer
//
//  Description:
//
//  Добавить нового потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::AddCustomer ()
{
    
    CCustomerPropertiesDlg dlg (this, L"Добавить потребителя");
    Customer customer;
    customer.SetPowerLimit (1);
    customer.SetName (L"Название потребителя");
    dlg.SetCustomer (customer);

    INT_PTR result = dlg.DoModal ();
    if (IDOK != result)
    {
        return;
    }
    customer = dlg.GetCustomer ();
    CString &strNewName = customer.GetName ();

    CustomerList &customers = m_DataMgr.GetCustomers ();
    size_t nIdx = 0;
    size_t nCount = customers.size ();
    for (; nIdx < nCount; ++ nIdx)
    {
        Customer& currentCustomer = customers [nIdx];
        if (customer < currentCustomer)
        {
            break;
        }
    }

    m_DataMgr.InsertCustomer (customer, nIdx);

    SetModified ();    
    HTREEITEM hNewItem = InsertCustomerIntoTree (nIdx);
    FixReferences (hNewItem, nIdx, 1);

    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    treeCtrl.Expand (treeCtrl.GetRootItem (), TVE_EXPAND);
    
    //UpdateHtmlView (false, true);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       InsertCustomerIntoTree
//
//  Description:
//
//  Добавить нового потребителя в дерево.
//
///////////////////////////////////////////////////////////////////////////////

HTREEITEM CMainFrame::InsertCustomerIntoTree (size_t nCustomerIdx)
{
    CustomerList &customers = m_DataMgr.GetCustomers ();
    Customer& customer = customers [nCustomerIdx];
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();            

    TVINSERTSTRUCT insertStruct;
    insertStruct.hInsertAfter = TVI_FIRST;
    insertStruct.hParent = treeCtrl.GetRootItem ();

    insertStruct.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    CString strBuffer;
    insertStruct.item.pszText = (LPWSTR)(LPCWSTR) customer.GetDisplayName (strBuffer);
    insertStruct.item.state = 0;
    insertStruct.item.iSelectedImage = insertStruct.item.iImage = CUSTOMER_ICON;


    /*
    for (HTREEITEM hSiblingItem = treeCtrl.GetChildItem (insertStruct.hParent); 
    NULL != hSiblingItem;  
    hSiblingItem = treeCtrl.GetNextSiblingItem (hSiblingItem))
    {
    CString strText = treeCtrl.GetItemText (hSiblingItem);
    if (strText.CompareNoCase (customer.GetName ()) > 0)
    {
    break;
    }
    insertStruct.hInsertAfter = hSiblingItem;
    }
    */

    HTREEITEM hNewItem = treeCtrl.InsertItem (&insertStruct);
    treeCtrl.SetItemData (hNewItem, (DWORD_PTR) ((nCustomerIdx & 0xffff) | (titCustomer << 16)));
    treeCtrl.SortChildren (insertStruct.hParent);

    return hNewItem;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       AddCounter
//
//  Description:
//
//  Добавить нового счетчика.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::AddCounter (HTREEITEM hItem, size_t nCustomerIdx)
{
    CustomerList &customers = m_DataMgr.GetCustomers ();
    if (nCustomerIdx >= customers.size ())
    {
        return;
    }
    Customer &customer = customers [nCustomerIdx];
    CCounterPropertiesDlg dlg (this, L"Добавить счётчик");
    Customer::Counter counter;
    counter.m_nProtocolType = Mercury::ptMercury;
    counter.m_nKn = counter.m_nKt = 1;
    counter.m_nSerialNumber = 0;
    dlg.SetCounter (counter);

    INT_PTR result = dlg.DoModal ();

    if (IDOK != result)
    {
        return;
    }
    counter = dlg.GetCounter ();
    
    Customer::CounterList &counters = customer.Counters ();

    size_t nIdx = 0;
    size_t nCount = counters.size ();
    for (; nIdx < nCount; ++ nIdx)
    {
        Customer::Counter& currentCounter = counters [nIdx];
        if (counter < currentCounter)
        {
            break;
        }
    }

    counters.insert (counters.begin () + nIdx, counter);
    
    SetModified ();    
    HTREEITEM hNewItem = InsertCounterIntoTree (hItem, counter, nIdx);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       InsertCounterIntoTree
//
//  Description:
//
//  Добавить новый счетчик в дерево.
//
///////////////////////////////////////////////////////////////////////////////

HTREEITEM CMainFrame::InsertCounterIntoTree (HTREEITEM hCustomer, 
                                             const Customer::Counter &counter, 
                                             size_t nCounterIdx)
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();            

    TVINSERTSTRUCT insertStruct;
    insertStruct.hInsertAfter = TVI_FIRST;
    insertStruct.hParent = hCustomer;
    insertStruct.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    CString strBuffer;
    insertStruct.item.pszText = (LPWSTR)(LPCWSTR) counter.GetFullDisplayName (strBuffer);
    insertStruct.item.state = 0;

    if (0 == wcscmp(counter.GetTypeString (), MERCURY_TYPE_STRING))
    {
        insertStruct.item.iSelectedImage = insertStruct.item.iImage = MERCURY_DEVICE_WITHOUTDATA_ICON;
    }

    /*
    for (HTREEITEM hSiblingItem = treeCtrl.GetChildItem (insertStruct.hParent); 
    NULL != hSiblingItem;  
    hSiblingItem = treeCtrl.GetNextSiblingItem (hSiblingItem))
    {
    CString strText = treeCtrl.GetItemText (hSiblingItem);
    if (strText.CompareNoCase (customer.GetName ()) > 0)
    {
    break;
    }
    insertStruct.hInsertAfter = hSiblingItem;
    }
    */

    HTREEITEM hNewItem = treeCtrl.InsertItem (&insertStruct);
    treeCtrl.SetItemData (hNewItem, (DWORD_PTR) ((nCounterIdx & 0xffff) | (titCounter << 16)));
    treeCtrl.SortChildren (insertStruct.hParent);

    return hNewItem;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DeleteCustomer
//
//  Description:
//
//  Удалить потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::DeleteCustomer (HTREEITEM hItem, size_t nCustomerIdx)
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    CustomerList& customers = m_DataMgr.GetCustomers ();
    if (nCustomerIdx >= customers.size ())
    {
        return;
    }

    SetModified ();
    m_DataMgr.DeleteCustomer (nCustomerIdx);
    FixReferences (hItem, nCustomerIdx, -1);
    treeCtrl.DeleteItem (hItem);

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       EditCustomer
//
//  Description:
//
//  Редактировать потребителя.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::EditCustomer (HTREEITEM hItem, size_t nCustomerIdx)
{
    CustomerList &customers = m_DataMgr.GetCustomers ();
    if (nCustomerIdx > customers.size ())
    {
        return;
    }

    CCustomerPropertiesDlg dlg;
    Customer &customer = customers [nCustomerIdx];
    CString strOldName = customer.GetName ();
    dlg.SetCustomer (customer);
    INT_PTR  result = dlg.DoModal ();

    if (IDOK != result)
    {
        //
        // Редактирование отменено.
        //

        return;
    }

    if (customer == dlg.GetCustomer ())
    {
        //
        // Потребитель не изменился.
        //

        return;
    }

    //
    // Потребитель изменился. Копируем изменения из диалога в память.
    //

    SetModified ();
    Customer newCustomer (dlg.GetCustomer ());
    m_DataMgr.DeleteCustomer (nCustomerIdx);

    size_t nIdx = 0;
    size_t nCount = customers.size ();
    for (; nIdx < nCount; ++ nIdx)
    {
        Customer& currentCustomer = customers [nIdx];
        if (newCustomer < currentCustomer)
        {
            break;
        }
    }

    m_DataMgr.InsertCustomer (newCustomer, nIdx);

    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    CString strBuffer;
    treeCtrl.SetItemText (hItem, newCustomer.GetDisplayName (strBuffer));
    /*
    if (nIdx == nCustomerIdx) 
    {
    return;
    }
    */

    treeCtrl.SortChildren (treeCtrl.GetRootItem ());
    treeCtrl.SetItemData (hItem, (DWORD_PTR) ((nIdx & 0xffff) | (titCustomer << 16)));

    FixReferences (hItem, nCustomerIdx, -1);
    FixReferences (hItem, nIdx, 1);

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DeleteCounter
//
//  Description:
//
//  Удалить счетчик.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::DeleteCounter (HTREEITEM hItem, size_t nCustomerIdx, size_t nCounterIdx)
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    CustomerList& customers = m_DataMgr.GetCustomers ();
    if (nCustomerIdx >= customers.size ())
    {
        return;
    }

    Customer::CounterList& counters = customers [nCustomerIdx].Counters ();
    if (nCounterIdx >= counters.size ())
    {
        return;
    }

    SetModified ();
    counters.erase (counters .begin () + nCounterIdx);
    FixReferences (hItem, nCounterIdx, -1);

    m_DataMgr.GenerateAllReports ();

    treeCtrl.DeleteItem (hItem);        

    //UpdateHtmlView (true, true);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       EditCounter
//
//  Description:
//
//  Редактировать счетчик.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::EditCounter (HTREEITEM hItem, size_t nCustomerIdx, size_t nCounterIdx)
{
    CustomerList &customers = m_DataMgr.GetCustomers ();
    if (nCustomerIdx > customers.size ())
    {
        return;
    }

    Customer::CounterList &counters = customers [nCustomerIdx].Counters ();
    if (nCounterIdx > counters.size ())
    {
        return;
    }
    
    CCounterPropertiesDlg dlg;
    Customer::Counter &counter = counters [nCounterIdx];
    dlg.SetCounter (counter);

    INT_PTR  result = dlg.DoModal ();
    if (IDOK != result)
    {
        //
        // Редактирование отменено.
        //

        return;
    }

    if (counter == dlg.GetCounter ())
    {
        //
        // Счетчик не изменился.
        //

        return;
    }

    //
    // Cчетчик изменился. Копируем изменения из диалога в память.
    //

    SetModified ();
    counter = dlg.GetCounter ();

    Customer::Counter newCounter (counter);
    counters.erase (counters.begin () + nCounterIdx);
    if (nCustomerIdx == m_DataMgr.GetCurrentCustomerIdx ())
    {
        CounterDataList &dataList = m_DataMgr.GetCounterData ();
        dataList.erase (dataList.begin () + nCounterIdx);
    }

    size_t nIdx = 0;
    size_t nCount = counters.size ();
    for (; nIdx < nCount; ++ nIdx)
    {
        Customer::Counter& currentCounter = counters [nIdx];
        if (newCounter < currentCounter)
        {
            break;
        }
    }

    counters.insert (counters.begin () + nIdx, newCounter);
   
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    CString strBuffer;
    treeCtrl.SetItemText (hItem, newCounter.GetFullDisplayName (strBuffer));
    /*    if (nIdx == nCounterIdx) 
    {
    return;
    }
    */                   
    treeCtrl.SortChildren (treeCtrl.GetParentItem (hItem));
    treeCtrl.SetItemData (hItem, (DWORD_PTR) ((nIdx & 0xffff) | (titCounter << 16)));

    FixReferences (hItem, nCounterIdx, -1);
    FixReferences (hItem, nIdx, 1);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ClearCustomers
//
//  Description:
//
//  Удаление всех потребителей.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::ClearCustomers ()
{
    m_DataMgr.Reset ();    
    RefreshCustomersTree ();
    SetModified ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       FixReferences
//
//  Description:
//
//  Исправляет ссылки из элементов дерева - соседей hItem.
//  Для всех соседей, чья ссылка >= nBaseIdx, к ссылке добвляется nDelta. 
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::FixReferences (HTREEITEM hItem, size_t nBaseIdx, int nDelta)
{
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();

    for (HTREEITEM hSiblingItem = treeCtrl.GetNextSiblingItem (hItem); NULL != hSiblingItem;  hSiblingItem = treeCtrl.GetNextSiblingItem (hSiblingItem))
    {
        DWORD_PTR pData = treeCtrl.GetItemData (hSiblingItem);
        size_t nData = ((size_t) pData) & 0xffff;
        if (nData >= nBaseIdx) 
        {
            nData += nDelta;
            pData = (DWORD_PTR) ((pData & 0xffff0000) | nData);
            treeCtrl.SetItemData (hSiblingItem, pData);
        }
    }

    for (HTREEITEM hSiblingItem = treeCtrl.GetPrevSiblingItem (hItem); NULL != hSiblingItem;  hSiblingItem = treeCtrl.GetPrevSiblingItem (hSiblingItem))
    {
        DWORD_PTR pData = treeCtrl.GetItemData (hSiblingItem);
        size_t nData = ((size_t) pData) & 0xffff;
        if (nData >= nBaseIdx) 
        {
            nData += nDelta;
            pData = (DWORD_PTR) ((pData & 0xffff0000) | nData);
            treeCtrl.SetItemData (hSiblingItem, pData);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnViewCounterLog
//
//  Description:
//
//  Обработчик отображения лога сканирования счетчика.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnViewCounterLog ()
{
    /*
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem ();
    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    if (NULL == hItem ||
        ! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        itemType = titInvalid;
    }

    if (itemType != titCounter)
    {
        return;
    }

    CustomerList &customers = m_DataMgr.GetCustomers ();
    if (nCustomerIdx >= customers.size ())
    {               
        return;
    }

    Customer::CounterList &counters = customers [nCustomerIdx].Counters ();
    if (nCounterIdx >= counters.size ())
    {
        return;
    }

    CString strFileName;
    GetProgramFolder (strFileName);
    strFileName += DEFAULT_RESULTS_FOLDER;
    CString strBuffer;
    strFileName += counters [nCounterIdx].GetLogFileName (strBuffer);
    //strFileName += L".txt";  

    CString strLogMsg (L"");
    std::ifstream is (strFileName);
    if (is.is_open ())
    {
        while (! is.eof ())        
        {
            std::string strLine;
            std::getline (is, strLine);
            CString strBuffer;
            MultiByte2WideChar (strLine.c_str (), strBuffer);
            strLogMsg += strBuffer;
            strLogMsg += L"\n";
        }

        m_wndStatusBar.SetPaneText (0, L"Загружен журнал сканирования счетчика.");
    }

    CViewerDlg dlg (this);

    dlg.SetLogMsg (strLogMsg);
    dlg.DoModal ();
    */
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnScanOptions
//
//  Description:
//
//  Обработчик отображения диалога настроек параметров сканирования.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnScanOptions ()
{
    CString strBaseFolder;
    GetProgramFolder (strBaseFolder);
    strBaseFolder += "\\";
    CScanOptionsDlg dlg (strBaseFolder, this);
    dlg.DoModal ();
}