#if !defined(AFX_UPGRADEDLG_H__9A897D66_140C_4B25_AA67_E0FA99820D30__INCLUDED_)
#define AFX_UPGRADEDLG_H__9A897D66_140C_4B25_AA67_E0FA99820D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpgradeDlg.h : header file
//
class CKillerDlg;
/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog

class CUpgradeDlg : public CDialog
{
// Construction
public:
	CUpgradeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUpgradeDlg)
	enum { IDD = IDD_DIALOG_UPGRADE };
	long	m_OffSet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpgradeDlg)
	afx_msg void OnButtonOk();
	afx_msg void OnStatUp();
	afx_msg void OnButtonMail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CKillerDlg *m_pCKillerDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADEDLG_H__9A897D66_140C_4B25_AA67_E0FA99820D30__INCLUDED_)
