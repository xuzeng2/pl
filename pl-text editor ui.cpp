#include <windows.h>
#include <commdlg.h>
#include <richedit.h>
#include <string>
#include <shellapi.h>
#include <fstream>
#include <shlwapi.h>  // 添加路径处理头文件

using namespace std;

// 菜单命令ID
#define ID_FILE_NEW 1001
#define ID_FILE_OPEN 1002
#define ID_FILE_SAVE 1003
#define ID_FILE_SAVEAS 1004
#define ID_FILE_EXIT 1005
#define ID_EDIT_UNDO 2001
#define ID_EDIT_CUT 2002
#define ID_EDIT_COPY 2003
#define ID_EDIT_PASTE 2004
#define ID_EDIT_DELETE 2005
#define ID_EDIT_SELECTALL 2006
#define ID_FORMAT_FONT 3001
#define ID_TOOLS_COMPILE 3002
#define ID_HELP_ABOUT 4001

// 状态栏常量定义
#define SBARS_SIZEGRIP 0x0100
#define SB_SETTEXT (WM_USER+1)
#define SB_SETPARTS (WM_USER+4)

class TextEditor {
private:
    HWND hwnd;
    HWND hEdit;
    HWND hStatus;
    wstring currentFile;
    bool isModified;
    HFONT hFont;

public:
    TextEditor() : hwnd(NULL), hEdit(NULL), hStatus(NULL), isModified(false), hFont(NULL) {}

    bool initialize() {
        // 注册窗口类
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = L"TextEditor";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassExW(&wc)) {
            MessageBoxW(NULL, L"窗口类注册失败!", L"错误", MB_ICONERROR);
            return false;
        }

        // 加载richedit控件
        LoadLibraryW(L"Msftedit.dll");

        // 创建主窗口
        hwnd = CreateWindowExW(
            0, L"TextEditor", L"简易文本编辑器",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            NULL, NULL, GetModuleHandle(NULL), this
        );

        if (!hwnd) {
            MessageBoxW(NULL, L"窗口创建失败!", L"错误", MB_ICONERROR);
            return false;
        }

        createMenu();
        createEditControl();
        createStatusBar();
        setFont(L"宋体", 12);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        return true;
    }

    void createMenu() {
        HMENU hMenuBar = CreateMenu();
        HMENU hFileMenu = CreatePopupMenu();
        HMENU hEditMenu = CreatePopupMenu();
        HMENU hFormatMenu = CreatePopupMenu();
        HMENU hToolsMenu = CreatePopupMenu();
        HMENU hHelpMenu = CreatePopupMenu();

        // 文件菜单
        AppendMenuW(hFileMenu, MF_STRING, ID_FILE_NEW, L"新建(&N)\tCtrl+N");
        AppendMenuW(hFileMenu, MF_STRING, ID_FILE_OPEN, L"打开(&O)...\tCtrl+O");
        AppendMenuW(hFileMenu, MF_STRING, ID_FILE_SAVE, L"保存(&S)\tCtrl+S");
        AppendMenuW(hFileMenu, MF_STRING, ID_FILE_SAVEAS, L"另存为(&A)...");
        AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hFileMenu, MF_STRING, ID_FILE_EXIT, L"退出(&X)");

        // 编辑菜单
        AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_UNDO, L"撤销(&U)\tCtrl+Z");
        AppendMenuW(hEditMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_CUT, L"剪切(&T)\tCtrl+X");
        AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_COPY, L"复制(&C)\tCtrl+C");
        AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_PASTE, L"粘贴(&P)\tCtrl+V");
        AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_DELETE, L"删除(&D)");
        AppendMenuW(hEditMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_SELECTALL, L"全选(&A)\tCtrl+A");

        // 格式菜单
        AppendMenuW(hFormatMenu, MF_STRING, ID_FORMAT_FONT, L"字体(&F)...");

        // 工具菜单
        AppendMenuW(hToolsMenu, MF_STRING, ID_TOOLS_COMPILE, L"编译(&C)\tF5");

        // 帮助菜单
        AppendMenuW(hHelpMenu, MF_STRING, ID_HELP_ABOUT, L"关于(&A)...");

        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"文件(&F)");
        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hEditMenu, L"编辑(&E)");
        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFormatMenu, L"格式(&O)");
        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hToolsMenu, L"工具(&T)");
        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, L"帮助(&H)");

        SetMenu(hwnd, hMenuBar);
    }

    void createEditControl() {
        hEdit = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"RICHEDIT50W", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
            ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN,
            0, 0, 100, 100, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
        );
    }

    void createStatusBar() {
        hStatus = CreateWindowExW(
            0, L"msctls_statusbar32", L"",
            WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
            0, 0, 0, 0, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
        );

        int parts[] = { 200, 400, -1 };
        SendMessageW(hStatus, SB_SETPARTS, 3, (LPARAM)parts);
        updateStatusBar();
    }

    void setFont(const wchar_t* fontName, int fontSize) {
        if (hFont) DeleteObject(hFont);

        hFont = CreateFontW(
            fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName
        );

        if (hEdit && hFont) {
            SendMessageW(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        }
    }

    void updateStatusBar() {
        // 光标位置
        DWORD pos = (DWORD)SendMessageW(hEdit, EM_GETSEL, 0, 0);
        int startPos = LOWORD(pos);
        int line = (int)SendMessageW(hEdit, EM_LINEFROMCHAR, startPos, 0) + 1;
        int col = startPos - (int)SendMessageW(hEdit, EM_LINEINDEX, -1, 0) + 1;

        wchar_t status[100];
        swprintf_s(status, 100, L"行: %d, 列: %d", line, col);
        SendMessageW(hStatus, SB_SETTEXT, 0, (LPARAM)status);

        // 文件信息
        wchar_t fileInfo[500];
        const wchar_t* fileName = currentFile.empty() ? L"未命名" : currentFile.c_str();
        swprintf_s(fileInfo, 500, L"%s%s", fileName, isModified ? L" [已修改]" : L"");
        SendMessageW(hStatus, SB_SETTEXT, 1, (LPARAM)fileInfo);

        SendMessageW(hStatus, SB_SETTEXT, 2, (LPARAM)L"就绪");
    }

    void updateTitle() {
        wstring title = L"简易文本编辑器 - ";
        title += currentFile.empty() ? L"未命名" : currentFile;
        if (isModified) title += L" *";
        SetWindowTextW(hwnd, title.c_str());
    }

    bool newFile() {
        if (checkSave()) {
            SetWindowTextW(hEdit, L"");
            currentFile.clear();
            isModified = false;
            updateTitle();
            updateStatusBar();
            return true;
        }
        return false;
    }

    bool openFile() {
        if (!checkSave()) return false;

        wchar_t filename[MAX_PATH] = { 0 };
        OPENFILENAMEW ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0";
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameW(&ofn)) {
            return loadFile(filename);
        }
        return false;
    }

    bool saveFile() {
        if (currentFile.empty()) {
            return saveFileAs();
        }
        return saveToFile(currentFile.c_str());
    }

    bool saveFileAs() {
        wchar_t filename[MAX_PATH] = { 0 };
        OPENFILENAMEW ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"文本文件 (*.txt)\0*.txt\0C++文件 (*.cpp)\0*.cpp\0头文件 (*.h)\0*.h\0所有文件 (*.*)\0*.*\0";
        ofn.lpstrDefExt = L"txt";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

        if (GetSaveFileNameW(&ofn)) {
            wstring finalFilename = filename;

            // 检查是否已有扩展名，如果没有则根据选择的过滤器添加
            if (finalFilename.find(L'.') == wstring::npos) {
                switch (ofn.nFilterIndex) {
                case 1: // .txt
                    finalFilename += L".txt";
                    break;
                case 2: // .cpp
                    finalFilename += L".cpp";
                    break;
                case 3: // .h
                    finalFilename += L".h";
                    break;
                default:
                    finalFilename += L".txt";
                    break;
                }
            }

            return saveToFile(finalFilename.c_str());
        }
        return false;
    }

    // 询问保存文件名
    bool askSaveFileName(wstring& filename) {
        wchar_t buffer[MAX_PATH] = { 0 };
        OPENFILENAMEW ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = buffer;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"文本文件 (*.txt)\0*.txt\0C++文件 (*.cpp)\0*.cpp\0头文件 (*.h)\0*.h\0所有文件 (*.*)\0*.*\0";
        ofn.lpstrDefExt = L"txt";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

        if (GetSaveFileNameW(&ofn)) {
            filename = buffer;

            // 检查是否已有扩展名，如果没有则根据选择的过滤器添加
            if (filename.find(L'.') == wstring::npos) {
                switch (ofn.nFilterIndex) {
                case 1: // .txt
                    filename += L".txt";
                    break;
                case 2: // .cpp
                    filename += L".cpp";
                    break;
                case 3: // .h
                    filename += L".h";
                    break;
                default:
                    filename += L".txt";
                    break;
                }
            }
            return true;
        }
        return false;
    }

    // 从完整路径中提取文件名
    wstring extractFileName(const wstring& fullPath) {
        if (fullPath.empty()) {
            return L"";
        }

        // 手动实现文件名提取
        size_t lastSlash = fullPath.find_last_of(L"\\/");
        if (lastSlash != wstring::npos) {
            return fullPath.substr(lastSlash + 1);
        }
        return fullPath;
    }

    bool loadFile(const wchar_t* filename) {
        HANDLE hFile = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }

        DWORD size = GetFileSize(hFile, NULL);
        if (size == INVALID_FILE_SIZE) {
            CloseHandle(hFile);
            return false;
        }

        // 初始化缓冲区
        BYTE* buffer = new BYTE[size + 2]();  // 使用()进行值初始化
        DWORD read = 0;
        if (ReadFile(hFile, buffer, size, &read, NULL) && read == size) {
            buffer[size] = 0;
            buffer[size + 1] = 0;

            // 检测BOM并处理编码
            wstring wideText;
            if (size >= 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF) {
                // UTF-8 with BOM
                char* utf8Text = reinterpret_cast<char*>(buffer + 3);
                int wideSize = MultiByteToWideChar(CP_UTF8, 0, utf8Text, -1, NULL, 0);
                if (wideSize > 0) {
                    wideText.resize(wideSize);
                    MultiByteToWideChar(CP_UTF8, 0, utf8Text, -1, &wideText[0], wideSize);
                }
            }
            else if (size >= 2 && buffer[0] == 0xFF && buffer[1] == 0xFE) {
                // UTF-16 LE
                wideText = reinterpret_cast<wchar_t*>(buffer + 2);
            }
            else {
                // 尝试ANSI编码
                int wideSize = MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<char*>(buffer), -1, NULL, 0);
                if (wideSize > 0) {
                    wideText.resize(wideSize);
                    MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<char*>(buffer), -1, &wideText[0], wideSize);
                }
            }

            SetWindowTextW(hEdit, wideText.c_str());
            delete[] buffer;
            CloseHandle(hFile);

            currentFile = filename;
            isModified = false;
            updateTitle();
            updateStatusBar();
            return true;
        }

        delete[] buffer;
        CloseHandle(hFile);
        return false;
    }

    bool saveToFile(const wchar_t* filename) {
        // 获取文本长度
        int len = GetWindowTextLengthW(hEdit);

        // 初始化文件句柄
        HANDLE hFile = CreateFileW(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }

        // 如果文件为空，只写入BOM
        if (len <= 0) {
            BYTE bom[] = { 0xEF, 0xBB, 0xBF };
            DWORD written = 0;
            WriteFile(hFile, bom, sizeof(bom), &written, NULL);
            CloseHandle(hFile);

            currentFile = filename;
            isModified = false;
            updateTitle();
            updateStatusBar();
            return true;
        }

        // 获取文本内容
        wchar_t* buffer = new wchar_t[len + 1]();  // 初始化缓冲区
        GetWindowTextW(hEdit, buffer, len + 1);

        // 写入UTF-8 BOM
        BYTE bom[] = { 0xEF, 0xBB, 0xBF };
        DWORD written = 0;
        WriteFile(hFile, bom, sizeof(bom), &written, NULL);

        // 转换为UTF-8
        int utf8Size = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
        if (utf8Size > 0) {
            char* utf8Buffer = new char[utf8Size]();  // 初始化缓冲区
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, utf8Buffer, utf8Size, NULL, NULL);

            // 写入UTF-8内容（不包括null终止符）
            BOOL success = WriteFile(hFile, utf8Buffer, utf8Size - 1, &written, NULL);
            delete[] utf8Buffer;

            delete[] buffer;
            CloseHandle(hFile);

            if (success) {
                currentFile = filename;
                isModified = false;
                updateTitle();
                updateStatusBar();
                return true;
            }
        }

        delete[] buffer;
        CloseHandle(hFile);
        return false;
    }

    bool checkSave() {
        if (isModified) {
            int result = MessageBoxW(hwnd, L"文件已修改，是否保存？", L"保存文件", MB_YESNOCANCEL | MB_ICONQUESTION);
            if (result == IDYES) return saveFile();
            else if (result == IDCANCEL) return false;
        }
        return true;
    }

    void compileFile() {
        wstring filenameToCompile;

        // 如果文件未保存，询问保存文件名
        if (currentFile.empty()) {
            int result = MessageBoxW(hwnd, L"文件未保存，需要先保存文件才能编译。是否现在保存？", L"编译", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                if (!askSaveFileName(filenameToCompile)) {
                    return; // 用户取消保存
                }
                if (!saveToFile(filenameToCompile.c_str())) {
                    MessageBoxW(hwnd, L"保存失败，编译取消。", L"错误", MB_ICONERROR);
                    return;
                }
            }
            else {
                return; // 用户选择不保存，取消编译
            }
        }
        else if (isModified) {
            // 如果文件已命名但有修改，询问是否保存
            int result = MessageBoxW(hwnd, L"文件已修改，是否保存后再编译？", L"编译", MB_YESNOCANCEL | MB_ICONQUESTION);
            if (result == IDYES) {
                if (!saveFile()) {
                    MessageBoxW(hwnd, L"保存失败，编译取消。", L"错误", MB_ICONERROR);
                    return;
                }
            }
            else if (result == IDCANCEL) {
                return; // 用户取消编译
            }
        }

        // 使用当前文件名作为编译参数（只传递文件名，不包含路径）
        filenameToCompile = currentFile;
        wstring fileNameOnly = extractFileName(filenameToCompile);

        // 检查pl-main.exe是否存在
        if (GetFileAttributesW(L"pl-main.exe") == INVALID_FILE_ATTRIBUTES) {
            MessageBoxW(hwnd, L"未找到 pl-main.exe 文件！\n请确保该文件在当前目录下。", L"错误", MB_ICONERROR);
            return;
        }

        // 准备命令行参数（只传递文件名，不包含路径）
        wstring parameters = L"\"";
        parameters += fileNameOnly;
        parameters += L"\"";

        // 显示编译信息
        wstring compileInfo = L"正在编译文件：\n";
        compileInfo += filenameToCompile;
        compileInfo += L"\n\n传递给编译器的参数：";
        compileInfo += parameters;
        compileInfo += L"\n\n注意：只传递文件名，不包含路径";
        MessageBoxW(hwnd, compileInfo.c_str(), L"编译", MB_ICONINFORMATION);

        // 启动编译程序，传递文件名作为参数
        HINSTANCE result = ShellExecuteW(
            hwnd,
            L"open",
            L"pl-main.exe",
            parameters.c_str(),  // 传递文件名作为参数
            NULL,
            SW_SHOW
        );

        if ((INT_PTR)result > 32) {
            wstring successMsg = L"编译命令已启动！\n\n完整路径：";
            successMsg += filenameToCompile;
            successMsg += L"\n传递的参数：";
            successMsg += parameters;
            successMsg += L"\n\npl-main.exe 将只接收到文件名";
            MessageBoxW(hwnd, successMsg.c_str(), L"编译", MB_ICONINFORMATION);
        }
        else {
            MessageBoxW(hwnd, L"启动编译失败！", L"错误", MB_ICONERROR);
        }
    }

    void showAbout() {
        MessageBoxW(hwnd,
            L"简易文本编辑器\n\n"
            L"基于Windows API开发的文本编辑器\n"
            L"版本 1.0\n\n"
            L"功能特性：\n"
            L"• 支持中文和UTF-8编码\n"
            L"• 自动添加文件后缀\n"
            L"• 编译功能（F5键）- 只传递文件名给编译器\n"
            L"• 字体设置\n\n"
            L"编译功能：执行当前目录下的 pl-main.exe，并将当前文件名（不含路径）作为参数传递",
            L"关于",
            MB_OK | MB_ICONINFORMATION
        );
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        TextEditor* editor = nullptr;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            editor = (TextEditor*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)editor);
            editor->hwnd = hwnd;
        }
        else {
            editor = (TextEditor*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (editor) {
            switch (uMsg) {
            case WM_CREATE:
                return 0;

            case WM_SIZE: {
                RECT rc;
                GetClientRect(hwnd, &rc);
                int statusHeight = 20;
                MoveWindow(editor->hEdit, 0, 0, rc.right, rc.bottom - statusHeight, TRUE);
                MoveWindow(editor->hStatus, 0, rc.bottom - statusHeight, rc.right, statusHeight, TRUE);
                SendMessageW(editor->hStatus, WM_SIZE, 0, 0);
                return 0;
            }

            case WM_COMMAND: {
                int cmd = LOWORD(wParam);
                switch (cmd) {
                case ID_FILE_NEW: editor->newFile(); break;
                case ID_FILE_OPEN: editor->openFile(); break;
                case ID_FILE_SAVE: editor->saveFile(); break;
                case ID_FILE_SAVEAS: editor->saveFileAs(); break;
                case ID_FILE_EXIT:
                    if (editor->checkSave()) DestroyWindow(hwnd);
                    break;

                case ID_EDIT_UNDO: SendMessageW(editor->hEdit, EM_UNDO, 0, 0); break;
                case ID_EDIT_CUT: SendMessageW(editor->hEdit, WM_CUT, 0, 0); break;
                case ID_EDIT_COPY: SendMessageW(editor->hEdit, WM_COPY, 0, 0); break;
                case ID_EDIT_PASTE: SendMessageW(editor->hEdit, WM_PASTE, 0, 0); break;
                case ID_EDIT_DELETE: SendMessageW(editor->hEdit, WM_CLEAR, 0, 0); break;
                case ID_EDIT_SELECTALL:
                    SendMessageW(editor->hEdit, EM_SETSEL, 0, -1);
                    break;

                case ID_FORMAT_FONT: {
                    CHOOSEFONTW cf = { sizeof(CHOOSEFONTW) };
                    LOGFONTW lf = {};
                    if (editor->hFont) {
                        GetObjectW(editor->hFont, sizeof(lf), &lf);
                    }
                    else {
                        wcscpy_s(lf.lfFaceName, L"宋体");
                        lf.lfHeight = -12;
                    }
                    cf.lpLogFont = &lf;
                    cf.hwndOwner = hwnd;
                    cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
                    if (ChooseFontW(&cf)) {
                        editor->setFont(lf.lfFaceName, lf.lfHeight);
                    }
                    break;
                }

                case ID_TOOLS_COMPILE: editor->compileFile(); break;
                case ID_HELP_ABOUT: editor->showAbout(); break;
                }
                return 0;
            }

            case WM_KEYDOWN: {
                // 处理F5快捷键
                if (wParam == VK_F5) {
                    editor->compileFile();
                    return 0;
                }
                break;
            }

            case WM_NOTIFY:
                editor->isModified = true;
                editor->updateTitle();
                editor->updateStatusBar();
                return 0;

            case WM_CLOSE:
                if (editor->checkSave()) {
                    DestroyWindow(hwnd);
                }
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    void run() {
        MSG msg = {};
        while (GetMessageW(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
};

// 修复WinMain函数签名
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    TextEditor editor;
    if (editor.initialize()) {
        editor.run();
    }
    return 0;
}

int main() {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}