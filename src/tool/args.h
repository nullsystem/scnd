#ifndef TOOL__ARGS_H
#define TOOL__ARGS_H

#include <string>
#include <deque>

namespace tool
{
  std::deque<std::string> toArgs(int argc, char **argv);
}

#endif
