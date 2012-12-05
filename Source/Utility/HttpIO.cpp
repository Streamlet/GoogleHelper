//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   HttpIO.cpp
//    Author:      Streamlet
//    Create Time: 2010-10-26
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include "HttpIO.h"
#include <tchar.h>
#include <Loki/ScopeGuard.h>
#pragma comment(lib, "WinHttp.lib")

HttpIO::HttpIO(LPCTSTR lpAgent /*= nullptr*/) :
    m_hSession(nullptr),
    m_hConnect(nullptr),
    m_hRequest(nullptr),
    m_hHttpReady(nullptr),
    m_hHttpFailed(nullptr),
    m_dwBytesAvailable(0),
    m_dwBytesRead(0),
    m_dwBytesWritten(0)
{
    m_strAgent = lpAgent;
    m_bSsl = false;

    Initialize();
}

HttpIO::~HttpIO()
{
    Release();
}

bool HttpIO::Initialize()
{
    m_hHttpReady = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_hHttpReady == nullptr)
    {
        return FALSE;
    }

    m_hHttpFailed = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_hHttpFailed == nullptr)
    {
        return FALSE;
    }

    return true;
}

void HttpIO::Release()
{
    Disconnect();

    if (m_hHttpFailed != nullptr)
    {
        CloseHandle(m_hHttpFailed);
        m_hHttpFailed = nullptr;
    }

    if (m_hHttpReady != nullptr)
    {
        CloseHandle(m_hHttpReady);
        m_hHttpReady = nullptr;
    }
}

bool HttpIO::Connect(LPCTSTR lpHost, INTERNET_PORT nPort /*= INTERNET_DEFAULT_PORT*/)
{
    if (m_hSession != nullptr && m_hConnect != nullptr)
    {
        return true;
    }

    Disconnect();

    m_hSession = WinHttpOpen(m_strAgent.GetAddress(),
                              WINHTTP_ACCESS_TYPE_NO_PROXY,
                              WINHTTP_NO_PROXY_NAME,
                              WINHTTP_NO_PROXY_BYPASS,
                              WINHTTP_FLAG_ASYNC);

    if (m_hSession == nullptr)
    {
        return false;
    }

    WinHttpSetStatusCallback(m_hSession, StaticWinHttpStatusCallback, WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS, 0);

    m_hConnect = WinHttpConnect(m_hSession, lpHost, nPort, 0);

    if (m_hConnect == nullptr)
    {
        return false;
    }

    if (nPort == INTERNET_DEFAULT_HTTPS_PORT)
    {
        m_bSsl = true;
    }

    return true;
}

void HttpIO::Disconnect()
{
    CancelRequest();

    if (m_hSession == nullptr)
    {
        return;
    }

    if (m_hConnect != nullptr)
    {
        WinHttpCloseHandle(m_hConnect);
        m_hConnect = nullptr;
    }

    if (m_hSession != nullptr)
    {
        WinHttpSetStatusCallback(m_hSession, nullptr, WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS, 0);

        WinHttpCloseHandle(m_hSession);
        m_hSession = nullptr;
    }
}

bool HttpIO::SendRequest(LPCTSTR lpVerb,
                         LPCTSTR lpPagePath,
                         LPCTSTR lpExtraHeader,
                         HANDLE  hEventCancel,
                         DWORD   dwCbDataSize /*= 0*/,
                         HttpIOReader fnRead /*= nullptr*/,
                         HttpIOWriter fnWrite /*= nullptr*/)
{
    CancelRequest();

    m_hRequest = WinHttpOpenRequest(m_hConnect,
                                    lpVerb,
                                    lpPagePath,
                                    nullptr,
                                    WINHTTP_NO_REFERER,
                                    WINHTTP_DEFAULT_ACCEPT_TYPES,
                                    m_bSsl ? WINHTTP_FLAG_SECURE : 0);
    
    if (m_hRequest == nullptr)
    {
        return false;
    }

    LOKI_ON_BLOCK_EXIT_OBJ(*this, &HttpIO::CancelRequest);

    if (!WinHttpSendRequest(m_hRequest,
                            lpExtraHeader,
                            -1L,
                            WINHTTP_NO_REQUEST_DATA,
                            0,
                            dwCbDataSize,
                            (DWORD_PTR)this))
    {
        return false;
    }

    enum
    {
        EVENT_READY = 0,
        EVENT_FAILED,
        EVENT_CANCEL,
        EVENT_COUNT
    };

    HANDLE lpHandles[] = { m_hHttpReady, m_hHttpFailed, hEventCancel };
    DWORD dwWaitResult = WaitForMultipleObjects(EVENT_COUNT, lpHandles, FALSE, INFINITE);

    if (dwWaitResult != EVENT_READY)
    {
        return false;
    }

    DWORD dwCbDataRemain = dwCbDataSize;

    while (dwCbDataRemain > 0)
    {
        if (fnRead == nullptr)
        {
            return false;
        }

        xl::Array<BYTE> arrBuffer;
        fnRead(&arrBuffer);
        DWORD dwSize = arrBuffer.Size();

        if (dwSize > dwCbDataRemain)
        {
            return false;
        }

        DWORD dwRemain = dwSize;

        while (dwRemain > 0)
        {
            LPCVOID lpBuffer = &arrBuffer[0] + dwSize - dwRemain;
            DWORD dwToWrite = dwRemain;
                
            if (!WinHttpWriteData(m_hRequest, lpBuffer, dwToWrite, nullptr))
            {
                return false;
            }

            DWORD dwWaitResult = WaitForMultipleObjects(EVENT_COUNT, lpHandles, FALSE, INFINITE);

            if (dwWaitResult != EVENT_READY)
            {
                return false;
            }

            dwRemain -= m_dwBytesWritten;
        }

        dwCbDataRemain -= dwSize;
    }

    if (!WinHttpReceiveResponse(m_hRequest, nullptr))
    {
        return false;
    }

    dwWaitResult = WaitForMultipleObjects(EVENT_COUNT, lpHandles, FALSE, INFINITE);

    if (dwWaitResult != EVENT_READY)
    {
        return false;
    }

    if (fnWrite == nullptr)
    {
        return true;
    }

    const DWORD BUFFER_SIZE = 4096;
    BYTE BUFFER[BUFFER_SIZE];

    while (true)
    {
        if (!WinHttpQueryDataAvailable(m_hRequest, nullptr))
        {
            return false;
        }

        dwWaitResult = WaitForMultipleObjects(EVENT_COUNT, lpHandles, FALSE, INFINITE);

        if (dwWaitResult != EVENT_READY)
        {
            return false;
        }

        if (m_dwBytesAvailable == 0)
        {
            break;
        }

        while (m_dwBytesAvailable > 0)
        {
            DWORD dwBytesToRead = (m_dwBytesAvailable < BUFFER_SIZE ? m_dwBytesAvailable : BUFFER_SIZE);

            if (!WinHttpReadData(m_hRequest, BUFFER, dwBytesToRead, nullptr))
            {
                return false;
            }

            dwWaitResult = WaitForMultipleObjects(EVENT_COUNT, lpHandles, FALSE, INFINITE);

            if (dwWaitResult != EVENT_READY)
            {
                return false;
            }

            fnWrite(BUFFER, m_dwBytesRead);

            m_dwBytesAvailable -= m_dwBytesRead;
        }
    }

    return true;
}

void HttpIO::CancelRequest()
{
    if (m_hRequest != nullptr)
    {
        WinHttpCloseHandle(m_hRequest);
        m_hRequest = nullptr;
    }
}

void CALLBACK HttpIO::WinHttpStatusCallback(HINTERNET hInternet,
                                            DWORD dwInternetStatus,
                                            LPVOID lpvStatusInformation,
                                            DWORD dwStatusInformationLength)
{
    switch (dwInternetStatus)
    {
    case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
        SetEvent(m_hHttpReady);
        break;
    case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
        SetEvent(m_hHttpReady);
        break;
    case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
        m_dwBytesAvailable = *(DWORD *)lpvStatusInformation;
        SetEvent(m_hHttpReady);
        break;
    case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
        m_dwBytesRead = dwStatusInformationLength;
        SetEvent(m_hHttpReady);
        break;
    case WINHTTP_CALLBACK_STATUS_WRITE_COMPLETE:
        m_dwBytesWritten = *(DWORD *)lpvStatusInformation;
        SetEvent(m_hHttpReady);
        break;
    case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
        SetEvent(m_hHttpFailed);
        break;
    default:
        break;
    }
}

void CALLBACK HttpIO::StaticWinHttpStatusCallback(HINTERNET hInternet,
                                                  DWORD_PTR dwContext,
                                                  DWORD dwInternetStatus,
                                                  LPVOID lpvStatusInformation,
                                                  DWORD dwStatusInformationLength)
{
    ((HttpIO *)dwContext)->WinHttpStatusCallback(hInternet,
                                                 dwInternetStatus,
                                                 lpvStatusInformation,
                                                 dwStatusInformationLength);
}
