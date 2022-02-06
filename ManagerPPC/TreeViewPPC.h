///////////////////////////////////////////////////////////////////////////////
//
//  File:           TreeViewPPC.h
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
* $History: TreeViewPPC.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:43
 * Created in $/PPCMercury.root/ManagerPPC
*/

#ifndef _TREEVIEWPPC_H_
#define _TREEVIEWPPC_H_

class CTreeViewPPC : public CTreeView
{
public:

    //
    // Конструктор
    //

	CTreeViewPPC ();
	DECLARE_DYNCREATE (CTreeViewPPC)

    //
    // Деструктор.
    //

    virtual ~CTreeViewPPC();

public:

    //
    // Метод вызывается перед созданием окна
    // 

	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

    //
    // Создание окна
    //

	virtual BOOL Create (LPCTSTR lpszClassName, 
                         LPCTSTR lpszWindowName, 
                         DWORD dwStyle, 
                         const RECT& rect, 
                         CWnd* pParentWnd, 
                         UINT nID, 
                         CCreateContext* pContext = NULL);

protected:

	//
    // Called first time after construct
    //

    virtual void OnInitialUpdate ();

    //
    // Обработчик события "создание окна"
    //

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct );

    //
    // Обработчик события "получен фокус"
    //

	afx_msg void OnSetFocus (CWnd* pOldWnd );

    //
    // Обработчик события "уничтожение окна"
    //

	afx_msg void OnDestroy ();

    //
    // Обработчик события "нажата левая кнопка мыши"
    //

    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);

    //
    // Карта сообщений
    //

    DECLARE_MESSAGE_MAP()
};

#endif //_TREEVIEWPPC_H_


