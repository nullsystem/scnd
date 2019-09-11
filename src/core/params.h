#ifndef CORE__PARAMS_H
#define CORE__PARAMS_H

#include <string>
#include <string_view>
#include <deque>
#include <map>

namespace param
{
  struct appidName_s
  {
    std::string  name;
    unsigned int threshold;
  };

  using appidNameMap = std::map<unsigned int, appidName_s>;

  class config
  {
  private:
    unsigned int intervalMins;
    unsigned int thresholdIntervalMins;
    unsigned int connectionTimeout;
    unsigned int notificationTimeout;
    appidNameMap appidMap;
    bool         setArg(std::string &arg);
    void         readConfigurationFile(const std::string &confFilePath);

  public:
    config();
    ~config();
    unsigned int getIntervalMins();
    unsigned int getThresholdIntervalMins();
    unsigned int getConnectionTimeout();
    unsigned int getNotificationTimeout();
    appidNameMap getAppidMap();
    bool         setFromArgs(std::deque<std::string> &args);
  };
}

#endif
