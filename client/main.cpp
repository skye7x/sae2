#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Callback function to write response data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    // Initialize curl
    CURL* curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (!curl) {
        std::cerr << "Failed to initialize cURL" << std::endl;
        return 1;
    }
    
    // Collect user input
    std::string hostname, website, login, password;
    
    std::cout << "=== SAE2 Client ===" << std::endl;
    std::cout << "Please enter the following information:" << std::endl;
    
    std::cout << "Hostname: ";
    std::getline(std::cin, hostname);
    
    std::cout << "Website: ";
    std::getline(std::cin, website);
    
    std::cout << "Login: ";
    std::getline(std::cin, login);
    
    std::cout << "Password: ";
    std::getline(std::cin, password);
    
    // Create JSON payload
    json payload = {
        {"hostname", hostname},
        {"website", website},
        {"login", login},
        {"password", password}
    };
    
    // Display data for confirmation
    std::cout << "\n=== CONFIRMATION ===" << std::endl;
    std::cout << "Hostname: " << hostname << std::endl;
    std::cout << "Website: " << website << std::endl;
    std::cout << "Login: " << login << std::endl;
    std::cout << "Password: " << password << std::endl;
    
    // Ask for confirmation
    std::string confirm;
    std::cout << "\nSend data? (type 'Send' to proceed): ";
    std::getline(std::cin, confirm);
    
    if (confirm != "Send") {
        std::cout << "Operation cancelled." << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    // Setup cURL for HTTPS POST
    std::string url = "https://localhost/api/submissions"; 
    std::string responseBuffer;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.dump().c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
    
    // Set headers
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    // SSL options (assuming self-signed certificate)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable SSL verification for simplicity
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // Set write callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
    
    // Perform the request
    res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 1;
    }
    
    // Get HTTP response code
    long httpResponseCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
    
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        std::cout << "Data sent successfully!" << std::endl;
        std::cout << "Server response: " << responseBuffer << std::endl;
    } else {
        std::cerr << "Failed to send data. HTTP Status Code: " << httpResponseCode << std::endl;
        std::cerr << "Response: " << responseBuffer << std::endl;
    }
    
    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}