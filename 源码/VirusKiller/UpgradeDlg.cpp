// UpgradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirusKiller.h"
#include "KillerDlg.h"
#include "UpgradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog


CUpgradeDlg::CUpgradeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpgradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpgradeDlg)
	m_OffSet = 0;
	//}}AFX_DATA_INIT
}


void CUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpgradeDlg)
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_OffSet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpgradeDlg, CDialog)
	//{{AFX_MSG_MAP(CUpgradeDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_STAT_UP, OnStatUp)
	ON_BN_CLICKED(IDC_BUTTON_MAIL, OnButtonMail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg message handlers

// 单击确定 按钮
void CUpgradeDlg::OnButtonOk() 
{
	UpdateData(true);
	// 加入病毒对比链表
	m_pCKillerDlg=GetCKillerDlg();
	VIRUS_DATA *pVirusInfo = new VIRUS_DATA;
	memset(pVirusInfo,0,sizeof(VIRUS_DATA));
	CString temp=_T("");
	GetDlgItemText(IDC_EDIT_VIRUS_NAME,temp);
	strcpy(pVirusInfo->VirusName,(LPSTR)(LPCTSTR)temp);
    GetDlgItemText(IDC_EDIT_LEN,temp);
	pVirusInfo->dataLen=atoi((LPSTR)(LPCTSTR)temp);
	GetDlgItemText(IDC_EDIT_EXTNAME,temp);
	strcpy(pVirusInfo->ExtName,(LPSTR)(LPCTSTR)temp);
	pVirusInfo->offset=m_OffSet;
	GetDlgItemText(IDC_EDIT_VIRUS_DATA,temp);
	memcpy(pVirusInfo->VirusData,(BYTE*)(LPSTR)(LPCTSTR)temp,VIRUSDATA_LEN);
	m_pCKillerDlg->m_VirusLibLIst.push_back(*pVirusInfo);
}
// 下载更新 
void CUpgradeDlg::OnStatUp() 
{
	ShellExecute(NULL,"open","http://blog.csdn.net/gfsfg8545",NULL,NULL, SW_SHOWNORMAL );
}
// 发送邮件 需要 outLook
void CUpgradeDlg::OnButtonMail() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open","mailto:wangdanit@126.com",NULL,NULL, SW_SHOWNORMAL );
}
