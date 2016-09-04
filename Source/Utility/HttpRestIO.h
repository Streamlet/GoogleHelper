//--------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   HttpRestIO.h
//    Author:      Streamlet
//    Create Time: 2012-11-25
//    Description: 
//
//    Version history:
//
//
//--------------------------------------------------------------------

#ifndef __HTTPRESTIO_H_B162F85F_F581_408C_A27C_5183CBA60367_INCLUDED__
#define __HTTPRESTIO_H_B162F85F_F581_408C_A27C_5183CBA60367_INCLUDED__


#include <xl/Common/Containers/xlArray.h>
#include <xl/Common/String/xlString.h>
#include <Windows.h>

class HttpRestIO
{
public:
    HttpRestIO(LPCTSTR lpAgent = nullptr);
    ~HttpRestIO();

public:
    bool SendRequest(LPCTSTR lpMethod,
                     LPCTSTR lpUrl,
                     LPCTSTR lpHeaders,
                     LPVOID lpData,
                     DWORD dwDataSize,
                     HANDLE hEventCancel,
                     xl::Array<BYTE> *pContent);

    void HttpIOCallback(LPCVOID lpBuffer, DWORD cbSize, xl::Array<BYTE> *pArrayOut);

private:
    xl::String m_strAgent;
};

#endif // #ifndef __HTTPRESTIO_H_B162F85F_F581_408C_A27C_5183CBA60367_INCLUDED__
