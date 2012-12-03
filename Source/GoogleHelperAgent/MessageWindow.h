//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   MessageWindow.h
//    Author:      Streamlet
//    Create Time: 2012-12-03
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __MESSAGEWINDOW_H_F2D9ACF7_E60E_449D_9DB8_225D5B818A91_INCLUDED__
#define __MESSAGEWINDOW_H_F2D9ACF7_E60E_449D_9DB8_225D5B818A91_INCLUDED__


#include <xl/Win32/GUI/xlWindow.h>

class MessageWindow : public xl::Window
{
public:
    MessageWindow();
    ~MessageWindow();

public:
    void ReportUrl(LPCTSTR lpszUrl);

private:
    LRESULT OnCreate  (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnDestroy (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnTrayIcon(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
    NOTIFYICONDATA m_nid;
};

__declspec(selectany) MessageWindow g_wndMessage;

#endif // #ifndef __MESSAGEWINDOW_H_F2D9ACF7_E60E_449D_9DB8_225D5B818A91_INCLUDED__
