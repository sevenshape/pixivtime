#include <cqcppsdk/cqcppsdk.hpp>
#include "api.h"
#include <io.h>
#include <direct.h>

CQ_INIT
{
    cq::on_enable([] { curl_global_init(CURL_GLOBAL_ALL); });

    cq::on_private_message([](const cq::PrivateMessageEvent& event)
    {
        std::string message = cq::message::unescape(event.message);
        std::string mesHead;

        if (message == "r18Confine on")
        {
            pixivtime::r18Confine = true;
            return;
        }
        if (message == "r18Confine off")
        {
            pixivtime::r18Confine = false;
            return;
        }
        mesHead = regex::oneSearch(message, R"(^numberConfine [123456789]\d{0,8}$)");
        mesHead = regex::oneSearch(message, R"(\d+)");
        if (mesHead != "")
        {
            pixivtime::numberConfine = atoi(mesHead.c_str());
            pixivtime::timeConfine = pixivtime::numberConfine * 4;//得数为0至3,999,999,996‬
        }

        mesHead = regex::oneSearch(message, R"(^.*?time$)");
        if (mesHead != "")
        {
            api api;
            std::string dirPath = cq::utils::ansi(cq::dir::root("data", "image") + "pixivsearch");
            if (_access(dirPath.c_str(), 0) == -1)
            {
                _mkdir(dirPath.c_str());
                if (_access(dirPath.c_str(), 0) == -1)
                {
                    cq::send_message(event.target, "创建目录失败:" + dirPath);
                    return;
                }
            }
            dirPath += "\\";
            std::string word = mesHead.substr(0, mesHead.size() - 4);
            time_t now;
            time_t overTime = time(0) + pixivtime::timeConfine;
            std::vector<pixivtime::Illust> illustList;
            api.appSearch(pixivtime::numberConfine, word, &illustList);
            std::vector<std::string> imgList;
            for (int i = 0; i < illustList.size(); i++)
            {
                illustList[i].get_pages(&imgList);
            }
            std::string base64word = cq::utils::base64_encode((unsigned char*)word.c_str(), word.size());
            int loop = 0;
            do
            {
                std::string b64w = base64word + "_" + std::to_string(loop) + ".jpg";
                api.getAndMergeImg(dirPath, &imgList, b64w);
                if (_access((dirPath + b64w).c_str(), 0) != -1)
                    cq::send_message(event.target, cq::message::MessageSegment::image("pixivsearch\\" + b64w));
                else
                    cq::send_message(event.target, "pixivsearch\\" + b64w);
                now = time(0);
                loop++;
            } while (now < overTime && !imgList.empty());
            cq::send_message(event.target, message + " over");
        }
    });
}