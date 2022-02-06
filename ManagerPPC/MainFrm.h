///////////////////////////////////////////////////////////////////////////////
//
//  File:           MainFrm.h
//
//  Facility:       ������� ������ ��������
//                  
//
//
//  Abstract:       ����� �������� ���� ����������
//
//
//  Environment:    MSVC 2005
//
//  Author:         ������ �.�.
//
//  Creation Date:  22-Jan-2007
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
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
// ��� �������� ������
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
    // �����������
    //

	CMainFrame();
	DECLARE_DYNCREATE (CMainFrame)

    //
    // ����������
    //
	
    virtual ~CMainFrame();

    //
    // ���������� true, ���� ������ ������������ ��� �������.
    //

    bool IsModified () const;

    //
    // �������� ������� ����, ��� ������ ������������ ��� �������.
    //

    void SetModified (bool bModified = true);

    //
    // ���������� ����������� "����� ��������� ����".
    //

    virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

    //
    // ��������������� ������.
    //

    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

    //
    //  ��������� ������ ������������.
    //

    void RefreshCustomersTree ();

    //
    // �������� ������ ������������.
    //

    bool LoadCustomers (wchar_t *wcsFileName = NULL);

    //
    // ���������� ������ ������������.
    //

    bool SaveCustomers (wchar_t *wcsFileName = NULL);

    //
    // ������ ���������� � �������� tree item. ���������� ��� item, � ����� 
    // ������� ������� �� �����������.
    //

    bool ReadTreeItemInfo (IN HTREEITEM hItem, 
        OUT TreeItemType &itemType, 
        OUT size_t &nCustomerIdx, 
        OUT size_t &nCounterIdx);

    //
    // �������� ������ �����������.
    //

    void AddCustomer ();

    //
    // ������� �����������.
    //

    void DeleteCustomer (HTREEITEM hItem, size_t nCustomerIdx);

    //
    // ������������� �����������.
    //

    void EditCustomer (HTREEITEM hItem, size_t nCustomerIdx);

    //
    // �������� ����� �������.
    //

    void AddCounter (HTREEITEM hItem, size_t nCustomerIdx);

    //
    // ������� �������.
    //

    void DeleteCounter (HTREEITEM hItem, size_t nCustomerIdx, size_t nCounterIdx);

    //
    // ������������� �������.
    //

    void EditCounter (HTREEITEM hItem, size_t nCustomerIdx, size_t nCounterIdx);

    //
    // �������� ���� ������������.
    //

    void ClearCustomers ();

    //
    // ���������� ������ �� ��������� ������ - ������� hItem.
    // ��� ���� �������, ��� ������ >= nBaseIdx, � ������ ���������� nDelta. 
    //

    void FixReferences (HTREEITEM hItem, size_t nBaseIdx, int nDelta);

    //
    // ���������� ��������� ���� "��� �����������".
    //

    void OnTreeCustomersSelected ();

    //
    // ���������� ��������� �����������.
    //

    void OnTreeCustomerSelected (size_t nCustomerIdx);

    //
    // ���������� ��������� ��������.
    //

    void OnTreeCounterSelected (size_t nCustomerIdx, size_t nCounterIdx);

    //
    // �������� ������ ����������� � ������.
    //

    HTREEITEM InsertCustomerIntoTree (size_t nCustomerIdx);

    //
    // �������� ����� ������� � ������.
    //

    HTREEITEM InsertCounterIntoTree (HTREEITEM hCustomer, 
        const Customer::Counter &counter, 
        size_t nCounterIdx);

protected:
	CCommandBar   m_wndCommandBar;      // ���� ��� ���������� ����.
    CStatusBar    m_wndStatusBar;       // ������ ���������. 
    CTreeView    *m_pTreeView;          // ��������� �� "���" ����������.
    CTreeViewPPC  m_wndView;            // "���" ����������
    CMenu         m_TreeDefaultMenu;    // ����������� ���� ��-���������.
    CMenu         m_TreeCustomerMenu;   // ����������� ���� ��� �������� "�����������".
    CMenu         m_TreeCounterMenu;    // ����������� ���� ��� �������� "�������".
    DataManager   m_DataMgr;            // ����� ���������� �������.
    bool          m_bIsModified;        // ������� ����, ��� ������ ������������ ����������.
protected:

    //
    // ���������� ����������� ������������ ����
    //

    afx_msg LRESULT OnMenu (WPARAM wParam, LPARAM lParam);
    
    //
    // ���������� �������� ����.
    //

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    //
    // ���������� ��������� ������.
    //

    afx_msg void OnSetFocus(CWnd *pOldWnd);

    //
    // ���������� ������������ ������ �� TreeView
    //

    afx_msg void OnTreeKeyDown (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result);

    //
    // ���������� ��������� ��������� � TreeView.
    //

    afx_msg void OnTreeSelChanged (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result);
    
    //
    // ���������� ������� "���������� ������ �����������".
    //

    afx_msg void OnRefreshTree ();

    //
    // ���������� ������� "��������� ������ �����������".
    //

    afx_msg void OnSaveTree ();

    //
    // � ����������� �� ��������� ��������� ������������ menu items.
    //

    afx_msg void OnUpdateMenuItem (CCmdUI *pCmdUI);

    //
    // ���������� ��������� ���� ����������.
    //

    afx_msg void OnClose();

    //
    // ���������� ���������� �����������.
    //

    afx_msg void OnAddCustomer ();

    //
    // ���������� ���������� ��������.
    //

    afx_msg void OnAddCounter ();

    //
    // ���������� �������� ���� ������������.
    //

    afx_msg void OnClearCustomers ();

    //
    // ���������� �������� ��������.
    //

    afx_msg void OnDeleteCounter ();

    //
    // ���������� �������� �����������.
    //

    afx_msg void OnDeleteCustomer ();

    //
    // ���������� �������������� ��������.
    //

    afx_msg void OnEditCounter ();

    //
    // ���������� �������������� �����������.
    //

    afx_msg void OnEditCustomer ();

    //
    // ��������� ����������� ���� ������������ ��������.
    //

    afx_msg void OnViewCounterLog ();

    //
    // ��������� ����������� ������� �������� ���������� ������������.
    //

    afx_msg void OnScanOptions ();
	
    //
    // ����� ���������
    //

	DECLARE_MESSAGE_MAP()
};

#endif //_MAINFRM_H_


