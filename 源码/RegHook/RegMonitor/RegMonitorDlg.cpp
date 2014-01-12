// RegMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegMonitor.h"
#include "RegMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TYPE_PROCESSMANAGE_MSG WM_USER+111 //进程管理器 发送的消息
/////////////////////////////////////////////////////////////////////////////
// CRegMonitorDlg dialog
CRegMonitorDlg::CRegMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegMonitorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);// 加载图标
	m_hHook=NULL;
	m_Psubmenu=NULL;
	m_bRegCreateKeyEx = TRUE;
	m_bRegSetValueEx = TRUE;
	m_bRegDeleteKey = TRUE;
	m_bRegDeleteValue = TRUE;
	m_bRegQueryValueEx = TRUE;
	m_bIgnoreExplorer = TRUE;
	m_bIgnoreRegedit = TRUE;
}

CRegMonitorDlg::~CRegMonitorDlg()//析构函数
{
	//释放Skin.dll资源
	if(m_HmoduleSkin)
	{
		::FreeLibrary(m_HmoduleSkin);
	}
	//释放User32.dll资源
	if(m_HmoduleUser32)
	{
		::FreeLibrary(m_HmoduleSkin);
	}
}
void CRegMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegMonitorDlg)
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CRegMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_COPYDATA()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_COMMAND(IDR_MENU11, OnMenuCreateKey)
	ON_COMMAND(IDR_MENU12, OnMenuDelKey)
	ON_COMMAND(IDR_MENU13, OnMenuSetValue)
	ON_COMMAND(IDR_MENU14, OnMenuDelValue)
	ON_COMMAND(IDR_MENU15, OnMenuQueryValue)
	ON_COMMAND(IDR_MENU21, OnMenuOpenRegedit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_COMMAND(IDR_MENU22, OnMenuOpenProcessManage)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegMonitorDlg message handlers

BOOL CRegMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();



	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	/*******************************///加载皮肤/*****************************************/
	srand((unsigned)time(0));
	int i=rand()%35 +1;
	CString s=_T("");
	s.Format("skin//%d.she",i);
	//采用运行期间动态链接方式
	//加载目标DLL
	m_HmoduleSkin=::LoadLibrary("SkinH.dll");
	//宏定义函数指针类型 
	typedef int ( WINAPI *SKINH_ATTACHEX)(LPCTSTR strSkinFile,LPCTSTR strPassword);
	
	if(NULL !=m_HmoduleSkin)
	{						// 取得SKINH_ATTACHEX函数的地址
		SKINH_ATTACHEX pFunc = (SKINH_ATTACHEX)::GetProcAddress(m_HmoduleSkin, "SkinH_AttachEx");
		if(pFunc)
		{
			pFunc(s, NULL);
		}
		else
		{
			MessageBox(_T("皮肤库加载失败！"),_T("缺少SkinH.dll"));
		}
	}

	/***********************************************************************************************************/
	// 允许Vista/Win7下,不同权限进程间(高向低不需要，低向高需要)发送消息：WM_COPYDATA 
	// 这一段很重要不然的话 编译出的程序不能以管理员身份运行(应用程序在Windows 7上运行时，因为UIPI机制，
	// 这种消息传递被阻断了) ChangeWindowMessageFilter
	// 利用这个函数，我们可以添加或者删除能够通过特权等级隔离的Windows消息。这就像拥有较高特权等级的进程,
	// 设置了一个过滤器,允许通过的Windows消息都被添加到这个过滤器的白名单,只有在这个白名单上的消息才允许传
	// 递进来

	typedef BOOL (WINAPI FAR *ChangeWindowMessageFilter_PROC)(UINT,DWORD); 
	ChangeWindowMessageFilter_PROC pfnChangeWindowMessageFilter; 
	pfnChangeWindowMessageFilter = (ChangeWindowMessageFilter_PROC)::GetProcAddress\
		( (m_HmoduleUser32=::GetModuleHandle(_T("USER32.dll") ) ),"ChangeWindowMessageFilter"); 
	if (pfnChangeWindowMessageFilter) 
	{  
		// 设置了一个过滤器 WM_COPYDATA
		pfnChangeWindowMessageFilter(WM_COPYDATA, 1); 
	}
/*************************************************************************************************************/

	InitDlgItem();//初始化对话框
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRegMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	CDialog::OnSysCommand(nID, lParam);

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegMonitorDlg::OnPaint() 
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
HCURSOR CRegMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 开始
void CRegMonitorDlg::OnButtonStart() 
{
	if(InstallHook(m_hWnd))
	{
		DWORD pid = 0;
		if(m_bIgnoreExplorer)
			pid = GetProcessID("explorer.exe");
		SetExplorerPID(pid);
		UpdateHookFunction();// 更新监视
		CButton *pBt=(CButton *)GetDlgItem(IDC_BUTTON_START);
		pBt->SetCheck(BST_CHECKED);
		pBt->EnableWindow(false);
		pBt=(CButton *)GetDlgItem(IDC_BUTTON_STOP);
		pBt->SetCheck(BST_CHECKED);
		pBt->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(_T("正在监视..."));
		GetDlgItem(IDC_STATIC_TIP2)->SetWindowText(_T("监视所有进程"));
	}	
}

void CRegMonitorDlg::UpdateHookFunction()
{
	SetHookFunction(m_bRegCreateKeyEx, m_bRegSetValueEx, m_bRegDeleteKey, 
		m_bRegDeleteValue, m_bRegQueryValueEx);// 设置钩子监视的内容
}

// 通过ProcessName得到进程ID
DWORD CRegMonitorDlg::GetProcessID(PCHAR ProcessName)
{
	
	CString str1;
	CString str2;
	PROCESSENTRY32 info = {sizeof(PROCESSENTRY32)};
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);// 创建快照
    BOOL bReport = Process32First(hProcess,&info);
	
    while(bReport)
    {
		str1 = ProcessName;
		str2 = info.szExeFile;
		str1.MakeLower();//都改变为小写字母方便比较
		str2.MakeLower();
		if(str1 == str2) // 判断是否为此进程
			return info.th32ProcessID; // 返回进程 ID
        bReport = Process32Next(hProcess, &info);   
    }
	return 0;
}

void CRegMonitorDlg::InitDlgItem()
{
	SetWindowText(_T("RegMonitor"));
	GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(_T("注册表监视,请选择开始"));
	//列表初始化列表
	m_listCtrl.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);

	m_listCtrl.SetBkColor(RGB(247,247,255));
	m_listCtrl.SetTextColor(RGB(0,100,155));
	m_listCtrl.SetTextBkColor(RGB(247,247,255));

	m_listCtrl.InsertColumn(0,_T("进程名"),LVCFMT_LEFT,120);
	m_listCtrl.InsertColumn(1,_T("时间"),LVCFMT_LEFT,80); 
	m_listCtrl.InsertColumn(2,_T("动作"),LVCFMT_LEFT,83);
	m_listCtrl.InsertColumn(3,_T("键名"),LVCFMT_LEFT,266);
	m_listCtrl.InsertColumn(4,_T("名称"),LVCFMT_LEFT,80);
	m_listCtrl.InsertColumn(5,_T("数据"),LVCFMT_LEFT,80);
	m_listCtrl.InsertColumn(6,_T("类型"),LVCFMT_LEFT,80);
	
	// 设置列表字体
	LOGFONT lf;
	::ZeroMemory(&lf,sizeof(lf)); 
	lf.lfHeight = 90;         
	lf.lfWeight = FW_NORMAL;   
	lf.lfItalic = false;	
	::lstrcpy(lf.lfFaceName,_T("Verdana")); 
	m_hFont.CreatePointFontIndirect(&lf);
	HIMAGELIST m_hSmallImageList;
	// 定义函数指针 SHGetImageList
	typedef BOOL (WINAPI * SHGIL_PROC)	(HIMAGELIST *phLarge, HIMAGELIST *phSmall);
	// 加载 shell32.dll 模块
	HMODULE hShell32 = LoadLibrary("shell32.dll");
	//  得到函数SHGetImageList的入口地址 注意指明为 71 不然出错
	SHGIL_PROC Shell_GetImageLists = (SHGIL_PROC)GetProcAddress(hShell32, (LPCSTR)71);
	Shell_GetImageLists(NULL, &m_hSmallImageList);// 获得程序小图标
	
	if(m_hSmallImageList != NULL) 
		m_imglst.Attach(m_hSmallImageList);
	
	m_listCtrl.SetImageList(&m_imglst, LVSIL_SMALL);// 设置列表小图标

	//ZwQueryKey 枚举子项 
	m_pZwQueryKey = (ZwQueryKey)::GetProcAddress(LoadLibrary("ntdll.dll"), "ZwQueryKey");


	// Get the popup menu 
	CMenu* mmenu = GetMenu();
	m_Psubmenu = mmenu->GetSubMenu(0);// 获得子菜单
	// 默认设置sub(0) 全选中
	m_Psubmenu->CheckMenuItem(IDR_MENU11, MF_CHECKED | MF_BYCOMMAND);
	m_Psubmenu->CheckMenuItem(IDR_MENU12, MF_CHECKED | MF_BYCOMMAND);
	m_Psubmenu->CheckMenuItem(IDR_MENU13, MF_CHECKED | MF_BYCOMMAND);
	m_Psubmenu->CheckMenuItem(IDR_MENU14, MF_CHECKED | MF_BYCOMMAND);
	m_Psubmenu->CheckMenuItem(IDR_MENU15, MF_CHECKED | MF_BYCOMMAND);

}
//////////////////////////////////////////////////////////////////////////////////////

// 处理相应的监视消息
void   CRegMonitorDlg::CreateKeyExA_Msg(PARAMS* params, int index)
{
	char szTemp[1024]={0};
	CString str=_T("创建新键(A)");
	m_listCtrl.SetItemText(index, 2, str);
		
	switch (int(params->hKey)) // Reserved Key Handles.
	{
		// 分别对几大根键进行讨论
	case HKEY_CLASSES_ROOT:
		str.Format("HKEY_CLASSES_ROOT\\%s", params->buf1);
		break;
	case HKEY_CURRENT_USER:
		str.Format("HKEY_CURRENT_USER\\%s", params->buf1);
		break;
	case HKEY_LOCAL_MACHINE:
		str.Format("HKEY_LOCAL_MACHINE\\%s", params->buf1);
		break;
	case HKEY_USERS:
		str.Format("HKEY_USERS\\%s", params->buf1);
		break;
		// WINNT操作系统
	case HKEY_PERFORMANCE_DATA:
		str.Format("HKEY_PERFORMANCE_DATA\\%s", params->buf1);
		break;
	case HKEY_CURRENT_CONFIG:
		str.Format("HKEY_CURRENT_CONFIG\\%s", params->buf1);
		break;
		// WIN9X操作系统
	case HKEY_DYN_DATA:
		str.Format("HKEY_DYN_DATA\\%s", params->buf1);
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp);
		str.Format("%s\\%s", szTemp, params->buf1);
	}
	m_listCtrl.SetItemText(index, 3, str);
	m_listCtrl.SetItemText(index, 4, "N/A");
	m_listCtrl.SetItemText(index, 5, "N/A");
	m_listCtrl.SetItemText(index, 6, "N/A");
}

// 处理相应的监视消息

void  CRegMonitorDlg::CreateKeyExW_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	char szTemp2[1024]={0};
	CString str=_T("创建新键(W)");
	m_listCtrl.SetItemText(index, 2, str);
		
	WideToMultiByte((WCHAR*)params->buf1, szTemp);
	switch (int(params->hKey))// Reserved Key Handles.
	{
	case HKEY_CLASSES_ROOT:
		str.Format("HKEY_CLASSES_ROOT\\%s", szTemp);
		break;
	case HKEY_CURRENT_USER:
		str.Format("HKEY_CURRENT_USER\\%s", szTemp);
		break;
	case HKEY_LOCAL_MACHINE:
		str.Format("HKEY_LOCAL_MACHINE\\%s", szTemp);
		break;
	case HKEY_USERS:
		str.Format("HKEY_USERS\\%s", szTemp);
		break;
	case HKEY_PERFORMANCE_DATA:
		str.Format("HKEY_PERFORMANCE_DATA\\%s", szTemp);
		break;
	case HKEY_CURRENT_CONFIG:
		str.Format("HKEY_CURRENT_CONFIG\\%s", szTemp);
		break;
	case HKEY_DYN_DATA:
		str.Format("HKEY_DYN_DATA\\%s", szTemp);
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp2);
		str.Format("%s\\%s", szTemp2, szTemp);
	}
	m_listCtrl.SetItemText(index, 3, str);
	m_listCtrl.SetItemText(index, 4, "N/A");
	m_listCtrl.SetItemText(index, 5, "N/A");
	m_listCtrl.SetItemText(index, 6, "N/A");
}
// 处理相应的监视消息
void  CRegMonitorDlg::SetValueExA_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	CString str=_T("设置键值(A)");
	m_listCtrl.SetItemText(index, 2, str);
	HKeyToKeyName(params->PID, params->hKey, szTemp);
	m_listCtrl.SetItemText(index, 3, szTemp);
	if(params->buf1 == 0 || strlen(params->buf1) == 0)
		m_listCtrl.SetItemText(index, 4, "默认值");
	else
		m_listCtrl.SetItemText(index, 4, params->buf1);

	switch (params->type) // 提取键值数据类型
	{
	case REG_NONE:  // No value type
		m_listCtrl.SetItemText(index, 6, "REG_NONE");
		break;
	case REG_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_SZ");
		str.Format("%s", params->buf2);
		break;
	case REG_EXPAND_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_EXPAND_SZ");
		str.Format("%s", params->buf2);
		break;
	case REG_BINARY:
		m_listCtrl.SetItemText(index, 6, "REG_BINARY");
		str = "";
		int i;
		for(i=0; i < params->cbbuf2; i++)
		{
			str.Format("%s%02X ", str, *((BYTE*)params->buf2 + i));
		}
		break;
	case REG_DWORD:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2),
			*((DWORD*)params->buf2));
		break;
	case REG_DWORD_BIG_ENDIAN:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD_BIG_ENDIAN");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2),
			*((DWORD*)params->buf2));
		break;
	case REG_LINK:
		m_listCtrl.SetItemText(index, 6, "REG_LINK");
		break;
	case REG_MULTI_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_MULTI_SZ");
		break;
	case REG_RESOURCE_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_LIST");
		break;
	case REG_FULL_RESOURCE_DESCRIPTOR:
		m_listCtrl.SetItemText(index, 6, "REG_FULL_RESOURCE_DESCRIPTOR");
		break;
	case REG_RESOURCE_REQUIREMENTS_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_REQUIREMENTS_LIST");
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp);
		str.Format("%s\\%s", szTemp, params->buf2);
		break;
		}
		
	m_listCtrl.SetItemText(index, 5, str);
}

// 处理相应的监视消息
void  CRegMonitorDlg::SetValueExW_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	char szTemp2[1024]={0};
	CString str=_T("设置键值(W)");
	m_listCtrl.SetItemText(index, 2, str);
	HKeyToKeyName(params->PID, params->hKey, szTemp2);
	m_listCtrl.SetItemText(index, 3, szTemp2);

	WideToMultiByte((WCHAR*)params->buf1, szTemp);
	if(params->buf1 == 0 || strlen(params->buf1) == 0)
		m_listCtrl.SetItemText(index, 4, "默认值");
	else
		m_listCtrl.SetItemText(index, 4, szTemp);

	WideToMultiByte((WCHAR*)params->buf2, szTemp);
	switch (params->type) // 提取键值数据类型
	{
	case REG_NONE:
		m_listCtrl.SetItemText(index, 6, "REG_NONE");
		break;
	case REG_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_SZ");
		str.Format("%s", szTemp);
		break;
	case REG_EXPAND_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_EXPAND_SZ");
		str.Format("%s", szTemp);
		break;
	case REG_BINARY:
		m_listCtrl.SetItemText(index, 6, "REG_BINARY");
		str = "";
		int i;
		for(i=0; i<params->cbbuf2; i++)
		{
			str.Format("%s%02X ", str, *((BYTE*)params->buf2 + i));
		}
		break;
	case REG_DWORD:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2), 
			*((DWORD*)params->buf2));
		break;
	case REG_DWORD_BIG_ENDIAN:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD_BIG_ENDIAN");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2), 
			*((DWORD*)params->buf2));
		break;
	case REG_LINK:
		m_listCtrl.SetItemText(index, 6, "REG_LINK");
		break;
	case REG_MULTI_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_MULTI_SZ");
		break;
	case REG_RESOURCE_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_LIST");
		break;
	case REG_FULL_RESOURCE_DESCRIPTOR:
		m_listCtrl.SetItemText(index, 6, "REG_FULL_RESOURCE_DESCRIPTOR");
		break;
	case REG_RESOURCE_REQUIREMENTS_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_REQUIREMENTS_LIST");
		break;
	default:
		break;
	}

	m_listCtrl.SetItemText(index, 5, str);
}

// 处理相应的监视消息
void  CRegMonitorDlg::DeleteKeyA_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	CString str=_T("删除子键(A)");
	m_listCtrl.SetItemText(index, 2, str);

	switch (int(params->hKey))
	{
	case HKEY_CLASSES_ROOT:
		str.Format("HKEY_CLASSES_ROOT\\%s", params->buf1);
		break;
	case HKEY_CURRENT_USER:
		str.Format("HKEY_CURRENT_USER\\%s", params->buf1);
		break;
	case HKEY_LOCAL_MACHINE:
		str.Format("HKEY_LOCAL_MACHINE\\%s", params->buf1);
		break;
	case HKEY_USERS:
		str.Format("HKEY_USERS\\%s", params->buf1);
		break;
	case HKEY_PERFORMANCE_DATA:
		str.Format("HKEY_PERFORMANCE_DATA\\%s", params->buf1);
		break;
	case HKEY_CURRENT_CONFIG:
		str.Format("HKEY_CURRENT_CONFIG\\%s", params->buf1);
		break;
	case HKEY_DYN_DATA:
		str.Format("HKEY_DYN_DATA\\%s", params->buf1);
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp);
		str.Format("%s\\%s", szTemp, params->buf1);
	}
	m_listCtrl.SetItemText(index, 3, str);
	m_listCtrl.SetItemText(index, 4, "N/A");
	m_listCtrl.SetItemText(index, 5, "N/A");
	m_listCtrl.SetItemText(index, 6, "N/A");
}

// 处理相应的监视消息
void  CRegMonitorDlg::DeleteKeyW_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	char szTemp2[1024]={0};
	CString str=_T("删除子键(W)");
	m_listCtrl.SetItemText(index, 2, str);
		
	WideToMultiByte((WCHAR*)params->buf1, szTemp);
	switch (int(params->hKey))
	{
	case HKEY_CLASSES_ROOT:
		str.Format("HKEY_CLASSES_ROOT\\%s", szTemp);
		break;
	case HKEY_CURRENT_USER:
		str.Format("HKEY_CURRENT_USER\\%s", szTemp);
		break;
	case HKEY_LOCAL_MACHINE:
		str.Format("HKEY_LOCAL_MACHINE\\%s", szTemp);
		break;
	case HKEY_USERS:
		str.Format("HKEY_USERS\\%s", szTemp);
		break;
	case HKEY_PERFORMANCE_DATA:
		str.Format("HKEY_PERFORMANCE_DATA\\%s", szTemp);
		break;
	case HKEY_CURRENT_CONFIG:
		str.Format("HKEY_CURRENT_CONFIG\\%s", szTemp);
		break;
	case HKEY_DYN_DATA:
		str.Format("HKEY_DYN_DATA\\%s", szTemp);
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp2);
		str.Format("%s\\%s", szTemp2, szTemp);
	}
	m_listCtrl.SetItemText(index, 3, str);
	m_listCtrl.SetItemText(index, 4, "N/A");
	m_listCtrl.SetItemText(index, 5, "N/A");
	m_listCtrl.SetItemText(index, 6, "N/A");
}

// 处理相应的监视消息

void  CRegMonitorDlg::DeleteValueA_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
		CString str=_T("删除键值(A)");
	m_listCtrl.SetItemText(index, 2, str);
		
	switch (int(params->hKey))
	{
	case HKEY_CLASSES_ROOT:
		str = "HKEY_CLASSES_ROOT";
		break;
	case HKEY_CURRENT_USER:
		str = "HKEY_CURRENT_USER";
		break;
	case HKEY_LOCAL_MACHINE:
		str = "HKEY_LOCAL_MACHINE";
		break;
	case HKEY_USERS:
		str = "HKEY_USERS";
		break;
	case HKEY_PERFORMANCE_DATA:
		str = "HKEY_PERFORMANCE_DATA";
		break;
	case HKEY_CURRENT_CONFIG:
		str = "HKEY_CURRENT_CONFIG";
		break;
	case HKEY_DYN_DATA:
		str = "HKEY_DYN_DATA";
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp);
		str.Format("%s", szTemp);
	}
	m_listCtrl.SetItemText(index, 3, str);

	if(params->buf1 == 0 || strlen(params->buf1) == 0)
		m_listCtrl.SetItemText(index, 4, "默认值");
	else
		m_listCtrl.SetItemText(index, 4, params->buf1);

		m_listCtrl.SetItemText(index, 5, "N/A");
		m_listCtrl.SetItemText(index, 6, "N/A");
}

// 处理相应的监视消息

void  CRegMonitorDlg::DeleteValueW_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	char szTemp2[1024]={0};
	CString str=_T("删除键值(W)");
	m_listCtrl.SetItemText(index, 2, str);
		
	WideToMultiByte((WCHAR*)params->buf1, szTemp);
	switch (int(params->hKey))
	{
	case HKEY_CLASSES_ROOT:
		str = "HKEY_CLASSES_ROOT";
		break;
	case HKEY_CURRENT_USER:
		str = "HKEY_CURRENT_USER";
		break;
	case HKEY_LOCAL_MACHINE:
		str = "HKEY_LOCAL_MACHINE";
		break;
	case HKEY_USERS:
		str = "HKEY_USERS";
		break;
	case HKEY_PERFORMANCE_DATA:
		str = "HKEY_PERFORMANCE_DATA";
		break;
	case HKEY_CURRENT_CONFIG:
		str = "HKEY_CURRENT_CONFIG";
		break;
	case HKEY_DYN_DATA:
		str = "HKEY_DYN_DATA";
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp2);
		str.Format("%s", szTemp2);
	}
	m_listCtrl.SetItemText(index, 3, str);

	WideToMultiByte((WCHAR*)params->buf1, szTemp);
	if(params->buf1 == 0 || strlen(params->buf1) == 0)
		m_listCtrl.SetItemText(index, 4, "默认值");
	else
		m_listCtrl.SetItemText(index, 4, szTemp);

		m_listCtrl.SetItemText(index, 5, "N/A");
		m_listCtrl.SetItemText(index, 6, "N/A");
}


// 处理相应的监视消息
void  CRegMonitorDlg::QueryValueExA_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	char szTemp2[1024]={0};
	CString str=_T("键值查询(A)");
	m_listCtrl.SetItemText(index, 2, str);
		
	switch (int(params->hKey))
	{
	case HKEY_CLASSES_ROOT:
		str = "HKEY_CLASSES_ROOT";
		break;
	case HKEY_CURRENT_USER:
		str = "HKEY_CURRENT_USER";
		break;
	case HKEY_LOCAL_MACHINE:
		str = "HKEY_LOCAL_MACHINE";
		break;
	case HKEY_USERS:
		str = "HKEY_USERS";
		break;
	case HKEY_PERFORMANCE_DATA:
		str = "HKEY_PERFORMANCE_DATA";
		break;
	case HKEY_CURRENT_CONFIG:
		str = "HKEY_CURRENT_CONFIG";
		break;
	case HKEY_DYN_DATA:
		str = "HKEY_DYN_DATA";
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp2);
		str.Format("%s", szTemp2);
	}
	m_listCtrl.SetItemText(index, 3, str);
		
	if(params->buf1 == 0 || strlen(params->buf1) == 0)
		m_listCtrl.SetItemText(index, 4, "默认值");
	else
		m_listCtrl.SetItemText(index, 4, params->buf1);
		
	switch (params->type)
	{
	case REG_NONE:
		m_listCtrl.SetItemText(index, 6, "REG_NONE");
		if(params->cbbuf2 == 4)
			str.Format("0x%08X(%d)", *((DWORD*)params->buf2), 
			*((DWORD*)params->buf2));
		else
		{
			if (params->cbbuf2 == (int)strlen(params->buf2)+1)
				str.Format("%s", params->buf2);
			else
			{
				str = "";
				int i;
				for(i=0; i<params->cbbuf2; i++)
				{
					str.Format("%s%02X ", str, *((BYTE*)params->buf2 + i));
				}
			}
		}
		break;
	case REG_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_SZ");
		str.Format("%s", params->buf2);
		break;
	case REG_EXPAND_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_EXPAND_SZ");
		str.Format("%s", params->buf2);
		break;
	case REG_BINARY:
		m_listCtrl.SetItemText(index, 6, "REG_BINARY");
		str = "";
		int i;
		for(i=0; i<params->cbbuf2; i++)	
		{
				str.Format("%s%02X ", str,*((BYTE*)params->buf2 + i));
		}
		break;
	case REG_DWORD:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2), 
			*((DWORD*)params->buf2));
		break;
	case REG_DWORD_BIG_ENDIAN:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD_BIG_ENDIAN");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2), 
			*((DWORD*)params->buf2));
		break;
	case REG_LINK:
		m_listCtrl.SetItemText(index, 6, "REG_LINK");
		break;
	case REG_MULTI_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_MULTI_SZ");
		break;
	case REG_RESOURCE_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_LIST");
		break;
	case REG_FULL_RESOURCE_DESCRIPTOR:
		m_listCtrl.SetItemText(index, 6, "REG_FULL_RESOURCE_DESCRIPTOR");
		break;
	case REG_RESOURCE_REQUIREMENTS_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_REQUIREMENTS_LIST");
		break;
	default:
		CString sTemp;
		sTemp.Format("%d", params->type);
		m_listCtrl.SetItemText(index, 6, sTemp);
			
		str = "";
		break;
	}
	m_listCtrl.SetItemText(index, 5, str);
}

// 处理相应的监视消息
void  CRegMonitorDlg::QueryValueExW_Msg(PARAMS* params,int index)
{
	char szTemp[1024]={0};
	char szTemp2[1024]={0};
	CString str=_T("键值查询(W)");
	m_listCtrl.SetItemText(index, 2, str);

	switch (int(params->hKey))
	{
	case HKEY_CLASSES_ROOT:
		str = "HKEY_CLASSES_ROOT";
		break;
	case HKEY_CURRENT_USER:
		str = "HKEY_CURRENT_USER";
		break;
	case HKEY_LOCAL_MACHINE:
		str = "HKEY_LOCAL_MACHINE";
		break;
	case HKEY_USERS:
		str = "HKEY_USERS";
		break;
	case HKEY_PERFORMANCE_DATA:
		str = "HKEY_PERFORMANCE_DATA";
		break;
	case HKEY_CURRENT_CONFIG:
		str = "HKEY_CURRENT_CONFIG";
		break;
	case HKEY_DYN_DATA:
		str = "HKEY_DYN_DATA";
		break;
	default:
		HKeyToKeyName(params->PID, params->hKey, szTemp2);
		str.Format("%s", szTemp2);	
	}
	m_listCtrl.SetItemText(index, 3, str);
		
	WideToMultiByte((WCHAR*)params->buf1, szTemp);
	if(params->buf1 == 0 || strlen(params->buf1) == 0)
		m_listCtrl.SetItemText(index,4, "默认值");
	else
		m_listCtrl.SetItemText(index, 4, szTemp);
		
	WideToMultiByte((WCHAR*)params->buf2, szTemp);
	switch (params->type)
	{
	case REG_NONE:
		m_listCtrl.SetItemText(index, 6, "REG_NONE");
		if(params->cbbuf2 == 4)
			str.Format("0x%08X(%d)", *((DWORD*)params->buf2), 
			*((DWORD*)params->buf2));
		else
		{
			if (params->cbbuf2 == (int)wcslen((WCHAR*)params->buf2)*2+2)
					str.Format("%s", szTemp);
			else
			{
				str = "";
				int i;
				for(i=0; i<params->cbbuf2; i++)
				{
					str.Format("%s%02X ", str, *((BYTE*)params->buf2 + i));
				}
			}
		}
		break;
	case REG_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_SZ");
		str.Format("%s", szTemp);
		break;
	case REG_EXPAND_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_EXPAND_SZ");
		str.Format("%s", szTemp);
		break;
	case REG_BINARY:
		m_listCtrl.SetItemText(index, 6, "REG_BINARY");
		str = "";
		int i;
		for(i=0; i<params->cbbuf2; i++)
		{
			str.Format("%s%02X ", str, *((BYTE*)params->buf2 + i));
		}
		break;
	case REG_DWORD:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2), *((DWORD*)params->buf2));
			break;
	case REG_DWORD_BIG_ENDIAN:
		m_listCtrl.SetItemText(index, 6, "REG_DWORD_BIG_ENDIAN");
		str.Format("0x%08X(%d)", *((DWORD*)params->buf2), *((DWORD*)params->buf2));
		break;
	case REG_LINK:
		m_listCtrl.SetItemText(index, 6, "REG_LINK");
		break;
	case REG_MULTI_SZ:
		m_listCtrl.SetItemText(index, 6, "REG_MULTI_SZ");
		break;
	case REG_RESOURCE_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_LIST");
		break;
	case REG_FULL_RESOURCE_DESCRIPTOR:
		m_listCtrl.SetItemText(index, 6, "REG_FULL_RESOURCE_DESCRIPTOR");
		break;
	case REG_RESOURCE_REQUIREMENTS_LIST:
		m_listCtrl.SetItemText(index, 6, "REG_RESOURCE_REQUIREMENTS_LIST");
		break;
	default:
		CString sTemp;
		sTemp.Format("%d", params->type);
		m_listCtrl.SetItemText(index, 6, sTemp);
		str = "";
		break;
	}
	m_listCtrl.SetItemText(index, 5, str);
}

//////////////////////////////////////////////////////////////////////////////////////

// 处理接收到的  CopyData 消息
BOOL CRegMonitorDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// 判断是否为 进程管理器发来的进程间通信消息
	if(TYPE_PROCESSMANAGE_MSG == pCopyDataStruct->dwData)
	{
		ProcessMsg *pProMsg=(ProcessMsg *)pCopyDataStruct->lpData;
		if(0==pProMsg->PID)
		{
			GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(_T("请设置有效进程"));
		}
		else
		{
			//OnButtonStart();
			HWND hBtn = GetDlgItem(IDC_BUTTON_START)->GetSafeHwnd();
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_START, BN_CLICKED), (LPARAM)hBtn);
			SetFilter(pProMsg->PID);
			GetDlgItem(IDC_STATIC_TIP2)->SetWindowText("监视:"+(CString)pProMsg->ProcessName);
		}
		return CDialog::OnCopyData(pWnd, pCopyDataStruct);
	}

	SHFILEINFO shFileInfo = {0};
	CString str = _T("");
	PARAMS* params= (PARAMS*)pCopyDataStruct->lpData;
	char szPath[MAX_PATH];
	char szName[MAX_PATH];
	ProcessPidToPath(params->PID, szName, szPath);
	::SHGetFileInfo(szPath, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);
	if (m_bIgnoreRegedit)
	{
		CString strTemp = szName;
		strTemp.MakeLower();
		if (strTemp.Find("regedit.exe") > -1)
			return TRUE;
	}
	int index = m_listCtrl.InsertItem(0xFFFF, szPath, shFileInfo.iIcon);
   //获取当前时间
	CTime time = CTime::GetCurrentTime();	
	CString strTime;						
	strTime.Format("%d:%d:%d", time.GetHour(), time.GetMinute(), time.GetSecond());
    

  switch (pCopyDataStruct->dwData)
  {
	case TYPE_RegCreateKeyExA:
		m_listCtrl.SetItemText(index, 1, strTime);
        CreateKeyExA_Msg(params,index);
		break;
	case TYPE_RegCreateKeyExW:
	    m_listCtrl.SetItemText(index, 1, strTime);
        CreateKeyExW_Msg(params,index);
		break;
	///////////////////////////////////////////////////////////////////////
	case TYPE_RegSetValueExA:
		m_listCtrl.SetItemText(index, 1, strTime);
		SetValueExA_Msg(params,index);
		break;

	case TYPE_RegSetValueExW:
		m_listCtrl.SetItemText(index, 1, strTime);
		SetValueExW_Msg(params,index);
		break;
		///////////////////////////////////////////////////////////////////////
	case TYPE_RegDeleteKeyA:
		m_listCtrl.SetItemText(index, 1, strTime);
		DeleteKeyA_Msg(params,index);
		break;

	case TYPE_RegDeleteKeyW:
		m_listCtrl.SetItemText(index, 1, strTime);
		DeleteKeyW_Msg(params,index);
		break;
		///////////////////////////////////////////////////////////////////////
	case TYPE_RegDeleteValueA:
		m_listCtrl.SetItemText(index, 1, strTime);
		DeleteValueA_Msg(params,index);
		break;
	case TYPE_RegDeleteValueW:
		m_listCtrl.SetItemText(index, 1, strTime);
		DeleteValueW_Msg(params,index);
		break;
		///////////////////////////////////////////////////////////////////////
	case TYPE_RegQueryValueExA:
		m_listCtrl.SetItemText(index, 1, strTime);
		QueryValueExA_Msg(params,index);
		break;

	case TYPE_RegQueryValueExW:
		m_listCtrl.SetItemText(index, 1, strTime);
		QueryValueExA_Msg(params,index);
			break;
	default:
		break;
	}
	m_listCtrl.Scroll(CSize(0,10));
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CRegMonitorDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

// 通过进程ID转化到相应的进程 路径
void CRegMonitorDlg::ProcessPidToPath(DWORD ProcessId, PCHAR ProcessName, PCHAR ProcessPath)
{
	PROCESSENTRY32 info = {sizeof(PROCESSENTRY32)};
    MODULEENTRY32 minfo = {sizeof(MODULEENTRY32)};
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bReport = Process32First(hProcess,&info);
	
    while(bReport)
    {
		if(info.th32ProcessID == ProcessId)
		{
			HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, info.th32ProcessID);
			Module32First(hModule, &minfo);
			strcpy(ProcessName, info.szExeFile);
			strcpy(ProcessPath, minfo.szExePath);
			return ;
		}
        bReport = Process32Next(hProcess, &info);   
    }
	strcpy(ProcessName, "");
	strcpy(ProcessPath, "");
}

// 通过 Key 到 KeyName 枚举子键使用 ZwQueryKey(内核模式下)
void CRegMonitorDlg::HKeyToKeyName(DWORD PID, HKEY hKey, char *pszKeyName)
{
	HANDLE hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID); // 获得远程进程句柄
	
	HANDLE hKey2;
	// 复制远程进程句柄 通过这种方法达到多个进程共享一个内核对象
	DuplicateHandle(
		hRemoteProcess, 
		HANDLE(hKey),
		GetCurrentProcess(), 
		&hKey2, 
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS);

	WCHAR wszText[1024] = {0};
	ULONG len = 0;
	if(m_pZwQueryKey)
	{
		(*m_pZwQueryKey)(hKey2, KeyNameInformation, wszText, 1024, &len);// 查询该键的相关信息
		WideToMultiByte(wszText+2, pszKeyName); // 字符转换
	}
}



// 宽字节转换 映射一个unicode字符串到一个多字节字符串

int CRegMonitorDlg::WideToMultiByte(const WCHAR *wChar, char *mChar)
{
	int aLen = WideCharToMultiByte(CP_ACP,// ANSI代码页
		0, 
		wChar,// 指向将被转换的unicode字符串
		wcslen(wChar) + 1,
		NULL, 0, NULL, NULL);// 返回值是接收到待转换字符串的缓冲区所必需的字节数
	return WideCharToMultiByte(CP_ACP, 0, wChar, wcslen(wChar) + 1, mChar, aLen, NULL, NULL);
}

// 停止
void CRegMonitorDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	if(UninstallHook())
	{
		// 卸载后进行相应的操作
		m_hHook = NULL;
		CButton *pBt=(CButton *)GetDlgItem(IDC_BUTTON_STOP);
		pBt->SetCheck(BST_CHECKED);
		pBt->EnableWindow(false);
		pBt=(CButton *)GetDlgItem(IDC_BUTTON_START);
		pBt->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(_T("监视停止"));
		GetDlgItem(IDC_STATIC_TIP2)->SetWindowText(_T(""));
	}
	
}

// 处理菜单消息

void CRegMonitorDlg::OnMenuCreateKey() 
{
	m_bRegCreateKeyEx = !m_bRegCreateKeyEx;
	// 根据上次选择设置 Check  状态
	UINT state = m_Psubmenu->GetMenuState(IDR_MENU11, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	if (state & MF_CHECKED)
		m_Psubmenu->CheckMenuItem(IDR_MENU11, MF_UNCHECKED | MF_BYCOMMAND);
	else
		m_Psubmenu->CheckMenuItem(IDR_MENU11, MF_CHECKED | MF_BYCOMMAND);
	
	UpdateHookFunction();
}

void CRegMonitorDlg::OnMenuDelKey() 
{
	m_bRegDeleteKey = !m_bRegDeleteKey;
	// 根据上次选择设置 Check  状态
	UINT state = m_Psubmenu->GetMenuState(IDR_MENU12, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	
	if (state & MF_CHECKED)
		m_Psubmenu->CheckMenuItem(IDR_MENU12, MF_UNCHECKED | MF_BYCOMMAND);
	else
		m_Psubmenu->CheckMenuItem(IDR_MENU12, MF_CHECKED | MF_BYCOMMAND);
	
	UpdateHookFunction();	
}

void CRegMonitorDlg::OnMenuSetValue() 
{
	m_bRegSetValueEx = !m_bRegSetValueEx;
	// 根据上次选择设置 Check  状态
	UINT state = m_Psubmenu->GetMenuState(IDR_MENU13, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	
	if (state & MF_CHECKED)
		m_Psubmenu->CheckMenuItem(IDR_MENU13, MF_UNCHECKED | MF_BYCOMMAND);
	else
		m_Psubmenu->CheckMenuItem(IDR_MENU13, MF_CHECKED | MF_BYCOMMAND);
	
	UpdateHookFunction();
}

void CRegMonitorDlg::OnMenuDelValue() 
{
	m_bRegDeleteValue = !m_bRegDeleteValue;
	// 根据上次选择设置 Check  状态
	UINT state = m_Psubmenu->GetMenuState(IDR_MENU14, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	
	if (state & MF_CHECKED)
		m_Psubmenu->CheckMenuItem(IDR_MENU14, MF_UNCHECKED | MF_BYCOMMAND);
	else
		m_Psubmenu->CheckMenuItem(IDR_MENU14, MF_CHECKED | MF_BYCOMMAND);
	
	UpdateHookFunction();
}

void CRegMonitorDlg::OnMenuQueryValue() 
{
	m_bRegQueryValueEx = !m_bRegQueryValueEx;
	// 根据上次选择设置 Check  状态
	UINT state = m_Psubmenu->GetMenuState(IDR_MENU15, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);
	
	if (state & MF_CHECKED)
		m_Psubmenu->CheckMenuItem(IDR_MENU15, MF_UNCHECKED | MF_BYCOMMAND);
	else
		m_Psubmenu->CheckMenuItem(IDR_MENU15, MF_CHECKED | MF_BYCOMMAND);
	
	UpdateHookFunction();
}

// 打开注册表工具
void CRegMonitorDlg::OnMenuOpenRegedit() 
{
	SHELLEXECUTEINFO ShellInfo; //定义SHELLEXECUTEINFO结构
	memset(&ShellInfo, 0, sizeof(ShellInfo)); 
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL; 
	ShellInfo.lpVerb = _T("open"); //打开
	ShellInfo.lpFile = _T("regedit.exe");//打开注册表编辑器
	ShellInfo.nShow = SW_SHOWNORMAL;//正常显示一个窗口同时令其进入活动状态 
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShellExecuteEx(&ShellInfo);
}

 // 保存记录
void CRegMonitorDlg::OnButtonSave() 
{
	//判断列表是否为空
	if (m_listCtrl.GetItemCount() <= 0)
	{
		MessageBox("列表信息为空!");
		return;
	}

	//构造保存对话框类
	CFileDialog filedlg(FALSE, NULL, "Log.txt", OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,
		"日志文件(*.log)|*.log||", this); 
	// 打开对话框
	if (IDOK != filedlg.DoModal()) 
	{
		return;
	}
	// 获取选择的文件路径名
	CString strPathName = filedlg.GetPathName(); 


	//保存文件
	CFile file;
	file.Open(strPathName, CFile::modeCreate | CFile::modeWrite); // 打开文件
	for (int i=0; i<m_listCtrl.GetItemCount(); i++)
	{
		for (int j=0; j<7; j++)
		{
			file.Write(m_listCtrl.GetItemText(i, j), strlen(m_listCtrl.GetItemText(i, j))); // 数据写入文件
			file.Write("   ", 3); // 各项之间设置空格
		}
		file.Write("\r\n\r\n", strlen("\r\n\r\n")); //写换行
	}
	file.Close(); // 关闭文件
}

void CRegMonitorDlg::OnMenuOpenProcessManage() 
{
	// 查找ProcessManage 窗口是否存在
	HWND hWnd =::FindWindow(NULL,_T("ProcessManage")); 
	if(NULL==hWnd)
	{
		// 运行此程序

		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };   
		// Ask for privileges elevation.   
		sei.lpVerb = TEXT("runas");      
		sei.lpFile = _T("ProcessManage.exe");   
		sei.nShow = SW_SHOWNORMAL;   
		ShellExecuteEx(&sei);
		WaitForSingleObject(sei.hProcess, INFINITE);
	}
	else // 显示此窗口
	{

		::ShowWindow(hWnd,SW_HIDE);// 隐藏一下
		::ShowWindow(hWnd,SW_RESTORE);// 显示窗口
	}
}

// 清除列表记录
void CRegMonitorDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_listCtrl.DeleteAllItems();
}
