/* 
 * steamcountsnotifyd
 *
 * Steam counter notiifcation daemon
 * version Pre-Alpha 2019-09-11
 *
 * TODO:
 *  Threshold interval
 *  Threadding
 *  Daemonise
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <deque>
#include <vector>
#include <functional>

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
  bool          running = config.setFromArgs(args);

  // Functional lambda
  std::function<void(unsigned int, param::appidName_s, const param::config &)> appidRunningThread = [](unsigned int appid, param::appidName_s game, const param::config &config)
  {
    unsigned int  currentCount = 0;
    bool          notify   = false;
    bool          running  = true;
    std::string   messageTitle;
    std::string   messageDetails;
    wrapper::curl curlJob;
    unsigned int  currentThreadInterval = config.getIntervalMins();

    curlJob.setTimeout(config.getConnectionTimeout());

    while (running)
    {
      curlJob.setUrl("https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid="+std::to_string(appid));

      // Perform the job
      curlJob.perform();

      // Determine message
      if (curlJob.getHttpResponseCode() == 200)
      {
        currentCount = tool::getPlayerCount(curlJob.getHttpData());
        if (currentCount >= game.threshold)
        {
          notify = true;
          messageTitle = game.name;
          messageDetails = "Player counts: "+std::to_string(currentCount);
        }
      }
      else
      {
        messageTitle = "ERROR";
        messageDetails = "Cannot fetch player numbers - no internet connection or steam API is down.";
      }

      // Clear data
      curlJob.clearHttpData();

      // Notify
      if (notify)
      {
        wrapper::notify::init();
        wrapper::notify notifyJob(messageTitle, messageDetails);
        notifyJob.setTimeout(config.getNotificationTimeout());
        notifyJob.show();

        notify = false;
      }

      // Delay
      std::this_thread::sleep_for(std::chrono::minutes(currentThreadInterval));
    }
  };

  // If help or version message not used (normal execution)
  if (running)
  {
    // Initialise and run threads for each appid/game 
    std::vector<std::thread> appidThreadVector;

    for (const auto &[appid, game] : config.getAppidMap())
    {
      appidThreadVector.emplace_back(std::thread(appidRunningThread, appid, game, config));
    }

    // Join thread if joinable
    for (std::thread &thread : appidThreadVector)
    {
      if (thread.joinable())
      {
        thread.join();
      }
    }
  }

  return 0;
}

