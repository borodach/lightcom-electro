///////////////////////////////////////////////////////////////////////////////
//
//  File:           MainFrm.h
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
* $History: MainFrm.h $
 * 
 * *****************  Version 7  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Manager
 * ��������� Source Safe comments
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
    // �����������.
    //

	CMainFrame();

    //
    // ����������.
    //

    virtual ~CMainFrame();

    //
    // ���������� true, ���� ������ ������������ ��� �������.
    //

    bool IsModified () const;

    //
    // ��������� ������� ����, ��� ������ ������������ ��� �������.
    //

    void SetModified (bool bModified = true);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

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
    // ��������� ������ ������������.
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

    //
    // ���������� HTML view � ������ ����� ����.
    //

    void UpdateHtmlView (bool bReload, bool bRenderXML);

    //
    // ���������� XML ����� � ��������
    //

    void GenerateXMLFiles ();

    //
    // ������� ��� .XML ������
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
    // ���������� ������� ����� �� TreeView.
    //

    afx_msg void OnTreeRightClick (UINT nCode, NMHDR *pNotifyStruct, LRESULT *result);

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

    DECLARE_MESSAGE_MAP()    
    DECLARE_DYNAMIC(CMainFrame)
};


