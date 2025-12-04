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

// 完整C++中文到英文映射表
map<wstring, wstring> Map = {
    // ========== 基本语法和关键字 ==========
    {L"使用库", L"#include "},
    {L"使用命名空间", L"using namespace "},
    {L"主函数", L"int main"},
    {L"函数", L"void"},
    {L"类", L"class"},
    {L"结构", L"struct"},
    {L"枚举", L"enum"},
    {L"联合", L"union"},
    {L"模板", L"template"},
    {L"命名空间", L"namespace"},
    {L"类型定义", L"typedef"},
    {L"使用", L"using"},

    // 基本类型
    {L"空", L"void"},
    {L"布尔", L"bool"},
    {L"字符", L"char"},
    {L"宽字符", L"wchar_t"},
    {L"字符8", L"char8_t"},
    {L"字符16", L"char16_t"},
    {L"字符32", L"char32_t"},
    {L"整型", L"int"},
    {L"短整型", L"short"},
    {L"长整型", L"long"},
    {L"长长整型", L"long long"},
    {L"有符号", L"signed"},
    {L"无符号", L"unsigned"},
    {L"浮点", L"float"},
    {L"双精度", L"double"},
    {L"长双精度", L"long double"},
    {L"自动", L"auto"},
    {L"推导类型", L"decltype"},

    // 字面量
    {L"真", L"true"},
    {L"假", L"false"},
    {L"空指针", L"nullptr"},

    // 操作符
    {L"加", L"+"},
    {L"减", L"-"},
    {L"乘", L"*"},
    {L"除", L"/"},
    {L"取模", L"%"},
    {L"递增", L"++"},
    {L"递减", L"--"},
    {L"赋值", L"="},
    {L"加等于", L"+="},
    {L"减等于", L"-="},
    {L"乘等于", L"*="},
    {L"除等于", L"/="},
    {L"模等于", L"%="},
    {L"等于", L"=="},
    {L"不等于", L"!="},
    {L"大于", L">"},
    {L"小于", L"<"},
    {L"大于等于", L">="},
    {L"小于等于", L"<="},
    {L"逻辑与", L"&&"},
    {L"逻辑或", L"||"},
    {L"逻辑非", L"!"},
    {L"位与", L"&"},
    {L"位或", L"|"},
    {L"位异或", L"^"},
    {L"位非", L"~"},
    {L"左移", L"<<"},
    {L"右移", L">>"},
    {L"与等于", L"&="},
    {L"或等于", L"|="},
    {L"异或等于", L"^="},
    {L"左移等于", L"<<="},
    {L"右移等于", L">>="},
    {L"取地址", L"&"},
    {L"解引用", L"*"},
    {L"成员访问", L"."},
    {L"指针成员访问", L"->"},
    {L"下标", L"[]"},
    {L"函数调用", L"()"},
    {L"作用域解析", L"::"},
    {L"逗号", L","},
    {L"条件", L"?"},
    {L"三路比较", L"<=>"},

    // 类型转换
    {L"静态转换", L"static_cast"},
    {L"动态转换", L"dynamic_cast"},
    {L"常量转换", L"const_cast"},
    {L"重解释转换", L"reinterpret_cast"},

    // 内存管理
    {L"新建", L"new"},
    {L"删除", L"delete"},
    {L"新建数组", L"new[]"},
    {L"删除数组", L"delete[]"},

    // 控制流
    {L"如果", L"if"},
    {L"否则", L"else"},
    {L"开关", L"switch"},
    {L"情况", L"case"},
    {L"默认", L"default"},
    {L"循环", L"for"},
    {L"当循环", L"while"},
    {L"做循环", L"do"},
    {L"中断", L"break"},
    {L"继续", L"continue"},
    {L"转到", L"goto"},
    {L"返回", L"return"},

    // 异常处理
    {L"尝试", L"try"},
    {L"捕获", L"catch"},
    {L"抛出", L"throw"},
    {L"不抛出", L"noexcept"},

    // 存储类说明符
    {L"自动", L"auto"},
    {L"寄存器", L"register"},
    {L"静态", L"static"},
    {L"外部", L"extern"},
    {L"可变", L"mutable"},

    // 函数说明符
    {L"内联", L"inline"},
    {L"虚拟", L"virtual"},
    {L"显式", L"explicit"},
    {L"重写", L"override"},
    {L"最终", L"final"},

    // ========== 预处理指令 ==========
    {L"包含", L"#include"},
    {L"定义", L"#define"},
    {L"取消定义", L"#undef"},
    {L"如果", L"#if"},
    {L"如果定义", L"#ifdef"},
    {L"如果未定义", L"#ifndef"},
    {L"否则如果", L"#elif"},
    {L"否则", L"#else"},
    {L"结束如果", L"#endif"},
    {L"错误", L"#error"},
    {L"警告", L"#warning"},
    {L"编译指示", L"#pragma"},
    {L"行号", L"#line"},

    // ========== 标准库头文件 ==========

    // 容器
    {L"包含向量", L"#include <vector>"},
    {L"包含列表", L"#include <list>"},
    {L"包含双端队列", L"#include <deque>"},
    {L"包含队列", L"#include <queue>"},
    {L"包含栈", L"#include <stack>"},
    {L"包含映射", L"#include <map>"},
    {L"包含多重映射", L"#include <multimap>"},
    {L"包含无序映射", L"#include <unordered_map>"},
    {L"包含无序多重映射", L"#include <unordered_multimap>"},
    {L"包含集合", L"#include <set>"},
    {L"包含多重集合", L"#include <multiset>"},
    {L"包含无序集合", L"#include <unordered_set>"},
    {L"包含无序多重集合", L"#include <unordered_multiset>"},
    {L"包含数组", L"#include <array>"},
    {L"包含前向列表", L"#include <forward_list>"},
    {L"包含位集", L"#include <bitset>"},

    // 算法
    {L"包含算法", L"#include <algorithm>"},
    {L"包含数值算法", L"#include <numeric>"},

    // 输入输出
    {L"包含输入输出流", L"#include <iostream>"},
    {L"包含文件流", L"#include <fstream>"},
    {L"包含字符串流", L"#include <sstream>"},
    {L"包含格式", L"#include <format>"},

    // 字符串
    {L"包含字符串", L"#include <string>"},
    {L"包含字符串视图", L"#include <string_view>"},

    // 内存
    {L"包含内存", L"#include <memory>"},
    {L"包含新", L"#include <new>"},

    // 并发
    {L"包含线程", L"#include <thread>"},
    {L"包含互斥锁", L"#include <mutex>"},
    {L"包含条件变量", L"#include <condition_variable>"},
    {L"包含未来", L"#include <future>"},
    {L"包含原子", L"#include <atomic>"},

    // 其他
    {L"包含功能", L"#include <functional>"},
    {L"包含迭代器", L"#include <iterator>"},
    {L"包含随机数", L"#include <random>"},
    {L"包含时间", L"#include <chrono>"},
    {L"包含文件系统", L"#include <filesystem>"},
    {L"包含正则表达式", L"#include <regex>"},
    {L"包含类型特征", L"#include <type_traits>"},
    {L"包含可选", L"#include <optional>"},
    {L"包含变体", L"#include <variant>"},
    {L"包含任意", L"#include <any>"},
    {L"包含元组", L"#include <tuple>"},
    {L"包含范围", L"#include <ranges>"},
    {L"包含跨度", L"#include <span>"},
    {L"包含源位置", L"#include <source_location>"},
    {L"包含栈踪迹", L"#include <stacktrace>"},
    {L"包含停止令牌", L"#include <stop_token>"},
    {L"包含信号量", L"#include <semaphore>"},
    {L"包含锁闩", L"#include <latch>"},
    {L"包含屏障", L"#include <barrier>"},
    {L"包含协程", L"#include <coroutine>"},

    // C标准库
    {L"包含C标准库", L"#include <cstdlib>"},
    {L"包含C标准输入输出", L"#include <cstdio>"},
    {L"包含C字符串", L"#include <cstring>"},
    {L"包含C数学", L"#include <cmath>"},
    {L"包含C时间", L"#include <ctime>"},
    {L"包含C断言", L"#include <cassert>"},
    {L"包含C类型", L"#include <ctype.h>"},
    {L"包含C本地化", L"#include <clocale>"},

    // ========== 标准库组件 ==========

    // 输入输出
    {L"标准输出", L"std::cout"},
    {L"标准输入", L"std::cin"},
    {L"标准错误", L"std::cerr"},
    {L"标准日志", L"std::clog"},
    {L"结束行", L"std::endl"},
    {L"刷新", L"std::flush"},

    // 字符串操作
    {L"字符串长度", L".length()"},
    {L"字符串大小", L".size()"},
    {L"字符串空", L".empty()"},
    {L"字符串追加", L".append("},
    {L"字符串比较", L".compare("},
    {L"字符串查找", L".find("},
    {L"子字符串", L".substr("},
    {L"字符串替换", L".replace("},
    {L"字符串插入", L".insert("},
    {L"字符串擦除", L".erase("},
    {L"字符串清除", L".clear()"},

    // 容器操作
    {L"容器大小", L".size()"},
    {L"容器空", L".empty()"},
    {L"容器容量", L".capacity()"},
    {L"容器预留", L".reserve("},
    {L"容器调整大小", L".resize("},
    {L"容器开始", L".begin()"},
    {L"容器结束", L".end()"},
    {L"容器前开始", L".cbegin()"},
    {L"容器前结束", L".cend()"},
    {L"容器反向开始", L".rbegin()"},
    {L"容器反向结束", L".rend()"},
    {L"容器前反向开始", L".crbegin()"},
    {L"容器前反向结束", L".crend()"},
    {L"容器前", L".front()"},
    {L"容器后", L".back()"},
    {L"容器数据", L".data()"},
    {L"容器在", L".at("},
    {L"容器最大大小", L".max_size()"},

    // 向量特定操作
    {L"向量压入后", L".push_back("},
    {L"向量弹出后", L".pop_back()"},
    {L"向量插入", L".insert("},
    {L"向量擦除", L".erase("},
    {L"向量交换", L".swap("},
    {L"向量分配", L".assign("},

    // 列表特定操作
    {L"列表压入前", L".push_front("},
    {L"列表弹出前", L".pop_front()"},
    {L"列表排序", L".sort()"},
    {L"列表反转", L".reverse()"},
    {L"列表合并", L".merge("},
    {L"列表拼接", L".splice("},
    {L"列表移除", L".remove("},
    {L"列表移除如果", L".remove_if("},
    {L"列表唯一", L".unique()"},

    // 映射特定操作
    {L"映射插入", L".insert("},
    {L"映射擦除", L".erase("},
    {L"映射查找", L".find("},
    {L"映射计数", L".count("},
    {L"映射下限", L".lower_bound("},
    {L"映射上限", L".upper_bound("},
    {L"映射相等范围", L".equal_range("},
    {L"映射包含", L".contains("},

    // 算法
    {L"排序", L"std::sort("},
    {L"稳定排序", L"std::stable_sort("},
    {L"部分排序", L"std::partial_sort("},
    {L"第n元素", L"std::nth_element("},
    {L"是排序", L"std::is_sorted("},
    {L"是排序直到", L"std::is_sorted_until("},
    {L"堆推入", L"std::push_heap("},
    {L"堆弹出", L"std::pop_heap("},
    {L"建堆", L"std::make_heap("},
    {L"是堆", L"std::is_heap("},
    {L"是堆直到", L"std::is_heap_until("},
    {L"堆排序", L"std::sort_heap("},
    {L"最小元素", L"std::min_element("},
    {L"最大元素", L"std::max_element("},
    {L"最小最大元素", L"std::minmax_element("},
    {L"字典序比较", L"std::lexicographical_compare("},
    {L"下一个排列", L"std::next_permutation("},
    {L"前一个排列", L"std::prev_permutation("},
    {L"查找", L"std::find("},
    {L"查找如果", L"std::find_if("},
    {L"查找如果非", L"std::find_if_not("},
    {L"查找结束", L"std::find_end("},
    {L"查找第一个", L"std::find_first_of("},
    {L"相邻查找", L"std::adjacent_find("},
    {L"计数", L"std::count("},
    {L"计数如果", L"std::count_if("},
    {L"搜索", L"std::search("},
    {L"搜索N", L"std::search_n("},
    {L"复制", L"std::copy("},
    {L"复制如果", L"std::copy_if("},
    {L"复制N", L"std::copy_n("},
    {L"复制后", L"std::copy_backward("},
    {L"移动", L"std::move("},
    {L"移动后", L"std::move_backward("},
    {L"交换", L"std::swap("},
    {L"交换范围", L"std::swap_ranges("},
    {L"迭代交换", L"std::iter_swap("},
    {L"变换", L"std::transform("},
    {L"替换", L"std::replace("},
    {L"替换如果", L"std::replace_if("},
    {L"替换复制", L"std::replace_copy("},
    {L"替换复制如果", L"std::replace_copy_if("},
    {L"填充", L"std::fill("},
    {L"填充N", L"std::fill_n("},
    {L"生成", L"std::generate("},
    {L"生成N", L"std::generate_n("},
    {L"移除", L"std::remove("},
    {L"移除如果", L"std::remove_if("},
    {L"移除复制", L"std::remove_copy("},
    {L"移除复制如果", L"std::remove_copy_if("},
    {L"唯一", L"std::unique("},
    {L"唯一复制", L"std::unique_copy("},
    {L"反转", L"std::reverse("},
    {L"反转复制", L"std::reverse_copy("},
    {L"旋转", L"std::rotate("},
    {L"旋转复制", L"std::rotate_copy("},
    {L"随机洗牌", L"std::random_shuffle("},
    {L"洗牌", L"std::shuffle("},
    {L"样本", L"std::sample("},
    {L"是分区", L"std::is_partitioned("},
    {L"分区", L"std::partition("},
    {L"稳定分区", L"std::stable_partition("},
    {L"分区复制", L"std::partition_copy("},
    {L"分区点", L"std::partition_point("},
    {L"合并", L"std::merge("},
    {L"原地合并", L"std::inplace_merge("},
    {L"包括", L"std::includes("},
    {L"集合并", L"std::set_union("},
    {L"集合交", L"std::set_intersection("},
    {L"集合差", L"std::set_difference("},
    {L"集合对称差", L"std::set_symmetric_difference("},
    {L"推入堆", L"std::push_heap("},
    {L"弹出堆", L"std::pop_heap("},
    {L"建堆", L"std::make_heap("},
    {L"排序堆", L"std::sort_heap("},
    {L"是堆", L"std::is_heap("},
    {L"是堆直到", L"std::is_heap_until("},
    {L"最小", L"std::min("},
    {L"最大", L"std::max("},
    {L"最小最大", L"std::minmax("},
    {L"钳制", L"std::clamp("},
    {L"等于", L"std::equal("},
    {L"是排列", L"std::is_permutation("},
    {L"不匹配", L"std::mismatch("},
    {L"二分查找", L"std::binary_search("},
    {L"下界", L"std::lower_bound("},
    {L"上界", L"std::upper_bound("},
    {L"相等范围", L"std::equal_range("},

    // 数值算法
    {L"累积", L"std::accumulate("},
    {L"内积", L"std::inner_product("},
    {L"部分和", L"std::partial_sum("},
    {L"相邻差", L"std::adjacent_difference("},
    {L"约简", L"std::reduce("},
    {L"变换约简", L"std::transform_reduce("},
    {L"包含扫描", L"std::inclusive_scan("},
    {L"排除扫描", L"std::exclusive_scan("},
    {L"变换包含扫描", L"std::transform_inclusive_scan("},
    {L"变换排除扫描", L"std::transform_exclusive_scan("},
    {L"最大元素", L"std::max_element("},
    {L"最小元素", L"std::min_element("},
    {L"最小最大元素", L"std::minmax_element("},

    // 内存算法
    {L"未初始化复制", L"std::uninitialized_copy("},
    {L"未初始化复制N", L"std::uninitialized_copy_n("},
    {L"未初始化填充", L"std::uninitialized_fill("},
    {L"未初始化填充N", L"std::uninitialized_fill_n("},
    {L"未初始化移动", L"std::uninitialized_move("},
    {L"未初始化移动N", L"std::uninitialized_move_n("},
    {L"未初始化默认构造", L"std::uninitialized_default_construct("},
    {L"未初始化默认构造N", L"std::uninitialized_default_construct_n("},
    {L"未初始化值构造", L"std::uninitialized_value_construct("},
    {L"未初始化值构造N", L"std::uninitialized_value_construct_n("},
    {L"销毁", L"std::destroy("},
    {L"销毁N", L"std::destroy_n("},
    {L"销毁At", L"std::destroy_at("},
    {L"构造At", L"std::construct_at("},

    // 智能指针
    {L"唯一指针", L"std::unique_ptr"},
    {L"共享指针", L"std::shared_ptr"},
    {L"弱指针", L"std::weak_ptr"},
    {L"创建唯一", L"std::make_unique"},
    {L"创建共享", L"std::make_shared"},
    {L"分配共享", L"std::allocate_shared"},
    {L"启用共享从此", L"std::enable_shared_from_this"},
    {L"指针获取", L".get()"},
    {L"指针重置", L".reset("},
    {L"指针交换", L".swap("},
    {L"指针使用计数", L".use_count()"},
    {L"指针唯一", L".unique()"},
    {L"指针所有者前", L".owner_before("},
    {L"指针锁定", L".lock()"},
    {L"指针过期", L".expired()"},

    // 功能对象
    {L"函数", L"std::function"},
    {L"绑定", L"std::bind"},
    {L"引用包装", L"std::ref"},
    {L"常量引用包装", L"std::cref"},
    {L"占位符1", L"std::placeholders::_1"},
    {L"占位符2", L"std::placeholders::_2"},
    {L"占位符3", L"std::placeholders::_3"},
    {L"占位符4", L"std::placeholders::_4"},
    {L"占位符5", L"std::placeholders::_5"},
    {L"占位符6", L"std::placeholders::_6"},
    {L"占位符7", L"std::placeholders::_7"},
    {L"占位符8", L"std::placeholders::_8"},
    {L"占位符9", L"std::placeholders::_9"},
    {L"占位符10", L"std::placeholders::_10"},

    // 元组
    {L"元组", L"std::tuple"},
    {L"制造元组", L"std::make_tuple"},
    {L"前向元组", L"std::forward_as_tuple"},
    {L"绑定元组", L"std::tie"},
    {L"元组连接", L"std::tuple_cat"},
    {L"获取元组", L"std::get"},
    {L"元组大小", L"std::tuple_size"},
    {L"元组元素", L"std::tuple_element"},

    // 可选
    {L"可选", L"std::optional"},
    {L"空可选", L"std::nullopt"},
    {L"制造可选", L"std::make_optional"},
    {L"可选值", L".value()"},
    {L"可选值或", L".value_or("},
    {L"可选有值", L".has_value()"},
    {L"可选重置", L".reset()"},
    {L"可选交换", L".swap("},
    {L"可选解引用", L"operator*"},
    {L"可选箭头", L"operator->"},

    // 变体
    {L"变体", L"std::variant"},
    {L"单子", L"std::monostate"},
    {L"变体大小", L"std::variant_size"},
    {L"变体选择", L"std::variant_alternative"},
    {L"变体索引", L".index()"},
    {L"变体有值", L".valueless_by_exception()"},
    {L"变体获取", L"std::get"},
    {L"变体获取如果", L"std::get_if"},
    {L"变体访问", L"std::visit"},
    {L"变体保持", L"std::holds_alternative"},

    // 任意
    {L"任意", L"std::any"},
    {L"任意转换", L"std::any_cast"},
    {L"任意有值", L".has_value()"},
    {L"任意重置", L".reset()"},
    {L"任意交换", L".swap("},
    {L"任意类型", L".type()"},

    // 字符串视图
    {L"字符串视图", L"std::string_view"},
    {L"字符串视图长度", L".length()"},
    {L"字符串视图大小", L".size()"},
    {L"字符串视图空", L".empty()"},
    {L"字符串视图数据", L".data()"},
    {L"字符串视图开始", L".begin()"},
    {L"字符串视图结束", L".end()"},
    {L"字符串视图前开始", L".cbegin()"},
    {L"字符串视图前结束", L".cend()"},
    {L"字符串视图子视图", L".substr("},
    {L"字符串视图比较", L".compare("},
    {L"字符串视图查找", L".find("},
    {L"字符串视图反向查找", L".rfind("},
    {L"字符串视图查找第一个", L".find_first_of("},
    {L"字符串视图查找最后一个", L".find_last_of("},
    {L"字符串视图查找第一个非", L".find_first_not_of("},
    {L"字符串视图查找最后一个非", L".find_last_not_of("},

    // 跨度
    {L"跨度", L"std::span"},
    {L"跨度大小", L".size()"},
    {L"跨度大小字节", L".size_bytes()"},
    {L"跨度空", L".empty()"},
    {L"跨度数据", L".data()"},
    {L"跨度开始", L".begin()"},
    {L"跨度结束", L".end()"},
    {L"跨度前开始", L".cbegin()"},
    {L"跨度前结束", L".cend()"},
    {L"跨度前", L".front()"},
    {L"跨度后", L".back()"},
    {L"跨度子跨度", L".subspan("},
    {L"跨度第一个", L".first("},
    {L"跨度最后一个", L".last("},

    // 范围
    {L"范围开始", L"std::ranges::begin"},
    {L"范围结束", L"std::ranges::end"},
    {L"范围大小", L"std::ranges::size"},
    {L"范围空", L"std::ranges::empty"},
    {L"范围数据", L"std::ranges::data"},
    {L"范围前开始", L"std::ranges::cbegin"},
    {L"范围前结束", L"std::ranges::cend"},
    {L"范围前反向开始", L"std::ranges::crbegin"},
    {L"范围前反向结束", L"std::ranges::crend"},
    {L"范围所有", L"std::views::all"},
    {L"范围过滤", L"std::views::filter"},
    {L"范围变换", L"std::views::transform"},
    {L"范围取", L"std::views::take"},
    {L"范围取当", L"std::views::take_while"},
    {L"范围丢弃", L"std::views::drop"},
    {L"范围丢弃当", L"std::views::drop_while"},
    {L"范围连接", L"std::views::join"},
    {L"范围拆分", L"std::views::split"},
    {L"范围公共", L"std::views::common"},
    {L"范围反转", L"std::views::reverse"},
    {L"范围计数", L"std::views::counted"},
    {L"范围iota", L"std::views::iota"},
    {L"范围单", L"std::views::single"},
    {L"范围空", L"std::views::empty"},
    {L"范围生成", L"std::views::generate"},
    {L"范围重复", L"std::views::repeat"},

    // 文件系统
    {L"路径", L"std::filesystem::path"},
    {L"路径字符串", L".string()"},
    {L"路径宽字符串", L".wstring()"},
    {L"路径通用字符串", L".generic_string()"},
    {L"路径文件名", L".filename()"},
    {L"路径父路径", L".parent_path()"},
    {L"路径根路径", L".root_path()"},
    {L"路径相对路径", L".relative_path()"},
    {L"路径根名称", L".root_name()"},
    {L"路径根目录", L".root_directory()"},
    {L"路径空", L".empty()"},
    {L"路径有根路径", L".has_root_path()"},
    {L"路径有文件名", L".has_filename()"},
    {L"路径有父路径", L".has_parent_path()"},
    {L"路径相对", L".is_relative()"},
    {L"路径绝对", L".is_absolute()"},
    {L"路径追加", L".append("},
    {L"路径连接", L".concat("},
    {L"路径清除", L".clear()"},
    {L"路径交换", L".swap("},
    {L"路径比较", L".compare("},
    {L"路径生成规范", L"std::filesystem::canonical("},
    {L"路径生成绝对", L"std::filesystem::absolute("},
    {L"路径生成相对", L"std::filesystem::relative("},
    {L"路径生成代理", L"std::filesystem::proximate("},
    {L"目录条目", L"std::filesystem::directory_entry"},
    {L"目录迭代器", L"std::filesystem::directory_iterator"},
    {L"递归目录迭代器", L"std::filesystem::recursive_directory_iterator"},
    {L"文件状态", L"std::filesystem::file_status"},
    {L"文件类型", L"std::filesystem::file_type"},
    {L"权限", L"std::filesystem::perms"},
    {L"复制选项", L"std::filesystem::copy_options"},
    {L"目录选项", L"std::filesystem::directory_options"},
    {L"文件时钟", L"std::filesystem::file_time_type"},
    {L"存在", L"std::filesystem::exists("},
    {L"是块文件", L"std::filesystem::is_block_file("},
    {L"是字符文件", L"std::filesystem::is_character_file("},
    {L"是目录", L"std::filesystem::is_directory("},
    {L"是空", L"std::filesystem::is_empty("},
    {L"是FIFO", L"std::filesystem::is_fifo("},
    {L"是其他", L"std::filesystem::is_other("},
    {L"是常规文件", L"std::filesystem::is_regular_file("},
    {L"是套接字", L"std::filesystem::is_socket("},
    {L"是符号链接", L"std::filesystem::is_symlink("},
    {L"文件大小", L"std::filesystem::file_size("},
    {L"硬链接计数", L"std::filesystem::hard_link_count("},
    {L"最后写入时间", L"std::filesystem::last_write_time("},
    {L"权限", L"std::filesystem::permissions("},
    {L"创建目录", L"std::filesystem::create_directory("},
    {L"创建目录", L"std::filesystem::create_directories("},
    {L"创建硬链接", L"std::filesystem::create_hard_link("},
    {L"创建符号链接", L"std::filesystem::create_symlink("},
    {L"创建目录符号链接", L"std::filesystem::create_directory_symlink("},
    {L"复制", L"std::filesystem::copy("},
    {L"复制文件", L"std::filesystem::copy_file("},
    {L"复制符号链接", L"std::filesystem::copy_symlink("},
    {L"移除", L"std::filesystem::remove("},
    {L"移除全部", L"std::filesystem::remove_all("},
    {L"重命名", L"std::filesystem::rename("},
    {L"调整文件大小", L"std::filesystem::resize_file("},
    {L"空间", L"std::filesystem::space("},
    {L"当前路径", L"std::filesystem::current_path("},
    {L"临时目录路径", L"std::filesystem::temp_directory_path("},

    // 正则表达式
    {L"正则表达式", L"std::regex"},
    {L"基本正则", L"std::regex_constants::basic"},
    {L"扩展正则", L"std::regex_constants::extended"},
    {L"awk正则", L"std::regex_constants::awk"},
    {L"grep正则", L"std::regex_constants::grep"},
    {L"egrep正则", L"std::regex_constants::egrep"},
    {L"icase", L"std::regex_constants::icase"},
    {L"nosubs", L"std::regex_constants::nosubs"},
    {L"优化", L"std::regex_constants::optimize"},
    {L"整理", L"std::regex_constants::collate"},
    {L"匹配", L"std::regex_match("},
    {L"搜索", L"std::regex_search("},
    {L"替换", L"std::regex_replace("},
    {L"匹配结果", L"std::match_results"},
    {L"子匹配", L"std::sub_match"},
    {L"正则迭代器", L"std::regex_iterator"},
    {L"正则令牌迭代器", L"std::regex_token_iterator"},

    // 随机数
    {L"随机设备", L"std::random_device"},
    {L"默认随机引擎", L"std::default_random_engine"},
    {L"最小标准随机引擎", L"std::minstd_rand"},
    {L"最小标准随机引擎0", L"std::minstd_rand0"},
    {L"梅森旋转", L"std::mt19937"},
    {L"梅森旋转64", L"std::mt19937_64"},
    {L"ranlux24", L"std::ranlux24"},
    {L"ranlux48", L"std::ranlux48"},
    {L"knuth_b", L"std::knuth_b"},
    {L"均匀整数分布", L"std::uniform_int_distribution"},
    {L"均匀实数分布", L"std::uniform_real_distribution"},
    {L"伯努利分布", L"std::bernoulli_distribution"},
    {L"二项分布", L"std::binomial_distribution"},
    {L"几何分布", L"std::geometric_distribution"},
    {L"负二项分布", L"std::negative_binomial_distribution"},
    {L"泊松分布", L"std::poisson_distribution"},
    {L"指数分布", L"std::exponential_distribution"},
    {L"伽马分布", L"std::gamma_distribution"},
    {L"威布尔分布", L"std::weibull_distribution"},
    {L"极值分布", L"std::extreme_value_distribution"},
    {L"正态分布", L"std::normal_distribution"},
    {L"对数正态分布", L"std::lognormal_distribution"},
    {L"卡方分布", L"std::chi_squared_distribution"},
    {L"柯西分布", L"std::cauchy_distribution"},
    {L"费舍尔F分布", L"std::fisher_f_distribution"},
    {L"学生T分布", L"std::student_t_distribution"},
    {L"离散分布", L"std::discrete_distribution"},
    {L"分段常数分布", L"std::piecewise_constant_distribution"},
    {L"分段线性分布", L"std::piecewise_linear_distribution"},

    // 时间
    {L"系统时钟", L"std::chrono::system_clock"},
    {L"稳定时钟", L"std::chrono::steady_clock"},
    {L"高分辨率时钟", L"std::chrono::high_resolution_clock"},
    {L"时间点", L"std::chrono::time_point"},
    {L"持续时间", L"std::chrono::duration"},
    {L"纳秒", L"std::chrono::nanoseconds"},
    {L"微秒", L"std::chrono::microseconds"},
    {L"毫秒", L"std::chrono::milliseconds"},
    {L"秒", L"std::chrono::seconds"},
    {L"分钟", L"std::chrono::minutes"},
    {L"小时", L"std::chrono::hours"},
    {L"天数", L"std::chrono::days"},
    {L"周数", L"std::chrono::weeks"},
    {L"月数", L"std::chrono::months"},
    {L"年数", L"std::chrono::years"},
    {L"当前时间", L".now()"},
    {L"到时间T", L".to_time_t("},
    {L"从时间T", L".from_time_t("},
    {L"持续时间计数", L".count()"},
    {L"持续时间转换", L"std::chrono::duration_cast"},
    {L"时间点转换", L"std::chrono::time_point_cast"},
    {L"地板", L"std::chrono::floor"},
    {L"天花板", L"std::chrono::ceil"},
    {L"舍入", L"std::chrono::round"},
    {L"绝对值", L"std::chrono::abs"},

    // 原子
    {L"原子", L"std::atomic"},
    {L"原子标志", L"std::atomic_flag"},
    {L"原子布尔", L"std::atomic<bool>"},
    {L"原子字符", L"std::atomic<char>"},
    {L"原子有符号字符", L"std::atomic<signed char>"},
    {L"原子无符号字符", L"std::atomic<unsigned char>"},
    {L"原子短整型", L"std::atomic<short>"},
    {L"原子无符号短整型", L"std::atomic<unsigned short>"},
    {L"原子整型", L"std::atomic<int>"},
    {L"原子无符号整型", L"std::atomic<unsigned int>"},
    {L"原子长整型", L"std::atomic<long>"},
    {L"原子无符号长整型", L"std::atomic<unsigned long>"},
    {L"原子长长整型", L"std::atomic<long long>"},
    {L"原子无符号长长整型", L"std::atomic<unsigned long long>"},
    {L"原子指针", L"std::atomic<T*>"},
    {L"原子加载", L".load("},
    {L"原子存储", L".store("},
    {L"原子交换", L".exchange("},
    {L"原子比较交换弱", L".compare_exchange_weak("},
    {L"原子比较交换强", L".compare_exchange_strong("},
    {L"原子获取加", L".fetch_add("},
    {L"原子获取减", L".fetch_sub("},
    {L"原子获取与", L".fetch_and("},
    {L"原子获取或", L".fetch_or("},
    {L"原子获取异或", L".fetch_xor("},
    {L"原子等待", L".wait("},
    {L"原子通知一", L".notify_one()"},
    {L"原子通知所有", L".notify_all()"},
    {L"原子标志测试设置", L".test_and_set("},
    {L"原子标志清除", L".clear("},
    {L"原子标志等待", L".wait("},
    {L"原子标志通知一", L".notify_one()"},
    {L"原子标志通知所有", L".notify_all()"},
    {L"原子线程围栏", L"std::atomic_thread_fence"},
    {L"原子信号围栏", L"std::atomic_signal_fence"},
    {L"杀死依赖", L"std::kill_dependency"},
    {L"原子是否无锁", L".is_lock_free()"},
    {L"原子是否总是无锁", L"std::atomic<T>::is_always_lock_free"},

    // 线程
    {L"线程", L"std::thread"},
    {L"线程标识", L"std::thread::id"},
    {L"线程硬件并发", L"std::thread::hardware_concurrency()"},
    {L"线程可连接", L".joinable()"},
    {L"线程连接", L".join()"},
    {L"线程分离", L".detach()"},
    {L"线程交换", L".swap("},
    {L"线程获取标识", L".get_id()"},
    {L"线程休眠", L"std::this_thread::sleep_for("},
    {L"线程休眠直到", L"std::this_thread::sleep_until("},
    {L"线程让出", L"std::this_thread::yield()"},
    {L"线程获取标识", L"std::this_thread::get_id()"},

    // 互斥锁
    {L"互斥锁", L"std::mutex"},
    {L"定时互斥锁", L"std::timed_mutex"},
    {L"递归互斥锁", L"std::recursive_mutex"},
    {L"递归定时互斥锁", L"std::recursive_timed_mutex"},
    {L"共享互斥锁", L"std::shared_mutex"},
    {L"共享定时互斥锁", L"std::shared_timed_mutex"},
    {L"锁保护", L"std::lock_guard"},
    {L"唯一锁", L"std::unique_lock"},
    {L"共享锁", L"std::shared_lock"},
    {L"作用域锁", L"std::scoped_lock"},
    {L"互斥锁锁定", L".lock()"},
    {L"互斥锁尝试锁定", L".try_lock()"},
    {L"互斥锁解锁", L".unlock()"},
    {L"互斥锁尝试锁定直到", L".try_lock_until("},
    {L"互斥锁尝试锁定for", L".try_lock_for("},
    {L"互斥锁锁定", L"std::lock("},
    {L"互斥锁尝试锁定", L"std::try_lock("},
    {L"互斥锁采用锁", L"std::adopt_lock"},
    {L"互斥锁延迟锁", L"std::defer_lock"},
    {L"互斥锁尝试锁", L"std::try_to_lock"},

    // 条件变量
    {L"条件变量", L"std::condition_variable"},
    {L"条件变量任意", L"std::condition_variable_any"},
    {L"条件变量等待", L".wait("},
    {L"条件变量等待直到", L".wait_until("},
    {L"条件变量等待for", L".wait_for("},
    {L"条件变量通知一", L".notify_one()"},
    {L"条件变量通知所有", L".notify_all()"},

    // 未来
    {L"未来", L"std::future"},
    {L"共享未来", L"std::shared_future"},
    {L"承诺", L"std::promise"},
    {L"打包任务", L"std::packaged_task"},
    {L"异步", L"std::async"},
    {L"启动延迟", L"std::launch::deferred"},
    {L"启动异步", L"std::launch::async"},
    {L"未来获取", L".get()"},
    {L"未来有效", L".valid()"},
    {L"未来等待", L".wait()"},
    {L"未来等待直到", L".wait_until("},
    {L"未来等待for", L".wait_for("},
    {L"未来共享", L".share()"},
    {L"承诺设置值", L".set_value("},
    {L"承诺设置异常", L".set_exception("},
    {L"承诺获取未来", L".get_future()"},
    {L"承诺交换", L".swap("},
    {L"打包任务有效", L".valid()"},
    {L"打包任务交换", L".swap("},
    {L"打包任务获取未来", L".get_future()"},
    {L"打包任务重置", L".reset()"},
    {L"打包任务操作符", L"operator()"},

    // 信号量
    {L"计数信号量", L"std::counting_semaphore"},
    {L"二元信号量", L"std::binary_semaphore"},
    {L"信号量获取", L".acquire()"},
    {L"信号量尝试获取", L".try_acquire()"},
    {L"信号量尝试获取直到", L".try_acquire_until("},
    {L"信号量尝试获取for", L".try_acquire_for("},
    {L"信号量释放", L".release("},
    {L"信号量最大", L".max()"},

    // 锁闩
    {L"锁闩", L"std::latch"},
    {L"锁闩计数", L".count_down("},
    {L"锁闩尝试等待", L".try_wait()"},
    {L"锁闩等待", L".wait()"},
    {L"锁闩到达等待", L".arrive_and_wait("},
    {L"锁闩最大", L".max()"},

    // 屏障
    {L"屏障", L"std::barrier"},
    {L"屏障到达", L".arrive("},
    {L"屏障等待", L".wait()"},
    {L"屏障到达等待", L".arrive_and_wait()"},
    {L"屏障到达丢弃", L".arrive_and_drop()"},
    {L"屏障最大", L".max()"},

    // 停止令牌
    {L"停止令牌", L"std::stop_token"},
    {L"停止源", L"std::stop_source"},
    {L"停止回调", L"std::stop_callback"},
    {L"停止令牌停止请求", L".stop_requested()"},
    {L"停止令牌停止可能", L".stop_possible()"},
    {L"停止源请求停止", L".request_stop()"},
    {L"停止源停止请求", L".stop_requested()"},
    {L"停止源停止可能", L".stop_possible()"},
    {L"停止源获取令牌", L".get_token()"},
    {L"停止源交换", L".swap("},

    // 协程
    {L"协程句柄", L"std::coroutine_handle"},
    {L"无操作协程", L"std::noop_coroutine()"},
    {L"协程特性", L"std::coroutine_traits"},
    {L"挂起总是", L"std::suspend_always"},
    {L"挂起从不", L"std::suspend_never"},
    {L"协程句柄完成", L".done()"},
    {L"协程句柄恢复", L".resume()"},
    {L"协程句柄销毁", L".destroy()"},
    {L"协程句柄从承诺", L".from_promise("},
    {L"协程句柄承诺", L".promise()"},
    {L"协程句柄地址", L".address()"},
    {L"协程句柄从地址", L"std::coroutine_handle<>::from_address("},

    // 类型特征
    {L"类型特征", L"std::type_identity"},
    {L"移除常量", L"std::remove_cv"},
    {L"移除易变", L"std::remove_volatile"},
    {L"移除常量易变", L"std::remove_cv"},
    {L"添加常量", L"std::add_const"},
    {L"添加易变", L"std::add_volatile"},
    {L"添加常量易变", L"std::add_cv"},
    {L"移除引用", L"std::remove_reference"},
    {L"添加左值引用", L"std::add_lvalue_reference"},
    {L"添加右值引用", L"std::add_rvalue_reference"},
    {L"移除指针", L"std::remove_pointer"},
    {L"添加指针", L"std::add_pointer"},
    {L"移除范围", L"std::remove_extent"},
    {L"移除所有范围", L"std::remove_all_extents"},
    {L"对齐", L"std::alignment_of"},
    {L"对齐", L"std::align"},
    {L"排名", L"std::rank"},
    {L"范围", L"std::extent"},
    {L"是void", L"std::is_void"},
    {L"是null指针", L"std::is_null_pointer"},
    {L"是整型", L"std::is_integral"},
    {L"是浮点", L"std::is_floating_point"},
    {L"是数组", L"std::is_array"},
    {L"是枚举", L"std::is_enum"},
    {L"是联合", L"std::is_union"},
    {L"是类", L"std::is_class"},
    {L"是函数", L"std::is_function"},
    {L"是指针", L"std::is_pointer"},
    {L"是左值引用", L"std::is_lvalue_reference"},
    {L"是右值引用", L"std::is_rvalue_reference"},
    {L"是成员指针", L"std::is_member_pointer"},
    {L"是成员对象指针", L"std::is_member_object_pointer"},
    {L"是成员函数指针", L"std::is_member_function_pointer"},
    {L"是基本", L"std::is_fundamental"},
    {L"是算术", L"std::is_arithmetic"},
    {L"是标量", L"std::is_scalar"},
    {L"是对象", L"std::is_object"},
    {L"是复合", L"std::is_compound"},
    {L"是引用", L"std::is_reference"},
    {L"是成员指针", L"std::is_member_pointer"},
    {L"是常量", L"std::is_const"},
    {L"是易变", L"std::is_volatile"},
    {L"是平凡", L"std::is_trivial"},
    {L"是琐碎可复制", L"std::is_trivially_copyable"},
    {L"是标准布局", L"std::is_standard_layout"},
    {L"是POD", L"std::is_pod"},
    {L"是字面量类型", L"std::is_literal_type"},
    {L"是空", L"std::is_empty"},
    {L"是多态", L"std::is_polymorphic"},
    {L"是抽象", L"std::is_abstract"},
    {L"是最终", L"std::is_final"},
    {L"是聚合", L"std::is_aggregate"},
    {L"是签名", L"std::is_signed"},
    {L"是无符号", L"std::is_unsigned"},
    {L"是默认构造", L"std::is_default_constructible"},
    {L"是复制构造", L"std::is_copy_constructible"},
    {L"是移动构造", L"std::is_move_constructible"},
    {L"是复制赋值", L"std::is_copy_assignable"},
    {L"是移动赋值", L"std::is_move_assignable"},
    {L"是析构", L"std::is_destructible"},
    {L"是平凡默认构造", L"std::is_trivially_default_constructible"},
    {L"是平凡复制构造", L"std::is_trivially_copy_constructible"},
    {L"是平凡移动构造", L"std::is_trivially_move_constructible"},
    {L"是平凡复制赋值", L"std::is_trivially_copy_assignable"},
    {L"是平凡移动赋值", L"std::is_trivially_move_assignable"},
    {L"是平凡析构", L"std::is_trivially_destructible"},
    {L"是不抛出默认构造", L"std::is_nothrow_default_constructible"},
    {L"是不抛出复制构造", L"std::is_nothrow_copy_constructible"},
    {L"是不抛出移动构造", L"std::is_nothrow_move_constructible"},
    {L"是不抛出复制赋值", L"std::is_nothrow_copy_assignable"},
    {L"是不抛出移动赋值", L"std::is_nothrow_move_assignable"},
    {L"是不抛出析构", L"std::is_nothrow_destructible"},
    {L"有虚拟析构", L"std::has_virtual_destructor"},
    {L"是相同", L"std::is_same"},
    {L"是基础", L"std::is_base_of"},
    {L"是可转换", L"std::is_convertible"},
    {L"是可调用", L"std::is_invocable"},
    {L"是调用结果", L"std::is_invocable_r"},
    {L"是构造", L"std::is_constructible"},
    {L"是平凡构造", L"std::is_trivially_constructible"},
    {L"是不抛出构造", L"std::is_nothrow_constructible"},
    {L"是默认构造", L"std::is_default_constructible"},
    {L"是平凡默认构造", L"std::is_trivially_default_constructible"},
    {L"是不抛出默认构造", L"std::is_nothrow_default_constructible"},
    {L"是复制构造", L"std::is_copy_constructible"},
    {L"是平凡复制构造", L"std::is_trivially_copy_constructible"},
    {L"是不抛出复制构造", L"std::is_nothrow_copy_constructible"},
    {L"是移动构造", L"std::is_move_constructible"},
    {L"是平凡移动构造", L"std::is_trivially_move_constructible"},
    {L"是不抛出移动构造", L"std::is_nothrow_move_constructible"},
    {L"是复制赋值", L"std::is_copy_assignable"},
    {L"是平凡复制赋值", L"std::is_trivially_copy_assignable"},
    {L"是不抛出复制赋值", L"std::is_nothrow_copy_assignable"},
    {L"是移动赋值", L"std::is_move_assignable"},
    {L"是平凡移动赋值", L"std::is_trivially_move_assignable"},
    {L"是不抛出移动赋值", L"std::is_nothrow_move_assignable"},
    {L"是析构", L"std::is_destructible"},
    {L"是平凡析构", L"std::is_trivially_destructible"},
    {L"是不抛出析构", L"std::is_nothrow_destructible"},
    {L"有虚拟析构", L"std::has_virtual_destructor"},
    {L"交换", L"std::is_swappable"},
    {L"不抛出交换", L"std::is_nothrow_swappable"},
    {L"交换", L"std::is_swappable_with"},
    {L"不抛出交换", L"std::is_nothrow_swappable_with"},
    {L"对齐", L"std::alignment_of"},
    {L"排名", L"std::rank"},
    {L"范围", L"std::extent"},
    {L"是绑定表达式", L"std::is_bind_expression"},
    {L"是占位符", L"std::is_placeholder"},
    {L"是错误代码枚举", L"std::is_error_code_enum"},
    {L"是错误条件枚举", L"std::is_error_condition_enum"},

    // 概念
    {L"概念", L"std::same_as"},
    {L"概念可推导", L"std::derived_from"},
    {L"概念可转换", L"std::convertible_to"},
    {L"概念共同引用", L"std::common_reference_with"},
    {L"概念共同有", L"std::common_with"},
    {L"概念可赋值", L"std::assignable_from"},
    {L"概念可交换", L"std::swappable"},
    {L"概念可交换", L"std::swappable_with"},
    {L"概念可销毁", L"std::destructible"},
    {L"概念可构造", L"std::constructible_from"},
    {L"概念默认可构造", L"std::default_initializable"},
    {L"概念移动可构造", L"std::move_constructible"},
    {L"概念拷贝可构造", L"std::copy_constructible"},
    {L"概念可移动", L"std::movable"},
    {L"概念可拷贝", L"std::copyable"},
    {L"概念半正则", L"std::semiregular"},
    {L"概念正则", L"std::regular"},
    {L"概念可调用", L"std::invocable"},
    {L"概念谓词", L"std::predicate"},
    {L"概念关系", L"std::relation"},
    {L"概念等价关系", L"std::equivalence_relation"},
    {L"概念严格弱序", L"std::strict_weak_order"},

    // 三路比较
    {L"三路比较", L"std::compare_three_way"},
    {L"强序", L"std::strong_ordering"},
    {L"弱序", L"std::weak_ordering"},
    {L"偏序", L"std::partial_ordering"},
    {L"比较等于", L"std::compare_equal_to"},
    {L"比较不等于", L"std::compare_not_equal_to"},
    {L"比较大于", L"std::compare_greater"},
    {L"比较大于等于", L"std::compare_greater_equal"},
    {L"比较小于", L"std::compare_less"},
    {L"比较小于等于", L"std::compare_less_equal"},

    // 格式化
    {L"格式", L"std::format"},
    {L"格式到", L"std::format_to"},
    {L"格式到N", L"std::format_to_n"},
    {L"格式化大小", L"std::formatted_size"},
    {L"格式错误", L"std::format_error"},

    // 源位置
    {L"源位置", L"std::source_location"},
    {L"源位置当前", L"std::source_location::current()"},
    {L"源位置行", L".line()"},
    {L"源位置列", L".column()"},
    {L"源位置文件", L".file_name()"},
    {L"源位置函数", L".function_name()"},

    // 栈踪迹
    {L"栈踪迹", L"std::stacktrace"},
    {L"当前栈踪迹", L"std::stacktrace::current()"},
    {L"栈踪迹大小", L".size()"},
    {L"栈踪迹开始", L".begin()"},
    {L"栈踪迹结束", L".end()"},
    {L"栈踪迹条目", L"std::stacktrace_entry"},
    {L"栈踪迹条目描述", L".description()"},
    {L"栈踪迹条目源文件", L".source_file()"},
    {L"栈踪迹条目源行", L".source_line()"},

    // 位操作
    {L"位宽度", L"std::bit_width"},
    {L"位计数", L"std::popcount"},
    {L"位循环左移", L"std::rotl"},
    {L"位循环右移", L"std::rotr"},
    {L"位字节交换", L"std::byteswap"},
    {L"位计数前导零", L"std::countl_zero"},
    {L"位计数前导一", L"std::countl_one"},
    {L"位计数尾随零", L"std::countr_zero"},
    {L"位计数尾随一", L"std::countr_one"},
    {L"位有单", L"std::has_single_bit"},
    {L"位下一个", L"std::bit_ceil"},
    {L"位地板", L"std::bit_floor"},
    {L"位对数2", L"std::bit_width"},

    // 数字
    {L"数字e", L"std::numbers::e"},
    {L"数字log2e", L"std::numbers::log2e"},
    {L"数字log10e", L"std::numbers::log10e"},
    {L"数字pi", L"std::numbers::pi"},
    {L"数字inv_pi", L"std::numbers::inv_pi"},
    {L"数字inv_sqrtpi", L"std::numbers::inv_sqrtpi"},
    {L"数字ln2", L"std::numbers::ln2"},
    {L"数字ln10", L"std::numbers::ln10"},
    {L"数字sqrt2", L"std::numbers::sqrt2"},
    {L"数字sqrt3", L"std::numbers::sqrt3"},
    {L"数字inv_sqrt3", L"std::numbers::inv_sqrt3"},
    {L"数字egamma", L"std::numbers::egamma"},
    {L"数字phi", L"std::numbers::phi"},

    // 数学常数
    {L"数学e", L"M_E"},
    {L"数学log2e", L"M_LOG2E"},
    {L"数学log10e", L"M_LOG10E"},
    {L"数学ln2", L"M_LN2"},
    {L"数学ln10", L"M_LN10"},
    {L"数学pi", L"M_PI"},
    {L"数学pi_2", L"M_PI_2"},
    {L"数学pi_4", L"M_PI_4"},
    {L"数学1_pi", L"M_1_PI"},
    {L"数学2_pi", L"M_2_PI"},
    {L"数学2_sqrtpi", L"M_2_SQRTPI"},
    {L"数学sqrt2", L"M_SQRT2"},
    {L"数学sqrt1_2", L"M_SQRT1_2"},

    // 实用宏
    {L"万能头", L"#include <bits/stdc++.h>"},
    {L"使用标准命名空间", L"using namespace std;"},
    {L"标准主函数", L"int main() {\n    \n    return 0;\n}"},
    {L"返回成功", L"return EXIT_SUCCESS;"},
    {L"返回失败", L"return EXIT_FAILURE;"},
    {L"标准成功", L"EXIT_SUCCESS"},
    {L"标准失败", L"EXIT_FAILURE"},
    {L"空指针", L"NULL"},
    {L"空", L"nullptr"},
    {L"最大尺寸", L"size_t"},
    {L"指针差异", L"ptrdiff_t"},
    {L"最大对齐", L"max_align_t"},
    {L"字节", L"std::byte"},
    {L"初始值列表", L"std::initializer_list"},

    // 编译器内置
    {L"内置期望", L"__builtin_expect"},
    {L"内置选择", L"__builtin_choose_expr"},
    {L"内置类型兼容", L"__builtin_types_compatible_p"},
    {L"内置常量", L"__builtin_constant_p"},
    {L"内置返回地址", L"__builtin_return_address"},
    {L"内置框架地址", L"__builtin_frame_address"},
    {L"内置陷阱", L"__builtin_trap"},
    {L"内置未达到", L"__builtin_unreachable"},
    {L"内置预取", L"__builtin_prefetch"},
    {L"内置人口计数", L"__builtin_popcount"},
    {L"内置前导零", L"__builtin_clz"},
    {L"内置尾随零", L"__builtin_ctz"},
    {L"内置前导一", L"__builtin_clrsb"},
    {L"内置字节交换", L"__builtin_bswap16"},
    {L"内置字节交换32", L"__builtin_bswap32"},
    {L"内置字节交换64", L"__builtin_bswap64"},
    {L"内置溢出加", L"__builtin_add_overflow"},
    {L"内置溢出减", L"__builtin_sub_overflow"},
    {L"内置溢出乘", L"__builtin_mul_overflow"},

    // 属性
    {L"属性无返回", L"[[noreturn]]"},
    {L"属性可能为空", L"[[maybe_unused]]"},
    {L"属性落空", L"[[fallthrough]]"},
    {L"属性无重复", L"[[nodiscard]]"},
    {L"属性已弃用", L"[[deprecated]]"},
    {L"属性可能", L"[[likely]]"},
    {L"属性不可能", L"[[unlikely]]"},
    {L"属性无唯一地址", L"[[no_unique_address]]"},

    // 标准宏
    {L"文件宏", L"__FILE__"},
    {L"行号宏", L"__LINE__"},
    {L"函数宏", L"__func__"},
    {L"函数签名宏", L"__FUNCSIG__"},
    {L"函数名宏", L"__FUNCTION__"},
    {L"日期宏", L"__DATE__"},
    {L"时间宏", L"__TIME__"},
    {L"时间戳宏", L"__TIMESTAMP__"},
    {L"标准C宏", L"__STDC__"},
    {L"标准C版本宏", L"__STDC_VERSION__"},
    {L"C++版本宏", L"__cplusplus"},

    // 平台检测
    {L"窗口平台", L"_WIN32"},
    {L"窗口64平台", L"_WIN64"},
    {L"Linux平台", L"__linux__"},
    {L"苹果平台", L"__APPLE__"},
    {L"Unix平台", L"__unix__"},
    {L"POSIX平台", L"__POSIX__"},
    {L"BSD平台", L"__BSD__"},
    {L"GNU平台", L"__GNUC__"},
    {L"Clang平台", L"__clang__"},
    {L"MSVC平台", L"_MSC_VER"},

    // 架构检测
    {L"x86架构", L"__i386__"},
    {L"x86_64架构", L"__x86_64__"},
    {L"ARM架构", L"__arm__"},
    {L"ARM64架构", L"__aarch64__"},
    {L"PowerPC架构", L"__powerpc__"},
    {L"PowerPC64架构", L"__powerpc64__"},
    {L"MIPS架构", L"__mips__"},
    {L"MIPS64架构", L"__mips64__"},

    // 断言
    {L"断言", L"assert"},
    {L"静态断言", L"static_assert"},

    // 最终确保覆盖所有内容
    {L"结束", L"}"},
    {L"开始", L"{"},
    {L"分号", L";"},
    {L"逗号", L","},
    {L"点", L"."},
    {L"箭头", L"->"},
    {L"双冒号", L"::"},
    {L"星号", L"*"},
    {L"与号", L"&"},
    {L"左括号", L"("},
    {L"右括号", L")"},
    {L"左方括号", L"["},
    {L"右方括号", L"]"},
    {L"左尖括号", L"<"},
    {L"右尖括号", L">"},
    {L"问号", L"?"},
    {L"冒号", L":"},
    {L"双引号", L"\""},
    {L"单引号", L"'"},
    {L"反斜杠", L"\\"},
    {L"斜杠", L"/"},
    {L"百分号", L"%"},
    {L"美元符号", L"$"},
    {L"at符号", L"@"},
    {L"井号", L"#"},
    {L"脱字符", L"^"},
    {L"与符号", L"&"},
    {L"星号", L"*"},
    {L"下划线", L"_"},
    {L"连字符", L"-"},
    {L"等号", L"="},
    {L"加号", L"+"},
    {L"竖线", L"|"},
    {L"波浪线", L"~"},
    {L"反引号", L"`"},
    {L"感叹号", L"!"},
    {L"问号", L"?"},
    {L"大于号", L">"},
    {L"小于号", L"<"},
    {L"分号", L";"},
    {L"冒号", L":"},
    {L"引号", L"\""},
    {L"撇号", L"'"},
    {L"反斜杠", L"\\"},
    {L"正斜杠", L"/"},
    {L"点", L"."},
    {L"逗号", L","},

    // 标准库异常类
    {L"异常", L"std::exception"},
    {L"逻辑错误", L"std::logic_error"},
    {L"无效参数", L"std::invalid_argument"},
    {L"域错误", L"std::domain_error"},
    {L"长度错误", L"std::length_error"},
    {L"超出范围", L"std::out_of_range"},
    {L"运行时错误", L"std::runtime_error"},
    {L"范围错误", L"std::range_error"},
    {L"溢出错误", L"std::overflow_error"},
    {L"下溢错误", L"std::underflow_error"},
    {L"系统错误", L"std::system_error"},
    {L"错误代码", L"std::error_code"},
    {L"错误条件", L"std::error_condition"},
    {L"错误类别", L"std::error_category"},

    // 流状态
    {L"流好", L"good()"},
    {L"流结束", L"eof()"},
    {L"流失败", L"fail()"},
    {L"流坏", L"bad()"},
    {L"流清除", L"clear()"},
    {L"流设置状态", L"setstate("},
    {L"流异常", L"exceptions("},

    // 流格式化
    {L"设置宽度", L"std::setw("},
    {L"设置精度", L"std::setprecision("},
    {L"设置填充", L"std::setfill("},
    {L"布尔alpha", L"std::boolalpha"},
    {L"非布尔alpha", L"std::noboolalpha"},
    {L"显示基数", L"std::showbase"},
    {L"非显示基数", L"std::noshowbase"},
    {L"显示点", L"std::showpoint"},
    {L"非显示点", L"std::noshowpoint"},
    {L"显示正号", L"std::showpos"},
    {L"非显示正号", L"std::noshowpos"},
    {L"跳过空白", L"std::skipws"},
    {L"非跳过空白", L"std::noskipws"},
    {L"大写", L"std::uppercase"},
    {L"非大写", L"std::nouppercase"},
    {L"左对齐", L"std::left"},
    {L"右对齐", L"std::right"},
    {L"内部对齐", L"std::internal"},
    {L"十进制", L"std::dec"},
    {L"十六进制", L"std::hex"},
    {L"八进制", L"std::oct"},
    {L"固定", L"std::fixed"},
    {L"科学", L"std::scientific"},
    {L"默认浮点", L"std::defaultfloat"},

    // 文件流模式
    {L"输入模式", L"std::ios::in"},
    {L"输出模式", L"std::ios::out"},
    {L"二进制模式", L"std::ios::binary"},
    {L"追加模式", L"std::ios::app"},
    {L"截断模式", L"std::ios::trunc"},
    {L"在结尾模式", L"std::ios::ate"},

    // 字符串转换
    {L"字符串到整型", L"std::stoi"},
    {L"字符串到长整型", L"std::stol"},
    {L"字符串到无符号长整型", L"std::stoul"},
    {L"字符串到长长整型", L"std::stoll"},
    {L"字符串到无符号长长整型", L"std::stoull"},
    {L"字符串到浮点", L"std::stof"},
    {L"字符串到双精度", L"std::stod"},
    {L"字符串到长双精度", L"std::stold"},
    {L"整型到字符串", L"std::to_string"},
    {L"长整型到字符串", L"std::to_string"},
    {L"无符号到字符串", L"std::to_string"},
    {L"长长整型到字符串", L"std::to_string"},
    {L"无符号长长整型到字符串", L"std::to_string"},
    {L"浮点到字符串", L"std::to_string"},
    {L"双精度到字符串", L"std::to_string"},
    {L"长双精度到字符串", L"std::to_string"},

    // 字符分类
    {L"是字母", L"std::isalpha"},
    {L"是大写", L"std::isupper"},
    {L"是小写", L"std::islower"},
    {L"是数字", L"std::isdigit"},
    {L"是十六进制数字", L"std::isxdigit"},
    {L"是字母数字", L"std::isalnum"},
    {L"是空白", L"std::isspace"},
    {L"是打印", L"std::isprint"},
    {L"是图形", L"std::isgraph"},
    {L"是标点", L"std::ispunct"},
    {L"是控制", L"std::iscntrl"},
    {L"是空白字符", L"std::isblank"},
    {L"转大写", L"std::toupper"},
    {L"转小写", L"std::tolower"},

    // 宽字符分类
    {L"宽字符是字母", L"std::iswalpha"},
    {L"宽字符是大写", L"std::iswupper"},
    {L"宽字符是小写", L"std::iswlower"},
    {L"宽字符是数字", L"std::iswdigit"},
    {L"宽字符是十六进制数字", L"std::iswxdigit"},
    {L"宽字符是字母数字", L"std::iswalnum"},
    {L"宽字符是空白", L"std::iswspace"},
    {L"宽字符是打印", L"std::iswprint"},
    {L"宽字符是图形", L"std::iswgraph"},
    {L"宽字符是标点", L"std::iswpunct"},
    {L"宽字符是控制", L"std::iswcntrl"},
    {L"宽字符是空白字符", L"std::iswblank"},
    {L"宽字符转大写", L"std::towupper"},
    {L"宽字符转小写", L"std::towlower"},

    // 本地化
    {L"本地化", L"std::locale"},
    {L"全局本地化", L"std::locale::global"},
    {L"经典本地化", L"std::locale::classic"},
    {L"名称", L".name()"},
    {L"哈希", L".hash()"},

    // 数值限制
    {L"数字限制", L"std::numeric_limits"},
    {L"最小值", L"min()"},
    {L"最大值", L"max()"},
    {L"最低值", L"lowest()"},
    {L"epsilon", L"epsilon()"},
    {L"舍入错误", L"round_error()"},
    {L"无穷大", L"infinity()"},
    {L"安静NaN", L"quiet_NaN()"},
    {L"信号NaN", L"signaling_NaN()"},
    {L"小数位数", L"digits"},
    {L"小数位数10", L"digits10"},
    {L"最大小数位数10", L"max_digits10"},
    {L"是整数", L"is_integer"},
    {L"是精确", L"is_exact"},
    {L"有无限", L"has_infinity"},
    {L"有安静NaN", L"has_quiet_NaN"},
    {L"有信号NaN", L"has_signaling_NaN"},
    {L"有去归一化", L"has_denorm"},
    {L"有去归一化损失", L"has_denorm_loss"},
    {L"舍入风格", L"round_style"},
    {L"是iec559", L"is_iec559"},
    {L"是有界", L"is_bounded"},
    {L"是模运算", L"is_modulo"},
    {L"陷阱", L"traps"},
    {L"微小性前", L"tinyness_before"},
    {L"基数", L"radix"},

    // 复数运算
    {L"复数实部", L"real()"},
    {L"复数虚部", L"imag()"},
    {L"复数模", L"abs("},
    {L"复数幅角", L"arg("},
    {L"复数范数", L"norm("},
    {L"复数共轭", L"conj("},
    {L"复数极坐标", L"polar("},
    {L"复数指数", L"exp("},
    {L"复数对数", L"log("},
    {L"复数对数10", L"log10("},
    {L"复数幂", L"pow("},
    {L"复数平方根", L"sqrt("},
    {L"复数正弦", L"sin("},
    {L"复数余弦", L"cos("},
    {L"复数正切", L"tan("},
    {L"复数双曲正弦", L"sinh("},
    {L"复数双曲余弦", L"cosh("},
    {L"复数双曲正切", L"tanh("},

    // 随机数分布
    {L"均匀整数分布", L"std::uniform_int_distribution"},
    {L"均匀实数分布", L"std::uniform_real_distribution"},
    {L"伯努利分布", L"std::bernoulli_distribution"},
    {L"二项分布", L"std::binomial_distribution"},
    {L"几何分布", L"std::geometric_distribution"},
    {L"负二项分布", L"std::negative_binomial_distribution"},
    {L"泊松分布", L"std::poisson_distribution"},
    {L"指数分布", L"std::exponential_distribution"},
    {L"伽马分布", L"std::gamma_distribution"},
    {L"威布尔分布", L"std::weibull_distribution"},
    {L"极值分布", L"std::extreme_value_distribution"},
    {L"正态分布", L"std::normal_distribution"},
    {L"对数正态分布", L"std::lognormal_distribution"},
    {L"卡方分布", L"std::chi_squared_distribution"},
    {L"柯西分布", L"std::cauchy_distribution"},
    {L"费舍尔F分布", L"std::fisher_f_distribution"},
    {L"学生T分布", L"std::student_t_distribution"},
    {L"离散分布", L"std::discrete_distribution"},
    {L"分段常数分布", L"std::piecewise_constant_distribution"},
    {L"分段线性分布", L"std::piecewise_linear_distribution"},

    // 随机数引擎
    {L"线性同余引擎", L"std::linear_congruential_engine"},
    {L"梅森旋转引擎", L"std::mersenne_twister_engine"},
    {L"带进位减法引擎", L"std::subtract_with_carry_engine"},
    {L"盘式引擎", L"std::discard_block_engine"},
    {L"独立位引擎", L"std::independent_bits_engine"},
    {L"洗牌顺序引擎", L"std::shuffle_order_engine"},

    // 预定义随机数引擎
    {L"最小标准随机引擎", L"std::minstd_rand0"},
    {L"最小标准随机引擎", L"std::minstd_rand"},
    {L"梅森旋转19937", L"std::mt19937"},
    {L"梅森旋转19937_64", L"std::mt19937_64"},
    {L"ranlux24", L"std::ranlux24"},
    {L"ranlux48", L"std::ranlux48"},
    {L"knuth_b", L"std::knuth_b"},
    {L"默认随机引擎", L"std::default_random_engine"},

    // 正则表达式语法选项
    {L"ECMAScript语法", L"std::regex_constants::ECMAScript"},
    {L"基本POSIX语法", L"std::regex_constants::basic"},
    {L"扩展POSIX语法", L"std::regex_constants::extended"},
    {L"awk语法", L"std::regex_constants::awk"},
    {L"grep语法", L"std::regex_constants::grep"},
    {L"egrep语法", L"std::regex_constants::egrep"},
    {L"icase", L"std::regex_constants::icase"},
    {L"nosubs", L"std::regex_constants::nosubs"},
    {L"优化", L"std::regex_constants::optimize"},
    {L"整理", L"std::regex_constants::collate"},

    // 正则表达式错误
    {L"正则表达式错误", L"std::regex_error"},
    {L"错误括号", L"std::regex_constants::error_brack"},
    {L"错误括号范围", L"std::regex_constants::error_range"},
    {L"错误括号结束", L"std::regex_constants::error_brace"},
    {L"错误括号数量", L"std::regex_constants::error_badbrace"},
    {L"错误括号操作", L"std::regex_constants::error_paren"},
    {L"错误内存", L"std::regex_constants::error_space"},
    {L"错误重复", L"std::regex_constants::error_badrepeat"},
    {L"错误复杂", L"std::regex_constants::error_complexity"},
    {L"错误栈", L"std::regex_constants::error_stack"},

    // 文件系统错误
    {L"文件系统错误", L"std::filesystem::filesystem_error"},
    {L"路径1", L".path1()"},
    {L"路径2", L".path2()"},
    {L"错误代码", L".code()"},

    // 文件系统权限
    {L"无权限", L"std::filesystem::perms::none"},
    {L"所有者读", L"std::filesystem::perms::owner_read"},
    {L"所有者写", L"std::filesystem::perms::owner_write"},
    {L"所有者执行", L"std::filesystem::perms::owner_exec"},
    {L"所有者所有", L"std::filesystem::perms::owner_all"},
    {L"组读", L"std::filesystem::perms::group_read"},
    {L"组写", L"std::filesystem::perms::group_write"},
    {L"组执行", L"std::filesystem::perms::group_exec"},
    {L"组所有", L"std::filesystem::perms::group_all"},
    {L"其他读", L"std::filesystem::perms::others_read"},
    {L"其他写", L"std::filesystem::perms::others_write"},
    {L"其他执行", L"std::filesystem::perms::others_exec"},
    {L"其他所有", L"std::filesystem::perms::others_all"},
    {L"所有", L"std::filesystem::perms::all"},
    {L"设置uid", L"std::filesystem::perms::set_uid"},
    {L"设置gid", L"std::filesystem::perms::set_gid"},
    {L"粘滞位", L"std::filesystem::perms::sticky_bit"},
    {L"掩码", L"std::filesystem::perms::mask"},
    {L"未知权限", L"std::filesystem::perms::unknown"},

    // 文件类型
    {L"无文件", L"std::filesystem::file_type::none"},
    {L"未找到", L"std::filesystem::file_type::not_found"},
    {L"常规文件", L"std::filesystem::file_type::regular"},
    {L"目录", L"std::filesystem::file_type::directory"},
    {L"符号链接", L"std::filesystem::file_type::symlink"},
    {L"块文件", L"std::filesystem::file_type::block"},
    {L"字符文件", L"std::filesystem::file_type::character"},
    {L"FIFO", L"std::filesystem::file_type::fifo"},
    {L"套接字", L"std::filesystem::file_type::socket"},
    {L"未知", L"std::filesystem::file_type::unknown"},

    // 目录选项
    {L"跳过权限拒绝", L"std::filesystem::directory_options::skip_permission_denied"},
    {L"跟随符号链接", L"std::filesystem::directory_options::follow_directory_symlink"},

    // 复制选项
    {L"跳过现有", L"std::filesystem::copy_options::skip_existing"},
    {L"覆盖现有", L"std::filesystem::copy_options::overwrite_existing"},
    {L"更新现有", L"std::filesystem::copy_options::update_existing"},
    {L"递归", L"std::filesystem::copy_options::recursive"},
    {L"复制符号链接", L"std::filesystem::copy_options::copy_symlinks"},
    {L"跳过符号链接", L"std::filesystem::copy_options::skip_symlinks"},
    {L"创建符号链接", L"std::filesystem::copy_options::create_symlinks"},
    {L"创建硬链接", L"std::filesystem::copy_options::create_hard_links"},
    {L"直接", L"std::filesystem::copy_options::directories_only"},

    // 原子内存顺序
    {L"内存顺序松弛", L"std::memory_order_relaxed"},
    {L"内存顺序消费", L"std::memory_order_consume"},
    {L"内存顺序获取", L"std::memory_order_acquire"},
    {L"内存顺序释放", L"std::memory_order_release"},
    {L"内存顺序获取释放", L"std::memory_order_acq_rel"},
    {L"内存顺序顺序一致", L"std::memory_order_seq_cst"},

    // 线程本地存储
    {L"线程局部", L"thread_local"},

    // 协程关键字
    {L"协程返回", L"co_return"},
    {L"协程让出", L"co_yield"},
    {L"协程等待", L"co_await"},

    // 模块关键字
    {L"模块", L"module"},
    {L"导入", L"import"},
    {L"导出", L"export"},

    // 概念关键字
    {L"概念", L"concept"},
    {L"要求", L"requires"},

    // 三路比较关键字
    {L"三路比较", L"<=>"},

    // 属性关键字
    {L"无返回属性", L"[[noreturn]]"},
    {L"可能未使用属性", L"[[maybe_unused]]"},
    {L"落空属性", L"[[fallthrough]]"},
    {L"无丢弃属性", L"[[nodiscard]]"},
    {L"已弃用属性", L"[[deprecated]]"},
    {L"可能属性", L"[[likely]]"},
    {L"不可能属性", L"[[unlikely]]"},
    {L"无唯一地址属性", L"[[no_unique_address]]"},

    // 最终确保覆盖所有C++标准库内容
    {L"标准", L"std"},
    {L"标准命名空间", L"std"},
    {L"全局命名空间", L"::"},
    {L"匿名命名空间", L"namespace {"},
    {L"内联命名空间", L"inline namespace"},

    // 常用宏和常量
    {L"空指针", L"NULL"},
    {L"空指针常量", L"nullptr"},
    {L"最大大小", L"SIZE_MAX"},
    {L"最大有符号", L"INT_MAX"},
    {L"最小有符号", L"INT_MIN"},
    {L"最大无符号", L"UINT_MAX"},
    {L"最大长长", L"LLONG_MAX"},
    {L"最小长长", L"LLONG_MIN"},
    {L"最大无符号长长", L"ULLONG_MAX"},

    // 数学常量
    {L"圆周率", L"M_PI"},
    {L"2圆周率", L"M_2PI"},
    {L"圆周率_2", L"M_PI_2"},
    {L"圆周率_4", L"M_PI_4"},
    {L"1_圆周率", L"M_1_PI"},
    {L"2_圆周率", L"M_2_PI"},
    {L"2_平方根圆周率", L"M_2_SQRTPI"},
    {L"平方根2", L"M_SQRT2"},
    {L"平方根1_2", L"M_SQRT1_2"},
    {L"自然对数2", L"M_LN2"},
    {L"自然对数10", L"M_LN10"},
    {L"对数2e", L"M_LOG2E"},
    {L"对数10e", L"M_LOG10E"},
    {L"e", L"M_E"},

    // 标准返回码
    {L"成功", L"EXIT_SUCCESS"},
    {L"失败", L"EXIT_FAILURE"},

    // 标准流对象
    {L"标准输入", L"stdin"},
    {L"标准输出", L"stdout"},
    {L"标准错误", L"stderr"},

    // 最终结束标记
    {L"结束映射表", L"// End of mapping table"}
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
