#include <stdlib.h>

#ifdef _WIN32
  #define PATH_SEPARATOR ";"
  #define DIRECTORY_SEPARATOR "\\"
  #define IS_SLASH(c) ((c) == '/' || (c) == '\\')
  #define IS_SLASH_P(c) (*(c) == '/' || *(c) == '\\')
#else
  #define PATH_SEPARATOR ":"
  #define DIRECTORY_SEPARATOR "/"
  #define IS_SLASH(c) ((c) == '/')
  #define IS_SLASH_P(c) (*(c) == '/')
#endif


#define DEFAULT_SLASH DIRECTORY_SEPARATOR[0]

using namespace std;

namespace wiznet
{
  class futils
  {
  public:
    static bool is_dir(string sDir) { return is_dir(sDir.c_str()); };
    static bool is_dir(const char *pDir);
    static bool is_file(string sFile) { return is_file(sFile.c_str()); };
    static bool is_file(const char *pFile);
    static bool exists(const char *pDirFile);
    static bool exists(string sDirFile) { return exists(sDirFile.c_str()); };

    static bool set_current_path(const char *pPath);
    static bool set_current_path(string sPath) { return set_current_path(sPath.c_str()); };
    static char *get_current_path(char *pbuffer);
    static std::string get_current_path();

    static char *basename(const char *pPath, const char *pSuffix = "");
    static char *dirname(const char *pPath);

    static string real_path(string sPath);

    static string& add_slash(string& sPath);
    static string& remove_slash(string& sPath);
    static int mkpath(string s, mode_t mode);

    static const char *os_pathsep;
    static const char *os_dirsep;

  private:
    static size_t _dirname(const char *pPath);
  };
}
