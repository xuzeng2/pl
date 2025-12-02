#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    wcout.imbue(locale("en_US.UTF-8"));

    // 构建命令行参数
    string args = "";
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            args += " ";
            args += argv[i];
        }
    }

    wcout << L"=== 中文编译器 ===" << endl;
    wcout << L"请选择要使用的编译器：" << endl;
    wcout << L"1. pl-cc (编译为C++)" << endl;
    wcout << L"2. pl-cpy (编译为Python)" << endl;
    wcout << L"3. pl-chb (编译为汇编语言)" << endl;
    wcout << L"4. pl-ai (使用ai编译)" << endl;
    wcout << L"5. 退出" << endl;
    wcout << L"请输入选择 (1-4): ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        wcout << L"输入无效！" << endl;
        return 1;
    }

    switch (choice) {
    case 1:
        wcout << L"启动 Programming Language Chinese C++ 编译器..." << endl;
        return system(("pl-cc" + args).c_str());
    case 2:
        wcout << L"启动 Programming Language Chinese Python 编译器..." << endl;
        return system(("pl-cpy" + args).c_str());
    case 3:
        wcout << L"启动 Programming Language Chinese assembly 编译器..." << endl;
        return system(("pl-chb" + args).c_str());
    case 4:
        wcout << L"启动 ai 编译器..." << endl;
        return system(("pl-ai" + args).c_str());
    case 5:
        wcout << L"退出程序。" << endl;
        return 0;
    default:
        wcout << L"无效的选择！" << endl;
        return 1;
    }
}
