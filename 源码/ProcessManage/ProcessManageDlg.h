// ProcessManageDlg.h : header file
//

#if !defined(AFX_PROCESSMANAGEDLG_H__A3F29421_DE79_4D52_816F_5D080647CB10__INCLUDED_)
#define AFX_PROCESSMANAGEDLG_H__A3F29421_DE79_4D52_816F_5D080647CB10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxcmn.h>
#include <Tlhelp32.h>//系统快照
#include <psapi.h>//PSAPI
#pragma comment (lib, "psapi.lib")
/////////////////////////////////////////////////////////////////////////////
// CProcessManageDlg dialog

class CProcessManageDlg : public CDialog
{
// Construction
public:
	CProcessManageDlg(CWnd* pParent = NULL);	// standard constructor
    ~CProcessManageDlg();
// Dialog Data
	//{{AFX_DATA(CProcessManageDlg)
	enum { IDD = IDD_PROCESSMANAGE_DIALOG };
	CListCtrl	m_ListDll;//模块信息列表控件关联变量
	CListCtrl	m_ListProcess;//进程列表控件关联变量
	CString	m_nDll;//包含的模块数量静态 文本控件 关联变量
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessManageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProcessManageDlg)
	virtual BOOL OnInitDialog();//初始化
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//定时器用于定时更新系统内存使用量
	afx_msg void OnTimer(UINT nIDEvent);
	//单击进程列表栏某一项消息处理(LVN_COLUMNCLICK)
	afx_msg void OnColumnclickListProcess(NMHDR* pNMHDR, LRESULT* pResult);
	//单击(显示进程模块信息)按钮消息处理
	afx_msg void OnButtonShow();
	//单击模块列表栏某一项消息处理
	afx_msg void OnColumnclickListDll(NMHDR* pNMHDR, LRESULT* pResult);
	//单击进程列表某一行消息处理
	afx_msg void OnClickListProcess(NMHDR* pNMHDR, LRESULT* pResult);
	//右键弹出菜单
	afx_msg void OnRclickListProcess(NMHDR* pNMHDR, LRESULT* pResult);
	//处理 刷新菜单项消息
	afx_msg void OnRefresh();
	//处理 终止进程菜单项消息
	afx_msg void OnTerminate();
	//处理 暂停进程菜单项消息
	afx_msg void OnSuspend();
	//处理 恢复进程菜单项消息
	afx_msg void OnResume();
	//处理 文件所在 菜单项消息
	afx_msg void OnFilePath();
	//处理 百度一下 菜单项消息
	afx_msg void OnBaiduCheck();
	//处理 复制进程路径菜单项消息
	afx_msg void OnCopy();
	//处理 调用注册表监视菜单项消息
	afx_msg void OnRegMonitor();
	//处理 模块列表右键消息
	afx_msg void OnRclickListDll(NMHDR* pNMHDR, LRESULT* pResult);
	//处理 打开到相应模块位置菜单消息
	afx_msg void OnOpenModule();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	HMODULE m_Hmodule;//皮肤模块句柄
	CString m_numOfDll;//所含模块数
	static bool m_bAccend;   //判断这次是怎样的排序方式
    static int m_nSortCol;  //指定的排序的关键列
	CImageList m_ImageList; //进程图标列表
	DWORD m_vProcessId[100];//存放进程ID数组
	bool EnablePrivilege();// 提升本进程权限
	int EnumProcess(void); //枚举进程
	CString GetProcessPath(DWORD idProcess);//获得进程路径
	BOOL GetDllInfo(DWORD dwPID);//获得模块信息
	//排序函数(当用户单击列表顶端某一项时需要此排顺序函数,需要列表先做个索引)
	static int CALLBACK MyCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);//排序回调函数
};

//{{AFX_INSERT_LOCATION}}()
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMANAGEDLG_H__A3F29421_DE79_4D52_816F_5D080647CB10__INCLUDED_)
