#ifndef WRAPPER__DAEMON_H
#define WRAPPER__DAEMON_H

#include <string>

namespace wrapper
{
  namespace daemon
  {
    void init();
    void uninit();
    void log(const std::string &str);
  }
}

#endif
