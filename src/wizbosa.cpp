#include <iostream>
#include <stdio.h>
//#include <google/profiler.h>
#include "application.h"
#include "wizbosa.h"

using namespace std;
using namespace wizbosa;

namespace wizbosa
{
  Wizbosa app;
}

int main(int argc, const char* argv[])
{
  //ProfilerStart("wizbosa.prof");

  bool parseArgs = app.ParseArgs(argc, argv);

  if(!parseArgs)
    return 0;

  app.PrintLogo();

  if(app.GetShowHelp())
  {
    app.ShowHelp();
    return 0;
  }

  if(!app.config.Load(app.ConfigFile(), app.GetNoConfigConfirm()))
  {
    app.ShowHelp();
    return 0;
  }

  //cout << "After show help" << endl;

  app.PrintCmdOptsOverrides();

  if(app.ConfigConfirm())
    if(!app.Run())
      app.PrintAbort();

  //if(app.ConfigConfirm())
  //	cout << "Move file: " << app.MoveInfectedFile("/root/tmp/test.txt") << endl;

  /*cout << "We have " << argc << " arguments:\n";

  for(int i=0; i<argc; i++)
  {
    cout << "\tArg " << (i+1) << ": \"" << argv[i] << "\"\n";
  }*/

  //ProfilerStop();

  return 0;
}
