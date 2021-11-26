// DetectWin11.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <sstream>
inline BOOL getOsVersion(OSVERSIONINFOEXW& OSVersionInfo) {
	ZeroMemory(&OSVersionInfo, sizeof(OSVERSIONINFOEXW));

#pragma warning( disable : 4996 )
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
	if (!GetVersionExW((LPOSVERSIONINFOW)&OSVersionInfo)) {
		return FALSE;
	}
#pragma warning( default : 4996 )

	if (OSVersionInfo.dwMajorVersion == 6 && OSVersionInfo.dwMinorVersion == 2) {
		// 从Windows8开始，用GetVersionEx()获取版本号都返回6.2，要调用内部API来获取准确的版本
		HMODULE hModNtdll = NULL;
		DWORD dwMajorVer = 0;
		DWORD dwMinorVer = 0;
		DWORD dwBuildNumber = 0;
		if (hModNtdll = ::LoadLibraryA("ntdll.dll")) {
			typedef void (WINAPI* pfRTLGETNTVERSIONNUMBERS)(DWORD*, DWORD*, DWORD*);
			pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
			pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
			if (pfRtlGetNtVersionNumbers) {
				pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
				dwBuildNumber &= 0x0ffff;
			}
		}
		else {
			return FALSE;
		}
		::FreeLibrary(hModNtdll);
		OSVersionInfo.dwMajorVersion = dwMajorVer;
		OSVersionInfo.dwMinorVersion = dwMinorVer;
		OSVersionInfo.dwBuildNumber = dwBuildNumber;
	}

	return TRUE;
}

inline  BOOL IsWin11() {
	OSVERSIONINFOEXW OSVersionInfo = { 0 };
	BOOL bIsWin11 = FALSE;
	if (getOsVersion(OSVersionInfo)) {
		std::cout << "dwMajorVersion : " << OSVersionInfo.dwMajorVersion << std::endl
			<< "dwMinorVersion : " << OSVersionInfo.dwMinorVersion << std::endl
			<< "dwBuildNumber : " << OSVersionInfo.dwBuildNumber << std::endl;
		if (OSVersionInfo.dwMajorVersion >= 10
			&& OSVersionInfo.dwBuildNumber >= 22000) {
			bIsWin11 = TRUE;
		}
	}
	else {
		std::cout << "getOsVersion fail" << std::endl;
	}
	return bIsWin11;
}

std::string GetVersionStr() {
	const int VERSION_STRING_LEN = 1024;
	HKEY key;
	if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), &key) != ERROR_SUCCESS) {
		return "";
	}
	char value[VERSION_STRING_LEN];
	DWORD len = sizeof(value);
	std::string res;
	ZeroMemory(value, len);
	if (RegQueryValueExA(key, "ProductName", 0, NULL, (LPBYTE)&value, &len) != ERROR_SUCCESS) {
		RegCloseKey(key);
		return "";
	}
	res = std::string(value);
	len = sizeof(value);
	ZeroMemory(value, len);
	if (RegQueryValueExA(key, "ReleaseId", 0, NULL, (LPBYTE)&value, &len) != ERROR_SUCCESS) {
		RegCloseKey(key);
		return res;
	}
	res = res + " " + value;
	RegCloseKey(key);
	return res;
}

std::string GetVersionStrV2() {
	std::string productName = GetVersionStr();
	if (IsWin11()) {
		static std::string win10 = "Windows 10";
		static std::string win11 = "Windows 11";
		auto findWin10 = productName.find(win10);
		if (findWin10 != std::string::npos) {
			productName.replace(findWin10, win10.size(), win11);
		}
	}
	return productName;
}

int main()
{
	std::cout<< GetVersionStrV2() << std::endl;
	IsWin11();
	getchar();
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
