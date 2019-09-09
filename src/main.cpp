/*
 *
 */
// https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid=244630

#include <iostream>
#include <chrono>
#include <thread>
#include <deque>

#include "wrapper/curl.h"
#include "wrapper/notify.h"
#include "tool/getPlayerCount.h"
#include "tool/args.h"
#include "core/help.h"
#include "core/params.h"

int main(int argc, char **argv)
{
  std::deque<std::string> args = tool::toArgs(argc, argv);
  args.pop_front();     // Program execution name not needed
  
  param::config config;

  unsigned int  currentCount = 0;
  bool          running  = false;
  bool          notify   = false;
  std::string   messageTitle;
  std::string   messageDetails;
  wrapper::curl curlJob;

  while (args.size() > 0)
  {
    if (args.front() == "-h")
    {
      std::cout << HELP;
    }
    else if (args.front() == "-v")
    {
      std::cout << VERSION;
    }
    else
    {
      std::cerr << "ERROR: Parameter '" << args.front() << "' not found.\n";
    }

    // Discard front string
    args.pop_front();
  }

  curlJob.setUrl("https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid="+std::to_string(config.getAppid()));
  curlJob.setTimeout(config.getConnectionTimeout());

  running = true;

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
        messageTitle = "NEOTOKYO";
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

