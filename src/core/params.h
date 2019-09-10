#ifndef CORE__PARAMS_H
#define CORE__PARAMS_H

#include <string>
#include <string_view>
#include <deque>
#include <vector>

namespace param
{
  struct appidName_s
  {
    unsigned int appid;
    std::string  name;
    unsigned int threshold;
  };

  using appidNameList = std::vector<appidName_s>;

  class config
  {
  private:
    unsigned int  intervalMins;
    unsigned int  thresholdIntervalMins;
    unsigned int  connectionTimeout;
    unsigned int  notificationTimeout;
    unsigned int  appid;                   // LEGACY
    unsigned int  threshold;               // LEGACY
    appidNameList appidName;
    bool          setArg(std::string &arg);
    void          readConfigurationFile(const std::string &confFilePath);

  public:
    config();
    ~config();
    unsigned int  getIntervalMins();
    unsigned int  getThresholdIntervalMins();
    unsigned int  getConnectionTimeout();
    unsigned int  getNotificationTimeout();
    unsigned int  getAppid();
    unsigned int  getThreshold();
    bool          setFromArgs(std::deque<std::string> &args);
  };
}

#endif
