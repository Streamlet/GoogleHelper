@Echo off
Echo 请输入版本号：
:Input
Set /p Version=
If "%Version%"=="" Goto Input

For /f "tokens=1-4 delims=." %%a In ("%Version%") Do (
    If "%%a"=="" Goto Input
    If "%%b"=="" Goto Input
    If "%%c"=="" Goto Input
    If "%%d"=="" Goto Input
)

For /f "tokens=1-2 delims=." %%a In ("%Version%") Do Set PVersion=%%a.%%b
For /f "tokens=1-3 delims=-/ " %%a In ('Date /T') Do Set DateString=%%a%%b%%c
For /f "tokens=1-2 delims=: " %%a In ('Time /T') Do Set TimeString=%%a%%b

Set ToolsDir=Tools
Set SourceDir=..\Source
Set AppName=GoogleHelper
Set ReleaseDir=Release\%AppName%_%Version%_%DateString%%TimeString%

Echo 正在编译...

"%~dp0%ToolsDir%\RCVersion.exe" /FileVersion:%Version:.=,% /ProductVersion:%PVersion:.=,%,0,0 /String:FileVersion=%Version% /String:ProductVersion=%PVersion% "%~dp0%SourceDir%\%AppName%\*.rc"

Set BuildCmd=%VS110COMNTOOLS%..\IDE\devenv.com
"%BuildCmd%" "%~dp0%SourceDir%\%AppName%.sln" /Build "Release|Win32"
"%BuildCmd%" "%~dp0%SourceDir%\%AppName%.sln" /Build "Release|x64"

Echo 正在组织文件...

Md "%~dp0%ReleaseDir%"

Copy "%~dp0%SourceDir%\bin\Win32\Release\GoogleHelper.dll"      "%~dp0%ReleaseDir%\"
Copy "%~dp0%SourceDir%\bin\x64\Release\GoogleHelper.dll"        "%~dp0%ReleaseDir%\GoogleHelper64.dll"
Copy "%~dp0%SourceDir%\bin\Win32\Release\GoogleHelperAgent.exe" "%~dp0%ReleaseDir%\"

Echo 正在编译安装程序...

"%PROGRAMFILES(x86)%\NSIS\Unicode\makensis.exe" /DVERSION=%Version% /DSOURCEDIR=%ReleaseDir% "%~dp0Installer.nsi"

Echo 生成结束

Pause
