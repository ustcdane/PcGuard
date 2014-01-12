; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPcGuardDlg
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PcGuard.h"

ClassCount=4
Class1=CPcGuardApp
Class2=CPcGuardDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=COpenScreenWnd
Resource3=IDD_PCGUARD_DIALOG
Resource4=IDR_MENU1

[CLS:CPcGuardApp]
Type=0
HeaderFile=PcGuard.h
ImplementationFile=PcGuard.cpp
Filter=N

[CLS:CPcGuardDlg]
Type=0
HeaderFile=PcGuardDlg.h
ImplementationFile=PcGuardDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LINK

[CLS:CAboutDlg]
Type=0
HeaderFile=PcGuardDlg.h
ImplementationFile=PcGuardDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PCGUARD_DIALOG]
Type=1
Class=CPcGuardDlg
ControlCount=15
Control1=IDC_BUTTON_PRO,button,1342243584
Control2=IDC_BUTTON_REG,button,1342242816
Control3=IDC_BUTTON_FILE,button,1342242816
Control4=IDC_BUTTON_KILL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC_PRO,static,1342308352
Control11=IDC_STATIC_REG,static,1342308352
Control12=IDC_STATIC_FILE,static,1342308352
Control13=IDC_STATIC_KILL,static,1342308352
Control14=IDC_LINK,static,1342308353
Control15=IDC_STATIC_MARQUE,static,1342308352

[CLS:COpenScreenWnd]
Type=0
HeaderFile=OpenScreenWnd.h
ImplementationFile=OpenScreenWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC

[MNU:IDR_MENU1]
Type=1
Class=CPcGuardDlg
Command1=ID_MENU_SHOW
Command2=ID_MENU_QUIT
CommandCount=2

