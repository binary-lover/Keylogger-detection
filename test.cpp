// #include <windows.h>
// #include <iostream>
// #include <string>

// std::string GetProcessFilePath(DWORD processId) {
//     std::string filePath;
//     HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
//     if (hProcess != NULL) {
//         TCHAR szProcessPath[MAX_PATH];
//         DWORD dwSize = sizeof(szProcessPath) / sizeof(TCHAR);
//         if (QueryFullProcessImageName(hProcess, 0, szProcessPath, &dwSize) != 0) {
//             filePath = szProcessPath;
//         }
//         CloseHandle(hProcess);
//     }
//     return filePath;
// }

// int main() {
//     DWORD processId;
//     std::cout << "Enter the process ID: ";
//     std::cin >> processId;

//     std::string filePath = GetProcessFilePath(processId);
//     if (!filePath.empty()) {
//         std::cout << "Process file path: " << filePath << std::endl;
//     } else {
//         std::cerr << "Failed to get process file path." << std::endl;
//     }

//     return 0;
// }

#include <iostream>
#include <vector>
#include <string>

struct IntStringPair {
    int intValue;
    std::string stringValue;
};

void addPair(std::vector<IntStringPair*>& pairs, int intValue, const std::string& stringValue) {
    IntStringPair* pair = new IntStringPair{intValue, stringValue};
    pairs.push_back(pair);
}

int main() {
    std::vector<IntStringPair*> pairVector;

    // Add pairs using the addPair function
    addPair(pairVector, 10, "Hello");
    addPair(pairVector, 20, "World");

    // Access and print pairs in the vector
    for (const auto& pair : pairVector) {
        std::cout << "Int value: " << pair->intValue << ", String value: " << pair->stringValue << std::endl;
    }

    // Clean up memory by deleting allocated pairs
    for (auto& pair : pairVector) {
        delete pair;
    }
    pairVector.clear(); // Optional: Clear the vector

    return 0;
}
