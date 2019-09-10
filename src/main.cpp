/* 
 * steamcountsnotifyd
 *
 * Steam counter notiifcation daemon
 * version Pre-Alpha 2019-09-10
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <deque>

#include "wrapper/curl.h"
#include "wrapper/notify.h"
#include "tool/getPlayerCount.h"
#include "tool/args.h"
#include "core/params.h"

int main(int argc, char **argv)
{
  std::deque<std::string> args = tool::toArgs(argc, argv);
  args.pop_front();     // Program execution name not needed
  
  param::config config;
  unsigned int  currentCount = 0;
  bool          running  = true;
  bool          notify   = false;
  std::string   messageTitle;
  std::string   messageDetails;
  wrapper::curl curlJob;

  running = config.setFromArgs(args);

  curlJob.setUrl("https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid="+std::to_string(config.getAppid()));
  curlJob.setTimeout(config.getConnectionTimeout());

  while (running)
  {
    // Perform the job
    curlJob.perform();

    // Determine message
    if (curlJob.getHttpResponseCode() == 200)
    {
      currentCount = tool::getPlayerCount(curlJob.getHttpData());
      if (currentCount >= config.getThreshold())
      {
        notify = true;
        messageTitle = "Steam counts";
        messageDetails = "Player counts: "+std::to_string(currentCount);
      }
    }
    else
    {
      messageTitle = "ERROR";
      messageDetails = "Cannot fetch player numbers - no internet connection or steam API is down.";
    }

    // Notify
    if (notify)
    {
      wrapper::notify::init();
      wrapper::notify notifyJob(messageTitle, messageDetails);
      notifyJob.setTimeout(config.getNotificationTimeout());
      notifyJob.show();

      notify = false;
    }

    // Clear data
    curlJob.clearHttpData();

    // Delay
    std::this_thread::sleep_for(std::chrono::minutes(config.getIntervalMins()));
  }

  return 0;
}

