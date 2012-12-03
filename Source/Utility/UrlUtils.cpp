//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   UrlUtils.cpp
//    Author:      Streamlet
//    Create Time: 2012-08-05
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include "UrlUtils.h"
#include "Encoding.h"
#include <tchar.h>
#include <stdio.h>

xl::String UrlUtils::UrlEncode(const xl::String &strUrl, DWORD dwCodePage /*= CP_UTF8*/)
{
    xl::String strResult;
    xl::StringA strUrlA = Encoding::StringToAnsi(strUrl, dwCodePage);

    TCHAR szBuffer[4] = {};

    for (int i = 0; i < strUrlA.Length(); ++i)
    {
        xl::CharA ch = strUrlA[i];

        if ((ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z') ||
            (ch >= '0' && ch <= '9') ||
            ch == '-' ||
            ch == '.' ||
            ch == '_' ||
            ch == '~')
        {
            strResult.AppendBack(ch);
        }
        else
        {
            _stprintf_s(szBuffer, _T("%%%02X"), (0x000000ff & ch));
            strResult += szBuffer;
        }
    }

    return strResult;
}

xl::String UrlUtils::UrlDecode(const xl::String &strUrl, DWORD dwCodePage /*= CP_UTF8*/)
{
    xl::StringA strResultA;
    xl::StringA strUrlA = Encoding::StringToAnsi(strUrl, dwCodePage);

    for (int i = 0; i < strUrlA.Length(); ++i)
    {
        xl::CharA ch = strUrlA[i];
        
        if (ch != _T('%') || i + 2 >= strUrl.Length())
        {
            strResultA.AppendBack(ch);
            continue;
        }

        int nHigh = 0, nLow = 0;
        int nCount = sscanf_s(&strUrlA[i + 1], "%01X%01X", &nHigh, &nLow);

        if (nCount < 2)
        {
            strResultA.AppendBack(ch);
            continue;
        }

        xl::CharA chAppend = (xl::CharA)(nHigh * 0x10 + nLow);
        strResultA.AppendBack(chAppend);

        i += 2;
    }

    return Encoding::AnsiToUnicode(strResultA, dwCodePage);
}


void QueryStringCombinor::SetQueryString(const xl::String &strName, const xl::String &strValue)
{
    m_mapQueryStrings[strName] = strValue;
}

void QueryStringCombinor::RemoveQueryString(const xl::String &strName)
{
    m_mapQueryStrings.Delete(strName);
}

void QueryStringCombinor::RemoveQueryStrings()
{
    m_mapQueryStrings.Clear();
}

xl::String QueryStringCombinor::GetCombinedQueryString(DWORD dwCodePage /*= CP_UTF8*/)
{
    xl::String strQueryString;

    for (auto it = m_mapQueryStrings.Begin(); it != m_mapQueryStrings.End(); ++it)
    {
        if (it != m_mapQueryStrings.Begin())
        {
            strQueryString += _T("&");
        }

        strQueryString += UrlUtils::UrlEncode(it->Key);
        strQueryString += _T("=");
        strQueryString += UrlUtils::UrlEncode(it->Value, dwCodePage);
    }

    return strQueryString;
}


void UrlCombinor::SetMainUrl(const xl::String &strUrl)
{
    m_strUrl = strUrl;
}

void UrlCombinor::SetQueryStrings(const xl::String &strQueryStrings)
{
    m_strQueryStrings = strQueryStrings;
}

void UrlCombinor::SetFragmentId(const xl::String &strFragmentId)
{
    m_strFragmentId = strFragmentId;
}

xl::String UrlCombinor::GetCombinedUrl()
{
    xl::String strUrl = m_strUrl;

    if (!m_strQueryStrings.Empty())
    {
        strUrl += _T("?");
        strUrl += m_strQueryStrings;
    }

    if (!m_strFragmentId.Empty())
    {
        strUrl += _T("#");
        strUrl += m_strFragmentId;
    }

    return strUrl;
}


bool QueryStringSplitter::Split(const xl::String &strQueryStrings)
{
    m_mapQueryStrings.Clear();

    auto arrQueryStrings = strQueryStrings.Split(_T("&"));

    for (auto it = arrQueryStrings.Begin(); it != arrQueryStrings.End(); ++it)
    {
        auto arrKeyValue = it->Split(_T("="));

        if (arrKeyValue.Size() > 2)
        {
            return false;
        }

        xl::String strKey = arrKeyValue[0];
        xl::String strValue;

        if (arrKeyValue.Size() > 1)
        {
            strValue = arrKeyValue[1];
        }

        m_mapQueryStrings[strKey] = strValue;
    }

    return true;
}

xl::Map<xl::String, xl::String> QueryStringSplitter::GetKeyValues(DWORD dwCodePage /*= CP_UTF8*/)
{
    auto mapQueryStrings = m_mapQueryStrings;

    for (auto it = mapQueryStrings.Begin(); it != m_mapQueryStrings.End(); ++it)
    {
        it->Key   = UrlUtils::UrlDecode(it->Key, dwCodePage);
        it->Value = UrlUtils::UrlDecode(it->Value, dwCodePage);
    }

    return mapQueryStrings;
}

xl::String QueryStringSplitter::GetValue(const xl::String &strName, DWORD dwCodePage /*= CP_UTF8*/)
{
    auto it = m_mapQueryStrings.Find(UrlUtils::UrlEncode(strName));

    if (it == m_mapQueryStrings.End())
    {
        return _T("");
    }

    return UrlUtils::UrlDecode(it->Value, dwCodePage);
}


bool UrlSplitter::Split(const xl::String &strUrl)
{
    m_strUrl.Clear();
    m_strQueryStrings.Clear();
    m_strFragmentId.Clear();

    auto arrUrlFragmentId = strUrl.Split(_T("#"));

    if (arrUrlFragmentId.Size() > 2)
    {
        return false;
    }

    if (arrUrlFragmentId.Size() > 1)
    {
        m_strFragmentId = arrUrlFragmentId[1];
    }

    auto arrUrlParts = arrUrlFragmentId[0].Split(_T("?"));

    if (arrUrlParts.Size() > 2)
    {
        return false;
    }

    m_strUrl = arrUrlParts[0];

    if (arrUrlParts.Size() > 1)
    {
        m_strQueryStrings = arrUrlParts[1];
    }

    return true;
}

xl::String UrlSplitter::GetMainUrl()
{
    return m_strUrl;
}

xl::String UrlSplitter::GetQueryStrings()
{
    return m_strQueryStrings;
}

xl::String UrlSplitter::GetFragmentId()
{
    return m_strFragmentId;
}
