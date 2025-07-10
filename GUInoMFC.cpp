#include <windows.h>
#include <string>
#include <cwctype> // cho iswupper, towupper.

#define ID_EDIT       1
#define ID_BUTTON     2
#define ID_CHECKBOX   3
#define ID_OUTPUT     4

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::wstring ReverseAndToggleCase(const std::wstring& input, bool toggleCase) {
    std::wstring result(input.rbegin(), input.rend());
    if (toggleCase) {
        for (wchar_t& c : result) {
            if (iswlower(c)) c = towupper(c);
            else if (iswupper(c)) c = towlower(c);
        }
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"ReverseApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_INACTIVECAPTION + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Reverse Text App",
        WS_OVERLAPPEDWINDOW,
        750, 350, 365, 160,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit, hButton, hCheckBox, hResult;

    switch (uMsg) {
    case WM_CREATE:
        hEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            5, 10, 340, 25, hwnd, (HMENU)ID_EDIT, nullptr, nullptr);
        hCheckBox = CreateWindow(L"BUTTON", L"UpCase / LowCase", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            5, 45, 200, 20, hwnd, (HMENU)ID_CHECKBOX, nullptr, nullptr);
        hButton = CreateWindow(L"BUTTON", L"Reverse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            225, 44, 120, 22, hwnd, (HMENU)ID_BUTTON, nullptr, nullptr);
        hResult = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE| ES_READONLY | ES_AUTOHSCROLL,
            5, 80, 340, 25, hwnd, (HMENU)ID_OUTPUT, nullptr, nullptr);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON) {
            wchar_t buffer[256] = {};
            GetWindowText(hEdit, buffer, 256);
            bool toggleCase = (SendMessage(hCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED);
            std::wstring result = ReverseAndToggleCase(buffer, toggleCase);
            SetWindowText(hResult, result.c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
