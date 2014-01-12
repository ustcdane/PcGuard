#if !defined(AFX_GUARDDLG_H__6921C162_C977_41D6_9694_B068F075257E__INCLUDED_)
#define AFX_GUARDDLG_H__6921C162_C977_41D6_9694_B068F075257E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuardDlg dialog

class CGuardDlg : public CDialog
{
// Construction
public:
	CGuardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGuardDlg)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuardDlg)
	afx_msg void OnButtonFirewall();
	afx_msg void OnButtonComputer();
	afx_msg void OnBUTTONmagnify();
	afx_msg void OnButtonNslookup();
	afx_msg void OnButtonPer();
	afx_msg void OnButtonService();
	afx_msg void OnButtonSysConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUARDDLG_H__6921C162_C977_41D6_9694_B068F075257E__INCLUDED_)
