#include "tool/homeDirectory.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

std::string tool::getHomeDirectory()
{
  struct passwd *pw = getpwuid(getuid());
  return pw->pw_dir;
}

