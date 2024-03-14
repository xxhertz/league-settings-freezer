#pragma once
#include <Windows.h>
#include <string>

inline std::string get_league_dir() {
	HKEY hkey;
	// Open the registry key
	LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Riot Game league_of_legends.live", 0, KEY_READ, &hkey);
	if (result != ERROR_SUCCESS) {
		MessageBoxA(NULL, "Error opening registry key: " + result, "[LSF] Registry Error", MB_OK);
		throw 1;
	}

	// Read the registry value
	BYTE valueData[1024];
	DWORD valueDataSize = sizeof(valueData);
	result = RegQueryValueExA(hkey, "InstallLocation", nullptr, nullptr, valueData, &valueDataSize);
	if (result != ERROR_SUCCESS) {
		MessageBoxA(NULL, "Error reading registry value: " + result, "[LSF] Registry Error", MB_OK);
		RegCloseKey(hkey);
		throw 1;
	}

	// Close the registry key
	RegCloseKey(hkey);
	
	return std::string(reinterpret_cast<const char*>(valueData)).append("/Config/PersistedSettings.json");
}