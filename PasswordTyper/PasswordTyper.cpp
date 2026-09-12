#include <windows.h>
#include <shellapi.h>
#include <string>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")

// ------------------------------------------------------------
// Window procedure
// ------------------------------------------------------------
LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(
        hwnd,
        msg,
        wParam,
        lParam
    );
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    const wchar_t CLASS_NAME[] =
        L"PasswordTyperWindow";

    WNDCLASSW wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursorW(
        nullptr,
        IDC_ARROW
    );

    wc.hbrBackground =
        reinterpret_cast<HBRUSH>(
            COLOR_WINDOW + 1
            );

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"PasswordTyper - Created by Yigal Neeman",
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        520,
        220,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd)
        return 1;

    // --------------------------------------------------------
    // Program information
    // --------------------------------------------------------

    CreateWindowW(
        L"STATIC",
        L"PasswordTyper",
        WS_VISIBLE | WS_CHILD,
        25,
        25,
        400,
        30,
        hwnd,
        nullptr,
        hInstance,
        nullptr
    );

    CreateWindowW(
        L"STATIC",
        L"Created by Yigal Neeman",
        WS_VISIBLE | WS_CHILD,
        25,
        60,
        400,
        25,
        hwnd,
        nullptr,
        hInstance,
        nullptr
    );

    CreateWindowW(
        L"STATIC",
        L"Music is running in the background.",
        WS_VISIBLE | WS_CHILD,
        25,
        100,
        450,
        25,
        hwnd,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // --------------------------------------------------------
    // Start song automatically
    // --------------------------------------------------------

    wchar_t exePath[MAX_PATH] = {};

    GetModuleFileNameW(
        nullptr,
        exePath,
        MAX_PATH
    );

    std::wstring fullPath(exePath);

    size_t slash =
        fullPath.find_last_of(
            L"\\/"
        );

    if (slash != std::wstring::npos)
    {
        fullPath =
            fullPath.substr(
                0,
                slash + 1
            );
    }

    fullPath += L"song.mp4";

    HINSTANCE result = ShellExecuteW(
        hwnd,
        L"open",
        fullPath.c_str(),
        nullptr,
        nullptr,
        SW_SHOWNOACTIVATE
    );

    if ((INT_PTR)result <= 32)
    {
        MessageBoxW(
            hwnd,
            L"Could not start song.mp4.\n\n"
            L"Make sure song.mp4 is located next to PasswordTyper.exe.",
            L"PasswordTyper",
            MB_ICONERROR
        );
    }

    // --------------------------------------------------------
    // Message loop
    // --------------------------------------------------------

    MSG msg = {};

    while (GetMessageW(
        &msg,
        nullptr,
        0,
        0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}