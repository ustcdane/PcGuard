// PcGuard.h : main header file for the PCGUARD application
//

#if !defined(AFX_PCGUARD_H__ABAB13F8_EDDF_485E_B2EF_AD5EB6B3C421__INCLUDED_)
#define AFX_PCGUARD_H__ABAB13F8_EDDF_485E_B2EF_AD5EB6B3C421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPcGuardApp:
// See PcGuard.cpp for the implementation of this class
//

class CPcGuardApp : public CWinApp
{
public:
	CPcGuardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcGuardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPcGuardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCGUARD_H__ABAB13F8_EDDF_485E_B2EF_AD5EB6B3C421__INCLUDED_)
