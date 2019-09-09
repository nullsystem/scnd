#include "tool/args.h"

std::deque<std::string> tool::toArgs(int argc, char **argv)
{
  std::deque<std::string> retDequeStrings;

  for (int i=0; i < argc; ++i)
  {
    retDequeStrings.emplace_back(argv[i]);
  }

  return retDequeStrings;
}

