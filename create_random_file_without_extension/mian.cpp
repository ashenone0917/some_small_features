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
wstring wstrRand(int length) {			
    wchar_t tmp;							
    wstring buffer;						
    buffer.reserve(length);
    random_device rd;					
    default_random_engine random(rd());
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


int createFileRand(const std::wstring& file_path,int file_name_max,int file_count) {			
    random_device rd;					
    default_random_engine random(rd());	
    //std::wcout << L"create file..."<< std::endl;
    int failCount = 0;
    for (auto i = 0; i < file_count; ++i) {
        ::Sleep(1);
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
            //std::wcout << L"create file seq = " << i << std::endl;
            ::CloseHandle(handle);
        }
    }
    return failCount;
}

std::vector<std::wstring> createDirRand(const std::wstring& file_path, int file_name_max, int file_count) {
    random_device rd;
    default_random_engine random(rd());
    std::vector<std::wstring> ret;
    for (auto i = 0; i < file_count; ++i) {
        ::Sleep(1);
        auto fileLength = random() % file_name_max;
        auto fileName = file_path + L"\\" + std::to_wstring(i) + wstrRand(fileLength);
        auto handle = CreateDirectory(fileName.c_str(), NULL);
        if (FALSE == handle) {
            auto lastErrorCode = ::GetLastError();
            std::wcout << L"create dir last error = "
                << lastErrorCode << std::endl;
            --i;
            continue;
        }
        ret.emplace_back(std::move(fileName));
    }
    return ret;
}

//<param  "path","file count","current file max" />
void createFileAndDir(const std::wstring& file_path, int file_count,int file_max_layer) {
    if (file_count <= file_max_layer) {
        auto name_name_len = MAX_PATH - file_path.length() > 100 ? 100 : MAX_PATH - file_path.length();
        createFileRand(file_path, name_name_len, file_count);
    }
    else {
        auto modsize = file_count % file_max_layer;

        if (modsize > 0) {
            auto name_name_len = MAX_PATH - file_path.length() > 100 ? 100 : MAX_PATH - file_path.length();
            createFileRand(file_path, name_name_len, modsize);
        }

        auto subDir = createDirRand(file_path, 10, file_max_layer);
        int subsize = 0;
        if (file_max_layer == 1) subsize = file_count - file_max_layer;
        else subsize = file_count / file_max_layer;
        for (auto& dir : subDir) {
            createFileAndDir(dir, subsize, file_max_layer);
        }
    }
}

int wmain(int argc,wchar_t* argv[]) {
    if (argc != 4) {
        std::wcout << L"command line input is wrong, the correct input is \"file path\" \"file_cout\" \"file count in dir\" " << std::endl;
        std::wcout << L"example. \"D:\\filetemp\" 100000 1000" << std::endl;
        getchar();
        return 0;
    }

    auto path = argv[1];
    auto count = std::stol(argv[2]);
    auto count_in_dir = std::stol(argv[3]);
    //目录，文件总数，每个目录下最多的文件或目录数量
    std::wcout << L"start" << std::endl;
    createFileAndDir(path, count, count_in_dir);
    std::wcout << L"done"<<std::endl;
    getchar();
    return 0;

}