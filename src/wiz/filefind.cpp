#include <iostream>
#include "utils/sutils.h"
#include "utils/futils.h"
#include "filefind.h"

namespace wiznet
{
  CFileFind::CFileFind() :
      m_bFilterDirs(false) { }

  bool CFileFind::FindFile(string sName)
  {
    return FindFile((const char*)sName.c_str());
  }

  bool CFileFind::FindFile(const char *pName)
  {
    char *ppath = NULL,
       *pexp = NULL;
    bool result = false;

    //m_path[0] = '\0';
    //m_exp[0] = '\0';
    m_path = "";
    m_exp = "";
    m_bHasExp = false;
    m_bHasWildcard = false;

    if((m_bHasWildcard = has_wildcard(pName)) ||
       futils::is_file(pName))
    {
      ppath = futils::dirname(pName);
      pexp = futils::basename(pName);

      //strcpy(m_path, ppath);
      //strcpy(m_exp, pexp);
      m_path = ppath;
      m_exp = pexp;

      free(ppath);
      free(pexp);
    }
    else if(futils::is_dir(pName))
      //strcpy(m_path, pName);
      m_path = pName;
    else
      return false;

    m_pDir = opendir(m_path.c_str());
    m_bHasExp = !m_exp.empty();

    m_realpath = futils::real_path(m_path);

    result = static_cast<bool>(m_pDir);

    /*if(m_pDir)
    {
      m_pEnt = readdir(m_pDir);
      result = static_cast<bool>(m_pEnt);
    }*/

    return result;
  }

  bool CFileFind::FindNextFile()
  {
    bool result = false;

    if(m_pDir)
    {
      while((m_pEnt = readdir(m_pDir)))
      {
        if(!m_bFilterDirs && (IsDirectory() || IsDots()))
        {
          result = true;
          break;
        }

        if(m_bHasExp)
        {
          if(m_bHasWildcard)
          {
            if((result = (sutils::wildcmp(m_exp.c_str(), m_pEnt->d_name)) != 0))
              break;
          }
          else if((result = (m_exp == m_pEnt->d_name))) //(strcmp(m_exp.c_str(), m_pEnt->d_name) == 0))
            break;
        }
        else
        {
          result = true; //static_cast<bool>(m_pEnt);
          break;
        }
      }
    }

    return result;
  }

  void CFileFind::Close()
  {
    if(m_pDir)
      closedir(m_pDir);
  }

  bool CFileFind::has_wildcard(const char *pName)
  {
    return strstr(pName, "*") != NULL || strstr(pName, "?") != NULL;
  }

  string CFileFind::GetFilePath()
  {
    string ret(m_realpath);
    futils::add_slash(ret);
    ret += m_pEnt->d_name;
    return ret;
  }

  bool CFileFind::CompareFileName(string exp)
  {
    return sutils::wildcmp(exp.c_str(), m_pEnt->d_name) != 0;
  }
}
