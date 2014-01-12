// PcGuardDlg.h : header file
//

#if !defined(AFX_PCGUARDDLG_H__D2E7D285_AF26_49BC_88AF_15F219AC8B1B__INCLUDED_)
#define AFX_PCGUARDDLG_H__D2E7D285_AF26_49BC_88AF_15F219AC8B1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_TRAYICON_MSG WM_USER+11
/////////////////////////////////////////////////////////////////////////////
// CPcGuardDlg dialog

class CPcGuardDlg : public CDialog
{
// Construction
public:
	int m_nScrollXPos;
	CString m_sText;
	//用于保存超链接静态文本框的屏幕坐标 
	RECT m_pRectLink;
	// 任务托盘
	LRESULT OnTrayCallBackMsg(WPARAM wparam, LPARAM lparam);
	NOTIFYICONDATA m_Ntnd;//任务托盘有关结构
	// 托盘 默认托盘
	bool TrayMyIcon(bool bAdd=true);
	CPcGuardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPcGuardDlg)
	enum { IDD = IDD_PCGUARD_DIALOG };
	CButton	m_BtKill; // 杀毒按钮
	CButton	m_BtReg;// 注册表监控按钮
	CButton	m_BtFile;// 文件监控按钮
	CButton	m_BtPro;// 进程管理按钮
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcGuardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPcGuardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// 处理按钮事件
	afx_msg void OnButtonPro(); 
	afx_msg void OnButtonKill();
	afx_msg void OnButtonReg();
	afx_msg void OnButtonFile();
	// 计时器事件
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	// 处理托盘菜单显示消息
	afx_msg void OnMenuShow();
	afx_msg void OnMenuQuit();
	// 鼠标消息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// 鼠标左单击消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCGUARDDLG_H__D2E7D285_AF26_49BC_88AF_15F219AC8B1B__INCLUDED_)
