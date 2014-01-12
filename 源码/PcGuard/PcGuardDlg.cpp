// PcGuardDlg.cpp : implementation file
////////////////////////////////////////////////////////////
// PcGuard 主程序	2011.6.09
// 网络08-2 王丹 
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PcGuard.h"
#include "PcGuardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcGuardDlg dialog

CPcGuardDlg::CPcGuardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPcGuardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPcGuardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPcGuardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPcGuardDlg)
	DDX_Control(pDX, IDC_BUTTON_KILL, m_BtKill);
	DDX_Control(pDX, IDC_BUTTON_REG, m_BtReg);
	DDX_Control(pDX, IDC_BUTTON_FILE, m_BtFile);
	DDX_Control(pDX, IDC_BUTTON_PRO, m_BtPro);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPcGuardDlg, CDialog)
	//{{AFX_MSG_MAP(CPcGuardDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PRO, OnButtonPro)
	ON_BN_CLICKED(IDC_BUTTON_KILL, OnButtonKill)
	ON_BN_CLICKED(IDC_BUTTON_REG, OnButtonReg)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_COMMAND(ID_MENU_SHOW, OnMenuShow)
	ON_COMMAND(ID_MENU_QUIT, OnMenuQuit)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE( WM_TRAYICON_MSG,OnTrayCallBackMsg)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcGuardDlg message handlers

BOOL CPcGuardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//   给按钮加载 icon
    m_BtPro.ModifyStyle(0,BS_ICON);
	m_BtPro.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_PRO));

	m_BtReg.ModifyStyle(0,BS_ICON);
	m_BtReg.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_REG));

	m_BtFile.ModifyStyle(0,BS_ICON);
	m_BtFile.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_FILE));

	m_BtKill.ModifyStyle(0,BS_ICON);
	m_BtKill.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_KILL));

     m_sText=_T("Virus-free PC, enjoy the Internet of life!");
	//将静态文本的屏幕坐标存放在m_pRectLink中    
     GetDlgItem(IDC_LINK) -> GetWindowRect(&m_pRectLink);    
    //将屏幕坐标转换为客户坐标    
     ScreenToClient(&m_pRectLink); 
     m_nScrollXPos=0; // 滚动字幕开始位置

		//宏定义函数指针类型 
	typedef int ( WINAPI *SKINH_ATTACHEX)(LPCTSTR strSkinFile,LPCTSTR strPassword);
	// 取得SKINH_ATTACHEX函数的地址
		SKINH_ATTACHEX pFunc = (SKINH_ATTACHEX)::GetProcAddress(::LoadLibrary("SkinH.dll"), "SkinH_AttachEx");
		if(pFunc)
		{
			// 加载皮肤文件
			pFunc(_T("skin\\pixos.she"), NULL);
		}
		else
		{
			MessageBox(_T("皮肤库加载失败！"),_T("缺少SkinH.dll"));
		}
    // 设置计时器
	SetTimer(1,800,0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPcGuardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_MINIMIZE )
	{	
		TrayMyIcon(); // 最小化时任务托盘
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPcGuardDlg::OnPaint() 
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
HCURSOR CPcGuardDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPcGuardDlg::OnButtonPro() 
{
	// TODO: Add your control notification handler code here
	// 查找ProcessManage 窗口是否存在
	HWND hWnd =::FindWindow(NULL,_T("ProcessManage")); 
	if(NULL==hWnd)
	{
		// 运行此程序
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };   
		// Ask for privileges elevation.   
		sei.lpVerb = _T("runas");      
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
	GetDlgItem(IDC_STATIC_PRO)->SetWindowText(_T("进程管理器已运行!"));
}

void CPcGuardDlg::OnButtonKill() 
{
	// TODO: Add your control notification handler code here
	// 查找 窗口是否存在
	HWND hWnd =::FindWindow(NULL,_T("VirusKiller")); 
	if(NULL==hWnd)
	{
		// 运行此程序

		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };   
		// Ask for privileges elevation.   
		sei.lpVerb = _T("runas");      
		sei.lpFile = _T("VirusKiller.exe");   
		sei.nShow = SW_SHOWNORMAL;   
		ShellExecuteEx(&sei);
		WaitForSingleObject(sei.hProcess, INFINITE);
	}
	else // 显示此窗口
	{

		::ShowWindow(hWnd,SW_HIDE);// 隐藏一下
		::ShowWindow(hWnd,SW_RESTORE);// 显示窗口
	}
	GetDlgItem(IDC_STATIC_KILL)->SetWindowText(_T("病毒查杀已运行!"));
}

void CPcGuardDlg::OnButtonReg() 
{
	// TODO: Add your control notification handler code here
		// 查找 窗口是否存在
	HWND hWnd =::FindWindow(NULL,_T("RegMonitor")); 
	if(NULL==hWnd)
	{
		// 运行此程序

		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };   
		// Ask for privileges elevation.   
		sei.lpVerb = _T("runas");      
		sei.lpFile = _T("RegMonitor.exe");   
		sei.nShow = SW_SHOWNORMAL;   
		ShellExecuteEx(&sei);
		WaitForSingleObject(sei.hProcess, INFINITE);
	}
	else // 显示此窗口
	{

		::ShowWindow(hWnd,SW_HIDE);// 隐藏一下
		::ShowWindow(hWnd,SW_RESTORE);// 显示窗口
	}
	GetDlgItem(IDC_STATIC_REG)->SetWindowText(_T("注册表监视已运行!"));
}

void CPcGuardDlg::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
		// 查找 窗口是否存在
	HWND hWnd =::FindWindow(NULL,_T("FileMonitor")); 
	if(NULL==hWnd)
	{
		// 运行此程序

		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };   
		// Ask for privileges elevation.   
		sei.lpVerb = _T("runas");      
		sei.lpFile = _T("FileMonitor.exe");   
		sei.nShow = SW_SHOWNORMAL;   
		ShellExecuteEx(&sei);
		WaitForSingleObject(sei.hProcess, INFINITE);
	}
	else // 显示此窗口
	{

		::ShowWindow(hWnd,SW_HIDE);// 隐藏一下
		::ShowWindow(hWnd,SW_RESTORE);// 显示窗口
	}
	GetDlgItem(IDC_STATIC_FILE)->SetWindowText(_T("文件监视已运行!"));
}

void CPcGuardDlg::OnTimer(UINT nIDEvent) 
{
	// 动态显示字幕
	if(m_nScrollXPos>m_sText.GetLength())// 判断显示完毕吗
	{
		m_nScrollXPos=0; // 开始位置设为0 重新开始显示
	}
	else
	{
		GetDlgItem(IDC_STATIC_MARQUE)->SetWindowText(m_sText.Left(m_nScrollXPos));
		m_nScrollXPos+=3;// 每次增加三个字符
	}
	// 查看进程管理器开启吗？
	if(::FindWindow(NULL,_T("ProcessManage")))
	{
		GetDlgItem(IDC_STATIC_PRO)->SetWindowText(_T("进程管理器已运行!"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_PRO)->SetWindowText(_T("进程管理器没启动!"));
	}
	// 查看杀毒开启吗？
	if(::FindWindow(NULL,_T("VirusKiller")))
	{
		GetDlgItem(IDC_STATIC_KILL)->SetWindowText("病毒查杀已运行!");
	}
	else
	{
		GetDlgItem(IDC_STATIC_KILL)->SetWindowText("病毒查杀没启动!");
	}
	// 查看注册表监视开启吗？
	if(::FindWindow(NULL,_T("RegMonitor")))
	{
		GetDlgItem(IDC_STATIC_REG)->SetWindowText(_T("注册表监视已运行!"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_REG)->SetWindowText(_T("注册表监视没启动!"));
	}
    // 查看文件监视开启吗?
	if(::FindWindow(NULL,_T("FileMonitor")))
	{
		GetDlgItem(IDC_STATIC_FILE)->SetWindowText(_T("文件监视已运行!"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_FILE)->SetWindowText(_T("文件监视没启动!"));
	}
	CDialog::OnTimer(nIDEvent);
}

void CPcGuardDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CPcGuardDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}

bool CPcGuardDlg::TrayMyIcon(bool bAdd)
{
	BOOL bRet = false;
	m_Ntnd.cbSize = sizeof(NOTIFYICONDATA); //设置任务托盘有关结构大小
	m_Ntnd.hWnd = GetSafeHwnd(); // 如果对托盘中的图标进行操作，相应的消息就传给这个句柄所代表的窗口
	m_Ntnd.uID = IDR_MAINFRAME; // 设置托盘图标ID
	if ( bAdd == TRUE )
	{
		// 这个成员标志着其他哪些成员的数据是有效的，分别为
		// NIF_ICON, NIF_MESSAGE, NIF_TIP，分别代表着数据有效的成员是hIcon, uCallbackMessage, szTip
		m_Ntnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		// 这是个消息标志，当用鼠标对托盘区相应图标进行操作的时候，就会传递消息给Hwnd所代表的窗口
		m_Ntnd.uCallbackMessage = WM_TRAYICON_MSG; ;// 自定义的消息名称
		// 要增加，删除或修改的图标句柄
		m_Ntnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		// 鼠标移动到托盘图标上时的提示文字
		strncpy(m_Ntnd.szTip, _T("PcGuard \n PC卫士"), sizeof(m_Ntnd.szTip));
		ShowWindow(SW_MINIMIZE);// 最小化
		ShowWindow(SW_HIDE);// 隐藏主窗口
		bRet = Shell_NotifyIcon(NIM_ADD, &m_Ntnd);// 向系统传递消息，以添加托盘区的图标
	}
	else
	{
		ShowWindow(SW_SHOWNA); // 显示窗口
		SetForegroundWindow(); // 把窗口设置在前
		bRet = Shell_NotifyIcon(NIM_DELETE, &m_Ntnd);// 向系统传递消息 删除托盘区的图标
	}
	return bRet;
}

// 当用户用鼠标点击托盘区的图标的时候(无论是左键还是右键),会向hWnd所代表的窗口传送消息
LRESULT CPcGuardDlg::OnTrayCallBackMsg(WPARAM wparam, LPARAM lparam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
	if(wparam!=IDR_MAINFRAME)
		return 0;
	switch(lparam)
	{
	case WM_RBUTTONUP:
		{
			CMenu mMenu, *pMenu = NULL;
			CPoint pt;
			mMenu.LoadMenu(IDR_MENU1);
			pMenu = mMenu.GetSubMenu(0);
			GetCursorPos(&pt);//得到鼠标位置
			SetForegroundWindow();
			//确定弹出式菜单的位置
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
			//资源回收
			HMENU hmenu=mMenu.Detach();
			mMenu.DestroyMenu();
			break;
		}
	case WM_LBUTTONDBLCLK: // 单击任务托盘图标
		ShowWindow(SW_RESTORE);// 激活窗口
		SetForegroundWindow(); // 显示最前
		TrayMyIcon(false);  // 取消托盘
		break;

	default:
		break;
	}
	return 1;
}

//显示主程序界面
void CPcGuardDlg::OnMenuShow() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_RESTORE); // 显示窗口
	TrayMyIcon(FALSE);// 取消托盘
}

// 退出

void CPcGuardDlg::OnMenuQuit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE,0,0);
}

void CPcGuardDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	   //下面设置鼠标在静态文本区时，将光标设成小手状    
    if (point.x > m_pRectLink.left && point.x < m_pRectLink.right && point.y > m_pRectLink.top && point.y < m_pRectLink.bottom )    
    //此处添加判断坐标算法    
     {    
        HCURSOR hCursor;    
         hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);    
        //将鼠标设为小手状    
         SetCursor(hCursor);    
     }    

	CDialog::OnMouseMove(nFlags, point);
}

void CPcGuardDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//此处添加判断坐标算法    
    if (point.x > m_pRectLink.left && point.x < m_pRectLink.right && point.y > m_pRectLink.top && point.y < m_pRectLink.bottom)    
	{    
        //鼠标左键按下    
        if (nFlags == MK_LBUTTON)       
		{    
            //为改善鼠标效果，此处加入以上变换鼠标形状的代码    
			ShellExecute(0, NULL, "http://blog.csdn.net/gfsfg8545", NULL,NULL, SW_NORMAL);        
		}    
	}    
	CDialog::OnLButtonDown(nFlags, point);
}
