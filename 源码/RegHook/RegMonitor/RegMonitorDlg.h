// RegMonitorDlg.h : header file
//

#if !defined(AFX_REGMONITORDLG_H__ED21C84E_B1E6_410E_8442_B6A65F39F475__INCLUDED_)
#define AFX_REGMONITORDLG_H__ED21C84E_B1E6_410E_8442_B6A65F39F475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\\RegHook.h"
#include <ctime> //产生随机函数需要
#include <cstdlib>
#include <Tlhelp32.h>//快照所需头文件
#pragma comment(lib,"RegHook.lib")

//定义进程管理工具发过来的消息结构
typedef struct PROCESS_MANAGE_MSG
{
	DWORD	PID;//进程PID
	char	ProcessName[MAX_PATH];//进程名用CString易出错 
}ProcessMsg;
/////////////////////////////////////////////////////////////////////////////
// CRegMonitorDlg dialog

class CRegMonitorDlg : public CDialog
{
// Construction
public:
	// 通过进程名得到进程路径
	void ProcessPidToPath(DWORD ProcessId, PCHAR ProcessName, PCHAR ProcessPath);
	CFont		m_hFont;
	// 初始化对话框元素
	void InitDlgItem();
	// 通过进程名的到进程ID 
	DWORD GetProcessID(PCHAR ProcessName);
	// 更新钩子监视内容
	void UpdateHookFunction();
	CRegMonitorDlg(CWnd* pParent = NULL);	// standard constructor
	~CRegMonitorDlg();
// Dialog Data
	//{{AFX_DATA(CRegMonitorDlg)
	enum { IDD = IDD_REGMONITOR_DIALOG };
	CListCtrl	m_listCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    
	// Generated message map functions
	//{{AFX_MSG(CRegMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnCancelMode();
	afx_msg void OnButtonStop();
	afx_msg void OnMenuCreateKey();
	afx_msg void OnMenuDelKey();
	afx_msg void OnMenuSetValue();
	afx_msg void OnMenuDelValue();
	afx_msg void OnMenuQueryValue();
	afx_msg void OnMenuOpenRegedit();
	afx_msg void OnButtonSave();
	afx_msg void OnMenuOpenProcessManage();
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CImageList m_imglst; 
	bool m_bIgnoreRegedit; 
	HHOOK m_hHook;
	ZwQueryKey m_pZwQueryKey; // 定义查询信息函数的指针
	//  根据一下决定是否监视相应的项
	BOOL	m_bRegCreateKeyEx;
	BOOL	m_bRegSetValueEx;
	BOOL	m_bRegDeleteKey;
	BOOL	m_bRegDeleteValue;
	BOOL	m_bRegQueryValueEx;
	BOOL	m_bIgnoreExplorer;
	
public:

	HMODULE m_HmoduleSkin;//皮肤模块句柄
	HMODULE m_HmoduleUser32;//User32模块句柄
	CMenu* m_Psubmenu;
	int WideToMultiByte(const WCHAR* wChar, char* mChar);
	void HKeyToKeyName(DWORD PID, HKEY hKey, char* pszKeyName);

	// 处理相应的监视消息 PARAMS自定义结构  index列表索引
	void  CreateKeyExA_Msg(PARAMS* params,int index);
	void  CreateKeyExW_Msg(PARAMS* params,int index); 
	void  SetValueExA_Msg(PARAMS* params,int index); 
	void  SetValueExW_Msg(PARAMS* params,int index);
	void  DeleteKeyA_Msg(PARAMS* params,int index);
	void  DeleteKeyW_Msg(PARAMS* params,int index);
	void  DeleteValueA_Msg(PARAMS* params,int index);
	void  DeleteValueW_Msg(PARAMS* params,int index);
	void  QueryValueExA_Msg(PARAMS* params,int index);
	void  QueryValueExW_Msg(PARAMS* params,int index);
};          

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMONITORDLG_H__ED21C84E_B1E6_410E_8442_B6A65F39F475__INCLUDED_)
