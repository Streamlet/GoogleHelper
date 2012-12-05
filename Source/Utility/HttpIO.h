//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   HttpIO.h
//    Author:      Streamlet
//    Create Time: 2010-10-26
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __HTTPIO_H_2059AD7B_5F50_4E99_AB5A_AEA1956C9EC6_INCLUDED__
#define __HTTPIO_H_2059AD7B_5F50_4E99_AB5A_AEA1956C9EC6_INCLUDED__


#include <xl/Containers/xlArray.h>
#include <xl/String/xlString.h>
#include <xl/Meta/xlFunction.h>
#include <Windows.h>
#include <WinHttp.h>


typedef xl::Function<void (xl::Array<BYTE> *)> HttpIOReader;
typedef xl::Function<void (LPCVOID lpBuffer, DWORD cbSize)> HttpIOWriter;

class HttpIO
{
public:
    HttpIO(LPCTSTR lpAgent = nullptr);
    ~HttpIO();

private:
    bool Initialize();
    void Release();

public:
    bool Connect(LPCTSTR lpHost, INTERNET_PORT nPort = INTERNET_DEFAULT_PORT);
    void Disconnect();

public:
    bool SendRequest(LPCTSTR lpVerb,
                     LPCTSTR lpPagePath,
                     LPCTSTR lpExtraHeader,
                     HANDLE  hEventCancel,
                     DWORD   dwCbDataSize = 0,
                     HttpIOReader fnRead = nullptr,
                     HttpIOWriter fnWrite = nullptr);

private:
    void CancelRequest();

private:
    xl::String m_strAgent;
    bool m_bSsl;

    HINTERNET m_hSession;
    HINTERNET m_hConnect;
    HINTERNET m_hRequest;
    HANDLE m_hHttpReady;
    HANDLE m_hHttpFailed;

    DWORD m_dwBytesAvailable;
    DWORD m_dwBytesRead;
    DWORD m_dwBytesWritten;

private:
    void CALLBACK WinHttpStatusCallback(HINTERNET hInternet,
                                        DWORD dwInternetStatus,
                                        LPVOID lpvStatusInformation,
                                        DWORD dwStatusInformationLength);

private:
    static void CALLBACK StaticWinHttpStatusCallback(HINTERNET hInternet,
                                                     DWORD_PTR dwContext,
                                                     DWORD dwInternetStatus,
                                                     LPVOID lpvStatusInformation,
                                                     DWORD dwStatusInformationLength);
};

#endif // #ifndef __HTTPIO_H_2059AD7B_5F50_4E99_AB5A_AEA1956C9EC6_INCLUDED__
