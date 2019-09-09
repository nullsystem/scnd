#include "core/params.h"

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
