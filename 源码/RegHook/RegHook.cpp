
#include "RegHook.h"
#include "detours.h"
#pragma comment(lib, "advapi32.lib")//release版本 需要包含 （有关注册表API）
#pragma comment(lib, "User32.lib")//release 需要包含 （有关消息等API）
#pragma comment(lib, "detours.lib")//detous 静态库

#pragma comment(linker, "/OPT:nowin98")
/**********************************************************************************************/
//DLL共享数据段
#pragma data_seg(".shareddata")
HWND	g_hWnd = NULL;//监视窗口句柄
HHOOK	g_hHook= NULL;//Hook 句柄
DWORD	PID = 0; // 要监视的进程 pid号
DWORD	ExplorerPID = 0;//资源管理器PID
DWORD	RegMonPID = 0;//监视进程PID
//是否监视的行为
BOOL	HOOK_RegCreateKeyEx = TRUE;
BOOL	HOOK_RegSetValueEx = TRUE;
BOOL	HOOK_RegDeleteKey = TRUE;
BOOL	HOOK_RegDeleteValue = TRUE;
BOOL	HOOK_RegQueryValueEx = TRUE;
#pragma data_seg()
#pragma comment(linker,"/section:.shareddata,rws")

bool	g_bIntercepted	= false;
bool	g_bTryed = false;
//钩子函数
LRESULT CALLBACK ShellProc( int nCode, WPARAM wParam, LPARAM lParam );
void Intercept();//安装拦截
void UnIntercept();//卸载拦截

/************************************************************原理******************************************************************/
// 拦截WIN32 API的原理 
// Detours定义了三个概念：
//(1) Target函数：要拦截的函数，通常为Windows的API。
//(2) Trampoline函数：Target函数的部分复制品。因为Detours将会改写Target函数，所以先把Target函数的前5个字节复制保存好，一方面仍然保存
//    Target函数的过程调用语义，另一方面便于以后的恢复。
//(3) Detour 函数：用来替代Target函数的函数。 
// Detours在Target函数的开头加入JMP Address_of_ Detour_ Function指令（共5个字节）把对Target函数 的调用引导到自己的Detour函数， 把
// Target函数的开头的5个字节加上JMP Address_of_ Target _ Function+ 5共10个字节作为Trampoline函数。
/**********************************************************************************************************************************/


/************************************************************申明******************************************************************/
/*DETOUR_TRAMPOLINE(trampoline_prototype, target_name);
功能：该宏把名为target_name 的Target函数生成Trampoline函数，以后调用 trampoline_prototype在语义上等于调用Target函数*/

DETOUR_TRAMPOLINE(LONG WINAPI Real_RegCreateKeyExA(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY,
				  LPDWORD), RegCreateKeyExA);

DETOUR_TRAMPOLINE(LONG WINAPI Real_RegCreateKeyExW(HKEY, LPCWSTR, DWORD, LPWSTR, DWORD,REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, 
				  LPDWORD), RegCreateKeyExW);


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegSetValueExA(HKEY, LPCSTR, DWORD, DWORD, CONST BYTE*, DWORD), RegSetValueExA);


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegSetValueExW(HKEY, LPCWSTR, DWORD, DWORD, CONST BYTE*, DWORD), RegSetValueExW);


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteKeyA(HKEY, LPCSTR), RegDeleteKeyA);

DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteKeyW(HKEY, LPCWSTR), RegDeleteKeyW);


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteValueA(HKEY, LPCSTR), RegDeleteValueA);


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteValueW(HKEY, LPCWSTR), RegDeleteValueW);


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegQueryValueExA(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD), RegQueryValueExA);

DETOUR_TRAMPOLINE(LONG APIENTRY Real_RegQueryValueExW(HKEY, LPCWSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD), RegQueryValueExW);

/**********************************************///替代函数申明///**************************************************************/

// Replace_ 替代真实的 API  Real_ 即Trampoline函数  

LONG WINAPI Replace_RegCreateKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD Reserved,LPSTR lpClass,DWORD dwOptions,REGSAM samDesired,
									LPSECURITY_ATTRIBUTES lpSecurityAttributes,PHKEY phkResult,LPDWORD lpdwDisposition);

LONG WINAPI Replace_RegCreateKeyExW(HKEY hKey,LPCWSTR lpSubKey,DWORD Reserved,LPWSTR lpClass,DWORD dwOptions,REGSAM samDesired,
									LPSECURITY_ATTRIBUTES lpSecurityAttributes,PHKEY phkResult,LPDWORD lpdwDisposition);

LONG WINAPI Replace_RegSetValueExA(HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE* lpData,DWORD cbData);				

LONG WINAPI Replace_RegSetValueExW(HKEY hKey,LPCWSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE* lpData,DWORD cbData);

LONG WINAPI Replace_RegDeleteKeyA(HKEY hKey,LPCSTR lpSubKey);

LONG WINAPI Replace_RegDeleteKeyW(HKEY hKey,LPCWSTR lpSubKey );

LONG WINAPI Replace_RegDeleteValueA(HKEY hKey,LPCSTR lpValueName);

LONG WINAPI Replace_RegDeleteValueW(HKEY hKey,LPCWSTR lpValueName);

LONG WINAPI Replace_RegQueryValueExA(HKEY hKey, LPCSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType, LPBYTE lpData,LPDWORD lpcbData);

LONG APIENTRY Replace_RegQueryValueExW(HKEY hKey,LPCWSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData);

/**************************************************************************************************************************************/



BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH: // 卸载操作
		{		
			if (g_bIntercepted )
			{
				g_bIntercepted = false;
				UnIntercept(); // 停止拦截
			}
		}
		break;
    }
    return TRUE;
}

/****************************************************************************************************************/

LONG WINAPI Replace_RegCreateKeyExA(HKEY hKey,LPCSTR lpSubKey,DWORD Reserved,LPSTR lpClass,DWORD dwOptions,REGSAM samDesired,
									LPSECURITY_ATTRIBUTES lpSecurityAttributes,PHKEY phkResult,LPDWORD lpdwDisposition)
{
	LONG nRet;
	__try // 捕获异常
	{
		// Real_RegCreateKeyExA即替换 RegCreateKeyExA 的函数
		nRet = Real_RegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,
			lpSecurityAttributes, phkResult, lpdwDisposition);

		if(!HOOK_RegCreateKeyEx)// 判断是否监视
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)//判断是否为资源管理器进程ID  
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)//是否为监视进程ID
			return nRet;
		// PID = 0 即监视所有进程  || 判断是否为要监视的进程
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) ) 
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data; // 申明COPYDATASTRUCT结构 用于进程间发送数据
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();// 得到当前进程 ID
			params.hKey = hKey;
			memcpy(params.buf1, lpSubKey, strlen(lpSubKey)); // 存储子键缓冲信息
			params.result = nRet; // 取得操作返回值
			
			data.cbData = sizeof(PARAMS); // 获得参数大小
			data.lpData =(void *)&params; // 赋值缓冲区指针
			data.dwData = TYPE_RegCreateKeyExA; // 定义动作类型
			/*这里涉及到进程间通信，通常有三种方法：
			 1，使用内存映射文件；
			 2，通过共享内存DLL共享内存
			 3，使用SendMessage 向另一进程发送WM_COPYDATA消息
			在此使用第三种方法
		     WM_COPYDATA消息用法见
              http://wenku.baidu.com/view/1a078b3283c4bb4cf7ecd1ae.html
          
			*/
			// 向监视窗口发送消息
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
			
		}
	}__finally
	{
	};
	return nRet;
}

// 以下和上面的操作类似不再详细解释

 // RegCreateKeyExW 替换
LONG WINAPI Replace_RegCreateKeyExW(HKEY hKey,LPCWSTR lpSubKey,DWORD Reserved,LPWSTR lpClass,DWORD dwOptions,REGSAM samDesired,
									LPSECURITY_ATTRIBUTES lpSecurityAttributes,PHKEY phkResult,LPDWORD lpdwDisposition)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,
										lpSecurityAttributes, phkResult, lpdwDisposition);
		if(!HOOK_RegCreateKeyEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params ;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpSubKey, wcslen(lpSubKey)*2);
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegCreateKeyExW;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}
  
// RegCreateKeyExA 替换

LONG WINAPI Replace_RegSetValueExA(HKEY hKey,LPCSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE* lpData,DWORD cbData)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
		if(!HOOK_RegSetValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL || lpData == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params ;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpValueName, strlen(lpValueName));
			params.type = dwType;
			if(cbData>MAX_BUF_LEN )
			{
				memcpy(params.buf2, lpData, 1024);
				params.cbbuf2 = MAX_BUF_LEN;
			}
			else
			{
				memcpy(params.buf2, lpData, cbData);
				params.cbbuf2 = cbData;
			}
			params.result = nRet;

			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegSetValueExA;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}

// RegSetValueExW 替换

LONG WINAPI Replace_RegSetValueExW(HKEY hKey,LPCWSTR lpValueName,DWORD Reserved,DWORD dwType,CONST BYTE* lpData,DWORD cbData)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
		if(!HOOK_RegSetValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL || lpData == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpValueName, wcslen(lpValueName)*2);
			params.type = dwType;
			if(cbData>MAX_BUF_LEN ) // 判断是否大于第一缓冲区的内容
			{
				memcpy(params.buf2, lpData, MAX_BUF_LEN );
				params.cbbuf2 = MAX_BUF_LEN ;
			}
			else // 存放在第二缓冲区内
			{
				memcpy(params.buf2, lpData, cbData);
				params.cbbuf2 = cbData;
			}
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegSetValueExW;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}

// RegSetValueExA 替换
LONG WINAPI Replace_RegDeleteKeyA(HKEY hKey,LPCSTR lpSubKey)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegDeleteKeyA(hKey, lpSubKey);
		if(!HOOK_RegDeleteKey)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpSubKey, strlen(lpSubKey));
			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegDeleteKeyA;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}

// RegDeleteKeyW替换
LONG WINAPI Replace_RegDeleteKeyW(HKEY hKey,LPCWSTR lpSubKey )
{
	LONG nRet;
	__try
	{
		nRet = Real_RegDeleteKeyW(hKey, lpSubKey);
		if(!HOOK_RegDeleteKey)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpSubKey, wcslen(lpSubKey)*2);
			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegDeleteKeyW;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}

// RegDeleteKeyA替换
LONG WINAPI Replace_RegDeleteValueA(HKEY hKey,LPCSTR lpValueName)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegDeleteValueA(hKey, lpValueName);
		if(!HOOK_RegDeleteValue)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpValueName, strlen(lpValueName));
			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegDeleteValueA;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}
 // RegDeleteValueW替换
LONG WINAPI Replace_RegDeleteValueW(HKEY hKey,LPCWSTR lpValueName)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegDeleteValueW(hKey,lpValueName);
		if(!HOOK_RegDeleteValue)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buf1, lpValueName, wcslen(lpValueName)*2);
			data.cbData = sizeof(PARAMS);
			data.lpData =(void *)&params;
			data.dwData = TYPE_RegDeleteValueW;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
	}__finally
	{
	};
	return nRet;
}

 // RegDeleteValueA替换
LONG WINAPI Replace_RegQueryValueExA(HKEY hKey, LPCSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType, LPBYTE lpData,LPDWORD lpcbData)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
		if(!HOOK_RegQueryValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(nRet != ERROR_SUCCESS)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			if(lpValueName == NULL || /*lpType == NULL ||*/ lpData == NULL || lpcbData == NULL)
				return nRet;
			
			memcpy(params.buf1, lpValueName, strlen(lpValueName));
			
			if(lpType == NULL)
				params.type = 0;
			else
				params.type = *lpType;
			
			if(*lpcbData == 0)
			{
				return nRet;
			}
			else if(*lpcbData>MAX_BUF_LEN )
			{
				memcpy(params.buf2, lpData, 1024);
				params.cbbuf2 = MAX_BUF_LEN;
			}
			else
			{
				memcpy(params.buf2, lpData, *lpcbData);
				params.cbbuf2 = *lpcbData;
			}
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = (void *)&params;
			data.dwData = TYPE_RegQueryValueExA;

		::SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
		
	}__finally
	{
	};
	return nRet;
}
  // RegQueryValueExW替换
LONG APIENTRY Replace_RegQueryValueExW(HKEY hKey,LPCWSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData)
{
	LONG nRet;
	__try
	{
		nRet = Real_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
		if(!HOOK_RegQueryValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(nRet != ERROR_SUCCESS)
				return nRet;

			COPYDATASTRUCT data;
			PARAMS params;
			memset(&params,0,sizeof(PARAMS));
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			
			if(lpValueName == NULL || /*lpType == NULL ||*/ lpData == NULL || lpcbData == NULL)
				return nRet;
			
			memcpy(params.buf1, lpValueName, wcslen(lpValueName)*2);
			
			if(lpType == NULL)
				params.type = 0;
			else
				params.type = *lpType;
			
			if(*lpcbData == 0)
			{
				return nRet;
			}
			else if(*lpcbData>MAX_BUF_LEN )
			{
				memcpy(params.buf2, lpData, MAX_BUF_LEN );
				params.cbbuf2 = MAX_BUF_LEN ;
			}
			else
			{
				memcpy(params.buf2, lpData, *lpcbData);
				params.cbbuf2 = *lpcbData;
			}

			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData =(void *)&params;
			data.dwData = TYPE_RegQueryValueExW;
			SendMessage(g_hWnd, WM_COPYDATA, (WPARAM)g_hWnd, (LPARAM)&data);
		}
		
	}__finally
	{
	};
	return nRet;
}
  





/****************************************************************************************************************/

// 拦截函数
void Intercept()
{
	// 使用DetourFunctionWithTrampoline函数来截获目标函数。这个函数有两个参数：trampoline函数以
	// 及截获函数的指针。因为目标函数已经被加到trampoline函数中，所有不需要在参数中特别指定。

	DetourFunctionWithTrampoline((PBYTE)Real_RegCreateKeyExA, (PBYTE)Replace_RegCreateKeyExA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegCreateKeyExW, (PBYTE)Replace_RegCreateKeyExW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegSetValueExA, (PBYTE)Replace_RegSetValueExA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegSetValueExW, (PBYTE)Replace_RegSetValueExW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteKeyA, (PBYTE)Replace_RegDeleteKeyA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteKeyW, (PBYTE)Replace_RegDeleteKeyW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteValueA, (PBYTE)Replace_RegDeleteValueA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteValueW, (PBYTE)Replace_RegDeleteValueW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegQueryValueExA, (PBYTE)Replace_RegQueryValueExA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegQueryValueExW, (PBYTE)Replace_RegQueryValueExW);
}

// 卸载拦截
void UnIntercept()
{
	// 调用DetourRemove来去掉对一个目标函数的截获
	DetourRemove((PBYTE)Real_RegCreateKeyExA,(PBYTE)Replace_RegCreateKeyExA);
	DetourRemove((PBYTE)Real_RegCreateKeyExW,(PBYTE)Replace_RegCreateKeyExW);
	DetourRemove((PBYTE)Real_RegSetValueExA,(PBYTE)Replace_RegSetValueExA);
	DetourRemove((PBYTE)Real_RegSetValueExW,(PBYTE)Replace_RegSetValueExW);
	DetourRemove((PBYTE)Real_RegDeleteKeyA, (PBYTE)Replace_RegDeleteKeyA);
	DetourRemove((PBYTE)Real_RegDeleteKeyW, (PBYTE)Replace_RegDeleteKeyW);
	DetourRemove((PBYTE)Real_RegDeleteValueA, (PBYTE)Replace_RegDeleteValueA);
	DetourRemove((PBYTE)Real_RegDeleteValueW, (PBYTE)Replace_RegDeleteValueW);
	DetourRemove((PBYTE)Real_RegQueryValueExA, (PBYTE)Replace_RegQueryValueExA);
	DetourRemove((PBYTE)Real_RegQueryValueExW, (PBYTE)Replace_RegQueryValueExW);
}


// 钩子函数
LRESULT CALLBACK ShellProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if(!g_bTryed)
	{
		g_bTryed=true;
		Intercept();
		g_bIntercepted=true;
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam); // 钩子链
}



	/************************************************************************/
	/*	功能  :	卸载钩子函数，把系统中的钩子卸载出去						*/
	/*	返回值:	成功返回true, 失败返回 false								*/
	/************************************************************************/
 bool InstallHook(HWND handle)
{
	 // 设置钩子
	g_hHook=::SetWindowsHookEx(WH_SHELL, ShellProc, GetModuleHandle("RegHook.dll"), 0);
	g_hWnd = handle;
	RegMonPID = GetCurrentProcessId();
	if(g_hHook)
		return true;
	else
		return false;
}

 
	/************************************************************************/
	/*	功能  :	卸载钩子函数，把系统中的钩子卸载出去						*/
	/*	返回值:	成功返回true, 失败返回 false								*/
	/************************************************************************/
bool UninstallHook()
{
	if(::UnhookWindowsHookEx(g_hHook))
	{
		g_hHook = NULL;
		return true;
	}
	else
		return false;
}

	/************************************************************************/
	/*	功能  :	过滤函数，对指定的进程进行挂钩监视							*/
	/*	返回值:	无															*/
	/************************************************************************/
 void SetFilter(DWORD pid)
{
	PID = pid;
}

	/*************************************************************************/
	/*  功能 :钩子监视的内容,对指定的行为进行挂钩监视(相应的参数为true表示是)*/		
	/*	返回值:	无															 */
	/*************************************************************************/
 void SetHookFunction(BOOL RegCreateKeyEx, BOOL RegSetValueEx, BOOL RegDeleteKey, BOOL RegDeleteValue, BOOL RegQueryValueEx)
{
	HOOK_RegCreateKeyEx = RegCreateKeyEx;
	HOOK_RegSetValueEx = RegSetValueEx;
	HOOK_RegDeleteKey = RegDeleteKey;
	HOOK_RegDeleteValue = RegDeleteValue;
	HOOK_RegQueryValueEx = RegQueryValueEx;
}

 	/*************************************************************************/
	/*  功能 :设置资源管理器进程 ID号										*/		
	/*	返回值:	无															 */
	/*************************************************************************/

 void SetExplorerPID(DWORD pid)
{
	ExplorerPID = pid;
}
