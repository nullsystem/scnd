#include "core/counterThread.h"

#include <thread>
#include <chrono>
#include <cstdint>
#include <iostream>

#include "tool/getPlayerCount.h"
#include "wrapper/curl.h"
#include "wrapper/notify.h"

// Called once clicked
// Requires GMainLoop
void gameCallback(NotifyNotification * /*notify*/, char * /*action*/, gpointer data)
{
  unsigned int appid = static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(data));
  std::string command = R"steamRun(steam steam://rungameid/)steamRun"+std::to_string(appid);
  system(command.c_str());
}

void cthread::run(unsigned int appid, param::appidName_s game, const param::config &config)
{
  unsigned int  currentCount = 0;
  bool          notify   = false;
  bool          running  = true;
  std::string   messageTitle;
  std::string   messageDetails;
  wrapper::curl curlJob;
  unsigned int  currentThreadInterval = config.getIntervalMins();

  curlJob.setTimeout(config.getConnectionTimeout());

  wrapper::notify notifyJob;
  notifyJob.setTimeout(config.getNotificationTimeout());
  notifyJob.addAction(config.getActionType(), "Launch game", gameCallback, reinterpret_cast<void *>(appid));

  while (running)
  {
    curlJob.setUrl("https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid="+std::to_string(appid));

    // Perform the job
    curlJob.perform();

    // Determine message
    if (curlJob.getHttpResponseCode() == 200)
    {
      currentCount = tool::getPlayerCount(curlJob.getHttpData());
      // If threshold met, then start notifying the user
      if (currentCount >= game.threshold)
      {
        notify = true;
        messageTitle = game.name;
        messageDetails = "Player count: "+std::to_string(currentCount);
        currentThreadInterval = config.getThresholdIntervalMins();
      }
      // Otherwise reset to the desired under threshold interval
      else
      {
        notify = false;
        currentThreadInterval = config.getIntervalMins();
      }
    }
    else
    {
      messageTitle = game.name+"(ERROR)";
      messageDetails = "Cannot fetch player numbers - no internet connection or steam API is down.";
    }

    // Clear data
    curlJob.clearHttpData();

    // Notify
    if (notify)
    {
      notifyJob.update(messageTitle, messageDetails);
      notifyJob.show();

      notify = false;
    }
    
    // Delay
    std::this_thread::sleep_for(std::chrono::minutes(currentThreadInterval));
  }
}

