/*************************************************************
*	RegHook API										
*   注册表监控钩子
/*************************************************************/
#ifdef __cplusplus //dll工程自动定义
#define REG_HOOK_H extern "C" __declspec (dllexport)
#else
#define REG_HOOK_H  __declspec(dllimport)
#endif

#include <windows.h>
#include <Winternl.h>   

/**********************动作类型******************************/
#define		TYPE_RegCreateKeyExA	0 //创建新子键(窄字符A)
#define		TYPE_RegCreateKeyExW	1 //创建新子键(UNICODE W)
#define		TYPE_RegSetValueExA		2 //设置键值(A)
#define		TYPE_RegSetValueExW		3
#define		TYPE_RegDeleteKeyA		4 //删除子键
#define		TYPE_RegDeleteKeyW		5
#define		TYPE_RegDeleteValueA	6 //删除键值
#define		TYPE_RegDeleteValueW	7
#define		TYPE_RegQueryValueExA	8 //查询键值
#define		TYPE_RegQueryValueExW	9
/************************************************************/
#define      MAX_BUF_LEN            1024 //缓冲区最大长度
// 钩子向监视进程通信 传送的数据结构
typedef struct 
{
	DWORD	type;//键值数据类型
	DWORD	PID; //进程 ID
	HKEY	hKey;//子键句柄
	char	buf1[MAX_BUF_LEN ];//缓冲区1  如存取获取键值的名字 
	int		cbbuf1;//数据存储池1 长度
	char	buf2[MAX_BUF_LEN ];//装载指定值的缓冲区
	int		cbbuf2;//数据存储池2 长度
	long	result; //操作结果
}PARAMS;

//KEY_FULL_INFORMATION 结构 ZwQueryKey 函数需要此结构
typedef enum _KEY_INFORMATION_CLASS {  
    	KeyBasicInformation,  
		KeyNodeInformation,
		KeyFullInformation,
		KeyNameInformation,  
		KeyCachedInformation,  
		KeyVirtualizationInformation
}
KEY_INFORMATION_CLASS;

//枚举子项 查询信息的一个关键, 从 NTDLL 中运行时动态加载 (此函数为NT内核函数)
typedef LONG(WINAPI*ZwQueryKey)(IN HANDLE, IN KEY_INFORMATION_CLASS, OUT PVOID, IN ULONG, OUT PULONG); 

///////////////////////声明要导出的函数///////////////////////////////////////////////////
	/************************************************************************/
	/*	功能  :	安装钩子函数，把自定义钩子安装到系统						*/
	/*	返回值:	成功返回true, 失败返回 false								*/
	/************************************************************************/
REG_HOOK_H	bool InstallHook(
							 HWND hWnd //安装钩子函数的窗口句柄
							 );

	/************************************************************************/
	/*	功能  :	卸载钩子函数，把系统中的钩子卸载出去						*/
	/*	返回值:	成功返回true, 失败返回 false								*/
	/************************************************************************/
REG_HOOK_H	bool UninstallHook();

	/************************************************************************/
	/*	功能  :	过滤函数，对指定的进程进行挂钩监视							*/
	/*	返回值:	无															*/
	/************************************************************************/
REG_HOOK_H	void SetFilter(
						   DWORD pid//进程ID号
						   );

	/*************************************************************************/
	/*  功能 :钩子监视的内容,对指定的行为进行挂钩监视(相应的参数为true表示是)*/		
	/*	返回值:	无															 */
	/*************************************************************************/
REG_HOOK_H	void SetHookFunction(
								 BOOL RegCreateKeyEx, //是否监视创建子键
								 BOOL RegSetValueEx, //是否监视键值设置
								 BOOL RegDeleteKey, //是否监视删除子键
								 BOOL RegDeleteValue, //是否监视删除键值
								 BOOL RegQueryValueEx //是否监视查询键值
			);

	/*************************************************************************/
	/*  功能 :设置资源管理器进程 ID号										*/		
	/*	返回值:	无															 */
	/*************************************************************************/
REG_HOOK_H	void SetExplorerPID(
								DWORD pid //进程ID 号
								);
