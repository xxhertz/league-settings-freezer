#pragma once
#include <Windows.h>
#include <string>

// Unsets a bit, then sets it if `enable` is true
inline DWORD set_bit(DWORD bitflag, DWORD flag, bool enable) {
	return (bitflag & ~flag) | (enable ? flag : 0);
}

// Returns `true` if a file is readonly, and false otherwise
inline bool is_readonly(std::string file) {
	return GetFileAttributesA(file.c_str()) & FILE_ATTRIBUTE_READONLY;
}

// Toggles a file's readonly state
inline void toggle_readonly(std::string file) {
	SetFileAttributesA(file.c_str(), GetFileAttributesA(file.c_str()) ^ FILE_ATTRIBUTE_READONLY);
}

// Changes a file's readonly state
inline void set_readonly(std::string file, bool readonly) {
	SetFileAttributesA(file.c_str(), set_bit(GetFileAttributesA(file.c_str()), FILE_ATTRIBUTE_READONLY, readonly));
}