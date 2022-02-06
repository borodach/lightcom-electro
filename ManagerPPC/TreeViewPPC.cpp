///////////////////////////////////////////////////////////////////////////////
//
//  File:           TreeViewPPC.cpp
//
//  Facility:       ЛайтКом Энерго Менеджер, версия для Windows CE
//                  
//
//
//  Abstract:       TreeView, в котором исправляется ошибка MFC в обработке
//                  tap and hold.
//
//
//  Environment:    MSVC 2005
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  23-Jan-2007
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
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
 * Исправленный TreeView
*/

#include "stdafx.h"
#include "TreeViewPPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTreeViewPPC

IMPLEMENT_DYNCREATE(CTreeViewPPC, CTreeView)

//
// Карта сообщений
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
//  Конструктор
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
//  Деструктор
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
//  Метод вызывается перед созданием окна
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
//  Создание окна
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
//  Обработчик события "получен фокус"
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
//  Обработчик события "уничтожение окна"
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
//  Обработчик события "создание окна"
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
//  Обработчик события "нажата левая кнопка мыши"
//
///////////////////////////////////////////////////////////////////////////////

void CTreeViewPPC::OnLButtonDown (UINT nFlags, CPoint point)
{
    Default ();
    //CTreeView::OnLButtonDown(nFlags, point);
}