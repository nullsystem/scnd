#include "core/params.h"

#include "core/help.h"
#include "tool/homeDirectory.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <unordered_map>

// Reads configuration file
void param::config::readConfigurationFile(const std::string &confFilePath)
{
  std::unordered_map<std::string, unsigned int> strMapToVal;
  // Set default parameters
  strMapToVal["interval"] = 1;
  strMapToVal["thresholdinterval"] = 2;
  strMapToVal["connectiontimeout"] = 10;
  strMapToVal["notificationtimeout"] = 10;
  this->appidMap[244630] = {"NEOTOKYO", 0};
  this->appidMap[282440] = {"Quake Live", 100};

  // Configuration file stream
  std::ifstream confFile(confFilePath);

  for (std::string line; std::getline(confFile, line); )
  {
    std::istringstream iss(line);
    std::string parameter;
    appidName_s valAN;
    int val;

    iss >> parameter;
    if (parameter == "newappid")
    {
      iss >> val >> std::quoted(valAN.name) >> valAN.threshold;
      this->appidMap[val] = valAN;
    }
    else
    {
      iss >> val;
      if (val >= 0)
      {
        strMapToVal[parameter] = static_cast<unsigned int>(val);
      }
      else
      {
        std::cerr << "ERROR: Value for '" << parameter << "' cannot be less than 0.\n";
      }
    }
  }

  confFile.close();

  this->intervalMins = strMapToVal["interval"];
  this->thresholdIntervalMins = strMapToVal["thresholdinterval"];
  this->connectionTimeout = strMapToVal["connectiontimeout"];
  this->notificationTimeout = strMapToVal["notificationtimeout"];
}

param::config::config()
{
  // Read configuration file
  const std::string &confFilePath = tool::getHomeDirectory()+"/.config/steamcountsnotifyd/config";

  if (std::filesystem::exists(confFilePath))
  {
    this->readConfigurationFile(confFilePath);
  }

  /* VERBOSE
  for (auto &an : this->appidMap)
  {
    std::cout << an.first << ' ' << an.second.name << ' ' << an.second.threshold << '\n';
  }
  */
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

param::appidNameMap param::config::getAppidMap()
{
  return this->appidMap;
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
      this->thresholdIntervalMins = val;
      break;
    case 'c':
      this->connectionTimeout = val;
      break;
    case 'n':
      this->notificationTimeout = val;
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
