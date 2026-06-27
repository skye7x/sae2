#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#ifdef _WIN32
  #include <windows.h>
  #include <winsock2.h>
#else
  #include <unistd.h>
  #include <termios.h>
  #include <limits.h>
#endif

using json = nlohmann::json;

// ── Helpers ────────────────────────────────────────────────────────────────

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    out->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Auto-detect machine hostname
std::string getHostname() {
#ifdef _WIN32
    char buf[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buf);
    if (GetComputerNameA(buf, &size)) return std::string(buf);
#else
    char buf[HOST_NAME_MAX];
    if (gethostname(buf, sizeof(buf)) == 0) return std::string(buf);
#endif
    return "unknown";
}

// Read password without echoing characters
std::string readPassword() {
    std::string password;
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);
    std::getline(std::cin, password);
    SetConsoleMode(hStdin, mode);
#else
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    std::cout << std::endl;
    return password;
}

// Load server URL from config file, fall back to default
std::string loadServerUrl() {
    std::ifstream cfg("config.txt");
    if (cfg.is_open()) {
        std::string url;
        std::getline(cfg, url);
        if (!url.empty()) return url;
    }
    return "https://192.168.228.131/api/submissions";
}

// ── Send data ──────────────────────────────────────────────────────────────

bool sendData(const std::string& url, const std::string& payload, int maxRetries = 3) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    for (int attempt = 1; attempt <= maxRetries; attempt++) {
        if (attempt > 1) {
            std::cout << "Retrying... (attempt " << attempt << "/" << maxRetries << ")" << std::endl;
#ifdef _WIN32
            Sleep(2000);
#else
            sleep(2);
#endif
        }

        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize cURL" << std::endl;
            curl_global_cleanup();
            return false;
        }

        std::string responseBuffer;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Connection error: " << curl_easy_strerror(res) << std::endl;
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            continue;
        }

        long httpCode;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (httpCode >= 200 && httpCode < 300) {
            std::cout << "Sent successfully." << std::endl;
            curl_global_cleanup();
            return true;
        } else {
            std::cerr << "Server returned HTTP " << httpCode << ": " << responseBuffer << std::endl;
            curl_global_cleanup();
            return false;
        }
    }

    std::cerr << "Failed after " << maxRetries << " attempts." << std::endl;
    curl_global_cleanup();
    return false;
}

// ── Main ───────────────────────────────────────────────────────────────────

int main() {
    std::string serverUrl = loadServerUrl();
    std::string hostname  = getHostname();

    std::cout << "=== SAE2 Client ===" << std::endl;
    std::cout << "Server : " << serverUrl << std::endl;
    std::cout << "Host   : " << hostname  << std::endl;
    std::cout << std::endl;

    while (true) {
        std::string website, login, password;

        std::cout << "Website  : ";
        std::getline(std::cin, website);
        if (website.empty()) break;

        std::cout << "Login    : ";
        std::getline(std::cin, login);

        std::cout << "Password : ";
        password = readPassword();

        if (login.empty() || password.empty()) {
            std::cerr << "Login and password cannot be empty." << std::endl;
            continue;
        }

        // Confirm
        std::cout << "\n--- Confirm ---" << std::endl;
        std::cout << "Website : " << website << std::endl;
        std::cout << "Login   : " << login   << std::endl;
        std::cout << "Send? (y/n): ";
        std::string confirm;
        std::getline(std::cin, confirm);

        if (confirm != "y" && confirm != "Y") {
            std::cout << "Cancelled." << std::endl << std::endl;
            continue;
        }

        json payload = {
            {"hostname", hostname},
            {"website",  website},
            {"login",    login},
            {"password", password}
        };

        sendData(serverUrl, payload.dump());
        std::cout << std::endl;

        std::cout << "Add another entry? (y/n): ";
        std::string again;
        std::getline(std::cin, again);
        if (again != "y" && again != "Y") break;
        std::cout << std::endl;
    }

    std::cout << "Goodbye." << std::endl;
    return 0;
}