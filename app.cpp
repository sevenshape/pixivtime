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

        if (message == "显示调试信息")
        {
            pixivtime::showAppDevMessage = true;
            return;
        }
        if (message == "不显示调试信息")
        {
            pixivtime::showAppDevMessage = false;
            return;
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
            time_t overTime = time(0) + 180;
            std::vector<std::string> imgList;
            api.appSearch(300, word, &imgList);
            std::string base64word = cq::utils::base64_encode((unsigned char*)word.c_str(), word.size());
            int loop = 0;
            do
            {
                std::string b64w = base64word + "_" + std::to_string(loop) + ".jpg";
                api.getAndMergeImg(dirPath, &imgList, b64w);
                cq::send_message(event.target, cq::message::MessageSegment::image("pixivsearch\\" + b64w));
                now = time(0);
                loop++;
            } while (now < overTime && !imgList.empty());
            cq::send_message(event.target, message + " over");
        }
    });
}