#ifndef CORE__PARAMS_H
#define CORE__PARAMS_H

#include <string>
#include <deque>

namespace param
{
  class config
  {
  private:
    unsigned int intervalMins;
    unsigned int connectionTimeout;
    unsigned int notificationTimeout;
    unsigned int appid;
    unsigned int threshold;
    bool setArg(std::string &arg);

  public:
    config();
    ~config();
    unsigned int getIntervalMins();
    unsigned int getConnectionTimeout();
    unsigned int getNotificationTimeout();
    unsigned int getAppid();
    unsigned int getThreshold();
    bool setFromArgs(std::deque<std::string> &args);
  };
}

#endif
