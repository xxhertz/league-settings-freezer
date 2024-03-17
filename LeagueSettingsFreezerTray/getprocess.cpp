#include "getprocess.hpp"

DWORD get_process_id(std::wstring process_name) {
	auto processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	DWORD process_id = 0;

	PROCESSENTRY32W process;
	process.dwSize = sizeof(process);

	if (Process32FirstW(processes, &process))
		while (Process32NextW(processes, &process))
			if (process.szExeFile == process_name) {
				process_id = process.th32ProcessID;
				break;
			}

	CloseHandle(processes);

	return process_id;
}