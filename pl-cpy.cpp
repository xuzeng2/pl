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
#include <stack>
#include <windows.h>
#include <limits>
using namespace std;

// 颜色函数
inline std::wstring wred(const std::wstring& text) { return L"\033[31m" + text + L"\033[0m"; }
inline std::wstring wgreen(const std::wstring& text) { return L"\033[32m" + text + L"\033[0m"; }
inline std::wstring wblue(const std::wstring& text) { return L"\033[34m" + text + L"\033[0m"; }
inline std::wstring wyellow(const std::wstring& text) { return L"\033[33m" + text + L"\033[0m"; }

// 映射表
map<wstring, wstring> Map = {
    {L"输出", L"print("},
    {L"输入", L"input("},
    {L"如果", L"if "},
    {L"否则", L"else:"},
    {L"否则如果", L"elif "},
    {L"循环w", L"while "},
    {L"循环", L"for "},
    {L"定义", L"def "},
    {L"返回", L"return "},
    {L"是", L"True"},
    {L"否", L"False"},
    {L"且", L" and "},
    {L"或", L" or "},
    {L"字符串", L"str"},
    {L"长度", L"len"}
};

// 转换函数
wstring string_to_wstring(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstring_to_string(const wstring& wstr) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// 计算括号是否匹配
int count_parentheses(const wstring& text) {
    int balance = 0;
    for (wchar_t c : text) {
        if (c == L'(') balance++;
        else if (c == L')') balance--;
    }
    return balance;
}

// 处理输出语句 - 确保正确的括号
wstring process_output_statement(const wstring& line) {
    wstring result = line;

    // 处理中文引号
    size_t pos = 0;
    while ((pos = result.find(L"「", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find(L"」", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find(L"“", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find(L"”", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }

    if (result.find(L"输出") == 0) {
        size_t output_pos = result.find(L"输出");
        wstring content = result.substr(output_pos + 2);
        size_t first = content.find_first_not_of(L" ");
        if (first != wstring::npos) content = content.substr(first);

        // 检查括号平衡
        int balance = count_parentheses(content);

        // 如果左括号比右括号多，添加缺失的右括号
        if (balance > 0) {
            content += wstring(balance, L')');
        }

        // 确保print语句有完整的括号结构
        if (content.find(L"(") == wstring::npos && content.find(L")") == wstring::npos) {
            // 如果没有括号，添加一对括号
            result = L"print(" + content + L")";
        }
        else {
            result = L"print(" + content;
            // 再次检查整体平衡
            balance = count_parentheses(result);
            if (balance > 0) {
                result += wstring(balance, L')');
            }
        }
    }
    return result;
}

// 处理输入语句
wstring process_input_statement(const wstring& line) {
    wstring result = line;

    if (result.find(L"输入") == 0) {
        size_t input_pos = result.find(L"输入");
        wstring content = result.substr(input_pos + 2);
        size_t first = content.find_first_not_of(L" ");
        if (first != wstring::npos) content = content.substr(first);

        // 如果输入语句没有参数，直接使用input()
        if (content.empty()) {
            result = L"input()";
        }
        else {
            // 检查括号平衡
            int balance = count_parentheses(content);

            // 确保input语句有完整的括号结构
            if (content.find(L"(") == wstring::npos && content.find(L")") == wstring::npos) {
                result = L"input(" + content + L")";
            }
            else {
                result = L"input(" + content;
                // 添加缺失的右括号
                balance = count_parentheses(result);
                if (balance > 0) {
                    result += wstring(balance, L')');
                }
            }
        }
    }

    return result;
}

// 处理循环语句
wstring process_loop_statement(const wstring& line) {
    wstring result = line;

    if (result.find(L"循环") == 0 && result.find(L"循环w") == wstring::npos) {
        size_t loop_pos = result.find(L"循环");
        wstring content = result.substr(loop_pos + 2);
        size_t first = content.find_first_not_of(L" ");
        if (first != wstring::npos) content = content.substr(first);

        if (content.find(L"在范围内") != wstring::npos) {
            wregex pattern(L"(\\w+)\\s+在范围内\\((.*)\\)");
            wsmatch match;
            if (regex_search(content, match, pattern) && match.size() > 2) {
                wstring range_content = match[2].str();
                // 检查range函数的括号
                int balance = count_parentheses(range_content);
                if (balance > 0) {
                    range_content += wstring(balance, L')');
                }
                result = L"for " + match[1].str() + L" in range(" + range_content + L"):";
            }
        }
        else if (content.find(L"在") != wstring::npos) {
            wregex pattern(L"(\\w+)\\s+在\\s+(.+)");
            wsmatch match;
            if (regex_search(content, match, pattern) && match.size() > 2) {
                result = L"for " + match[1].str() + L" in " + match[2].str() + L":";
            }
        }
        else {
            result = L"for " + content + L":";
        }
    }

    if (result.find(L"循环w") == 0) {
        size_t while_pos = result.find(L"循环w");
        wstring condition = result.substr(while_pos + 3);
        size_t first = condition.find_first_not_of(L" ");
        if (first != wstring::npos) condition = condition.substr(first);

        // 检查条件中的括号平衡
        int balance = count_parentheses(condition);
        if (balance > 0) {
            condition += wstring(balance, L')');
        }

        result = L"while " + condition + L":";
    }

    return result;
}

// 处理条件语句
wstring process_condition_statement(const wstring& line) {
    wstring result = line;

    if (result.find(L"如果") == 0) {
        size_t if_pos = result.find(L"如果");
        wstring condition = result.substr(if_pos + 2);
        size_t first = condition.find_first_not_of(L" ");
        if (first != wstring::npos) condition = condition.substr(first);

        // 检查条件中的括号平衡
        int balance = count_parentheses(condition);
        if (balance > 0) {
            condition += wstring(balance, L')');
        }

        result = L"if " + condition + L":";
    }
    else if (result.find(L"否则如果") == 0) {
        size_t elif_pos = result.find(L"否则如果");
        wstring condition = result.substr(elif_pos + 4);
        size_t first = condition.find_first_not_of(L" ");
        if (first != wstring::npos) condition = condition.substr(first);

        // 检查条件中的括号平衡
        int balance = count_parentheses(condition);
        if (balance > 0) {
            condition += wstring(balance, L')');
        }

        result = L"elif " + condition + L":";
    }
    else if (result.find(L"否则") == 0) {
        result = L"else:";
    }

    return result;
}

// 处理函数定义
wstring process_function_statement(const wstring& line) {
    wstring result = line;

    if (result.find(L"定义") == 0) {
        size_t def_pos = result.find(L"定义");
        wstring content = result.substr(def_pos + 2);
        size_t first = content.find_first_not_of(L" ");
        if (first != wstring::npos) content = content.substr(first);

        // 检查函数参数中的括号平衡
        int balance = count_parentheses(content);
        if (balance > 0) {
            content += wstring(balance, L')');
        }
        else if (balance < 0) {
            // 如果右括号比左括号多，添加缺失的左括号
            content = wstring(-balance, L'(') + content;
        }

        result = L"def " + content + L":";
    }

    return result;
}

// 主要替换函数
void replace_code(wstring& text, const map<wstring, wstring>& Map) {
    // 先处理特殊语句
    if (text.find(L"输出") == 0) {
        text = process_output_statement(text);
    }
    else if (text.find(L"输入") == 0) {
        text = process_input_statement(text);
    }
    else if (text.find(L"循环") == 0 || text.find(L"循环w") == 0) {
        text = process_loop_statement(text);
    }
    else if (text.find(L"如果") == 0 || text.find(L"否则") == 0) {
        text = process_condition_statement(text);
    }
    else if (text.find(L"定义") == 0) {
        text = process_function_statement(text);
    }

    // 然后进行词汇替换
    for (auto& pair : Map) {
        size_t pos = 0;
        while ((pos = text.find(pair.first, pos)) != wstring::npos) {
            bool is_whole_word = true;
            if (pos > 0) {
                wchar_t prev_char = text[pos - 1];
                if (iswalnum(prev_char) || prev_char == L'_') is_whole_word = false;
            }
            if (pos + pair.first.length() < text.length()) {
                wchar_t next_char = text[pos + pair.first.length()];
                if (iswalnum(next_char) || next_char == L'_') is_whole_word = false;
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

    // 修复常见问题
    size_t pos = text.find(L"::");
    if (pos != wstring::npos) text.replace(pos, 2, L":");

    pos = text.find(L"字符串(");
    while (pos != wstring::npos) {
        text.replace(pos, 4, L"str(");
        pos = text.find(L"字符串(", pos + 4);
    }

    // 修复else: if为elif
    pos = text.find(L"else: if");
    if (pos != wstring::npos) {
        text.replace(pos, 8, L"elif");
    }

    // 修复input(())为input()
    pos = text.find(L"input(())");
    if (pos != wstring::npos) {
        text.replace(pos, 9, L"input()");
    }

    // 最后检查整行的括号平衡
    int final_balance = count_parentheses(text);
    if (final_balance > 0) {
        text += wstring(final_balance, L')');
    }
}

wstring read_document(const wstring& filename) {
    wifstream file(filename, ios::binary);
    if (!file.is_open()) return L"";
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    wstringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 计算原始行的缩进级别
int calculate_indent_level(const wstring& line) {
    int spaces = 0;
    for (wchar_t c : line) {
        if (c == L' ') spaces++;
        else if (c == L'\t') spaces += 4;
        else break;
    }
    return spaces / 4;
}

// 修复语法错误 - 彻底修复括号问题
string fix_syntax_errors(const string& line) {
    string result = line;

    // 修复input(())为input()
    size_t pos = result.find("input(())");
    if (pos != string::npos) {
        result.replace(pos, 9, "input()");
    }

    // 修复input(()为input()
    pos = result.find("input(()");
    if (pos != string::npos) {
        result.replace(pos, 8, "input()");
    }

    // 修复input( 后面有内容的情况
    pos = result.find("input(");
    if (pos != string::npos) {
        size_t end_pos = result.find("))", pos);
        if (end_pos != string::npos) {
            // 检查是否是input(())的情况
            if (result.substr(pos, end_pos - pos + 2) == "input(())") {
                result.replace(pos, 9, "input()");
            }
        }
    }

    // 修复print语句的括号
    if (result.find("print") == 0) {
        // 计算括号是否匹配
        int open_parens = 0;
        int close_parens = 0;
        bool in_quotes = false;
        char quote_char = 0;

        for (size_t i = 0; i < result.length(); i++) {
            char c = result[i];

            // 处理引号内的内容
            if (c == '"' || c == '\'') {
                if (!in_quotes) {
                    in_quotes = true;
                    quote_char = c;
                }
                else if (c == quote_char) {
                    // 检查是否是转义引号
                    if (i > 0 && result[i - 1] == '\\') {
                        continue; // 转义引号，不结束引号区域
                    }
                    in_quotes = false;
                }
            }

            if (!in_quotes) {
                if (c == '(') open_parens++;
                else if (c == ')') close_parens++;
            }
        }

        // 如果左括号比右括号多，添加右括号
        if (open_parens > close_parens) {
            result += string(open_parens - close_parens, ')');
        }

        // 确保print语句以右括号结尾（如果没有分号等其他内容）
        if (result.find(";") == string::npos && open_parens > 0 && result.back() != ')') {
            result += ")";
        }
    }

    // 修复input语句的括号
    if (result.find("input") == 0) {
        int open_parens = 0;
        int close_parens = 0;
        for (char c : result) {
            if (c == '(') open_parens++;
            else if (c == ')') close_parens++;
        }

        if (open_parens > close_parens) {
            result += string(open_parens - close_parens, ')');
        }

        // 如果input后面没有参数，确保是input()
        if (result == "input" || result.find("input(") == 0 && result.length() == 6) {
            result = "input()";
        }
    }

    // 修复else: if 为 elif
    pos = result.find("else: if");
    if (pos != string::npos) {
        result.replace(pos, 8, "elif");
    }

    // 修复strstr为str
    pos = result.find("strstr");
    while (pos != string::npos) {
        result.replace(pos, 6, "str");
        pos = result.find("strstr", pos + 3);
    }

    return result;
}

// 生成Python程序
string generate_python_program(const vector<wstring>& original_lines, const vector<wstring>& converted_lines) {
    stringstream program;
    program << "# -*- coding: utf-8 -*-\n\n";

    for (size_t i = 0; i < converted_lines.size(); i++) {
        int indent_level = calculate_indent_level(original_lines[i]);
        string indent(indent_level * 4, ' ');

        string line = wstring_to_string(converted_lines[i]);
        line = fix_syntax_errors(line);

        program << indent << line << "\n";
    }

    return program.str();
}

bool save_to_file(const string& content, const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return false;
    file << "\xEF\xBB\xBF" << content;
    file.close();
    return true;
}

void display_file_content(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    char bom[3];
    file.read(bom, 3);
    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') file.seekg(0);

    string line;
    wcout << wgreen(L"\n=== 生成的Python代码 ===") << endl;
    while (getline(file, line)) cout << line << endl;
    wcout << wgreen(L"=== 转换结束 ===") << endl;
    file.close();
}

bool file_exists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    string input_filename = "code.code";
    wcout.imbue(locale("en_US.UTF-8"));
    wcout << wgreen(L"=== 中文代码编译器 ===") << endl;
    string first_arg = "";
    if (argc > 1) {
        first_arg = argv[1];
    }
    if (first_arg == "") {
        wcout << L"请输入要编译文件例(code.code)：";
        getline(cin, input_filename);
    }
    else input_filename = first_arg;
    if (!file_exists(input_filename)) {
        wcout << wred(L"错误：文件不存在: ") << string_to_wstring(input_filename) << endl;
        wcout << L"\n按Enter退出...";
        cin.get();
    }

    wstring wtext = read_document(string_to_wstring(input_filename));
    if (wtext.empty()) {
        wcerr << wred(L"错误：无法读取文件或文件为空") << endl;
        wcout << L"\n按Enter退出...";
        cin.get();
    }

    wcout << wgreen(L"\n1. 原始代码:") << endl;
    wcout << wtext << endl;

    // 读取原始行
    vector<wstring> original_lines;
    wistringstream wstream(wtext);
    wstring line;
    while (getline(wstream, line)) {
        if (!line.empty()) {
            line.erase(remove(line.begin(), line.end(), L'\r'), line.end());
            original_lines.push_back(line);
        }
    }

    // 转换每一行
    vector<wstring> converted_lines;
    for (auto& orig_line : original_lines) {
        wstring converted = orig_line;
        replace_code(converted, Map);
        converted_lines.push_back(converted);
    }

    wcout << wgreen(L"\n2. 转换后代码:") << endl;
    for (size_t i = 0; i < converted_lines.size(); i++) {
        wcout << L"   " << converted_lines[i] << endl;
    }

    string py_filename = "code.py";
    string python_code = generate_python_program(original_lines, converted_lines);

    if (save_to_file(python_code, py_filename)) {
        wcout << wgreen(L"\n3. Python程序已保存到: ") << string_to_wstring(py_filename) << endl;
        display_file_content(py_filename);

        wcout << wblue(L"\n是否运行Python程序？(y/n): ");
        string choice;
        getline(cin, choice);

        if (choice == "y" || choice == "Y") {
            wcout << wgreen(L"\n运行程序...") << endl;
            wcout << L"=== 程序输出 ===" << endl;

            string cmd = "python \"" + py_filename + "\"";
            int result = system(cmd.c_str());

            if (result != 0) {
                cmd = "python3 \"" + py_filename + "\"";
                result = system(cmd.c_str());
            }

            wcout << L"=== 程序结束 ===" << endl;
        }
    }

    wcout << L"\n按Enter退出...";
    cin.get();
    return 0;
}