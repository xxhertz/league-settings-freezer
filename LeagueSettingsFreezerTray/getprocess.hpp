#pragma once
#include <string>
#include <windows.h>
#include <tlhelp32.h>

DWORD get_process_id(std::wstring process_name);