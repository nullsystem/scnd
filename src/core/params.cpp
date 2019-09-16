#include "core/params.h"

#include "core/help.h"
#include "tool/homeDirectory.h"
#include "wrapper/daemon.h"

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
  strMapToVal["interval"] = 1;                    // Minutes
  strMapToVal["thresholdinterval"] = 2;           // Minutes
  strMapToVal["connectiontimeout"] = 10;          // Seconds
  strMapToVal["notificationtimeout"] = 10;        // Seconds
  strMapToVal["actiontype"] = 0;                  // 0 = default (default), 1 = clicked
  strMapToVal["daemonize"] = 1;                   // 0 = non-daemonize, 1 = daemonize (default)
  this->daemonize = true;

  wrapper::daemon::log(confFilePath);

  // Go over the configuration file
  if (std::filesystem::exists(confFilePath))
  {
    wrapper::daemon::log("Found configuration file.");

    // Configuration file stream
    std::ifstream confFile(confFilePath);

    for (std::string line; std::getline(confFile, line); )
    {
      std::istringstream iss(line);
      std::string parameter;
      appidName_s valAN;
      int val;
      std::string valStr;

      iss >> parameter;
      if (parameter == "newappid")
      {
        iss >> val >> std::quoted(valAN.name) >> valAN.threshold;
        this->appidMap[val] = valAN;
      }
      else if (parameter == "daemonize")
      {
        iss >> valStr;
        this->daemonize = (valStr == "false") ? false : true;
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
  }
  else
  {
    wrapper::daemon::log("Cannot find configuration file.");
  }

  // Set the configuration file's settings to the program
  this->intervalMins = strMapToVal["interval"];
  this->thresholdIntervalMins = strMapToVal["thresholdinterval"];
  this->connectionTimeout = strMapToVal["connectiontimeout"];
  this->notificationTimeout = strMapToVal["notificationtimeout"];
  this->actionType = strMapToVal["actiontype"];
  this->daemonize = strMapToVal["daemonize"];
}

param::config::config()
{
  // Read configuration file
  this->readConfigurationFile(tool::getHomeDirectory()+"/.config/steamcountsnotifyd/config");
}

param::config::~config()
{
}

bool param::config::getDaemonize() const
{
  return this->daemonize;
}

unsigned int param::config::getIntervalMins() const
{
  return this->intervalMins;
}

unsigned int param::config::getThresholdIntervalMins() const
{
  return this->thresholdIntervalMins;
}

unsigned int param::config::getConnectionTimeout() const
{
  return this->connectionTimeout;
}

unsigned int param::config::getNotificationTimeout() const
{
  return this->notificationTimeout;
}

std::string_view param::config::getActionType() const
{
  switch (this->actionType)
  {
  case 0:
    return "default";
  case 1:
    return "clicked";
  default:
    return "default";
  }
}

param::appidNameMap param::config::getAppidMap() const
{
  return this->appidMap;
}

bool param::config::setArg(std::string &arg)
{
  if (arg.size() < 4)
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
    case 'd':
      this->daemonize = static_cast<bool>(val);
      break;
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
    case 'a':
      this->actionType = val;
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
      default:
        this->setArg(argf);
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
