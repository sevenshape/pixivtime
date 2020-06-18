#ifndef PIXIVVIEW_HTTP
#define PIXIVVIEW_HTTP
#include <string>
#include <vector>

#include <curl/curl.h>

struct http
{
    std::string baseUrl = R"(https://api.imjad.cn/pixiv/)";

    CURL* curlHandle = nullptr;

    static size_t writeFunction(void* buffer, size_t size, size_t nmemb, std::string* writeData);

    std::string get(std::string url);

    struct img
    {
        std::string baseUrl = R"(https://pixiv.cat/)";

        CURL* curlHandle = nullptr;

        static size_t writeFunction(void* buffer, size_t size, size_t nmemb, std::string** writeData);

        std::string* get(std::string url);
    }img;
};
#endif