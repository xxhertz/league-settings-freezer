// LeagueSettingsFreezerTray.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files

#include "resource.h"
#include "tray.h"
#include "readonly.h"
#include "leaguedir.h"

#include <shellapi.h>

// Global Variables:
HINSTANCE instance;
std::string league_dir;
NOTIFYICONDATA nid;
bool automatic = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM command, LPARAM param) {
	if (message == WM_COMMAND) {
		if (command == 1) {
			MessageBoxA(NULL, is_readonly(league_dir) ? "Closing LSF, your settings will remain frozen" : "Closing LSF, your settings will remain unfrozen", "League Settings Freezer", MB_OK);
			PostQuitMessage(0);
		}
		else if (command == 2) {
			automatic = !automatic;
		}
	}
	else if (message == WM_MYMESSAGE) {
		if (param == WM_LBUTTONUP) {
			toggle_readonly(league_dir);
			update_tray(is_readonly(league_dir), instance, &nid);
		}
		else if (param == WM_RBUTTONUP) {
			HMENU menu = CreatePopupMenu();
			AppendMenuA(menu, MF_STRING, 1, "Exit");
			AppendMenuA(menu, automatic ? MF_CHECKED : MF_UNCHECKED, 2, "Automatically Toggle");
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hwnd);
			TrackPopupMenu(menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
			PostMessageW(hwnd, WM_NULL, 0, 0); // Close the popup menu
		}
		else {
			return DefWindowProcW(hwnd, message, command, param);
		}
	}
	else {
		return DefWindowProcW(hwnd, message, command, param);
	}
}

int APIENTRY WinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE, _In_ PSTR, _In_ int) {
	instance = inst;
	league_dir = get_league_dir();

	if (!setup_tray(is_readonly(league_dir), instance, &nid, &WndProc))
		return FALSE;

	MSG msg;
	while (GetMessageA(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, LoadAccelerators(inst, MAKEINTRESOURCE(IDC_LEAGUESETTINGSFREEZERTRAY)), &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	Shell_NotifyIconW(NIM_DELETE, &nid);

	return (int)msg.wParam;
}