; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKillerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VirusKiller.h"

ClassCount=6
Class1=CVirusKillerApp
Class2=CVirusKillerDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_PROPPAGE_LARGE (English (U.S.))
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG2
Class4=CKillerDlg
Resource4=IDD_DIALOG_UPGRADE
Class5=CGuardDlg
Resource5=IDD_VIRUSKILLER_DIALOG
Class6=CUpgradeDlg
Resource6=IDD_DIALOG1
Resource7=IDR_MENU1

[CLS:CVirusKillerApp]
Type=0
HeaderFile=VirusKiller.h
ImplementationFile=VirusKiller.cpp
Filter=N

[CLS:CVirusKillerDlg]
Type=0
HeaderFile=VirusKillerDlg.h
ImplementationFile=VirusKillerDlg.cpp
Filter=D
LastObject=IDC_TAB1
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=VirusKillerDlg.h
ImplementationFile=VirusKillerDlg.cpp
Filter=D

[DLG:IDD_PROPPAGE_LARGE (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_VIRUSKILLER_DIALOG]
Type=1
Class=CVirusKillerDlg
ControlCount=2
Control1=IDC_TAB1,SysTabControl32,1342177280
Control2=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG1]
Type=1
Class=CKillerDlg
ControlCount=13
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BUTTON_SELECT,button,1342242816
Control3=IDC_EDIT_PATH,edit,1350631552
Control4=IDC_BUTTON_START,button,1342242816
Control5=IDC_STATIC_TOTAL,static,1342308352
Control6=IDC_STATIC_VIR_NUM,static,1342308352
Control7=IDC_STATIC_KILLED_NUM,static,1342308352
Control8=IDC_STATIC_ERR,static,1342308352
Control9=IDC_BUTTON_STOP,button,1342259200
Control10=IDC_EDIT_SCAN,edit,1350633476
Control11=IDC_EDIT_SAFE,edit,1350633604
Control12=IDC_LIST1,SysListView32,1350631424
Control13=IDC_BUTTON_CLEAR,button,1342242816

[CLS:CKillerDlg]
Type=0
HeaderFile=KillerDlg.h
ImplementationFile=KillerDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_CLEAR
VirtualFilter=dWC

[CLS:CGuardDlg]
Type=0
HeaderFile=GuardDlg.h
ImplementationFile=GuardDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_SYS_CONFIG
VirtualFilter=dWC

[CLS:CUpgradeDlg]
Type=0
HeaderFile=UpgradeDlg.h
ImplementationFile=UpgradeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CUpgradeDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CUpgradeDlg
ControlCount=9
Control1=IDC_STATIC,button,1342178055
Control2=IDC_BUTTON_FIREWALL,button,1342242816
Control3=IDC_BUTTON_SYS_CONFIG,button,1342242816
Control4=IDC_BUTTON_PER,button,1342242816
Control5=IDC_BUTTON_NSLOOKUP,button,1342242816
Control6=IDC_BUTTON_COMPUTER,button,1342242816
Control7=IDC_BUTTON_magnify,button,1342242816
Control8=IDC_BUTTON_SERVICE,button,1342242816
Control9=IDC_STATIC,button,1342178055

[DLG:IDD_DIALOG_UPGRADE]
Type=1
Class=CGuardDlg
ControlCount=17
Control1=id_static,button,1342177287
Control2=IDC_STATIC_VIRUS_NAME,static,1342308352
Control3=IDC_STATIC_LEN,static,1342308352
Control4=IDC_STATIC_EXTNAME,static,1342308352
Control5=IDC_STATIC_VIRUS_DATA,static,1342308352
Control6=IDC_EDIT_VIRUS_NAME,edit,1350631552
Control7=IDC_EDIT_LEN,edit,1350631552
Control8=IDC_EDIT_EXTNAME,edit,1350631552
Control9=IDC_EDIT_VIRUS_DATA,edit,1350631552
Control10=IDC_STATIC_OFFSET,static,1342308352
Control11=IDC_EDIT_OFFSET,edit,1350631552
Control12=IDC_BUTTON_OK,button,1342242816
Control13=IDC_STATIC_UP,static,1342308608
Control14=IDC_STAT_UP,static,1342177539
Control15=IDC_BUTTON_MAIL,button,1342259968
Control16=IDC_STATIC,static,1342308353
Control17=IDC_STATIC,static,1342308352

[MNU:IDR_MENU1]
Type=1
Class=CKillerDlg
Command1=ID_MENUE_OPEN
Command2=ID_MENUE_DEL
CommandCount=2

