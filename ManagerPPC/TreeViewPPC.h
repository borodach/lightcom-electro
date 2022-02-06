///////////////////////////////////////////////////////////////////////////////
//
//  File:           TreeViewPPC.h
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
    // �����������
    //

	CTreeViewPPC ();
	DECLARE_DYNCREATE (CTreeViewPPC)

    //
    // ����������.
    //

    virtual ~CTreeViewPPC();

public:

    //
    // ����� ���������� ����� ��������� ����
    // 

	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

    //
    // �������� ����
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
    // ���������� ������� "�������� ����"
    //

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct );

    //
    // ���������� ������� "������� �����"
    //

	afx_msg void OnSetFocus (CWnd* pOldWnd );

    //
    // ���������� ������� "����������� ����"
    //

	afx_msg void OnDestroy ();

    //
    // ���������� ������� "������ ����� ������ ����"
    //

    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);

    //
    // ����� ���������
    //

    DECLARE_MESSAGE_MAP()
};

#endif //_TREEVIEWPPC_H_


