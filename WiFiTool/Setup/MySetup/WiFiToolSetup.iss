; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "WiFi����"
#define MyAppVersion "2.0"
#define MyAppPublisher "������"
#define MyAppURL ""
#define MyAppExeName "WifiTool.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
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
;�汾��Ϣ
VersionInfoCompany=
VersionInfoDescription= "WiFi���߰�װ��"
VersionInfoVersion= 2.0
VersionInfoCopyright= "Copyright (C) ������ 2016.1"

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
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"  ;Components:GUI
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent ;Components:GUI 
Filename: "{app}\InstallService.bat"; Description: "��װ WiFi���߷���"; Flags:  shellexec runhidden nowait postinstall skipifsilent ;Components:Service

[UninstallRun]
Filename: "{app}\UninstallService.bat"; Flags: runhidden ;Components:Service



[Components] 
Name: GUI; Description: "WiFi����"; Types:full
Name: Service; Description: "WiFi���߷���"; Types:full