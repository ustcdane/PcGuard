// KillerDlg.cpp : implementation file
/////////////////////////////////////////////////////////////////////////
// 病毒查杀对话框 运行流程是：选择扫描目标——开始扫描硬盘——按照特征码识
// 别出病毒文件——查找病毒进程——结束病毒进程——列出病毒文件——用户选择
// 是否删除可疑文件启动设置——其余清理。
/////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VirusKiller.h"
#include "KillerDlg.h"
#include <tlhelp32.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
/////////////////////////////////////////////////////////////////////////
//    PathFindExtension();函数的使用
//    作用：取得文件的后缀名
//   注意：头文件 shlwapi.h   包含库：shlwapi.lib  
/////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 临界区 全局变量
	CRITICAL_SECTION g_FileCs; //文件扫描队列
	CRITICAL_SECTION g_VirusCs;// 可以文件队列

/////////////////////////////////////////////////////////////////////////////
// CKillerDlg dialog


CKillerDlg::CKillerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKillerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKillerDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_bStartScan=false;
	m_bStop=false;
	//}}AFX_DATA_INIT
	m_hMainrds=INVALID_HANDLE_VALUE;
	for(int i=0;i<MAX_THREAD_NUM;i++)//初始化线程数组句柄
		m_hThread[i]= INVALID_HANDLE_VALUE;
}
// 初始化为NULL
CKillerDlg * CKillerDlg::m_pCKillerDlg=NULL;
//析构
CKillerDlg::~CKillerDlg()
{
	for(int i=0;i<MAX_THREAD_NUM;i++)//关闭句柄
	{
		if(m_hThread[i] !=INVALID_HANDLE_VALUE)
			CloseHandle(m_hThread[i]);
	}
}

void CKillerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKillerDlg)
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPathName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKillerDlg, CDialog)
	//{{AFX_MSG_MAP(CKillerDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList)
	ON_COMMAND(ID_MENUE_OPEN, OnMenueOpen)
	ON_COMMAND(ID_MENUE_DEL, OnMenueDel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillerDlg message handlers

void CKillerDlg::OnBtnBrowse() 
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
	GetDlgItem(IDC_EDIT_SCAN)->SetWindowText("您选择的监控目录为：" + m_strPathName);
	UpdateData(FALSE);
}

void CKillerDlg::OnButtonStart() 
{
	m_nTotalFileNum =0;  // 总文件数
	m_VirusNum=0;    // 发现的病毒数
	m_KillVirusNum=0; // 清除病毒数
	m_errNum=0;   // 出现错误数	
	m_FileList.clear();// 清空要扫描的文件列表
	m_VirusList.clear();// 可疑文件列表
	m_listCtrl.DeleteAllItems();// 清空列表
	GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText("总文件数：0");
	GetDlgItem(IDC_STATIC_VIR_NUM)->SetWindowText("病毒数：0");
	GetDlgItem(IDC_STATIC_KILLED_NUM)->SetWindowText("清除数：0");  
	GetDlgItem(IDC_STATIC_ERR)->SetWindowText("错误数：0");
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_START )->EnableWindow(FALSE);               
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	m_bStop=false;
	if(!m_bStartScan)
	{
		//准备开始扫描:  扫描路径  
		OnStartKillVirus(m_strPathName);
	}
}

//创建一个主线程，用于扫描一指定目录
void CKillerDlg::OnStartKillVirus(CString strDir)
{
	    m_bStartScan = true;
		DWORD ThreadID =0;
		 m_hMainrds=::CreateThread(NULL,0,MainThreadProc,LPVOID(this),0,(LPDWORD)&ThreadID);
		::CloseHandle(m_hMainrds);
}  
//////////////////////////////////////////////////////////////
//创建各线程开始进行查找
DWORD WINAPI  MainThreadProc(LPVOID lpParam)
{	
	CKillerDlg * pDlg=(CKillerDlg *)lpParam;
	CString str;
	//对扫描目录进行处理(为空时指全部目录)  注意此处要把磁盘符最右边的\删除/*不然 就变成 c:\\*/
	pDlg->ScanFile((LPSTR)(LPCTSTR)pDlg->m_strPathName.Left(pDlg->m_strPathName.GetLength()-1),"*.*");
	str.Format("%d",pDlg->m_FileList.size());
	pDlg->GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText("总文件数:"+str);
	DWORD ThreadID;
	//创建指定多个比对线程
	for(int i = 0; i <MAX_THREAD_NUM; i++)
	{
		pDlg->m_hThread[i] = CreateThread(NULL,0,VirusCompareProc,lpParam,0, &ThreadID);
		
	}
	if(true==pDlg->m_bStop)
	{
		return 0; // 如果停止监视则返回
	}
	// 等待对比线程结束
	::WaitForMultipleObjects(MAX_THREAD_NUM,pDlg->m_hThread, TRUE, INFINITE);
	for(i=0; i< MAX_THREAD_NUM; i++)
		::CloseHandle(pDlg->m_hThread[i]);
	pDlg->ScanDisk();
	int j=0;
	CString strTemp;
	//遍历整个链表
	for (FileNameList::iterator it = pDlg->m_VirusList.begin(); it != pDlg->m_VirusList.end(); ++it)
	{
		strTemp.Format("%d",++j);
		pDlg->m_listCtrl.InsertItem(0, strTemp);
		pDlg->m_listCtrl.SetItemText(0,1 ,(CString)(*it) );
	}
	pDlg->m_bStartScan=false;
	pDlg->GetDlgItem(IDC_STATIC_VIR_NUM)->SetWindowText("可疑文件数:"+strTemp);
    pDlg->GetDlgItem(IDC_EDIT_SCAN)->SetWindowText("\t内存中无毒..\n 自启动文件成功清除！");
	pDlg->GetDlgItem(IDC_EDIT_SAFE)->SetWindowText("\t\t查杀完毕....\n可疑文件");
	pDlg->GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
	pDlg->GetDlgItem(IDC_BUTTON_START )->EnableWindow(TRUE);               
	return 1;
}
int CKillerDlg::ScanFile(char *FileDir,char *FileType)
{   // 扫描文件
    CString MyExt;
    char buffer[2*MAX_PATH]={'\0'};
    sprintf(buffer,"%s\\%s",FileDir,FileType);
	
    HANDLE hFind;
    WIN32_FIND_DATA dat;
    hFind = ::FindFirstFile(buffer,&dat);
    if( hFind == INVALID_HANDLE_VALUE )
    {
        GetDlgItem(IDC_EDIT_SCAN)->SetWindowText(" 扫描过程中出错！");
        return -1;
    }
    else
    {
        char NewBuf[2*MAX_PATH];
        do
        {
			
			if(dat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)// 判断是否为文件夹
			{
				if(dat.cFileName[0] == '.')
					continue;
				else
				{
					sprintf(NewBuf,"%s\\%s",FileDir,dat.cFileName);
					// 递归
					ScanFile(NewBuf,"*.*");
				}
			}
			else// 是文件
			{
				sprintf(buffer,"%s\\%s",FileDir,dat.cFileName);
				MyExt = PathFindExtension(dat.cFileName);
				if( MyExt == ".exe" || MyExt == ".com" || MyExt == ".pif" || MyExt == ".scr" || MyExt == ".html" ||	MyExt 
					== ".asp" || MyExt == ".php" || MyExt == ".jsp" || MyExt==".dll" || MyExt==".txt" )
				{
					 m_FileList.push_back((CString)buffer);
					 GetDlgItem(IDC_EDIT_SCAN)->SetWindowText(((CString)buffer)+"加入扫描队列");
				}
			}
		}while(::FindNextFile(hFind,&dat));
		CString str;
		str.Format("%d",m_FileList.size());
		GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText("总文件数:"+str);
	}
	return 1;
}

//病毒码比对工作线程(最多可以分配 USE_MAXTRD_NUM个线程)
DWORD WINAPI VirusCompareProc(LPVOID lpParam)
{
	CKillerDlg * pDlg=(CKillerDlg *)lpParam;
	CString curFilePath;
    while(!(pDlg->m_bStop))
	{
		if(pDlg->m_FileList.empty())
		{
			pDlg->m_bStop=true;
			break;
		}
		//进入临界区
		::EnterCriticalSection(&g_FileCs);
		curFilePath=pDlg->m_FileList.front();// 表头出队列
		pDlg->GetDlgItem(IDC_EDIT_SCAN)->SetWindowText((CString)("正在扫描"+curFilePath));
		pDlg->m_FileList.pop_front();
				//离开临界区，供比对线程从队列中取出文件名(从队列头取出)
		::LeaveCriticalSection(&g_FileCs);
		if(curFilePath.IsEmpty())
		{
			continue;
		}
		else
		{
			pDlg->IsVirus(curFilePath);
			curFilePath="";
		}
    }

	return 1;
}
bool CKillerDlg::InitVirusData()
{
	LoadUserSet();
	// 病毒库初始化失败
	if(m_VirusLibLIst.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CKillerDlg::IsVirus(CString strFilePath)
{
	CString MyExt = PathFindExtension(strFilePath);
	// 打开检查的文件
	BOOL bValid = FALSE;
	DWORD dwRead;
	if(MyExt==".exe" || MyExt==".dll") // 是 PE 文件 进行检测
	{
		HANDLE hFile = ::CreateFile(strFilePath, GENERIC_READ, 
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			GetDlgItem(IDC_EDIT_SAFE)->SetWindowText( _T("无效文件!"));
			return false;
		}
		// 定义PE文件中的DOS头
		IMAGE_DOS_HEADER dosHeader;	
		// 读取DOS头
		::ReadFile(hFile, &dosHeader, sizeof(dosHeader), &dwRead, NULL);
		if(dosHeader.e_magic == IMAGE_DOS_SIGNATURE) // 是不是有效的DOS头？
		{
			char Buffer[VIRUSDATA_LEN+1];
			// 判断是否被感染，因为在一般情况下这个位置(0x7f)都应该不变，为0x00
			::SetFilePointer(hFile,0x7f, NULL,FILE_BEGIN);
			::ReadFile(hFile,Buffer,sizeof(Buffer),&dwRead,NULL);
			if(Buffer[0]!=0x00)
			{
				GetDlgItem(IDC_EDIT_SAFE)->SetWindowText(strFilePath+"此文件已被感染...");
			}
			else
			{
				//遍历整个链表
				for (VirusList::iterator it = m_VirusLibLIst.begin(); it != m_VirusLibLIst.end(); ++it)
				{
					VIRUS_DATA da=(*it);
					::SetFilePointer(hFile,da.offset, NULL,FILE_BEGIN);
					::ReadFile(hFile,Buffer,da.dataLen,&dwRead,NULL);
					// 从文件的路径中得到 文件名
					CString FileName=strFilePath.Right(strFilePath.GetLength()-strFilePath.ReverseFind('\\')-1);
					// 当特征码匹配 或者 病毒名字相同时 认为是可疑文件
					if( !strcmp(Buffer,(char *)da.VirusData ) || FileName==(CString)(*it).VirusName )
					{
						// 加入可疑文件列表
						::EnterCriticalSection(&g_VirusCs);
						m_VirusList.push_back(strFilePath);
						GetDlgItem(IDC_EDIT_SAFE)->SetWindowText("\t\t\t此文件已构成威胁！开始内存扫描...");
						::LeaveCriticalSection(&g_VirusCs);
						ScanProcess(strFilePath);// 扫描进程
					}
					else
					{
						GetDlgItem(IDC_EDIT_SAFE)->SetWindowText("\t\t\t此文件安全！");
					}
					
				}//for
				
				
			}//else
			
		}//if dos
		::CloseHandle(hFile);
	}// if
	else if(MyExt==".txt")
	{
		CString   strSum=_T("");
		CString   strLine;
		CStdioFile   fpTxtFile; 
		
		//只读方式打开文件 
		if(   !fpTxtFile.Open( strFilePath, CFile::modeRead)) 
		{ 
			return   FALSE; 
		} 
		//循环读取行 
		while(fpTxtFile.ReadString(strLine)) 
		{ 
			strSum+=strLine;
		} 
		fpTxtFile.Close();
		//遍历整个病毒库链表
		for (VirusList::iterator it = m_VirusLibLIst.begin(); it != m_VirusLibLIst.end(); ++it)
		{
			VIRUS_DATA da=(*it);
			// 当特征码匹配 认为是可疑文件
			if(-1!=strSum.Find(CString(da.VirusData)))
			{
				// 加入可疑文件列表
				::EnterCriticalSection(&g_VirusCs);
				m_VirusList.push_back(strFilePath);
				GetDlgItem(IDC_EDIT_SAFE)->SetWindowText("\t\t\t此文件已构成威胁！");
				::LeaveCriticalSection(&g_VirusCs);
			}
			else
			{
				GetDlgItem(IDC_EDIT_SAFE)->SetWindowText("\t\t\t此文件安全！");
			}
		}
	}

	return true;
}

void CKillerDlg::SaveUserSet()
{
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//保存Ini文件名
	filePath.Format("%s",appPath);
	filePath +="\\";
	filePath += "virus.ini";	
	
	if(m_VirusLibLIst.empty())// 病毒库列表为空时
		return;

	//删除原来的文件
	DeleteFile(filePath);
	int		nIndex = 0;
	
	//遍历整个链表
	for (VirusList::iterator it = m_VirusLibLIst.begin(); it != m_VirusLibLIst.end(); ++it)
	{	
		VIRUS_DATA da=(*it);
		CString strSection("section");							
		
		//section
		CString strIndex;
		strIndex.Format("%d",nIndex);
		strSection += strIndex;
		// 特征码长度
		CString strdataLen;								
		strdataLen.Format("%ld", (long)da.dataLen);
		// 特征码内容
		CString strOffset;
		strOffset.Format("%d",da.offset);
		CString strdata;
		strdata=(char *)(LPCTSTR)da.VirusData;
			//写病毒名称48字节
		WritePrivateProfileString(strSection, _T("VirusName"), da.VirusName, filePath);	
			//特征码长度
		WritePrivateProfileString(strSection, _T("DataLen"), strdataLen, filePath);
		//文件类型
		WritePrivateProfileString(strSection, _T("ExtName"), da.ExtName,filePath);	
		// 偏移地址
		WritePrivateProfileString(strSection, _T("OffSet"),strOffset ,filePath);
		//特征码内容
		WritePrivateProfileString(strSection, _T("VirusData"),strdata,filePath);	
		++nIndex;
	}

	//将病毒数量写入ini文件
	CString strCount;
	strCount.Format("%d", nIndex);
	::WritePrivateProfileString("VirusCount", "Count", strCount, filePath);	
	
	
	m_VirusLibLIst.clear();//删除链表所有节点
}

void CKillerDlg::LoadUserSet()
{
	#define  TEMP_BUF_SIZE 125//缓冲区长度
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//保存Ini文件名
	filePath.Format("%s",appPath);
	filePath += "\\";
	filePath += "virus.ini";
	
	//从ini文件中获得病毒特征数量
 	int count =::GetPrivateProfileInt("VirusCount", "Count", 0, filePath);
	if(count<=0)
		return;
	int nIndex = 0;
	while (TRUE)
	{
		CString strSection("section");
		CString strIndex;
		strIndex.Format("%d",nIndex);
		strSection += strIndex;
		
		//病毒名称
		CString strSectionKey = "VirusName";					
		CString strValue = _T("");
		char cBuf[TEMP_BUF_SIZE];
		memset(cBuf, 0, TEMP_BUF_SIZE);
		VIRUS_DATA *pVirusInfo = new VIRUS_DATA;//新建VIRUS_DATA对象
		memset(pVirusInfo,0,sizeof(VIRUS_DATA));
		if(GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			strcpy(pVirusInfo->VirusName,cBuf);
		}
		//  特征码长度
		strSectionKey = "DataLen";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			pVirusInfo->dataLen = (LONG)atoi(cBuf);
		}
		
		//文件扩展名
		strSectionKey = "ExtName";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			strcpy(pVirusInfo->ExtName,cBuf);
		}
		
		//偏移量
		strSectionKey = "OffSet";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			pVirusInfo->offset = (LONG)atoi(cBuf);
		}

		//读取特征码
		strSectionKey = "VirusData";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(GetPrivateProfileString (strSection, strSectionKey, NULL,  cBuf,TEMP_BUF_SIZE, filePath))
		{
			strcpy(pVirusInfo->VirusData,cBuf);
		}
		m_VirusLibLIst.push_back(*pVirusInfo);		//加入链表
		delete pVirusInfo;
		if(++nIndex>=count) // 判断病毒库加载完毕
			break;
	}	
}

CKillerDlg * CKillerDlg::GetCKillerDlg()
{
	if(NULL==m_pCKillerDlg)
	{
		return NULL;
	}
	else
	{
		return m_pCKillerDlg;
	}
}


CKillerDlg * GetCKillerDlg()
{
	return CKillerDlg::GetCKillerDlg();
}


BOOL CKillerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pCKillerDlg=this;
	    		//初始化临界区
	::InitializeCriticalSection(&g_FileCs);
	::InitializeCriticalSection(&g_VirusCs);
	//列表初始化列表
	m_listCtrl.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);
	m_listCtrl.SetBkColor(RGB(247,247,255));
	m_listCtrl.SetTextColor(RGB(0,100,155));
	m_listCtrl.SetTextBkColor(RGB(247,247,255));

	m_listCtrl.InsertColumn(0,_T("序号"),LVCFMT_LEFT,60);
	m_listCtrl.InsertColumn(1,_T("文件路径"),LVCFMT_LEFT,420);
	if(!InitVirusData())
	{
		AfxMessageBox(_T("病毒库文件加载失败，无法进行病毒查杀工作!"));
		return 0;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKillerDlg::ScanProcess(CString ModuleName)
{
	// 扫描进程及模块信息
    //StaBar->SimpleText = " 正在扫描进程...";

	PROCESSENTRY32 ProcessEntry = { 0 };
    MODULEENTRY32  ModuleEntry  = { 0 };
	HANDLE hProcessSnap;
    HANDLE hModuleSnap;
    HANDLE hVirus;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    ModuleEntry.dwSize  = sizeof(MODULEENTRY32);
	
    // 遍历进程 创建快照
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	BOOL bRet = Process32First(hProcessSnap,&ProcessEntry);
	while(bRet)
	{
        // 遍历进程模块
		CString st=_T(" 正在扫描进程");
       GetDlgItem(IDC_EDIT_SAFE)->SetWindowText(st + CString(ProcessEntry.szExeFile));
        hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,ProcessEntry.th32ProcessID);
        bRet = Module32First(hModuleSnap,&ModuleEntry);
        while(bRet)
        {
            if(strstr(ModuleEntry.szExePath,ModuleName.Right(ModuleName.GetLength()-ModuleName.ReverseFind('\\')-1) ) )
            {
				ModuleName.ReleaseBuffer();
                hVirus = OpenProcess(PROCESS_ALL_ACCESS,FALSE,ProcessEntry.th32ProcessID);
                TerminateProcess(hVirus,0);
                CloseHandle(hVirus);
				st=_T(" 结束可疑进程");
                GetDlgItem(IDC_EDIT_SAFE)->SetWindowText(st+ModuleEntry.szExePath );
            }
            bRet = Module32Next(hModuleSnap,&ModuleEntry);
        }
		bRet = Process32Next(hProcessSnap,&ProcessEntry);
	}
    return;
}

// 扫描所有磁盘 目录
void CKillerDlg::ScanDisk()
{
	char Disk = NULL;
    UINT Type = 0;
    char strFileName[MAX_PATH] = {0};
    char szDriveName[4] = {0};
	wsprintf(szDriveName,"C:\0");

	// 遍历所有可能存在的分区
	for(szDriveName[0] = 'C';szDriveName[0] < 'Z';szDriveName[0]++)
	{	// 得到该分区类型
        Type = GetDriveType(szDriveName);
        // 类型为1表明该分区根目录不存在
	    if(Type != 1)
		{
            Disk = szDriveName[0];

            sprintf(strFileName,"%c:\\autorun.inf",Disk);
            SetFileAttributes(strFileName,~FILE_ATTRIBUTE_HIDDEN);
            if(DeleteFile(strFileName))
			{
				GetDlgItem(IDC_EDIT_SAFE)->SetWindowText((CString)strFileName+"成功删除..");
			}
            sprintf(strFileName,"%c:\\setup.exe",Disk);
            SetFileAttributes(strFileName,~FILE_ATTRIBUTE_HIDDEN);
            if(DeleteFile(strFileName))
			{
				GetDlgItem(IDC_EDIT_SAFE)->SetWindowText((CString)strFileName+"成功删除..");
			}
        }
    }
    return;
}
// 处理停止按钮消息

void CKillerDlg::OnButtonStop() //Terminate
{
	m_bStop=true; //设置停止为真
	m_bStartScan=false;// 开始扫描为假
	for(int i=0;i<MAX_THREAD_NUM;i++)
	{
		if(INVALID_HANDLE_VALUE!=m_hThread[i])// 判断是否为有效句柄
		{
			TerminateThread(m_hThread[i],0);// 结束病毒库对比线程
			CloseHandle(m_hThread[i]);
			m_hThread[i]=INVALID_HANDLE_VALUE;
		}
	}
	if(INVALID_HANDLE_VALUE!=m_hMainrds)
	{
		TerminateThread(m_hMainrds,0);// 结束主线程
		CloseHandle(m_hMainrds);
		m_hMainrds=INVALID_HANDLE_VALUE;
	}
	m_FileList.clear();// 清空要扫描的文件列表
	m_VirusList.clear();// 可疑文件列表
	GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText("总文件数：0");
	GetDlgItem(IDC_STATIC_VIR_NUM)->SetWindowText("可疑文件数:0");
    GetDlgItem(IDC_EDIT_SCAN)->SetWindowText("\t停止扫描");
	GetDlgItem(IDC_EDIT_SAFE)->SetWindowText("\t\t.....");
	GetDlgItem(IDC_BUTTON_START )->EnableWindow(TRUE);               
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

// 处理列表框右键消息 弹出浮动菜单
void CKillerDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem != -1)
	{
	  DWORD dwPos = GetMessagePos();// 得到鼠标坐标
      CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
      CMenu menu;
      VERIFY( menu.LoadMenu(IDR_MENU1));
      CMenu* popup = menu.GetSubMenu(0);
      ASSERT( popup != NULL );
	  //确定弹出式菜单的位置
      popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	  //资源回收
		HMENU hmenu=menu.Detach();
		menu.DestroyMenu();
   }
	*pResult = 0;
	*pResult = 0;
}

void CKillerDlg::OnMenueOpen() 
{
	// TODO: Add your command handler code here
	CString tem = _T("");
	CString str = _T("Explorer /select,");
	int nItem = m_listCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	tem = m_listCtrl.GetItemText(nItem, 1);
	SetCurrentDirectory(tem);
	str +=tem;
	WinExec(str,SW_SHOWNORMAL);//显示文件的位置
}

void CKillerDlg::OnMenueDel() 
{
	// TODO: Add your command handler code here
	CString tem = _T("");
	int nItem = m_listCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	tem = m_listCtrl.GetItemText(nItem, 1);
	DeleteFile(tem);
}

void CKillerDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_listCtrl.DeleteAllItems(); // 清空可疑文件列表
}
