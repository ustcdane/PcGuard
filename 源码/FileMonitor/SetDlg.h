#if !defined(AFX_SETDLG_H__C517A181_ABA4_4A39_B894_96000EC51EDA__INCLUDED_)
#define AFX_SETDLG_H__C517A181_ABA4_4A39_B894_96000EC51EDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog

class CSetDlg : public CDialog
{
// Construction
public:
	CSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDlg)
	enum { IDD = IDD_DIALOG_SET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	public:
	//以下成员为判断是否加入监视
	BOOL	m_bAddNew;//新增
	BOOL	m_bDel;//删除
	BOOL	m_bRename;//重命名
	BOOL	m_bAttribute;//属性修改
	BOOL    m_bOccurTime;//发生时间
	BOOL	m_bOther;//其它

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSetDlg)
	afx_msg void OnBtSure();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDLG_H__C517A181_ABA4_4A39_B894_96000EC51EDA__INCLUDED_)
