// ProcessManageDlg.cpp : implementation file

#include <Ctime>
#include <cstdlib>
#include "stdafx.h"
#include "ProcessManage.h"
#include "ProcessManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma comment(lib,"version.lib")//版本信息

#define TYPE_PROCESSMANAGE_MSG WM_USER+111 //发送给注册表监视的消息

typedef struct PROCESS_MANAGE_MSG
{
	DWORD	PID;//进程ID号
	char	ProcessName[MAX_PATH];//进程名用CString易出错
}ProcessMsg;

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
__cdecl:   多用于   C和   C++语言编写的例程，也用于需要由调用者清除参数的例程；   
__stdcall:   和safecall主要用于调用Windows   API   函数；其中safecall还用于双重接口

因为vc6.0 为低版本的其中没有封装OpenThread 函数所以要采用动态加载dll 的方法  
（注意 一定要指明为stdcall方式）(此外也可以安装新版本的Microsoft Platform SDK for Windows **)
*/

typedef  HANDLE  ( __stdcall *OPENTHREAD) (DWORD dwFlag, BOOL bUnknow, DWORD dwThreadId);
OPENTHREAD lpfnOpenThread = (OPENTHREAD) ::GetProcAddress(::LoadLibrary("kernel32.dll"),"OpenThread");
///////////////////////////////////////////////////////////////////////////////////////////////////////

// CProcessManageDlg 
//静态成员初始化
 bool CProcessManageDlg::m_bAccend=false;
 int CProcessManageDlg::m_nSortCol=0;


CProcessManageDlg::CProcessManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessManageDlg)
	m_nDll = _T("");
	m_numOfDll=_T("");
	m_Hmodule=NULL;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CProcessManageDlg::~CProcessManageDlg()
{
	
	//释放资源
	if(m_Hmodule)
	{
		::FreeLibrary(m_Hmodule);
	}
}

void CProcessManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessManageDlg)
	DDX_Control(pDX, IDC_LIST_DLL, m_ListDll);
	DDX_Text(pDX, IDC_STATIC_DLL, m_nDll);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ListProcess);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcessManageDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessManageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PROCESS, OnColumnclickListProcess)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, OnButtonShow)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DLL, OnColumnclickListDll)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROCESS, OnClickListProcess)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, OnRclickListProcess)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_TERMINATE, OnTerminate)
	ON_COMMAND(ID_SUSPEND, OnSuspend)
	ON_COMMAND(ID_RESUME, OnResume)
	ON_COMMAND(ID_FILE_PATH, OnFilePath)
	ON_COMMAND(ID_BAIDU_CHECK, OnBaiduCheck)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_REG_MONITOR, OnRegMonitor)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DLL, OnRclickListDll)
	ON_COMMAND(ID_OPEN_MODULE, OnOpenModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessManageDlg message handlers

BOOL CProcessManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();



	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ListProcess.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);//更改列表样式
	//列表项字符串数组
	CString strHeader1[5]={_T("进程名"),_T("进程ID"),_T("已运行时间"),_T("当前占用内存"),_T("文件路径")};
	int nWidth1[5]={120,55,90,92,300};
	int i=0;
	//插入列表项
	for (i=0;i<5;i++)
	m_ListProcess.InsertColumn( i,strHeader1[i], LVCFMT_LEFT, nWidth1[i]);
	m_ListProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//设置网格

	m_ImageList.Create(16,16,true|ILC_COLOR32,12,30);//创建图像列表
	m_ImageList.SetBkColor(RGB(255, 255, 255));//将图像背景色设置为白色
	EnablePrivilege();//提升权限
	EnumProcess();//列举进程
	m_ListProcess.SetImageList(&m_ImageList, LVSIL_SMALL);//设定列表视图控件的图象列表

	GetDlgItem(IDC_LIST_DLL)->ShowWindow(SW_HIDE); //隐藏显示DLL的列表
	m_ListDll.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_ListDll.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP); //网格
	CString strHeader2[7]={"模块路径","描述","公司","版本","原始文件名","合法版权","修改时间"};
	int nWidth2[7]={180,150,110,80,60,80,66};
	//插入列表项
	for (int nCol=0;nCol<7;nCol++)
		m_ListDll.InsertColumn(nCol,strHeader2[nCol],LVCFMT_LEFT,nWidth2[nCol]);
    
	/*******************************///加载皮肤/*****************************************/
	//产生随机数 用于动态产生一个皮肤
	srand((unsigned)time(0));
	i=rand()%35 +1;
	CString s=_T("");
	s.Format("skin//%d.she",i);
	//采用运行期间动态链接方式
	//加载目标DLL
	m_Hmodule=::LoadLibrary("SkinH.dll");
	//宏定义函数指针类型 
	typedef int ( WINAPI *SKINH_ATTACHEX)(LPCTSTR strSkinFile,LPCTSTR strPassword);
	
	if(NULL !=m_Hmodule)
	{	
		// 取得SKINH_ATTACHEX函数的地址
		SKINH_ATTACHEX pFunc = (SKINH_ATTACHEX)::GetProcAddress(m_Hmodule, "SkinH_AttachEx");
		if(pFunc)
		{
			pFunc(s, NULL);
		}
		else
		{
			MessageBox(_T("皮肤库加载失败！"),_T("缺少SkinH.dll"));
		}
	}

	/********************************************************************************************/
	SetTimer(0,300,NULL);//设置定时器 300毫秒
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	CDialog::OnSysCommand(nID, lParam);

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProcessManageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProcessManageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//提升权限
bool CProcessManageDlg::EnablePrivilege()
{
	// 提升进程特权,以便访问系统进程
	BOOL status = false;
	HANDLE hToken;
	// 打开一个进程的访问令牌
	if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	{
		// 取得特权名称为"SetPrivilege"的LUID
		LUID uID;
		::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID);
		
		// 调整特权级别
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = uID;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		if (ERROR_SUCCESS == GetLastError())
			status =true;
		else
			status =false;
		// 关闭访问令牌句柄
		::CloseHandle(hToken);
	}
	return status;
}



//列举进程
int CProcessManageDlg::EnumProcess(void)
{
	int i=0;
	int j=0;
	HANDLE hProcessSnap;//快照句柄	
	CString tem,title;
    //The SHGetFileInfo API provides an easy way to get attributes for a file given a pathname
	SHFILEINFO info;
	//定义文件系统时间(创建,退出,内核,用户)
	FILETIME      ftCreation,
                  ftExit,
                  ftKernel,
                  ftUser;
	//定义时间结构 COleDateTime使用的位数是双浮点的两倍(更精确)
    COleDateTime  timeCreation,
                  timeCurrent;
    COleDateTimeSpan   timeDiff;

	m_ListProcess.DeleteAllItems();// 清除列表中的所有项
	PROCESSENTRY32 pe32={0};// 定义PROCESSENTRY32 结构
	pe32.dwSize = sizeof( PROCESSENTRY32 );	// 设置PROCESSENTRY32结构的大小
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );// 创建进程快照

	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	// 首次调用
	if(!Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );
		return 0;
	}
   // 遍历进程快照，轮流显示每个进程的信息
	while( Process32Next( hProcessSnap, &pe32 ))
	{	
		// 获取文件图标
		SHGetFileInfo(GetProcessPath(pe32.th32ProcessID),0,&info,sizeof(&info),
			SHGFI_ICON | SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
		j=m_ImageList.Add(info.hIcon);// 添加程序图标
		m_ListProcess.InsertItem(i,pe32.szExeFile,j);  // 进程名  

		tem.Format("%d",pe32.th32ProcessID);
		m_ListProcess.SetItemText(i,1,tem);	// PID

		title=pe32.szExeFile;
		// 判断是否为 smss.exe进程
		//smss.exe(Session Manager Subsystem)，该进程为会话管理子系统用以初始化系统变量
		if(!strcmp(title,"smss.exe"))
		{
			m_ListProcess.SetItemText(i,4,"C:\\WINDOWS\\system32\\smss.exe");
		}
		//是否为系统进程
		else if(!strcmp(pe32.szExeFile,"system")||!strcmp(pe32.szExeFile,"[System Process]"))
		{
			m_ListProcess.SetItemText(i,0,"Idle");
			m_ListProcess.SetItemText(i,4,"NT OS Kernel");// NT 核心进程
		}
		else if(!strcmp(pe32.szExeFile,"system")||!strcmp(pe32.szExeFile,"System"))
		{
			m_ListProcess.SetItemText(i,4,"NT OS Kernel");
		}
		else
		{
			m_ListProcess.SetItemText(i,4,GetProcessPath(pe32.th32ProcessID));//进程路径
		}	
		HANDLE     hProcess;// 进程句柄
		timeCurrent = COleDateTime::GetCurrentTime();// 获得当前时间
		hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, 
			FALSE,pe32.th32ProcessID);// 打开此进程
		if(NULL != hProcess)
		{
			GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser);
			timeCreation = ftCreation;
			timeDiff = timeCurrent - timeCreation;
			tem.Format("%u小时:%u分:%u秒",timeDiff.GetHours(), timeDiff.GetMinutes(), 
				timeDiff.GetSeconds());// 获取进程运行时间
			m_ListProcess.SetItemText(i,2,tem);// 设置进程运行时间
			//使用API函数GetProcessMemoryInfo来获取内存的使用情况
			//ppsmemCounters是返回内存使用情况的结构
			PROCESS_MEMORY_COUNTERS pmc;
			pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);
			if(::GetProcessMemoryInfo(hProcess, &pmc,sizeof(pmc)))
			{
				tem.Format(_T("%.2f M"),(float)(pmc.WorkingSetSize/1024)/1024);
				m_ListProcess.SetItemText(i,3,tem);
			}
			CloseHandle(hProcess);// 关闭此进程句柄
		}
		m_vProcessId[i]=pe32.th32ProcessID;  //存放ID信息
		m_ListProcess.SetItemData(i,i);//索引,为排序做准备
		i++;
	} 
	CString str1;
	str1.Format("%d",i);// 进程数量
	SetDlgItemText(IDC_STATIC_PRO_NUM,str1);
	CloseHandle( hProcessSnap );//关闭句柄
	UpdateData(FALSE);
	return 0;
}


//列举进程模块信息
BOOL CProcessManageDlg::GetDllInfo(DWORD dwPID)
{
	// 防止闪屏
	m_ListDll.SetRedraw(FALSE);
	// 模块句柄数组
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbHmodule;// 所含模块数
	unsigned int i;
	m_ListDll.DeleteAllItems();
	char szModName[MAX_PATH];
	// 打开目标进程，取得进程句柄
	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE, dwPID);
	// 枚举 进程模块
	if( hProcess &&  EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbHmodule))
	{
		for ( i = 0; i <=(cbHmodule / sizeof(HMODULE)); i++ )
		{        	
			// 得到模块的完整路径名
			if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
				sizeof(szModName)))
			{
				//模块名，修改时间
				CString ModName, FTime;
				// 文件的属性
				CFileStatus status;
				ModName = szModName;
				ModName.Replace("\\??\\","");
				ModName.Replace("\\SystemRoot","C:\\WINDOWS");
				m_ListDll.InsertItem(0xffff,ModName);
				//CString 转 char*
				char* szFileName=(LPTSTR)(LPCTSTR)ModName;
				// 获得文件版本信息 Returns size of version info in bytes 
				DWORD dwSize =::GetFileVersionInfoSize(szFileName,NULL); 
				LPVOID pBlock = malloc(dwSize); 
				// 把版本信息 写到缓冲区 Read version info into buffer 
				if(::GetFileVersionInfo(szFileName,0,dwSize,pBlock))
				{
					char* pVerValue = NULL; 
					UINT nSize = 0; 
					//这是得到语言
					VerQueryValue(pBlock,_T("\\VarFileInfo\\Translation"), 
						(LPVOID*)&pVerValue,&nSize); 
					
					CString strSubBlock,strTranslation,strTemp;
					//语言转化  080404b0为中文，040904E4为英文
					strTemp.Format("000%x",*((unsigned short int *)pVerValue)); 
					strTranslation = strTemp.Right(4); 
					strTemp.Format("000%x",*((unsigned short int *)&pVerValue[2])); 
					strTranslation += strTemp.Right(4); 
					//文件描述  FileDescription
					strSubBlock.Format("\\StringFileInfo\\%s\\FileDescription",strTranslation); 
					VerQueryValue(pBlock,(LPTSTR)(LPCTSTR)strSubBlock,(LPVOID*)&pVerValue,&nSize);
					if((NULL!=pVerValue)&&(0!=nSize))// 判断是否得到查询结果
					{
						strTemp.Format("%s",pVerValue); 
						m_ListDll.SetItemText(i, 1, strTemp);
					}
					else
					{
						m_ListDll.SetItemText(i, 1, "");
					}
					//公司名称 CompanyName
					strSubBlock.Format("\\StringFileInfo\\%s\\CompanyName",strTranslation); 
					VerQueryValue(pBlock,(LPTSTR)(LPCTSTR)strSubBlock,(LPVOID*)&pVerValue,&nSize); 
					if((NULL!=pVerValue)&&(0!=nSize))
					{
						strTemp.Format("%s",pVerValue); 
						m_ListDll.SetItemText(i, 2, strTemp);
					}
					else
					{
						m_ListDll.SetItemText(i, 2, "");
					}
					
					//产品版本 ProductVersion
					strSubBlock.Format("\\StringFileInfo\\%s\\ProductVersion",strTranslation); 
					VerQueryValue(pBlock,(LPTSTR)(LPCTSTR)strSubBlock,(LPVOID*)&pVerValue,&nSize); 
					if((NULL!=pVerValue)&&(0!=nSize))
					{
						strTemp.Format("%s",pVerValue); 
						m_ListDll.SetItemText(i, 3, strTemp);
					}
					else
					{
						m_ListDll.SetItemText(i, 3, "");
					}
					//原始文件名 OriginalFileName
					strSubBlock.Format("\\StringFileInfo\\%s\\OriginalFileName",strTranslation);
					VerQueryValue(pBlock,(LPTSTR)(LPCTSTR)strSubBlock,(LPVOID*)&pVerValue,&nSize);
					if((NULL!=pVerValue)&&(0!=nSize))
					{
						strTemp.Format("%s",pVerValue);
						m_ListDll.SetItemText(i, 4, strTemp);
					}
					else
					{
						m_ListDll.SetItemText(i, 4, "");
					}
					//合法版权 LegalTradeMarks
					strSubBlock.Format("\\StringFileInfo\\%s\\LegalTradeMarks",strTranslation);
					VerQueryValue(pBlock,(LPTSTR)(LPCTSTR)strSubBlock,(LPVOID*)&pVerValue,&nSize);
					if((NULL!=pVerValue)&&(0!=nSize))
					{
						strTemp.Format("%s",pVerValue);
						m_ListDll.SetItemText(i, 5, strTemp);
					}
					else
					{
						m_ListDll.SetItemText(i, 5, "");
					}
				}
				else
				{
					m_ListDll.SetItemText(i, 1, "");
					m_ListDll.SetItemText(i, 2, "");
					m_ListDll.SetItemText(i, 3, "");
					m_ListDll.SetItemText(i, 4, "");
					m_ListDll.SetItemText(i, 5, "");
				}
				delete pBlock; // 释放申请的内存
				// 用于查看文件信息的函数
				CFile::GetStatus(szFileName,status);
				FTime = status.m_mtime.Format("%m/%d/%y");
				m_ListDll.SetItemText(i, 6, FTime); 
				
				
				m_ListDll.SetItemData(i,i);
			}
		}
		int j = 0;
		j= m_ListDll.GetItemCount();	
		m_numOfDll.Format("%d",j-1);
		CloseHandle(hProcess);
		//更新内容
		m_ListDll.SetRedraw(TRUE);
		m_ListDll.Invalidate();
		m_ListDll.UpdateWindow();
		
		return TRUE;
	}
	else  
	{
		CloseHandle( hProcess );
		return FALSE;
	}
}


//获取进程路径
CString CProcessManageDlg::GetProcessPath(DWORD idProcess)
{
	// 获取进程路径
	CString sPath;
	char lPath[MAX_PATH] = "";

	// 打开目标进程(通过 PID)
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, idProcess );

	if( NULL != hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;
		// 枚举该进程的模块
		if(::EnumProcessModules(hProcess, &hMod, sizeof( hMod ), &cbNeeded ) )
		{
			// 获得程序当前的目录
			DWORD dw =::GetModuleFileNameEx( hProcess, hMod, (LPSTR)(LPCTSTR)sPath, MAX_PATH );
			// 找到第一次出现:\\字符串的位置
			int n=sPath.Find(":\\");
			// 从磁盘符开始的绝对路径 比如(c:\\...)
			sPath=sPath.Mid(n-1);
			// 将文件名转化成长文件名
			::GetLongPathName(sPath,lPath,255);
		}
		CloseHandle( hProcess );
	}
	return(lPath);
}
//定时器用于定时更新系统内存使用量
void CProcessManageDlg::OnTimer(UINT nIDEvent) 
{
	CString str;
	MEMORYSTATUS ms;// 定义MEMORYSTATUS结构
	GlobalMemoryStatus(&ms);//获得内存使用状态
	str.Format("%d", ms.dwMemoryLoad);
	str = str +"%";
	SetDlgItemText(IDC_STATIC_MEMORY,str);//在静态文本框上显示
	CDialog::OnTimer(nIDEvent);
}




 //排序函数  排序之前需要做好索引 --->  m_list.SetItemData(i,i);
 int CALLBACK CProcessManageDlg::MyCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{	
	CListCtrl * pListCtrl = (CListCtrl *)lParamSort;
	CString text1; 
	CString text2;

	int index;
	LVFINDINFO info;
	info.flags = LVFI_PARAM;

	info.lParam = lParam1;
	index = pListCtrl->FindItem(&info);
	text1 = pListCtrl->GetItemText(index, m_nSortCol); //获取前一个文本信息

	info.lParam = lParam2;
	index = pListCtrl->FindItem(&info);
	text2 = pListCtrl->GetItemText(index, m_nSortCol);  //获取后一个文本信息 

	//字符串型
	if(m_bAccend)
		return text1.CompareNoCase(text2);
	else
		return text2.CompareNoCase(text1);

}
 //单击列表头的时候会产生LVN_COLUMNCLICK通知消息  处理此消息函数
void CProcessManageDlg::OnColumnclickListProcess(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//
	m_nSortCol = pNMListView->iSubItem; //所指定的排序关键列 
	//调用自定义的排序函数进行排序
	m_ListProcess.SortItems(MyCompare, (DWORD)&m_ListProcess);
	m_bAccend = !m_bAccend;
	*pResult = 0;
}

void CProcessManageDlg::OnButtonShow() 
{
	// TODO: Add your control notification handler code here
	CButton* pBut;	
	pBut=(CButton*)GetDlgItem(IDC_BUTTON_SHOW);
	CWnd *pWnd, *pWnd2;
	CRect   rcWnd, rcWnd2;//定义显示区域
	CString str;
	pBut->GetWindowText(str);
	if(_T("显示进程包含的模块")==str)//显示进程模块
	{
		GetDlgItem(IDC_LIST_DLL)->ShowWindow(SW_SHOW);//显示隐藏的模块列表
		pBut->SetWindowText(_T("不显示进程包含的模块"));
		pWnd = GetDlgItem( IDC_LIST_PROCESS );
		pWnd->GetWindowRect(&rcWnd); 
		pWnd->SetWindowPos( NULL,0,0,rcWnd.Width(),rcWnd.Height()-140,SWP_NOZORDER | SWP_NOMOVE );
		pWnd2 = GetDlgItem( IDC_LIST_DLL);
		pWnd2->GetWindowRect(&rcWnd2);
		pWnd2->SetWindowPos( NULL,0,0,rcWnd.Width(),rcWnd2.Height(),SWP_NOZORDER | SWP_NOMOVE);
	}
	else //不显示模块信息
	{
		GetDlgItem(IDC_LIST_DLL)->ShowWindow(SW_HIDE);// 隐藏模块信息列表
		pBut->SetWindowText(_T("显示进程包含的模块"));
		pWnd = GetDlgItem( IDC_LIST_PROCESS );
		pWnd->GetWindowRect(&rcWnd); 
		pWnd->SetWindowPos( NULL,0,0,rcWnd.Width(),rcWnd.Height()+140,SWP_NOZORDER | SWP_NOMOVE );
	}
	
}

void CProcessManageDlg::OnColumnclickListDll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nSortCol = pNMListView->iSubItem; //所指定的排序关键列 
	m_ListDll.SortItems(MyCompare, (DWORD)&m_ListProcess);
	m_bAccend = !m_bAccend;
	*pResult = 0;
}

void CProcessManageDlg::OnClickListProcess(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString szCID,exp;
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (nItem != -1)
	{
		szCID= m_ListProcess.GetItemText(nItem,1);
		DWORD dwPID = atol(szCID);	//从列表中取出PID
		GetDllInfo(dwPID);// 通过 PID获得此进程的模块信息
	}
   //在静态文本中显示DLL个数
	m_nDll = m_ListProcess.GetItemText(nItem,0)+"包含 ";
	m_nDll += m_numOfDll +" 个模块";
	UpdateData(FALSE);	
	*pResult = 0;
}

//处理列表右键消息 
void CProcessManageDlg::OnRclickListProcess(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem != -1)
	{
	  DWORD dwPos = GetMessagePos();
      CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
      CMenu menu;
      VERIFY( menu.LoadMenu(IDR_MENU1));
      CMenu* popup = menu.GetSubMenu(0);
      ASSERT( popup != NULL );
      popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
   }
	*pResult = 0;
}

//处理菜单 刷新消息
void CProcessManageDlg::OnRefresh() 
{
	// TODO: Add your command handler code here
	m_ListProcess.SetRedraw(FALSE);  //避免闪屏
	EnumProcess();        //重新列举进程
	m_ListProcess.SetRedraw(TRUE);
	m_ListProcess.Invalidate();
	m_ListProcess.UpdateWindow();
}

void CProcessManageDlg::OnTerminate() 
{
	// TODO: Add your command handler code here
	int i=m_ListProcess.GetSelectionMark(); //从一个列表视图控件检索选择标记(用于删除行)
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	//先获得该进程句柄，通过进程标识
	HANDLE ProcessHandle;

	//利用数组ID指定的获得存在的一个进程的句柄
	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,m_vProcessId[nItem]);

	if(ProcessHandle)
	{
		BOOL bRet=TerminateProcess(ProcessHandle,0);
		if (!bRet)
		{
			AfxMessageBox("结束进程失败!");
		}
		else
		{
			m_ListProcess.DeleteItem(i);     //从控件中删除本项,本方法不会导致整个列表跳动
		}
	}
	else
	{
		AfxMessageBox("打开进程句柄失败!");
	}
}

// 暂停进程
void CProcessManageDlg::OnSuspend() 
{
	// TODO: Add your command handler code here
	CString str;
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	str = m_ListProcess.GetItemText(nItem, 1);

	unsigned int Pid;
	THREADENTRY32 th32;
	th32.dwSize=sizeof(th32);
	Pid = (DWORD)atoi(str); // CString 转化为DWORD
	HANDLE hThreadSnap=::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0); //创建进程快照
	if(hThreadSnap==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateToolhelp32Snapshot调用失败");
	}

	BOOL bRet = ::Thread32First(hThreadSnap,&th32);
	while(bRet)
	{
		if(th32.th32OwnerProcessID == Pid)
		{	//HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, __dwMainThreadId);
			// 由于vc6.0（低版本中的Winbase.h 没有此函数，因此
			HANDLE handle=lpfnOpenThread(THREAD_ALL_ACCESS,FALSE,th32.th32ThreadID); // 打开线程内核对象
			if(!(::SuspendThread(handle))) //暂停线程
			{
			}
			CloseHandle(handle);// 关闭内核对象
		}
		bRet=::Thread32Next(hThreadSnap,&th32);
	}
	::CloseHandle(hThreadSnap);// 关闭内核对象
}

// 恢复进程运行
void CProcessManageDlg::OnResume() 
{
	// TODO: Add your command handler code here
	CString str;
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	str = m_ListProcess.GetItemText(nItem, 1);

	THREADENTRY32 th32;
	th32.dwSize=sizeof(th32);
	unsigned int Pid;
	Pid = (DWORD)atoi(str);
	HANDLE hThreadSnap=::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(hThreadSnap==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateToolhelp32Snapshot调用失败！");
	}

	BOOL bRet = ::Thread32First(hThreadSnap,&th32);
	while(bRet)
	{
		if(th32.th32OwnerProcessID==Pid)
		{	
			HANDLE handle=lpfnOpenThread(THREAD_ALL_ACCESS,FALSE,th32.th32ThreadID);// 打开线程内核对象
			if(::ResumeThread(handle)) //恢复进程运行
			{
			}
			CloseHandle(handle);
		}
		bRet = ::Thread32Next(hThreadSnap,&th32);
	}
	::CloseHandle(hThreadSnap);
}

// 打开到文件所在
void CProcessManageDlg::OnFilePath() 
{
	CString tem = "";
	char szt[255];
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_ListProcess.GetItemText(nItem, 0, szt, 512);
	if(!strcmp(szt,"system")||!strcmp(szt,"[System Process]"))
	{
	}
	else
	{
		m_ListProcess.GetItemText(nItem, 4, szt, 512);
		tem.Format("Explorer /select, %s",szt);
		WinExec(tem,SW_SHOWNORMAL);
	}
}
// 百度一下该进程
void CProcessManageDlg::OnBaiduCheck() 
{
	CString strParams,  strDir,strURL;
	SHELLEXECUTEINFO ShellInfo;
    memset(&ShellInfo, 0, sizeof(ShellInfo)); 
    
	char ProcessName[100]=" ";	
	int nItem=m_ListProcess.GetSelectionMark();
	m_ListProcess.GetItemText(nItem,0,ProcessName,100);
    strURL=_T("www.baidu.com/s?wd=");
	strURL+= ProcessName;
    ShellInfo.cbSize = sizeof(ShellInfo);
    ShellInfo.hwnd = NULL; 
    ShellInfo.lpVerb = _T("open"); 
    ShellInfo.lpFile = strURL;
    ShellInfo.nShow = SW_SHOWNORMAL; 
    ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
    BOOL bResult = ShellExecuteEx(&ShellInfo);

}

// 复制进程路径
void CProcessManageDlg::OnCopy() 
{
	//获得当前选中项
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED); 
	if(OpenClipboard())//打开剪贴板
	{
		CString str;
		HANDLE hClip;
		char *pBuf;
		EmptyClipboard();//清空剪贴板
		str=m_ListProcess.GetItemText(nItem,4);//获得文本
		hClip=::GlobalAlloc(GMEM_MOVEABLE,str.GetLength()+1);//从堆中分配内存
		pBuf=(char*)GlobalLock(hClip);//对内存加锁
		strcpy(pBuf,str);//拷贝到分配的内存中
		GlobalUnlock(hClip);//解锁
		SetClipboardData(CF_TEXT,hClip);
		CloseClipboard();//关闭剪贴板
	}
}




void CProcessManageDlg::OnRegMonitor() 
{
	/*
	用WM_COPYDATA的前提：
	1，知道接收消息进程的句柄。
	2，接收消息进程重载了WM_COPYDATA消息映射，能对其做出反应（
	*/
	//获得当前选中项
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED); 
	CString strPid=m_ListProcess.GetItemText(nItem,1);//获得PID号
	CString StrProName=m_ListProcess.GetItemText(nItem,0);//获得进程名
	HWND hWnd =::FindWindow(NULL,_T("RegMonitor")); // 查找RegMonitor
	if(NULL==hWnd)
	{ 
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };   
		// Ask for privileges elevation.   
		sei.lpVerb = TEXT("runas");      
		sei.lpFile = _T("RegMonitor.exe");   
		sei.nShow = SW_SHOWNORMAL;   
		ShellExecuteEx(&sei);
		//CreateProcessAs("RegMonitor.exe");
		Sleep(300);
		WaitForSingleObject(sei.hProcess, INFINITE);
		OnRegMonitor();
    }
	else
	{
		
		
		// 允许Vista/Win7下,不同权限进程间(高向低不需要，低向高需要)发送消息：WM_COPYDATA 
		/*typedef BOOL (WINAPI FAR *ChangeWindowMessageFilter_PROC)(UINT,DWORD); 
		ChangeWindowMessageFilter_PROC m_pfnChangeWindowMessageFilter; 
		m_pfnChangeWindowMessageFilter = 
			(ChangeWindowMessageFilter_PROC)::GetProcAddress (::GetModuleHandle(_T("USER32")),"ChangeWindowMessageFilter"); 
		if (m_pfnChangeWindowMessageFilter) 
		{ 
			//m_pfnChangeWindowMessageFilter(WM_COPYDATA, 1); 
		}*/
		
		COPYDATASTRUCT data;//定义数据结构
		ProcessMsg ProMsg ;//此处不能利用指针(切忌)即 不能动态分配内存存储数据
		ProMsg.PID=(DWORD)atoi(strPid);
		char * pStr = (LPTSTR)(LPCTSTR)StrProName;//转化成char*
		strncpy(ProMsg.ProcessName, pStr, sizeof(ProMsg.ProcessName));//拷贝到目标
		data.cbData = sizeof(ProcessMsg);
		data.lpData = (void*)&ProMsg;
		data.dwData = TYPE_PROCESSMANAGE_MSG;
		::SendMessage(hWnd,WM_COPYDATA,(WPARAM)hWnd,(LPARAM)&data);//发送！
	}


}

void CProcessManageDlg::OnRclickListDll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem != -1)
	{
	  DWORD dwPos = GetMessagePos();
      CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
  
      CMenu menu;
      VERIFY( menu.LoadMenu(IDR_MENU2));
      CMenu* popup = menu.GetSubMenu(0);
      ASSERT( popup != NULL );
      popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
   }
	*pResult = 0;
}
//处理 打开到相应模块位置菜单消息
void CProcessManageDlg::OnOpenModule() 
{
	CString tem = _T("");
	char sztemp[255];
	// 获得选中的项
	int nItem = m_ListProcess.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	// 获得 模块路径
	m_ListDll.GetItemText(nItem, 0, sztemp, 512);
	tem.Format("Explorer /select, %s",sztemp);
	// 打开到此模块
	WinExec(tem,SW_SHOWNORMAL);
}
