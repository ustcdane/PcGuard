// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B1B71F5C_EAA0_4F01_A037_4EE268D851EF__INCLUDED_)
#define AFX_STDAFX_H__B1B71F5C_EAA0_4F01_A037_4EE268D851EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x0500
//ReadDirectoryChangesW这个函数只在WinNT中能够使用，是新加的一个API函数，所以在VC++中加入#define _WIN32_WINNT 0x0500

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B1B71F5C_EAA0_4F01_A037_4EE268D851EF__INCLUDED_)
