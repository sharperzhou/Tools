; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

!include "LogicLib.nsh"
!include "nsDialogs.nsh"

; 安装程序初始定义常量
!define PRODUCT_NAME "AutoStart263EM"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "周晓春"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\AutoStart263em.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma
RequestExecutionLevel admin

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"
!include "FileFunc.nsh"
!include "newpage.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "AutoStart263EM.ico"
!define MUI_UNICON "Uninst.ico"

!define MUI_HEADERIMAGE 
!define MUI_HEADERIMAGE_RIGHT 
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r.bmp"
!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
;添加自定义页面
Page custom fnc_newpage_Create fnc_newpage_leave
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

;----------开始定义版本信息----------------
VIProductVersion "1.0.0.0"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "AutoStart263EM安装包"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "Copyright (C) 周晓春 2017.1"
;----------结束定义版本信息----------------


Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "AutoStart263EMSetup.exe"
InstallDir "$PROGRAMFILES\AutoStart263EM"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "AutoStart263EM安装向导"


Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "AutoStart263em.exe"
  File "Setting.exe"
  CreateDirectory "$SMPROGRAMS\AutoStart263EM"
  CreateShortCut "$SMPROGRAMS\AutoStart263EM\设置AutoStart263EM.lnk" "$INSTDIR\Setting.exe"
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\AutoStart263EM\卸载AutoStart263EM.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
	;计算安装文件大小
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0

  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\AutoStart263em.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\Setting.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "EstimatedSize" "$0"
  WriteRegDword HKLM "${PRODUCT_DIR_REGKEY}" "Installed" 1 ;禁止多次安装标记
  
  #AutoStart263em注册表项目
  StrCpy $RegString "$\"$INSTDIR\AutoStart263em.exe$\" $RegString"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "AutoStart263em" $RegString
SectionEnd

Function .onInit
	InitPluginsDir
  ;防止重复运行安装程序
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "AutoStart263EM") i .r1 ?e'
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
FunctionEnd

/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/



Section Uninstall

  ; 清除安装程序创建的且在卸载时可能为空的子目录，对于递归添加的文件目录，请由最内层的子目录开始清除(注意，不要带 /r 参数，否则会失去 DelFileByLog 的意义)

	Killer::KillProcess "AutoStart263em.exe"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\Setting.exe"
  Delete "$INSTDIR\AutoStart263em.exe"
  
  Delete "$SMPROGRAMS\AutoStart263EM\卸载AutoStart263EM.lnk"
  Delete "$SMPROGRAMS\AutoStart263EM\设置AutoStart263EM.lnk"

	RMDir "$SMPROGRAMS\AutoStart263EM"
  RMDir "$INSTDIR"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "AutoStart263em"
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 $(^Name) ，及其所有的组件？" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
# HideWindow
# MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。"
FunctionEnd

