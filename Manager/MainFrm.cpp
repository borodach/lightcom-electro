///////////////////////////////////////////////////////////////////////////////
//
//  File:           MainFrm.cpp
//
//  Facility:       Desktop ������� ��� ������ � ������������ ������������ 
//                  ���������.
//                  
//
//
//  Abstract:       ������� ���� ����������.
//
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  11-Sep-2006
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MainFrm.cpp $
 * 
 * *****************  Version 16  *****************
 * User: Sergey       Date: 16.02.07   Time: 22:52
 * Updated in $/PPCMercury.root/Manager
 * ��������� ���������� ������ �� ����
 * 
 * *****************  Version 15  *****************
 * User: Sergey       Date: 21.01.07   Time: 13:38
 * Updated in $/PPCMercury.root/Manager
 * ��� ��������...
 * 
 * *****************  Version 14  *****************
 * User: Sergey       Date: 21.01.07   Time: 13:35
 * Updated in $/PPCMercury.root/Manager
 * 
 * *****************  Version 13  *****************
 * User: Sergey       Date: 19.01.07   Time: 21:56
 * Updated in $/PPCMercury.root/Manager
 * ���������� ������ ������
 * 
 * *****************  Version 12  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
* 
*/

#include "MainFrm.h"
#include "Manager.h"
#include "strsafe.h"
#include "CustomerPropertiesDlg.h"
#include "CounterPropertiesDlg.h"
#include "ScanOptionsDlg.h"
#include "ViewerDlg.h"
#include <string>
#include "MSXMLUtils.hpp"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
// �������� root tree item
//

#define CUSTOMERS_TREE_ITEM_NAME L"�����������"
#define CUSTOMERS_TREE_ITEM_NAME_MODIFIED L"*�����������"

//
// ����� ��� modified pane � status bar
//

#define CUSTOMERS_NOT_MODIFIED L"�� �������"
#define CUSTOMERS_MODIFIED L"�������"
#define CUSTOMERS_MODIFIED_IDX 1

//
// ������ ������� "������"
//

#define EDIT_MENU_ITEM_IDX 2

// ������� ������ ��� ����������� � ������. ������� ������������� ����������� ��������
// ������ � CImageList �������� CTreeCtrl

#define CUSTOMER_ICON 0
#define MERCURY_DEVICE_ICON 1
#define MERCURY_DEVICE_WITHDATA_ICON 2
#define MERCURY_DEVICE_WITHOUTDATA_ICON 3
#define ALL_CUSTOMER_ICON 4

// ����������� ����� ���������

#define MERCURY_TYPE_STRING L"��������"
#define SET_TYPE_STRING L"���"
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
    ON_NOTIFY_RANGE(TVN_KEYDOWN, 0, 0xffffffff, OnTreeKeyDown)
    ON_NOTIFY_RANGE(TVN_SELCHANGED, 0, 0xffffffff, OnTreeSelChanged)
    ON_NOTIFY_RANGE(NM_RCLICK, 0, 0xffffffff,OnTreeRightClick)
    ON_COMMAND(ID_REFRESH_TREE, &CMainFrame::OnRefreshTree)
    ON_COMMAND(ID_SAVE_CUSTOMERS, &CMainFrame::OnSaveTree)    
    ON_UPDATE_COMMAND_UI(ID_ADD_COUNTER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COUNTER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_DELETE_COUNTER, &CMainFrame::OnUpdateMenuItem)
    ON_UPDATE_COMMAND_UI(ID_COUNTER_LOG, &CMainFrame::OnUpdateMenuItem)    
//    ON_UPDATE_COMMAND_UI(ID_ADD_CUSTOMER, &CMainFrame::OnUpdateMenuItem)
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
//  �����������
//
///////////////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame (): m_bIsModified (false)
{
    setlocale (LC_ALL, ".1251");
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CMainFrame
//
//  Description:
//
//  ����������
//
///////////////////////////////////////////////////////////////////////////////

CMainFrame::~CMainFrame ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       IsModified
//
//  Description:
//
//  ���������� true, ���� ������ ������������ ��� �������.
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::IsModified () const
{
    return m_bIsModified;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SetModified
//
//  Description:
//
//  ��������� ������� ����, ��� ������ ������������ ��� �������.
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

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCreate
//
//  Description:
//
//  ���������� �������� ����
//
///////////////////////////////////////////////////////////////////////////////

int CMainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
 	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
    /*
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_LEFT | CBRS_SIZE_DYNAMIC , AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	} */
    
    if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar)/* ||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	} 

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
    if (!m_wndSplitter.CreateStatic  (this, 1, 2))
    {
        TRACE0("Failed to create splitter window\n");
		return FALSE;
    }
    
    CSize htmlSize (300, 50);
    CSize treeSize (300, 50);

    m_wndSplitter.CreateView (0, 0, RUNTIME_CLASS(CTreeView), treeSize, NULL); 
    m_wndSplitter.CreateView (0, 1, RUNTIME_CLASS(CHtmlView), htmlSize, NULL); 
    
    m_pTreeView = (CTreeView*) m_wndSplitter.GetPane (0, 0);
    m_pHtmlView = (CHtmlView*) m_wndSplitter.GetPane (0, 1);

    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    LONG nStyle = GetWindowLong (treeCtrl.m_hWnd, GWL_STYLE);
    SetWindowLong (treeCtrl.m_hWnd, GWL_STYLE, nStyle | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT);
    CImageList* pImageList = treeCtrl.GetImageList (TVSIL_NORMAL);

    if (NULL == pImageList)
    {
        pImageList = new CImageList ();
        pImageList->Create (16, 16, ILC_COLOR8 | ILC_MASK, 5, 1);
        
        pImageList->SetBkColor ((COLORREF) GetSysColor (COLOR_WINDOW));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON1));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON2));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON3));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON4));
        pImageList->Add (AfxGetApp ()->LoadIcon (IDI_ICON5));

        treeCtrl.SetImageList (pImageList, TVSIL_NORMAL);
    }
    //
    // ��������� ����������� ����
    //

    m_TreeDefaultMenu.LoadMenu (IDR_DEFAULT_MENU);
    m_TreeCustomerMenu.LoadMenu (IDR_CUSTOMER_MENU);
    m_TreeCounterMenu.LoadMenu (IDR_COUNTER_MENU);


    //m_pHtmlView->Navigate (L"about:blank");

 
    LoadCustomers ();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       PreCreateWindow
//
//  Description:
//
//  ���������� ����������� "����� ��������� ����".
//
///////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::PreCreateWindow (CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSetFocus
//
//  Description:
//
//  ���������� ��������� ������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSetFocus (CWnd* /*pOldWnd*/)
{
    m_wndSplitter.SetFocus();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCmdMsg
//
//  Description:
//
//  ��������������� ������.
//
///////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{

    if (m_wndSplitter.OnCmdMsg (nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       RefreshCustomersTree
//
//  Description:
//
//  ��������� ������ ������������.
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
    tvInsert.item.pszText = _T("�����������");
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
//  �������� ������ ������������
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::LoadCustomers (wchar_t *wcsFileName/* = NULL*/)
{
    m_wndStatusBar.SetPaneText (0, L"�������� ������ ������������ �� �����...");

    wchar_t wcsPath [1024];
    if (NULL == wcsFileName)
    {
        GetProgramFolder (wcsPath, sizeof (wcsPath) / sizeof (wcsPath [0]));
        wcscat (wcsPath, DEFAULT_CUSTOMERS_FILE);
    }
    else
    {
        wcscpy (wcsPath, wcsFileName);
    }

    SetModified (false);
    bool bResult = m_DataMgr.LoadCustomers (wcsPath);
    RefreshCustomersTree ();

    if (! bResult)
    {
        m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ������ ������������.");
    }
    else
    {
        m_wndStatusBar.SetPaneText (0, L"������ ������������ ������� ��������.");
    }

    return bResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       SaveCustomers
//
//  Description:
//
//  ��������� ������ ������������.
//
///////////////////////////////////////////////////////////////////////////////

bool CMainFrame::SaveCustomers (wchar_t *wcsFileName/* = NULL*/)
{
    m_wndStatusBar.SetPaneText (0, L"���������� ������ ������������ � ����...");

    CString strPath;
    if (NULL == wcsFileName)
    {
        GetProgramFolder (strPath);
        strPath += DEFAULT_CUSTOMERS_FILE;
    }
    else
    {
        strPath = wcsFileName;
    }
    
    if (m_DataMgr.SaveCustomers (strPath))
    {
        SetModified (false);
        m_wndStatusBar.SetPaneText (0, L"������ ������������ ������� �������.");

        return true;
    }
    else
    {
        m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ������ ������������.");

        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ReadTreeItemInfo
//
//  Description:
//
//  ������ ���������� � �������� tree item. ���������� ��� item, � ����� 
//  ������� ������� � �����������.
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
//  ���������� ������������ ������ �� TreeView.
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
            if (IDYES != MessageBox (L"������� �����������?", NULL, 
                MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
            {
                return;
            }

            DeleteCustomer (hItem, nCustomerIdx);
        }
        else if (titCounter == itemType)
        {
            if (IDYES != MessageBox (L"������� �������?", NULL, 
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
//  ���������� ��������� ��������� � TreeView.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeSelChanged (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result)
{
    NM_TREEVIEW *pNotify = (NM_TREEVIEW*) pNotifyStruct;
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = pNotify->itemNew.hItem;
    if (NULL != hItem)
    {
        UpdateHtmlView (false, false);
        //UpdateHtmlView ();
        /*
        TreeItemType itemType;
        size_t nCustomerIdx = ~0;
        size_t nCounterIdx = ~0;

        if (! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
        {
            return;
        }

        switch (itemType)
        {
        case titCustomers:
            OnTreeCustomersSelected ();
            break;
        case titCustomer:
            OnTreeCustomerSelected (nCustomerIdx);
            break;
        case titCounter:
            OnTreeCounterSelected (nCustomerIdx, nCounterIdx);
            break;
        };*/
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeRightClick
//
//  Description:
//
//  ���������� ������� ����� �� TreeView.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeRightClick (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result)
{
    POINT cursorPos;
    GetCursorPos (&cursorPos);
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    treeCtrl.ScreenToClient (&cursorPos);
    if (pNotifyStruct->hwndFrom != treeCtrl.m_hWnd)
    {
        return;
    }

    TreeItemType itemType = titInvalid;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;

    HTREEITEM hItem = treeCtrl.HitTest (cursorPos);
    if (NULL != hItem)
    {
        if (! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
        {
            itemType = titInvalid;
        }
        treeCtrl.SelectItem (hItem);
    }

    DWORD dwFlag = TPM_LEFTALIGN | TPM_LEFTBUTTON;
    POINT pt;
    GetCursorPos (&pt);

    switch (itemType)
    {
    //case titCustomers:
    //    break;
    case titCustomer:
        m_TreeCustomerMenu.GetSubMenu (0)->TrackPopupMenu (dwFlag, pt.x, pt.y, this);
        break;
    case titCounter:
        m_TreeCounterMenu.GetSubMenu (0)->TrackPopupMenu (dwFlag, pt.x, pt.y, this);
        break;
    default:
        m_TreeDefaultMenu.GetSubMenu (0)->TrackPopupMenu (dwFlag, pt.x, pt.y, this);
        break;
    };
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnRefreshTree
//
//  Description:
//
//  ���������� ������� "���������� ������ �����������".
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnRefreshTree ()
{
    if (IsModified ())
    {
        if (IDYES != MessageBox (L"� ������ ������������ ���� ������������ ���������.\n������������� ����� ������ �� ����� �������� � ������ ���� ���������.\n���������� ������ �� �����?", NULL, 
            MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1))
        {
            return;
        }
    }

    if (! LoadCustomers ())
    {
        MessageBox (L"�� ������� ���������� ������ ������������.", NULL, MB_OK | MB_ICONERROR);
    } 
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSaveTree
//
//  Description:
//
//  ���������� ������� "��������� ������ �����������".
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSaveTree ()
{
    if (! SaveCustomers ())
    {
        MessageBox (L"�� ������� ��������� ������ ������������.", NULL, MB_OK | MB_ICONERROR);
    }    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnUpdateMenuItem
//
//  Description:
//
//  � ����������� �� ��������� ��������� ������������ menu items.
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
        strFileName += DEFAULT_RESULTS_FOLDER;
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
//  ���������� ��������� ���� ����������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnClose ()
{
    if (! IsModified ())
    {
        CFrameWnd::OnClose ();
        return;
    }

    DWORD dwResult = MessageBox (L"��������� ��������� � ������ ������������?", NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
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
            MessageBox (L"�� ������� ��������� ������ ������������.", NULL, MB_OK | MB_ICONERROR);
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
//  ���������� ���������� �����������.
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
//  ���������� ���������� ��������.
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
//  ���������� �������� ���� ������������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnClearCustomers ()
{
    if (IDYES != MessageBox (L"������� ���� ������������?", NULL, 
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
//  ���������� �������� ��������.
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
        if (IDYES != MessageBox (L"������� �������?", NULL, 
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
//  ���������� �������� �����������.
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
        if (IDYES != MessageBox (L"������� �����������?", NULL, 
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
//  ���������� �������������� ��������.
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
//  ���������� �������������� �����������.
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
//  ���������� ��������� ���� "��� �����������".
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeCustomersSelected ()
{

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
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeCustomerSelected
//
//  Description:
//
//  ���������� ��������� �����������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeCustomerSelected (size_t nCustomerIdx)
{
    bool bCustomerChanged = m_DataMgr.GetCurrentCustomerIdx () != nCustomerIdx;
    UpdateHtmlView (bCustomerChanged, bCustomerChanged);    

    CString strFolderName;
    GetProgramFolder (strFolderName);
    CString strXMLFile (strFolderName);
    strXMLFile += DEFAULT_HTML_FOLDER;
    strXMLFile += "customer.html";

    strFolderName += DEFAULT_RESULTS_FOLDER;
    m_DataMgr.SetCurrentCustomerIdx (nCustomerIdx, (LPCWSTR) strFolderName);
    CWaitCursor wait;
    /*
    MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
    if (NULL != xmlDocument)
    {
        MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCounterReport (xmlDocument, nCounterIdx); 
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
    */
    m_pHtmlView->Navigate2 (strXMLFile, 
        navNoReadFromCache | navNoWriteToCache | navNoHistory );

    //MessageBox (L"Stub CMainFrame::OnTreeCustomerSelected");
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnTreeCounterSelected
//
//  Description:
//
//  ���������� ��������� ��������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTreeCounterSelected (size_t nCustomerIdx, size_t nCounterIdx)
{
    bool bCustomerChanged = m_DataMgr.GetCurrentCustomerIdx () != nCustomerIdx;
    UpdateHtmlView (bCustomerChanged, bCustomerChanged);    

    CString strFolderName;
    GetProgramFolder (strFolderName);    
    CString strXMLFile (strFolderName);
    strXMLFile += DEFAULT_XML_FOLDER;
    strXMLFile += "counter.xml";

    strFolderName += DEFAULT_RESULTS_FOLDER;
    m_DataMgr.SetCurrentCustomerIdx (nCustomerIdx, (LPCWSTR) strFolderName);
    CWaitCursor wait;
    MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
    if (NULL != xmlDocument)
    {
        MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCounterReport (xmlDocument, nCounterIdx); 
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
    // MessageBox (L"Stub CMainFrame::OnTreeCounterSelected");
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       AddCustomer
//
//  Description:
//
//  �������� ������ �����������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::AddCustomer ()
{
    CCustomerPropertiesDlg dlg (this, L"�������� �����������");
    Customer customer;
    customer.SetPowerLimit (1);
    customer.SetName (L"�������� �����������");
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

    UpdateHtmlView (false, true);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       InsertCustomerIntoTree
//
//  Description:
//
//  �������� ������ ����������� � ������.
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
//  �������� ������ ��������.
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
    CCounterPropertiesDlg dlg (this, L"�������� �������");
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
    if (nCustomerIdx == m_DataMgr.GetCurrentCustomerIdx ())
    {
        CounterDataList &dataList = m_DataMgr.GetCounterData ();
        CounterData newData;
        newData.SetCounter (counter);
        CString strFullFilePath;
        GetProgramFolder (strFullFilePath);
        strFullFilePath += DEFAULT_RESULTS_FOLDER;
        CString strFileName;
        counter.GetFileName (strFileName);
        strFileName += L"."APP_RESULTS_EXT;
        strFullFilePath += strFileName;
        newData.LoadMeteringDataFromFile (strFullFilePath);
        dataList.insert (dataList.begin () + nIdx, newData);

        UpdateHtmlView (false, true);
    }

    SetModified ();    
    HTREEITEM hNewItem = InsertCounterIntoTree (hItem, counter, nIdx);    

    
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       InsertCounterIntoTree
//
//  Description:
//
//  �������� ����� ������� � ������.
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
//  ������� �����������.
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

    UpdateHtmlView (false, true);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       EditCustomer
//
//  Description:
//
//  ������������� �����������.
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
        // �������������� ��������.
        //

        return;
    }

    if (customer == dlg.GetCustomer ())
    {
        //
        // ����������� �� ���������.
        //

        return;
    }

    //
    // ����������� ���������. �������� ��������� �� ������� � ������.
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

    UpdateHtmlView (false, true);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       DeleteCounter
//
//  Description:
//
//  ������� �������.
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

    UpdateHtmlView (true, true);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       EditCounter
//
//  Description:
//
//  ������������� �������.
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
        // �������������� ��������.
        //

        return;
    }

    if (counter == dlg.GetCounter ())
    {
        //
        // ������� �� ���������.
        //

        return;
    }

    //
    // C������ ���������. �������� ��������� �� ������� � ������.
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
    if (nCustomerIdx == m_DataMgr.GetCurrentCustomerIdx ())
    {
        CounterDataList &dataList = m_DataMgr.GetCounterData ();
        CounterData newData;
        newData.SetCounter (newCounter);
        CString strFullFilePath;
        GetProgramFolder (strFullFilePath);
        strFullFilePath += DEFAULT_RESULTS_FOLDER;
        CString strFileName;
        newCounter.GetFileName (strFileName);
        strFileName += L"."APP_RESULTS_EXT;
        strFullFilePath += strFileName;
        newData.LoadMeteringDataFromFile (strFullFilePath);
        dataList.insert (dataList.begin () + nIdx, newData);
    }

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

    if (nCustomerIdx == m_DataMgr.GetCurrentCustomerIdx ())
    {
        UpdateHtmlView (false, true);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ClearCustomers
//
//  Description:
//
//  �������� ���� ������������.
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
//  ���������� ������ �� ��������� ������ - ������� hItem.
//  ��� ���� �������, ��� ������ >= nBaseIdx, � ������ ���������� nDelta. 
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
//  ���������� ����������� ���� ������������ ��������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnViewCounterLog ()
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

        m_wndStatusBar.SetPaneText (0, L"�������� ������ ������������ ��������.");
    }

    CViewerDlg dlg (this);
    
    dlg.SetLogMsg (strLogMsg);
    dlg.DoModal ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnScanOptions
//
//  Description:
//
//  ���������� ����������� ������� �������� ���������� ������������.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnScanOptions ()
{
    CString strBaseFolder;
    GetProgramFolder (strBaseFolder);
    strBaseFolder += DEFAULT_BASE_FOLDER;
    CScanOptionsDlg dlg (strBaseFolder, this);
    dlg.DoModal ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       UpdateHtmlView
//
//  Description:
//
//  ���������� HTML view � ������ ����� ����.
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::UpdateHtmlView (bool bReload, bool bRenderXML)
{
    CString strBaseFolder;
    GetProgramFolder (strBaseFolder);
    CString strXMLFolderName (strBaseFolder);
    strXMLFolderName += DEFAULT_XML_FOLDER;
    CString strHTMLFolderName (strBaseFolder);
    strHTMLFolderName += DEFAULT_HTML_FOLDER;
    
    TreeItemType itemType;
    size_t nCustomerIdx = ~0;
    size_t nCounterIdx = ~0;
    CTreeCtrl& treeCtrl = m_pTreeView->GetTreeCtrl ();
    HTREEITEM hItem = treeCtrl.GetSelectedItem  ();
    if (NULL == hItem) 
    {
        return;
    }

    if (! ReadTreeItemInfo (hItem, itemType, nCustomerIdx, nCounterIdx))
    {
        return;
    }

    try
    {
        if (titCustomers == itemType)
        {
            m_DataMgr.SetCurrentCustomerIdx (m_DataMgr.m_nInvalidIdx);
            GenerateXMLFiles ();
            
            CString strHTMLFile = strHTMLFolderName + "summary.html";
            m_pHtmlView->Navigate2 (strHTMLFile, 
                                    navNoReadFromCache | navNoWriteToCache | navNoHistory);
            return;
        }

        //
        // �������� ����������� ������������.
        //
        
        if (bReload) m_DataMgr.SetCurrentCustomerIdx (m_DataMgr.m_nInvalidIdx);
        size_t nCurrentCustomerIdx = m_DataMgr.GetCurrentCustomerIdx ();
        if (nCurrentCustomerIdx != nCustomerIdx)
        {
            m_wndStatusBar.SetPaneText (0, L"�������� ����������� ������������ ���������...");
            m_DataMgr.SetCurrentCustomerIdx (nCustomerIdx, strBaseFolder + DEFAULT_RESULTS_FOLDER);
            m_wndStatusBar.SetPaneText (0, L"�������� ����������� ������������ ��������� ���������");
        }

        if (nCurrentCustomerIdx != nCustomerIdx) bRenderXML = true;

        //
        // ���������� XML ������
        //

        if (bRenderXML)
        {
            GenerateXMLFiles ();
        }

        //
        // ����� �������� ��������
        //

        MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
        if (NULL != xmlDocument)
        {
            const CounterDataList &counterDataList = m_DataMgr.GetCounterData ();
            CString strBuffer;
            tagAttribute attributes [] = 
            {
                {_bstr_t (L"idx"),       _variant_t ((unsigned) nCounterIdx)},
                {_bstr_t (L"base_name"), _variant_t (nCounterIdx <  counterDataList.size () ? (const wchar_t*) (counterDataList [nCounterIdx].GetCounter ().GetFileName (strBuffer)) : L"")},
            };
            size_t nAttributeCount = SIZE_OF (attributes);

            MSXML::IXMLDOMElementPtr ptrElement = 
                CreateXMLElement (xmlDocument,
                L"selected_counter",
                nAttributeCount,
                attributes);
            if (NULL != ptrElement)
            {
                if (! FAILED (xmlDocument->documentElement->appendChild (ptrElement)))
                {
                    _variant_t fileName (strXMLFolderName + L"counter.xml");
                    xmlDocument->save (fileName);
                }
            }
        }

        //
        // ���������� HTML view
        //

        CString strHTMLFile;
        switch (itemType)
        {
        case titCustomer:
            strHTMLFile = strHTMLFolderName + L"customer.html";
            break;
        case titCounter:
            strHTMLFile = strHTMLFolderName + L"counter.html";
            break;
        };

        m_pHtmlView->Navigate2 (strHTMLFile, 
                                navNoReadFromCache | navNoWriteToCache | navNoHistory);

    }
    catch (...)
    {
        MessageBox (L"��� �������� ������ �������� ����������� ������.", L"������", MB_ICONERROR);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateXMLFiles
//
//  Description:
//
//  ���������� XML ����� � ��������
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::GenerateXMLFiles ()
{
    ClearFiles ();
    CString strBaseFolder;
    GetProgramFolder (strBaseFolder);
    CString strXMLFolderName (strBaseFolder);
    strXMLFolderName += DEFAULT_XML_FOLDER;

    //
    // ����� �����
    //

    {
        CWaitCursor wait;
        CString strXMLFile = strXMLFolderName + "summary.xml";
        MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
        if (NULL != xmlDocument)
        {
            m_wndStatusBar.SetPaneText (0, L"������������� ������ ������...");
            MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateSummaryReport (xmlDocument); 
            if (NULL != ptrElement)
            {
                xmlDocument->documentElement->appendChild (ptrElement);
                m_wndStatusBar.SetPaneText (0, L"����� ����� ������� ������������");
                m_wndStatusBar.SetPaneText (0, L"���������� ������ ������...");
                _variant_t fileName (strXMLFile);
                HRESULT hr = xmlDocument->save (fileName);
                if (FAILED (hr)) 
                {
                    m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ����� �����");
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, L"����� ����� ������� �������");
                }
            }
            else
            {
                m_wndStatusBar.SetPaneText (0, L"�� ������� ������������� ����� �����");
            }
        }
        m_wndStatusBar.SetPaneText (0, L"����� ����� ������");
    }
    
    size_t nCustomerIdx = m_DataMgr.GetCurrentCustomerIdx ();
    if (nCustomerIdx == m_DataMgr.m_nInvalidIdx)
    {
        return;
    }
    
    //
    // ����� �� �����������
    //

    {
        CWaitCursor wait;
        CString strXMLFile = strXMLFolderName + "customer.xml";
        MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
        if (NULL != xmlDocument)
        {
            m_wndStatusBar.SetPaneText (0, L"������������� ������ �� �����������...");
            MSXML::IXMLDOMElementPtr ptrElement = 
                m_DataMgr.GenerateCustomerReport (xmlDocument, 
                                                  nCustomerIdx); 
            if (NULL != ptrElement)
            {
                xmlDocument->documentElement->appendChild (ptrElement);
                m_wndStatusBar.SetPaneText (0, L"����� �� ����������� ������� ������������");
                m_wndStatusBar.SetPaneText (0, L"���������� ������ �� �����������...");
                _variant_t fileName (strXMLFile);
                HRESULT hr = xmlDocument->save (fileName);
                if (FAILED (hr)) 
                {
                    m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ����� �� �����������");
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, L"����� �� ����������� ������� �������");
                }
            }
            else
            {
                m_wndStatusBar.SetPaneText (0, L"�� ������� ������������� ����� �� �����������");
            }
        }
        m_wndStatusBar.SetPaneText (0, L"����� �� ����������� ������");
    }

    //
    // ��������� ����� �� �����������������
    //

    {
        CWaitCursor wait;
        CString strXMLFile = strXMLFolderName + "customer_energy.xml";
        MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
        if (NULL != xmlDocument)
        {
            m_wndStatusBar.SetPaneText (0, L"������������� ������ �� ���������� �����������������...");
            MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCustomerEnergyReport (xmlDocument); 
            if (NULL != ptrElement)
            {
                xmlDocument->documentElement->appendChild (ptrElement);
                m_wndStatusBar.SetPaneText (0, L"����� �� ���������� ����������������� ������� ������������");
                m_wndStatusBar.SetPaneText (0, L"���������� ������ �� ���������� �����������������...");
                _variant_t fileName (strXMLFile);
                HRESULT hr = xmlDocument->save (fileName);
                if (FAILED (hr)) 
                {
                    m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ����� �� ���������� �����������������");
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, L"����� �� ���������� ����������������� ������� �������");
                }
            }
            else
            {
                m_wndStatusBar.SetPaneText (0, L"�� ������� ������������� ����� �� ���������� �����������������");
            }
        }
        m_wndStatusBar.SetPaneText (0, L"����� �� ���������� ����������������� ������");
    }

    //
    // ��������� ����� �� ��������
    //

    PowerUsageSummarizer summaryPowerReport;
    {
        CWaitCursor wait;
        CString strXMLFile = strXMLFolderName + "customer_power.xml";
        MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
        if (NULL != xmlDocument)
        {
            m_wndStatusBar.SetPaneText (0, L"������������� ������ �� ���������� ����������� ��������...");
            MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCustomerSummaryPowerReport (xmlDocument, summaryPowerReport); 
            if (NULL != ptrElement)
            {
                xmlDocument->documentElement->appendChild (ptrElement);
                m_wndStatusBar.SetPaneText (0, L"����� �� ���������� ����������� �������� ������� ������������");
                m_wndStatusBar.SetPaneText (0, L"���������� ������ �� ���������� ����������� ��������...");
                _variant_t fileName (strXMLFile);
                HRESULT hr = xmlDocument->save (fileName);
                if (FAILED (hr)) 
                {
                    m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ����� �� ���������� ����������� ��������");
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, L"����� �� ���������� ����������� �������� ������� �������");
                }
            }
            else
            {
                m_wndStatusBar.SetPaneText (0, L"�� ������� ������������� ����� �� ���������� ����������� ��������");
            }
        }
        m_wndStatusBar.SetPaneText (0, L"����� �� ���������� ����������� �������� ������");
    }
    
    //
    // ���������� ����� �� ��������
    //

    {
        CWaitCursor wait;
        CString strXMLFile = strXMLFolderName + "customer_power_month.xml";
        MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
        if (NULL != xmlDocument)
        {
            m_wndStatusBar.SetPaneText (0, L"������������� ������ �� ����������� ����������� ��������...");
            MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCustomerMonthlyPowerReport (xmlDocument, 
                                                                                                summaryPowerReport.GetStorage ()); 
            if (NULL != ptrElement)
            {
                xmlDocument->documentElement->appendChild (ptrElement);
                m_wndStatusBar.SetPaneText (0, L"����� �� ����������� ����������� �������� ������� ������������");
                m_wndStatusBar.SetPaneText (0, L"���������� ������ �� ����������� ����������� ��������...");
                _variant_t fileName (strXMLFile);
                HRESULT hr = xmlDocument->save (fileName);
                if (FAILED (hr)) 
                {
                    m_wndStatusBar.SetPaneText (0, L"�� ������� ��������� ����� �� ����������� ����������� ��������");
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, L"����� �� ����������� ����������� �������� ������� �������");
                }
            }
            else
            {
                m_wndStatusBar.SetPaneText (0, L"�� ������� ������������� ����� �� ����������� ����������� ��������");
            }
        }
        m_wndStatusBar.SetPaneText (0, L"����� �� ����������� ����������� ��������");
    }

    summaryPowerReport.Reset ();

    //
    // ������ �� ���������
    //

    m_wndStatusBar.SetPaneText (0, L"������ �� ���������...");
    const CounterDataList &counterData = m_DataMgr.GetCounterData ();
    size_t nCounterCount = counterData.size ();
    for (size_t nIdx = 0; nIdx < nCounterCount; ++ nIdx)
    {
        const CounterData &data = counterData [nIdx];
        CString strPrefix;
        data.GetCounter ().GetShortDisplayName (strPrefix);
        strPrefix += L": ";
        CString strCounterFileName;
        data.GetCounter ().GetFileName (strCounterFileName);        

        //
        // ����� �����
        //

        {
            CWaitCursor wait;
            CString strXMLFile = strXMLFolderName + strCounterFileName + L".xml";
            MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
            if (NULL != xmlDocument)
            {
                m_wndStatusBar.SetPaneText (0, strPrefix + L"������������� ������ ������...");
                MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCounterReport (xmlDocument, nIdx);
                if (NULL != ptrElement)
                {
                    xmlDocument->documentElement->appendChild (ptrElement);
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"����� ����� ������� ������������");
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"���������� ������ ������...");
                    _variant_t fileName (strXMLFile);             
                    HRESULT hr = xmlDocument->save (fileName);
                    if (FAILED (hr)) 
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ��������� ����� �����");
                    }
                    else
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"����� ����� ������� �������");
                    }
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ������������� ����� �����");
                }
            }
        }

        //
        // ������ �������
        //

        {
            CWaitCursor wait;
            CString strXMLFile = strXMLFolderName + strCounterFileName + L"_log.xml";
            MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
            if (NULL != xmlDocument)
            {
                m_wndStatusBar.SetPaneText (0, strPrefix + L"������������� ������� �������...");
                MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCounterEventLog (xmlDocument, nIdx);
                if (NULL != ptrElement)
                {
                    xmlDocument->documentElement->appendChild (ptrElement);
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"������ ������� ������� ������������");
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"���������� ������� �������...");
                    _variant_t fileName (strXMLFile);
                    HRESULT hr = xmlDocument->save (fileName);
                    if (FAILED (hr)) 
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ��������� ������ �������");
                    }
                    else
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"������ ������� ������� �������");
                    }
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ������������� ������ �������");
                }
            }
        }

        //
        // ����� �� �������
        //

        {
            CWaitCursor wait;
            CString strXMLFile = strXMLFolderName + strCounterFileName + L"_energy.xml";
            MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
            if (NULL != xmlDocument)
            {
                m_wndStatusBar.SetPaneText (0, strPrefix + L"������������� ������ �� �����������������...");
                MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCounterEnergyReport (xmlDocument, nIdx);
                if (NULL != ptrElement)
                {
                    xmlDocument->documentElement->appendChild (ptrElement);
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"����� �� ����������������� ������� ������������");
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"���������� ������ �� �����������������...");
                    _variant_t fileName (strXMLFile);
                    HRESULT hr = xmlDocument->save (fileName);
                    if (FAILED (hr)) 
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ��������� ����� �� �����������������");
                    }
                    else
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"����� �� ����������������� ������� �������");
                    }
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ������������� ����� �� �����������������");
                }
            }
        }

        //
        // ����� �� ��������
        //

        {
            CWaitCursor wait;
            CString strXMLFile = strXMLFolderName + strCounterFileName + L"_power.xml";
            MSXML::IXMLDOMDocumentPtr xmlDocument = CreateXMLDocument ();
            if (NULL != xmlDocument)
            {
                m_wndStatusBar.SetPaneText (0, strPrefix + L"������������� ������ �� ��������...");
                MSXML::IXMLDOMElementPtr ptrElement = m_DataMgr.GenerateCounterPowerReport (xmlDocument, nIdx);
                if (NULL != ptrElement)
                {
                    xmlDocument->documentElement->appendChild (ptrElement);
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"����� �� �������� ������� ������������");
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"���������� ������ �� ��������...");
                    _variant_t fileName (strXMLFile);
                    HRESULT hr = xmlDocument->save (fileName);
                    if (FAILED (hr)) 
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ��������� ����� �� ��������");
                    }
                    else
                    {
                        m_wndStatusBar.SetPaneText (0, strPrefix + L"����� �� �������� ������� �������");
                    }
                }
                else
                {
                    m_wndStatusBar.SetPaneText (0, strPrefix + L"�� ������� ������������� ����� �� ��������");
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ClearFiles
//
//  Description:
//
//  ������� ��� .XML ������
//
///////////////////////////////////////////////////////////////////////////////

void CMainFrame::ClearFiles ()
{
    CString strBaseFolder;
    GetProgramFolder (strBaseFolder);
    CString strXMLFolderName (strBaseFolder);
    strXMLFolderName += DEFAULT_XML_FOLDER;
    
    WIN32_FIND_DATA ffd;
    memset (&ffd, 0, sizeof (ffd));
    HANDLE handle = FindFirstFile (strXMLFolderName + L"*.xml", &ffd);
    if (NULL == handle) return;
    do
    {
        DeleteFile (strXMLFolderName + ffd.cFileName);   
    } 
    while (FindNextFile (handle, &ffd));

    FindClose (handle);
}