//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   WinMain.cpp
//    Author:      Streamlet
//    Create Time: 2012-12-03
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include <Windows.h>
#include <tchar.h>
#include <xl/Windows/COM/xlComInclude.h>
#include "MessageWindow.h"

int APIENTRY _tWinMain(_In_ HINSTANCE     hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR        lpCmdLine,
                       _In_ int           nCmdShow)
{
    xl::Windows::g_pComModule = new xl::Windows::ComModule(hInstance, _T("Streamlet GoogleHelperAgent TypeLib 1.0"));
    
    if (_tcsicmp(lpCmdLine, _T("/RegServer")) == 0 || _tcsicmp(lpCmdLine, _T("-RegServer")) == 0)
    {
        xl::Windows::g_pComModule->ExeRegisterServer();
    }
    else if (_tcsicmp(lpCmdLine, _T("/UnregServer")) == 0 || _tcsicmp(lpCmdLine, _T("-UnregServer")) == 0)
    {
        xl::Windows::g_pComModule->ExeUnregisterServer();
    }
    else if (_tcsicmp(lpCmdLine, _T("/Embedding")) == 0 || _tcsicmp(lpCmdLine, _T("-Embedding")) == 0)
    {
        HRESULT hr = xl::Windows::g_pComModule->ExeRegisterClassObject();

        if (SUCCEEDED(hr))
        {
            MSG msg = {};

            while (GetMessage(&msg, nullptr, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (g_wndMessage.IsWindow())
            {
                g_wndMessage.SendMessage(WM_CLOSE, 0, 0);
            }
        }

        xl::Windows::g_pComModule->ExeUnregisterClassObject();
    }

    delete xl::Windows::g_pComModule;

    return 0;
}
