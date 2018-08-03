//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   TraceLog.h
//    Author:      Streamlet
//    Create Time: 2010-05-01
//    Description: 
//
//    Version history:
//
//
//
//------------------------------------------------------------------------------

#ifndef __LOG_H_404E7488_857C_4041_8BD7_4388BFE30810_INCLUDED__
#define __LOG_H_404E7488_857C_4041_8BD7_4388BFE30810_INCLUDED__

#define XL_LOG_PREFIX L"[LogPrefix] "           // Log prefix
#define XL_LOG_TARGET_DEBUGGER                  // Output to debugger
#define XL_LOG_TARGET_CONSOLE                   // Output to console
#define XL_LOG_TARGET_FILE L"%Temp%\\xlLog.log" // Output to file, supporting environment variables
#define XL_LOG_CONTENT_SOURCEFILE               // Output source file name
#define XL_LOG_CONTENT_FUNCTION                 // Output function name
#define XL_LOG_CONTENT_LINE                     // Output line number
#define XL_LOG_LEVEL_FATAL                      // Output log level fatal
#define XL_LOG_LEVEL_ERROR                      // Output log level error
#define XL_LOG_LEVEL_WARNING                    // Output log level warning
#define XL_LOG_LEVEL_INFO                       // Output log level info
#define XL_LOG_LEVEL_VERBOSE                    // Output log level verbose

#include <xl/AppHelper/xlLog.h>

#endif // #ifndef __LOG_H_404E7488_857C_4041_8BD7_4388BFE30810_INCLUDED__
