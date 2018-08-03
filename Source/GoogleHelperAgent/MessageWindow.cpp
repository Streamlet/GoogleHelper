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


#include "../GlobalDef.h"
#include "MessageWindow.h"
#include "AboutDialog.h"
#include "resource.h"
#include <xl/Common/String/xlString.h>
#include <xl/Common/String/xlEncoding.h>
#include "../Utility/Log.h"
#include "../Utility/HttpRestIO.h"
#include <json/json.h>
#include <tchar.h>
#pragma comment(lib, "Version.lib")

enum
{
    MYWM_NOTIFYICON = (WM_USER + 1)
};

enum
{
    ID_MENU_UPDATE,
    ID_MENU_ABOUT,
    ID_MENU_WEBSITE,
    ID_MENU_MAX
};

LPCTSTR MENU_TEXT[] =
{
    _T("检查新版本"),
    _T("关于 ") PRODUCT_NAME,
    _T("访问溪流软件工作室网站")
};

#define MESSAGE_NAVIGATE            (_T("本次跳转由 ") PRODUCT_NAME _T("引导。\r\n网址：%s\r\n"))

#define MESSAGE_UPDATE_CAPTION      _T("软件更新")
#define MESSAGE_UPDATE_LATEST       _T("当前版本 %s 已经是最新版本。")
#define MESSAGE_UPDATE_NEW_VERSION  _T("检查到新版本 %s，要立即下载吗？")

MessageWindow::MessageWindow()
{
    ZeroMemory(&m_nid, sizeof(m_nid));

    AppendMsgHandler(WM_CREATE,         MsgHandler(this, &MessageWindow::OnCreate));
    AppendMsgHandler(WM_DESTROY,        MsgHandler(this, &MessageWindow::OnDestroy));
    AppendMsgHandler(MYWM_NOTIFYICON,   MsgHandler(this, &MessageWindow::OnTrayIcon));

    AppendMenuCommandMsgHandler(ID_MENU_UPDATE,  CommandMsgHandler(this, &MessageWindow::OnMenuUpdate));
    AppendMenuCommandMsgHandler(ID_MENU_ABOUT,   CommandMsgHandler(this, &MessageWindow::OnMenuAbout));
    AppendMenuCommandMsgHandler(ID_MENU_WEBSITE, CommandMsgHandler(this, &MessageWindow::OnMenuWebsite));
}

MessageWindow::~MessageWindow()
{

}

void MessageWindow::ReportUrl(LPCTSTR lpszUrl)
{
    m_nid.uFlags = NIF_INFO;
    _tcsncpy_s(m_nid.szInfoTitle, PRODUCT_NAME, ARRAYSIZE(m_nid.szInfoTitle));
    _stprintf_s(m_nid.szInfo, MESSAGE_NAVIGATE, lpszUrl);
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
    _tcsncpy_s(m_nid.szTip, PRODUCT_NAME, ARRAYSIZE(m_nid.szTip));
    Shell_NotifyIcon(NIM_ADD, &m_nid);

    m_menuTray.CreatePopup();
    m_menuTray.AppendMenu(MF_STRING, ID_MENU_UPDATE, MENU_TEXT[ID_MENU_UPDATE]);
    m_menuTray.AppendMenu(MF_SEPARATOR, 0, nullptr);
    m_menuTray.AppendMenu(MF_STRING, ID_MENU_ABOUT, MENU_TEXT[ID_MENU_ABOUT]);
    m_menuTray.AppendMenu(MF_STRING, ID_MENU_WEBSITE, MENU_TEXT[ID_MENU_WEBSITE]);

    m_tUpdateThread.Create(UpdateThreadType::ProcType(this, &MessageWindow::UpdateThread), 30000);

    return 0;
}

LRESULT MessageWindow::OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);

    return 0;
}

LRESULT MessageWindow::OnTrayIcon(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    switch(lParam)
    {
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONUP:
        SetForegroundWindow(m_hWnd);
        {
            POINT point = {};
            GetCursorPos(&point);
            m_menuTray.TrackPopupMenu(TPM_RIGHTALIGN, point.x, point.y, m_hWnd);
        }
        break;

    default:
        break;
    }

    return 0;
}

LRESULT MessageWindow::OnMenuUpdate(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    m_tUpdateThread.Create(UpdateThreadType::ProcType(this, &MessageWindow::UpdateThread), 0);
    return 0;
}

LRESULT MessageWindow::OnMenuAbout(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    AboutDialog::ShowDialog(this);
    return 0;
}

LRESULT MessageWindow::OnMenuWebsite(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    ShellExecute(m_hWnd, _T("open"), _T("http://www.streamlet.org/"), NULL, NULL, SW_SHOW);
    return 0;
}

DWORD MessageWindow::UpdateThread(HANDLE hQuit, DWORD dwDelay)
{
    XL_LOG_INFO_FUNCTION();

    if (WaitForSingleObject(hQuit, dwDelay) == WAIT_OBJECT_0)
    {
        return 0;
    }

    TCHAR szExeName[MAX_PATH] = {};
    
    if (!GetModuleFileName(NULL, szExeName, MAX_PATH))
    {
        return 0;
    }

    DWORD dwHandle = 0;
    DWORD dwSize = GetFileVersionInfoSize(szExeName, &dwHandle);

    if (dwSize == 0)
    {
        return 0;
    }

    xl::SharedArray<BYTE> spBuffer = new BYTE[dwSize];
    LPBYTE lpBuffer = spBuffer.RawPointer();

    if (!GetFileVersionInfo(szExeName, 0, dwSize, lpBuffer))
    {
        return 0;
    }

    VS_FIXEDFILEINFO *pVer;
    UINT uLen = 0;
    
    if (!VerQueryValue(lpBuffer, _T("\\"), (LPVOID *)&pVer, &uLen))
    {
        return 0;
    }

    DWORD dwMajor    = HIWORD(pVer->dwFileVersionMS);
    DWORD dwMinor    = LOWORD(pVer->dwFileVersionMS);
    DWORD dwBuild    = HIWORD(pVer->dwFileVersionLS);
    DWORD dwRevision = LOWORD(pVer->dwFileVersionLS);

    TCHAR szLocalVersion[MAX_PATH] = {};
    _stprintf_s(szLocalVersion, _T("%u.%u.%u.%u"), dwMajor, dwMinor, dwBuild, dwRevision);

    XL_LOG_INFO(_T("Local version: %s"), szLocalVersion);

    xl::String strUrl = _T("http://www.streamlet.org/api/Update/Query/GoogleHelper/");
    strUrl += szLocalVersion;

    HttpRestIO http(_T("GoogleHelper"));
    xl::Array<BYTE> arrResponse;

    if (!http.SendRequest(_T("GET"), strUrl, nullptr, nullptr, 0, hQuit, &arrResponse))
    {
        return 0;
    }

    if (arrResponse.Size() > 3 &&
        arrResponse[0] == 0xef &&
        arrResponse[0] == 0xbb &&
        arrResponse[0] == 0xbf)
    {
        arrResponse.Delete(0, 3);
    }

    arrResponse.PushBack(0);
    xl::StringA strJsonA = (LPCSTR)&arrResponse[0];
    xl::String strJson;
    
    if (!xl::Encoding::Utf8ToUtf16(strJsonA, &strJson))
    {
        return 0;
    }

    XL_LOG_INFO(_T("Update result:\r\n%s"), (LPCTSTR)strJson);

    //
    // Json Format
    //
    // {
    //     "Result": true,
    //     "Version": "1.0.0.0",
    //     "DisplayVersion": "1.0 正式版",
    //     "Url": "http://www.streamlet.org/Software/GoogleHelper/"
    // }
    //

    Json::Value root;

    if (!Json::Reader().parse((LPCSTR)strJsonA, (LPCSTR)strJsonA + strJsonA.Length(), root))
    {
        XL_LOG_INFO(_T("Failed to parse Json result."));
        return 0;
    }

    bool bResult = root["Result"].asBool();
    xl::StringA strVersionA = root["Version"].asCString();
    xl::StringA strDisplayVersionA = root["DisplayVersion"].asCString();
    xl::StringA strDownloadUrlA = root["Url"].asCString();

    xl::String strVersion;
    xl::String strDisplayVersion;
    xl::String strDownloadUrl;

    if (!xl::Encoding::Utf8ToUtf16(strVersionA, &strVersion))
    {
        return 0;
    }
    if (!xl::Encoding::Utf8ToUtf16(strDisplayVersionA, &strDisplayVersion))
    {
        return 0;
    }
    if (!xl::Encoding::Utf8ToUtf16(strDownloadUrlA, &strDownloadUrl))
    {
        return 0;
    }

    if (!bResult)
    {
        XL_LOG_INFO(_T("Not update. Prompt: %u"), (DWORD)(dwDelay == 0));

        if (dwDelay == 0)
        {
            TCHAR szMessage[MAX_PATH] = {};
            _stprintf_s(szMessage, MESSAGE_UPDATE_LATEST, szLocalVersion);
            MessageBox(szMessage, MESSAGE_UPDATE_CAPTION, MB_OK | MB_ICONINFORMATION);
        }

        return 0;
    }

    TCHAR szMessage[MAX_PATH] = {};
    xl::String strVersionString = strDisplayVersion + _T(" (") + strVersion + _T(") ");
    _stprintf_s(szMessage, MESSAGE_UPDATE_NEW_VERSION, (LPCTSTR)strVersionString);

    if (MessageBox(szMessage, MESSAGE_UPDATE_CAPTION, MB_YESNO | MB_ICONINFORMATION) != IDYES)
    {
        XL_LOG_INFO(_T("User refused to update."));
        return 0;
    }

    ShellExecute(m_hWnd, _T("open"), strDownloadUrl, NULL, NULL, SW_SHOW);
    XL_LOG_INFO(_T("User accept to update."));

    return 0;
}
