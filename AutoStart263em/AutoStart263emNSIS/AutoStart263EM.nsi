; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

!include "LogicLib.nsh"
!include "nsDialogs.nsh"

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "AutoStart263EM"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "������"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\AutoStart263em.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma
RequestExecutionLevel admin

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"
!include "FileFunc.nsh"
!include "newpage.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "AutoStart263EM.ico"
!define MUI_UNICON "Uninst.ico"

!define MUI_HEADERIMAGE 
!define MUI_HEADERIMAGE_RIGHT 
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r.bmp"
!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ��װĿ¼ѡ��ҳ��
!insertmacro MUI_PAGE_DIRECTORY
;����Զ���ҳ��
Page custom fnc_newpage_Create fnc_newpage_leave
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

;----------��ʼ����汾��Ϣ----------------
VIProductVersion "1.0.0.0"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "AutoStart263EM��װ��"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "Copyright (C) ������ 2017.1"
;----------��������汾��Ϣ----------------


Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "AutoStart263EMSetup.exe"
InstallDir "$PROGRAMFILES\AutoStart263EM"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "AutoStart263EM��װ��"


Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "AutoStart263em.exe"
  File "Setting.exe"
  CreateDirectory "$SMPROGRAMS\AutoStart263EM"
  CreateShortCut "$SMPROGRAMS\AutoStart263EM\����AutoStart263EM.lnk" "$INSTDIR\Setting.exe"
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\AutoStart263EM\ж��AutoStart263EM.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
	;���㰲װ�ļ���С
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
  WriteRegDword HKLM "${PRODUCT_DIR_REGKEY}" "Installed" 1 ;��ֹ��ΰ�װ���
  
  #AutoStart263emע�����Ŀ
  StrCpy $RegString "$\"$INSTDIR\AutoStart263em.exe$\" $RegString"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "AutoStart263em" $RegString
SectionEnd

Function .onInit
	InitPluginsDir
  ;��ֹ�ظ����а�װ����
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "AutoStart263EM") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "����һ�� $(^Name) ��װʵ������ͬ�ظ�ִ�У�"
    Abort
	;��ֹ��ΰ�װ
	ReadRegDWORD $0 HKLM '${PRODUCT_DIR_REGKEY}' "Installed"
	IntCmp $0 +1 +4
	MessageBox MB_OK|MB_USERICON '$(^Name) �Ѱ�װ�ڼ�����С�$\n�������°�װ����ж�����еİ�װ��'
	Quit
	nop
FunctionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/



Section Uninstall

  ; �����װ���򴴽�������ж��ʱ����Ϊ�յ���Ŀ¼�����ڵݹ���ӵ��ļ�Ŀ¼���������ڲ����Ŀ¼��ʼ���(ע�⣬��Ҫ�� /r �����������ʧȥ DelFileByLog ������)

	Killer::KillProcess "AutoStart263em.exe"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\Setting.exe"
  Delete "$INSTDIR\AutoStart263em.exe"
  
  Delete "$SMPROGRAMS\AutoStart263EM\ж��AutoStart263EM.lnk"
  Delete "$SMPROGRAMS\AutoStart263EM\����AutoStart263EM.lnk"

	RMDir "$SMPROGRAMS\AutoStart263EM"
  RMDir "$INSTDIR"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "AutoStart263em"
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
# HideWindow
# MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd

