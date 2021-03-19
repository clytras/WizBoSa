#include <dirent.h>
#include <string.h>
#include <linux/limits.h>

#define WIZ_CFF_REGULAR_FILE DT_REG
#define WIZ_CFF_DIRECTORY_FILE DT_DIR

using namespace std;

namespace wiznet
{
  class CFileFind
  {
  public:
    CFileFind();

  public:
    bool FindFile(string sName);
    bool FindFile(const char *pName);
    bool FindNextFile();
    void Close();

    string GetFileName() { return string(m_pEnt->d_name); }
    string GetFilePath();
    const string& GetRootDirectory() const { return m_path; }
    const string& GetFileExpression() const { return m_exp; }

    int FType() { return (int)m_pEnt->d_type; }

    bool IsFile() { return m_pEnt->d_type == WIZ_CFF_REGULAR_FILE; }
    bool IsDirectory() { return m_pEnt->d_type == WIZ_CFF_DIRECTORY_FILE; };
    bool IsDots() { return strcmp(m_pEnt->d_name, ".") == 0 || strcmp(m_pEnt->d_name, "..") == 0; };
    void SetFilterDirectories(bool value = true) { m_bFilterDirs = value; }
    bool GetFilterDirectories() { return m_bFilterDirs; }
    bool CompareFileName(string exp);

  private:
    bool has_wildcard(const char *pName);

  private:
    DIR *m_pDir;
    struct dirent *m_pEnt;
    string m_path;
    string m_realpath;
    string m_exp;
    bool m_bHasExp;
    bool m_bHasWildcard;
    bool m_bFilterDirs;
  };
}
