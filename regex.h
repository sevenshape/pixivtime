#ifndef BAIKEJSON_REGEX
#define BAIKEJSON_REGEX
#include <string>
#include <regex>
#include <vector>
#include "winapi.h"
namespace regex
{
	bool have(std::string text, std::string re);

	std::string oneSearch(std::string text, std::string re);

	std::string replace(std::string text, std::string re, std::string replacement);

	bool error(std::string re, std::regex reg);

	std::string utf8ToUrl(std::string input);

	std::string toHexString(unsigned char c);

	std::vector<std::string> search(std::string text, std::string re);

	std::string Utf8ToGbk(const char* src_str);

	wchar_t* multiByteToWideChar(std::string pKey);
}
#endif