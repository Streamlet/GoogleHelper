//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   UrlUtils.h
//    Author:      Streamlet
//    Create Time: 2012-08-05
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __URLUTILS_H_50B9A6CC_3C27_4FA0_8B2D_F6586D1BD623_INCLUDED__
#define __URLUTILS_H_50B9A6CC_3C27_4FA0_8B2D_F6586D1BD623_INCLUDED__


#include <xl/Meta/xlUtility.h>
#include <xl/Containers/xlMap.h>
#include <xl/String/xlString.h>
#include <Windows.h>

class UrlUtils : public xl::NonInstantiable
{
public:
    static xl::String UrlEncode(const xl::String &strUrl, DWORD dwCodePage = CP_UTF8);
    static xl::String UrlDecode(const xl::String &strUrl, DWORD dwCodePage = CP_UTF8);
};

class QueryStringCombinor : public xl::NonCopyable
{
public:
    void SetQueryString(const xl::String &strName, const xl::String &strValue);
    void RemoveQueryString(const xl::String &strName);
    void RemoveQueryStrings();

public:
    xl::String GetCombinedQueryString(DWORD dwCodePage = CP_UTF8);

private:
    xl::Map<xl::String, xl::String> m_mapQueryStrings;
};

class UrlCombinor : public xl::NonCopyable
{
public:
    void SetMainUrl(const xl::String &strUrl);
    void SetQueryStrings(const xl::String &strQueryStrings);
    void SetFragmentId(const xl::String &strFragmentId);

public:
    xl::String GetCombinedUrl();

private:
    xl::String m_strUrl;
    xl::String m_strQueryStrings;
    xl::String m_strFragmentId;
};

class QueryStringSplitter : public xl::NonCopyable
{
public:
    bool Split(const xl::String &strQueryStrings);

public:
    xl::Map<xl::String, xl::String> GetKeyValues(DWORD dwCodePage = CP_UTF8);
    xl::String GetValue(const xl::String &strName, DWORD dwCodePage = CP_UTF8);

private:
    xl::Map<xl::String, xl::String> m_mapQueryStrings;
};

class UrlSplitter : public xl::NonCopyable
{
public:
    bool Split(const xl::String &strUrl);

public:
    xl::String GetMainUrl();
    xl::String GetQueryStrings();
    xl::String GetFragmentId();

private:
    xl::String m_strUrl;
    xl::String m_strQueryStrings;
    xl::String m_strFragmentId;
};

#endif // #ifndef __URLUTILS_H_50B9A6CC_3C27_4FA0_8B2D_F6586D1BD623_INCLUDED__
