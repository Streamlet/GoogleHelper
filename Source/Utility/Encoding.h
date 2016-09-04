//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Encoding.h
//    Author:      Streamlet
//    Create Time: 2009-08-28
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __ENCODING_H_818FBE13_C7B5_4AB2_9B4B_7988CEBBE6A6_INCLUDED__
#define __ENCODING_H_818FBE13_C7B5_4AB2_9B4B_7988CEBBE6A6_INCLUDED__

#include <Windows.h>
#include <xl/Common/String/xlString.h>

class Encoding
{
public:
    static xl::StringW AnsiToUnicode(const xl::StringA &strAnsi, DWORD dwCodePage = CP_ACP);
    static xl::StringA UnicodeToAnsi(const xl::StringW &strUnicode, DWORD dwCodePage = CP_ACP);
    static xl::StringW Utf8ToUnicode(const xl::StringA &strUtf8);
    static xl::StringA UnicodeToUtf8(const xl::StringW &strUnicode);

    static xl::StringA StringToAnsi(const xl::String &str, DWORD dwCodePage = CP_ACP);
    static xl::StringW StringToUnicode(const xl::String &str);
    static xl::StringA StringToUtf8(const xl::String &str);

    static xl::String AnsiToString(const xl::StringA &strAnsi, DWORD dwCodePage = CP_ACP);
    static xl::String UnicodeToString(const xl::StringW &strUnicode);
    static xl::String Utf8ToString(const xl::StringA &strUtf8);

private:
    Encoding();
};

#endif // #ifndef __ENCODING_H_818FBE13_C7B5_4AB2_9B4B_7988CEBBE6A6_INCLUDED__
