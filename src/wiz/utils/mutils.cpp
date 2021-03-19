#include <cmath>
#include "mutils.h"

namespace wiznet
{
  double mutils::setprecision(double x, int prec) {
    /*return
      ceil( x * pow(10,(double)prec) - .4999999999999)
      / pow(10,(double)prec);*/


    double power = 1.0;
    int i;

    if (prec > 0)
      for (i = 0; i < prec; i++)
        power *= 10.0;
    else if (prec < 0)
      for (i = 0; i < prec; i++)
        power /= 10.0;

    if (x > 0)
      x = floor(x * power + 0.5) / power;
    else if (x < 0)
      x = ceil(x * power - 0.5) / power;

    if (x == -0)
      x = 0;

    return x;
  }
}
