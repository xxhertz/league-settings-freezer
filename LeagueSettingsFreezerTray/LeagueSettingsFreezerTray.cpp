// LeagueSettingsFreezerTray.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files

#include "resource.hpp"
#include "tray.hpp"
#include "readonly.hpp"
#include "leaguedir.hpp"
#include "getprocess.hpp"
#include <shellapi.h>
#include <thread>

// Global Variables:
HINSTANCE instance;
std::string league_dir;
NOTIFYICONDATA nid;
bool automatic = true;
bool check_if_in_game = true;
bool check_if_client_open = false;

void automatic_loop() {
	if (!automatic) return; // pointless just a micro-optimization
	// (the thread would exit before running any code anyways but there's just no point in creating a thread if we're just going to exit)

	std::thread([]() {
		while (automatic) {
			bool in_game = false;

			if (check_if_in_game && get_process_id(L"League of Legends.exe") > 0)
				in_game = true;

			if (check_if_client_open && get_process_id(L"LeagueClient.exe") > 0)
				in_game = true;

			if (is_readonly(league_dir) == in_game) { // pointless also micro optimization, avoid multiple api calls if they change nothing
				set_readonly(league_dir, !in_game);
				update_tray(!in_game, instance, &nid);
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}).detach();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM command, LPARAM param) {
	if (message == WM_COMMAND) {
		if (command == 1) {
			MessageBoxA(NULL, is_readonly(league_dir) ? "Closing LSF, your settings will remain frozen" : "Closing LSF, your settings will remain unfrozen", "League Settings Freezer", MB_OK);
			PostQuitMessage(0);
		}
		else if (command == 2) {
			automatic = !automatic;
			automatic_loop();
		}
		else if (command == 3) {
			check_if_in_game = !check_if_in_game;
		}
		else if (command == 4) {
			check_if_client_open = !check_if_client_open;
		}
	}
	else if (message == WM_MYMESSAGE) {
		if (param == WM_LBUTTONUP) {
			toggle_readonly(league_dir);
			update_tray(is_readonly(league_dir), instance, &nid);
		}
		else if (param == WM_RBUTTONUP) {
			auto menu = CreatePopupMenu();
			AppendMenuA(menu, automatic ? MF_CHECKED : MF_UNCHECKED, 2, "Automatically Toggle");
			if (automatic) {
				auto automatic_menu = CreatePopupMenu();
				AppendMenuA(automatic_menu, check_if_in_game ? MF_CHECKED : MF_UNCHECKED, 3, "Check if game is open");
				AppendMenuA(automatic_menu, check_if_client_open ? MF_CHECKED : MF_UNCHECKED, 4, "Check if client is open");
				
				AppendMenuA(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(automatic_menu), "Automatic Checks");
			}
			AppendMenuA(menu, MF_STRING, 1, "Exit");
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
	
	automatic_loop();
	
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