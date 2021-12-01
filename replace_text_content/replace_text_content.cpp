// replace_text_content.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>
#include <shlwapi.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
VOID RepalceText(_In_ std::wstring wstrOld, _In_ std::wstring wstrNew, _In_ PCWSTR pwszPath)
{
	std::wifstream wif;
	std::wofstream wof;

	do
	{
		if (NULL == pwszPath ||
			wstrOld.empty() ||
			wstrNew.empty()
			) break;

		try
		{
			wif.open(pwszPath, std::wios::beg);//std::wios::beg就是begin,打开文件

			std::wstring wstrLine;
			std::wstring wstrFileData;

			BOOL bWrite = FALSE;

			while (std::getline(wif, wstrLine))//循环读取每行，直到读完
			{
				size_t pos = wstrLine.find(wstrOld);

				if (std::wstring::npos == pos)//pos == npos表示没有找到
				{
					wstrFileData += wstrLine;
					wstrFileData += L"\n";
					continue;
				}

				std::wstring wstrTemp = std::wstring(wstrLine.c_str(), pos);//获取前pos个字符
				//std::wstring temp2(temp, j);可以获取下标为j开始的字符串
				wstrTemp += wstrNew.c_str();
				wstrTemp += (wstrLine.c_str() + pos + wstrOld.size());

				wstrFileData += wstrTemp;
				wstrFileData += L"\n";

				bWrite = TRUE;
			}

			wif.close();//关闭读取流

			if (TRUE == bWrite)
			{
				wof.open(pwszPath, std::wios::beg);
				wof.flush();//清空缓冲区
				wof << wstrFileData;
				wof.close();
			}
		}
		catch (...) {}

	} while (0);
}

void GetAllDCubeAgentConfigPath() {
	HANDLE hFile;
	LPCTSTR lpFileName = L"C:\\ProgramData\\DCubeAgent*";
	WIN32_FIND_DATA pNextInfo;
	hFile = FindFirstFile(lpFileName, &pNextInfo);
	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}
	do {
		auto gflagsPath = L"C:\\ProgramData\\" +
			std::wstring(pNextInfo.cFileName) + L"\\Config\\meili_agent.gflags";
		if (PathFileExists(gflagsPath.c_str())) {
			RepalceText(L"=help.datacloak.cn", L"=https://help.datacloak.cn:12800", gflagsPath.c_str());
		}

	} while (FindNextFile(hFile, &pNextInfo));
}

int main()
{
	GetAllDCubeAgentConfigPath();
	std::cout << "done..." << std::endl;
	getchar();
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
