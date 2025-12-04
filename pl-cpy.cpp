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

// 默认映射关系
map<wstring, wstring> Map = {
    // 基本输入输出
    {L"输出", L"print("},
    {L"输入", L"input("},

    // 控制流
    {L"如果", L"if "},
    {L"否则", L"else:"},
    {L"否则如果", L"elif "},
    {L"循环", L"for "},
    {L"循环w", L"while "},

    // 函数相关
    {L"定义", L"def "},
    {L"返回", L"return "},

    // 布尔值
    {L"是", L"True"},
    {L"否", L"False"},
    {L"且", L" and "},
    {L"或", L" or "},

    // 数据类型
    {L"字符串", L"str"},
    {L"长度", L"len"},

    // 变量声明和赋值
    {L"让", L""},  // 让 x = 5 → x = 5
    {L"设", L""},  // 设 x = 5 → x = 5
    {L"变量", L""}, // 变量 x = 5 → x = 5
    {L"等于", L"="},
    {L"为", L"="},  // x 为 5 → x = 5

    // 删除语句
    {L"删除", L"del "},
    {L"移除变量", L"del "},

    // 导入语句
    {L"导入", L"import "},
    {L"引入", L"import "},
    {L"作为", L" as "},
    {L"从", L"from "},

    // 断言语句
    {L"断言", L"assert "},
    {L"确保", L"assert "},

    // 全局变量
    {L"全局", L"global "},
    {L"全局变量", L"global "},

    // 非局部变量
    {L"非局部", L"nonlocal "},
    {L"外部变量", L"nonlocal "},

    // 异常处理
    {L"尝试", L"try:"},
    {L"捕获", L"except"},
    {L"异常", L"except"},
    {L"最后", L"finally:"},
    {L"抛出", L"raise "},
    {L"引发", L"raise "},

    // 循环控制
    {L"跳出", L"break"},
    {L"终止循环", L"break"},
    {L"继续", L"continue"},
    {L"跳过本次", L"continue"},
    {L"通过", L"pass"},
    {L"空操作", L"pass"},

    // with语句
    {L"用", L"with "},
    {L"使用", L"with "},

    // 执行语句
    {L"执行", L"exec("},
    {L"评估", L"eval("},

    // 打印到文件
    {L"打印到", L"print("},
    {L"输出到文件", L"print("},

    // 文件操作
    {L"打开文件", L"open("},
    {L"关闭文件", L".close()"},
    {L"读取文件", L".read()"},
    {L"写入文件", L".write("},

    // 类相关
    {L"类", L"class "},
    {L"继承", L"("},
    {L"自", L"self"},
    {L"自身", L"self"},
    {L"初始化", L"__init__"},
    {L"构造", L"__init__"},
    {L"析构", L"__del__"},

    // 特殊方法
    {L"字符串化", L"__str__"},
    {L"表示", L"__repr__"},
    {L"调用", L"__call__"},
    {L"获取属性", L"__getattr__"},
    {L"设置属性", L"__setattr__"},

    // 属性装饰器
    {L"属性", L"@property"},
    {L"静态方法", L"@staticmethod"},
    {L"类方法", L"@classmethod"},

    // 异步编程
    {L"异步", L"async "},
    {L"等待", L"await "},

    // 生成器
    {L"生成器", L"yield "},
    {L"产出", L"yield "},

    // 调试语句
    {L"调试", L"breakpoint()"},
    {L"设置断点", L"breakpoint()"},

    // 退出语句
    {L"退出", L"exit()"},
    {L"退出程序", L"exit()"},
    {L"终止", L"quit()"},

    // 编译语句
    {L"编译", L"compile("},

    // 内存查看
    {L"内存视图", L"memoryview("},
    {L"内存大小", L"getsizeof("},

    // 哈希和ID
    {L"哈希值", L"hash("},
    {L"标识", L"id("},

    // 迭代器
    {L"迭代", L"iter("},
    {L"下一个", L"next("},

    // 切片操作
    {L"切片", L"slice("},

    // 静态方法
    {L"静态", L"staticmethod("},

    // 类方法
    {L"类方法", L"classmethod("},

    // 超类
    {L"超类", L"super("},
    {L"父类", L"super("},

    // 对象检查
    {L"是实例", L"isinstance("},
    {L"是子类", L"issubclass("},
    {L"有属性", L"hasattr("},
    {L"获取属性值", L"getattr("},
    {L"设置属性值", L"setattr("},
    {L"删除属性", L"delattr("},

    // 变量检查
    {L"局部变量", L"locals()"},
    {L"全局变量表", L"globals()"},
    {L"变量字典", L"vars("},

    // 模块操作
    {L"重载模块", L"reload("},
    {L"模块属性", L"__import__("},

    // 输入输出重定向
    {L"输入重定向", L"input = "},
    {L"输出重定向", L"print = "},

    // 系统操作
    {L"系统调用", L"system("},
    {L"执行命令", L"popen("},

    // 内存操作
    {L"垃圾回收", L"gc.collect()"},
    {L"引用计数", L"sys.getrefcount("},

    // 序列化
    {L"序列化", L"pickle.dumps("},
    {L"反序列化", L"pickle.loads("},
    {L"转储", L"pickle.dump("},
    {L"加载", L"pickle.load("},

    // JSON操作
    {L"转JSON", L"json.dumps("},
    {L"解析JSON", L"json.loads("},

    // 时间操作
    {L"当前时间", L"time.time()"},
    {L"睡眠", L"time.sleep("},
    {L"日期时间", L"datetime.now()"},

    // 随机数
    {L"随机数", L"random.random()"},
    {L"随机整数", L"random.randint("},
    {L"随机选择", L"random.choice("},

    // 数学运算
    {L"绝对值", L"abs("},
    {L"四舍五入", L"round("},
    {L"最大值", L"max("},
    {L"最小值", L"min("},
    {L"求和", L"sum("},
    {L"幂运算", L"pow("},
    {L"除数余数", L"divmod("},

    // 类型转换
    {L"转整数", L"int("},
    {L"转浮点数", L"float("},
    {L"转字符串", L"str("},
    {L"转列表", L"list("},
    {L"转元组", L"tuple("},
    {L"转字典", L"dict("},
    {L"转集合", L"set("},
    {L"转布尔", L"bool("},
    {L"转字节", L"bytes("},

    // 进制转换
    {L"二进制", L"bin("},
    {L"八进制", L"oct("},
    {L"十六进制", L"hex("},
    {L"ASCII字符", L"chr("},
    {L"ASCII码", L"ord("},

    // 面向对象检查
    {L"是类", L"isclass("},
    {L"是函数", L"isfunction("},
    {L"是方法", L"ismethod("},
    {L"可调用", L"callable("},

    // 文档字符串
    {L"文档", L"__doc__"},
    {L"帮助", L"help("},

    // 版权信息
    {L"版权", L"__copyright__"},
    {L"作者", L"__author__"},
    {L"版本", L"__version__"},

    // 文件路径
    {L"文件路径", L"__file__"},
    {L"模块名", L"__name__"},

    // 包相关
    {L"包路径", L"__path__"},
    {L"包名", L"__package__"},

    // 注解相关
    {L"注解", L"__annotations__"},

    // 未来特性
    {L"未来", L"__future__"},

    // 警告控制
    {L"忽略警告", L"warnings.filterwarnings('ignore')"},
    {L"发出警告", L"warnings.warn("},

    // 日志记录
    {L"日志", L"logging.info("},
    {L"调试日志", L"logging.debug("},
    {L"警告日志", L"logging.warning("},
    {L"错误日志", L"logging.error("},

    // 测试相关
    {L"测试", L"unittest.main()"},
    {L"断言相等", L"assertEqual("},
    {L"断言为真", L"assertTrue("},
    {L"断言为假", L"assertFalse("},
    {L"断言抛出", L"assertRaises("},

    // 性能分析
    {L"性能分析", L"cProfile.run("},
    {L"时间分析", L"timeit.timeit("},

    // 内存分析
    {L"内存分析", L"tracemalloc.start()"},

    // 并发编程
    {L"线程", L"threading.Thread("},
    {L"进程", L"multiprocessing.Process("},
    {L"锁", L"threading.Lock()"},
    {L"信号量", L"threading.Semaphore("},

    // 网络编程
    {L"套接字", L"socket.socket("},
    {L"连接", L".connect("},
    {L"绑定", L".bind("},
    {L"监听", L".listen("},
    {L"接受连接", L".accept("},
    {L"发送数据", L".send("},
    {L"接收数据", L".recv("}
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

// 检查文件是否存在
bool file_exists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// 从JSON文件加载映射关系
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
    while ((pos = result.find(L"「", pos)) != wstring::npos) {
        result.replace(pos, 1, L"\"");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find(L"」", pos)) != wstring::npos) {
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
    else if (text.find(L"如果") == 0 || text.find(L"否则如果") == 0 || text.find(L"否则") == 0) {
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

int main(int argc, char* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    string input_filename = "code.code";
    wcout.imbue(locale("en_US.UTF-8"));
    wcout << wgreen(L"=== 中文代码编译器 ===") << endl;

    // 首先尝试加载ctopy.json配置文件
    wcout << wblue(L"正在检查配置文件 ctopy.json...") << endl;
    load_mappings_from_json("ctopy.json", Map);

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
        return 1;
    }

    wstring wtext = read_document(string_to_wstring(input_filename));
    if (wtext.empty()) {
        wcerr << wred(L"错误：无法读取文件或文件为空") << endl;
        wcout << L"\n按Enter退出...";
        cin.get();
        return 1;
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
    else {
        wcerr << wred(L"错误：保存Python文件失败") << endl;
        wcout << L"\n按Enter退出...";
        cin.get();
        return 1;
    }

    wcout << L"\n按Enter退出...";
    cin.get();
    return 0;
}
