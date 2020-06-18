#ifndef PIXIVVIEW_API
#define PIXIVVIEW_API
#include <vector>
#include <string>
#include <map>
#include <time.h>
#include <fstream>

#include <nlohmann/json.hpp>
#include "regex.h"
#include "http.h"
#include <opencv2/opencv.hpp>

namespace pixivtime
{
    extern std::map<std::string, std::string> storage;

    extern bool showAppDevMessage;
    
    extern int imgRowsConfine;
}

struct api
{
    http http;

    std::string imgProxy(std::string url);

    std::string url(int id, int index);

    std::string search(std::string word, int page);

    std::string getDate();

    std::string appSearch(std::string word, int markLevel, int page, std::vector<std::string>* imgList);

    void appSearch(int conSize, std::string word, std::vector<std::string>* imgList);

    std::string getImg(std::string path, std::string name, std::string** img);
    
    void getAndMergeImg(std::string dirPath, std::vector<std::string>* inName, std::string outName);
};
#endif