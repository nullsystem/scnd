#ifndef CORE__PARAMS_H
#define CORE__PARAMS_H

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

  public:
    config();
    ~config();
    unsigned int getIntervalMins();
    unsigned int getConnectionTimeout();
    unsigned int getNotificationTimeout();
    unsigned int getAppid();
    unsigned int getThreshold();
  };
}

#endif
