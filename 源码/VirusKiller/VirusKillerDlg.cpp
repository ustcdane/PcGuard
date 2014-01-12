// VirusKillerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirusKiller.h"
#include "VirusKillerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CVirusKillerDlg dialog

CVirusKillerDlg::CVirusKillerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirusKillerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVirusKillerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirusKillerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirusKillerDlg)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVirusKillerDlg, CDialog)
	//{{AFX_MSG_MAP(CVirusKillerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirusKillerDlg message handlers

BOOL CVirusKillerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here
	

	//宏定义函数指针类型 
	typedef int ( WINAPI *SKINH_ATTACHEX)(LPCTSTR strSkinFile,LPCTSTR strPassword);
	// 取得SKINH_ATTACHEX函数的地址
		SKINH_ATTACHEX pFunc = (SKINH_ATTACHEX)::GetProcAddress(::LoadLibrary("SkinH.dll"), "SkinH_AttachEx");
		if(pFunc)
		{
			pFunc(_T("skin\\dogmax.she"), NULL);
		}
		else
		{
			MessageBox(_T("皮肤库加载失败！"),_T("缺少SkinH.dll"));
		}
	InitItem();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVirusKillerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}
HCURSOR CVirusKillerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CVirusKillerDlg::InitItem()
{
	// 创建并初始化 图像列表 ，其中IDI_ICON1等等是已经在资源中创建的ICON的ID号
	m_ImageList.Create(32, 32, ILC_COLOR16 | ILC_MASK, 3, 2);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	//m_tab.SetItemSize(CSize(30,50));
	m_tab.SetExtendedStyle(m_tab.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_tab.SetImageList(&m_ImageList);
	//创建选项卡 
	m_tab.InsertItem(0, _T(" 病 毒 查 杀"),0);
	m_tab.InsertItem(1, _T(" 实时防护及功能大全"),1);
	m_tab.InsertItem(2, _T(" 产品升级及自定义查杀"),2);
	//创建页面     
	m_page1.Create(IDD_DIALOG1,&m_tab);   
	m_page2.Create(IDD_DIALOG2,&m_tab); 
	m_page3.Create(IDD_DIALOG_UPGRADE,&m_tab);
	/*
	后面rc和MoveWindow的操作时调整对话框的位置的操作，否则你会发现程序运
	行后看不到选项卡了（原因是默认的tab对话框的大小为主对话框显示区域的大
	小，因此把tab标签覆盖掉了，所以要先获取主对话框的大小，然后做下调整)。
	*/
	//显示范围     
	CRect rc;   
	m_tab.GetClientRect(&rc);   
	rc.top += 36;   
	rc.bottom -= 2;   
	rc.left += 5;   
	rc.right -= 5;   
	m_page1.MoveWindow(&rc);   
	//m_page2.MoveWindow(&rc);  
	//保存对话框对象指针   
	m_pDialog[0] =&m_page1;
	m_pDialog[1] =&m_page2;
	m_pDialog[2] =&m_page3;
	//显示   
	m_pDialog[0]->ShowWindow(SW_SHOW);   
	m_pDialog[1]->ShowWindow(SW_HIDE);
	m_pDialog[2]->ShowWindow(SW_HIDE); 
	//保存当前选项卡指针   
	m_nsltIndex = 0;     
return true;
}

void CVirusKillerDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CRect rc;   
	m_nsltIndex = m_tab.GetCurSel();
    switch(m_nsltIndex)
	{
    case 0:
		m_pDialog[0]->ShowWindow(true);
		m_pDialog[1]->ShowWindow(false);
		m_pDialog[2]->ShowWindow(false);
		m_tab.GetClientRect(&rc);   
		rc.top += 36;   
		rc.bottom -= 2;   
		rc.left += 5;   
		rc.right -= 5;   
		m_page1.MoveWindow(&rc);  
        break;
    case 1:
		m_pDialog[0]->ShowWindow(false);
		m_pDialog[1]->ShowWindow(true);
		m_pDialog[2]->ShowWindow(false);
		m_tab.GetClientRect(&rc);   
		rc.top += 36;   
		rc.bottom -= 2;   
		rc.left += 5;   
		rc.right -= 5;   
		m_page2.MoveWindow(&rc);  
        break;
    case 2:
		m_pDialog[0]->ShowWindow(false);
		m_pDialog[1]->ShowWindow(false);
		m_pDialog[2]->ShowWindow(true);
		m_tab.GetClientRect(&rc);   
		rc.top += 36;   
		rc.bottom -= 2;   
		rc.left += 5;   
		rc.right -= 5;   
		m_page3.MoveWindow(&rc);  
        break;
    default:
		break;
		;
	}  
	*pResult = 0;
}

BOOL CVirusKillerDlg::DestroyWindow() 
{
	// 销毁窗口前保存病毒库的设置
	GetCKillerDlg()->SaveUserSet();
	return CDialog::DestroyWindow();
}
