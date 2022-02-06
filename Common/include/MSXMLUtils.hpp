///////////////////////////////////////////////////////////////////////////////
//
//  File:           MSXMLUtils.hpp
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
* $History: MSXMLUtils.hpp $
 * 
 * *****************  Version 2  *****************
 * User: Sergey       Date: 23.11.06   Time: 5:53
 * Updated in $/PPCMercury.root/Common/include
 * ��������� Source Safe comments
* 
*/

#ifndef _MSXMLUTILS_HPP_
#define _MSXMLUTILS_HPP_

#include <windows.h>
#include <comutil.h>
#import "msxml.dll"

//
// �������� ��������
//

struct tagAttribute
{
    _bstr_t    strName;
    _variant_t value;
};

//
// ������� ��� �������� XML ��������.
//   m_Document      - XML ��������,
//   strElemName     - ��� ��������,
//   nAttributeCount - ���������� ���������,
//   attributes      - �������� ���������. ��� ��������� �������� 
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
// �������� XML ��������
//

template <class TString, class TContainer> 
struct tagEntity
{
    const TString &strElemName;            // �������� ��������
    MSXML::IXMLDOMElementPtr &parentElem;  // ������������ �������
    MSXML::IXMLDOMElementPtr &newElem;     // ��������� �������
    size_t nAttributeCount;                // ���������� ���������
    const TContainer &attributes;          // ������� ���������

public:

    //
    // ������� �������.
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
// ������� XML ��������.
//

extern MSXML::IXMLDOMDocumentPtr CreateXMLDocument (LPCWSTR wcsRootName = L"root");

#endif //_MSXMLUTILS_HPP_