; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "WiFi工具"
#define MyAppVersion "2.0"
#define MyAppPublisher "周晓春"
#define MyAppURL ""
#define MyAppExeName "WifiTool.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
PrivilegesRequired=admin
AppId={{8A7F7AB7-7A2C-4710-8FAF-9E883E343AF8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\WiFiTool
DefaultGroupName=WiFiTool
AllowNoIcons=yes
OutputDir=.
OutputBaseFilename=WiFiToolSetup
Compression=lzma
SolidCompression=yes
;版本信息
VersionInfoCompany=
VersionInfoDescription= "WiFi工具安装包"
VersionInfoVersion= 2.0
VersionInfoCopyright= "Copyright (C) 周晓春 2016.1"

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce ;Components:GUI

[Files]
Source: ".\WifiTool.exe"; DestDir: "{app}"; Flags: ignoreversion ;Components:GUI 
Source: ".\WifiToolService.exe"; DestDir: "{app}"; Flags: ignoreversion ;Components:Service
Source: ".\InstallService.bat"; DestDir: "{app}"; Flags: ignoreversion ;Components:Service
Source: ".\UninstallService.bat"; DestDir: "{app}"; Flags: ignoreversion ;Components:Service
Source: ".\WifiToolService.log"; DestDir: "{app}"; Flags: ignoreversion ;Components:Service
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"  ;Components:GUI
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent ;Components:GUI 
Filename: "{app}\InstallService.bat"; Description: "安装 WiFi工具服务"; Flags:  shellexec runhidden nowait postinstall skipifsilent ;Components:Service

[UninstallRun]
Filename: "{app}\UninstallService.bat"; Flags: runhidden ;Components:Service



[Components] 
Name: GUI; Description: "WiFi工具"; Types:full
Name: Service; Description: "WiFi工具服务"; Types:full