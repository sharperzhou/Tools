; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

; 安装程序初始定义常量
!define PRODUCT_NAME "WiFi工具"
!define PRODUCT_VERSION "2.0"
!define PRODUCT_PUBLISHER "周晓春"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\WifiTool.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma
RequestExecutionLevel admin

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"
!include "FileFunc.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "E:\安装制作\WiFiToolNSIS\Setup.ico"
!define MUI_UNICON "E:\安装制作\WiFiToolNSIS\Uninst.ico"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r.bmp"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 组件选择页面
!insertmacro MUI_PAGE_COMPONENTS
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
;!define MUI_FINISHPAGE_RUN "$INSTDIR\WifiTool.exe"
;!define MUI_FINISHPAGE_NOAUTOCLOSE
;!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

;----------开始定义版本信息----------------
VIProductVersion "2.0.0.0"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "WiFi工具安装包"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "Copyright (C) 周晓春 2016.1"
;----------结束定义版本信息----------------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "WiFiToolSetup.exe"
InstallDir "$PROGRAMFILES\WiFiTool"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "WiFi工具安装向导"

Section -AdditionalIcons
	CreateDirectory "$SMPROGRAMS\WiFiTool"
  CreateShortCut "$SMPROGRAMS\WiFiTool\卸载${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe"
SectionEnd

SectionGroup /e "WiFi工具" SEC01
	Section -
		DetailPrint "提示：正在安装WiFi工具..."
	  SetOutPath "$INSTDIR"
	  SetOverwrite ifnewer
	  File "E:\安装制作\WiFiToolNSIS\WifiTool.exe"
	  ;CreateDirectory "$SMPROGRAMS\WiFiTool"
	  CreateShortCut "$SMPROGRAMS\WiFiTool\WiFi工具.lnk" "$INSTDIR\WifiTool.exe"
  SectionEnd
  Section "快捷方式" SEC001
  	CreateShortCut "$DESKTOP\WiFi工具.lnk" "$INSTDIR\WifiTool.exe"
  SectionEnd
  Section -
	  DetailPrint "提示：WiFi工具已成功安装"
	SectionEnd
SectionGroupEnd

Section "WiFi工具服务" SEC02
  DetailPrint "提示：正在安装WiFi工具服务..."
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "E:\安装制作\WiFiToolNSIS\WifiToolService.exe"
	services::IsServiceInstalled 'WiFi Tool Service'
	Pop $0
	${if} $0 == 'Yes'
		DetailPrint "提示：发现WiFi工具服务已安装，需卸载服务后再安装"
	  services::SendServiceCommand 'stop' 'WiFi Tool Service'
	  services::SendServiceCommand 'delete' 'WiFi Tool Service'
	  DetailPrint "提示：旧版本服务已经卸载完毕，接下来安装新版本服务"
	${endif}
	;SimpleSC::InstallService "WiFi Tool Service" "WiFi Tool Service" "16" "2" "$INSTDIR\WifiToolService.exe" "" "" ""
  nsExec::Exec 'cmd /c sc create "WiFi Tool Service" binpath= "$INSTDIR\WifiToolService.exe" displayname= "WiFi Tool Service" start= delayed-auto'
  SimpleSC::SetServiceDescription "WiFi Tool Service" "WiFi工具服务，用于开机自动打开WiFi共享、掉线自动连接的Widows服务。"
  services::SendServiceCommand 'start' 'WiFi Tool Service'
	services::IsServiceInstalled 'WiFi Tool Service'
	Pop $0
	${if} $0 == 'Yes'
		DetailPrint "提示：WiFi工具服务已成功安装"
	${else}
	  DetailPrint "提示：WiFi工具服务安装失败"
	${endif}
SectionEnd

Section -Post
	;计算安装文件大小
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0

  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\WifiTool.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\WifiTool.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "EstimatedSize" "$0"
  WriteRegDword HKLM "${PRODUCT_DIR_REGKEY}" "Installed" 1 ;禁止多次安装标记
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

; 区段组件描述
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "用于手动开启和关闭WiFi共享，管理WiFi工具服务。"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC001} "用于为WiFi工具创建桌面快捷方式。"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "用于开机自动开启WiFi共享，掉线自动重新连接。"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit
	InitPluginsDir
  ;防止重复运行安装程序
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "WiFi Tool") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "已有一个 $(^Name) 安装实例，不同重复执行！"
    Abort
	;禁止多次安装
	ReadRegDWORD $0 HKLM '${PRODUCT_DIR_REGKEY}' "Installed"
	IntCmp $0 +1 +4
	MessageBox MB_OK|MB_USERICON '$(^Name) 已安装在计算机中。$\n如需重新安装，请卸载已有的安装。'
	Quit
	nop
	;操作系统版本
	ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
	StrCpy $R1 $R0 1
	StrCmp $R1 "6" 0 notNT6
	StrCpy $R2 $R0 "" 2
	IntCmp $R2 "0" stopinst stopinst startinst
	notNT6:
  IntCmp $R0 "6" stopinst stopinst startinst
	stopinst:
	MessageBox MB_OK "此程序仅支持Windows 7或Windows Server 2008 R2以上版本的操作系统！$\n$\n请点击 [确定] 按钮退出此安装程序。"
	Abort
	startinst:
FunctionEnd


/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/

Section Uninstall
	services::IsServiceInstalled 'WiFi Tool Service'
	Pop $0
	${if} $0 == 'Yes'
	  services::SendServiceCommand 'stop' 'WiFi Tool Service'
	  DetailPrint "提示：正在停止WiFi工具服务..."
	  services::SendServiceCommand 'delete' 'WiFi Tool Service'
	  DetailPrint "提示：WiFi工具服务已成功卸载"
	${endif}
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\WifiToolService.exe"
  Delete "$INSTDIR\WifiToolService.log"
  Delete "$INSTDIR\WifiTool.exe"

  Delete "$SMPROGRAMS\WiFiTool\卸载${PRODUCT_NAME}.lnk"
  Delete "$DESKTOP\WiFi工具.lnk"
  Delete "$SMPROGRAMS\WiFiTool\WiFi工具.lnk"

  RMDir "$SMPROGRAMS\WiFiTool"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  ;SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 $(^Name) ，及其所有的组件？" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  ;HideWindow
  ;MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。"
FunctionEnd
