#include <libconfig.h++>
#include "wizbosa.h"
#include "wiz/console.h"
#include "wiz/utils/sutils.h"
#include "wiz/utils/futils.h"
#include "config.h"


#define DEF_IGNORE_FILES_OVER 0x500000 // 5 MB

using namespace std;
using namespace libconfig;
using namespace wiznet;

namespace wizbosa
{
  Config::Config() :
    m_sSearchDir(""),
    m_ullIgnoreFilesOver(DEF_IGNORE_FILES_OVER)
  {
  };

  bool Config::Load(string cfgFile, bool bNoConfigConfirm)
  {
    bool prnt = !bNoConfigConfirm;
    m_sConfigFile.clear();
    m_vKeywords.clear();
    m_vFilesFilter.clear();

    if (cfgFile.empty()) {
      string cfgFiles[] = WIZBOSA_DEFAUT_CONFIG_FILES;

      for (unsigned int i = 0; i < sizeof(cfgFiles); i++) {
        if (futils::exists(cfgFiles[i])) {
          m_sConfigFile = cfgFiles[i];
          break;
        }
      }

      if (m_sConfigFile.empty())
        cout << "No default configuration file found\n";
    } else
      m_sConfigFile = cfgFile;

    if (!m_sConfigFile.empty()) {
      if (futils::exists(m_sConfigFile)) {
        libconfig::Config config;
        string sTempArg;

        try {
          config.readFile(m_sConfigFile.c_str());
        } catch (libconfig::FileIOException &pex) {
          std::cout << "I/O error while reading the file \"" << m_sConfigFile << endl;
          return false;
        } catch (libconfig::ParseException &pex) {
          std::cout << "Parse error at "
                    << console::fore::red << m_sConfigFile << ":" << pex.getLine()
                    << console::reset << " - " << pex.getError() << endl;
          return false;
        }

        const Setting &wizbosa = config.getRoot();

        try {
          Setting& keywords = wizbosa["wizbosa"]["keywords"];
          int count = keywords.getLength();

          if (count == 0) {
            if (prnt) cout << "No keywords specified. Application will search only for filenames\n";
          } else {
            if (prnt) cout << "Keywords(" << count << "):\n";
            for (int i = 0; i < count; i++) {
              m_vKeywords.push_back(keywords[i]);
              if (prnt) cout << setw(4) << (i+1) << ": " << (const char*)keywords[i] << "\n";
            }
            if (prnt) cout << endl;
          }

          if (!config.lookupValue("wizbosa.use_files_filter", m_bUseFilesFilter))
            m_bUseFilesFilter = true; // default use files filter

          count = 0;

          if (m_bUseFilesFilter) {
            Setting& files_filter = wizbosa["wizbosa"]["files_filter"];
            count = files_filter.getLength();
            if (prnt) cout << "File filters(" << count << "):\n";

            for (int i = 0; i < count; i++) {
              m_vFilesFilter.push_back(files_filter[i]);
              if (prnt) cout << setw(4) << (i+1) << ": " << (const char*)files_filter[i] << "\n";
            }
            if (prnt) cout << endl;
          }

          if (config.exists("wizbosa.alert")) {
            m_bAlert = true;
            if (prnt) cout << "Alerts are On:\n";

            string pad = std::string(4, ' ');

            if (!config.lookupValue("wizbosa.alert.mailto", m_sAlertMailTo))
              m_sAlertMailTo = "root@localhost";
            if (prnt) cout << pad << "Mail To: " << m_sAlertMailTo << endl;


            if (!config.lookupValue("wizbosa.alert.mailfrom", m_sAlertMailFrom))
              m_sAlertMailFrom = "wizbosa@localhost";
            if (prnt) cout << pad << "Mail From: " << m_sAlertMailFrom << endl;

            if (!config.lookupValue("wizbosa.alert.subject", m_sAlertMailSubject))
              m_sAlertMailSubject = "Wizbosa Scan Report";
            if (prnt) cout << pad << "Mail Subject: " << m_sAlertMailSubject << endl;

            if (prnt) cout << endl;
          } else {
            m_bAlert = false;
            if (prnt) cout << "Alerts are Off\n";
          }

          if (count == 0)
            cout << "No file filters specified. Application will search for all files (*)\n";

          if (!config.lookupValue("wizbosa.recurse_subdirectories", m_bRecurseSubdirs))
            m_bRecurseSubdirs = false; // default do not recurse subdirectories

          if (prnt) cout << "Recurse subdirectories: " << sutils::YesNo(m_bRecurseSubdirs) << endl;

          if (!config.lookupValue("wizbosa.search_directory", m_sSearchDir))
            m_sSearchDir = "."; // default directory to current directory

          if (prnt) cout << "Search directory: \"" << m_sSearchDir << "\"\n";

          if (config.lookupValue("wizbosa.ignore_files_over", sTempArg))
            m_ullIgnoreFilesOver = sutils::translate_to_bytes(sTempArg);
          else
            m_ullIgnoreFilesOver = DEF_IGNORE_FILES_OVER; // 5 MB

          if (prnt) cout << "Ignore files over: " << sutils::translate_from_bytes(m_ullIgnoreFilesOver) << endl;

          if (!config.lookupValue("wizbosa.print_ignored_files", m_bPrintIgnoredFiles))
            m_bPrintIgnoredFiles = false;
          
          if (prnt) cout << "Print ignored files: " << sutils::YesNo(m_bPrintIgnoredFiles) << endl;

          if (!config.lookupValue("wizbosa.move_infected_files", m_bMoveInfected))
            m_bMoveInfected = false;

          if (prnt) cout << "Move infected files: " << sutils::YesNo(m_bMoveInfected) << endl;

          if (m_bMoveInfected) {
            if (!config.lookupValue("wizbosa.move_infected_files_to_directory", m_sMoveToPath))
              m_sMoveToPath = "";

            if (prnt) cout << "Move infected files to directory: \"" << m_sMoveToPath << "\"\n";

            if (!config.lookupValue("wizbosa.move_infected_files_create_directory_tree", m_bMoveCreateTree))
              m_bMoveCreateTree = true;

            if (prnt) cout << "Create subdirectories when moving infected files: " << sutils::YesNo(m_bMoveCreateTree) << endl;
          } else {
            m_sMoveToPath = "";
            m_bMoveCreateTree = false;
          }
        } catch (const SettingNotFoundException &nfex) {
          // Ignore.
        }

        return true;
      } else {
        cout << "Configuration file " << "\"" << console::fore::red << m_sConfigFile << console::reset << "\"" << " not found\n";
        return false;
      }
    } else {
      cout << "No configuration file given\n";
      return false;
    }
  }
}
