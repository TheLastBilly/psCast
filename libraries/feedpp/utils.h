#ifndef _UTIL_H
#define _UTIL_H

#include <sstream>
#include <vector>
#include <cstring>
#include <string>      // due to a bug in Xcode
#include <stdexcept>
#include <locale>
#include <iomanip>

#include <curl/curl.h>
#include <libxml/parser.h>

namespace {
	extern "C" char* strptime_i(const char* s, const char* f, struct tm* tm) {
		std::istringstream input(s);
		input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
		input >> std::get_time(tm, f);
		if (input.fail()) {
			return nullptr;
		}
		return (char*)(s + input.tellg());
	}
}

namespace feedpp {


// wrapped curl handle for exception safety and so on
// see also: https://github.com/gsauthof/ccurl
class curl_handle {
	private:
		CURL *h;
		curl_handle(const curl_handle &);
		curl_handle &operator=(const curl_handle &);
	public:
		curl_handle()
			: h(0) {
			h = curl_easy_init();
			if (!h)
				throw std::runtime_error("Can't obtain curl handle");
		}
		~curl_handle() {
			curl_easy_cleanup(h);
		}
		CURL *ptr() {
			return h;
		}
};

class utils {
	public:
		static std::vector<std::string> tokenize(const std::string& str, std::string delimiters = " \r\n\t");
		static std::vector<std::string> tokenize_spaced(const std::string& str, std::string delimiters = " \r\n\t");
		static std::vector<std::string> tokenize_nl(const std::string& str, std::string delimiters = "\r\n");
		static std::vector<std::string> tokenize_quoted(const std::string& str, std::string delimiters = " \r\n\t");

		static std::vector<std::wstring> wtokenize(const std::wstring& str, std::wstring delimiters = L" \r\n\t");

		static char* strptime(const char* s, const char* f, struct tm* tm)
		{return strptime_i(s, f, tm);}

		static std::string absolute_url(const std::string& url, const std::string& link);

		static std::string strprintf(const char * format, ...);

		static void trim_end(std::string& str);
		static void trim(std::string& str);
  private:
		static void append_escapes(std::string& str, char c);
};

}

#endif /*UTIL_H_*/
