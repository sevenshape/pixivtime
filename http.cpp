#include "http.h"

size_t http::writeFunction(void* buffer, size_t size, size_t nmemb, std::string* writeData)
{
    std::string tmp = std::string((char*)buffer, nmemb);
    *writeData += tmp;
    return size * nmemb;
}

std::string http::get(std::string url)
{
    std::string result;
    url = baseUrl + url;
    
    if (curlHandle == NULL)
    {
        curlHandle = curl_easy_init();
        curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeFunction);
    }
    
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &result);
    curl_easy_perform(curlHandle);
    
    return result;
}

size_t http::img::writeFunction(void* buffer, size_t size, size_t nmemb, std::string** writeData)
{
    std::string tmp = std::string((char*)buffer, nmemb);
    *writeData[0] += tmp;
    return size * nmemb;
}

std::string* http::img::get(std::string url)
{
    std::string* result = new std::string();
    url = baseUrl + url;
    
    if (curlHandle == NULL)
    {
        curlHandle = curl_easy_init();
        curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1);
        //curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 60);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeFunction);
    }
    
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &result);
    CURLcode curlCode = curl_easy_perform(curlHandle);

    if (curlCode == 0)
    {
        return result;
    }
    else
    {
        delete(result);
        return nullptr;
    }
}