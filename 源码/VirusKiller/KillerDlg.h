#if !defined(AFX_KILLERDLG_H__2726E8E3_378F_4CA6_BAC8_89CCD811F933__INCLUDED_)
#define AFX_KILLERDLG_H__2726E8E3_378F_4CA6_BAC8_89CCD811F933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KillerDlg.h : header file
//
#include <list> //STL头文件
using namespace std;

typedef list<CString>  FileNameList;// 文件链表结构定义

#define MAX_THREAD_NUM 2 // 最大线程数
#define VIRUSDATA_LEN 124 // 
//病毒数据结构
typedef struct VIRUS_DATA   
	{
		char VirusName[49];    // 病毒名称48字节
       LONG dataLen;        // 特征码长度
	   char ExtName[21];   // 指明在此类型的文件扩展名中查找(为空时表示所有文件)60字节
	   LONG offset; // 特征码所在偏移量
	   char  VirusData[VIRUSDATA_LEN + 1];   // 病毒特征码
	   // 重载赋值运算符
	   const VIRUS_DATA & operator=( const VIRUS_DATA temp )
	   {
		   dataLen = temp.dataLen;
		   ZeroMemory(VirusName,49);
		   ZeroMemory(ExtName,21);
		   strcpy(VirusName,temp.VirusName);
		   VirusName[48] = '\0';
		   strcpy(ExtName,temp.ExtName); 
		   ExtName[20] = '\0';
		   offset=temp.offset;
		   ZeroMemory(VirusData,VIRUSDATA_LEN+1);
		   memcpy(VirusData,temp.VirusData,VIRUSDATA_LEN); 
		   return *this;
	   }
	}VIRUS_DATA ,*PVIRUS_DATA ;
typedef list<VIRUS_DATA>  VirusList;//文件链表结构定义
/////////////////////////////////////////////////////////////////////////////
// CKillerDlg dialog

class CKillerDlg : public CDialog
{
// Construction
public:
	CString m_strPathName;
	CKillerDlg(CWnd* pParent = NULL);   // standard constructor
	~CKillerDlg();
// Dialog Data
	//{{AFX_DATA(CKillerDlg)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_listCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKillerDlg)
	afx_msg void OnBtnBrowse();
	afx_msg void OnButtonStart();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStop();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenueOpen();
	afx_msg void OnMenueDel();
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool m_bStartScan;
	int	m_nTotalFileNum;  // 总文件数
	int m_VirusNum;    // 发现的病毒数
	int m_KillVirusNum; // 清除病毒数
	int m_errNum;   // 出现错误数
	FileNameList m_FileList;// 要扫描的文件列表
	FileNameList m_VirusList;// 可疑文件列表
	VirusList   m_VirusLibLIst;// 病毒库列表
public:
	void ScanDisk();
	void ScanProcess(CString ModuleName);
	// 获得本自身指针
	static CKillerDlg * m_pCKillerDlg;
	static CKillerDlg * GetCKillerDlg();
	void LoadUserSet();
	void SaveUserSet();
	bool IsVirus(CString strFilePath);
	HANDLE m_hMainrds;
	HANDLE m_hThread[MAX_THREAD_NUM];
	bool InitVirusData();
	bool m_bStop; // 是否停止
	void OnStartKillVirus(CString strDir=_T(""));
	int ScanFile(char *FileDir,char *FileType);
};

// 如果线程要执行的是类的成员函数,则必须用static声明为静态函数
// 一般都是申明成全局函数  
DWORD WINAPI MainThreadProc(LPVOID lpParam);
DWORD WINAPI VirusCompareProc(LPVOID lpParam);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLERDLG_H__2726E8E3_378F_4CA6_BAC8_89CCD811F933__INCLUDED_)
