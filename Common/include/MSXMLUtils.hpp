///////////////////////////////////////////////////////////////////////////////
//
//  File:           MSXMLUtils.hpp
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
* $History: MSXMLUtils.hpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Common/include
 * Добавлены Source Safe comments
* 
*/

#ifndef _MSXMLUTILS_HPP_
#define _MSXMLUTILS_HPP_

#include <windows.h>
#include <comutil.h>
#import "msxml.dll"

//
// Описание атрибута
//

struct tagAttribute
{
    _bstr_t    strName;
    _variant_t value;
};

//
// Функция для создания XML элемента.
//   m_Document      - XML документ,
//   strElemName     - имя элемента,
//   nAttributeCount - количество элементов,
//   attributes      - описания атрибутов. Это контейнер структур 
//   tagAttribute
//

template <class TString, class TContainer> 
MSXML::IXMLDOMElementPtr CreateXMLElement (MSXML::IXMLDOMDocumentPtr xmlDocument,
                                           const TString &strElemName, 
                                           size_t nAttributeCount,
                                           const TContainer &attributes)
{
    try 
    {
        _bstr_t bstrName (strElemName);
        IXMLDOMElementPtr ptrNewElement (NULL);
        HRESULT hr = 0;
        ptrNewElement = xmlDocument->createElement (bstrName);
        if (NULL == ptrNewElement)
        {
            return NULL;
        }

        for (size_t nIdx = 0; nIdx < nAttributeCount; ++ nIdx)
        {
            const tagAttribute &tag = attributes [nIdx];
            HRESULT hr = ptrNewElement->setAttribute (tag.strName, 
                tag.value);
            if (FAILED (hr))
            {
                return NULL;
            }
        }

        return ptrNewElement;
    }
    catch (...)
    {
        return NULL;
    }
}

//
// Описание XML элемента
//

template <class TString, class TContainer> 
struct tagEntity
{
    const TString &strElemName;            // Название элемента
    MSXML::IXMLDOMElementPtr &parentElem;  // Родительский элемент
    MSXML::IXMLDOMElementPtr &newElem;     // Созданный элемент
    size_t nAttributeCount;                // Количество атрибутов
    const TContainer &attributes;          // Описане птрибутов

public:

    //
    // Создает элемент.
    //

    bool CreateXMLElement (MSXML::IXMLDOMDocumentPtr xmlDocument)
    {
        newElem = ::CreateXMLElement (xmlDocument,
            strElemName, 
            nAttributeCount,
            attributes);
        if (NULL == newElem)
        {
            return false;
        }

        if (parentElem != NULL)
        {
            HRESULT hr = parentElem->appendChild (newElem);
            if (FAILED (hr))
            {
                return false;
            }
        }

        return true;
    }
};

//
// Создает XML документ.
//

extern MSXML::IXMLDOMDocumentPtr CreateXMLDocument (LPCWSTR wcsRootName = L"root");

#endif //_MSXMLUTILS_HPP_