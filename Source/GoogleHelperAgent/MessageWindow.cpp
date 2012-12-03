//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   MessageWindow.cpp
//    Author:      Streamlet
//    Create Time: 2012-12-03
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include "MessageWindow.h"
#include "resource.h"

enum
{
    MYWM_NOTIFYICON = (WM_USER + 1)
};

MessageWindow::MessageWindow()
{
    ZeroMemory(&m_nid, sizeof(m_nid));

    AppendMsgHandler(WM_CREATE,         MsgHandler(this, &MessageWindow::OnCreate));
    AppendMsgHandler(WM_DESTROY,        MsgHandler(this, &MessageWindow::OnDestroy));
    AppendMsgHandler(MYWM_NOTIFYICON,   MsgHandler(this, &MessageWindow::OnTrayIcon));
}

MessageWindow::~MessageWindow()
{

}

void MessageWindow::ReportUrl(LPCTSTR lpszUrl)
{
    m_nid.uFlags = NIF_INFO;
    _tcsncpy_s(m_nid.szInfoTitle, _T("GoogleHelper"), ARRAYSIZE(m_nid.szInfoTitle));
    _tcsncpy_s(m_nid.szInfo, lpszUrl, ARRAYSIZE(m_nid.szInfo));
    m_nid.uTimeout = 1000;
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

LRESULT MessageWindow::OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    m_nid.cbSize = sizeof(m_nid);
    m_nid.hWnd = m_hWnd;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_nid.uCallbackMessage = MYWM_NOTIFYICON;
    m_nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRAY));
    _tcsncpy_s(m_nid.szTip, _T("Google Helper Agent"), ARRAYSIZE(m_nid.szTip));
    Shell_NotifyIcon(NIM_ADD, &m_nid);

    return 0;
}

LRESULT MessageWindow::OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);

    return 0;
}

LRESULT MessageWindow::OnTrayIcon(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return 0;
}
