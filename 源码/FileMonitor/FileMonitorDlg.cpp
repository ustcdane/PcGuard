// FileMonitorDlg.cpp : implementation file
//

#include <ctime>
#include <cstdlib>
#include "stdafx.h"
#include "FileMonitor.h"
#include "FileMonitorDlg.h"
#include "SetDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CFileMonitorDlg dialog

CFileMonitorDlg::CFileMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileMonitorDlg)
	m_strPathName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hThread = NULL; // 初始化线程句柄
	m_nNumber = 0; // 初始化文件或文件夹的序号 
	m_cNumber = new char[MAX_PATH]; // 初始化文件或文件夹的序号 
	itoa(m_nNumber, m_cNumber, 10); // 整型转换成字符型
	m_hDir = INVALID_HANDLE_VALUE; // 初始化目录句柄
    m_Hmodule=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//析构函数，释放资源
CFileMonitorDlg::~CFileMonitorDlg()
{
	//释放资源
	if(m_Hmodule)
	{
		::FreeLibrary(m_Hmodule);
	}
	// 终止线程
	if(m_hThread != NULL)
	{
		::TerminateThread(m_hThread, 0);
		m_hThread = NULL;
	}
	// 关闭目录句柄
	if(m_hDir !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDir);
		m_hDir = INVALID_HANDLE_VALUE;
	}
}

void CFileMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileMonitorDlg)
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtSet);
	DDX_Control(pDX, IDC_LIST, m_CListCtrl);
	DDX_Text(pDX, IDC_EDIT_PATHNAME, m_strPathName);
	//}}AFX_DATA_MAP
}
//消息映射
BEGIN_MESSAGE_MAP(CFileMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CFileMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_COMMAND(IDM_OPEN,OnOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileMonitorDlg message handlers

//初始化对话框元素
void CFileMonitorDlg::InitDlgItem()
{
	m_CListCtrl.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_CListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);
	m_CListCtrl.SetBkColor(RGB(247,247,255));
	m_CListCtrl.SetTextColor(RGB(0,100,155));
	m_CListCtrl.SetTextBkColor(RGB(247,247,255));

	m_CListCtrl.InsertColumn(0,_T("序号"),LVCFMT_LEFT,60);
	m_CListCtrl.InsertColumn(1,_T("时间"),LVCFMT_LEFT,80);
	m_CListCtrl.InsertColumn(2,_T("动作类型"),LVCFMT_LEFT,100);
	m_CListCtrl.InsertColumn(3,_T("文件所在路径"),LVCFMT_LEFT,400);
	
	//读取配置文件
	m_bAddNew = (::GetPrivateProfileInt("Selections","Add",0,".\\Config.ini")==0 ?false:true);
	m_bDel = (::GetPrivateProfileInt("Selections","Delete",0,".\\Config.ini")==0 ?false:true);
	m_bAttribute = (::GetPrivateProfileInt("Selections","Attribute",0,".\\Config.ini")==0 ?false:true);
	m_bOccurTime = (::GetPrivateProfileInt("Selections","OccurTime",0,".\\Config.ini")==0 ?false:true);
	m_bRename=(::GetPrivateProfileInt("Selections","Rename",0,".\\Config.ini")==0 ?false:true);
	m_bOther = (::GetPrivateProfileInt("Selections","Other",0,".\\Config.ini")==0 ?false:true);
	//初始化字体
	LOGFONT lf;
	::ZeroMemory(&lf,sizeof(lf)); 
	lf.lfHeight = 90;         
	lf.lfWeight = FW_NORMAL;   
	lf.lfItalic = false;	
	::lstrcpy(lf.lfFaceName,_T("Verdana")); 
	m_hFont.CreatePointFontIndirect(&lf);
	GetDlgItem(IDC_EDIT_PATHNAME)->SetFont(&m_hFont);
	/*******************************///加载皮肤/*****************************************/
	//产生随机数
	srand((unsigned)time(0));
	int i=rand()%35 +1;
	CString s=_T("");
	s.Format("skin//%d.she",i);
	//采用运行期间动态链接方式
	//加载目标DLL
	m_Hmodule=::LoadLibrary("SkinH.dll");
	//宏定义函数指针类型 
	typedef int ( WINAPI *SKINH_ATTACHEX)(LPCTSTR strSkinFile,LPCTSTR strPassword);
	
	if(NULL !=m_Hmodule)
	{						// 取得SKINH_ATTACHEX函数的地址
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

   
	m_BtSet.ModifyStyle(0, BS_ICON);
	//加载
	 m_hIconSet=AfxGetApp()->LoadIcon(IDI_ICON1);
	m_BtSet.SetIcon(m_hIconSet);

}
BOOL CFileMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitDlgItem();//调用初始化函数

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	CDialog::OnSysCommand(nID, lParam);
	
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileMonitorDlg::OnPaint() 
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
HCURSOR CFileMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFileMonitorDlg::OnButtonStart() 
{
	if (m_strPathName != "") // 判断要监视的目录是否空
	{
		DWORD ThreadID;  
		m_hThread=::CreateThread(NULL, 0, ThreadMonitorProc, this, 0, &ThreadID); //创建监视线程
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PATH)->SetWindowText("正在监控...");
	}
	else
	{
		MessageBox("请选择要监控的目录!");
	}
}

//停止监视
void CFileMonitorDlg::OnButtonStop() 
{
	
	// 终止线程
	if(m_hThread != NULL)
	{
		::TerminateThread(m_hThread, 0);
		m_hThread = NULL;
	}
	// 关闭目录句柄
	if(m_hDir !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDir);
		m_hDir = INVALID_HANDLE_VALUE;
	}
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText("停止了监控...");
}

//清空列表
void CFileMonitorDlg::OnButtonClear() 
{
	m_CListCtrl.DeleteAllItems();
    SetDlgItemText(IDC_STATIC_LOG,_T("0条记录"));
}
//保存列表记录
void CFileMonitorDlg::OnButtonSave() 
{
	if (m_CListCtrl.GetItemCount() <= 0)
	{
		MessageBox("列表信息为空!");
		return;
	}
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
	char appPath[256];
	GetCurrentDirectory(256, appPath);//取得应用程序当前路径
	CString strFilePath=_T(""); //路径
	strFilePath.Format("%s",appPath);
	strFilePath +="\\log.txt";
	//保存文件
	CFile file;
	file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite); // 打开文件
	for (int i=0; i<m_CListCtrl.GetItemCount(); i++)
	{
		for (int j=0; j<4; j++)
		{
			file.Write(m_CListCtrl.GetItemText(i, j), strlen(m_CListCtrl.GetItemText(i, j))); // 数据写入文件
			file.Write("   ", 3); //各项空格隔开
		}
		file.Write("\r\n\r\n", strlen("\r\n\r\n")); //各个条目换行
	}
	file.Close(); // 关闭文件
}

//导出监视记录
void CFileMonitorDlg::OnButtonExport() 
{
	// CFileDialog内存放路径的缓冲区长度为260,你反复选择文件之后,超过了缓冲区长度会出错 所以要 new 
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
	//构造保存对话框类
	CFileDialog *FileDlg= new CFileDialog(FALSE, NULL, ".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
		"日志文件(*.log)|*.log||", this); 
	CString strfileNamePath;//导出到的文件名及路径
	char appPath[MAX_PATH]={'\0'};
	::GetCurrentDirectory(MAX_PATH, appPath);//取得应用程序当前路径
	CString strFilePath; //要导出文件路径
	strFilePath.Format("%s",appPath);
	strFilePath +="\\log.txt";
	if(IDOK!=FileDlg->DoModal())
	{
		return ;
	}
	strfileNamePath=FileDlg->GetPathName();//得到文件名及路径
		
	::MoveFile((LPCTSTR)strFilePath,(LPCTSTR)strfileNamePath);//移动到要导出的目录中
	delete FileDlg;
}

void CFileMonitorDlg::OnBtnBrowse() 
{
	TCHAR bufName[MAX_PATH]; //定义宽字符型字符串,结构显示名  
	TCHAR bufPath[MAX_PATH]; //定义宽字符型字符串,路径

	BROWSEINFO broInfo; // 定义浏览目录结构
	broInfo.hwndOwner = m_hWnd; // 父窗口的句柄
	broInfo.pidlRoot = NULL; // 指定根目录
	broInfo.pszDisplayName = bufName; 
	broInfo.lpszTitle = _T("选择目录"); // 位于对话框顶端的一行文字 
	broInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT; // 标志变量，按位有效
	broInfo.lpfn = NULL; // 回调函数   
	broInfo.lParam = NULL; // 传给回调函数的参数，一个32位值  

	//获取目录路径
	LPCITEMIDLIST pidl = SHBrowseForFolder(&broInfo); 
	if( pidl == NULL )
		return;

	//赋给宽字符串变量并显示
	SHGetPathFromIDList( pidl, bufPath );	
	m_strPathName = bufPath;	
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText("您选择的监控目录为：" + m_strPathName);
	UpdateData(FALSE);
}
//监视线程
 DWORD WINAPI CFileMonitorDlg::ThreadMonitorProc(LPVOID lParam)
{
	//参数转换
	CFileMonitorDlg * pDlg = (CFileMonitorDlg *)lParam;
	//创建文件，打开目录，得到目录句柄
	pDlg->m_hDir = CreateFile(pDlg->m_strPathName, GENERIC_READ|GENERIC_WRITE, 
                                               		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
		                                       		NULL, 
		                                       		OPEN_EXISTING,
		                                       		FILE_FLAG_BACKUP_SEMANTICS, NULL);
	//创建成功与否判断
	if (pDlg->m_hDir == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// 获取监控的返回值
	char bufTemp[(sizeof(FILE_NOTIFY_INFORMATION)+MAX_PATH)*2];
	FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)bufTemp;
	DWORD dwReturn;
	
	int Num=0; //显示记录条数
	while (TRUE)
	{
		//检测目录发生变化
		if (::ReadDirectoryChangesW(pDlg->m_hDir, pNotify, sizeof(bufTemp), true, 
			FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|
			FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|
			FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_LAST_ACCESS|
			FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY,
			&dwReturn, NULL, NULL))
		{
			//获取当前时间
			CTime time = CTime::GetCurrentTime();	
			CString strTime;						
			strTime.Format("%d:%d:%d", time.GetHour(), time.GetMinute(), time.GetSecond());
			//获取变化的文件名
			char strName[1024];
			memset(strName, 0, sizeof(strName) );
			WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength/2, strName, 99, NULL, NULL);
			//监视动作
			switch (pNotify->Action)
			{
			case FILE_ACTION_ADDED: // 新增
				{
					if(pDlg->m_bAddNew)
					{
						pDlg->m_CListCtrl.InsertItem(0, pDlg->m_cNumber);
						if(pDlg->m_bOccurTime)
						{
							pDlg->m_CListCtrl.SetItemText(0, 1, strTime);
						}
						pDlg->m_CListCtrl.SetItemText(0, 2, "添加了文件");
						pDlg->m_CListCtrl.SetItemText(0, 3, strName);
					}
					break;
				}
			case FILE_ACTION_REMOVED: // 删除
				
					if(pDlg->m_bDel)
					{
						//列表框中添加信息
						pDlg->m_CListCtrl.InsertItem(0, pDlg->m_cNumber);
						if(pDlg->m_bOccurTime)
						{
							pDlg->m_CListCtrl.SetItemText(0, 1, strTime);
						}
						pDlg->m_CListCtrl.SetItemText(0, 2, "删除了文件");
						pDlg->m_CListCtrl.SetItemText(0, 3, strName);
					}
				
					break;
				
			case FILE_ACTION_MODIFIED: // 属性改变
				
					if(pDlg->m_bAttribute)
					{
						pDlg->m_CListCtrl.InsertItem(0, pDlg->m_cNumber);
						if(pDlg->m_bOccurTime)
						{
							pDlg->m_CListCtrl.SetItemText(0, 1, strTime);
						}
						pDlg->m_CListCtrl.SetItemText(0, 2, "修改了文件");
						pDlg->m_CListCtrl.SetItemText(0, 3, strName);
			
					}

					break;
				
			case FILE_ACTION_RENAMED_OLD_NAME: // 重命名
				
					if(pDlg->m_bRename)
					{
						pDlg->m_CListCtrl.InsertItem(0, pDlg->m_cNumber);
						if(pDlg->m_bOccurTime)
						{
							pDlg->m_CListCtrl.SetItemText(0, 1, strTime);
						}
						pDlg->m_CListCtrl.SetItemText(0, 2, "重命名了文件");
						pDlg->m_CListCtrl.SetItemText(0, 3, strName);
					}
					break;
				
			case FILE_ACTION_RENAMED_NEW_NAME: // 重命名
				
					if(pDlg->m_bRename)
					{
						pDlg->m_CListCtrl.InsertItem(0, pDlg->m_cNumber);
						if(pDlg->m_bOccurTime)
						{
							pDlg->m_CListCtrl.SetItemText(0, 1, strTime);
						}
						pDlg->m_CListCtrl.SetItemText(0, 2, "重命名了文件");
						pDlg->m_CListCtrl.SetItemText(0, 3, strName);

					}
				
					break;
				
			default:
					//其它
					if(pDlg->m_bOther)
					{
						pDlg->m_CListCtrl.InsertItem(0, pDlg->m_cNumber);
						if(pDlg->m_bOccurTime)
						{
							pDlg->m_CListCtrl.SetItemText(0, 1, strTime);
						}
						pDlg->m_CListCtrl.SetItemText(0, 2, "未发生变化");
						pDlg->m_CListCtrl.SetItemText(0, 3, strName);
					}
					break;
			}
			//增加相应的序号
			Num++;
			pDlg->m_nNumber++; // 序号加一
			itoa(pDlg->m_nNumber, pDlg->m_cNumber, 10); // 整型转换成字符型(十进制)
			
		}
		else 
		{
			break; 
		}

		CString str;
		str.Format("%d",Num);
		str += "条记录";

		pDlg->SetDlgItemText(IDC_STATIC_LOG,str);//显示已经监视的条目数
	}

	return 0;
}
void CFileMonitorDlg::OnButtonSet() 
{
	CSetDlg set;
	if(set.DoModal() == IDOK)
	{
		this->m_bAddNew = set.m_bAddNew;
		this->m_bDel = set.m_bDel;
		this->m_bAttribute = set.m_bAttribute;
		this->m_bRename = set.m_bRename;
		this->m_bOccurTime= set.m_bOccurTime;
		this->m_bOther  = set.m_bOther;
		
		//写入.ini 配置文件
		::WritePrivateProfileString("Selections","Add",set.m_bAddNew? "1":"0",".\\Config.ini");
		::WritePrivateProfileString("Selections","Delete",set.m_bDel? "1":"0",".\\Config.ini");
		::WritePrivateProfileString("Selections","Attribute",set.m_bAttribute? "1":"0",".\\Config.ini");
		::WritePrivateProfileString("Selections","OccurTime",set.m_bOccurTime? "1":"0",".\\Config.ini");
		::WritePrivateProfileString("Selections","Rename",set.m_bRename? "1":"0",".\\Config.ini");
		::WritePrivateProfileString("Selections","Other",set.m_bOther? "1":"0",".\\Config.ini");
	}	
}



void CFileMonitorDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CFileMonitorDlg::OnOpen() 
{
	
  	CString tem = m_strPathName;
	CString str = _T("Explorer /select,");
	int nItem = m_CListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	tem += m_CListCtrl.GetItemText(nItem, 3);
	str +=tem;
	WinExec(str,SW_SHOWNORMAL);//显示文件的位置
	
}
