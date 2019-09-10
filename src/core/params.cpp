#include "core/params.h"

#include "core/help.h"
#include "tool/homeDirectory.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <unordered_map>

// Reads configuration file
void param::config::readConfigurationFile(const std::string &confFilePath)
{
  std::unordered_map<std::string, unsigned int *> strMapToVal;
  strMapToVal["interval"] = &this->intervalMins;
  strMapToVal["thresholdinterval"] = &this->thresholdIntervalMins;
  strMapToVal["connectiontimeout"] = &this->connectionTimeout;
  strMapToVal["notificationtimeout"] = &this->notificationTimeout;

  std::ifstream confFile(confFilePath);
  std::string line;

  while (std::getline(confFile, line))
  {
    std::istringstream iss(line);
    std::string parameter;
    appidName_s valAN;
    int val;

    iss >> parameter;
    if (parameter == "newappid")
    {
      iss >> valAN.appid >> std::quoted(valAN.name) >> valAN.threshold;
      this->appidName.emplace_back(valAN);
    }
    else
    {
      iss >> val;
      *strMapToVal[parameter] = static_cast<unsigned int>(val);
    }
  }

  confFile.close();
}

param::config::config()
{
  // Set default parameters
  this->intervalMins = 1;
  this->thresholdIntervalMins = 2;
  this->connectionTimeout = 10;
  this->notificationTimeout = 10;
  this->appidName = {
      {244630, "NEOTOKYO", 0}
    , {282440, "Quake Live", 100}
  };
  this->appid = 244630;   // LEGACY
  this->threshold = 0;    // LEGACY

  // Read configuration file
  const std::string &confFilePath = tool::getHomeDirectory()+"/.config/steamcountsnotifyd/config";

  if (std::filesystem::exists(confFilePath))
  {
    this->readConfigurationFile(confFilePath);
  }
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
