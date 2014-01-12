// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileMonitor.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog


CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDlg)
	ON_BN_CLICKED(IDC_BT_SURE, OnBtSure)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDlg message handlers
//单击确定按钮
void CSetDlg::OnBtSure() 
{
	//获得用户的选择结果
	CButton *pBt = (CButton*)GetDlgItem(IDC_CHECK_ADD);
	m_bAddNew = pBt->GetCheck();

	pBt = (CButton*)GetDlgItem(IDC_CHECK_DELETE);
	m_bDel = pBt->GetCheck();

	pBt = (CButton*)GetDlgItem(IDC_CHECK_ATTRIBUTE);
	m_bAttribute = pBt->GetCheck();

	pBt = (CButton*)GetDlgItem(IDC_CHECK_TIME);
	m_bOccurTime = pBt->GetCheck();

	pBt = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	m_bOther = pBt->GetCheck();

	pBt = (CButton*)GetDlgItem(IDC_CHECK_RENAME);
	m_bRename = pBt->GetCheck();
	UpdateData(TRUE);
	OnOK();
}

BOOL CSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//从配置文件中读取用户的设置 
	m_bAddNew = (::GetPrivateProfileInt("Selections","Add",0,".\\Config.ini")==0 ?false:true);
	m_bDel = (::GetPrivateProfileInt("Selections","Delete",0,".\\Config.ini")==0 ?false:true);
	m_bAttribute = (::GetPrivateProfileInt("Selections","Attribute",0,".\\Config.ini")==0 ?false:true);
	m_bOccurTime = (::GetPrivateProfileInt("Selections","OccurTime",0,".\\Config.ini")==0 ?false:true);
	m_bRename=(::GetPrivateProfileInt("Selections","Rename",0,".\\Config.ini")==0 ?false:true);
	m_bOther = (::GetPrivateProfileInt("Selections","Other",0,".\\Config.ini")==0 ?false:true);
	
	//显示用户的配置信息
	CButton *pBt = (CButton*)GetDlgItem(IDC_CHECK_ADD);
	pBt->SetCheck(m_bAddNew);

	pBt = (CButton*)GetDlgItem(IDC_CHECK_DELETE);
	pBt->SetCheck(m_bDel);
	
	pBt = (CButton*)GetDlgItem(IDC_CHECK_ATTRIBUTE);
	pBt->SetCheck(m_bAttribute);

	pBt = (CButton*)GetDlgItem(IDC_CHECK_TIME);
	pBt->SetCheck(m_bOccurTime);

	pBt = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	pBt->SetCheck(m_bOther);

	pBt = (CButton*)GetDlgItem(IDC_CHECK_RENAME);
	pBt->SetCheck(m_bRename);

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SET);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	UpdateData(FALSE);
	return TRUE;  
}

void CSetDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
