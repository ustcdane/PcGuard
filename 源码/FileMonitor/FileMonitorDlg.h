// FileMonitorDlg.h : header file
//

#if !defined(AFX_FILEMONITORDLG_H__0D2E531E_1010_4904_AFF4_B827CA66DD0C__INCLUDED_)
#define AFX_FILEMONITORDLG_H__0D2E531E_1010_4904_AFF4_B827CA66DD0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CFileMonitorDlg dialog
class CFileMonitorDlg : public CDialog
{
// Construction
public:
	CFileMonitorDlg(CWnd* pParent = NULL);	// standard constructor
	~CFileMonitorDlg();
// Dialog Data
	//{{AFX_DATA(CFileMonitorDlg)
	enum { IDD = IDD_FILEMONITOR_DIALOG };
	CButton	m_BtSet;
	CListCtrl	m_CListCtrl;
	CString	m_strPathName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    HICON m_hIconSet;//设置按钮图标句柄
	// Generated message map functions
	//{{AFX_MSG(CFileMonitorDlg)
	virtual BOOL OnInitDialog();//初始化
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();//开始监视
	afx_msg void OnButtonStop();//停止监视
	afx_msg void OnButtonClear();//清楚列表记录
	afx_msg void OnButtonSave();//保存监视记录
	afx_msg void OnButtonExport();//导出监视记录
	afx_msg void OnBtnBrowse();//浏览磁盘
	afx_msg void OnButtonSet();//设置
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);//右键消息处理
	afx_msg void OnOpen();//右键菜单 打开消息处理
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



public:
	HMODULE m_Hmodule;//皮肤模块句柄
		//以下成员为判断是否加入监视
	BOOL	m_bAddNew;//新增
	BOOL	m_bDel;//删除
	BOOL	m_bRename;//重命名
	BOOL	m_bAttribute;//属性修改
	BOOL    m_bOccurTime;//发生时间
	BOOL	m_bOther;//其它

	void InitDlgItem();//初始化控件
	//线程函数，用来监视
	static DWORD WINAPI ThreadMonitorProc(LPVOID lParam);
	CFont		m_hFont;
	HANDLE m_hThread; // 线程句柄
	HANDLE m_hDir; // 存储选择的目录句柄
	int m_nNumber;  // 变化文件或文件夹的序号
	char* m_cNumber; // 变化文件或文件夹的序号

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMONITORDLG_H__0D2E531E_1010_4904_AFF4_B827CA66DD0C__INCLUDED_)
