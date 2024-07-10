// Compile Command(Clang++):clang++ -std=c++17 main.cpp -o 7z_expl.exe -lShell32 -lUser32 -mwindows

#include <string>
#include <windows.h>
#include <shellapi.h>
#include <filesystem>

// 7-Zipのインストールパスを指定
const std::wstring seven_zip_path = L"\"C:\\Program Files\\7-Zip\\7zG.exe\"";

// ファイルパスからディレクトリパスを取得する関数
std::wstring get_directory_path(const std::wstring& file_path) {
    size_t last_slash_pos = file_path.find_last_of(L"\\/");
    return (std::wstring::npos == last_slash_pos) ? L"" : file_path.substr(0, last_slash_pos);
}

// ファイルパスから拡張子を取り除いたファイル名を取得する関数
std::wstring get_file_name_without_extension(const std::wstring& file_path) {
    size_t last_dot_pos = file_path.find_last_of(L".");
    size_t last_slash_pos = file_path.find_last_of(L"\\/");
    return file_path.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1);
}

// 相対パスを絶対パスに変換する関数
std::wstring get_absolute_path(const std::wstring& relative_path) {
    return std::filesystem::absolute(relative_path).wstring();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc < 2) {
        MessageBoxA(NULL, "Usage: <program> <archive_path>", "Error", MB_ICONERROR);
        return 1;
    }

    std::wstring archive_path(argv[1]);
    std::wstring output_directory = get_absolute_path(get_directory_path(archive_path) + L"\\" + get_file_name_without_extension(archive_path));

    // 7-Zipコマンドを作成
    std::wstring command = seven_zip_path + L" x \"" + archive_path + L"\" -aos -o\"" + output_directory + L"\"";

    // コマンドを実行するための構造体を初期化
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // コマンドを実行
    if (!CreateProcessW(
            NULL,
            const_cast<LPWSTR>(command.c_str()),
            NULL,
            NULL,
            FALSE,
            CREATE_NO_WINDOW,  // コンソールウィンドウを表示しない
            NULL,
            NULL,
            &si,
            &pi)
    ) {
        MessageBoxW(NULL, L"Failed to execute 7-Zip command.", L"Error", MB_ICONERROR);
        return 1;
    }

    // プロセスの終了を待機
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // 解凍したディレクトリをエクスプローラで開く
    HINSTANCE exec_result = ShellExecuteW(NULL, L"open", output_directory.c_str(), NULL, NULL, SW_SHOWNORMAL);

    // ShellExecuteWの結果をチェック
    if (reinterpret_cast<intptr_t>(exec_result) <= 32) {
        MessageBoxW(NULL, L"Failed to open explorer window.", L"Error", MB_ICONERROR);
        return 1;
    }
}
