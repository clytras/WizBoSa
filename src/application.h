#include <string.h>
#include <vector>
#include <time.h>
#include "config.h"

using namespace std;

namespace wizbosa
{
  class Application
  {

  };

  class Wizbosa : public Application
  {
  public:
    Wizbosa();
    ~Wizbosa();
    void PrintLogo();
    void PrintSummary();
    void PrintTotalTime();
    void PrintAbort();
    void ShowHelp();
    bool LoadConfigFile(string);
    bool ParseArgs(int argc, const char* argv[]);
    bool Run();
    bool ConfigConfirm();
    void PrintCmdOptsOverrides();
    bool MoveInfectedFile(string);
    wizbosa::Config config;

  private:
    void searchDir(string dir);
    int searchFile(string file, vector<int> &positions);
    void printFileFound(string file, int matches, vector<int> pos);
    void printFileMoved(bool isMoved);
    void resetCounters();
    bool sendAlert();

  public:
    bool HasSearchDir() { return m_bHasArgSearchDir; }
    string SearchDir() { return m_sArgSearchDir; }
    bool HasConfigFile() { return m_bHasArgConfigFile; }
    string ConfigFile() { return m_sConfigFile; }
    bool HasRecurseSubdirs() { return m_bHasArgRecurseSubdirs; }
    bool RecurseSubdirs() { return m_bArgRecurseSubdirs; }
    bool GetShowHelp() { return m_bShowHelp; }
    void SetShowHelp(bool value) { m_bShowHelp = value; }
    bool GetStart() { return m_bStart; }
    bool GetNoConfigConfirm() { return m_bNoConfigConfirm; }

  private:
    string m_sOut;
    string m_sConfigFile;
    bool m_bHasArgSearchDir;
    string m_sArgSearchDir;
    bool m_bHasArgRecurseSubdirs;
    bool m_bArgRecurseSubdirs;
    bool m_bHasArgConfigFile;
    bool m_bShowHelp;
    bool m_bStart;
    bool m_bNoConfigConfirm;
    bool m_bNoPrintIgnored;

    bool m_bHasArgMoveInfected;
    bool m_bArgMoveInfected;
    bool m_bMoveInfected;

    bool m_bMoveCreateTree;
    string m_sMoveToPath;

    string m_sLastMovedfilePath;

    string m_sRealPathSearch;

    int m_iDirsSearched;
    int m_iFilesSearched;
    int m_iFilesWithMatches;
    int m_iFilesIgnoredSizeLimit;
    int m_iFilesMoved;

    clock_t m_ctTimeBegin;
    clock_t m_ctTimeEnd;
  };
}
