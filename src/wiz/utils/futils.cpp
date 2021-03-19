#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <linux/limits.h>
#include "futils.h"

namespace wiznet
{
  const char *futils::os_pathsep = PATH_SEPARATOR;
  const char *futils::os_dirsep = DIRECTORY_SEPARATOR;

  bool futils::is_dir(const char *pDir)
  {
    bool result = false;
    struct stat st;

    if(lstat(pDir, &st) == 0)
      result = S_ISDIR(st.st_mode);

    return result;
  }

  bool futils::is_file(const char *pFile)
  {
    bool result = false;
    struct stat st;

    if(lstat(pFile, &st) == 0)
      result = S_ISREG(st.st_mode);

    return result;
  }

  bool futils::exists(const char *pDirFile)
  {
    bool result = false;
    struct stat st;

    result = lstat(pDirFile, &st) == 0;

    return result;
  }

  bool futils::set_current_path(const char *pPath)
  {
    return chdir(pPath) == 0;
  }

  char *futils::get_current_path(char *pbuffer)
  {
    return getcwd(pbuffer, PATH_MAX);
  }

  std::string futils::get_current_path()
  {
    char buffer[PATH_MAX];
    return getcwd(buffer, PATH_MAX) ? std::string(buffer) : std::string("");
  }

  // basename from PHP Version 5 source
  char *futils::basename(const char *pPath, const char *pSuffix /*= ""*/)
  {
    //const char *s, size_t len, char *suffix, size_t sufflen, char **p_ret, size_t *p_len

    const char *s = pPath,
           *suffix = pSuffix;
    size_t len = strlen(s),
         sufflen = strlen(suffix);

    //static char buffer[256];
    char *p_ret; // = buffer;
    //size_t *p_len;


    char *ret = NULL, *c, *comp, *cend;
    size_t inc_len, cnt;
    int state;

    c = comp = cend = (char*)s;
    cnt = len;
    state = 0;
    while (cnt > 0) {
      inc_len = (*c == '\0' ? 1: /*php_mblen(c, cnt)*/ 1);

      switch (inc_len) {
        case -2:
        case -1:
          inc_len = 1;
          //php_ignore_value(php_mblen(NULL, 0));
          break;
        case 0:
          goto quit_loop;
        case 1:
#ifdef _WIN32
          if (*c == '/' || *c == '\\') {
#else
          if (*c == '/') {
#endif
            if (state == 1) {
              state = 0;
              cend = c;
            }
          } else {
            if (state == 0) {
              comp = c;
              state = 1;
            }
          }
          break;
        default:
          if (state == 0) {
            comp = c;
            state = 1;
          }
          break;
      }
      c += inc_len;
      cnt -= inc_len;
    }

  quit_loop:
    if (state == 1) {
      cend = c;
    }
    if (suffix != NULL && sufflen < (uint)(cend - comp) &&
        memcmp(cend - sufflen, suffix, sufflen) == 0) {
      cend -= sufflen;
    }

    len = cend - comp;

    //if (p_ret) {
      ret = (char*)malloc(len + 1);
      memcpy(ret, comp, len);
      ret[len] = '\0';
      p_ret = ret;
    //}
    //if (p_len) {
    //	*p_len = len;
    //}

    return p_ret;
  }

  char *futils::dirname(const char *pPath)
  {
    size_t len = strlen(pPath);
    char *path = (char*)calloc(1, len + 1);
    //static char path[len + 1];
    strcpy(path, pPath);

    size_t ret_len = _dirname(path);
    char *ret = (char*)calloc(1, ret_len + 1);
    //static char ret[ret_len + 1];
    strncpy(ret, path, ret_len);
    return ret;
  }

  size_t futils::_dirname(const char *pPath)
  {
    // char *path, size_t len

    size_t len = strlen(pPath);
    char *path = (char*)pPath; //(char*)malloc(len + 1);
    //strcpy(path, pPath);
    //size_t ret;

    register char *end = path + len - 1;
    unsigned int len_adjust = 0;

//#ifdef _WIN32
//		/* Note that on Win32 CWD is per drive (heritage from CP/M).
//		 * This means dirname("c:foo") maps to "c:." or "c:" - which means CWD on C: drive.
//		 */
//		if ((2 <= len) && isalpha((int)((unsigned char *)path)[0]) && (':' == path[1])) {
//			/* Skip over the drive spec (if any) so as not to change */
//			path += 2;
//			len_adjust += 2;
//			if (2 == len) {
//				/* Return "c:" on Win32 for dirname("c:").
//				 * It would be more consistent to return "c:."
//				 * but that would require making the string *longer*.
//				 */
//				return len;
//			}
//		}
//#else
//		/*
//		 * Find the first occurence of : from the left
//		 * move the path pointer to the position just after :
//		 * increment the len_adjust to the length of path till colon character(inclusive)
//		 * If there is no character beyond : simple return len
//		 */
//		char *colonpos = NULL;
//		colonpos = strchr(path, ':');
//		if (colonpos != NULL) {
//			len_adjust = ((colonpos - path) + 1);
//			path += len_adjust;
//			if (len_adjust == len) {
//				return len;
//			}
//		}
//#endif

    if (len == 0) {
      /* Illegal use of this function */
      return 0;
    }

    /* Strip trailing slashes */
    while (end >= path && IS_SLASH_P(end)) {
      end--;
    }

    if (end < path) {
      /* The path only contained slashes */
      path[0] = DEFAULT_SLASH;
      path[1] = '\0';
      return 1 + len_adjust;
    }

    /* Strip filename */
    while (end >= path && !IS_SLASH_P(end)) {
      end--;
    }

    if (end < path) {
      /* No slash found, therefore return '.' */
      path[0] = '.';
      path[1] = '\0';
      return 1 + len_adjust;
    }

    /* Strip slashes which came before the file name */
    while (end >= path && IS_SLASH_P(end)) {
      end--;
    }

    if (end < path) {
      path[0] = DEFAULT_SLASH;
      path[1] = '\0';
      return 1 + len_adjust;
    }

    *(end+1) = '\0';

    size_t ret = (size_t)(end + 1 - path) + len_adjust;
    //free(path);

    return ret;
  }

  string futils::real_path(string sPath)
  {
    char *prp = realpath(sPath.c_str(), NULL);
    string ret(prp ? prp : "");
    free(prp);
    return ret;
  }

  string& futils::add_slash(string& sPath)
  {
    if(!IS_SLASH(sPath[sPath.length() - 1]))
      sPath += DIRECTORY_SEPARATOR;

    return sPath;
  }

  string& futils::remove_slash(string &sPath)
  {
    if(IS_SLASH(sPath[sPath.length() - 1]))
      sPath.erase(sPath.length() - 1, 1); //  [sPath.length() - 1] = "";

    return sPath;
  }

  int futils::mkpath(string s, mode_t mode)
  {
    size_t pre = 0, pos;
    std::string dir;
    int mdret;

    if(s[s.size()-1]!='/')
    {
      // force trailing / so we can handle everything in loop
      s+='/';
    }

    while((pos = s.find_first_of('/', pre)) != std::string::npos)
    {
      dir = s.substr(0, pos++);
      pre = pos;
      if(dir.size() == 0) continue; // if leading / first time is 0 length
      if((mdret = mkdir(dir.c_str(), mode)) && errno != EEXIST)
      {
        return mdret;
      }
    }

    return mdret;
  }
}
