#include "regex.h"

bool regex::have(std::string text, std::string re)
{
	std::regex reg;
	if (regex::error(re, reg)) { return false; }
	bool have = std::regex_search(text, (std::regex)re);
	return have;
}

std::string regex::oneSearch(std::string text, std::string re)
{
	std::regex reg;
	if (regex::error(re, reg)) { return ""; }
	std::smatch matchResults; 
	std::regex_search(text, matchResults, (std::regex)re);
	if (matchResults.size() == 0)
		return "";
	return matchResults[0];
}

std::string regex::replace(std::string text, std::string re, std::string replacement)
{
	std::regex reg;
	if (regex::error(re, reg)) { return ""; }
	std::string replaceResult = std::regex_replace(text, (std::regex)re, replacement);
	return replaceResult;
}

bool regex::error(std::string re, std::regex reg)
{
	try
	{
		reg = (std::regex)re;
	}
	catch (const std::regex_error & e)
	{
		return true;
	}
	return false;
}

std::string regex::utf8ToUrl(std::string input)
{
	std::string str = input;
	std::string re = "[^\\x00-\\x7f]";
	while (regex::have(str, re))
	{
		std::string match_str = regex::oneSearch(str, re);
		str = regex::replace(str, match_str, "%" + toHexString(match_str[0]));
	}
	return str;
}

std::string regex::toHexString(unsigned char c)
{
	char hexChar[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	std::string hexString;
	hexString += hexChar[c / 16];
	hexString += hexChar[c % 16];
	return hexString;
}

std::vector<std::string> regex::search(std::string text, std::string re)
{
	std::vector<std::string> dat;

	std::regex reg;
	if (regex::error(re, reg)) { return dat; }
	std::smatch matchResults;
	while (std::regex_search(text, matchResults, (std::regex)re)) {
		std::string tmp;
		for (std::string x : matchResults) tmp += x + " ";
		dat.push_back(tmp);
		text = matchResults.suffix().str();
	}

	return dat;
}

std::string regex::Utf8ToGbk(const char* src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}


wchar_t* regex::multiByteToWideChar(std::string pKey)
{
	//第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pKey.c_str(), strlen(pKey.c_str()) + 1, NULL, 0);
	wchar_t* pWCStrKey = new wchar_t[pSize];
	//第二次调用将单字节字符串转换成双字节字符串
	MultiByteToWideChar(CP_OEMCP, 0, pKey.c_str(), strlen(pKey.c_str()) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}