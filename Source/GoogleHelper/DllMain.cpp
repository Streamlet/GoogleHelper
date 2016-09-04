//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   DllMain.cpp
//    Author:      Streamlet
//    Create Time: 2012-12-02
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
#include <xl/Windows/Registry/xlRegistry.h>

#define REG_PATH_BHO _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\{2288AEBD-1891-4AF7-A9A1-50570241D789}")

STDAPI DllCanUnloadNow()
{
    return xl::Windows::g_pComModule->DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    return xl::Windows::g_pComModule->DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer()
{
    xl::Windows::Registry::SetString(HKEY_LOCAL_MACHINE, REG_PATH_BHO, _T(""), _T("GoogleHelper"));
    xl::Windows::Registry::SetString(HKEY_CURRENT_USER,  REG_PATH_BHO, _T(""), _T("GoogleHelper"));

    return xl::Windows::g_pComModule->DllRegisterServer();
}

STDAPI DllUnregisterServer()
{
    xl::Windows::Registry::DeleteKey(HKEY_LOCAL_MACHINE, REG_PATH_BHO);
    xl::Windows::Registry::DeleteKey(HKEY_CURRENT_USER,  REG_PATH_BHO);

    return xl::Windows::g_pComModule->DllUnregisterServer();
}

STDAPI DllInstall(BOOL bInstall, LPCTSTR lpszCmdLine)
{
    return xl::Windows::g_pComModule->DllInstall(bInstall, lpszCmdLine);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CoInitialize(nullptr);
        xl::Windows::g_pComModule = new xl::Windows::ComModule(hModule, _T("Streamlet GoogleHelper TypeLib 1.0"));
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        delete xl::Windows::g_pComModule;
        CoUninitialize();
        break;
    default:
        break;
    }

    return TRUE;
}


