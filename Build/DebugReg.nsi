OutFile "DebugReg.exe"
Icon "Icon.ico"

SetCompressor /SOLID lzma
RequestExecutionLevel admin

Function .onInit

    SetSilent silent

FunctionEnd

Section ""

    ExecWait 'regsvr32 /s "bin\Win32\Debug\GoogleHelper.dll"'
    ExecWait 'regsvr32 /s "bin\Win32\Debug\GoogleHelper64.dll"'
    ExecWait '"bin\Win32\Debug\GoogleHelperAgent.exe" /RegServer'

SectionEnd