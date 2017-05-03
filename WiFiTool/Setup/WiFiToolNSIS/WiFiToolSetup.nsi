; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "WiFi����"
!define PRODUCT_VERSION "2.0"
!define PRODUCT_PUBLISHER "������"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\WifiTool.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma
RequestExecutionLevel admin

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"
!include "FileFunc.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "E:\��װ����\WiFiToolNSIS\Setup.ico"
!define MUI_UNICON "E:\��װ����\WiFiToolNSIS\Uninst.ico"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r.bmp"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ���ѡ��ҳ��
!insertmacro MUI_PAGE_COMPONENTS
; ��װĿ¼ѡ��ҳ��
!insertmacro MUI_PAGE_DIRECTORY
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
;!define MUI_FINISHPAGE_RUN "$INSTDIR\WifiTool.exe"
;!define MUI_FINISHPAGE_NOAUTOCLOSE
;!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

;----------��ʼ����汾��Ϣ----------------
VIProductVersion "2.0.0.0"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "WiFi���߰�װ��"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion"  "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "Copyright (C) ������ 2016.1"
;----------��������汾��Ϣ----------------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "WiFiToolSetup.exe"
InstallDir "$PROGRAMFILES\WiFiTool"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "WiFi���߰�װ��"

Section -AdditionalIcons
	CreateDirectory "$SMPROGRAMS\WiFiTool"
  CreateShortCut "$SMPROGRAMS\WiFiTool\ж��${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe"
SectionEnd

SectionGroup /e "WiFi����" SEC01
	Section -
		DetailPrint "��ʾ�����ڰ�װWiFi����..."
	  SetOutPath "$INSTDIR"
	  SetOverwrite ifnewer
	  File "E:\��װ����\WiFiToolNSIS\WifiTool.exe"
	  ;CreateDirectory "$SMPROGRAMS\WiFiTool"
	  CreateShortCut "$SMPROGRAMS\WiFiTool\WiFi����.lnk" "$INSTDIR\WifiTool.exe"
  SectionEnd
  Section "��ݷ�ʽ" SEC001
  	CreateShortCut "$DESKTOP\WiFi����.lnk" "$INSTDIR\WifiTool.exe"
  SectionEnd
  Section -
	  DetailPrint "��ʾ��WiFi�����ѳɹ���װ"
	SectionEnd
SectionGroupEnd

Section "WiFi���߷���" SEC02
  DetailPrint "��ʾ�����ڰ�װWiFi���߷���..."
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "E:\��װ����\WiFiToolNSIS\WifiToolService.exe"
	services::IsServiceInstalled 'WiFi Tool Service'
	Pop $0
	${if} $0 == 'Yes'
		DetailPrint "��ʾ������WiFi���߷����Ѱ�װ����ж�ط�����ٰ�װ"
	  services::SendServiceCommand 'stop' 'WiFi Tool Service'
	  services::SendServiceCommand 'delete' 'WiFi Tool Service'
	  DetailPrint "��ʾ���ɰ汾�����Ѿ�ж����ϣ���������װ�°汾����"
	${endif}
	;SimpleSC::InstallService "WiFi Tool Service" "WiFi Tool Service" "16" "2" "$INSTDIR\WifiToolService.exe" "" "" ""
  nsExec::Exec 'cmd /c sc create "WiFi Tool Service" binpath= "$INSTDIR\WifiToolService.exe" displayname= "WiFi Tool Service" start= delayed-auto'
  SimpleSC::SetServiceDescription "WiFi Tool Service" "WiFi���߷������ڿ����Զ���WiFi���������Զ����ӵ�Widows����"
  services::SendServiceCommand 'start' 'WiFi Tool Service'
	services::IsServiceInstalled 'WiFi Tool Service'
	Pop $0
	${if} $0 == 'Yes'
		DetailPrint "��ʾ��WiFi���߷����ѳɹ���װ"
	${else}
	  DetailPrint "��ʾ��WiFi���߷���װʧ��"
	${endif}
SectionEnd

Section -Post
	;���㰲װ�ļ���С
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
  WriteRegDword HKLM "${PRODUCT_DIR_REGKEY}" "Installed" 1 ;��ֹ��ΰ�װ���
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

; �����������
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "�����ֶ������͹ر�WiFi��������WiFi���߷���"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC001} "����ΪWiFi���ߴ��������ݷ�ʽ��"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "���ڿ����Զ�����WiFi���������Զ��������ӡ�"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit
	InitPluginsDir
  ;��ֹ�ظ����а�װ����
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "WiFi Tool") i .r1 ?e'
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
	;����ϵͳ�汾
	ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
	StrCpy $R1 $R0 1
	StrCmp $R1 "6" 0 notNT6
	StrCpy $R2 $R0 "" 2
	IntCmp $R2 "0" stopinst stopinst startinst
	notNT6:
  IntCmp $R0 "6" stopinst stopinst startinst
	stopinst:
	MessageBox MB_OK "�˳����֧��Windows 7��Windows Server 2008 R2���ϰ汾�Ĳ���ϵͳ��$\n$\n���� [ȷ��] ��ť�˳��˰�װ����"
	Abort
	startinst:
FunctionEnd


/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
	services::IsServiceInstalled 'WiFi Tool Service'
	Pop $0
	${if} $0 == 'Yes'
	  services::SendServiceCommand 'stop' 'WiFi Tool Service'
	  DetailPrint "��ʾ������ֹͣWiFi���߷���..."
	  services::SendServiceCommand 'delete' 'WiFi Tool Service'
	  DetailPrint "��ʾ��WiFi���߷����ѳɹ�ж��"
	${endif}
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\WifiToolService.exe"
  Delete "$INSTDIR\WifiToolService.log"
  Delete "$INSTDIR\WifiTool.exe"

  Delete "$SMPROGRAMS\WiFiTool\ж��${PRODUCT_NAME}.lnk"
  Delete "$DESKTOP\WiFi����.lnk"
  Delete "$SMPROGRAMS\WiFiTool\WiFi����.lnk"

  RMDir "$SMPROGRAMS\WiFiTool"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  ;SetAutoClose true
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  ;HideWindow
  ;MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd
