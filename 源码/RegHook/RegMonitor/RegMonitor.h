// RegMonitor.h : main header file for the REGMONITOR application
//

#if !defined(AFX_REGMONITOR_H__B84AC592_127B_49A0_BE62_2572CB9DFF04__INCLUDED_)
#define AFX_REGMONITOR_H__B84AC592_127B_49A0_BE62_2572CB9DFF04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegMonitorApp:
// See RegMonitor.cpp for the implementation of this class
//

class CRegMonitorApp : public CWinApp
{
public:
	CRegMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRegMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMONITOR_H__B84AC592_127B_49A0_BE62_2572CB9DFF04__INCLUDED_)
