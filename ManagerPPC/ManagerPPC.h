///////////////////////////////////////////////////////////////////////////////
//
//  File:           ManagerPPC.h
//
//  Facility:       ������� ������ ��������
//                  
//
//
//  Abstract:       ����� ����������
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
* $History: ManagerPPC.h $
 * 
 * *****************  Version 1  *****************
 * User: Sergey       Date: 4.03.07    Time: 12:18
 * Created in $/LightCom/C++/Electro/ManagerPPC/ManagerPPC
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 26.01.07   Time: 15:41
 * Updated in $/PPCMercury.root/ManagerPPC
*/

#ifndef _MANAGERPPC_H_
#define _MANAGERPPC_H_

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resourceppc.h"

class CManagerPPCApp : public CWinApp
{
public:

    //
    // �����������
    //

	CManagerPPCApp ();

    //
    // ������������� ���������� ����������
    //

    virtual BOOL InitInstance ();

protected:
	
    //
    // ���������� ������� "� ���������"
    //

    afx_msg void OnAppAbout();

    //
    // ����� ���������
    //

	DECLARE_MESSAGE_MAP()
};

//
// ��������� ����������
//

extern CManagerPPCApp theApp;

#endif //_MANAGERPPC_H_