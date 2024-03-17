#include "readonly.hpp"
#include "resource.hpp"
#include "tray.hpp"

void update_tray(bool readonly, HINSTANCE instance, NOTIFYICONDATA* nid) {
	nid->hIcon = reinterpret_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCE(readonly ? READONLY_ICON : WRITABLE_ICON), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR));
	wcscpy_s(nid->szTip, readonly ? L"League Setting Freezer (LOCKED)" : L"League Setting Freezer");

	nid->uFlags |= NIF_ICON | NIF_TIP;

	Shell_NotifyIconW(NIM_MODIFY, nid);
	DestroyIcon(nid->hIcon);
}

BOOL setup_tray(bool readonly, HINSTANCE instance, NOTIFYICONDATA* nid, WNDPROC WndProc) {
	WNDCLASSA wcex;

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIconW(instance, MAKEINTRESOURCE(READONLY_ICON));
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEA(READONLY_ICON);
	wcex.lpszClassName = "LeagueSettingFreezer";

	if (!RegisterClassA(&wcex))
		return FALSE;

	auto hWnd = CreateWindowExA(NULL, "LeagueSettingFreezer", "League Setting Freezer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instance, nullptr);
	if (!hWnd)
		return FALSE;

	nid->cbSize = sizeof(NOTIFYICONDATA);
	nid->hWnd = hWnd;
	nid->uID = 100;
	nid->uVersion = NOTIFYICON_VERSION;
	nid->uCallbackMessage = WM_MYMESSAGE;
	nid->uFlags = NIF_MESSAGE;

	Shell_NotifyIconW(NIM_ADD, nid);
	DestroyIcon(nid->hIcon);
	update_tray(readonly, instance, nid);

	UpdateWindow(hWnd);

	return TRUE;
}