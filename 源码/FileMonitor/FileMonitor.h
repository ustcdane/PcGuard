// FileMonitor.h : main header file for the FILEMONITOR application
//

#if !defined(AFX_FILEMONITOR_H__012DA518_F8F5_4CCB_A0FA_13607FE50468__INCLUDED_)
#define AFX_FILEMONITOR_H__012DA518_F8F5_4CCB_A0FA_13607FE50468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileMonitorApp:
// See FileMonitor.cpp for the implementation of this class
//

class CFileMonitorApp : public CWinApp
{
public:
	CFileMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMONITOR_H__012DA518_F8F5_4CCB_A0FA_13607FE50468__INCLUDED_)
