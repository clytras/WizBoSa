
#define SUTILS_TRANSLATE_FROM_BYTES_FORMAT "{bytes}{sign}"
#define SUTILS_TRANSLATE_FROM_BYTES_PRECISION 0

namespace wiznet
{
  class sutils
  {
  public:
    static const char *YesNo(bool expr,
         const char* sYes = "Yes",
         const char* sNo = "No");

    static int wildcmp(const char *wild, const char *string);
    static char *last_strstr(const char *haystack, const char *needle);
    static unsigned long long translate_to_bytes(std::string bytesExp, unsigned long long defval = 0);
    static std::string translate_from_bytes(
			unsigned long long,
			int precision = SUTILS_TRANSLATE_FROM_BYTES_PRECISION,
			const char *format = SUTILS_TRANSLATE_FROM_BYTES_FORMAT
		);
    static std::string translate_from_bytes(
			unsigned long,
			int precision = SUTILS_TRANSLATE_FROM_BYTES_PRECISION,
			const char *format = SUTILS_TRANSLATE_FROM_BYTES_FORMAT
		);
    static std::string translate_from_bytes(
			unsigned int,
			int precision = SUTILS_TRANSLATE_FROM_BYTES_PRECISION,
			const char *format = SUTILS_TRANSLATE_FROM_BYTES_FORMAT
		);

    // trim from start
    static std::string &ltrim(std::string &s);

    // trim from end
    static std::string &rtrim(std::string &s);

    // trim from both ends
    static std::string &trim(std::string &s);

    static bool replace(std::string& str, const std::string& from, const std::string& to);
    static void replaceAll(std::string& str, const std::string& from, const std::string& to);

    static std::string to_string(int value);
  };
}
