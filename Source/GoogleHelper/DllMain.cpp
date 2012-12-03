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
#include <xl/Win32/COM/xlComInclude.h>
#include <xl/Win32/Registry/xlRegistry.h>

#define REG_PATH_BHO _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\{2288AEBD-1891-4AF7-A9A1-50570241D789}")

STDAPI DllCanUnloadNow()
{
    return xl::g_pComModule->DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    return xl::g_pComModule->DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer()
{
    xl::Registry::SetString(HKEY_LOCAL_MACHINE, REG_PATH_BHO, _T(""), _T("GoogleHelper"));
    xl::Registry::SetString(HKEY_CURRENT_USER,  REG_PATH_BHO, _T(""), _T("GoogleHelper"));

    return xl::g_pComModule->DllRegisterServer();
}

STDAPI DllUnregisterServer()
{
    xl::Registry::DeleteKey(HKEY_LOCAL_MACHINE, REG_PATH_BHO);
    xl::Registry::DeleteKey(HKEY_CURRENT_USER,  REG_PATH_BHO);

    return xl::g_pComModule->DllUnregisterServer();
}

STDAPI DllInstall(BOOL bInstall, LPCTSTR lpszCmdLine)
{
    return xl::g_pComModule->DllInstall(bInstall, lpszCmdLine);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        xl::g_pComModule = new xl::ComModule(hModule, _T("Streamlet GoogleHelper TypeLib 1.0"));
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        delete xl::g_pComModule;
        break;
    default:
        break;
    }

    return TRUE;
}


