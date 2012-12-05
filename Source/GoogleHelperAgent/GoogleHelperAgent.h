//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   GoogleHelperAgent.h
//    Author:      Streamlet
//    Create Time: 2012-12-03
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __GOOGLEHELPERAGENT_H_014C5786_CD87_4D52_976A_6CAE6D5C9E67_INCLUDED__
#define __GOOGLEHELPERAGENT_H_014C5786_CD87_4D52_976A_6CAE6D5C9E67_INCLUDED__


#include "GoogleHelperAgent_h.h"
#include "../GlobalDef.h"
#include <xl/Win32/COM/xlComInclude.h>

class GoogleHelperAgent : public xl::ComClass<GoogleHelperAgent>,
                          public xl::Dispatcher<IGoogleHelperAgent>
{
public:
    GoogleHelperAgent();
    ~GoogleHelperAgent();

public: // IGoogleHelperAgent Methods
    STDMETHOD(Report)(BSTR bstrUrl);

public:
    XL_COM_INTERFACE_BEGIN(GoogleHelperAgent)
        XL_COM_INTERFACE(IDispatch)
        XL_COM_INTERFACE(IGoogleHelperAgent)
    XL_COM_INTERFACE_END()
};

XL_DECLARE_COM_CLASS(GoogleHelperAgent,
                     MODULE_NAME_GOOGLEHELPER_AGENT,
                     _T("Streamlet.GoogleHelperAgent.Class"),
                     _T("1"));


#endif // #ifndef __GOOGLEHELPERAGENT_H_014C5786_CD87_4D52_976A_6CAE6D5C9E67_INCLUDED__
