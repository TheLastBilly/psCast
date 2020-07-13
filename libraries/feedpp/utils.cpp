#include "utils.h"
#include "log.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iconv.h>
#include <errno.h>
#include <pwd.h>
#include <libgen.h>

#include <unistd.h>
#include <sstream>
#include <locale>
#include <cwchar>
#include <cstring>
#include <cstdlib>
#include <cstdarg>

#include <curl/curl.h>

#include <langinfo.h>
#include <libxml/uri.h>

#if HAVE_GCRYPT
#include <gnutls/gnutls.h>
#include <gcrypt.h>
#include <errno.h>
#include <pthread.h>
GCRY_THREAD_OPTION_PTHREAD_IMPL;
#endif

#if HAVE_OPENSSL
#include <openssl/crypto.h>
#endif

namespace
{
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
	std::vector<std::string> utils::tokenize_quoted(const std::string& str, std::string delimiters)
	{
		/*
		* This function tokenizes strings, obeying quotes and throwing away comments that start
		* with a '#'.
		*
		* e.g. line: foo bar "foo bar" "a test"
		* is parsed to 4 elements:
		* 	[0]: foo
		* 	[1]: bar
		* 	[2]: foo bar
		* 	[3]: a test
		*
		* e.g. line: yes great "x\ny" # comment
		* is parsed to 3 elements:
		* 	[0]: yes
		* 	[1]: great
		* 	[2]: x
		* 	y
		*
		* 	\", \r, \n, \t and \v are replaced with the literals that you know from C/C++ strings.
		*
		*/
		bool attach_backslash = true;
		std::vector<std::string> tokens;
		std::string::size_type last_pos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = last_pos;

		while (pos != std::string::npos && last_pos != std::string::npos) {
			if (str[last_pos] == '#') // stop as soon as we found a comment
				break;

			if (str[last_pos] == '"') {
				++last_pos;
				pos = last_pos;
				int backslash_count = 0;
				while (pos < str.length() && (str[pos] != '"' || (backslash_count%2))) {
					if (str[pos] == '\\') {
						++backslash_count;
					} else {
						backslash_count = 0;
					}
					++pos;
				}
				if (pos >= str.length()) {
					pos = std::string::npos;
					std::string token;
					while (last_pos < str.length()) {
						if (str[last_pos] == '\\') {
							if (str[last_pos-1] == '\\') {
								if (attach_backslash) {
									token.append("\\");
								}
								attach_backslash = !attach_backslash;
							}
						} else {
							if (str[last_pos-1] == '\\') {
								append_escapes(token, str[last_pos]);
							} else {
								token.append(1, str[last_pos]);
							}
						}
						++last_pos;
					}
					tokens.push_back(token);
				} else {
					std::string token;
					while (last_pos < pos) {
						if (str[last_pos] == '\\') {
							if (str[last_pos-1] == '\\') {
								if (attach_backslash) {
									token.append("\\");
								}
								attach_backslash = !attach_backslash;
							}
						} else {
							if (str[last_pos-1] == '\\') {
								append_escapes(token, str[last_pos]);
							} else {
								token.append(1, str[last_pos]);
							}
						}
						++last_pos;
					}
					tokens.push_back(token);
					++pos;
				}
			} else {
				pos = str.find_first_of(delimiters, last_pos);
				tokens.push_back(str.substr(last_pos, pos - last_pos));
			}
			last_pos = str.find_first_not_of(delimiters, pos);
		}

		return tokens;
	}


	std::vector<std::string> utils::tokenize(const std::string& str, std::string delimiters) {
		/*
		* This function tokenizes a string by the delimiters. Plain and simple.
		*/
		std::vector<std::string> tokens;
		std::string::size_type last_pos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = str.find_first_of(delimiters, last_pos);

		while (std::string::npos != pos || std::string::npos != last_pos) {
			tokens.push_back(str.substr(last_pos, pos - last_pos));
			last_pos = str.find_first_not_of(delimiters, pos);
			pos = str.find_first_of(delimiters, last_pos);
		}
		return tokens;
	}

	std::vector<std::wstring> utils::wtokenize(const std::wstring& str, std::wstring delimiters) {
		/*
		* This function tokenizes a string by the delimiters. Plain and simple.
		*/
		std::vector<std::wstring> tokens;
		std::wstring::size_type last_pos = str.find_first_not_of(delimiters, 0);
		std::wstring::size_type pos = str.find_first_of(delimiters, last_pos);

		while (std::string::npos != pos || std::string::npos != last_pos) {
			tokens.push_back(str.substr(last_pos, pos - last_pos));
			last_pos = str.find_first_not_of(delimiters, pos);
			pos = str.find_first_of(delimiters, last_pos);
		}
		return tokens;
	}

	char* utils::strptime(const char* s, const char* f, struct tm* tm)
	{
		return strptime_i(s, f, tm);
	}

	std::vector<std::string> utils::tokenize_spaced(const std::string& str, std::string delimiters) {
		std::vector<std::string> tokens;
		std::string::size_type last_pos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = str.find_first_of(delimiters, last_pos);

		if (last_pos != 0) {
			tokens.push_back(std::string(" "));
		}

		while (std::string::npos != pos || std::string::npos != last_pos) {
			tokens.push_back(str.substr(last_pos, pos - last_pos));
			last_pos = str.find_first_not_of(delimiters, pos);
			if (last_pos > pos)
				tokens.push_back(std::string(" "));
			pos = str.find_first_of(delimiters, last_pos);
		}

		return tokens;
	}

	std::vector<std::string> utils::tokenize_nl(const std::string& str, std::string delimiters) {
		std::vector<std::string> tokens;
		std::string::size_type last_pos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = str.find_first_of(delimiters, last_pos);
		unsigned int i;

		if (last_pos != std::string::npos)
		{
			for (i=0; i<last_pos; ++i)
			{
				tokens.push_back(std::string("\n"));
			}
		}

		while (std::string::npos != pos || std::string::npos != last_pos)
		{
			tokens.push_back(str.substr(last_pos, pos - last_pos));
			last_pos = str.find_first_not_of(delimiters, pos);
			for (i=0; last_pos != std::string::npos && pos != std::string::npos && i<(last_pos - pos); ++i)
			{
				tokens.push_back(std::string("\n"));
			}
			pos = str.find_first_of(delimiters, last_pos);
		}

		return tokens;
	}

	std::string utils::absolute_url(const std::string& url, const std::string& link) {
		xmlChar * newurl = xmlBuildURI((const xmlChar *)link.c_str(), (const xmlChar *)url.c_str());
		std::string retval;
		if (newurl)
		{
			retval = (const char *)newurl;
			xmlFree(newurl);
		}
		else
		{
			retval = link;
		}
		return retval;
	}

	std::string utils::strprintf(const char * format, ...)
	{
		if (!format)
			return std::string();

		char buffer[1024];

		va_list ap;
		va_start(ap, format);

		unsigned int len = vsnprintf(buffer, sizeof(buffer), format, ap) + 1;

		va_end(ap);
		if (len <= sizeof(buffer))
			return buffer;

		va_start(ap, format);

		char * buf = new char[len];
		vsnprintf(buf, len, format, ap);
		va_end(ap);

		std::string ret(buf);
		delete[] buf;

		return ret;
	}

	void utils::trim(std::string& str)
	{
		while (str.length() > 0 && ::isspace(str[0]))
		{
			str.erase(0,1);
		}
		trim_end(str);
	}

	void utils::trim_end(std::string& str)
	{
		std::string::size_type pos = str.length() - 1;
		while (str.length()>0 && (str[pos] == '\n' || str[pos] == '\r'))
		{
			str.erase(pos);
			pos--;
		}
	}
	void utils::append_escapes(std::string& str, char c)
	{
		switch (c)
		{
		case 'n':
			str.append("\n");
			break;
		case 'r':
			str.append("\r");
			break;
		case 't':
			str.append("\t");
			break;
		case '"':
			str.append("\"");
			break;
		case '\\':
			break;
		default:
			str.append(1, c);
			break;
		}
	}

}
