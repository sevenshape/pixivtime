#include "http.h"
#include <iostream>
#include "api.h"
#include <corecrt_io.h>

int main()
{
    system("chcp 65001");
    
    api api;

    std::string dirPath = "";
    std::string word = "蕾姆";
    time_t now;
    time_t overTime = time(0) + 180;

    std::vector<pixivtime::Illust> illustList;
    api.appSearch(60, word, &illustList);

    std::vector<std::string> imgList;
    for (int i = 0; i < illustList.size(); i++)
    {
        illustList[i].get_pages(&imgList);
    }
    
    //std::string base64word = cq::utils::base64_encode((unsigned char*)word.c_str(), word.size());
    int loop = 0;
    do
    {
        std::string b64w = word + "_" + std::to_string(loop) + ".jpg";
        api.getAndMergeImg(dirPath, &imgList, b64w);
        if (_access(b64w.c_str(), 0) != -1)
            std::cout << "succe:pixivsearch\\" + b64w;
        else
            std::cout << "error:pixivsearch\\" + b64w;
        now = time(0);
        loop++;
    } while (now < overTime && !imgList.empty());
    
    return 0;
}