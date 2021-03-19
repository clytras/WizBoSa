#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "console.h"

#define CC_CONSOLE_COLOR_DEFAULT "\033[0m"
#define CC_FORECOLOR(C) "\033[" #C "m"
#define CC_BACKCOLOR(C) "\033[" #C "m"
#define CC_ATTR(A) "\033[" #A "m"

namespace wiznet
{
  enum Color
  {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Default = 9
  };

  enum Attributes
  {
    Reset,
    Bright,
    Dim,
    Underline,
    Blink,
    Reverse,
    Hidden
  };

  char *console::color(int attr, int fg, int bg)
  {
    static char command[13];

    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    return command;
  }

  char console::getch()
  {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
  }

  const char *console::reset = CC_CONSOLE_COLOR_DEFAULT;
  const char *console::underline = CC_ATTR(4);

  const char *console::fore::black = CC_FORECOLOR(30);
  const char *console::fore::blue = CC_FORECOLOR(34);
  const char *console::fore::red = CC_FORECOLOR(31);
  const char *console::fore::magenta = CC_FORECOLOR(35);
  const char *console::fore::green = CC_FORECOLOR(92);
  const char *console::fore::cyan = CC_FORECOLOR(36);
  const char *console::fore::yellow = CC_FORECOLOR(33);
  const char *console::fore::white = CC_FORECOLOR(37);
  const char *console::fore::console = CC_FORECOLOR(39);

  const char *console::fore::lightblack = CC_FORECOLOR(90);
  const char *console::fore::lightblue = CC_FORECOLOR(94);
  const char *console::fore::lightred = CC_FORECOLOR(91);
  const char *console::fore::lightmagenta = CC_FORECOLOR(95);
  const char *console::fore::lightgreen = CC_FORECOLOR(92);
  const char *console::fore::lightcyan = CC_FORECOLOR(96);
  const char *console::fore::lightyellow = CC_FORECOLOR(93);
  const char *console::fore::lightwhite = CC_FORECOLOR(97);

  const char *console::back::black = CC_BACKCOLOR(40);
  const char *console::back::blue = CC_BACKCOLOR(44);
  const char *console::back::red = CC_BACKCOLOR(41);
  const char *console::back::magenta = CC_BACKCOLOR(45);
  const char *console::back::green = CC_BACKCOLOR(42);
  const char *console::back::cyan = CC_BACKCOLOR(46);
  const char *console::back::yellow = CC_BACKCOLOR(43);
  const char *console::back::white = CC_BACKCOLOR(47);
  const char *console::back::console = CC_BACKCOLOR(49);

  const char *console::back::lightblack = CC_BACKCOLOR(100);
  const char *console::back::lightblue = CC_BACKCOLOR(104);
  const char *console::back::lightred = CC_BACKCOLOR(101);
  const char *console::back::lightmagenta = CC_BACKCOLOR(105);
  const char *console::back::lightgreen = CC_BACKCOLOR(102);
  const char *console::back::lightcyan = CC_BACKCOLOR(106);
  const char *console::back::lightyellow = CC_BACKCOLOR(103);
  const char *console::back::lightwhite = CC_BACKCOLOR(107);
}
