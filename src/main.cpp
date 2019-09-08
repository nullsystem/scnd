/*
 *
 */
// https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid=244630

#include <iostream>
#include <chrono>
#include <thread>

#include "wrapper/curl.h"
#include "wrapper/notify.h"
#include "tool/getPlayerCount.h"

int main(int argc, char **argv)
{
  const unsigned int intervalMins = 1;
  const unsigned int notificationTimeout = 10;
  int running = 0;
  unsigned int appid = 244630;

  std::string messageTitle;
  std::string messageDetails;

  wrapper::curl curlJob("https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid="+std::to_string(appid));
  running = 1;

  while (running)
  {
    // Perform the job
    curlJob.perform();

    // Determine message
    if (curlJob.getHttpResponseCode() == 200)
    {
      messageTitle = "NEOTOKYO";
      messageDetails = "Player counts: "+std::to_string(tool::getPlayerCount(curlJob.getHttpData()));
    }
    else
    {
      messageTitle = "ERROR";
      messageDetails = "Cannot fetch player numbers - no internet connection or steam API is down.";
    }

    // Notify
    wrapper::notify::init();
    wrapper::notify notifyJob(messageTitle, messageDetails);
    notifyJob.setTimeout(notificationTimeout);
    notifyJob.show();

    // Clear data
    curlJob.clearHttpData();

    // Delay
    std::this_thread::sleep_for(std::chrono::minutes(intervalMins));
  }

  return 0;
}

