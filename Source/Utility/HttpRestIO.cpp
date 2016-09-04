//--------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   HttpRestIO.cpp
//    Author:      Streamlet
//    Create Time: 2012-11-25
//    Description: 
//
//    Version history:
//
//
//--------------------------------------------------------------------



#include "HttpRestIO.h"
#include "HttpIO.h"
#include <xl/Common/Meta/xlBind.h>

HttpRestIO::HttpRestIO(LPCTSTR lpAgent /* = nullptr */)
    : m_strAgent(lpAgent)
{

}

HttpRestIO::~HttpRestIO()
{

}

bool HttpRestIO::SendRequest(LPCTSTR lpMethod,
                             LPCTSTR lpUrl,
                             LPCTSTR lpHeaders,
                             LPVOID lpData,
                             DWORD dwDataSize,
                             HANDLE hEventCancel,
                             xl::Array<BYTE> *pContent)
{
    URL_COMPONENTS urlComp = { sizeof(URL_COMPONENTS) };

    urlComp.dwHostNameLength  = (DWORD)-1;
    urlComp.dwUrlPathLength   = (DWORD)-1;
    urlComp.dwExtraInfoLength = (DWORD)-1;

    if (!WinHttpCrackUrl(lpUrl, 0, 0, &urlComp))
    {
        return false;
    }

    xl::String strHostName = xl::String(urlComp.lpszHostName, urlComp.dwHostNameLength);
    xl::String strPagePath = xl::String(urlComp.lpszUrlPath, urlComp.dwUrlPathLength) +
                             xl::String(urlComp.lpszExtraInfo, urlComp.dwExtraInfoLength);

    HttpIO http(m_strAgent);

    if (!http.Connect(strHostName, urlComp.nScheme == INTERNET_SCHEME_HTTPS ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT))
    {
        return false;
    }

    if (!http.SendRequest(lpMethod,
                          strPagePath,
                          lpHeaders,
                          hEventCancel,
                          dwDataSize,
                          xl::Bind(this, &HttpRestIO::HttpIOCallback, lpData, dwDataSize, xl::_1),
                          xl::Bind(this, &HttpRestIO::HttpIOCallback, xl::_1, xl::_2, pContent)))
    {
        return false;
    }

    return true;
}

void HttpRestIO::HttpIOCallback(LPCVOID lpBuffer, DWORD cbSize, xl::Array<BYTE> *pArrayOut)
{
    if (pArrayOut == nullptr)
    {
        return;
    }

    pArrayOut->Insert(pArrayOut->Size(), (const BYTE *)lpBuffer, cbSize);
}
