#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nutils.h"

namespace wiznet
{
  int nutils::sendmail(const char *to, const char *from, const char *subject, const char *message)
  //static int sendmail(std::string to, std::string from, std::string subject, std::string message)
  {
    int retval = -1;
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    if (mailpipe != NULL) {
      fprintf(mailpipe, "To: %s\n", to);
      fprintf(mailpipe, "From: %s\n", from);
      fprintf(mailpipe, "Subject: %s\n\n", subject);
      fwrite(message, 1, strlen(message), mailpipe);
      fwrite(".\n", 1, 2, mailpipe);
      pclose(mailpipe);
      retval = 0;
     }
     else {
       perror("Failed to invoke sendmail");
     }
     return retval;
  }
}
