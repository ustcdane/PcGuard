// PcGuard.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PcGuard.h"
#include "PcGuardDlg.h"
#include "OpenScreenWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPcGuardApp

BEGIN_MESSAGE_MAP(CPcGuardApp, CWinApp)
	//{{AFX_MSG_MAP(CPcGuardApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcGuardApp construction

CPcGuardApp::CPcGuardApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPcGuardApp object

CPcGuardApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPcGuardApp initialization

BOOL CPcGuardApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// 添加程序启动时的动画窗口
	COpenScreenWnd* imageWnd = new COpenScreenWnd;  //建立一个新窗口对象
	imageWnd->CreatScreenWnd ();			//创建窗口
	imageWnd->CenterWindow ();				//在屏幕中央
	imageWnd->ShowWindow (SW_SHOW);			//显示窗口
	imageWnd->UpdateWindow();				//更新窗口，激活OnPait函数
	if (imageWnd != NULL)
	{
		imageWnd->SendMessage (WM_CLOSE); //关闭窗口
	}
	delete imageWnd;

	CPcGuardDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
