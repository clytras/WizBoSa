#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>


using namespace std;

namespace wizbosa
{
  class Config
  {
  public:
    Config();
    bool Load(string = NULL, bool = false);

  public:
    const string& GetSearchDir() const { return m_sSearchDir; }
    bool GetRecurseSubdirectories() { return m_bRecurseSubdirs; }

    vector<string>& GetFilesFilter() { return m_vFilesFilter; }
    vector<string>& GetKeywords() { return m_vKeywords; }
    unsigned long long GetIgnoreFilesOver() { return m_ullIgnoreFilesOver; }
    bool GetPrintIgnoredFiles() { return m_bPrintIgnoredFiles; }

    bool GetSendAlert() { return m_bAlert; }
    const string& GetAlertMailTo() const { return m_sAlertMailTo; }
    const string& GetAlertMailFrom() const { return m_sAlertMailFrom; }
    const string& GetAlertMailSubject() const { return m_sAlertMailSubject; }

    bool GetMoveInfected() { return m_bMoveInfected; }
    bool GetMoveCreateTree() { return m_bMoveCreateTree; }
    const string &GetMoveToPath() const { return m_sMoveToPath; }

  private:
    string m_sConfigFile;
    vector<string> m_vKeywords;
    vector<string> m_vFilesFilter;
    string m_sSearchDir;
    bool m_bUseFilesFilter;
    bool m_bRecurseSubdirs;
    bool m_bAlert;
    string m_sAlertMailTo;
    string m_sAlertMailFrom;
    string m_sAlertMailSubject;
    unsigned long long m_ullIgnoreFilesOver;
    bool m_bPrintIgnoredFiles;
    bool m_bMoveInfected;
    bool m_bMoveCreateTree;
    string m_sMoveToPath;
  };
}
