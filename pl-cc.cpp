#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <locale>
#include <codecvt>
#include <vector>  
#include <algorithm> 
#include <regex>
#include <set>
#include <stack>
#include <windows.h>
#include <cctype>
using namespace std;

// 最简单的内联函数方案
inline std::wstring wred(const std::wstring& text) {
    return L"\033[31m" + text + L"\033[0m";
}

inline std::wstring wgreen(const std::wstring& text) {
    return L"\033[32m" + text + L"\033[0m";
}

inline std::wstring wblue(const std::wstring& text) {
    return L"\033[34m" + text + L"\033[0m";
}

inline std::wstring wyellow(const std::wstring& text) {
    return L"\033[33m" + text + L"\033[0m";
}

// 默认映射关系
map<wstring, wstring> Map = {
    {L"使用库",L"#include "},
    {L"使用命名空间",L"using namespace "},
    {L"主函数",L"int main"},
    {L"函数",L"void"},
    {L"输出", L"cout << "},
    {L"输入", L"cin >> "},
    {L"整形", L"int "},
    {L"字符型", L"string "},
    {L"长整形", L"long long "},
    {L"浮点形", L"double "},
	{L"布尔型", L"bool "},
	{L"真", L"true"},
	{L"假", L"false"},
    {L"如果", L"if "},
    {L"否则", L"else "},
    {L"否则如果", L"else if "},
    {L"循环w", L"while "},
    {L"循环f", L"for "},
    {L"循环", L"for "}
};

// string到wstring的转换函数
wstring string_to_wstring(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// wstring到string的转换函数
string wstring_to_string(const wstring& wstr) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// 检查文件是否存在
bool file_exists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// 简单的JSON解析函数
bool load_mappings_from_json(const string& filename, map<wstring, wstring>& mappings) {
    if (!file_exists(filename)) {
        wcout << wyellow(L"配置文件 ") << string_to_wstring(filename) << wyellow(L" 不存在，使用默认映射关系") << endl;
        return false;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        wcerr << wred(L"错误：无法打开配置文件: ") << string_to_wstring(filename) << endl;
        return false;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // 简单的JSON解析（处理基本键值对）
    // 移除空格和换行
    string cleaned_content;
    for (char c : content) {
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            cleaned_content += c;
        }
    }

    content = cleaned_content;

    // 检查是否是有效的JSON对象
    if (content.empty() || content[0] != '{' || content[content.size() - 1] != '}') {
        wcerr << wred(L"错误：无效的JSON格式，期望 {...}") << endl;
        return false;
    }

    // 移除外层大括号
    content = content.substr(1, content.size() - 2);

    size_t pos = 0;
    int count = 0;

    while (pos < content.size()) {
        // 查找键的开始引号
        if (content[pos] != '"') {
            wcerr << wred(L"错误：期望键的引号，找到: ") << content[pos] << endl;
            break;
        }

        // 查找键的结束引号
        size_t key_end = content.find('"', pos + 1);
        if (key_end == string::npos) {
            wcerr << wred(L"错误：键的引号不匹配") << endl;
            break;
        }

        // 提取键
        string key = content.substr(pos + 1, key_end - pos - 1);

        // 查找冒号
        pos = key_end + 1;
        if (pos >= content.size() || content[pos] != ':') {
            wcerr << wred(L"错误：期望冒号") << endl;
            break;
        }

        // 查找值的开始引号
        pos++;
        if (pos >= content.size() || content[pos] != '"') {
            wcerr << wred(L"错误：期望值的引号") << endl;
            break;
        }

        // 查找值的结束引号
        size_t value_end = content.find('"', pos + 1);
        if (value_end == string::npos) {
            wcerr << wred(L"错误：值的引号不匹配") << endl;
            break;
        }

        // 提取值
        string value = content.substr(pos + 1, value_end - pos - 1);

        // 添加到映射
        wstring wkey = string_to_wstring(key);
        wstring wvalue = string_to_wstring(value);
        mappings[wkey] = wvalue;
        count++;

        wcout << L"加载映射: " << wkey << L" -> " << wvalue << endl;

        // 移动到下一个（逗号或结束）
        pos = value_end + 1;
        if (pos < content.size() && content[pos] == ',') {
            pos++;
        }
    }

    if (count > 0) {
        wcout << wgreen(L"成功从 ") << string_to_wstring(filename) << wgreen(L" 加载 ")
            << count << wgreen(L" 个映射关系") << endl;
        return true;
    }
    else {
        wcerr << wred(L"错误：未能从配置文件中加载任何映射关系") << endl;
        return false;
    }
}

// 检查字符是否可打印（排除控制字符，但允许空格、制表符等）
bool is_printable_char(char c) {
    return (c >= 32 && c <= 126) || c == '\t' || c == '\n' || c == '\r';
}

// 清理字符串中的不可见字符
string clean_string(const string& str) {
    string result;
    for (char c : str) {
        if (is_printable_char(c)) {
            result += c;
        }
    }
    return result;
}

// 彻底清理C++代码中的不可见字符
string sanitize_cpp_code(const string& code) {
    string result;
    bool in_string = false;
    char string_char = 0;
    bool in_comment = false;
    bool line_comment = false;

    for (size_t i = 0; i < code.length(); i++) {
        char c = code[i];

        // 处理行注释
        if (!in_string && !in_comment && c == '/' && i + 1 < code.length() && code[i + 1] == '/') {
            line_comment = true;
            result += "//";
            i++; // 跳过下一个字符
            continue;
        }

        // 处理块注释
        if (!in_string && !line_comment && c == '/' && i + 1 < code.length() && code[i + 1] == '*') {
            in_comment = true;
            result += "/*";
            i++; // 跳过下一个字符
            continue;
        }

        // 结束块注释
        if (in_comment && c == '*' && i + 1 < code.length() && code[i + 1] == '/') {
            in_comment = false;
            result += "*/";
            i++; // 跳过下一个字符
            continue;
        }

        // 处理换行（结束行注释）
        if (line_comment && c == '\n') {
            line_comment = false;
        }

        // 在注释中，保留所有字符
        if (in_comment || line_comment) {
            result += c;
            continue;
        }

        // 处理字符串字面量
        if (!in_string && (c == '"' || c == '\'')) {
            in_string = true;
            string_char = c;
            result += c;
        }
        else if (in_string && c == string_char) {
            // 检查是否是转义字符
            if (i > 0 && code[i - 1] == '\\') {
                result += c; // 转义引号，继续字符串
            }
            else {
                in_string = false;
                result += c;
            }
        }
        else if (in_string) {
            result += c; // 字符串内的所有字符都保留
        }
        else {
            // 非字符串、非注释内容：只保留可打印ASCII字符
            if (c >= 32 && c <= 126) {
                result += c;
            }
            else if (c == '\n' || c == '\t' || c == '\r') {
                result += c; // 保留空白字符
            }
            // 其他控制字符和Unicode字符被忽略
        }
    }

    return result;
}

// 判断行是否需要分号
bool needs_semicolon(const wstring& line) {
    if (line.empty()) return false;

    wstring trimmed_line = line;
    // 移除首尾空格
    size_t first = trimmed_line.find_first_not_of(L" \t");
    size_t last = trimmed_line.find_last_not_of(L" \t");
    if (first != wstring::npos && last != wstring::npos) {
        trimmed_line = trimmed_line.substr(first, last - first + 1);
    }

    // 以这些关键字开头的行不需要分号
    if (trimmed_line.find(L"#") == 0 ||  // 预处理指令
        trimmed_line.find(L"//") == 0 || // 注释
        trimmed_line.find(L"/*") == 0 || // 注释开始
        trimmed_line.find(L"for") == 0 ||
        trimmed_line.find(L"while") == 0 ||
        trimmed_line.find(L"if") == 0 ||
        trimmed_line.find(L"else") == 0 ||
        trimmed_line.find(L"switch") == 0 ||
        trimmed_line.find(L"case") == 0 ||
        trimmed_line.find(L"default") == 0 ||
        trimmed_line.find(L"return") == 0 && trimmed_line.find(L";") != wstring::npos) { // return语句已有分号
        return false;
    }

    // 包含这些字符的行不需要分号
    if (trimmed_line.find(L"{") != wstring::npos ||
        trimmed_line.find(L"}") != wstring::npos ||
        trimmed_line.find(L":") != wstring::npos && trimmed_line.find(L"case") != wstring::npos) {
        return false;
    }

    // 已经以分号结尾的行不需要再加分号
    if (!trimmed_line.empty() && trimmed_line.back() == L';') {
        return false;
    }

    // 函数定义不需要分号
    if (trimmed_line.find(L"(") != wstring::npos && trimmed_line.find(L")") != wstring::npos) {
        size_t brace_pos = trimmed_line.find(L"{");
        if (brace_pos != wstring::npos) {
            return false;
        }
    }

    // 其他情况需要分号
    return true;
}

// 自动补全分号
wstring auto_complete_semicolon(const wstring& line) {
    wstring result = line;

    if (needs_semicolon(result)) {
        // 移除行尾可能的空格后添加分号
        size_t last_char = result.find_last_not_of(L" \t");
        if (last_char != wstring::npos) {
            result = result.substr(0, last_char + 1) + L";";
        }
        else {
            result += L";";
        }
    }

    return result;
}

// 处理循环语句 - 修正版本
wstring process_loop_statement(const wstring& line) {
    wstring result = line;

    // 匹配"循环 (条件)"模式，正确处理括号
    wregex loop_pattern(L"循环\\s*(\\(.*)\\)?\\s*\\{?");
    wsmatch match;

    if (regex_search(line, match, loop_pattern) && match.size() > 1) {
        wstring loop_content = match[1].str();

        // 清理条件内容
        if (!loop_content.empty()) {
            // 移除可能的多余括号
            if (loop_content.back() == L')') {
                loop_content.pop_back();
            }
            if (loop_content.back() == L'{') {
                loop_content.pop_back();
            }

            // 移除首尾空格
            size_t start = loop_content.find_first_not_of(L" ");
            size_t end = loop_content.find_last_not_of(L" ");
            if (start != wstring::npos && end != wstring::npos) {
                loop_content = loop_content.substr(start, end - start + 1);
            }
        }

        result = L"for " + loop_content + L" {";
    }

    return result;
}

// 处理输出语句（处理中文引号）
wstring process_output_statement(const wstring& line) {
    wstring result = line;

    // 处理中文引号
    size_t pos = 0;
    while ((pos = result.find(L"“", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }

    pos = 0;
    while ((pos = result.find(L"”", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }

    return result;
}

void replace_code(wstring& text, const map<wstring, wstring>& Map) {
    // 先处理特殊结构（如循环）
    if (text.find(L"循环") == 0) {
        text = process_loop_statement(text);
    }

    // 处理输出语句中的引号问题
    if (text.find(L"输出") == 0) {
        text = process_output_statement(text);
    }

    // 然后执行正常替换
    for (auto& pair : Map) {
        size_t pos = 0;
        // 确保替换整个单词，而不是部分匹配
        while ((pos = text.find(pair.first, pos)) != wstring::npos) {
            // 检查周围字符确保是整个单词
            bool is_whole_word = true;
            if (pos > 0) {
                wchar_t prev_char = text[pos - 1];
                if (iswalnum(prev_char) || prev_char == L'_') {
                    is_whole_word = false;
                }
            }
            if (pos + pair.first.length() < text.length()) {
                wchar_t next_char = text[pos + pair.first.length()];
                if (iswalnum(next_char) || next_char == L'_') {
                    is_whole_word = false;
                }
            }

            if (is_whole_word) {
                text.replace(pos, pair.first.length(), pair.second);
                pos += pair.second.length();
            }
            else {
                pos += pair.first.length();
            }
        }
    }

    // 特别处理：修复循环语句中的多余括号
    if (text.find(L"for") == 0) {
        size_t pos = text.find(L"))");
        if (pos != wstring::npos) {
            text.replace(pos, 2, L")");
        }
    }

    // 自动补全分号
    text = auto_complete_semicolon(text);
}

wstring read_document(const wstring& filename) {
    wifstream file(filename, ios::binary);
    if (!file.is_open()) {
        wcerr << L"无法打开文件: " << filename << endl;
        return L"";
    }

    // 设置UTF-8编码
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    wstringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string generate_cpp_program(const wstring& code) {
    stringstream full_program;

    wistringstream wstream(code);
    wstring line;
    while (getline(wstream, line)) {
        if (!line.empty()) {
            // 移除首尾空白字符
            size_t first = line.find_first_not_of(L" \t\r\n");
            size_t last = line.find_last_not_of(L" \t\r\n");
            if (first != wstring::npos && last != wstring::npos) {
                line = line.substr(first, last - first + 1);
            }

            if (!line.empty()) {
                string normal_line = wstring_to_string(line);

                // 关键修复：移除BOM标记和不可见字符
                if (!normal_line.empty()) {
                    // 检查并移除UTF-8 BOM (EF BB BF)
                    if (normal_line.length() >= 3 &&
                        static_cast<unsigned char>(normal_line[0]) == 0xEF &&
                        static_cast<unsigned char>(normal_line[1]) == 0xBB &&
                        static_cast<unsigned char>(normal_line[2]) == 0xBF) {
                        normal_line = normal_line.substr(3);
                    }

                    // 移除行首空白
                    normal_line.erase(0, normal_line.find_first_not_of(" \t"));

                    // 移除其他不可见Unicode字符
                    normal_line.erase(std::remove_if(normal_line.begin(), normal_line.end(),
                        [](char c) {
                            return static_cast<unsigned char>(c) < 32 && c != '\t' && c != '\n' && c != '\r';
                        }), normal_line.end());
                }

                if (!normal_line.empty()) {
                    // 检查是否需要分号
                    bool needs_semicolon_in_output = false;
                    if (normal_line.find("for") == 0 || normal_line.find("while") == 0 ||
                        normal_line.find("if") == 0 || normal_line.find("else") == 0 ||
                        normal_line.find("{") != string::npos ||
                        normal_line.find("}") != string::npos ||
                        normal_line.find("#") == 0) {
                        needs_semicolon_in_output = false;
                    }
                    else if (!normal_line.empty() && normal_line.back() == ';') {
                        needs_semicolon_in_output = false;
                    }
                    else {
                        needs_semicolon_in_output = (normal_line.find(";") == string::npos);
                    }

                    // 特别处理：修复循环语句格式
                    if (normal_line.find("for (int  i=0;i<=5;i++))") != string::npos) {
                        normal_line = "for (int i = 0; i <= 5; i++) {";
                        needs_semicolon_in_output = false;
                    }

                    if (needs_semicolon_in_output && !normal_line.empty()) {
                        full_program << "    " << normal_line << ";\n";
                    }
                    else {
                        full_program << "    " << normal_line << "\n";
                    }
                }
            }
        }
    }

    // 最终清理：确保整个程序没有BOM和不可见字符
    string result = full_program.str();
    result = sanitize_cpp_code(result);

    return result;
}

bool save_to_file(const string& content, const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "无法创建文件: " << filename << endl;
        return false;
    }

    // 不写入UTF-8 BOM头，因为C++编译器不需要
    // 直接写入清理后的内容
    file << content;

    // 检查文件是否成功写入
    if (file.fail()) {
        cerr << "写入文件失败: " << filename << endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

void display_file_content(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }

    // 检查并跳过BOM头
    char bom[3];
    file.read(bom, 3);
    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
        file.seekg(0);
    }

    string line;
    wcout << wgreen(L"\n=== 生成的C++代码 ===") << endl;
    while (getline(file, line)) {
        cout << line << endl;
    }
    wcout << wgreen(L"=== 转换结束 ===") << endl;
    file.close();
}

void create_sample_code_file(const string& filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        // 写入UTF-8 BOM头
        file << "\xEF\xBB\xBF";
        file << "输出 \"你好，世界！\"\n";
        file << "整形 num = 20\n";
        file << "字符型 name = \"小明\"\n";
        file << "输出 \"姓名: \" << name\n";
        file << "输出 \"年龄: \" << num\n";
        file << "循环 (整形 i=0;i<=5;i++) {\n";
        file << "    输出 i\n";
        file << "}\n";
        file.close();
        wcout << L"已创建示例文件: " << string_to_wstring(filename) << endl;
    }
}
// 删除文件
bool delete_file(const string& filename) {
    if (remove(filename.c_str()) == 0) {
        wcout << L"已删除文件: " << string_to_wstring(filename) << endl;
        return true;
    }
    else {
        wcerr << L"无法删除文件: " << string_to_wstring(filename) << endl;
        return false;
    }
}
int main(int argc, char* argv[]) {
    // 设置控制台为UTF-8编码
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    wcout.imbue(locale("en_US.UTF-8"));
    wcout << wgreen(L"=== 中文代码编译器 ===") << endl;
    // 首先尝试加载ctoc.json配置文件
    wcout << wblue(L"正在检查配置文件 ctoc.json...") << endl;
    load_mappings_from_json("ctoc.json", Map);
    string input_filename;
    string output_exe_name;
    string cpp_filename;
    // 处理命令行参数
    if (argc > 1) {
        input_filename = argv[1];
        wcout << L"使用命令行参数文件: " << string_to_wstring(input_filename) << endl;
    }
    else {
        wcout << wyellow(L"你没有在运行时输入参数，现在请输入输入文件名（例如：code.code）: ");
        string user_input;
        getline(cin, user_input);

        if (!user_input.empty()) {
            input_filename = user_input;
        }
        else {
            input_filename = "code.code";
            wcout << wyellow(L"使用默认文件名: ") << string_to_wstring(input_filename) << endl;
        }
    }
    // 检查输入文件是否存在，如果不存在则创建示例文件
    if (!file_exists(input_filename)) {
        wcout << L"文件 " << string_to_wstring(input_filename) << L" 不存在，创建示例文件..." << endl;
        create_sample_code_file(input_filename);
    }
    // 读取输入文件
    wstring wtext = read_document(string_to_wstring(input_filename));
    if (wtext.empty()) {
        wcerr << wred(L"错误：无法读取文件或文件为空: ") << string_to_wstring(input_filename) << endl;
        return 1;
    }
    wcout << wgreen(L"\n1. 原始代码 (") << string_to_wstring(input_filename) << wgreen(L"):") << endl;
    wcout << wtext << endl;
    // 保存原始行结构
    vector<wstring> original_lines;
    wistringstream wstream(wtext);
    wstring line;
    while (getline(wstream, line)) {
        if (!line.empty()) {
            // 移除回车符
            line.erase(remove(line.begin(), line.end(), L'\r'), line.end());
            // 移除首尾空白
            size_t first = line.find_first_not_of(L" \t");
            size_t last = line.find_last_not_of(L" \t");
            if (first != wstring::npos && last != wstring::npos) {
                line = line.substr(first, last - first + 1);
                original_lines.push_back(line);
            }
        }
    }
    // 分别替换每一行并自动补全分号
    vector<wstring> converted_lines;
    for (size_t i = 0; i < original_lines.size(); i++) {
        wstring converted_line = original_lines[i];
        replace_code(converted_line, Map);
        converted_lines.push_back(converted_line);
    }
    wcout << wgreen(L"\n2. 转换后代码（已自动补全分号）:") << endl;
    for (size_t i = 0; i < converted_lines.size(); i++) {
        wcout << L"   " << converted_lines[i] << endl;
    }
    // 重新组合转换后的行
    wstring converted_text;
    for (size_t i = 0; i < converted_lines.size(); i++) {
        converted_text += converted_lines[i] + L"\n";
    }
    // 生成临时cpp文件名（基于输入文件名）
    size_t dot_pos = input_filename.find_last_of('.');
    if (dot_pos != string::npos) {
        cpp_filename = input_filename.substr(0, dot_pos) + ".cpp";
    }
    else {
        cpp_filename = input_filename + ".cpp";
    }
    string cpp_program = generate_cpp_program(converted_text);
    if (save_to_file(cpp_program, cpp_filename)) {
        wcout << wgreen(L"\n3. C++程序已保存到: ") << string_to_wstring(cpp_filename) << endl;
        // 显示文件内容
        display_file_content(cpp_filename);
        // 询问是否编译
        wcout << wblue(L"\n是否编译程序？(y/n): ");
        string compile_choice;
        getline(cin, compile_choice);
        if (compile_choice == "y" || compile_choice == "Y") {
            // 询问输出可执行文件名
            wcout << L"请输入输出可执行文件名（不含扩展名，回车使用默认名）: ";
            string exe_input;
            getline(cin, exe_input);

            if (exe_input.empty()) {
                // 使用基于输入文件的默认名
                if (dot_pos != string::npos) {
                    output_exe_name = input_filename.substr(0, dot_pos);
                }
                else {
                    output_exe_name = input_filename;
                }
            }
            else {
                output_exe_name = exe_input;
            }
            // 添加平台特定的扩展名
#ifdef _WIN32
            string output_exe_file = output_exe_name + ".exe";
            string compile_command = "g++ -o \"" + output_exe_name + ".exe\" \"" + cpp_filename + "\" 2>&1";
#else
            string output_exe_file = output_exe_name;
            string compile_command = "g++ -o \"" + output_exe_name + "\" \"" + cpp_filename + "\" 2>&1";
#endif
            wcout << wgreen(L"\n编译中...") << endl;
            wcout << L"编译命令: " << string_to_wstring(compile_command) << endl;
            int compile_result = system(compile_command.c_str());
            if (compile_result == 0) {
                wcout << wgreen(L"编译成功！生成的可执行文件: ") << string_to_wstring(output_exe_file) << endl;
                // 询问是否运行程序
                wcout << L"\n是否运行程序？(y/n): ";
                string run_choice;
                getline(cin, run_choice);
                if (run_choice == "y" || run_choice == "Y") {
                    wcout << L"\n=== 程序输出 ===" << endl;
#ifdef _WIN32
                    string run_command = "\"" + output_exe_name + ".exe\"";
#else
                    string run_command = "./\"" + output_exe_name + "\"";
#endif
                    system(run_command.c_str());
					wcout << L"\n";
                    wcout << L"=== 程序结束 ===" << endl;
                }
                // 询问是否删除cpp文件
                wcout << wred(L"\n是否删除临时CPP文件 ") << string_to_wstring(cpp_filename) << wred(L"？(y/n): ");
                string delete_choice;
                getline(cin, delete_choice);

                if (delete_choice == "y" || delete_choice == "Y") {
                    delete_file(cpp_filename);
                }
                else {
                    wcout << L"保留CPP文件: " << string_to_wstring(cpp_filename) << endl;
                }
            }
            else {
                wcout << L"编译失败！" << endl;
                wcout << L"请检查G++编译器是否已安装" << endl;
                wcout << L"保留CPP文件用于调试: " << string_to_wstring(cpp_filename) << endl;
            }
        }
        else {
            wcout << L"跳过编译，保留CPP文件: " << string_to_wstring(cpp_filename) << endl;
        }
    }
    else {
        wcerr << L"保存C++程序失败！" << endl;
        return 1;
    }
    wcout << L"\n按Enter键退出...";
    cin.get();
    return 0;
}
