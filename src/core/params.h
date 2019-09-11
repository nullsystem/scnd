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
    unsigned int getIntervalMins() const;
    unsigned int getThresholdIntervalMins() const;
    unsigned int getConnectionTimeout() const;
    unsigned int getNotificationTimeout() const;
    appidNameMap getAppidMap() const;
    bool         setFromArgs(std::deque<std::string> &args);
  };
}

#endif
