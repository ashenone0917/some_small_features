#include <string>
#include <random>
#include <Windows.h>
#include <iostream>
using std::string;
using std::wstring;
using std::random_device;
using std::default_random_engine;

//不包含中文，只有大写
string strRand(int length) {
    char tmp;
    string buffer;
    buffer.reserve(length);
    random_device rd;
    default_random_engine random(rd());

    for (int i = 0; i < length; i++) {
        tmp = random() % 36;
        if (tmp < 10) {
            tmp += '0';
        }
        else {
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

//包含中文大小写
wstring wstrRand(int length) {			// length: 产生字符串的长度
    wchar_t tmp;							// tmp: 暂存一个随机数
    wstring buffer;						// buffer: 保存返回值
    buffer.reserve(length);
    // 下面这两行比较重要:
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random
    auto chineseRange = 0x9FA5 - 0x4E00;
    for (int i = 0; i < length; i++) {
        auto isChinese = random() % 2;
        if (isChinese) {
            tmp = 0x4E00 + (random() % chineseRange);
        }
        else {
            tmp = random() % 62;
            if (tmp < 10) {
                tmp += L'0';
            }
            else if (tmp < 36) {
                tmp += L'A' - 10;
            }
            else {
                tmp += L'a' - 36;
            }
        }
        buffer += tmp;
    }
    return buffer;
}


int createFileRand(const std::wstring& file_path,int file_name_max,int file_count) {			// length: 产生字符串的长度
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

    int failCount = 0;
    for (auto i = 0; i < file_count; ++i) {
        auto fileLength = random() % file_name_max;
        auto fileName = file_path + L"\\" + std::to_wstring(i) + wstrRand(fileLength);
        auto handle = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (INVALID_HANDLE_VALUE == handle) {
            auto lastErrorCode = ::GetLastError();
            std::wcout << L"create file last error = "
                << lastErrorCode << std::endl;
            --i;
            ++failCount;
        }
        else {
            std::wcout << L"create file seq = " << i << std::endl;
            ::CloseHandle(handle);
        }
    }
    return failCount;
}
int wmain(int argc,wchar_t* argv[]) {
    if (argc != 4) {
        std::wcout << L"command line input is wrong, the correct input is \"file path\" \"file_name_max_cout\" \"file count\" " <<std::endl;
        std::wcout << L"example. \"D:\\filetemp\" 260 1000" << std::endl;
        getchar();
        return 0;
    }

    auto path = argv[1];
    auto max_length = std::stol(argv[2]);
    auto file_count = std::stol(argv[3]);

    auto fail = createFileRand(path, max_length, file_count);
    std::wcout << L"done"<<std::endl;
    getchar();
    return 0;

}