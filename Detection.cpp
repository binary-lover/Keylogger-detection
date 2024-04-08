// keylogger detection application using C++ and WinAPI

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

std::string GetProcessFilePath(DWORD processId) {
    std::string filePath;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess != NULL) {
        TCHAR szProcessPath[MAX_PATH];
        DWORD dwSize = sizeof(szProcessPath) / sizeof(TCHAR);
        if (QueryFullProcessImageName(hProcess, 0, szProcessPath, &dwSize) != 0) {
            filePath = szProcessPath;
        }
        CloseHandle(hProcess);
    }
    return filePath;
}

struct IntStringPair {
    int processID;
    std::string filePath;
};

void addPair(std::vector<IntStringPair*>& pairs, int intValue, const std::string& stringValue) {
    IntStringPair* pair = new IntStringPair{intValue, stringValue};
    pairs.push_back(pair);
}

void isKeyloggerRunning( std::vector<IntStringPair*>& pairs) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    
    
    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateToolhelp32Snapshot failed." << std::endl;
        return;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process, and exit if unsuccessful.
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Process32First failed." << std::endl;
        CloseHandle(hProcessSnap);
        return;
    }

    cout<<"[+] Scanning running processes...\n";
    bool keyloggerDitected = 0;   
    do {
        std::string processName = pe32.szExeFile;
        // cout << "Process Name: " << processName<<" Process ID: "<<pe32.th32ProcessID<<"location "<<GetProcessFilePath(pe32.th32ProcessID)<<endl;
        // cout<<"file path"<<GetProcessFilePath(pe32.th32ProcessID)<<endl;
        addPair(pairs,pe32.th32ProcessID , GetProcessFilePath(pe32.th32ProcessID));

        if (processName.find("keylog") != std::string::npos || processName.find("key_log") != std::string::npos || processName.find("k_log") != std::string::npos || processName.find("logger") != std::string::npos )
        {
            keyloggerDitected = 1;
            cout << "[!] Keylogger detected..! Process Name: " << processName <<" Process ID: "<<pe32.th32ProcessID<<endl;
            cout<< "[+] Killing the process..."<<endl;
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            // in one liner condition
            if (hProcess != NULL && TerminateProcess(hProcess, 0))
            {
                cout << "[+] Process terminated successfully"<<endl;
            }
            else
            {
                cout << "[-] Unable to terminate the process"<<endl;
            }

            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    if (!keyloggerDitected)
    {
        cout << "[-] No Keylogger named process running"<<endl;
    }
    // clean up the snapshot object.
    CloseHandle(hProcessSnap);
}





int main() {
    std::vector<IntStringPair*> pairVector;
    isKeyloggerRunning(pairVector);
    // Access and print pairs in the vector
    for (const auto& pair : pairVector) {
        std::cout << "Process ID: " << pair->processID << ", File Path: " << pair->filePath << std::endl;
    }
    // helo();
    // using hash to detect keylogger
    // launch the keyLogger



    return 0;

}
