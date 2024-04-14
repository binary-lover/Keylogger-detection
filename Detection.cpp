// keylogger detection application using C++ and WinAPI

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <wininet.h>
#include <fstream>

using namespace std;

std::string GetProcessFilePath(DWORD processId)
{
    std::string filePath;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess != NULL)
    {
        TCHAR szProcessPath[MAX_PATH];
        DWORD dwSize = sizeof(szProcessPath) / sizeof(TCHAR);
        if (QueryFullProcessImageName(hProcess, 0, szProcessPath, &dwSize) != 0)
        {
            filePath = szProcessPath;
        }
        CloseHandle(hProcess);
    }
    return filePath;
}

struct IntStringPair
{
    int processID;
    std::string filePath;
};

void addPair(std::vector<IntStringPair *> &pairs, int intValue, const std::string &stringValue)
{
    IntStringPair *pair = new IntStringPair{intValue, stringValue};
    pairs.push_back(pair);
}

void isKeyloggerRunning(std::vector<IntStringPair *> &pairs)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateToolhelp32Snapshot failed." << std::endl;
        return;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process, and exit if unsuccessful.
    if (!Process32First(hProcessSnap, &pe32))
    {
        std::cerr << "Process32First failed." << std::endl;
        CloseHandle(hProcessSnap);
        return;
    }

    cout << "[+] Scanning running processes...\n";
    bool keyloggerDitected = 0;
    do
    {
        std::string processName = pe32.szExeFile;
        std::string processName2 = processName;
        addPair(pairs, pe32.th32ProcessID, processName);

        // remove special characters from process name accept space and .
        processName2.erase(std::remove_if(processName2.begin(), processName2.end(), [](char c) { return !std::isalnum(c) && c != ' ' && c != '.'; }), processName2.end());

        if (processName2.find("keylog") != std::string::npos || processName2.find("key_log") != std::string::npos || processName2.find("k_log") != std::string::npos || processName2.find("logger") != std::string::npos || processName2.find("key") != std::string::npos)
        {
            keyloggerDitected = 1;
            cout << "[!] Keylogger detected..! Process Name: " << processName << " Process ID: " << pe32.th32ProcessID << endl;
            cout << "[+] Killing the process..." << endl;
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            // in one liner condition
            if (hProcess != NULL && TerminateProcess(hProcess, 0))
            {
                cout << "[+] Process terminated successfully" << endl;
            }
            else
            {
                cout << "[-] Unable to terminate the process" << endl;
            }

            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    if (!keyloggerDitected)
    {
        cout << "[-] No Keylogger named process running" << endl;
    }
    // clean up the snapshot object.
    CloseHandle(hProcessSnap);
}

std::string performHttpGetRequest(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("HTTPGET", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    
    std::string readBuffer;
    char buffer[1024];
    DWORD bytesRead = 0;
    
    if (hConnect) {
        while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            readBuffer.append(buffer, bytesRead);
        }
        InternetCloseHandle(hConnect);
    }
    
    InternetCloseHandle(hInternet);
    
    return readBuffer;
}

// Function to search for a specific word online and append the results to a text file
void searchAndAppendResults(const std::string& searchWord, const std::string& outputFile) {
    // std::string API_KEY = "AIzaSyCOwb8Rx0lEmTIVhuJCFk62s3S_NCxgnFE";
    // std::string CX = "72312292f759b41d9";
    // std::string apiUrl = "https://www.googleapis.com/customsearch/v1?key=" + API_KEY + "&cx=" + CX + "&q=" + searchWord;
    // std::string jsonResponse = performHttpGetRequest(apiUrl);

    // Using brave search api to search for the word
    string url = "https://en.wikipedia.org/w/api.php?action=query&format=json&list=search&srsearch="+searchWord;
    std::string jsonResponse = performHttpGetRequest(url);
    

    // if value of jsonResponse one of field is totalhits = 0 then dont write to file
    if(jsonResponse.find("totalhits\":0") != std::string::npos)
    {
        // append to another file called not_found.txt
        std::ofstream outFile("not_found.txt", std::ios::app); // Open the file in append mode
        if (outFile.is_open()) {
            outFile << searchWord << std::endl;
            outFile.close();
        } else {
            std::cerr << "Error opening file: " << outputFile << std::endl;
        }

        return;
    }

    // Append results to the file
    std::ofstream outFile(outputFile, std::ios::app); // Open the file in append mode
    if (outFile.is_open()) {
        // if jsonResponce contains "keylogger" or "key_log" or "k_log" or "logger" or "keylog" then highlight the word
        if(jsonResponse.find("keylogger") != std::string::npos || jsonResponse.find("key_log") != std::string::npos || jsonResponse.find("k_log") != std::string::npos || jsonResponse.find("logger") != std::string::npos || jsonResponse.find("keylog") != std::string::npos)
        {
            cout<<"[!] Keylogger detected in search results"<<endl;
            outFile<<"[!] Keylogger detected in search results"<<endl;
        }
        outFile << jsonResponse << std::endl;
        outFile.close();
        // std::cout << "Search results appended to " << outputFile << std::endl;
    } else {
        std::cerr << "Error opening file: " << outputFile << std::endl;
    }
}


int main()
{
    std::vector<IntStringPair *> pairVector;
    isKeyloggerRunning(pairVector);

        // Sort the vector based on file Name
    std::sort(pairVector.begin(), pairVector.end(), [](const IntStringPair *a, const IntStringPair *b) {
        return a->filePath < b->filePath;
    });

    // remove duplicates Process Names from the vector
    pairVector.erase(std::unique(pairVector.begin(), pairVector.end(), [](const IntStringPair *a, const IntStringPair *b) {
        return a->filePath == b->filePath;
    }), pairVector.end());


    // Access and print pairs in the vector
    for (const auto &pair : pairVector)
    {
        // std::cout << "Process ID: " << pair->processID << ", File Path: " << pair->filePath << std::endl;
        std::string searchWord ="what is " + pair->filePath;       // Change this to the word you want to search for
        std::string outputFile = "search_results.txt"; // Change this to the desired output file name
        

        // remove .exe from file name if exists
        // if (searchWord.size() > 4 && searchWord.substr(searchWord.size() - 4) == ".exe")
        //     {
        //         searchWord.erase(searchWord.end()-4, searchWord.end());
        //     }

        // remove [] from filename
        // search for the exact file name
        searchWord.erase(std::remove_if(searchWord.begin(), searchWord.end(), [](char c) { return c == '[' || c == ']'; }), searchWord.end());
        cout << "[+] Searching for: " << searchWord <<"process ID: "<<pair->processID<< endl;
        searchAndAppendResults(searchWord, outputFile);

        // search for the file name without special characters, numbers escept space and . and -
        searchWord.erase(std::remove_if(searchWord.begin(), searchWord.end(), [](char c) { return !std::isalnum(c) && c != ' ' && c != '.' && c != '-'; }), searchWord.end());
        
        //remove numbers from file name
        searchWord.erase(std::remove_if(searchWord.begin(), searchWord.end(), [](char c) { return std::isdigit(c); }), searchWord.end());
        cout << "[+] Searching for: " << searchWord <<"process ID: "<<pair->processID<< endl;
        searchAndAppendResults(searchWord, outputFile);

    }

    // using hash to detect keylogger
    // launch the keyLogger
    return 0;
}