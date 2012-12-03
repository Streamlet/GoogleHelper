//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   GoogleHelperAgent.cpp
//    Author:      Streamlet
//    Create Time: 2012-12-03
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include "GoogleHelperAgent.h"
#include "MessageWindow.h"

GoogleHelperAgent::GoogleHelperAgent()
{

}

GoogleHelperAgent::~GoogleHelperAgent()
{

}

STDMETHODIMP GoogleHelperAgent::Report(BSTR bstrUrl)
{
    if (!g_wndMessage.IsWindow())
    {
        g_wndMessage.Create();
    }

    if (g_wndMessage.IsWindow())
    {
        g_wndMessage.ReportUrl(bstrUrl);
    }

    return S_OK;
}
