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

    extern bool r18Confine;

    extern unsigned int timeConfine;

    extern int numberConfine;

    class Illust
    {
    public:
        int id;
        int x_restrict;
        nlohmann::json meta_single_page;
        nlohmann::json meta_pages;
        int total_bookmarks;

        Illust(nlohmann::json json)
        {
            this->id = json["id"].get<int>();
            this->x_restrict = json["x_restrict"].get<int>();
            this->meta_single_page = json["meta_single_page"];
            this->meta_pages = json["meta_pages"];
            this->total_bookmarks = json["total_bookmarks"].get<int>();
        }

        bool Illust::operator==(const Illust& rhs) const
        {
            return (id == rhs.id);
        }

        bool is_single_page()
        {
            return meta_single_page.size();
        }

        void get_pages(std::vector<std::string>* pagesList)
        {
            if (is_single_page())
                pagesList->push_back( std::to_string(id) + ".jpg" );
            else
            {
                for (int i = 0; i < meta_pages.size(); i++)
                {
                    pagesList->push_back(std::to_string(id) + "-" + std::to_string(i + 1) + ".jpg");
                }
            }
        }
    };
}

struct api
{
    http http;

    std::string imgProxy(std::string url);

    std::string url(int id, int index);

    std::string search(std::string word, int page);

    std::string getDate();

    std::string appSearch(std::string word, int markLevel, int page, std::vector<pixivtime::Illust>* imgList, int* addSizeOut);

    void appSearch(int conSize, std::string word, std::vector<pixivtime::Illust>* imgList);

    std::string getImg(std::string path, std::string name, std::string** img);
    
    void getAndMergeImg(std::string dirPath, std::vector<std::string>* inName, std::string outName);
};
#endif