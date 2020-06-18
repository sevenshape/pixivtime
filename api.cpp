#include "api.h"

std::map<std::string, std::string> pixivtime::storage = std::map<std::string, std::string>();;
bool pixivtime::showAppDevMessage = false;
int pixivtime::imgRowsConfine = 12000;

std::string api::imgProxy(std::string url)
{
    std::string result = regex::replace(url, R"(i.pximg.net)", R"(pximg.pixiv-viewer.workers.dev)");
    return result;
}

std::string api::url(int id, int index)
{
    if(index > 0)
    {
        return R"(https://pixiv.cat/)" + std::to_string(id) + "-" + std::to_string(index) + ".jpg";
    }
    else
    {
        return R"(https://pixiv.cat/)" + std::to_string(id) + ".jpg";
    }
}

std::string api::getDate()
{
    char date[80] = { 0 };
    
    time_t newTime = time(0);
    tm* timeSt = localtime(&newTime);
    strftime(date, 80, "%Y-%m-%d", timeSt);
    return date;
}

std::string api::search(std::string word, int page)
{
    word = regex::utf8ToUrl(word);
    std::string key = "searchList_" + word + "_" + std::to_string(page);
    if(pixivtime::storage.find(key) == pixivtime::storage.end())
    {
        std::string url = "v2/?type=search&word=" + word + "&page=" + std::to_string(page);//v2/?type=search&word=abc&page=1
        std::string result = http.get(url);
        pixivtime::storage[key] = result;
        return result;
    }
    else
    {
        return pixivtime::storage[key];
    }
}

std::string api::appSearch(std::string word, int markLevel, int page, std::vector<std::string>* res)
{
    std::string bookmark;
    switch (markLevel)
    {
    case 0:
        bookmark = "";
        break;
    case 1:
        bookmark = " 100users入り";
        break;
    case 2:
        bookmark = " 250users入り";
        break;
    case 3:
        bookmark = " 500users入り";
        break;
    case 4:
        bookmark = " 1000users入り";
        break;
    case 5:
        bookmark = " 5000users入り";
        break;
    case 6:
        bookmark = " 10000users入り";
        break;
    case 7:
        bookmark = " 20000users入り";
        break;
    case 8:
        bookmark = " 30000users入り";
        break;
    case 9:
        bookmark = " 50000users入り";
        break;
    default:
        return "书签等级不在0-9";
        break;
    }

    std::string jsonStr = search(word + bookmark, page);
    std::string url = "https://api.imjad.cn/pixiv/v2/?type=search&word=" + regex::utf8ToUrl(word) + "&page=" + std::to_string(page);
    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(jsonStr);
    }
    catch (nlohmann::json::exception& e)
    {
        return "格式错误" + url;
    }

    if (json.find("illusts") != json.end())
    {
        nlohmann::json targetIllusts;
        if (json["illusts"].is_array())
        {
            for (int i = 0; i < json["illusts"].size(); i++)
            {
                if ((json["illusts"][i].find("id") != json["illusts"][i].end()) && json["illusts"][i]["id"].is_number_integer())
                {
                    std::string id = std::to_string(json["illusts"][i]["id"].get<int>());
                    int mangaSize = json["illusts"][i]["meta_pages"].size();
                    if (mangaSize == 0)
                    {
                        res->push_back(id + ".jpg");
                    }
                    else
                    {
                        for(int i=0; i<mangaSize; i++)
                        {
                            res->push_back(id + "-" + std::to_string(i+1) + ".jpg");
                        }
                    }
                }
                else
                {
                    return "id丢失" + url;
                }
            }
        }
        else
        {
            if ((json["illusts"].find("id") != json["illusts"].end()) && json["illusts"]["id"].is_number_integer())
            {
                std::string id = std::to_string(json["illusts"]["id"].get<int>());
                res->push_back(id + ".jpg");
            }
            else
            {
                return "id丢失" + url;
            }
        }
    }
    else
    {
        return "找不到插图" + url;
    }
    return "成功";
}

void api::appSearch(int conSize, std::string word, std::vector<std::string>* imgList)
{
    std::string result;
    int resNum = 0;
    
    int markLevel = 9;
    for(int level=0; level<markLevel+1; level++)
    {
        for(int page=0; ; page++)
        {
            int size = imgList->size();
            appSearch(word, markLevel - level, page, imgList);
            int addSize = imgList->size() - size;
            if(imgList->size() > conSize)
            {
                return;
            }
            if(addSize < 30)
            {
                break;
            }
        }
    }
    
    return;
}

std::string api::getImg(std::string path, std::string name, std::string** img)// img = nullptr
{
    std::ifstream imgFile(path + name, std::ios::binary);
    if(imgFile.is_open() == true)
    {
        imgFile.seekg(0, imgFile.end);
        int length = imgFile.tellg(); 
        imgFile.seekg(0, imgFile.beg);

        img[0] = new std::string();
        img[0]->resize(length);

        imgFile.read(&img[0]->at(0), length);
        return "已读取图片";
    }
    else
    {
        img[0] = http.img.get(name);
        if (img[0] != nullptr)
        {
            imgFile.close();
            std::ofstream create(path + name, std::ios::binary);
            if (create.is_open() == true)
            {
                create.write(&img[0]->at(0), img[0]->size());
                create.flush();
                return "已保存图片";
            }
            else
            {
                return "打不开文件:" + path + name;
            }
        }
        else
        {
            return "404:https://pixiv.cat/" + name;
        }
    }
}

void api::getAndMergeImg(std::string dirPath, std::vector<std::string>* inName, std::string outName)
{
    std::vector<cv::Mat> imgs;
    double sumRows = 0.0;
    int inNumber = inName->size();
    for (int i = 0; i < inNumber; i++)
    {
        std::string* imgData = nullptr;
        getImg(dirPath, inName->at(0), &imgData);
        delete(imgData);
        cv::Mat img = cv::imread(dirPath + inName->at(0), 1);
        if (img.empty())
        {
            remove((dirPath + inName->at(0)).c_str());
            inName->erase(inName->begin());
            continue;
        }
        double rows = img.rows * (double)720 / img.cols;
        if((sumRows + rows) > pixivtime::imgRowsConfine)
        {
            break;
        }
        inName->erase(inName->begin());
        cv::resize(img, img, cv::Size(720, rows));
        imgs.push_back(img);
        sumRows += rows;
    }
    if(imgs.size() == 0)return;
    cv::Mat result(sumRows, 720, imgs[0].type());
    double markRows = 0.0;
    for (int i = 0; i < imgs.size(); i++)
    {
        cv::Rect roiRect = cv::Rect(0, markRows, 720, imgs[i].rows);
        imgs[i].copyTo(result(roiRect));
        markRows += imgs[i].rows;
    }
    cv::imwrite(dirPath + outName, result);
}