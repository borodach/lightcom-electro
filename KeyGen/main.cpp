///////////////////////////////////////////////////////////////////////////////
//
//  File:           main.cpp
//
//  Facility:       ������� ������
//
//
//  Abstract:       ��������� �������� ��� ������� ������ ������ � 
//                  ������� ������ ���������
//
//  Environment:    VC++ 8.0
//
//  Author:         ������ �.�.
//
//  Creation Date:  02-07-2006
//
//  Copyright (C) OOO "�������", 2006. ��� ����� ��������.
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <windows.h>
#include "HardwareId.hpp"
#include "RegKeys.h"

std::string strHwIdFile ("HwId.txt");
std::string strScannerKeyFile ("BaseKey.txt");
std::string strInspectorKeyFile ("InspectorKey.txt");
std::string strManagerKeyFile ("ManagerKey.txt");

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       ShowUsage
//
//  Description:
//
//  ������� � stdout program usage
//
///////////////////////////////////////////////////////////////////////////////

void ShowUsage ()
{
    std::cout << "KeyGen.exe [<���� � ���������� ���������������> <���� � ������ ��� �������> <���� � ������ ��� ����������>]"
            << std::endl;
    std::cout << "��-��������� <���� � ���������� ���������������> - \"" << strHwIdFile << "\"\n"
               << "             <���� � ������ ��� �������> - \""<< strScannerKeyFile << "\"\n"
               << "             <���� � ������ ��� ����������> - \"" << strInspectorKeyFile << "\""
               << "             <���� � ������ ��� ��������� PPC> - \"" << strManagerKeyFile << "\""
               << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       GenerateKey
//
//  Description:
//
//  ���������� ����
//
///////////////////////////////////////////////////////////////////////////////

bool GenerateKey (HardwareId &hwId, const wchar_t *wcsComment, const char *szFileName)
{
    FILE *pFile = fopen (szFileName, "w");
    if (NULL == pFile)
    {
        std::cout << "�� ������� ������� ���� \"" << szFileName 
            << "\"." << std::endl;
        return false;    
    }

    unsigned char *szKey = hwId.GenerateKey (wcsComment);
    if (NULL == szKey)
    {
        fclose (pFile);
        std::cout << "������ ��� ������������� �����."
            << std::endl;

        return false;
    }

    fputs ((const char*) szKey, pFile);
    delete [] szKey;
    fclose (pFile);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Function:       wmain
//
//  Description:
//
//  ����� �����
//
///////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv [])
{
    SetConsoleOutputCP (1251);

    /*
    HardwareId test;
    wchar_t *wcsTest = new wchar_t [5];
    wcscpy (wcsTest, L"Test");
    test.Attatch (wcsTest);
    unsigned char * szEncoded = test.Encode ();
    size_t nLen = strlen ((const char*) szEncoded);
    test.Reset ();
    test.Decode (szEncoded);
    wchar_t * szDecoded = test.GetHardwareId ();
    test.Detatch ();
    return 0;
    */
    if (argc > 1)
    {
        if (strcmp (argv [1], "/?") == 0 ||
            strcmp (argv [1], "\\?") == 0 ||
            strcmp (argv [1], "-?") == 0)
        {
            ShowUsage ();

            return 0;
        }
        strHwIdFile = argv [1];
    }

    if (argc > 2)
    {
        strScannerKeyFile = argv [2];
    }

    if (argc > 3)
    {
        strInspectorKeyFile = argv [3];
    }

    if (argc > 4)
    {
        strManagerKeyFile = argv [4];
    }

    FILE *pFile = fopen (strHwIdFile.c_str (), "r");
    if (NULL == pFile)
    {
        std::cout << "�� ������� ������� ���� \"" << strHwIdFile.c_str () 
            << "\"." << std::endl;
        return 1;    
    }

    char szBuf [1024];
    memset (szBuf, 0, sizeof (szBuf));
    fgets (szBuf, sizeof (szBuf), pFile);
    fclose (pFile);

    HardwareId hwId;
    if (! hwId.Decode ((const unsigned char*) szBuf))
    {
        std::cout << "�� ������� ������������ ���������� �������������." 
            << std::endl;
        return 1;    
    }


    if (! GenerateKey (hwId, ELECTRO_SCANNER_KEY, strScannerKeyFile.c_str ()))
    {
        std::cout << "�� ������� ������������� ���� ��� �������." 
            << std::endl;
    }
    else
    {
        std::cout << "���� ��� ������� ������� ������� � \"" << strScannerKeyFile
            << "\"."
            << std::endl;
    }

    if (! GenerateKey (hwId, ELECTRO_INSPECTOR_KEY, strInspectorKeyFile.c_str ()))
    {
        std::cout << "�� ������� ������������� ���� ��� ����������." 
            << std::endl;
    }
    else
    {
        std::cout << "���� ��� ���������� ������� ������� � \"" << strInspectorKeyFile
            << "\"."
            << std::endl;
    }


    if (! GenerateKey (hwId, ELECTRO_MANAGER_KEY, strManagerKeyFile.c_str ()))
    {
        std::cout << "�� ������� ������������� ���� ��� ��������� PPC." 
            << std::endl;
    }
    else
    {
        std::cout << "���� ��� ��������� PPC ������� ������� � \"" << strManagerKeyFile
            << "\"."
            << std::endl;
    }

    return 0;
}