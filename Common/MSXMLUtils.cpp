///////////////////////////////////////////////////////////////////////////////
//
//  File:           MSXMLUtils.cpp
//
//  Facility:       ������ � XML
//                  
//
//
//  Abstract:       ������� � ������� ��� ������ � XML �����������.
//
//
//  Environment:    MSVC 2005
//
//  Author:         ������ �.�.
//
//  Creation Date:  25-Sep-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MSXMLUtils.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Common
 * ��������� Source Safe comments
* 
*/

#include "MSXMLUtils.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateXMLDocument
//
//  Description:
//
//  ������� XML ��������
//
///////////////////////////////////////////////////////////////////////////////

MSXML::IXMLDOMDocumentPtr CreateXMLDocument (LPCWSTR wcsRootName/* = L"root"*/)
{
    try
    {
        MSXML::IXMLDOMDocumentPtr xmlDocument;
        HRESULT hr = xmlDocument.CreateInstance (CLSID_DOMDocument);
        if (FAILED (hr))
        {
            return NULL;
        }

        MSXML::IXMLDOMProcessingInstructionPtr pi = xmlDocument->createProcessingInstruction (L"xml", L"version=\"1.0\" encoding=\"UTF-8\"");
        if (pi == NULL)
        {
            return NULL;
        }
        MSXML::IXMLDOMElementPtr root = xmlDocument->createElement (_bstr_t (wcsRootName));
        if (root == NULL)
        {
            return NULL;
        }
        xmlDocument->appendChild (pi);
        xmlDocument->putref_documentElement (root);

        return xmlDocument;
    }
    catch (...)
    {
    }
    return NULL; 
}