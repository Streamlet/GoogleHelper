//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   GoogleHelper.cpp
//    Author:      Streamlet
//    Create Time: 2012-12-02
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------


#include "GoogleHelper.h"
#include "../Utility/UrlUtils.h"
#include <ExDispid.h>

TCHAR szGoogleJumpUrl[] = _T("http://www.google.com.hk/url?");

GoogleHelper::GoogleHelper() :
    m_pUnkSite(nullptr),
    m_pWebBrowser(nullptr),
    m_pCPC(nullptr),
    m_pCP(nullptr),
    m_dwCookie(0),
    m_pAgent(nullptr)
{
    CoCreateInstance(__uuidof(GoogleHelperAgent),
                     nullptr,
                     CLSCTX_LOCAL_SERVER,
                     __uuidof(IGoogleHelperAgent),
                     (LPVOID *)&m_pAgent);
}

GoogleHelper::~GoogleHelper()
{
    ReleaseSiteResources();

    if (m_pAgent != nullptr)
    {
        m_pAgent->Release();
        m_pAgent = nullptr;
    }
}

STDMETHODIMP GoogleHelper::SetSite(IUnknown *pUnkSite)
{
    ReleaseSiteResources();

    if (pUnkSite != nullptr)
    {
        HRESULT hr = pUnkSite->QueryInterface(__uuidof(IWebBrowser2), (LPVOID *)&m_pWebBrowser);

        if (FAILED(hr))
        {
            return hr;
        }

        hr = m_pWebBrowser->QueryInterface(__uuidof(IConnectionPointContainer), (LPVOID *)&m_pCPC);

        if (FAILED(hr))
        {
            return hr;
        }

        hr = m_pCPC->FindConnectionPoint(__uuidof(DWebBrowserEvents2), &m_pCP);

        if (FAILED(hr))
        {
            return hr;
        }

        hr = m_pCP->Advise((DWebBrowserEvents2 *)this, &m_dwCookie);

        if (FAILED(hr))
        {
            return hr;
        }

        m_pUnkSite = pUnkSite;
        m_pUnkSite->AddRef();
    }
    
    return S_OK;
}

STDMETHODIMP GoogleHelper::GetSite(REFIID riid, void **ppvSite)
{
    if (ppvSite == nullptr)
    {
        return E_POINTER;
    }

    if (m_pUnkSite == nullptr)
    {
        *ppvSite = nullptr;
        return E_FAIL;
    }

    return m_pUnkSite->QueryInterface(riid, ppvSite);
}

STDMETHODIMP GoogleHelper::Invoke(DISPID dispIdMember,
                                  REFIID riid,
                                  LCID lcid,
                                  WORD wFlags,
                                  DISPPARAMS *pDispParams,
                                  VARIANT *pVarResult,
                                  EXCEPINFO *pExcepInfo,
                                  UINT *puArgErr)
{
    switch (dispIdMember)
    {
    case DISPID_BEFORENAVIGATE2:
        return OnBeforeNavigate2(pDispParams, pVarResult);
    default:
        break;
    }

    return xl::Dispatcher<xl::IObjectWithSiteImpl<>>::Invoke(dispIdMember,
                                                             riid,
                                                             lcid,
                                                             wFlags,
                                                             pDispParams,
                                                             pVarResult,
                                                             pExcepInfo,
                                                             puArgErr);
}

void GoogleHelper::ReleaseSiteResources()
{
    if (m_dwCookie != 0 && m_pCP != nullptr)
    {
        m_pCP->Unadvise(m_dwCookie);
        m_dwCookie = 0;
    }

    if (m_pCP != nullptr)
    {
        m_pCP->Release();
        m_pCP = nullptr;
    }

    if (m_pCPC != nullptr)
    {
        m_pCPC->Release();
        m_pCPC = nullptr;
    }

    if (m_pWebBrowser != nullptr)
    {
        m_pWebBrowser->Release();
        m_pWebBrowser = nullptr;
    }

    if (m_pUnkSite != nullptr)
    {
        m_pUnkSite->Release();
        m_pUnkSite = nullptr;
    }
}

HRESULT GoogleHelper::OnBeforeNavigate2(DISPPARAMS *pDispParams, VARIANT *pVarResult)
{
    /*
    http://msdn.microsoft.com/en-us/library/aa768280(v=vs.85).aspx
    Arguments:
        pDisp [in]
            A pointer to the IDispatch interface for the WebBrowser object that represents the window or frame.
            This interface can be queried for the IWebBrowser2 interface.  

        Url [in, ref]
            A pointer to a VARIANT structure of type VT_BSTR that contains the URL to navigate to.  

        Flags [in, ref]
            Type: Variant
            A pointer to a VARIANT of type VT_I4 that contains the following flag, or zero.
            
            beforeNavigateExternalFrameTarget (&H0001)
                Internet Explorer 7 or later. This navigation is the result of an external window or tab that
                targets this browser.
                
        TargetFrameName [in, ref]
            A pointer to a VARIANT structure of type VT_BSTR that contains the name of the frame
            in which to display the resource, or NULL, if no named frame is targeted for the resource.  

        PostData [in, ref]
            A pointer to a VARIANT structure of type VT_BYREF|VT_VARIANT that contains the data to
            send to the server if the HTTP POST transaction is used.  

        Headers [in, ref]
            A pointer to a VARIANT structure of type VT_BSTR that contains additional HTTP headers to
            send to the server (HTTPURLs only). The headers can specify information, such as the action
            required of the server, the type of data passed to the server, or a status code.  

        Cancel [in, out, ref]
            A pointer to a VARIANT structure of type VARIANT_BOOL that contains the cancel flag.
            An application can set this parameter to VARIANT_TRUE to cancel the navigation operation,
            or to VARIANT_FALSE to allow the navigation operation to proceed.

            Note
                The Cancel parameter is an OUT parameter to cancel or allow navigation as above.
                However, because there is a chain of event handlers, Cancel is also an IN parameter to
                provide notification of the value set by the previous handler.
    */

    VARIANTARG &Cancel          = pDispParams->rgvarg[0];
    VARIANTARG &Headers         = pDispParams->rgvarg[1];
    VARIANTARG &PostData        = pDispParams->rgvarg[2];
    VARIANTARG &TargetFrameName = pDispParams->rgvarg[3];
    VARIANTARG &Flags           = pDispParams->rgvarg[4];
    VARIANTARG &Url             = pDispParams->rgvarg[5];
    VARIANTARG &pDisp           = pDispParams->rgvarg[6];

    if (_tcsnicmp(Url.pvarVal->bstrVal, szGoogleJumpUrl, ARRAYSIZE(szGoogleJumpUrl) - 1) != 0)
    {
        return S_OK;
    }

    xl::String strOriginalUrl = Url.pvarVal->bstrVal + ARRAYSIZE(szGoogleJumpUrl) - 1;
    auto arrParams = strOriginalUrl.Split(_T("&"));
    xl::String strJumpingUrl;

    for (auto it = arrParams.Begin(); it != arrParams.End(); ++it)
    {
        if (it->IndexOf(_T("url=")) == 0)
        {
            strJumpingUrl = it->SubString(4);
            break;
        }
    }

    if (strJumpingUrl.Empty())
    {
        return S_OK;
    }

    xl::String strJumpingUrlDecoded = UrlUtils::UrlDecode(strJumpingUrl);

    VARIANT vtUrl = {};
    vtUrl.vt = VT_BSTR;
    vtUrl.bstrVal = SysAllocString(strJumpingUrlDecoded.GetAddress());

    if (m_pAgent != nullptr)
    {
        m_pAgent->Report(vtUrl.bstrVal);
    }

    *Cancel.pboolVal = VARIANT_TRUE;

    return m_pWebBrowser->Navigate2(&vtUrl, &Flags, &TargetFrameName, &PostData, &Headers);
}
