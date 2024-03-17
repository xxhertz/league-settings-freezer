#pragma once
#include <Windows.h>
#include <string>

// Returns `true` if a file is readonly, and false otherwise
inline bool is_readonly(std::string file) {
	return GetFileAttributesA(file.c_str()) & FILE_ATTRIBUTE_READONLY;
}

// Toggles a file's readonly state
inline void toggle_readonly(std::string file) {
	SetFileAttributesA(file.c_str(), GetFileAttributesA(file.c_str()) ^ FILE_ATTRIBUTE_READONLY);
}