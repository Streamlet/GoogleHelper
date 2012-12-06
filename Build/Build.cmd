@Echo off
Echo ������汾�ţ�
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

Echo ���ڱ���...

"%~dp0%ToolsDir%\RCVersion.exe" /FileVersion:%Version:.=,% /ProductVersion:%PVersion:.=,%,0,0 /String:FileVersion=%Version% /String:ProductVersion=%PVersion% "%~dp0%SourceDir%\%AppName%\*.rc"

Set BuildCmd=%VS110COMNTOOLS%..\IDE\devenv.com
"%BuildCmd%" "%~dp0%SourceDir%\%AppName%.sln" /Build "Release|Win32"
"%BuildCmd%" "%~dp0%SourceDir%\%AppName%.sln" /Build "Release|x64"

Echo ������֯�ļ�...

Md "%~dp0%ReleaseDir%"
Md "%~dp0%ReleaseDir%\Binaries"
Md "%~dp0%ReleaseDir%\Symbols"
Md "%~dp0%ReleaseDir%\Installer"

Copy "%~dp0%SourceDir%\bin\Win32\Release\GoogleHelper.dll"      "%~dp0%ReleaseDir%\Binaries\"
Copy "%~dp0%SourceDir%\bin\x64\Release\GoogleHelper.dll"        "%~dp0%ReleaseDir%\Binaries\GoogleHelper64.dll"
Copy "%~dp0%SourceDir%\bin\Win32\Release\GoogleHelperAgent.exe" "%~dp0%ReleaseDir%\Binaries\"

Copy "%~dp0%SourceDir%\bin\Win32\Release\GoogleHelper.pdb"      "%~dp0%ReleaseDir%\Symbols\"
Copy "%~dp0%SourceDir%\bin\x64\Release\GoogleHelper.pdb"        "%~dp0%ReleaseDir%\Symbols\GoogleHelper64.pdb"
Copy "%~dp0%SourceDir%\bin\Win32\Release\GoogleHelperAgent.pdb" "%~dp0%ReleaseDir%\Symbols\"

Echo ���ڱ��밲װ����...

"%PROGRAMFILES(x86)%\NSIS\Unicode\makensis.exe" /DVERSION=%Version% /DSOURCEDIR=%ReleaseDir%\Binaries /DOUTFILE=%ReleaseDir%\Installer\%AppName%%PVersion:.=%.exe "%~dp0Installer.nsi"

Echo ���ɽ���

Pause
