#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sstream>
#include <algorithm>
#include <functional>
#include <locale>
#include <iomanip>
#include <cmath>
#include "mutils.h"
#include "sutils.h"

using namespace std;

namespace wiznet
{
  const char *sutils::YesNo(bool expr,
             const char* sYes /*= "Yes"*/,
             const char* sNo /*= "No"*/)
  {
    return expr ? sYes : sNo;
  }

  int sutils::wildcmp(const char *wild, const char *string)
  {
    // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    const char *cp = NULL, *mp = NULL;

    while ((*string) && (*wild != '*'))
    {
      if ((*wild != *string) && (*wild != '?'))
      {
        return 0;
      }
      wild++;
      string++;
    }

    while (*string)
    {
      if (*wild == '*')
      {
        if (!*++wild)
        {
          return 1;
        }
        mp = wild;
        cp = string+1;
      }
      else if ((*wild == *string) || (*wild == '?'))
      {
        wild++;
        string++;
      }
      else
      {
        wild = mp;
        string = cp++;
      }
    }

    while (*wild == '*')
    {
      wild++;
    }

    return !*wild;
  }

  /* By liw. */
  char *sutils::last_strstr(const char *haystack, const char *needle)
  {
    if (*needle == '\0')
      return (char *) haystack;

    char *result = NULL;
    for (;;) {
      char *p = strstr((char*)haystack, needle);
      if (p == NULL)
        break;
      result = p;
      haystack = p + 1;
    }

    return result;
  }

  unsigned long long sutils::translate_to_bytes(string bytesExp, unsigned long long defval /*= 0*/)
  {
    unsigned long long isize = defval;
    double dsize = static_cast<double>(defval);
    string size, type;
    int i, type_found = -1, bytesmulty = 0;

    for(i=bytesExp.length()-1; i>=0; i--)
    {
      if(isdigit(bytesExp[i]))

      {
        type_found = i+1;
        break;
      }
    }

    if(type_found != -1)
    {
      size = bytesExp.substr(0, type_found);
      bytesExp = bytesExp.substr(type_found);
      type = trim(bytesExp);

      if(type.compare("KB") == 0)
        bytesmulty = 1;
      else if(type.compare("MB") == 0)
        bytesmulty = 2;
      else if(type.compare("GB") == 0)
        bytesmulty = 3;
      else if(type.compare("TB") == 0)
        bytesmulty = 4;
    }

    stringstream strs(size);
    strs >> dsize;
    if(!strs) dsize = static_cast<double>(defval);

    for(int i=0; i<bytesmulty; i++)
      dsize *= 1024ULL;

    return isize = static_cast<unsigned long long>(floor(dsize + 0.5));
  }

  string sutils::translate_from_bytes(unsigned long bytes,
                    int precision /*= SUTILS_TRANSLATE_FROM_BYTES_PRECISION*/,
                    const char *format /*= SUTILS_TRANSLATE_FROM_BYTES_FORMAT*/)
  {
    return translate_from_bytes(static_cast<unsigned long long>(bytes));
  }

  string sutils::translate_from_bytes(unsigned int bytes,
                    int precision /*= SUTILS_TRANSLATE_FROM_BYTES_PRECISION*/,
                    const char *format /*= SUTILS_TRANSLATE_FROM_BYTES_FORMAT*/)
  {
    return translate_from_bytes(static_cast<unsigned long long>(bytes));
  }

  string sutils::translate_from_bytes(unsigned long long bytes,
                    int precision /*= SUTILS_TRANSLATE_FROM_BYTES_PRECISION*/,
                    const char *format /*= SUTILS_TRANSLATE_FROM_BYTES_FORMAT*/)
  {
    string result(format);
    ostringstream strs;
    string sb[] = { "bytes", "KB", "MB", "GB", "TB", "PB" };
    double eb = floor(log(static_cast<double>(bytes)) / log(1024));

    strs << wiznet::mutils::setprecision(bytes / pow(1024, floor(eb)), precision);

    replaceAll(result, string("{bytes}"), strs.str());
    replaceAll(result, string("{sign}"), sb[static_cast<int>(eb)]);

    return result;
  }

  // trim from start
  string &sutils::ltrim(string &s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }

  // trim from end
  string &sutils::rtrim(string &s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }

  // trim from both ends
  string &sutils::trim(string &s)
  {
    return ltrim(rtrim(s));
  }

  bool sutils::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
      return false;
    str.replace(start_pos, from.length(), to);
    return true;
  }

  void sutils::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
      return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
      str.replace(start_pos, from.length(), to);
      start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
  }

  string sutils::to_string(int value)
  {
    std::ostringstream stm;
    stm << value;
    return stm.str();
  }
}
