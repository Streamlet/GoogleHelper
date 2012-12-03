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

GoogleHelperAgent::GoogleHelperAgent()
{

}

GoogleHelperAgent::~GoogleHelperAgent()
{

}

STDMETHODIMP GoogleHelperAgent::Report(BSTR bstrUrl)
{
    MessageBox(0, bstrUrl, 0, 0);
    return S_OK;
}
