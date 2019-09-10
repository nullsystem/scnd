#include "core/params.h"

#include "core/help.h"
#include <iostream>
#include <sstream>

param::config::config()
{
  this->intervalMins = 1;
  this->connectionTimeout = 10;
  this->notificationTimeout = 10;
  this->appid = 244630;
  this->threshold = 0;
}

param::config::~config()
{
}

unsigned int param::config::getIntervalMins()
{
  return this->intervalMins;
}

unsigned int param::config::getConnectionTimeout()
{
  return this->connectionTimeout;
}

unsigned int param::config::getNotificationTimeout()
{
  return this->notificationTimeout;
}

unsigned int param::config::getAppid()
{
  return this->appid;
}

unsigned int param::config::getThreshold()
{
  return this->threshold;
}

bool param::config::setArg(std::string &arg)
{
  if (arg.size() <= 4)
  {
    std::cerr << "ERROR: Invalid parameter setting '" << arg << "'.\n";
    return false;
  }
  else
  {
    std::string temp;
    std::stringstream ss;
    int val;

    arg[2] = ' ';
    ss << arg;
    ss >> temp >> val;

    if (val < 0)
    {
      std::cerr << "ERROR: Value cannot be less than 0.\n";
      return false;
    }

    switch (arg[1])
    {
    case 'i':
      this->intervalMins = val;
      break;
    case 't':
      this->threshold = val;
      break;
    case 'a':
      this->appid = val;
      break;
    default:
      std::cerr << "ERROR: '" << arg[1] << "' not found.\n";
      return false;
    }

    return true;
  }
}

bool param::config::setFromArgs(std::deque<std::string> &args)
{
  std::string &argf = args.front();

  // Options handling
  while (args.size() > 0)
  {
    if (argf[0] == '-')
    {
      switch (argf[1])
      {
      case 'h':
        std::cout << HELP;
        return false;
      case 'v':
        std::cout << VERSION;
        return false;
      case 'i':
      case 't':
      case 'a':
        this->setArg(argf);
        break;
      default:
        std::cerr << "ERROR: '" << argf[1] << "' not found.\n";
      }
    }
    else
    {
      std::cerr << "ERROR: Parameter '" << argf << "' not found.\n";
    }

    // Discard front string
    args.pop_front();
  }

  return true;
}
