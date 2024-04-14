// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <Windows.h>
// #include <wininet.h>

// const std::string API_KEY = "YOUR_API_KEY"; // Replace with your Google API key
// const std::string CX = "YOUR_CUSTOM_SEARCH_ENGINE_ID"; // Replace with your Custom Search Engine ID

// // Function to perform HTTP GET request using WinINet
// std::string performHttpGetRequest(const std::string& url) {
//     HINTERNET hInternet = InternetOpenA("HTTPGET", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
//     HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    
//     std::string readBuffer;
//     char buffer[1024];
//     DWORD bytesRead = 0;
    
//     if (hConnect) {
//         while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
//             readBuffer.append(buffer, bytesRead);
//         }
//         InternetCloseHandle(hConnect);
//     }
    
//     InternetCloseHandle(hInternet);
    
//     return readBuffer;
// }

// // Function to search for a specific word online and append the results to a text file
// void searchAndAppendResults(const std::string& searchWord, const std::string& outputFile) {
//     std::string API_KEY = "AIzaSyCOwb8Rx0lEmTIVhuJCFk62s3S_NCxgnFE";
//     std::string CX = "72312292f759b41d9";
//     std::string apiUrl = "https://www.googleapis.com/customsearch/v1?key=" + API_KEY + "&cx=" + CX + "&q=" + searchWord + "&num=1";
//     std::string jsonResponse = performHttpGetRequest(apiUrl);
    
//     // Append results to the file
//     std::ofstream outFile(outputFile, std::ios::app); // Open the file in append mode
//     if (outFile.is_open()) {
//         outFile << jsonResponse << std::endl;
//         outFile.close();
//         std::cout << "Search results appended to " << outputFile << std::endl;
//     } else {
//         std::cerr << "Error opening file: " << outputFile << std::endl;
//     }
// }

// int main() {
//     std::vector<std::string> searchWords = {"what isprogramming"}; // Change the search words as needed
//     std::string outputFile = "search_results.txt"; // Change this to the desired output file name

//     for (const std::string& word : searchWords) {
//         searchAndAppendResults(word, outputFile);
//     }

//     return 0;
// }



// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstdlib> // For system function


// int main() {
//     std::string searchQuery = "WUDFHost"; // Query with spaces
//     // Replace spaces with %20 to encode them in the URL
//     std::string encodedQuery;
//     for (char c : searchQuery) {
//         if (c == ' ')
//             encodedQuery += "%20";
//         else
//             encodedQuery += c;
//     }
//     // std::string command = "curl -s \"https://api.duckduckgo.com/?q=" + encodedQuery + "&format=json\"";
//     std::string command2 = "curl -s \"https://api.search.brave.com/res/v1/web/search?q=brave+search\" -H \"Accept: application/json\" -H \"Accept-Encoding: gzip\" -H \"X-Subscription-Token: BSAtGbokIfiNZTZppFzDMmQUyjlYiqU\"";
//     std::cout<<command2<<std::endl;
    
//     // Execute the curl command and capture its output
//     std::string jsonResponse;
//     FILE *pipe = _popen(command2.c_str(), "r");
//     if (pipe) {
//         char buffer[128];
//         while (!feof(pipe)) {
//             if (fgets(buffer, 128, pipe) != NULL)
//                 jsonResponse += buffer;
//         }
//         _pclose(pipe);
//     }

//     // Write the JSON response to a file
//     std::ofstream outFile("search_results.json");
//     if (outFile.is_open()) {
//         outFile << jsonResponse;
//         outFile.close();
//         std::cout << "JSON response written to search_results.json" << std::endl;
//     } else {
//         std::cerr << "Error opening file for writing" << std::endl;
//     }

//     return 0;
// }



#include <iostream>
#include <cstdio>

int main() {
    // DuckDuckGo Instant Answer API endpoint
    std::string apiUrl = "https://api.duckduckgo.com/";

    // Search query
    std::string query = "whatis20%programming";

    // Construct the search query URL
    std::string queryUrl = apiUrl + "?q=" + query + "&format=json";

    // Command to execute curl and redirect output to a file
    std::string command = "curl \"" + queryUrl + "\" > search_results.txt";
    std::cout << "Executing command: " << command << std::endl;

    // Execute the command using system()
    int result = system(command.c_str());

    // Check if the command executed successfully
    if (result == 0) {
        std::cout << "Search results saved to search_results.txt" << std::endl;
    } else {
        std::cerr << "Error executing command" << std::endl;
    }

    return 0;
}
