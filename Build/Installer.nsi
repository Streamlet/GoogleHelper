;--------------------------------
;Macro definitions

!define COMPANY         "Streamlet"
!define COMPANY_DISPLAY "溪流软件工作室"
!define PRODUCT         "GoogleHelper"
!define PRODUCT_DISPLAY "Google 搜索助手"

!ifndef VERSION
!define VERSION         "1.0.0.0"
!endif

!define VERSION_SHORT   "1.0"

!ifndef SOURCEDIR
!define SOURCEDIR       "Release\Test"
!endif

!ifndef OUTFILE
!define OUTFILE         "${SOURCEDIR}\${PRODUCT}_${VERSION}.exe"
!endif

;--------------------------------
;Configuration

OutFile "${OUTFILE}"
SetCompressor /SOLID lzma

Name "${PRODUCT_DISPLAY}"
Caption "${PRODUCT_DISPLAY} ${VERSION_SHORT} 安装程序"
BrandingText "${COMPANY_DISPLAY}"

InstType "Full"

InstallDir "$PROGRAMFILES\${COMPANY}\${PRODUCT}"
InstallDirRegKey HKLM "Software\${COMPANY}\${PRODUCT}" "InstallPath"

ShowInstDetails show
ShowUninstDetails show

RequestExecutionLevel admin


;--------------------------------
;Header Files

!include "MUI2.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "WordFunc.nsh"
!include "FileFunc.nsh"

;--------------------------------
;Interface

!define MUI_ABORTWARNING

!define MUI_ICON "Icon.ico"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\win.bmp"

!insertmacro MUI_PAGE_INIT
!insertmacro MUI_PAGE_WELCOME

!define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "license.rtf"

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_LINK_LOCATION "http://www.streamlet.org/"
!define MUI_FINISHPAGE_LINK "溪流软件工作室"
;!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_NOREBOOTSUPPORT
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
;Version Information
VIProductVersion "${VERSION}"
VIAddVersionKey "ProductName" "${PRODUCT_DISPLAY}"
VIAddVersionKey "FileDescription" "${PRODUCT_DISPLAY}，助您改善 Google 搜索的体验。"
VIAddVersionKey "CompanyName" "${COMPANY_DISPLAY}"
VIAddVersionKey "LegalCopyright" "版权所有 © 2012 溪流软件工作室. 保留所有权利."
VIAddVersionKey "FileVersion" "${VERSION}"
VIAddVersionKey "ProductVersion" "${VERSION_DISPLAY}"

;--------------------------------
;Installation


Function InstallFiles

    SetDetailsPrint both
    DetailPrint "正在复制文件..."
    SetDetailsPrint listonly

    SetOutPath $INSTDIR
    SetOverwrite on
    File "${SOURCEDIR}\GoogleHelper.dll"
    File "${SOURCEDIR}\GoogleHelper64.dll"
    File "${SOURCEDIR}\GoogleHelperAgent.exe"

    WriteUninstaller "$INSTDIR\Uninstall.exe"

    SetDetailsPrint both

FunctionEnd

Function un.RemoveFiles

    SetDetailsPrint both
    DetailPrint "正在删除文件..."
    SetDetailsPrint listonly

    SetOutPath $TEMP
    Delete "$INSTDIR\GoogleHelper.dll"
    Delete "$INSTDIR\GoogleHelper64.dll"
    Delete "$INSTDIR\GoogleHelperAgent.exe"
    Delete "$INSTDIR\Uninstall.exe"
    
    RMDir "$INSTDIR"

    SetDetailsPrint both

FunctionEnd

Function RegFiles

    SetDetailsPrint both
    DetailPrint "正在注册组件..."
    SetDetailsPrint listonly

    ExecWait 'regsvr32 /s "$INSTDIR\GoogleHelper.dll"'
    ExecWait 'regsvr32 /s "$INSTDIR\GoogleHelper64.dll"'
    ExecWait '"$INSTDIR\GoogleHelperAgent.exe" /RegServer'

    SetDetailsPrint both

FunctionEnd

Function un.UnregFiles

    SetDetailsPrint both
    DetailPrint "正在注销组件..."
    SetDetailsPrint listonly

    ExecWait 'regsvr32 /s /u "$INSTDIR\GoogleHelper.dll"'
    ExecWait 'regsvr32 /s /u "$INSTDIR\GoogleHelper64.dll"'
    ExecWait '"$INSTDIR\GoogleHelperAgent.exe" /UnregServer'

    SetDetailsPrint both

FunctionEnd


Function un.RemoveUserData

    SetDetailsPrint both
    DetailPrint "正在删除用户数据..."
    SetDetailsPrint listonly

    SetOutPath $TEMP
    SetShellVarContext all
    RMDir /r "$APPDATA\${COMPANY}\${PRODUCT}"
    RMDir "$APPDATA\${COMPANY}"

    SetDetailsPrint both

FunctionEnd

Function WriteRegistry

    SetDetailsPrint both
    DetailPrint "正在写注册表..."
    SetDetailsPrint listonly

    WriteRegStr HKLM "Software\${COMPANY}\${PRODUCT}" "InstallPath" "$INSTDIR"
    WriteRegStr HKLM "Software\${COMPANY}\${PRODUCT}" "Version" "${VERSION}"

    SetDetailsPrint both

FunctionEnd

Function un.ClearRegistry

    SetDetailsPrint both
    DetailPrint "正在删除注册表项..."
    SetDetailsPrint listonly

    DeleteRegKey HKLM "Software\${COMPANY}\${PRODUCT}"

    SetDetailsPrint both

FunctionEnd

Function WriteInstallInformation

    SetDetailsPrint both
    DetailPrint "正在写入安装信息..."
    SetDetailsPrint listonly

    WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT_DISPLAY}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayIcon" "$INSTDIR\GoogleHelperAgent.exe,0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayVersion" "${VERSION}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Publisher" "${COMPANY_DISPLAY}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "URLInfoAbout" "http://www.streamlet.org/"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "HelpLink" "http://www.streamlet.org/"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoModify" "1"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoRepair" "1"

    SetDetailsPrint both

FunctionEnd

Function un.ClearInstallInformation

    SetDetailsPrint both
    DetailPrint "正在删除安装信息..."
    SetDetailsPrint listonly

    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"

    SetDetailsPrint both

FunctionEnd

!insertmacro VersionCompare

Section "-Install"

    SetDetailsPrint both
    DetailPrint "正在检查旧版本..."
    SetDetailsPrint listonly

    Var /GLOBAL OldVersionInstalled
    Var /GLOBAL NewVersionInstalled
    StrCpy $OldVersionInstalled 0
    StrCpy $NewVersionInstalled 0

    Push $R0

    ReadRegStr $R0 HKLM "Software\${COMPANY}\${PRODUCT}" "Version"

    ${If} $R0 != ""
        ${VersionCompare} ${VERSION} $R0 $R0
        ;0: Same version installed
        ;1: Older version installed
        ;2: Newer version installed
        ${If} $R0 == 1
            StrCpy $OldVersionInstalled 1
        ${Else}
            StrCpy $NewVersionInstalled 1
        ${EndIf}
    ${EndIf}

    Pop $R0

    ${If} $NewVersionInstalled != 0
        DetailPrint "${PRODUCT_DISPLAY} 已经安装"
    ${Else}
        ${If} $OldVersionInstalled != 0
            DetailPrint "${PRODUCT_DISPLAY} 的一个旧版本已经安装"
        ${Else}
            DetailPrint "${PRODUCT_DISPLAY} 尚未安装"
        ${EndIf}

        Call InstallFiles
        Call RegFiles
        Call WriteRegistry
        Call WriteInstallInformation

    ${EndIf}

    SetDetailsPrint both

SectionEnd

Section Uninstall

    Call un.UnregFiles
    Call un.RemoveFiles
    Call un.ClearRegistry
    Call un.RemoveUserData
    Call un.ClearInstallInformation

SectionEnd
