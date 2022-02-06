///////////////////////////////////////////////////////////////////////////////
//
//  File:           TreeViewPPC.cpp
//
//  Facility:       ������� ������ ��������, ������ ��� Windows CE
//                  
//
//
//  Abstract:       TreeView, � ������� ������������ ������ MFC � ���������
//                  tap and hold.
//
//
//  Environment:    MSVC 2005
//
//  Author:         ������ �.�.
//
//  Creation Date:  23-Jan-2007
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: TreeViewPPC.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:43
 * Created in $/PPCMercury.root/ManagerPPC
 * ������������ TreeView
*/

#include "stdafx.h"
#include "TreeViewPPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTreeViewPPC

IMPLEMENT_DYNCREATE(CTreeViewPPC, CTreeView)

//
// ����� ���������
//

BEGIN_MESSAGE_MAP(CTreeViewPPC, CTreeView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CTreeViewPPC
//
//  Description:
//
//  �����������
//
///////////////////////////////////////////////////////////////////////////////

CTreeViewPPC::CTreeViewPPC ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ~CTreeViewPPC
//
//  Description:
//
//  ����������
//
///////////////////////////////////////////////////////////////////////////////

CTreeViewPPC::~CTreeViewPPC ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       PreCreateWindow
//
//  Description:
//
//  ����� ���������� ����� ��������� ����
//
///////////////////////////////////////////////////////////////////////////////

BOOL CTreeViewPPC::PreCreateWindow (CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnInitialUpdate
//
//  Description:
//
//  Called first time after construct
//
///////////////////////////////////////////////////////////////////////////////

void CTreeViewPPC::OnInitialUpdate ()
{
	CTreeView::OnInitialUpdate ();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnCreate
//
//  Description:
//
//  �������� ����
//
///////////////////////////////////////////////////////////////////////////////

int CTreeViewPPC::OnCreate (LPCREATESTRUCT lpCreateStruct )
{
	int iResult = CTreeView::OnCreate(lpCreateStruct);

	if(iResult == 0)
		SHDoneButton(TRUE);

	return iResult;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnSetFocus
//
//  Description:
//
//  ���������� ������� "������� �����"
//
///////////////////////////////////////////////////////////////////////////////

void CTreeViewPPC::OnSetFocus (CWnd* pOldWnd)
{
	CTreeView::OnSetFocus (pOldWnd);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnDestroy
//
//  Description:
//
//  ���������� ������� "����������� ����"
//
///////////////////////////////////////////////////////////////////////////////

void CTreeViewPPC::OnDestroy ()
{
	CTreeView::OnDestroy ();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       Create
//
//  Description:
//
//  ���������� ������� "�������� ����"
//
///////////////////////////////////////////////////////////////////////////////

BOOL CTreeViewPPC::Create (LPCTSTR lpszClassName, 
                           LPCTSTR lpszWindowName, 
                           DWORD dwStyle, 
                           const RECT& rect, 
                           CWnd* pParentWnd, 
                           UINT nID, 
                           CCreateContext* pContext) 
{
	return CTreeView::Create (lpszClassName, 
                         lpszWindowName, 
                         dwStyle, 
                         rect, 
                         pParentWnd, 
                         nID, 
                         pContext);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       OnLButtonDown
//
//  Description:
//
//  ���������� ������� "������ ����� ������ ����"
//
///////////////////////////////////////////////////////////////////////////////

void CTreeViewPPC::OnLButtonDown (UINT nFlags, CPoint point)
{
    Default ();
    //CTreeView::OnLButtonDown(nFlags, point);
}