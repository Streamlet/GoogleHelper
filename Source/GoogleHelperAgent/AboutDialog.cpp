//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   AboutDialog.cpp
//    Author:      Streamlet
//    Create Time: 2011-05-10
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include <xl/String/xlString.h>
#include <Windows.h>
#include <ShellAPI.h>
#include "AboutDialog.h"
#include "resource.h"
#include "../GlobalDef.h"

enum
{
    ID_STATIC   = -1,
    ID_NULL     = 0,

    ID_START    = 100,

    ID_STATIC_ICON,
    ID_STATIC_PRODUCTNAME,
    ID_STATIC_COPYRIGHT,
    ID_STATIC_THANKS,
    ID_STATIC_CONTACT,

    ID_LINK_WEBSITE,
    ID_LINK_EMAIL
};

void AboutDialog::ShowDialog(xl::Window *pParent /*= nullptr*/)
{
    if (ms_pDialog == nullptr)
    {
        AboutDialog dialog;

        if (dialog.Create(320, 304, pParent, xl::Dialog::GetClassName(), WS_POPUPWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, WS_EX_CONTROLPARENT | WS_EX_LAYERED))
        {
            ms_pDialog = &dialog;
            dialog.DoModal();
            ms_pDialog = nullptr;
        }
    }
    else
    {
        ms_pDialog->BringWindowToTop();
    }
}

AboutDialog::AboutDialog() :
    m_bIconMoving(false)
{
    AppendMsgHandler(WM_DESTROY, MsgHandler(this, &AboutDialog::OnDestroy));
    AppendMsgHandler(WM_TIMER,   MsgHandler(this, &AboutDialog::OnTimer));

    AppendCommandMsgHandler(ID_STATIC_ICON, CommandMsgHandler(this, &AboutDialog::OnStaticIconClick));

    AppendNotifyMsgHandler(ID_LINK_WEBSITE, NM_CLICK,   NotifyMsgHandler(this, &AboutDialog::OnLinkWebsiteClick));
    AppendNotifyMsgHandler(ID_LINK_EMAIL, NM_CLICK,     NotifyMsgHandler(this, &AboutDialog::OnLinkEmailClick));
}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::CreateControls()
{
    SetLayeredWindowAttributes(m_hWnd, 0, 255, LWA_ALPHA);

    m_buttonOK          .Create(IDOK,                   this, 224, 240, 75,  24);

    m_staticIcon        .Create(ID_STATIC_ICON,         this, 12,  10,  32,  32, WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_ICON);
    m_staticProductName .Create(ID_STATIC_PRODUCTNAME,  this, 60,  10,  240, 16);
    m_staticCopyright   .Create(ID_STATIC_COPYRIGHT,    this, 60,  30,  240, 32);
    m_staticThanks      .Create(ID_STATIC_THANKS,       this, 60,  80,  240, 48);
    m_staticContact     .Create(ID_STATIC_CONTACT,      this, 60,  148, 240, 32);
    m_linkWebsite       .Create(ID_LINK_WEBSITE,        this, 60,  192, 240, 16);
    m_linkEmail         .Create(ID_LINK_EMAIL,          this, 60,  212, 240, 16);
}

void AboutDialog::SetTexts()
{
    SetWindowText(_T("关于 ") PRODUCT_NAME);
    m_staticProductName.SetWindowText(PRODUCT_NAME);
    m_staticCopyright.SetWindowText(_T("版权所有 (C) 2012 溪流软件工作室\r\n保留所有权利\r\n"));
    m_staticThanks.SetWindowText(_T("以我无限热情\r\n传承自由文明\r\n"));
    m_staticContact.SetWindowText(_T("如果您在使用中遇到任何问题，或者有任何意见和建议，欢迎联系我们："));
    m_buttonOK.SetWindowText(_T("确定"));
    m_linkWebsite.SetWindowText(_T("<a href=\"http://www.streamlet.org/\">http://www.streamlet.org/</a>"));
    m_linkEmail.SetWindowText(_T("<a href=\"mailto:kejinjin@gmail.com\">kejinjin@gmail.com</a>"));
}

bool AboutDialog::OnCreate()
{
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP));
    SetIcon(hIcon);

    CreateControls();
    SetTexts();

    m_staticIcon.SetIcon(hIcon);

    return true;
}

LRESULT AboutDialog::OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    if (m_bIconMoving)
    {
        KillTimer(m_hWnd, 0);
        m_bIconMoving = false;
    }

    return FALSE;
}

LRESULT AboutDialog::OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    static bool bDownwards = true;

    RECT rect = {};
    m_staticIcon.GetWindowRect(&rect);

    POINT point = { rect.left, rect.top};
    ScreenToClient(&point);

    if (bDownwards)
    {
        point.y++;

        if (point.y >= 200)
        {
            bDownwards = false;
        }
    }
    else
    {
        point.y--;

        if (point.y <= 12)
        {
            bDownwards = true;
        }
    }

    m_staticIcon.MoveWindow(point.x, point.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    SetLayeredWindowAttributes(m_hWnd, 0, 255 + 11 - (BYTE)point.y, LWA_ALPHA);

    return FALSE;
}

LRESULT AboutDialog::OnStaticIconClick(HWND hWnd, WORD wID, WORD wCode, HWND hControl, BOOL &bHandled)
{
    if (m_bIconMoving)
    {
        KillTimer(m_hWnd, 0);
        m_bIconMoving = false;
    }
    else
    {
        if (SetTimer(m_hWnd, 0, 25, nullptr))
        {
            m_bIconMoving = true;
        }
    }

    return FALSE;
}

LRESULT AboutDialog::OnLinkWebsiteClick(HWND hWnd, UINT_PTR uID, UINT uCode, HWND hControl, BOOL &bHandled)
{
    ShellExecute(m_hWnd, _T("open"), _T("http://www.streamlet.org/"), NULL, NULL, SW_SHOW);
    return FALSE;
}

LRESULT AboutDialog::OnLinkEmailClick(HWND hWnd, UINT_PTR uID, UINT uCode, HWND hControl, BOOL &bHandled)
{
    ShellExecute(m_hWnd, _T("open"), _T("mailto:kejinjin@gmail.com"), NULL, NULL, SW_SHOW);
    return FALSE;
}
