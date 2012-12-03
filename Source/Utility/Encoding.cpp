//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Encoding.cpp
//    Author:      Streamlet
//    Create Time: 2009-08-28
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#include "Encoding.h"

xl::StringW Encoding::AnsiToUnicode(const xl::StringA &strAnsi, DWORD dwCodePage /*= CP_ACP*/)
{
    int size = MultiByteToWideChar(dwCodePage, 0, strAnsi.GetAddress(), -1, NULL, 0);

    if (size == 0)
    {
        return L"";
    }

    LPWSTR szUnicode = new WCHAR[size];

    if (MultiByteToWideChar(dwCodePage, 0, strAnsi.GetAddress(), -1, szUnicode, size) == 0)
    {
        delete szUnicode;
        return L"";
    }

    xl::StringW ret = szUnicode;
    delete szUnicode;

    return ret;
}

xl::StringA Encoding::UnicodeToAnsi(const xl::StringW &strUnicode, DWORD dwCodePage /*= CP_ACP*/)
{
    int size = WideCharToMultiByte(dwCodePage, 0, strUnicode.GetAddress(), -1, NULL, 0, NULL, NULL);

    if (size == 0)
    {
        return "";
    }

    LPSTR szAnsi = new CHAR[size];

    if (WideCharToMultiByte(dwCodePage, 0, strUnicode.GetAddress(), -1, szAnsi, size, NULL, NULL) == 0)
    {
        delete szAnsi;
        return "";
    }

    xl::StringA ret = szAnsi;
    delete szAnsi;

    return ret;
}

xl::StringW Encoding::Utf8ToUnicode(const xl::StringA &strUtf8)
{
    return AnsiToUnicode(strUtf8, CP_UTF8);
}

xl::StringA Encoding::UnicodeToUtf8(const xl::StringW &strUnicode)
{
    return UnicodeToAnsi(strUnicode, CP_UTF8);
}

xl::StringA Encoding::StringToAnsi(const xl::String &str, DWORD dwCodePage /*= CP_ACP*/)
{
#ifdef _UNICODE
    return UnicodeToAnsi(str, dwCodePage);
#else
    return str;
#endif
}

xl::StringW Encoding::StringToUnicode(const xl::String &str)
{
#ifdef _UNICODE
    return str;
#else
    return AnsiToUnicode(str);
#endif
}

xl::StringA Encoding::StringToUtf8(const xl::String &str)
{
#ifdef _UNICODE
    return UnicodeToUtf8(str);
#else
    return UnicodeToUtf8(AnsiToUnicode(str));
#endif
}

xl::String  Encoding::AnsiToString(const xl::StringA &strAnsi, DWORD dwCodePage /*= CP_ACP*/)
{
#ifdef _UNICODE
    return AnsiToUnicode(strAnsi, dwCodePage);
#else
    return strAnsi;
#endif
}

xl::String  Encoding::UnicodeToString(const xl::StringW &strUnicode)
{
#ifdef _UNICODE
    return strUnicode;
#else
    return UnicodeToAnsi(strUnicode);
#endif
}

xl::String  Encoding::Utf8ToString(const xl::StringA &strUtf8)
{
#ifdef _UNICODE
    return Utf8ToUnicode(strUtf8);
#else
    return UnicodeToAnsi(Utf8ToUnicode(strUtf8));
#endif
}


