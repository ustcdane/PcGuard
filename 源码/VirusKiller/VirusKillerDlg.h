// VirusKillerDlg.h : header file
//
#include "KillerDlg.h"
#include "GuardDlg.h"
#include "UpgradeDlg.h"
#if !defined(AFX_VIRUSKILLERDLG_H__24959FB3_C2A0_4DC3_BE1E_9338BCF6E3B7__INCLUDED_)
#define AFX_VIRUSKILLERDLG_H__24959FB3_C2A0_4DC3_BE1E_9338BCF6E3B7__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVirusKillerDlg dialog

class CVirusKillerDlg : public CDialog
{
// Construction
public:
	// Í¼ÏñÁÐ±í
	CImageList m_ImageList;
	int m_nsltIndex;
	CKillerDlg  m_page1;
	CGuardDlg   m_page2;
	CUpgradeDlg m_page3;
	bool InitItem();
	CDialog * m_pDialog[3];
	CVirusKillerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVirusKillerDlg)
	enum { IDD = IDD_VIRUSKILLER_DIALOG };
	CTabCtrl	m_tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirusKillerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVirusKillerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRUSKILLERDLG_H__24959FB3_C2A0_4DC3_BE1E_9338BCF6E3B7__INCLUDED_)
