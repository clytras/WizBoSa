#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <ctype.h>
#include <math.h>
#include "wiz/filefind.h"
#include "wiz/utils/futils.h"
#include "wiz/utils/nutils.h"
#include "wiz/utils/sutils.h"
#include "wiz/console.h"
#include "wizbosa.h"
#include "application.h"

using namespace std;
using namespace wiznet;

#define FSRCH_SIZELIMITIGNORE -1

namespace wizbosa
{
  Wizbosa::Wizbosa() :
      //m_sConfigFile(""),
      m_bHasArgSearchDir(false),
      //m_sArgSearchDir("."),
      m_bHasArgRecurseSubdirs(false),
      m_bArgRecurseSubdirs(false),
      m_bHasArgConfigFile(false),
      m_bShowHelp(false),
      m_bStart(false),
      m_bNoConfigConfirm(false),
      m_bMoveInfected(false),
      m_bMoveCreateTree(false),
      m_bHasArgMoveInfected(false)
  {
    m_sArgSearchDir = ".";
    m_sConfigFile = "";
    m_sMoveToPath = "";
  }

  Wizbosa::~Wizbosa()
  {
  }

  bool Wizbosa::Run()
  {
    resetCounters();

    //char *prealpath = realpath(m_bHasArgRecurseSubdirs ? m_sArgSearchDir : config.GetSearchDir());
    m_sRealPathSearch = m_bHasArgSearchDir ? m_sArgSearchDir : config.GetSearchDir();

    if(futils::exists(m_sRealPathSearch))
      m_sRealPathSearch = futils::real_path(m_sRealPathSearch);
    else
    {
      cerr << "Search directory \"" << m_sRealPathSearch << "\" not found.\n";
      return false;
    }

    m_bMoveInfected = m_bHasArgMoveInfected ? m_bArgMoveInfected : config.GetMoveInfected();
    m_sMoveToPath = config.GetMoveToPath();
    m_bMoveCreateTree = config.GetMoveCreateTree();

    if(m_bMoveInfected && m_sMoveToPath.length() > 0)
    {
      if(futils::exists(m_sMoveToPath))
      {
        m_sMoveToPath = futils::real_path(m_sMoveToPath);
        m_bMoveInfected = true;
      }
      else
      {
        if(futils::mkpath(m_sMoveToPath, 0755) != 0)
        {
          cerr << "Failed to create move to directory \"" << m_sMoveToPath << "\".\nAborting search.\n";
          return false;
        }

        m_sMoveToPath = futils::real_path(m_sMoveToPath);
        m_bMoveInfected = true;
      }

      futils::add_slash(m_sMoveToPath);
    }

    cout << endl;

    if(m_bStart)
    {
      cout << "Begin searching path: " << m_sRealPathSearch << endl << endl;

      m_ctTimeBegin = clock();

      searchDir(m_sRealPathSearch);

      m_ctTimeEnd = clock();

      PrintSummary();
      PrintTotalTime();
      sendAlert();
    }
    else
    {
      cout << "Apply -s parameter to start search process.\n";
    }

    return true;
  }

  void Wizbosa::PrintCmdOptsOverrides()
  {
    if(m_bHasArgSearchDir)
      cout << endl << "Override option for search directory: \"" << m_sArgSearchDir << "\"" << endl;

    if(m_bHasArgMoveInfected)
      cout << endl << "Override option for move infected files: " << sutils::YesNo(m_bArgMoveInfected) << endl;
  }

  void Wizbosa::PrintAbort()
  {
    cout << "Aborting search.\n";
  }

  bool Wizbosa::ConfigConfirm()
  {

    if(m_bNoConfigConfirm || !m_bStart)
      return true;

    char c;
    bool result = false;

    cout << endl << "Are you sure you want to begin the search process with the above options? [Y/n]";

    for(;;)
    {
      c = console::getch();

      if(tolower(c) == 'y')
      {
        result = true;
        break;
      }
      else if(tolower(c) == 'n' || int(c) == 27)
        break;
    }

    cout << endl;
    return result;
  }

  bool Wizbosa::sendAlert()
  {
    if(config.GetSendAlert())
    {
      return nutils::sendmail(config.GetAlertMailTo().c_str(), config.GetAlertMailFrom().c_str(), config.GetAlertMailSubject().c_str(), m_sOut.c_str()) == 0;
    }
  }

  void Wizbosa::resetCounters()
  {
    m_iDirsSearched = 0;
    m_iFilesSearched = 0;
    m_iFilesWithMatches = 0;
    m_iFilesIgnoredSizeLimit = 0;
    m_iFilesMoved = 0;
    m_sOut.clear();
  }

  void Wizbosa::printFileFound(string file, int matches, vector<int> pos)
  {
    string smatches(matches > 1 ? " matches" : " match");
    std::ostringstream sout;
    std::ostringstream snfout;
    sout << console::fore::lightwhite << "("
       << console::fore::red << ">"
       << console::fore::lightwhite << ")"
       << console::reset << ": "
       << file
       << " ["
       << console::fore::cyan
       << matches << smatches
       << console::reset
       << "] ("
       << console::fore::green
       << "line "
       << (pos[1] + 1)
       << console::reset
       << ")" << endl;

    snfout << "(>): " << file << " [" << matches << smatches << "] (line " << (pos[1] + 1) << ")" << endl;

    cout << sout.str();
    m_sOut += snfout.str();
  }

  void Wizbosa::printFileMoved(bool isMoved)
  {
    std::ostringstream sout;
    std::ostringstream snfout;

    sout << (isMoved ? console::fore::cyan : console::fore::yellow)
       << (isMoved ? "     moved: " : " move fail: ")
       << (isMoved ? console::fore::green : console::fore::red)
       << m_sLastMovedfilePath << console::reset << endl;

    snfout << (isMoved ? "     moved: " : " move fail: ") << m_sLastMovedfilePath << endl;

    cout << sout.str();
    m_sOut += snfout.str();
  }

  bool Wizbosa::MoveInfectedFile(string file)
  {
    string dest;

    //m_sRealPathSearch = m_bHasArgSearchDir ? m_sArgSearchDir : config.GetSearchDir();
    //m_sMoveToPath = futils::real_path(m_sMoveToPath);
    //futils::add_slash(m_sMoveToPath);

    //cout << "m_sRealPathSearch: " << m_sRealPathSearch << endl;
    //cout << "m_sMoveToPath: " << m_sMoveToPath << endl;
    //cout << "MoveInfectedFile with '" << file << "'" << endl;

    if(m_bMoveCreateTree)
    {
      //cout << "--move-create-dirtree" << endl;


      m_sLastMovedfilePath = m_sMoveToPath + file.substr(m_sRealPathSearch.length() + 1);
      string destdir = futils::dirname(m_sLastMovedfilePath.c_str());

      //cout << "DEBUG: Creating path '" << destdir << "'" << endl;

      //futils::mkpath(string(futils::dirname(m_sLastMovedfilePath.c_str())), (mode_t)0755);
      futils::mkpath(destdir, (mode_t)0755);
    }
    else
    {
      int i = 0;
      m_sLastMovedfilePath = m_sMoveToPath + futils::basename(file.c_str());
      while(futils::exists(m_sLastMovedfilePath))
      {
        m_sLastMovedfilePath =  m_sMoveToPath + futils::basename(file.c_str());
        m_sLastMovedfilePath += "." + sutils::to_string(++i);
      }
    }

    //cout << "DEBUG: Moving '" << file << "' to '" << m_sLastMovedfilePath << "'" << endl;

    //return true;

    return rename(file.c_str(), m_sLastMovedfilePath.c_str()) == 0;
  }

  void Wizbosa::searchDir(string dir)
  {
    CFileFind find;

    bool bWorking = find.FindFile(dir);

    if(bWorking)
    {
      m_iDirsSearched++;

      while((bWorking = find.FindNextFile()))
      {
        if(!find.IsDots())
        {
          if(find.IsDirectory() && config.GetRecurseSubdirectories())
          {
            searchDir(find.GetFilePath());
          }
          else if(find.IsFile())
          {
            vector<string>& ff = config.GetFilesFilter();
            unsigned int iff;
            bool bProcessFile;

            for(iff = 0; iff < ff.size(); iff++)
            {
              if(ff[iff].compare("!ext") == 0)
              {
                bProcessFile = find.GetFileName().find(".") == string::npos;
              }
              else
              {
                bProcessFile = find.CompareFileName(ff[iff]);
              }

              //if(find.CompareFileName(ff[iff]))
              if(bProcessFile) 
              {
                //cout << "Found file(" << ff[iff] << "): " << find.GetFilePath() << endl;
                vector<int> positions;
                int matches = searchFile(find.GetFilePath(), positions);

                if(matches > 0)
                {
                  //cout << "Found file with matches(" << matches << "): " << find.GetFilePath() << endl;
                  printFileFound(find.GetFilePath(), matches, positions);
                  m_iFilesWithMatches++;

                  if(m_bMoveInfected)
                  {
                    if(MoveInfectedFile(find.GetFilePath()))
                    {
                      printFileMoved(true);
                      m_iFilesMoved++;
                    }
                    else
                    {
                      printFileMoved(false);
                    }
                  }
                }
                else if(matches == FSRCH_SIZELIMITIGNORE)
                {
                  cout << "Ignoring big file: " << find.GetFilePath() << endl;
                  m_iFilesIgnoredSizeLimit++;
                }

                if(matches >= 0) m_iFilesSearched++;
              }
            }
          }
        }
      }

      find.Close();
    }
  }

  int Wizbosa::searchFile(string file, vector<int> &positions)
  {
    int result = 0;
    size_t found;
    vector<string>& kw = config.GetKeywords();
    ifstream ifs(file.c_str());

    ifs.seekg(0, ios::end);
    size_t size = ifs.tellg();

    if(size <= config.GetIgnoreFilesOver())
    {
      string buffer(size, '\0');

      ifs.seekg(0);
      ifs.read(&buffer[0], size);
      ifs.close();

      for(unsigned int i=0; i<kw.size(); i++)
      {
        found = buffer.find(kw[i]);
        if(found != string::npos)
        {
          positions.push_back(static_cast<int>(found));
          //std::vector<int> bufvect (buffer, buffer[found]);
          //size_t n = std::count(bufvect.begin(), bufvect.end(), '\n');
          size_t n = std::count(buffer.begin(), buffer.begin()+found, '\n');
          positions.push_back(static_cast<int>(n));
          result++;
        }
      }
    }
    else
    {
      result = FSRCH_SIZELIMITIGNORE;
    }

    return result;
  }

  void Wizbosa::PrintLogo()
  {
    char buffer[200];
    sprintf((char*)buffer, WIZBOSA_VERSION_NAME, WIZBOSA_VERSION_MAJOR, WIZBOSA_VERSION_MINOR, WIZBOSA_VERSION_REVISION);

    WIZBOSA_PRINT_HEADER(buffer) << endl;
    //cout << buffer << endl << endl;
  }

  void Wizbosa::PrintSummary()
  {
    cout << endl;
    WIZBOSA_PRINT_HEADER("Search summary") << endl;

    std::ostringstream sout;
    sout << "Files searched: " << m_iFilesSearched << endl
       << "Directories parsed: " << m_iDirsSearched << endl
       << "Files with matches: " << m_iFilesWithMatches << endl
       << "Files ignored due to size limit: " << m_iFilesIgnoredSizeLimit << endl;


    if(m_bMoveInfected)
    {
      sout << "Infected files moved: " << m_iFilesMoved << endl;
      if(m_iFilesMoved != m_iFilesWithMatches)
        sout << "Infected files failed to moved: " << (m_iFilesWithMatches - m_iFilesMoved) << endl;
    }

    sout << endl;

    cout << sout.str();
    m_sOut += sout.str();
  }

  void Wizbosa::PrintTotalTime()
  {
    float cs = static_cast<float>(m_ctTimeEnd - m_ctTimeBegin) / CLOCKS_PER_SEC;
    int ts = static_cast<int>(floor(cs));
    std::ostringstream sout;

    if(ts > 0)
    {
      int days = ts / 60 / 60 / 24,
        secs = ts % 60,
        mins = (ts / 60) % 60,
        hours = (ts / 60 / 60) % 24;
      float msecs = secs + static_cast<float>(cs - ts);

      sout << "Search finished after ";

      if(days > 0) sout << days << " day(s) ";
      if(hours > 0) sout << hours << " hour(s) ";
      if(mins > 0) sout << mins << " minute(s) ";
      if(msecs > 0) sout << msecs << " second(s) ";
      //if(msecs > 0) sout << msecs << " milisecods ";

      sout << endl;

      cout << sout.str();
      m_sOut += sout.str();
    }
  }

  void Wizbosa::ShowHelp()
  {
    cout << endl;
    WIZBOSA_PRINT_HEADER("Command line options");
    //cout << "Command line options:\n";
    cout << "  -s, --start\t\t\tStart searching\n";
    cout << "  -c <file>, --config <file>\tSpecify a config file (wizbosa.cfg will be used is no file is specified)\n";
    cout << "  -ncc, --no-config-confirm\tNo config file confirmation\n";
    cout << "  -p <path>, --path <path>\tSpecify the path to search (overrides config file path)\n";
    cout << "  -mv <path>, --moveto <path>\tSpecify the path to move infected files\n";
    cout << "  --move-create-dirtree\t\tCreate base directory tree for infected files on move destination directory\n";
    cout << "  -nmvi, -no-move-infected\tDo not move infected files found\n";
    cout << "  -mv, --moveto\tMove infected files to directory\n";



    cout << "  -h, --help\t\t\tShow help\n\n";
  }

  bool Wizbosa::ParseArgs(int argc, const char* argv[])
  {
//		cout << "Arguments count " << argc << endl;
//
//		for(int i=0; i<argc; i++)
//			cout << "Arg " << i << ": \"" << argv[i] << "\"\n";

    int i=0;
    while(i < argc)
    {
      if(strcmp(argv[i], "-c") == 0 ||
         strcmp(argv[i], "--config") == 0)
      {
        m_bHasArgConfigFile = true;
        m_sConfigFile = argv[++i];
      }
      else if(strcmp(argv[i], "-p") == 0 ||
          strcmp(argv[i], "--path") == 0)
      {
        m_bHasArgSearchDir = true;

        if(++i < argc)
          m_sArgSearchDir = argv[i];
        else
        {
          cerr << "Option -p must be followed by a valid path.\nAborting search.\n";
          return false;
        }
      }
      else if(strcmp(argv[i], "-h") == 0 ||
          strcmp(argv[i], "--help") == 0)
      {
        m_bShowHelp = true;
      }
      else if(strcmp(argv[i], "-s") == 0 ||
          strcmp(argv[i], "--start") == 0)
      {
        m_bStart = true;
      }
      else if(strcmp(argv[i], "-ncc") == 0 ||
          strcmp(argv[i], "--no-config-confirm") == 0)
      {
        m_bNoConfigConfirm = true;
      }
      else if(strcmp(argv[i], "-mvi") == 0 ||
          strcmp(argv[i], "--move-infected") == 0)
      {
        m_bHasArgMoveInfected = true;
        m_bArgMoveInfected = true;
      }
      else if(strcmp(argv[i], "-nmvi") == 0 ||
          strcmp(argv[i], "--no-move-infected") == 0)
      {
        m_bHasArgMoveInfected = true;
        m_bArgMoveInfected = false;
      }
      else if(strcmp(argv[i], "-mv") == 0 ||
          strcmp(argv[i], "--moveto") == 0)
      {
        if(++i < argc)
          m_sMoveToPath = argv[i];
        else
        {
          cerr << "Option -mv|--moveto must be followed by a valid path.\nAborting search.\n";
          return false;
        }
      }
      else if(strcmp(argv[i], "--move-create-dirtree") == 0)
      {
        m_bMoveCreateTree = true;
      }

      i++;
    }

    return true;
  }
}
