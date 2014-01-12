; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRegMonitorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "RegMonitor.h"

ClassCount=3
Class1=CRegMonitorApp
Class2=CRegMonitorDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDR_MAINFRAME
Resource2=IDD_REGMONITOR_DIALOG
Resource3=IDR_MENU1

[CLS:CRegMonitorApp]
Type=0
HeaderFile=RegMonitor.h
ImplementationFile=RegMonitor.cpp
Filter=N

[CLS:CRegMonitorDlg]
Type=0
HeaderFile=RegMonitorDlg.h
ImplementationFile=RegMonitorDlg.cpp
Filter=D
LastObject=IDC_BUTTON_CLEAR
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=RegMonitorDlg.h
ImplementationFile=RegMonitorDlg.cpp
Filter=D

[DLG:IDD_REGMONITOR_DIALOG]
Type=1
Class=CRegMonitorDlg
ControlCount=7
Control1=IDC_BUTTON_START,button,1342242816
Control2=IDC_LIST,SysListView32,1350631424
Control3=IDC_BUTTON_STOP,button,1342242816
Control4=IDC_STATIC_TIP2,static,1342308352
Control5=IDC_STATIC_TIP1,static,1342308352
Control6=IDC_BUTTON_SAVE,button,1342242816
Control7=IDC_BUTTON_CLEAR,button,1342242816

[MNU:IDR_MENU1]
Type=1
Class=CRegMonitorDlg
Command1=IDR_MENU11
Command2=IDR_MENU12
Command3=IDR_MENU13
Command4=IDR_MENU14
Command5=IDR_MENU15
Command6=IDR_MENU21
Command7=IDR_MENU22
CommandCount=7

