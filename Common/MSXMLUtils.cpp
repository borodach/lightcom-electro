///////////////////////////////////////////////////////////////////////////////
//
//  File:           MSXMLUtils.cpp
//
//  Facility:       Работа с XML
//                  
//
//
//  Abstract:       Функции и шаблоны для работы с XML документами.
//
//
//  Environment:    MSVC 2005
//
//  Author:         Зайцев С.А.
//
//  Creation Date:  25-Sep-2006
//
//  Copyright (C) OOO "ЛайтКом", 2006. Все права защищены.
//
///////////////////////////////////////////////////////////////////////////////

/*
* $History: MSXMLUtils.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Common
 * Добавлены Source Safe comments
* 
*/

#include "MSXMLUtils.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       CreateXMLDocument
//
//  Description:
//
//  Создает XML документ
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