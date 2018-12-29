#include "qtio.h"
#include <iostream>

namespace QtIO
{
  QTextStream in(stdin);
  QTextStream out(stdout);
  QTextStream err(stderr);
}
