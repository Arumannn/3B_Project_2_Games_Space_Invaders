#include "alien.h"

int WINDOW_WIDTH;
int WINDOW_HEIGHT;

static HWND hwnd;
static HDC hdc;
static PAINTSTRUCT ps;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void initWindow() {
    WINDOW_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    WINDOW_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SpaceInvaders";
    RegisterClass(&wc);

    hwnd = CreateWindow(
        "SpaceInvaders", "Space Alien Invaders",
        WS_POPUP | WS_VISIBLE,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);
    hdc = GetDC(hwnd);
}

void drawRect(int x, int y, int width, int height, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    RECT rect = {x, y, x + width, y + height};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
}

void drawEllipse(int x, int y, int width, int height, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    Ellipse(hdc, x, y, x + width, y + height);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}

void drawLine(int x1, int y1, int x2, int y2, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void clearScreen() {
    drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0));
}

void updateScreen() {
    MSG msg = {0};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int getKeyInput() {
    int keys = 0;
    if (GetAsyncKeyState('W') & 0x8000) keys |= 1;
    if (GetAsyncKeyState('A') & 0x8000) keys |= 2;
    if (GetAsyncKeyState('D') & 0x8000) keys |= 4;
    if (GetAsyncKeyState('X') & 0x8000) keys |= 8;
    return keys;
}